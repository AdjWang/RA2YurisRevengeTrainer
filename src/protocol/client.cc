#include "protocol/client.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "gsl/util"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace yrtr {

#define BIND_BUTTON(fnname)                                             \
  gui.AddButtonListener(FnLabel::k##fnname, [this]() {                  \
    thread_pool_.enqueue([this]() { SendButton(FnLabel::k##fnname); }); \
  });
#define BIND_CHECKBOX(fnname)                                                \
  gui.AddCheckboxListener(FnLabel::k##fnname, [this](bool activate) {        \
    thread_pool_.enqueue(                                                    \
        [this, activate]() { SendCheckbox(FnLabel::k##fnname, activate); }); \
  });

Client::Client(frontend::Gui& gui)
    : cli_("http://localhost:35271"),
      thread_pool_(1),
      get_state_count_(0) {
  thread_pool_.enqueue([&]() { SetupNetThreadOnce(); });
  render_loop_ch_.SetThreadId(GetRendererThreadId());

  gui.AddInputListener(FnLabel::kApply, [this](uint32_t val) {
    thread_pool_.enqueue([this, val]() { SendInput(FnLabel::kApply, val); });
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
#undef BIND_BUTTON
#undef BIND_CHECKBOX

Client::~Client() {
  thread_pool_.shutdown();
}

void Client::UpdateState() {
  DCHECK(yrtr::IsWithinRendererThread());
  thread_pool_.enqueue([&]() { GetState(); });
  render_loop_ch_.ExecuteTasks();
}

void Client::GetState() {
  DCHECK(yrtr::IsWithinNetThread());
  if (get_state_count_.load(std::memory_order_relaxed) >= kMaxGetState) {
    return;
  }
  get_state_count_.fetch_add(1, std::memory_order_relaxed);
  auto _ = gsl::finally(
      [&]() { get_state_count_.fetch_sub(1, std::memory_order_relaxed); });
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
  UNREFERENCED_PARAMETER(data);
  // TODO
}

void Client::SendInput(FnLabel label, uint32_t val) {
  DCHECK(yrtr::IsWithinNetThread());
  json data;
  data.emplace("type", "input");
  data.emplace("label", static_cast<int>(label));
  data.emplace("val", val);
  PostData(kApiPostEvent, data.dump());
}

void Client::SendButton(FnLabel label) {
  DCHECK(yrtr::IsWithinNetThread());
  json data;
  data.emplace("type", "button");
  data.emplace("label", static_cast<int>(label));
  PostData(kApiPostEvent, data.dump());
}

void Client::SendCheckbox(FnLabel label, bool activate) {
  DCHECK(yrtr::IsWithinNetThread());
  json data;
  data.emplace("type", "checkbox");
  data.emplace("label", static_cast<int>(label));
  data.emplace("val", activate);
  PostData(kApiPostEvent, data.dump());
}

void Client::PostData(std::string_view path, std::string&& data) {
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
