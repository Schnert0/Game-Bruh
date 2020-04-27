#include <stdio.h>
#include <stdint.h>
#include <SDL.h>
#include "types.h"

#include "system/system.h"

int main(int argc, char* argv[]){
    c_system system(true);

    //system.loadBIOS("BIOS/DMG_BIOS.gb");
    system.loadBIOS("BIOS/CGB_BIOS.gb");

    //system.loadROM("ROMS/Super Mario Land (JUE) (V1.1) [!].gb");
    //system.loadROM("ROMS/Super Mario Land 2 - 6 Golden Coins (USA, Europe).gb");

    //system.loadROM("ROMS/Dr. Mario (JU) (V1.1).gb");
    //system.loadROM("ROMS/Tetris.gb");

    //system.loadROM("ROMS/Legend of Zelda, The - Link's Awakening (USA, Europe).gb");
    //system.loadROM("ROMS/Legend of Zelda, The - Link's Awakening DX (U) (V1.2) [C][!].gbc");
    //system.loadROM("ROMS/Legend of Zelda, The - Oracle of Ages (U) [C][!].gbc");

    //system.loadROM("ROMS/Looney Tunes (U) [!].gb");
    //system.loadROM("ROMS/Bugs Bunny (U) [!].gb");

    //system.loadROM("ROMS/Kirby's Dream Land (USA, Europe).gb");

    //system.loadROM("ROMS/Pokemon - Red Version (USA, Europe).gb");
    //system.loadROM("ROMS/Pokemon - Yellow Version (UE) [C][!].gbc");
    //system.loadROM("ROMS/Pokemon - Gold Version (UE) [C][!].gbc");
    //system.loadROM("ROMS/Pocket Monsters Midori (Japan).gb");

    //system.loadROM("ROMS/cpu_instrs.gb");
    //system.loadROM("ROMS/halt_bug.gb");
    //system.loadROM("ROMS/individual/02-interrupts.gb");
    system.loadROM("ROMS/instr_timing.gb");
    system.reset();

    system.run();

    return 0;
}
