#include <plugin.h> // Plugin-SDK version 1005 from 2026-08-14 08:10:00
#include <CStreaming.h>
#include <CWeaponInfo.h>
#include <extensions/ScriptCommands.h>
#include <mini/ini.h>
#include <CCheat.h>
#include <CHud.h>

using namespace plugin;

struct Main
{
    bool silencer = 0;
    unsigned int key = VK_TAB;

    mINI::INIFile file{ PLUGIN_PATH("config.ini") };
    mINI::INIStructure ini;

    std::string cheatCode = "PSD";

    void updateConfigs() {
        static char msg[1024];
        try {
            file.read(ini);
            key = std::stoul(ini["settings"]["key"], nullptr, 0);
            silencer = std::stoi(ini["saved"]["silencer"]);
            cheatCode = ini["settings"]["cheat"];
            sprintf_s(msg, "~g~Refreshed DetachableSilencer!~w~~n~~n~Cheat is: %s~n~Key is: %d~n~Silencer: %d", cheatCode.c_str(), key, silencer);
            std::reverse(cheatCode.begin(), cheatCode.end());
            CHud::SetHelpMessage(msg, true, false, false);
        }
        catch(const std::exception& e){
            sprintf_s(msg, "~r~Error when reading the config file:~n~~w~%s", e.what());
            std::reverse(cheatCode.begin(), cheatCode.end());
            CHud::SetHelpMessage(msg, true, true, false);
        }
    }

    void saveToFile() {
        ini["saved"]["silencer"] = std::to_string(silencer);
        file.write(ini);
    }

    Main()
    {
        // register event callbacks
        Events::gameProcessEvent += [] { gInstance.OnGameProcess(); };

        updateConfigs();
    }

    bool wasPressed = false;
    bool wasAlive = true;

    eWeaponType lastWeapType;

    unsigned int ammoInGun = 0;

    void OnGameProcess()
    {
        if (strncmp(CCheat::m_CheatString, cheatCode.c_str(), cheatCode.length()) == 0) {
            CCheat::m_CheatString[0] = '\0';
            updateConfigs();
        }
        CPlayerPed* player = FindPlayerPed();
        if (!player) return;

        bool alive = player->IsAlive();
        if (!alive && wasAlive) {
            silencer = 0;
            saveToFile();
        }
        wasAlive = alive;

        unsigned char selectedSlot = player->m_nSelectedWepSlot;
        CWeapon currentWeapon = player->m_aWeapons[selectedSlot];
        eWeaponType weaponType = currentWeapon.m_eWeaponType;
        bool silencerSupport = weaponType == WEAPONTYPE_PISTOL || weaponType == WEAPONTYPE_PISTOL_SILENCED;

        eWeaponType silPist = WEAPONTYPE_PISTOL_SILENCED;
        unsigned char playerWeapSlot = player->GetWeaponSlot(silPist);
        CWeapon& playerWeap = player->m_aWeapons[playerWeapSlot];
        eWeaponType currentWeapType = playerWeap.m_eWeaponType;

        if (silencerSupport) {
            if (ammoInGun > playerWeap.m_nAmmoTotal && currentWeapType != lastWeapType) playerWeap.m_nAmmoTotal = ammoInGun + playerWeap.m_nAmmoTotal;
            ammoInGun = playerWeap.m_nAmmoTotal;
            lastWeapType = currentWeapType;

            bool pressed = KeyPressed(key);
            if (pressed && !wasPressed && silencer) {
                eWeaponType nextWeap = weaponType == WEAPONTYPE_PISTOL ? WEAPONTYPE_PISTOL_SILENCED : WEAPONTYPE_PISTOL;
                unsigned int nextWeapModel = CWeaponInfo::GetWeaponInfo(nextWeap)->m_nModelId;
                if (!CStreaming::HasModelLoaded(nextWeapModel)) {
                    CStreaming::RequestModel(nextWeapModel, false);
                    CStreaming::LoadAllRequestedModels(false);
                }
                plugin::Command<0x0605>(player, "bomber", "ped", 4.0f, 0, 0, 0, 0, -1);
                player->GiveWeapon(nextWeap, currentWeapon.m_nAmmoTotal, false);

                CWeapon& givenWeapon = player->m_aWeapons[selectedSlot];
                givenWeapon.m_nAmmoInClip = currentWeapon.m_nAmmoInClip;
            }
            wasPressed = pressed;
        }

        if(!silencer && currentWeapType == silPist) {
            silencer = 1;
            saveToFile();
        }
    }
} gInstance;
