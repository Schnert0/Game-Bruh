#include "util.h"

u8 setBit(u8 byte, u8 bit, bool on){
    if(on){
        byte |= (1 << bit);
    }else{
        byte = byte & ~(1 << bit);
    }
    return byte;
}

bool getBit(u8 byte, u8 bit){
    return (byte & (1 << bit)) != 0;
}
