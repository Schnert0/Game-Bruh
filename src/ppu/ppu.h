#ifndef ppu_h
#define ppu_h

#include <stdio.h>
#include <SDL.h>

#include "../memory/memory.h"
#include "../cpu/cpu.h"
#include "../util/util.h"


#define COLOR_WHITE     0b0111111111111111
#define COLOR_LIGHTGRAY 0b0110001100011000
#define COLOR_DARKGRAY  0b0100001000010000
#define COLOR_BLACK     0b0010000100001000

#define WHITE     0b00
#define LIGHTGRAY 0b01
#define DARKGRAY  0b10
#define BLACK     0b11

class c_ppu{
private:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Texture*  screenTexture;

    u16 screenData [144][160];

    c_mem* mem;
    c_cpu* cpu;

    int scanlineCounter;

    bool isCGB;

    void renderTiles();
    void renderSprites();

public:
     c_ppu();
    ~c_ppu();

    void getComponents(c_mem* memPtr, c_cpu* cpuPtr);
    void reset(bool cgb);

    void updateWindow();
    void updateGraphics(int cycles);
    void setLCDStatus();
    bool lcdEnabled();
    void drawScanLine();
    void drawVRAM();
    u8  getColor(u8 colorNum, u16 address);

};

#endif // ppu_h
