#include "tool/assist/assist.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "imgui.h"
#include "tool/assist/gui.h"
#include "tool/assist/version.h"

namespace {
static bool menu_active = false;
static absl::Mutex reclaim_mu;
}

extern "C" {
const char* SToolVersion() { return ASSIST_VER_STR; }

void SToolSetMenuActive(bool active) {
  DCHECK(yrtr::IsWithinRendererThread());
  menu_active = active;
}

void* SToolInit(const yrtr::ToolConfig& cfg) {
  yrtr::logging::InitLogging(yrtr::logging::LogSink::kDbgView);
  yrtr::SetupGameLoopThreadOnce(cfg.tid_game_loop);
  yrtr::SetupRendererThreadOnce(cfg.tid_render_loop);
  ImGui::SetAllocatorFunctions(
      reinterpret_cast<ImGuiMemAllocFunc>(cfg.fn_alloc),
      reinterpret_cast<ImGuiMemFreeFunc>(cfg.fn_free), cfg.user_data);
  return new assist::Assist(cfg);
}

void SToolDestroy(void* tool) {
  if (yrtr::IsWithinRendererThread()) {
    // After destroy, the module is going to be freed, not release lock to
    // prevent re-enter another game loop.
    reclaim_mu.Lock();
  }
  delete tool;
}

void SToolSetActive(void* tool, bool active) {
  DCHECK(yrtr::IsWithinRendererThread());
  reinterpret_cast<assist::Assist*>(tool)->SetActive(active);
}

void SToolUpdate(void* tool, double delta) {
  DCHECK(yrtr::IsWithinGameLoopThread());
  absl::MutexLock lk(&reclaim_mu);
  reinterpret_cast<assist::Assist*>(tool)->Update(delta);
}

void SToolRender(void* tool, void* ctx) {
  DCHECK(yrtr::IsWithinRendererThread());
  reinterpret_cast<assist::Assist*>(tool)->Render(ctx);
}
}

namespace assist {

#define BIND_BUTTON(fnname)                                                   \
  gui_.AddButtonListener(FnLabel::k##fnname, [this]() {                       \
    game_loop_ch_.ExecuteOrScheduleTask([this]() { game_.OnBtn##fnname(); }); \
  });
#define BIND_CHECKBOX(fnname)                                          \
  gui_.AddCheckboxListener(FnLabel::k##fnname, [this](bool activate) { \
    game_loop_ch_.ExecuteOrScheduleTask(                               \
        [this, activate]() { game_.OnCkbox##fnname(activate); });      \
  });

Assist::Assist(const yrtr::ToolConfig& cfg)
    : cfg_(cfg),
      gui_(cfg, state_),
      game_(state_) {
  game_loop_ch_.SetThreadId(cfg.tid_game_loop);
  render_loop_ch_.SetThreadId(cfg.tid_render_loop);

  if (cfg.rtenv == yrtr::RuntimeEnv::kTest) {
    state_.selecting_houses.emplace(1, SideDesc{.uniq_id = 1});
    state_.selecting_houses.emplace(2, SideDesc{.uniq_id = 2});
    state_.selecting_houses.emplace(3, SideDesc{.uniq_id = 3});
    state_.selecting_houses.emplace(4, SideDesc{.uniq_id = 4});
    state_.selecting_houses.emplace(5, SideDesc{.uniq_id = 5});
    state_.selecting_houses.emplace(6, SideDesc{.uniq_id = 6});
    state_.selecting_houses.emplace(7, SideDesc{.uniq_id = 7});
    state_.selecting_houses.emplace(8, SideDesc{.uniq_id = 8});
    state_.selecting_houses.emplace(9, SideDesc{.uniq_id = 9});
    state_.selecting_houses.emplace(10, SideDesc{.uniq_id = 10});
  } else {
    gui_.AddInputListener(FnLabel::kApply, [this](uint32_t val) {
      game_loop_ch_.ExecuteOrScheduleTask(
          [this, val]() { game_.OnInputCredit(val); });
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
}

#undef BIND_BUTTON
#undef BIND_BUTTON
#undef BIND_CHECKBOX

void Assist::SetActive(bool /*active*/) {}

void Assist::Update(double delta) {
  if (cfg_.rtenv == yrtr::RuntimeEnv::kGame) {
    game_.Update(delta);
  }
  game_loop_ch_.ExecuteTasks();
}

void Assist::Render(void* ctx) {
  gui_.Render(ctx);
  render_loop_ch_.ExecuteTasks();
}

}  // namespace assist
