export class Localization {
  constructor() {
    this.setLanguage(navigator.language);
    this.guiLabels = {
      zh: {
        Title: "辅助工具",
        State: "状态",
        StateOk: "游戏运行中",
        StateIdle: "未检测到游戏",
        Money: "钱",
        Assist: "功能",
        Filter: "过滤",
        SelectingHouseList: "选中阵营",
        ProtectedHouseList: "保护阵营",
        AddAll: "添加全部",
        ClearAll: "删除全部"
      },
      en: {
        Title: "Assist Tool",
        State: "State",
        StateOk: "Game running",
        StateIdle: "Game not running",
        Money: "Money",
        Assist: "Assist",
        Filter: "Filter",
        SelectingHouseList: "Selecting house",
        ProtectedHouseList: "Protected house",
        AddAll: "Add all",
        ClearAll: "Clear all"
      }
    };

    this.fnLabels = {
      zh: {
        Apply: "修改",
        FastBuild: "快速建造",
        DeleteUnit: "删除单位",
        ClearShroud: "地图全开",
        GiveMeABomb: "核弹攻击",
        UnitLevelUp: "单位升级",
        UnitSpeedUp: "单位加速",
        IAMWinner: "立即胜利",
        ThisIsMine: "这是我的",
        God: "无敌",
        InstBuild: "瞬间建造",
        UnlimitSuperWeapon: "无限超武",
        InstFire: "极速攻击",
        InstTurn: "极速转身",
        RangeToYourBase: "远程攻击",
        FireToYourBase: "远程警戒",
        FreezeGapGenerator: "瘫痪裂缝产生器",
        SellTheWorld: "卖卖卖",
        BuildEveryWhere: "随意建筑",
        AutoRepair: "自动修理",
        SocialismMajesty: "社会主义万岁",
        MakeCapturedMine: "全是我的-工程师占领",
        MakeGarrisonedMine: "全是我的-房屋驻军",
        InvadeMode: "侵略模式",
        UnlimitTech: "全科技",
        UnlimitFirePower: "大量弹药-重新建造生效",
        InstChrono: "瞬间超时空",
        SpySpy: "无间道",
        AdjustGameSpeed: "任务调速",
        SelectEnemy: "多选敌方单位"
      },
      en: {
        Apply: "Apply",
        FastBuild: "Fast build",
        DeleteUnit: "Delete unit",
        ClearShroud: "Clear shroud",
        GiveMeABomb: "Give me a bomb",
        UnitLevelUp: "Selected units level up",
        UnitSpeedUp: "Selected units speed up",
        IAMWinner: "I am winner",
        ThisIsMine: "This is mine",
        God: "I am god",
        InstBuild: "Instant build",
        UnlimitSuperWeapon: "No super weapon cd",
        InstFire: "No fire cd",
        InstTurn: "Instant turn",
        RangeToYourBase: "Range to your base",
        FireToYourBase: "Fire to your base",
        FreezeGapGenerator: "Disable gap generator",
        SellTheWorld: "Sell the world",
        BuildEveryWhere: "Build everywhere",
        AutoRepair: "Auto repair",
        SocialismMajesty: "Socialism majesty",
        MakeCapturedMine: "Make captured mine",
        MakeGarrisonedMine: "Make garrisoned mine",
        InvadeMode: "Invade mode",
        UnlimitTech: "Unlimit technology",
        UnlimitFirePower: "Unlimit gun power",
        InstChrono: "Instant chrono",
        SpySpy: "Spy spy",
        AdjustGameSpeed: "Enable game speed adjustment",
        SelectEnemy: "Enable enemy selecting"
      }
    };
  }

  setLanguage(lang) {
    if (lang === 'zh-CN') {
      this.lang = 'zh';
    } else {
      this.lang = 'en';
    }
  }

  getGuiStr(label) {
    return this.guiLabels[this.lang][label] || label;
  }

  getFnStr(label) {
    return this.fnLabels[this.lang][label] || label;
  }
}
