#ifndef input_h
#define input_h

#include <string.h>
#include <SDL.h>
#include "../util/util.h"

#include "../memory/memory.h"
#include "../cpu/cpu.h"

class c_mem;

enum{
    STATE_DONE,
    STATE_RUNNING,
    STATE_RESET
};

typedef union{
    struct{
        bool right;
        bool left;
        bool up;
        bool down;
        bool a;
        bool b;
        bool start;
        bool select;
    };
    bool buttons[8];
}s_buttons;

class c_input{
private:
    c_mem* mem;
    class c_cpu* cpu;
    s_buttons curr, prev;

    u8 joypadState;

public:
     c_input();
    ~c_input();
    void getComponents(c_mem* memPtr, c_cpu* cpuPtr);

    u8 getInput();
    u8 getJoypadInput();
    void keyPressed(int key);
    void keyReleased(int key);

};

#endif // input_h
