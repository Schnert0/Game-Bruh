#include "input.h"

c_input::c_input(){
    memset(&curr, 0, sizeof(s_buttons));
    memset(&prev, 0, sizeof(s_buttons));

    joypadState = 0;

    for(int i = 0; i < 8; i++){
        keyReleased(i);
    }

}

c_input::~c_input(){
}

void c_input::getComponents(c_mem* memPtr, c_cpu* cpuPtr){
    mem = memPtr;
    cpu = cpuPtr;
}

u8 c_input::getInput(){
    u8 state = STATE_RUNNING;

    memcpy(&prev, &curr, sizeof(s_buttons));

    s8 key = -1;

    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                state = STATE_DONE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_x:      key = 4; break;
                case SDLK_z:      key = 5; break;
                case SDLK_RETURN: key = 7; break;
                case SDLK_RSHIFT: key = 6; break;
                case SDLK_RIGHT:  key = 0; break;
                case SDLK_LEFT:   key = 1; break;
                case SDLK_UP:     key = 2; break;
                case SDLK_DOWN:   key = 3; break;
                case SDLK_r:      state = STATE_RESET; break;
                }
                if(key != -1)
                keyPressed(key);
                break;

            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                case SDLK_x:      key = 4; break;
                case SDLK_z:      key = 5; break;
                case SDLK_RETURN: key = 7; break;
                case SDLK_RSHIFT: key = 6; break;
                case SDLK_RIGHT:  key = 0; break;
                case SDLK_LEFT:   key = 1; break;
                case SDLK_UP:     key = 2; break;
                case SDLK_DOWN:   key = 3; break;
                }
                if(key != -1)
                    keyReleased(key);
                break;
        }
    }

    return state;

}


u8 c_input::getJoypadInput(){
    u8 res = ~mem->joyInput();

    if(!getBit(res, 4)){
        u8 topJoypad = joypadState >> 4;
        topJoypad |= 0xf0;
        res &= topJoypad;
    }else{
       u8 botJoypad = joypadState & 0xf;
        botJoypad |= 0xf0;
        res &= botJoypad;
    }

    return res;
}


void c_input::keyPressed(int key){
    bool prevUnset = false;
    if(!getBit(joypadState, key))
        prevUnset = true;

    joypadState = setBit(joypadState, key, false);

    bool button;
    if(key > 3)
        button = true;
    else
        button = false;

    u8 keyReq = mem->load8(0xff00);
    bool requestInt = false;

    if(button && !getBit(keyReq, 5))
        requestInt = true;

    if(!button && !getBit(keyReq, 4))
        requestInt = true;

    if(requestInt && !prevUnset)
        cpu->requestInterrupt(4);

    cpu->endStop();
}

void c_input::keyReleased(int key){
    joypadState = setBit(joypadState, key, true);
}
