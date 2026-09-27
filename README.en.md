# RA2YurisRevengeTrainer

English | [简体中文](README.md)

A memory trainer for the original Yuri's Revenge 1.001 and Ares versions.

## Build

You need [CMake](https://cmake.org/), [Python 3.12 or later](https://www.python.org/), [npm](https://www.npmjs.com/), and [Visual Studio 2022](https://visualstudio.microsoft.com/). Run the build commands in an **x86 Developer Command Prompt or Developer PowerShell** for Visual Studio.

Install the build tools and Python dependencies:

```powershell
cargo install tauri-cli
uv sync
```

### Desktop frontend

```powershell
cd src/frontend/desktop/src-tauri
cargo tauri build
```

### Backend

From the repository root, build the dependencies:

```powershell
uv run exccpkgfile.py
```

Then build the web frontend, embed it in the backend, and compile the DLLs:

```powershell
cd src/frontend/web
npm install
npm run build
cd ../../..
uv run scripts/generate_web_main_page.py
uv run cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded -DCMAKE_INSTALL_PREFIX=deps/out/Release -G "Visual Studio 17 2022" -S . -B ./build -T v143,host=x86 -A win32
uv run cmake --build ./build --config Release --target wsock32 ra2_trainer_backend -j $env:NUMBER_OF_PROCESSORS
```

In Command Prompt, use `%NUMBER_OF_PROCESSORS%` instead of `$env:NUMBER_OF_PROCESSORS`.

## Use

The backend must be loaded into the game before the desktop or web controls can work. After starting the game, you can press `Alt+1` to check for the injection confirmation sound.

### Load the backend automatically

- **Original Yuri's Revenge:** Put `wsock32.dll`, `ra2_trainer_backend.dll`, and `ra2_trainer_backend.toml` in the game directory.
- **Ares:** Put `ra2_trainer_backend.dll` and `ra2_trainer_backend.toml` in the game directory. Do not add `wsock32.dll`: it can crash some Ares mods, including Tiberium Crisis 2.

Some Ares versions do not automatically load the backend. In that case, use the manual method below. If you do not need Ares injection, the earlier [v4 release](https://github.com/AdjWang/RA2YurisRevengeTrainer/releases/tag/v4.2) is also available.

### Load the backend manually

For Ares versions that do not load the DLL automatically, such as Tiberium Crisis 2 or Revenge Time, inject `ra2_trainer_backend.dll` with a DLL injection tool (for example, [Cheat Engine's Inject DLL command](https://wiki.cheatengine.org/index.php?title=Help_File:Menus_and_Features)). Keep `ra2_trainer_backend.dll` and `ra2_trainer_backend.toml` in the game directory. Remove `wsock32.dll` if it is present.

### Open the controls

The desktop and web frontends can both be open, and their state synchronizes. You normally need only one.

- **Desktop:** Run `ra2_trainer.exe` with `ra2_trainer.toml` in the same directory. The default backend port is `35271`. The frontend does not modify the game directly and does not require administrator privileges. If it cannot connect, `test_server.exe` can temporarily stand in for the backend to check whether port `35271` is available.
- **Web:** Once the backend is loaded, open `http://localhost:35271` on the game PC. For a phone on the same local network, open `http://<PC IP>:35271`; `ipconfig` shows the PC's IP address. If you change the port, use the same value in `ra2_trainer.toml` and `ra2_trainer_backend.toml`. If the page works locally but times out on the phone, check [Windows Firewall settings](https://support.microsoft.com/en-us/windows/firewall-and-network-protection-in-the-windows-security-app-ec0844f7-aebd-0583-67fe-601ecf5d774f).

### Select your faction before using cheats

In the **Filter** tab, select one of your units in the game. Its faction appears under **Selected factions**. Click the faction or **Add all selected** to place it under **Protected factions**. Unit and faction related cheats, including **Add cash**, affect protected factions only. You may also protect an AI faction. Click a protected faction to remove it, or use **Clear protected** to remove them all.

Open the **Cheats** tab to use the actions and options below. On the desktop, the shortcut for an action is `Alt` plus the key shown in parentheses after its label. If a global shortcut conflicts with another application, registration may fail and the parentheses will be empty.

## Cheats

### Cash

Enter an amount and click **Add cash** to add it to protected factions. If the amount is left empty, the button uses a default of 23,333. Add your faction in the Filter tab first.

### Actions

| Action | Effect |
| --- | --- |
| Win now | Complete the current mission. |
| Delete selected units | Remove the units selected in-game. |
| Reveal map | Reveal the map. To see through gap generators, also enable **Disable gap generators**. |
| Get one nuke | Grant one nuclear strike. It is unaffected by **No superweapon cooldown** and does not work if a nuclear silo is present. |
| Make selected elite | Set selected units to elite rank; works on a group. |
| Speed up selected | Increase the movement speed of selected units; works on a group. |
| Fast build | Increase construction speed. |
| Claim selected units | Transfer selected units to your faction. |

### Options

| Option | Effect |
| --- | --- |
| Invulnerable | Prevent damage and chronoshift effects, but not engineer capture. Use **Claim captures** for captures. |
| Instant build | Finish construction immediately. |
| No superweapon cooldown | Reuse superweapons and paratroopers without waiting. Does not affect **Get one nuke**. |
| Maximum fire rate | Maximize attack speed. |
| Instant rotation | Maximize vehicle and turret rotation speed. |
| Maximum attack range | Maximize attack range; units do not automatically guard at that range. |
| Maximum guard range | Maximize automatic guard range when **Maximum attack range** is enabled. |
| Disable gap generators | Stop gap generators from obscuring the map. |
| Sell any unit/building | Allow selling units and buildings across the map, including enemy and neutral ones. |
| Build anywhere | Ignore placement restrictions such as adjacency and terrain. |
| Automatic repairs | Repair buildings, including captured neutral buildings. |
| Mind control protection | Enemy Yuri units attempting to control your units become yours. Units controlled by your Yuri do not revert when that Yuri dies. |
| Claim captures | Transfer the target of any capture event to your faction. |
| Claim garrisons | Transfer garrisoned buildings to your faction; units inside retain their original faction. |
| Auto-attack buildings | Let your units automatically attack enemy buildings. |
| Unlock all tech | Unlock all technology. Build something after enabling it for the effect to take hold. |
| Fast reload + 15 ammo | Maximize reload speed and increase ammunition capacity to 15. Rebuild units for the ammo change to take effect. |
| No chrono cooldown | Remove movement and attack cooldowns for chronoshift units. |
| Enemy spies grant tech | Gain enemy technology when an enemy spy infiltrates you. |
| Select enemy units | Allow `T` to select multiple enemy units or buildings. |
| Pause game | Pause during battle while still allowing you to inspect the map and select units. |
| Game speed | Adjust the game's speed with the slider. |

## Notes and known issues

- The backend writes `ra2_trainer_backend.log` in the game directory for troubleshooting.
- Dependencies are managed with [`exccpkg`](https://github.com/AdjWang/exccpkg), which requires Python 3.12 or later and Ninja. The dependency list is at the end of `exccpkgfile.py` if you need to build them yourself.
- The original Steam Yuri's Revenge can lag with the trainer. Changing `CpuAffinity = 1` to `CpuAffinity = all` in the game's `DDrawCompat.ini` may help. The frontend may still respond slowly even if the backend works.
- In the first mission of Tiberium Crisis 2's new chapter, disable **Invulnerable** before capturing the radio station with an engineer. Otherwise the mission event may not trigger.
- With a Debug build of Tiberium Crisis 2, exiting a mission without the Ares debugger attached can prevent progress and medals from being saved.

## Thanks

Thanks to [bigsinger](https://github.com/bigsinger/) for advice and consultation.
