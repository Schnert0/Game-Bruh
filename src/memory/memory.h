#ifndef memory_h
#define memory_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../types.h"

#include "../cpu/cpu.h"
#include "../input/input.h"

class c_mem{
private:
    class c_cpu*   cpu;
    class c_input* input;

    u8  romBankNumber;
    u8  ramBankNumber;
    u8 wramBankNumber;
    u8 vramBankNumber;

    bool isMBC1;
    bool isMBC2;
    bool saveExists;
    bool biosLoaded;

    u8* romBuffer;
    u8* ramBuffer;
    u32 romSize;
    u32 ramSize;

    u8 vram[0x4000];
    u8 bios[0xc00];
    u8 wram[0x8000];
    u8 paletteBG[0x40];
    u8 paletteSP[0x40];
    char* fileName;

    bool isCGB;

    bool hdmaActivated;
    u16  hdmaSrc;
    u16  hdmaDst;
    u8   hdmaSize;
    u8   hdmaCycles;

public:
    u8 memory[0x10000];

     c_mem();
    ~c_mem();
    void getComponents(c_cpu* cpuPtr, c_input* inputPtr);

    void reset();
    bool loadBIOS(const char* path, bool cgb);
    bool loadROM(const char* path);

    u8 load8(u16 address);
    void store8(u16 address, u8 data);

    void incDivider();
    u8 joyInput();

    u16 load16(u16 address);
    void store16(u16 address, u16 data);

    void dmaTransfer(u8 data);
    void doHDMA();

    void save();
    void load();

    u16 loadcolor(u8 color, bool sprite);
    u8  loadVBK1(u16 addr);

};

#endif // memory_h
