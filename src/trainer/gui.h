#pragma once
#include "common/vendor.h"
#include "trainer/vendor.h"
#include "trainer/char_table.h"

namespace yrtr {

class ImGuiContext {
public:
    ImGuiContext(GLFWwindow* window);
    ~ImGuiContext();

    void Update() {}
    void PreRender();
    void Render() const;

    void RenderClientArea();
    void AddCheckboxListener(FnLabel label, std::function<void(bool)> cb);

private:
    float hdpi_scale_factor_;

    bool ckbox_God_;
    bool ckbox_InstBuild_;
    bool ckbox_UnlimitSuperWeapon_;
    bool ckbox_UnlimitRadar_;
    bool ckbox_InstShot_;
    bool ckbox_InstTurn_;
    bool ckbox_RangeToYourBase_;
    bool ckbox_FireToYourBase_;
    bool ckbox_FreezeGapGenerator_;
    bool ckbox_FreezeUnit_;
    bool ckbox_SellTheWorld_;
    bool ckbox_UnlimitPower_;
    bool ckbox_BuildEveryWhere_;
    bool ckbox_AutoRepaire_;
    bool ckbox_EnermyRepaireDown_;
    bool ckbox_SocialismTheBest_;
    bool ckbox_MakeAttackedMine_;
    bool ckbox_MakeOccupiedMine_;
    bool ckbox_MakeGarrisonedMine_;
    bool ckbox_InvadeMode_;
    bool ckbox_UnlimitTech_;
    bool ckbox_InstLoad_;
    bool ckbox_UnlimitFirePower_;
    bool ckbox_InstChrono_;
    bool ckbox_SpySpy_;
    bool ckbox_InfantrySlip_;
    bool ckbox_UnitLeveledUp_;

    std::unordered_map<FnLabel, std::function<void()>> btn_cbs_;
    std::unordered_map<FnLabel, std::function<void(bool)>> ckbox_cbs_;

    const char* GetFnChar(FnLabel label);
};

}  // namespace yrtr
