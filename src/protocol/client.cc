#include "protocol/client.h"

#include <chrono>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "gsl/util"

namespace yrtr {

#define BIND_BUTTON(fnname)                                                 \
  gui.AddButtonListener(FnLabel::k##fnname, [this]() {                      \
    thread_pool_.enqueue([this]() { SendPostButton(FnLabel::k##fnname); }); \
  });
#define BIND_CHECKBOX(fnname)                                         \
  gui.AddCheckboxListener(FnLabel::k##fnname, [this](bool activate) { \
    thread_pool_.enqueue([this, activate]() {                         \
      SendPostCheckbox(FnLabel::k##fnname, activate);                 \
    });                                                               \
  });

Client::Client(frontend::Gui& gui, uint16_t port)
    : gui_(gui),
      cli_(std::format("http://localhost:{}", port)),
      thread_pool_(/*n*/ 1),
      get_state_count_(0) {
  // This tool is used in LAN, 50ms should be enough. Large timeout queues too
  // many jobs in sending queue when the backend is not running, making the gui
  // actions, like close window, acts lagging.
  cli_.set_connection_timeout(std::chrono::milliseconds(50));
  thread_pool_.enqueue([&]() { SetupNetThreadOnce(); });
  render_loop_ch_.SetThreadId(GetRendererThreadId());

  gui.AddHouseListListener([this](SideMap&& side_map) {
    thread_pool_.enqueue([this, side_map = std::move(side_map)]() mutable {
      SendPostProtectedList(std::move(side_map));
    });
  });
  gui.AddInputListener(FnLabel::kApply, [this](uint32_t val) {
    thread_pool_.enqueue(
        [this, val]() { SendPostInput(FnLabel::kApply, val); });
  });
  BIND_BUTTON(FastBuild);
  BIND_BUTTON(DeleteUnit);
  BIND_BUTTON(ClearShroud);
  BIND_BUTTON(GiveMeABomb);
  BIND_BUTTON(UnitLevelUp);
  BIND_BUTTON(UnitSpeedUp);
  BIND_BUTTON(IAMWinner);
  BIND_BUTTON(ThisIsMine);
  BIND_CHECKBOX(God);
  BIND_CHECKBOX(InstBuild);
  BIND_CHECKBOX(UnlimitSuperWeapon);
  BIND_CHECKBOX(InstFire);
  BIND_CHECKBOX(InstTurn);
  BIND_CHECKBOX(RangeToYourBase);
  BIND_CHECKBOX(FireToYourBase);
  BIND_CHECKBOX(FreezeGapGenerator);
  BIND_CHECKBOX(SellTheWorld);
  BIND_CHECKBOX(BuildEveryWhere);
  BIND_CHECKBOX(AutoRepair);
  BIND_CHECKBOX(MakeGarrisonedMine);
  BIND_CHECKBOX(InvadeMode);
  BIND_CHECKBOX(UnlimitTech);
  BIND_CHECKBOX(UnlimitFirePower);
  BIND_CHECKBOX(InstChrono);
  BIND_CHECKBOX(SpySpy);
  BIND_CHECKBOX(AdjustGameSpeed);
}

#undef BIND_BUTTON
#undef BIND_CHECKBOX

Client::~Client() {}

void Client::Stop() {
  cli_.stop();
  thread_pool_.shutdown();
}

void Client::Update() {
  DCHECK(yrtr::IsWithinRendererThread());
  render_loop_ch_.ExecuteTasks();
}

void Client::GetState() {
  DCHECK(yrtr::IsWithinRendererThread());
  thread_pool_.enqueue([this]() { SendGetState(); });
}

void Client::SendGetState() {
  DCHECK(yrtr::IsWithinNetThread());
  auto _ = gsl::finally([&]() { get_state_count_.fetch_sub(1); });
  if (get_state_count_.fetch_add(1) + 1 > kMaxGetState) {
    return;
  }
  httplib::Result res = cli_.Get(kApiGetState.data());
  if (res) {
    if (res->status == 200) {
      ParseState(res->body);
    } else {
      LOG_F(INFO, "Failed to get state with code={}", res->status);
    }
  } else {
    LOG_F(ERROR, "Failed to get state with error={}",
          httplib::to_string(res.error()));
  }
}

void Client::ParseState(const std::string& data) {
  DCHECK(yrtr::IsWithinNetThread());
  render_loop_ch_.ExecuteOrScheduleTask([this, data = data]() {
    DCHECK(yrtr::IsWithinRendererThread());
    State state = json::parse(data);
    gui_.UpdateState(state);
  });
}

void Client::SendPostInput(FnLabel label, uint32_t val) {
  DCHECK(yrtr::IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeInputEvent(label, val));
}

void Client::SendPostButton(FnLabel label) {
  DCHECK(yrtr::IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeButtonEvent(label));
}

void Client::SendPostCheckbox(FnLabel label, bool activate) {
  DCHECK(yrtr::IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeCheckboxEvent(label, activate));
}

void Client::SendPostProtectedList(SideMap&& side_map) {
  DCHECK(yrtr::IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeProtectedListEvent(std::move(side_map)));
}

void Client::SendPostData(std::string_view path, std::string&& data) {
  httplib::Result res = cli_.Post(path.data(), data, "application/json");
  if (res) {
    if (res->status != 200) {
      LOG_F(INFO, "Failed to post event with code={}", res->status);
    }
  } else {
    LOG_F(ERROR, "Failed to post event with error={}",
          httplib::to_string(res.error()));
  }
}

}  // namespace yrtr
