![logo](https://i.imgur.com/gDbunES.png)
Fall Guys - Advanced Internal Cheat - 1.2 - 15.08.2020  
[![Discord](https://discordapp.com/api/guilds/370909694056726528/widget.png?style=shield)](https://discord.gg/jqbq85J)
  
  
## How to:
- Check the publication date of the latest public branch (https://steamdb.info/app/1097150/depots/)
  - If it's the same as the version the internal fits, then continue
  - If it's not the same, go to https://github.com/xTeJk/Fall_Guys_Internal#offsets and update the offsets
- Start game in any configuration
- Inject cheat with Xenos (Native Inject / Existing Process)
- Press `INSERT` to open cheat menu
- Don't close console window!

## Compilation:
- VS 2019 / v142 / .dll library
  - Use multi-byte character set
- Compile as `Release x64` version

## Offsets:
- Download: https://github.com/Perfare/Il2CppDumper
- Choose metadata file from game directory
- Select `GameAssembly.dll` from game directory to load game binaries and unity's references
- Export to json
- Replace `il2cpp.h` to new one inside cheat files directory
- Replace offsets in `offsets.h` with new one from `script.json`
- Recompile

## Keybinds:
If you want to change keybinds use:  
https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes  
find interesting key `VK_` or `0x` code and put inside `menu.cpp` at correct place, eg.:  
`(io.KeysDown[VK_F1] && !OldKeysDown[VK_F1])`  
replace to:  
`(io.KeysDown[0x31] && !OldKeysDown[0x31])`  
to make flying mode bootable by `1` instead of `F1`.  

## Safe Values:
The code implements the recommended "safe" values of individual functions,  
they can be edited in the `values.h` file, but it's not recommended,  
because exceeding the recommended value may result in blocking the account.

## Visuals Colors
To edit the color of the visuals, search for `0xFF` in `render.cpp ` 
and change the code after that phrase to your own RGB code, eg.:  
`0xFF589C21` (Green)  
replace to:  
`0xFF7E3EDE` (Red)  

## Functions:
All functions are possible for any configuration  
  
- GENERAL:
  - HOME > Hide Corner Text
- VISUALS:
  - F5 > Real Doors (Door Rush)
  - F6 > Real Path (Tip Toe)
  - F7 > Non-Jinxed Players
  - F8 > Show all platforms (Fruit Match)
  - F9 > Show player with tail (Final - WIP)
- MOVEMENT:
  - F1 > Fly Mode
  - F2 > Speed Hack (Default (9.5)
  - F3 > Dive Boost (Default: 16.5)
  - F4 > Gravity Scale (Default: 1.5)
- COLLISIONS:
  - F10 > Disable Stuns
  - F11 > Disable Object Collisions
- CARRYING GRABBABLE ITEMS:
  - Normal Item Drop Force (Default: 50)
  - Dive Item Drop Force (Default: 90)
  
![1](https://i.gyazo.com/0d26cb65308a04f29906a1cd7adb49fb.png)
![2](https://media.giphy.com/media/lMyPbfquGuXZtp4xo6/giphy.gif)
![3](https://media.giphy.com/media/WU6FJLsf7eIkbgDpMc/giphy.gif)
![4](https://media.giphy.com/media/WoR2PCgTLjCTAyA7tT/giphy.gif)
![5](https://i.imgur.com/Ge73we8.png)
