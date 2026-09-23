#include <plugin.h> // Plugin-SDK version 1005 from 2026-08-14 08:10:00
#include <mini/ini.h>
#include <CHud.h>
#include <CStreaming.h>
#include <extensions/ScriptCommands.h>
#include <CWorld.h>
#include <CCheat.h>
#define INJECTOR_USE_VARIADIC_TEMPLATES
#include <Injector.h>

using namespace plugin;

unsigned int keyBind = 255;
std::string cheatCode;
unsigned int boneId;
float xOffset;
float yOffset;
float zOffset;
bool silencer;
std::string ifpName;
std::string animName;
unsigned int animDuration;
unsigned int animSpot;
unsigned int silencerModel;

mINI::INIFile file("config.ini");
mINI::INIStructure ini;

void readConfig() {
    keyBind = 192;
    cheatCode = "DSP";
    boneId = 35;
    xOffset = 0.03;
    yOffset = 0.0;
    zOffset = -0.01;
    ifpName = "silencer";
    animName = "silencer";
    animDuration = 1167;
    animSpot = 900;
    silencerModel = 328;
    silencer = 0;

    static char msg[1024];

    if (!file.read(ini)) {
        ini["settings"]["keybind"] = std::to_string(keyBind);
        ini["settings"]["cheatcode"] = cheatCode;
        ini["settings"]["boneid"] = std::to_string(boneId);
        ini["settings"]["xoffset"] = std::to_string(xOffset);
        ini["settings"]["yoffset"] = std::to_string(yOffset);
        ini["settings"]["zoffset"] = std::to_string(zOffset);
        ini["settings"]["ifp"] = ifpName;
        ini["settings"]["anim"] = animName;
        ini["settings"]["animduration"] = std::to_string(animDuration);
        ini["settings"]["animspot"] = std::to_string(animSpot);
        ini["settings"]["silencerModel"] = std::to_string(silencerModel);
        ini["saves"]["silencer"] = std::to_string(silencer);
        file.generate(ini);

        sprintf_s(msg, "Config ~r~not found!~w~~n~~n~"
            "~b~Default config~w~ with values:~n~~n~"
            "Keybind: ~b~%d~w~~n~"
            "Cheat code: ~b~%s~w~~n~"
            "BoneID: ~b~%d~w~~n~"
            "xyzOffset: ~b~%f %f %f~w~~n~"
            "ifp: ~b~%s~w~~n~"
            "anim: ~b~%s~w~~n~"
            "AD: ~b~%d~w~~n~"
            "AS: ~b~%d~w~~n~"
            "M: ~b~%d~w~~n~"
            "Silencer: ~b~%d~w~",
            keyBind, cheatCode.c_str(), boneId, xOffset, yOffset, zOffset, ifpName.c_str(), animName.c_str(), animDuration, animSpot, silencerModel, silencer);
        CHud::SetHelpMessage(msg, true, false, false);
    }
    try {
        file.read(ini);
        keyBind = std::stoul(ini["settings"]["keyBind"], nullptr, 0);
        cheatCode = ini["settings"]["cheatcode"];
        boneId = std::stoi(ini["settings"]["boneid"]);
        xOffset = std::stof(ini["settings"]["xoffset"]);
        yOffset = std::stof(ini["settings"]["yoffset"]);
        zOffset = std::stof(ini["settings"]["zoffset"]);
        ifpName = ini["settings"]["ifp"];
        animName = ini["settings"]["anim"];
        animDuration = std::stoi(ini["settings"]["animduration"]);
        animSpot = std::stoi(ini["settings"]["animspot"]);
        silencerModel = std::stoi(ini["settings"]["silencerModel"]);
        silencer = std::stoi(ini["saves"]["silencer"]);

        sprintf_s(msg, "Config ~g~found~w~:~n~~n~"
            "Keybind: ~b~%d~w~~n~"
            "Cheat code: ~b~%s~w~~n~"
            "BoneID: ~b~%d~w~~n~"
            "xyzOffset: ~b~%f %f %f~w~~n~"
            "IFP: ~b~%s~w~~n~"
            "Anim: ~b~%s~w~~n~"
            "AD: ~b~%d~w~~n~"
            "AS: ~b~%d~w~~n~"
            "M: ~b~%d~w~~n~"
            "Silencer: ~b~%d~w~",
            keyBind, cheatCode.c_str(), boneId, xOffset, yOffset, zOffset, ifpName.c_str(), animName.c_str(), animDuration, animSpot, silencerModel, silencer);
        CHud::SetHelpMessage(msg, true, false, false);
    }
    catch (const std::exception& e) {
        sprintf_s(msg, "~r~ERR!~w~~n~~n~"
            "Keybind: ~b~%d~w~~n~"
            "Cheat code: ~b~%s~w~~n~"
            "BoneID: ~b~%d~w~~n~"
            "xyzOffset: ~b~%f %f %f~w~~n~"
            "ifp: ~b~%s~w~~n~"
            "anim: ~b~%s~w~~n~"
            "AD: ~b~%d~w~~n~"
            "AS: ~b~%d~w~~n~"
            "M: ~b~%d~w~~n~"
            "Silencer: ~b~%d~w~",
            keyBind, cheatCode.c_str(), boneId, xOffset, yOffset, zOffset, ifpName.c_str(), animName.c_str(), animDuration, animSpot, silencerModel, silencer);
        CHud::SetHelpMessage(msg, true, false, false);
    }
    std::reverse(cheatCode.begin(), cheatCode.end());
}

struct Main
{
    size_t m_frame = 0; // render frame counter

    Main()
    {
        // register event callbacks
        Events::gameProcessEvent += [] { gInstance.OnGameProcess(); };
        linkInjector::save_manager::on_save([](int slot) {
            ini["saves"]["silencer"] = std::to_string(silencer);
            file.write(ini);
        });
        readConfig();
    }

    bool wasKeyPressed = false;
    bool wasAlive = true;

    unsigned int lastWeapAmmo = 0;
    bool hasSavedAmmo = false;
    unsigned int lastWeapClip;
    eWeaponType lastWeapType;

    CObject* silencerObject = nullptr;
    bool silencerState = false;

    bool isPistol(eWeaponType weapType) {
        return weapType == WEAPONTYPE_PISTOL || weapType == WEAPONTYPE_PISTOL_SILENCED;
    }

    unsigned int animDuration = 1500;
    unsigned int animTime = 0;
    unsigned int animEnd = 0;

    bool wasModSwap = false;

    void OnGameProcess()
    {
        CPlayerPed* player = FindPlayerPed();
        if (!player) return;

        bool alive = player->IsAlive();
        if (!alive && wasAlive) {
            hasSavedAmmo = false;
            lastWeapType = WEAPONTYPE_UNARMED;
            lastWeapAmmo = 0;
            lastWeapClip = 0;
            silencer = 0;

            animTime = 0;
            animEnd = 0;
            silencerState = false;
        }
        wasAlive = alive;

        if (!plugin::Command<0x04EE>(ifpName.c_str())) {
            plugin::Command<0x04ED>(ifpName.c_str());
            return;
        }

        if (strncmp(CCheat::m_CheatString, cheatCode.c_str(), cheatCode.length()) == 0) {
            CCheat::m_CheatString[0] = '\0';
            readConfig();
        }

        CWeapon& currentWeaponRef = player->m_aWeapons[player->m_nSelectedWepSlot];
        CWeapon currentWeapon = currentWeaponRef;
        eWeaponType currentWeapType = currentWeapon.m_eWeaponType;
        unsigned int currentWeapAmmo = currentWeaponRef.m_nAmmoTotal;

        if (alive) {
            if (isPistol(currentWeapType)) {
                if (lastWeapType != currentWeapType) {
                    if (currentWeapType == WEAPONTYPE_PISTOL_SILENCED) {
                        silencer = 1;
                    }
                    if (wasModSwap) {
                        wasModSwap = false;
                    }
                    else {
                        if (isPistol(lastWeapType) && currentWeapAmmo == lastWeapAmmo && hasSavedAmmo) {
                            currentWeaponRef.m_nAmmoTotal = lastWeapAmmo + currentWeapAmmo;
                            currentWeaponRef.m_nAmmoInClip = lastWeapClip;
                        }
                    }
                    if (currentWeapAmmo < lastWeapAmmo && hasSavedAmmo) {
                        currentWeaponRef.m_nAmmoTotal = lastWeapAmmo + currentWeapAmmo;
                        currentWeaponRef.m_nAmmoInClip = lastWeapClip;
                    }

                    lastWeapAmmo = currentWeaponRef.m_nAmmoTotal;
                    lastWeapClip = currentWeaponRef.m_nAmmoInClip;
                    hasSavedAmmo = true;
                }
                else {
                    lastWeapAmmo = currentWeapAmmo;
                    lastWeapClip = currentWeaponRef.m_nAmmoInClip;
                    hasSavedAmmo = true;
                }
            }
            lastWeapType = currentWeapType;
        }

        unsigned int currentTime = CTimer::m_snTimeInMilliseconds;
        bool giveWeapon = animTime != 0 && animTime < currentTime;
        bool pressed = KeyPressed(keyBind);
        if (alive && ((pressed && !wasKeyPressed && animTime == 0) || giveWeapon) && isPistol(currentWeapType) && silencer) {
            if (silencerObject == nullptr) {
                if (!CStreaming::HasModelLoaded(silencerModel)) {
                    CStreaming::RequestModel(silencerModel, false);
                    CStreaming::LoadAllRequestedModels(false);
                }
                silencerObject = new CObject(silencerModel, true);
                CWorld::Add(silencerObject);
            }

            if (giveWeapon) {
                eWeaponType nextWeap = currentWeapType == WEAPONTYPE_PISTOL ? WEAPONTYPE_PISTOL_SILENCED : WEAPONTYPE_PISTOL;
                unsigned int nextWeapModel = CWeaponInfo::GetWeaponInfo(nextWeap)->m_nModelId;
                if (!CStreaming::HasModelLoaded(nextWeapModel)) {
                    CStreaming::RequestModel(nextWeapModel, false);
                    CStreaming::LoadAllRequestedModels(false);
                }
                lastWeapAmmo = currentWeapon.m_nAmmoTotal;
                lastWeapClip = currentWeapon.m_nAmmoInClip;

                player->GiveWeapon(nextWeap, lastWeapAmmo, false);
                player->m_aWeapons[player->GetWeaponSlot(nextWeap)].m_nAmmoTotal = lastWeapAmmo;
                player->m_aWeapons[player->GetWeaponSlot(nextWeap)].m_nAmmoInClip = lastWeapClip;

                wasModSwap = true;

                CWeapon givenWeapon = player->m_aWeapons[player->GetWeaponSlot(nextWeap)];

                animTime = 0;

                silencerState = givenWeapon.m_eWeaponType == WEAPONTYPE_PISTOL_SILENCED ? false : true;
            }
            else {
                plugin::Command<COMMAND_TASK_PLAY_ANIM_SECONDARY>(player, animName.c_str(), ifpName.c_str(), 4.0f, 0, 0, 0, 0, 0, -1);
                animTime = currentTime + animSpot;
                animEnd = animTime + animDuration;

                silencerState = currentWeapType == WEAPONTYPE_PISTOL_SILENCED ? false : true;
            }
        }
        wasKeyPressed = pressed;

        if (animEnd != 0 && animEnd < currentTime) {
            silencerState = false;
        }

        if (silencerObject != nullptr) {
            if (silencerState) {
                CWorld::Remove(silencerObject);
                RpHAnimHierarchy* animHierarchy = GetAnimHierarchyFromSkinClump(player->m_pRwClump);
                if (animHierarchy) {
                    unsigned int boneIndex = RpHAnimIDGetIndex(animHierarchy, boneId);
                    RwMatrix* boneMatrix = &RpHAnimHierarchyGetMatrixArray(animHierarchy)[boneIndex];
                    CMatrix* silencerMatrix = silencerObject->m_matrix;
                    silencerMatrix->at = (CVector(-boneMatrix->right.x, -boneMatrix->right.y, -boneMatrix->right.z));
                    silencerMatrix->pos = boneMatrix->pos + (boneMatrix->right * xOffset) + (boneMatrix->up * yOffset) + (boneMatrix->at * zOffset);
                    silencerMatrix->up = boneMatrix->up;
                    silencerMatrix->right = boneMatrix->at;
                    silencerObject->UpdateRwMatrix();
                    silencerObject->UpdateRwFrame();
                };
                CWorld::Add(silencerObject);
            }
            else {
                CWorld::Remove(silencerObject);
                delete silencerObject;
                silencerObject = nullptr;
            }
        }

        if (!isPistol(player->m_aWeapons[player->GetWeaponSlot(WEAPONTYPE_PISTOL)].m_eWeaponType) && hasSavedAmmo) {
            lastWeapAmmo = 0;
            lastWeapClip = 0;
            hasSavedAmmo = false;
        }
    }
} gInstance;
