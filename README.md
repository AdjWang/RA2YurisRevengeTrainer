# RA2YurisRevengeTrainer

红警2尤里的复仇内存修改器, 适用于 尤里的复仇 1.001 原版及 Ares 版本。

## 编译

使用 [CMake](https://cmake.org/), python3, npm, Ninja 和 Visual Studio 17 2022 构建。

### 编译前端

```
git clone https://github.com/AdjWang/RA2YurisRevengeTrainer.git
cd ./RA2YurisRevengeTrainer/src/frontend/web
npm install
npm run build
cd ../../..
python scripts/generate_web_main_page.py
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_INSTALL_PREFIX=deps/out/Release -G Ninja -S . -B ./build
cmake --build ./build --config Release --target ra2_trainer -j $env:NUMBER_OF_PROCESSORS
```

如果不想编译网页，只需要桌面程序：

```
git clone https://github.com/AdjWang/RA2YurisRevengeTrainer.git
cd ./RA2YurisRevengeTrainer
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_INSTALL_PREFIX=deps/out/Release -DYRTR_ENABLE_WEB=OFF -G Ninja -S . -B ./build
cmake --build ./build --config Release --target ra2_trainer -j $env:NUMBER_OF_PROCESSORS
```

### 编译后端

```
cmake --build ./build --config Release --target ra2_trainer_backend -j $env:NUMBER_OF_PROCESSORS
```

## 使用方式

dll 注入比远程线程安全，程序也更好写，但是代价是注入流程更加复杂。没有注入 Ares 需求的玩家可以选择继续使用[之前的 v4 版本](https://github.com/AdjWang/RA2YurisRevengeTrainer/releases/tag/v4.2)。

### 自动注入后端模块

Ares：将 `ra2_trainer_backend.dll` 和 `ra2_trainer_backend.toml` 放到游戏目录。

原版：将 `wsock32.dll, ra2_trainer_backend.dll` 和 `ra2_trainer_backend.toml` 放到游戏目录。

### 手动注入后端模块

对于某些原版，`wsock32.dll` 会导致游戏启动时崩溃。对于某些 Ares 版本，如 `Tiberium Crisis 2` 和复仇时刻，不会自动识别可注入模块。此类版本要手动注入，使用任意 dll 注入工具注入 `ra2_trainer_backend.dll` 即可，注意仍需将 `ra2_trainer_backend.dll` 和 `ra2_trainer_backend.toml` 放在游戏目录下。

例如 [CheatEngine 自带的 dll 注入功能 (Inject DLL)](https://wiki.cheatengine.org/index.php?title=Help_File:Menus_and_Features)。

### 桌面端

打开 `ra2_trainer.exe` 即可，目录下需要有 `ra2_trainer.toml`.

### 网页端

后端启动后，会在默认端口 `35271` 开启监听服务供前端连接。此时将手机和电脑连接至同一局域网，手机访问 `http://<电脑 ip>:35271` 即可打开控制页面。

> 可在 `powershell` 或 `cmd` 中使用 `ipconfig` 指令获取电脑端 ip 地址。
> 
> 如果需要修改端口，需要保持 `ra2_trainer.toml` 和 `ra2_trainer_backend.toml` 中的配置一致。
> 
> 如果电脑浏览器可以打开 `http://localhost:35271` 但是手机访问超时，需要关闭[电脑防火墙](https://support.microsoft.com/en-us/windows/firewall-and-network-protection-in-the-windows-security-app-ec0844f7-aebd-0583-67fe-601ecf5d774f)。

## 注意事项

- 如果全局快捷键存在冲突，会注册失败，对应功能后的括号为空。

- 项目使用 [`exccpkg`](https://github.com/AdjWang/exccpkg) 管理依赖，需要安装 `python >= 3.12, ninja`，如果不喜欢 `exccpkg`，可以用任何东西，只要让 `CMakeLists.txt` 里的 `find_package` 正常工作即可。

- 修改器后端会在游戏目录下产生 `ra2_trainer_backend.log` 日志，以供故障排查。

## 已知问题

- 用于 Steam 尤里的复仇原版时游戏卡顿。需要将游戏目录下 `DDrawCompat.ini` 中的 `CpuAffinity = 1` 改为 `CpuAffinity = all`，修改后前端响应仍然略卡，但是后端看起来可以正常工作。

- `Tiberium Crisis 2` 新篇章第一关使用工程师占领无线电台时不能开启无敌，否则无法触发任务事件导致卡关。

- `Tiberium Crisis 2` 任务结束退出时如果没有附加 Ares 调试器会无法保存当前游戏进度和勋章。

## 界面说明

### 阵营过滤列表页面

**与单位或阵营相关的功能仅对被保护阵营生效**。框选任意单位后可在修改器中添加被选单位所属阵营为被保护阵营，也允许保护 AI 玩家。

### 功能页面

快捷键：Alt+功能括号后标记的按键

说明信息中涉及的功能用斜体表示。

#### 输入框

1. 钱：顾名思义

#### 按钮

1. 立即胜利：跳关用，解决偶尔游戏出 bug 卡关问题
2. 删除单位：让**选中的**单位直接消失
3. 地图全开：地图迷雾全开(如果要看透裂缝产生器还需要开启 *瘫痪裂缝产生器* 功能)
4. 核弹攻击：获得一枚核弹
5. 单位升级：**选中的**单位升3级(可以对群体使用)
6. 单位加速：**选中的**单位移动速度增加(可以对群体使用)
7. 快速建造：提高建造速度
8. 这是我的：**选中的**单位归到我方阵营

#### 选项

1. 无敌：免疫任何伤害和超时空，但是不免疫工程师占领(免疫工程师占领的功能是 *全是我的-占领* )
2. 瞬间建造：顾名思义。
3. 无限超武：无限施放超级武器和伞兵之类，但是对上面的一次性 *核弹攻击* 功能无效。如果有发射井，*核弹攻击* 也是无效的。
4. 极速攻击：攻击速度最大化。
5. 极速转身：战车和炮塔旋转速度最大化。(巨炮会变得很变态...)
6. 远程攻击：攻击距离最大化。(但是不警戒，不会自动远距离攻击)
7. 远程警戒：警戒距离最大化。( *远程攻击* 开启后使能。配合 *远程攻击* 使用，自动远距离攻击)
8. 瘫痪裂缝产生器：裂缝产生器无效化。
9. 卖卖卖：可以售卖地图上所有单位，比如敌方建筑，任意中立建筑，步兵，战车等。
10. 随意建筑：无视是否邻近、水面和陆地。
11. 自动修理：个人认为是最爽的功能。玩家占领的中立建筑也是可以修的~
12. 社会主义万岁：企图控制玩家单位的尤里(包括建筑和车)会归属给玩家。玩家的尤里阵亡后控制的单位不会回到敌方阵营。
13. 全是我的-占领：任何"占领"事件的目标单位归属给玩家。
14. 全是我的-房屋驻军：任何"房屋驻军"事件的目标单位归属给玩家，但是房子里的部队不会改变归属。
15. 侵略模式：开启后可以自动攻击敌方建筑物。
16. 全科技：开启全科技，开启后需要随便造个东西才能生效。
17. 无限火力：单位重新装填速度最大化，且单位弹药容量扩容到`15`。
18. 瞬间超时空：超时空单位移动和攻击无冷却。配合 *极速攻击* 使用效果更佳。
19. 无间道：被敌方间谍入侵会获得敌方科技。
20. 任务调速：开启后执行任务时可以在暂停界面调整游戏速度。

## 感谢

感谢 [bigsinger](https://github.com/bigsinger/) 提供的建议和咨询。
