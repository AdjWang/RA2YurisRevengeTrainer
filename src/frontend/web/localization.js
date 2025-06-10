export class Localization {
  constructor() {
    this.setLanguage(navigator.language);
    this.guiLabels = {
      zh: {
        kTitle: "辅助工具",
        kState: "状态",
        kStateOk: "游戏运行中",
        kStateIdle: "未检测到游戏",
        kMoney: "钱",
        kAssist: "功能",
        kFilter: "过滤",
        kSelectingHouseList: "选中阵营",
        kProtectedHouseList: "保护阵营",
        kAddAll: "添加全部",
        kClearAll: "删除全部"
      },
      en: {
        kTitle: "Assist Tool",
        kState: "State",
        kStateOk: "Game running",
        kStateIdle: "Game not running",
        kMoney: "Money",
        kAssist: "Assist",
        kFilter: "Filter",
        kSelectingHouseList: "Selecting house",
        kProtectedHouseList: "Protected house",
        kAddAll: "Add all",
        kClearAll: "Clear all"
      }
    };

    this.fnLabels = {
      zh: {
        kApply: "修改",
        kFastBuild: "快速建造",
        kDeleteUnit: "删除单位",
        kClearShroud: "地图全开",
        kGiveMeABomb: "核弹攻击",
        kUnitLevelUp: "单位升级",
        kUnitSpeedUp: "单位加速",
        kIAMWinner: "立即胜利",
        kThisIsMine: "这是我的",
        kGod: "无敌",
        kInstBuild: "瞬间建造",
        kUnlimitSuperWeapon: "无限超武",
        kInstFire: "极速攻击",
        kInstTurn: "极速转身",
        kRangeToYourBase: "远程攻击",
        kFireToYourBase: "远程警戒",
        kFreezeGapGenerator: "瘫痪裂缝产生器",
        kSellTheWorld: "卖卖卖",
        kBuildEveryWhere: "随意建筑",
        kAutoRepair: "自动修理",
        kSocialismMajesty: "社会主义万岁",
        kMakeGarrisonedMine: "全是我的-房屋驻军",
        kInvadeMode: "侵略模式",
        kUnlimitTech: "全科技",
        kUnlimitFirePower: "大量弹药-重新建造生效",
        kInstChrono: "瞬间超时空",
        kSpySpy: "无间道",
        kAdjustGameSpeed: "任务调速"
      },
      en: {
        kApply: "Apply",
        kFastBuild: "Fast build",
        kDeleteUnit: "Delete unit",
        kClearShroud: "Clear shroud",
        kGiveMeABomb: "Give me a bomb",
        kUnitLevelUp: "Selected units level up",
        kUnitSpeedUp: "Selected units speed up",
        kIAMWinner: "I am winner",
        kThisIsMine: "This is mine",
        kGod: "I am god",
        kInstBuild: "Instant build",
        kUnlimitSuperWeapon: "No super weapon cd",
        kInstFire: "No fire cd",
        kInstTurn: "Instant turn",
        kRangeToYourBase: "Range to your base",
        kFireToYourBase: "Fire to your base",
        kFreezeGapGenerator: "Disable gap generator",
        kSellTheWorld: "Sell the world",
        kBuildEveryWhere: "Build everywhere",
        kAutoRepair: "Auto repair",
        kSocialismMajesty: "Socialism majesty",
        kMakeGarrisonedMine: "Make garrisoned mine",
        kInvadeMode: "Invade mode",
        kUnlimitTech: "Unlimit technology",
        kUnlimitFirePower: "Unlimit gun power",
        kInstChrono: "Instant chrono",
        kSpySpy: "Spy spy",
        kAdjustGameSpeed: "Enable game speed adjustment"
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
