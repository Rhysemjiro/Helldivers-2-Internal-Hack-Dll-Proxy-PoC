// NewDllMain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include "version.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "Memory.h"
#include "GameData.h"
#include <conio.h> // For _getch() on Windows
#include "xorstr.hpp"

HANDLE hNewMainThread;
HMODULE g_NewhModule;

// Function to initialize and display the console window
void InitializeNewConsole() {
    FILE* pNewFile = nullptr;
    AllocConsole();
    //SetConsoleTitle(_XOR_(L"GalacticHeroes 3 Proxy PoC"));
    freopen_s(&pNewFile, _XOR_("CONOUT$"), _XOR_("w"), stdout);
}

void DestroyNewConsole()
{
    DestroyWindow(GetConsoleWindow());
    FreeConsole();
}

//Check Key Pressed
bool NewKeyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 1);
}

//Console Menu using checkbox
struct NewCheckbox {
    std::string NewTitle;
    bool NewChecked;
};

// Function to display checkboxes
void displayNewCheckboxes(const std::vector<NewCheckbox>& NewCheckboxes, size_t NewSelectedCheckbox) {
    system("cls"); // Clear the console (Windows specific)

    printf(_XOR_("[Init] - GalacticHeroes 3 PoC Proxy...\n"));
    printf(_XOR_("[Init] - Thanks to cfemen and gir489...\n"));

    printf(_XOR_("[Ready] : Select some of the features below by pressing the [Space] key.\n"));
    printf(_XOR_("[Ready] : Press [Enter] to run the feature you selected.\n"));
    printf(_XOR_("[Ready] : After pressing [Enter], the selected features cannot be changed.\n"));

    std::cout << _XOR_("Checkboxes:\n");
    for (size_t i = 0; i < NewCheckboxes.size(); ++i) {
        if (i == NewSelectedCheckbox) {
            std::cout << "> ";
        }
        else {
            std::cout << "  ";
        }

        std::cout << "[" << (NewCheckboxes[i].NewChecked ? 'X' : ' ') << "] " << NewCheckboxes[i].NewTitle << "\n";
    }
    std::cout << "\n";
}

DWORD WINAPI NewPayload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    HMODULE Newdllfwd = dllforward::setup();
    if (Newdllfwd)
        CloseHandle(Newdllfwd);

    //Console Menu
    std::vector<NewCheckbox> NewCheckboxes = {
          {_XOR_("Inf Health"), false}
        , {_XOR_("Inf Grenades"), false}
        , {_XOR_("Inf Ammo"), false}
        , {_XOR_("Inf Syringes"), false}
        , {_XOR_("Inf Stamina"), false}
        , {_XOR_("Inf Stratagems"), false}
        , {_XOR_("Inf Mission Time"), false}
        //, {"One / Two Hit Kill ( Bile Titan Bug, Aim Only Head )", false}
        , {_XOR_("No Reload"), false}
        , {_XOR_("Max Resources"), false}
        , {_XOR_("Inf Backpack"), false}
        , {_XOR_("Inf Special Weapon"), false}
        , {_XOR_("Instant Railgun"), false}
        , {_XOR_("Show All Map Icons"), false}
        , {_XOR_("No Backpack Shield Cooldown"), false}
        , {_XOR_("All Stratagems in Loadout"), false}
        , {_XOR_("All Gear in Loadout"), false}
        , {_XOR_("All Armor in Loadout"), false}

    }; // Initialize all checkboxes to unchecked
    const int NewNumCheckboxes = NewCheckboxes.size();
    size_t NewSelectedCheckbox = 0;
    char NewUserInput;

    HMODULE NewModuleHandle = nullptr;
    GameData gNewData;

    do
    {
        NewModuleHandle = GetModuleHandle(_XOR_(L"game.dll"));
        Sleep(1000);
    } while (!NewModuleHandle);
    Sleep(100);


    //Show Console
    InitializeNewConsole();

    do {

        displayNewCheckboxes(NewCheckboxes, NewSelectedCheckbox);

        // Get user input
        NewUserInput = _getch(); // Use _getch() for reading a single character without pressing Enter

        // Handle arrow key input
        switch (NewUserInput) {
        case 72: // Up arrow key
            NewSelectedCheckbox = (NewSelectedCheckbox == 0) ? NewNumCheckboxes - 1 : NewSelectedCheckbox - 1;
            break;

        case 80: // Down arrow key
            NewSelectedCheckbox = (NewSelectedCheckbox == NewNumCheckboxes - 1) ? 0 : NewSelectedCheckbox + 1;
            break;

        case ' ': // Space bar
            // Toggle the state of the selected checkbox
            NewCheckboxes[NewSelectedCheckbox].NewChecked = !NewCheckboxes[NewSelectedCheckbox].NewChecked;
            break;

        case 13: // Enter key
            std::cout << _XOR_("Activate Enhancements Feature.\n");
            break;

        case 27: // Esc key

            break;

        default:
            // Ignore other key presses
            break;
        }

    } while (NewUserInput != 13); // Loop until the Esc key is pressed


    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (checkboxes[i].checked)
        {

            if (checkboxes[i].title == _XOR_("Enh Health"))
            {
                if (!gData.InfHealth) // no need but its old code when activate using hotkey, but need to much hotkey for all feature
                {
                    BYTE WriteHealthBytes[] =
                    {
                        0x48, 0x85, 0xDB,                               // test rbx,rbx
                        0x74, 0x03,                                     // jz short @f
                        0x45, 0x89, 0x38,                               //  mov [r8],r15d
                                                                        // @@:
                        0x49, 0x8B, 0x84, 0xDE, 0x28, 0x04, 0x00, 0x00, // mov rax,[r14+rbx*8+00000428]
                        0x8B, 0x48, 0x10,                               // mov ecx,[rax+10]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_WriteHealth
                    };

                    BYTE SetHealthBytes[] =
                    {
                        0x48, 0x85, 0xD2,                                                       // test rdx,rdx
                        0x75, 0x0C,                                                             // jnz short @f
                        0x41, 0xC7, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00, 0x0F, 0x27, 0x00, 0x00, //   mov [r11+rcx*4+00004C28],#9999
                                                                                                // @@:
                        0x41, 0x8B, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,                         // mov eax,[r11+rcx*4+00004C28]
                        0x48, 0x8B, 0x5C, 0x24, 0x20,                                           // mov rbx,[rsp+20]
                        0x48, 0x8B, 0x74, 0x24, 0x28,                                           // mov rsi,[rsp+28]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_SetHealth
                    };

                    uintptr_t InfHealth = Memory::FindPattern(_XOR_("game.dll"), _XOR_("45 89 38 49 8B 84 DE 28 04 00 00"));
                    LPVOID memory = Memory::AllocateMemory(InfHealth, sizeof(WriteHealthBytes));
                    Memory::CreateTrampoline(InfHealth, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth + 14, WriteHealthBytes, Memory::ArrayLength(WriteHealthBytes));

                    uintptr_t InfHealth1 = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 8B 84 8B 28 4C 00 00"));
                    memory = Memory::AllocateMemory(InfHealth1, sizeof(SetHealthBytes));
                    Memory::CreateTrampoline(InfHealth1, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth1 + 18, SetHealthBytes, Memory::ArrayLength(SetHealthBytes));

                    gData.InfHealth = !gData.InfHealth;
                    //create trampolin
                    printf(_XOR_("[Active] Infinite Health\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Grenades"))
            {
                if (!gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 FF 08 4A 8B 84 ED"));
                    Memory::Nop((LPVOID)(GrenadesAddress), 3);
                    gData.InfGrenades = !gData.InfGrenades;
                    printf(_XOR_("[Active] Infinite Grenades\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Ammo"))
            {
                if (!gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 83 2C C2 01 4D 8D 04 C2 49 8B 84 CA"));
                    BYTE AmmoPatch[] = { 0x00 };
                    Memory::Patch((LPVOID)(AmmoAddress + 4), AmmoPatch, 1);
                    gData.InfAmmo = !gData.InfAmmo;
                    printf(_XOR_("[Active] Infinite Ammo\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Syringes"))
            {
                if (!gData.InfSyringes)
                {
                    uintptr_t Syringes = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 FF CF 3B C2 74 61"));
                    Memory::Nop((LPVOID)(Syringes), 3);
                    gData.InfSyringes = !gData.InfSyringes;
                    printf(_XOR_("[Active] Infinite Syringes\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Stamina"))
            {
                if (!gData.InfStamina)
                {
                    uintptr_t Stamina = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 41 0F 11 08 8B 48 10 E8 ?? ?? ?? ?? 41 8B 47 48"));
                    BYTE StaminaPatch[] = { 0xF3, 0x41, 0x0F, 0x11, 0x30 };
                    Memory::Patch((LPVOID)(Stamina), StaminaPatch, 5);
                    gData.InfStamina = !gData.InfStamina;
                    printf(_XOR_("[Active] Infinite Stamina\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Stratagems"))
            {
                if (!gData.InfStratagems)
                {
                    uintptr_t Stratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 86 BF 01 00 00 0F"));
                    BYTE StratagemsPatch1[] = { 0x90, 0xE9 };
                    Memory::Patch((LPVOID)(Stratagems), StratagemsPatch1, 2);
                    gData.InfStratagems = !gData.InfStratagems;
                    printf(_XOR_("[Active] Infinite Stratagems\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Mission Time"))
            {
                if (!gData.InfMissionTime)
                {
                    uintptr_t MissionTime = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 0F 5C C7 F3 41 0F 5F C5"));
                    Memory::Nop((LPVOID)(MissionTime), 4);
                    gData.InfMissionTime = !gData.InfMissionTime;
                    printf(_XOR_("[Active] Infinite Mission Time\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("No Reload"))
            {
                if (!gData.NoReload)
                {
                    BYTE NoReloadByte[] =
                    {
                        0x90,
                        0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,
                        0x8B, 0x48, 0x10,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t NoReload = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10"));

                    LPVOID memory = Memory::AllocateMemory(NoReload, 0x100);
                    Memory::CreateTrampoline(NoReload, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, NoReload + 14, NoReloadByte, Memory::ArrayLength(NoReloadByte));
                    gData.NoReload = !gData.NoReload;
                    printf(_XOR_("[Active] No Reload\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Max Resources"))
            {
                if (!gData.MaxResources)
                {
                    BYTE MaxResourcesByte[] =
                    {
                        0x41, 0x81, 0x84, 0x8A, 0xEC, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF0, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF4, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t MaxResources = Memory::FindPattern(_XOR_("game.dll"), _XOR_("45 01 B4 8A EC 17 00 00"));

                    LPVOID memory = Memory::AllocateMemory(MaxResources, 0x100);
                    Memory::CreateTrampoline(MaxResources, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, MaxResources + 17, MaxResourcesByte, Memory::ArrayLength(MaxResourcesByte));
                    gData.MaxResources = !gData.MaxResources;
                    printf(_XOR_("[Active] x500 Samples\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Instant Railgun"))
            {
                if (!gData.InstantRailGun)
                {
                    BYTE InstantRailGunByte[] =
                    {
                        0x43, 0xC7, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x40, // mov [r14+r15+00002C2C],403FFFFC { 3.00 }
                        0xF3, 0x43, 0x0F, 0x10, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00,             // movss xmm0,[r14+r15+00002C2C]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Railgun
                    };

                    uintptr_t InstantRailGun = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 43 0F 11 84 37 2C 2C 00 00"));

                    LPVOID memory = Memory::AllocateMemory(InstantRailGun, sizeof(InstantRailGunByte));
                    Memory::CreateTrampoline(InstantRailGun, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InstantRailGun + 14, InstantRailGunByte, Memory::ArrayLength(InstantRailGunByte));
                    gData.InstantRailGun = !gData.InstantRailGun;
                    printf(_XOR_("[Active] Instant Railgun\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Inf Special Weapon"))
            {
                if (!gData.InfSpecWeapon)
                {
                    uintptr_t SpecWeapon = Memory::FindPattern(_XOR_("game.dll"), _XOR_("44 89 7F 08 41 80 BC 24"));
                    Memory::Nop((LPVOID)(SpecWeapon), 4);
                    gData.InfSpecWeapon = !gData.InfSpecWeapon;
                    printf(_XOR_("[Active] Infinite Special Weapon\n"));
                }
            }


            if (checkboxes[i].title == _XOR_("No Backpack Shield Cooldown"))
            {
                if (!gData.ShieldNoCD)
                {
                    BYTE ShieldNoCDByte[] =
                    {
                        0xF3, 0x0F, 0x5C, 0xC9, 0x90
                    };

                    uintptr_t ShieldNoCD = Memory::FindPattern(_XOR_("game.dll"), _XOR_("F3 41 0F 5C CA F3 0F 11 8C EE"));
                    Memory::Patch((LPVOID)(ShieldNoCD), ShieldNoCDByte, 5);
                    gData.ShieldNoCD = !gData.ShieldNoCD;
                    printf(_XOR_("[Active] Backpack Shield No Cooldown\n"));
                }
            }
            

            if (checkboxes[i].title == _XOR_("Inf Backpack"))
            {
                if (!gData.InfBackpack)
                {
                    uintptr_t Backpack = Memory::FindPattern(_XOR_("game.dll"), _XOR_("2B C6 4D 8D 85 48 04 00 00"));
                    Memory::Nop((LPVOID)(Backpack), 2);
                    gData.InfBackpack = !gData.InfBackpack;
                    printf(_XOR_("[Active] Infinite Backpack\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("Show All Map Icons"))
            {
                if (!gData.ShowAllMapIcons)
                {
                    BYTE ShowAllMapIconsByte[] =
                    {
                        0xB8, 0x01, 0x00, 0x00, 0x00, 0x90
                    };

                    BYTE ShowAllMapIconsByte1[] =
                    {
                        0xF8
                    };

                    BYTE ShowAllMapIconsByte2n4[] =
                    {
                        0x90, 0xE9
                    };

                    BYTE ShowAllMapIconsByte3[] =
                    {
                        0xEB, 0x04
                    };

                    uintptr_t ShowAllMapIconsAddr = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 0F B6 44 97 23"));
                    uintptr_t aob_CheckIfAlienHivesAreObstructed = Memory::FindPattern(_XOR_("game.dll"), _XOR_("41 80 BE 3C BA 07 00 00"));
                    uintptr_t aob_CheckIfMinorInterestBlipIsDiscovered = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 ?? ?? ?? ?? 48 8B 44 24 ?? 80 78 29 00"));
                    uintptr_t aob_GetMinorInterestBlipIcon = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 84 ?? ?? ?? ?? 48 8B 4C 24 ?? F3 41 0F 10 4F"));
                    uintptr_t aob_CheckMissionBlip = Memory::FindPattern(_XOR_("game.dll"), _XOR_("0F 85 59 02 00 00 49 8D"));
                     
                    Memory::Patch((LPVOID)(ShowAllMapIconsAddr), ShowAllMapIconsByte, 6);
                    Memory::Patch((LPVOID)(aob_CheckIfAlienHivesAreObstructed), ShowAllMapIconsByte1, 1);
                    Memory::Nop((LPVOID)(aob_CheckIfAlienHivesAreObstructed+1), 7);
                    Memory::Patch((LPVOID)(aob_CheckIfMinorInterestBlipIsDiscovered), ShowAllMapIconsByte2n4, 2);
                    Memory::Patch((LPVOID)(aob_GetMinorInterestBlipIcon), ShowAllMapIconsByte3, 2);
                    Memory::Patch((LPVOID)(aob_CheckMissionBlip), ShowAllMapIconsByte2n4, 2);
                    gData.ShowAllMapIcons = !gData.ShowAllMapIcons;
                    printf(_XOR_("[Active] Show All Map Icons\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("All Stratagems in Loadout"))
            {
                if (!gData.AllStratagems)
                {
                    BYTE AllStratagemsByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllStratagems = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 89 5C 24 ?? 48 8B D9 85 D2 75 09"));
                    Memory::Patch((LPVOID)(AllStratagems), AllStratagemsByte, 3);
                    gData.AllStratagems = !gData.AllStratagems;
                    printf(_XOR_("[Active] Unlock All Stratagems\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("All Equipment in Armory"))
            {
                if (!gData.AllEquipment)
                {
                    BYTE AllEquipmentByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllEquipment = Memory::FindPattern(_XOR_("game.dll"), _XOR_("83 B9 ?? ?? ?? ?? ?? 75 ?? 85 D2 74 ?? 44 8B 89 ?? ?? ?? ?? 45 33 C0 45 85 C9 74 ?? 48 8D 81 ?? ?? ?? ?? 39 50 ?? 74 ?? 41 FF C0 48 83 C0 ?? 45 3B C1 72 ?? 32 C0 C3 8B 00 48 69 C8"));
                    Memory::Patch((LPVOID)(AllEquipment+11), AllEquipmentByte, 3);
                    gData.AllEquipment = !gData.AllEquipment;
                    printf(_XOR_("[Active] Unlock All Equipment\n"));
                }
            }

            if (checkboxes[i].title == _XOR_("All Armor in Armory"))
            {
                if (!gData.AllArmor)
                {
                    BYTE AllArmorByte[] =
                    {
                        0xB0, 0x01, 0xC3
                    };

                    uintptr_t AllArmor = Memory::FindPattern(_XOR_("game.dll"), _XOR_("48 83 EC ?? 44 8B 49 ?? 45 33 C0"));
                    Memory::Patch((LPVOID)(AllArmor), AllArmorByte, 3);
                    gData.AllArmor = !gData.AllArmor;
                    printf(_XOR_("[Active] Unlock All Armor\n"));
                }
            }

            /*if (checkboxes[i].title == "One / Two Hit Kill ( Bile Titan Bug, Aim Only Head )")
            {
                if (!gData.OHK)
                {
                    BYTE OHKByte[] =
                    {
                        0x83, 0xBF, 0x38, 0x0B, 0x00, 0x00, 0x0A,
                        0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,
                        0xE9, 0x18, 0x00, 0x00, 0x00,
                        0xC7, 0x87, 0x44, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x66, 0x4A, 0x6B, 0x80, 0x01, 0x00, 0x00, 0x00,
                        0x89, 0x87, 0x44, 0x64, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    uintptr_t OHK = Memory::FindPattern("game.dll", "89 87 44 64 00 00");
                    LPVOID memory = Memory::AllocateMemory(OHK, 0x100);
                    Memory::CreateTrampoline(OHK, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, OHK + 15, OHKByte, Memory::ArrayLength(OHKByte));
                    gData.OHK = !gData.OHK;
                    printf("[Active] Instant Railgun\n");
                }
            }*/
            


        }
    }
    printf(_XOR_("[Exit] Unload\n"));
    FreeConsole();
    FreeLibraryAndExitThread(g_hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        g_hModule = hModule;
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

