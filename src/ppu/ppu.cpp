#include "ppu.h"

c_ppu::c_ppu(){ // 320x288
    window = SDL_CreateWindow("Game Bruh", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,320, 288, SDL_WINDOW_SHOWN);
    SDL_Surface* icon = SDL_LoadBMP("../icons/GameBruh.bmp");
    SDL_SetWindowIcon(window, icon);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STATIC, 160, 144);
    memset(screenData, 0, sizeof(screenData));
}

c_ppu::~c_ppu(){
    SDL_DestroyTexture(screenTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void c_ppu::getComponents(c_mem* memPtr, c_cpu* cpuPtr){
    mem = memPtr;
    cpu = cpuPtr;
}

void c_ppu::reset(bool cgb){
    memset(screenData, 0, sizeof(screenData));

    isCGB = cgb;
}

void c_ppu::updateWindow(){

    for(int i = 0; i < 32; i++){
        screenData[0][i] = mem->loadcolor(i, false);
    }
    for(int i = 0; i < 32; i++){
        screenData[1][i] = mem->loadcolor(i, true);
    }


    SDL_RenderPresent(renderer);
}

void c_ppu::updateGraphics(int cycles){
    setLCDStatus();

    if(lcdEnabled())
        scanlineCounter -= cycles;
    else
        return;

    if(scanlineCounter <= 0){
        mem->memory[0xff44]++;
        u8 currLine = mem->load8(0xff44);
        scanlineCounter = 456;

        if(currLine == 144){
            cpu->requestInterrupt(0);
            SDL_UpdateTexture(screenTexture, NULL, screenData, 160*sizeof(u16));
            SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        }
        else if(currLine > 153){
            mem->memory[0xff44] = -1;
        }
        else if(currLine < 144){
            drawScanLine();
        }

    }
}

void c_ppu::setLCDStatus(){
    u8 status = mem->load8(0xff41);

    if(!lcdEnabled()){
        scanlineCounter = 456;
        mem->memory[0xff44] = 0;
        status &= 252;
        //status = setBit(status, 0, true);
        mem->store8(0xff41, status);
        return;
    }

    u8   currLine = mem->load8(0xff44);
    u8   currMode = status & 3;
    u8   mode = 0;
    bool requestInt = false;

    if(currLine >= 144){
        mode = 1;
        status = setBit(status,0, true);
        status = setBit(status,1, false);
        requestInt = getBit(status, 4);
    }else{
        if(scanlineCounter >= 376){
            mode = 2;
            status = setBit(status, 0, false);
            status = setBit(status, 1, true);
            requestInt = getBit(status, 5);
        }
        else if(scanlineCounter >= 204){
            mode = 3;
            status = setBit(status, 0, true);
            status = setBit(status, 1, true);
        }
        else{
            mode = 0;
            status = setBit(status, 0, false);
            status = setBit(status, 1, false);
            requestInt = getBit(status, 3);
            if(isCGB && cpu->isHalted())
                mem->doHDMA();
        }
    }

    if(requestInt && mode != currMode) cpu->requestInterrupt(1);

    mem->store8(0xff41, status);
}

bool c_ppu::lcdEnabled(){
    return getBit(mem->load8(0xff40), 7);
}

void c_ppu::drawScanLine(){
    u8 status = mem->load8(0xff41);
    if(mem->load8(0xff44) == mem->load8(0xff45)){
        status = setBit(status, 2, true);
        if(getBit(status, 6))
            cpu->requestInterrupt(1);
    }else{
        status = setBit(status, 2, false);
    }
    mem->store8(0xff41, status);

    u8 ctrl = mem->load8(0xff40);
    if(getBit(ctrl, 0))
        renderTiles();

    if(getBit(ctrl, 1))
        renderSprites();

}

void c_ppu::renderTiles(){
    u16 tileData = 0;
    u16 bkgndMem = 0;
    bool isUnsigned = true;

    u8 scrollY = mem->load8(0xff42);
    u8 scrollX = mem->load8(0xff43);
    u8 windowY = mem->load8(0xff4a);
    u8 windowX = mem->load8(0xff4b)-7;

    u8 lcdc = mem->load8(0xff40);

    bool usingWindow = false;

    if(getBit(lcdc, 5)){
        if(windowY <= mem->load8(0xff44)){
            usingWindow = true;
        }
    }

    if(getBit(lcdc, 4)){
        tileData = 0x8000;
    }else{
        tileData = 0x8800;
        isUnsigned = false;
    }

    if(!usingWindow){
        if(getBit(lcdc, 3)){
            bkgndMem = 0x9c00;
        }else{
            bkgndMem = 0x9800;
        }
    }else{
        if(getBit(lcdc, 6)){
            bkgndMem = 0x9c00;
        }else{
            bkgndMem = 0x9800;
        }
    }

    u8 yPos = 0;

    if(!usingWindow){
        yPos = scrollY + mem->load8(0xff44);
    }else{
        yPos = mem->load8(0xff44) - windowY;
    }

    u16 tileRow = (yPos >> 3) << 5;

    for(int i = 0; i < 160; i++){
        u8 xPos = i + scrollX;
        if(usingWindow){
            if(i >= windowX)
                xPos = i - windowX;
        }
        u16 tileCol = xPos >> 3;
        s16 tileNum;
        u16 tileAddr = bkgndMem + tileRow + tileCol;

        u8 cgbAttr = 0;
        if(isCGB)
            cgbAttr = mem->loadVBK1(tileAddr);

        if(isUnsigned){
            tileNum = (u8)mem->load8(tileAddr);
        }else{
            tileNum = (s8)mem->load8(tileAddr);
        }

        u16 tileLocation = tileData;

        if(isUnsigned){
            tileLocation += (tileNum << 4);
        }else{
            tileLocation += ((tileNum+128) << 4);
        }
        u8 line = (yPos & 7) << 1;

        u8 row1, row2;
        if(isCGB && getBit(cgbAttr, 3)){
            if(getBit(cgbAttr, 6)){
                row1 = mem->loadVBK1(tileLocation + (14-line));
                row2 = mem->loadVBK1(tileLocation + (15-line));
            }else{
                row1 = mem->loadVBK1(tileLocation + line);
                row2 = mem->loadVBK1(tileLocation + line + 1);
            }
        }else{
            if(isCGB && getBit(cgbAttr, 6)){
                row1 = mem->load8(tileLocation + (14-line));
                row2 = mem->load8(tileLocation + (15-line));
            }else{
                row1 = mem->load8(tileLocation + line);
                row2 = mem->load8(tileLocation + line + 1);
            }
        }

        if(isCGB && getBit(cgbAttr, 5)){
            u8 tmp1, tmp2;
            for(int i = 0; i < 8; i++){
                tmp1 = setBit(tmp1, i, getBit(row1, 7-i));
                tmp2 = setBit(tmp2, i, getBit(row2, 7-i));
            }
            row1 = tmp1;
            row2 = tmp2;
        }

        s32 colorBit  = xPos & 7;
            colorBit -=  7;
            colorBit *= -1;

        u8 colorNum  = getBit(row2, colorBit) << 1;
           colorNum |= getBit(row1, colorBit);

        u8 color;
        if(!isCGB){
            color = getColor(colorNum, 0xff47);
        }
        else{
            color = ((cgbAttr & 7) << 2) + colorNum;
        }

        int bounds = mem->load8(0xff44);
        if(!isCGB && (bounds >= 0)&&(bounds < 144)&&(i >= 0)&&(i < 160)){
            switch(color){
            case 0b00: screenData[bounds][i] = COLOR_WHITE;     break;
            case 0b01: screenData[bounds][i] = COLOR_LIGHTGRAY; break;
            case 0b10: screenData[bounds][i] = COLOR_DARKGRAY;  break;
            case 0b11: screenData[bounds][i] = COLOR_BLACK;     break;
            }
        }
        else if((bounds >= 0)&&(bounds < 144)&&(i >= 0)&&(i < 160)){
            screenData[bounds][i] = mem->loadcolor(color, false);
        }
    }
}

void c_ppu::renderSprites(){
    u8 lcdc = mem->load8(0xff40);
    u8 currLine = mem->load8(0xff44);

    bool is8x16 = false;
    if(getBit(lcdc, 2))
        is8x16 = true;

    for(int i = 39; i >= 0; i--){
        u8  index = i << 2;
        int yPos = mem->load8(0xfe00+index)  - 16;
        int xPos = mem->load8(0xfe00+index+1) - 8;
        u8  tile = mem->load8(0xfe00+index+2);
        u8  attr = mem->load8(0xfe00+index+3);

        bool behindBG = getBit(attr, 7);
        bool yFlip    = getBit(attr, 6);
        bool xFlip    = getBit(attr, 5);
        u8   pal      = attr & 7;

        u8 height = 8;
        if(is8x16) height = 16;

        if((currLine >= yPos)&&(currLine < (yPos+height))){
            int line = currLine - yPos;
            if(yFlip){
                line -= height;
                line *= -1;
            }
            line <<= 1;

            u16 dataAddr = (0x8000 + (tile << 4))+line;
            u8 data1 = mem->load8(dataAddr);
            u8 data2 = mem->load8(dataAddr+1);

            for(int t = 7; t >= 0; t--){
                int colorBit = t;
                if(xFlip){
                    colorBit -=  7;
                    colorBit *= -1;
                }

                int colorNum  = getBit(data2, colorBit) << 1;
                    colorNum |= getBit(data1, colorBit);

                u16 colorAddr = getBit(attr, 4) ? 0xff49 : 0xff48;

                u8 color = getColor(colorNum, colorAddr);

                if(colorNum){
                    int xPixel = -t;
                    xPixel += 7;

                    int pixel = xPos + xPixel;

                    if(!isCGB && (currLine >= 0)&&(currLine < 144)&&(pixel >= 0)&&(pixel < 160)){
                        if(!(behindBG && screenData[currLine][pixel] != COLOR_WHITE)){
                            switch(color){
                            case WHITE:     screenData[currLine][pixel] = COLOR_WHITE;     break;
                            case LIGHTGRAY: screenData[currLine][pixel] = COLOR_LIGHTGRAY; break;
                            case DARKGRAY:  screenData[currLine][pixel] = COLOR_DARKGRAY;  break;
                            case BLACK:     screenData[currLine][pixel] = COLOR_BLACK;     break;
                            }
                        }
                    }
                    else if((currLine >= 0)&&(currLine < 144)&&(pixel >= 0)&&(pixel < 160)){
                        if(!(behindBG && screenData[currLine][pixel] != COLOR_WHITE))
                            screenData[currLine][pixel] = mem->loadcolor(colorNum + (pal << 2), true);
                    }
                }
            }

        }
    }
}

u8 c_ppu::getColor(u8 colorNum, u16 address){
    u8  res = WHITE;
    u8  palette = mem->load8(address);
    int hi = 0;
    int lo = 0;

    switch(colorNum){
    case 0: hi = 1 ; lo = 0; break;
    case 1: hi = 3 ; lo = 2; break;
    case 2: hi = 5 ; lo = 4; break;
    case 3: hi = 7 ; lo = 6; break;
    }

    int color = 0;
    color  = getBit(palette, hi) << 1;
    color |= getBit(palette, lo);

    switch(color){
    case 0: res = WHITE;     break;
    case 1: res = LIGHTGRAY; break;
    case 2: res = DARKGRAY;  break;
    case 3: res = BLACK;     break;
    }

    return res;
}
