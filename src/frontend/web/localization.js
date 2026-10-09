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
        ClearAll: "删除全部",
        FilterHelp: "在游戏中选中己方单位，再将其阵营加入保护阵营。阵营相关功能仅对保护阵营生效。",
        MoneyHelp: "向保护阵营增加金钱。请先在过滤页面添加己方阵营。",
        MoneyPlaceholder: "输入金额"
      },
      en: {
        Title: "Yuri's Revenge Trainer",
        State: "State",
        StateOk: "Game running",
        StateIdle: "Game not running",
        Money: "Cash to add",
        Assist: "Cheats",
        Filter: "Filter",
        SelectingHouseList: "Selected factions",
        ProtectedHouseList: "Protected factions",
        AddAll: "Add all selected",
        ClearAll: "Clear protected",
        FilterHelp: "Select a unit in-game, then add its faction here. Faction cheats affect protected factions only.",
        MoneyHelp: "Adds cash to protected factions. Protect yours in Filter first.",
        MoneyPlaceholder: "Amount"
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
        SelectEnemy: "多选敌方单位",
        PauseGame: "暂停游戏",
        AdjustGameSpeed: "任务调速"
      },
      en: {
        Apply: "Add cash",
        FastBuild: "Fast build",
        DeleteUnit: "Delete selected units",
        ClearShroud: "Reveal map",
        GiveMeABomb: "Get one nuke",
        UnitLevelUp: "Make selected elite",
        UnitSpeedUp: "Speed up selected",
        IAMWinner: "Win now",
        ThisIsMine: "Capture selected units",
        God: "Invulnerable",
        InstBuild: "Instant build",
        UnlimitSuperWeapon: "No superweapon cooldown",
        InstFire: "Maximum fire rate",
        InstTurn: "Instant rotation",
        RangeToYourBase: "Maximum attack range",
        FireToYourBase: "Maximum guard range",
        FreezeGapGenerator: "Disable gap generators",
        SellTheWorld: "Sell any unit/building",
        BuildEveryWhere: "Build everywhere",
        AutoRepair: "Automatic repairs",
        SocialismMajesty: "Anti mind control",
        MakeCapturedMine: "Claim captures",
        MakeGarrisonedMine: "Claim garrisons",
        InvadeMode: "Auto-attack buildings",
        UnlimitTech: "Unlock all tech",
        UnlimitFirePower: "Fast reload ammo",
        InstChrono: "No chrono cooldown",
        SpySpy: "Enemy spies grant tech",
        SelectEnemy: "Multi-select enemy units",
        PauseGame: "Pause game",
        AdjustGameSpeed: "Game speed"
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
