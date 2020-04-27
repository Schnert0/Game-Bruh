#include "system.h"

c_system::c_system(bool cgb){
      mem.getComponents(&cpu, &input);
      cpu.getComponents(&mem);
      ppu.getComponents(&mem, &cpu);
    input.getComponents(&mem, &cpu);

    biosLoaded = false;
    romLoaded  = false;
    isCGB      = cgb;
}

c_system::~c_system(){
}

void c_system::loadBIOS(const char* path){
    biosLoaded = mem.loadBIOS(path, isCGB);

}

void c_system::loadROM(const char* path){
    romLoaded = mem.loadROM(path);
}

void c_system::reset(){
    mem.reset();
    cpu.reset(biosLoaded, isCGB);
    ppu.reset(isCGB);
}

void c_system::run(){
    //mem.load();
    u8 state = STATE_RUNNING;
    while(state > 0){
        state = input.getInput();
        if(state == STATE_RESET){
            //mem.save();
            reset();
        }
        int cyclesThisUpdate = 0;
        while(cyclesThisUpdate < 69905){
            int cycles = cpu.tick();
            cyclesThisUpdate += cycles;
            cpu.updateTimers(cycles);
            ppu.updateGraphics(cycles >> 1);
            cpu.doInterrupts();
        }
        ppu.updateWindow();

    }
    //mem.save();
}
