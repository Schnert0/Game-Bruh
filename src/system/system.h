#ifndef system_h
#define system_h

#include <SDL.h>
#include <stdio.h>
#include "../types.h"

#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "../ppu/ppu.h"
#include "../input/input.h"

#define FRAME_DELAY 1000 / 60

class c_system{
private:
    c_mem   mem;
    c_cpu   cpu;
    c_ppu   ppu;
    c_input input;

    bool biosLoaded;
    bool romLoaded;
    bool isCGB;

    int cycles;

public:
     c_system(bool cgb);
    ~c_system();

    void loadBIOS(const char* path);
    void loadROM(const char* path);
    void reset();
    void run();
};

#endif // system_h
