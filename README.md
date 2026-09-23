# DetachableSilencer
A GTA:SA mod that allows you to turn the silenced pistol into a normal pistol and vice-versa.

> [!WARNING]
> Contributors needed! I am unfortunately not able to add controller support as I do not have a controller at hand. If anybody does, and has some knowledge of plugin-sdk modding and controller-handling, please make a pull-request or contact me with one of the methods at the very bottom of this README. Thanks! and have fun!

<img width="1280" height="720" alt="ezgif-59b063de10593efa" src="https://github.com/user-attachments/assets/a9839668-b84f-4ba2-b957-330735cf6831" />

---
Installation
---
Drop the modloader folder to the root folder of GTA:SA.

config.ini
---
- **keybind:** the keybind to attach/detach the silencer.
- **cheatcode:** the cheatcode that can be used to refresh the configuration. (old cheatcode has to be used to refresh!)
- **boneid:** the bone id that the silencer model is attached to.
- **x,y & z offsets:** the offsets of the silencer model from the fingers.
- **ifp:** IFP file of the switch animation.
- **anim:** Anim name of the switch animation.
- **animDuration:** Duration of the anim. (After how long the silencer model is deleted)
- **animSpot:** When the silencer model is attached to the gun.
- **silencerModel:** The model id of the silencer model.
- **silencer:** a field that is used to save whether the player has a silencer or not.

COMMON VIRTUAL KEY CODES (Decimal)
---
> For DetachableSilencer config.ini

| Key | Decimal | Key | Decimal |
| :--- | :--- | :--- | :--- |
| **Letters** | | | |
| A | 65 | N | 78 |
| B | 66 | O | 79 |
| C | 67 | P | 80 |
| D | 68 | Q | 81 |
| E | 69 | R | 82 |
| F | 70 | S | 83 |
| G | 71 | T | 84 |
| H | 72 | U | 85 |
| I | 73 | V | 86 |
| J | 74 | W | 87 |
| K | 75 | X | 88 |
| L | 76 | Y | 89 |
| M | 77 | Z | 90 |
| **Numbers (Top Row)** | | **Function Keys** | |
| 0 | 48 | F1 | 112 |
| 1 | 49 | F2 | 113 |
| 2 | 50 | F3 | 114 |
| 3 | 51 | F4 | 115 |
| 4 | 52 | F5 | 116 |
| 5 | 53 | F6 | 117 |
| 6 | 54 | F7 | 118 |
| 7 | 55 | F8 | 119 |
| 8 | 56 | F9 | 120 |
| 9 | 57 | F10 | 121 |
| **Numpad** | | | |
| Numpad 0 | 96 | F11 | 122 |
| Numpad 1 | 97 | F12 | 123 |
| Numpad 2 | 98 | **Special Keys** | |
| Numpad 3 | 99 | Tilde (~) | 192 |
| Numpad 4 | 100 | Backspace | 8 |
| Numpad 5 | 101 | Tab | 9 |
| Numpad 6 | 102 | Enter | 13 |
| Numpad 7 | 103 | Shift | 16 |
| Numpad 8 | 104 | Ctrl | 17 |
| Numpad 9 | 105 | Alt | 18 |
| Multiply (*) | 106 | Spacebar | 32 |
| Add (+) | 107 | Left Arrow | 37 |
| Subtract (-) | 109 | Up Arrow | 38 |
| Decimal (.) | 110 | Right Arrow | 39 |
| Divide (/) | 111 | Down Arrow | 40 |

CREDITS:
---
> Distributed as FOSS under the MIT license.
- Me (vecondite): The mod itself and the animation.
- metayeti: [INI Library](https://github.com/metayeti/mINI).
- Dryxio: [Animation Editor](https://gtastuff.com/tools/ifp-editor).
- LINK2012: Injector (hooking) library.
- black.greyed_61525: Alternative animation.
- Rockstar Games: Default weapon models.
- Gemini: Minor bug fixes, Virtual key map above.
- Ezgif: Converting the preview video into .gif
- Silenced pistol model creator of Immersive Weapons Pack by pedrowav
- All credits go to the respective creators of the elements packaged with the mod, explicitly mentioned here or not. Issues? contact me as below.

Need Help?
---
- @vecondite on Discord
- vecondite on libertycity
- vecondite on github
