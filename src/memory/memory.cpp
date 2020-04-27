#include "memory.h"

c_mem::c_mem(){
    fileName = NULL;
    romBuffer = new u8[0x8000];
    memset(romBuffer, 0xff, 0x8000);
    saveExists = false;
    biosLoaded = false;
    hdmaActivated = true;
}

c_mem::~c_mem(){
    if(romBuffer){
        delete romBuffer;
    }
}

void c_mem::getComponents(c_cpu* cpuPtr, c_input* inputPtr){
    cpu = cpuPtr;
    input = inputPtr;
}

void c_mem::reset(){
    memset(memory, 0xff, 0x8000);
    memory[0xFF00] = 0x0F;
    memory[0xFF05] = 0x00;
    memory[0xFF06] = 0x00;
    memory[0xFF07] = 0x00;
    memory[0xFF10] = 0x80;
    memory[0xFF11] = 0xBF;
    memory[0xFF12] = 0xF3;
    memory[0xFF14] = 0xBF;
    memory[0xFF16] = 0x3F;
    memory[0xFF17] = 0x00;
    memory[0xFF19] = 0xBF;
    memory[0xFF1A] = 0x7F;
    memory[0xFF1B] = 0xFF;
    memory[0xFF1C] = 0x9F;
    memory[0xFF1E] = 0xBF;
    memory[0xFF20] = 0xFF;
    memory[0xFF21] = 0x00;
    memory[0xFF22] = 0x00;
    memory[0xFF23] = 0xBF;
    memory[0xFF24] = 0x77;
    memory[0xFF25] = 0xF3;
    memory[0xFF26] = 0xF1;
    memory[0xFF40] = 0x00;
    memory[0xFF41] = 0x84;
    memory[0xFF42] = 0x00;
    memory[0xFF43] = 0x00;
    memory[0xFF45] = 0x00;
    memory[0xFF47] = 0xFC;
    memory[0xFF48] = 0xFF;
    memory[0xFF49] = 0xFF;
    memory[0xFF4A] = 0x00;
    memory[0xFF4B] = 0x00;
    if(biosLoaded)
        memory[0xFF50] = 0x00;
    else
        memory[0xFF50] = 0x01;
    memory[0xFF55] = 0x80;
    memory[0xFFFF] = 0x00;
}

bool c_mem::loadROM(const char* path){
        FILE* f = fopen(path, "rb");
        if(f){
            /* gets the size of the ROM */
            fseek(f, 0, SEEK_END);
            romSize = ftell(f);
            fseek(f, 0, SEEK_SET);

            printf("ROM is %d bytes\n", romSize);

            if(romBuffer) delete romBuffer;
            romBuffer = new u8[romSize];

            fread(romBuffer, romSize, 1, f);
            fclose(f);

            fileName = (char*)malloc(strlen(path));
            memcpy(fileName, path, strlen(path));

            switch(romBuffer[0x149]){
            case 0x00:
            case 0x01:
            case 0x02: ramSize =  1; break;
            case 0x04: ramSize = 16; break;
            default:   ramSize =  4; break;
            }
            ramSize *= 0x2000;

            ramBuffer = new u8[ramSize];

             romBankNumber = 1;
             ramBankNumber = 0;
            wramBankNumber = 0;
            vramBankNumber = 0;

            printf("ROM loaded successfully\n");
            return true;
        }

        printf("ROM ERROR: could not open ROM\n");
        return false;
}

bool c_mem::loadBIOS(const char* path, bool cgb){
    isCGB = cgb;
    if(!isCGB){
        printf("loading BIOS...\n");
        FILE* file = fopen(path, "rb");
        if(!file){
            printf("ERROR: unable to open \"%s\"\n", path);
            return false;
        }
        fread(bios, 0x100, 1, file);
        fclose(file);

        biosLoaded = true;
        printf("BIOS loaded successfully\n");
        return true;
    }else{
        printf("loading CGB BIOS...\n");
        FILE* file = fopen(path, "rb");
        if(!file){
            printf("ERROR: unable to open \"%s\"\n", path);
            return false;
        }
        fread(bios, 0xc00, 1, file);
        fclose(file);

        biosLoaded = true;
        printf("CGB BIOS loaded successfully\n");
        return true;
    }
}

u8 c_mem::load8(u16 address){
    if(address < 0x4000){
        if((address < 0x100 && !memory[0xff50]))
            return bios[address];
        else if(isCGB && (address >= 0x200 && address < 0xc00) && !memory[0xff50])
            return bios[address];
        return romBuffer[address];

    }
    else if((address >= 0x4000) && (address < 0x8000)){
        if(romBankNumber* 0x4000 < romSize)
        return romBuffer[(address - 0x4000) + (romBankNumber*0x4000)];
    }
    else if((address >= 0x8000) && (address < 0xa000)){
        return vram[(address - 0x8000) + (vramBankNumber*0x2000)];
    }
    else if((address >= 0xa000) && (address < 0xc000)){
        return ramBuffer[(address - 0xa000) + (ramBankNumber*0x2000)];
    }
    else if(isCGB && (address >= 0xd000) && (address < 0xe000)){
        return wram[(address - 0xd000)+(wramBankNumber*0x1000)];
    }
    else if(address >= 0xff00){
        switch(address){
        case 0xff00: return input->getJoypadInput(); break;
        }
    }
    return memory[address];
}

void c_mem::store8(u16 address, u8 data){
    //printf("0x%04x <= 0x%02x\n", address, data);
    if(address < 0x8000){
        if(address >= 0x2000 && address < 0x4000){
            romBankNumber = data;
        }
        else if(address >= 0x4000 && address < 0x6000){
            if(data < 4)
                ramBankNumber = data;
            else if(data >= 8 && data < 0xd)
                printf("warning: write to unimplemented RTC\n");
        }
    }
    else if((address >= 0x8000) && (address < 0xa000)){
        vram[(address - 0x8000) + (vramBankNumber*0x2000)] = data;
    }
    else if((address >= 0xa000) && (address < 0xc000)){
        ramBuffer[(address - 0xa000) + (ramBankNumber*0x2000)] = data;
    }
    else if(isCGB && (address >= 0xd000) && (address < 0xe000)){
        wram[(address - 0xd000)+(wramBankNumber*0x1000)] = data;
    }
    else if((address >= 0xe000) && (address < 0xfe00)){
        memory[address] = data;
        memory[address - 0x2000] = data;
    }
    else if((address >= 0xfea0) && (address < 0xff00)){

    }
    else if((address >= 0xff00) && (address < 0xff80)){
        u8 currFreq;
        u8 tmp;
        switch(address){
        case 0xff02: if(data == 0x81) printf("%c", memory[0xff01]); break;
        case 0xff04: memory[0xff04] = 0; break;
        case 0xff07:
            currFreq = memory[0xff07] & 3;
            memory[0xff07] = data;
            if(currFreq != memory[0xff07]){
                cpu->setClockFrequency();
            }
            break;
        case 0xff44: memory[address] = 0; break;
        case 0xff46: dmaTransfer(data); break;
        case 0xff4f:
            if(isCGB)
                vramBankNumber = data;
            memory[0xff4f] = data;
            break;

        //case 0xff51 ... 0xff55: printf("0x%04x: 0x%02x\n", address, data); break;

        //case 0xff68: printf("BCPS = 0x%02x\n", data); memory[0xff68] = data; break;
        case 0xff69:
            //printf("BCPD = 0x%02x\n", data);
            paletteBG[memory[0xff68]&0x3f] = data;
            if(memory[0xff68] & 0x80){ // increments the only the first 6 bits of ff68
                tmp = memory[0xff68];
                tmp = (tmp+1) & 0x3f;
                memory[0xff68] &= ~0x3f;
                memory[0xff68] |= tmp;
            }
            break;
        case 0xff6b:
            paletteSP[(memory[0xff6a]&0x3f)] = data;
            if(memory[0xff6a] & 0x80){
                tmp = memory[0xff6a];
                tmp = (tmp+1) & 0x3f;
                memory[0xff6a] &= ~0x3f;
                memory[0xff6a] |= tmp;
            }
            break;
        case 0xff70:
            //printf("wramBank: %d\n", data);
            if(isCGB)
                wramBankNumber = data;
            memory[0xff70] = data;
            break;
        default: memory[address] = data;
        }
    }
    else{
        memory[address] = data;
    }
}

u16 c_mem::load16(u16 address){
    return ( load8(address) ) | ( load8(address+1) << 8 );
}

void c_mem::store16(u16 address, u16 data){
    store8(address,   data & 0xff);
    store8(address+1, data >> 8);
}

void c_mem::dmaTransfer(u8 data){
    u16 addr = data << 8;
    for(int i = 0; i < 0xa0; i++){
        store8(0xfe00+i, load8(addr+i));
    }
}

void c_mem::incDivider(){
    memory[0xff04]++;
}

u8 c_mem::joyInput(){
    return memory[0xff00];
}

void c_mem::save(){
    if(!saveExists) return;

    printf("MEM: saving game...\n");
    for(int i = 0xa000; i < 0xc000; i++){
        if(memory[i] != 0x00){
            char* path = (char*)malloc(strlen(fileName));
            memcpy(path, fileName, strlen(fileName));
            strcat(path, ".sav");


            FILE* file = fopen(path, "wb");
            fwrite(memory+0xa000, 1, 0x2000, file);
            fclose(file);
            printf("MEM: saved!\n");

            free(path);

            return;
        }
    }

    printf("MEM: no save data written\n");
}

void c_mem::load(){
    printf("MEM: loading save...\n");

    char* path = (char*)malloc(strlen(fileName));
    memcpy(path, fileName, strlen(fileName));
    strcat(path, ".sav");

    FILE* file = fopen(path, "rb");
    if(file && path){
        fread(memory+0xa000, 1, 0x2000, file);
        fclose(file);
        saveExists = true;
        printf("MEM: save loaded\n");
    }else{
        memset(memory+0xa000, 0xff, 0x2000);
        printf("MEM: no save found\n");
    }

    free(path);
}


u16 c_mem::loadcolor(u8 color, bool sprite){
    u8 index = color << 1;
    if(sprite)
        return paletteSP[index] | (paletteSP[index+1] << 8);
    return paletteBG[index] | (paletteBG[index+1] << 8);
}

void c_mem::doHDMA(){
    if(!(memory[0xff55] >> 7)) return;
    //getchar();

    if(hdmaActivated){
        hdmaSrc = (memory[0xff51] << 8) | (memory[0xff52] & 0xf0);
        hdmaDst = ((memory[0xff53] & 0x1f) << 8) | (memory[0xff54] & 0xf0) | 0x8000;
        hdmaSize =  memory[0xff55] & 0x7f;

        //printf("HDMA activated\n");

        //printf("src:\t0x%04x\n", hdmaSrc);
        //printf("dst:\t0x%04x\n", hdmaDst);
        //printf("size:\t0x%02x\n", hdmaSize);
    }

    for(int i = 0; i < 0x10; i++){
        store8(hdmaDst+i+(hdmaCycles*0x10), load8(hdmaSrc+i+(hdmaCycles*0x10)));
        //printf("0x%04x <= 0x%04x\n", hdmaDst+i+(hdmaCycles*0x10), hdmaSrc+i+(hdmaCycles*0x10));
    }
    hdmaDst += 0x10;
    hdmaSrc += 0x10;
    hdmaSize--;
    if(hdmaSize == 0){
        memory[0xff55] |= 0x80;
        hdmaActivated = true;
    }
}

u8 c_mem::loadVBK1(u16 addr){
    return vram[0x2000 + (addr - 0x8000)];
}
