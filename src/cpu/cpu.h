#ifndef cpu_h
#define cpu_h

#include "../util/util.h"
#include "../types.h"

#include "../memory/memory.h"

#define TIMA 0xFF05
#define TMA  0xFF06
#define TMC  0xFF07

#define CLOCK 4194304

const u8 instructionLength[256] = {
	1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1, // 0
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 1
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 2
	2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 3
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 5
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 7
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // A
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // B
	1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1, // C
	1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1, // D
	2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1, // E
	2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1  // F
};

const u8 instructionTiming[256] = {
	1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
	0, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
	2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
	2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	2, 2, 2, 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
	2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
	3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
	3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4,
};

const u8 CBinstructionTiming[256] = {
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
    2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
};

class c_cpu{
private:
    /* registers */
    struct{
        union{
            u16 af;
            struct{
                union{
                    u8 f;
                    struct{
                        bool    :  4;
                        bool fc :  1;
                        bool fh :  1;
                        bool fn :  1;
                        bool fz :  1;
                    };
                };
                u8 a;
            };
        };
        union{
            u16 bc;
            struct{
                u8 c;
                u8 b;
            };
        };
        union{
            u16 de;
            struct{
                u8 e;
                u8 d;
            };
        };
        union{
            u16 hl;
            struct{
                u8 l;
                u8 h;
            };
        };
        u16 sp;
        u16 pc;
    }reg;

    struct{
        int cycles;
        u64 cyclesRun;
        int divideReg;
        int timerCounter;
				int timaCycles;
    }timing;

    class c_mem* mem;

    u8 currentInstruction;
    u8 cbInstruction;
    bool isBranch;
    bool interruptEnabled;
    bool interruptMasterEnable;
    bool halted;
    bool stopped;
    bool isCGB;

    bool haltBug;

    bool debug;

    /* stack operations */
    void push8(u8 data);
    u8 pop8();
    void push16(u16 data);
    u16 pop16();

    /* instructions */
    void op_NOP();
    void op_LD(u16* dst, u16 data);
    void op_LD(u16 dst, u16 src);
    void op_LD(u16 dst, u8 src);
    void op_LD(u8* dst, u8 data);
    void op_LD(u8* dst, u16 addr);
    void op_LDH(u8* dst, u8 addr);
    void op_LDH(u8 addr, u8 src);
    void op_LD_HL_SP_n();
    void op_PUSH(u16 src);
    void op_POP(u16* dst);
    void op_INC(u16* src);
    void op_INC(u16 addr);
    void op_INC(u8* src);
    void op_DEC(u16* src);
    void op_DEC(u16 addr);
    void op_DEC(u8* src);
    void op_ADD(u8* dst, u8 src);
    void op_ADD_HL(u16 src);
    void op_ADD_SP(s8 offset);
    void op_SUB(u8* dst, u8 src);
    void op_ADC(u8* dst, u8 src);
    void op_SBC(u8* dst, u8 src);
    void op_RLC(u8* src);
    void op_RLC(u16 src);
    void op_RRC(u8* src);
    void op_RRC(u16 src);
    void op_RL(u8* src);
    void op_RL(u16 src);
    void op_RR(u8* src);
    void op_RR(u16 src);
    void op_SLA(u8* src);
    void op_SLA(u16 src);
    void op_SRA(u8* src);
    void op_SRA(u16 src);
    void op_SRL(u8* src);
    void op_SRL(u16 src);
    void op_STOP();
    void op_HALT();
    void op_DAA();
    void op_JR(bool cond);
    void op_JP(bool cond, u16 addr);
    void op_CALL(bool cond);
    void op_RET(bool cond);
    void op_RETI();
    void op_RST(u8 addr);
    void op_SCF();
    void op_CCF();
    void op_EI();
    void op_DI();
    void op_CPL();
    void op_AND(u8* dst, u8 src);
    void op_XOR(u8* dst, u8 src);
    void op_OR(u8* dst, u8 src);
    void op_CP(u8* dst, u8 src);
    void op_SWAP(u8* src);
    void op_SWAP(u16 src);
    void op_BIT(u8* src, u8 bit);
    void op_BIT(u16 src, u8 bit);
    void op_SET(u8* src, u8 bit);
    void op_SET(u16 src, u8 bit);
    void op_RES(u8* src, u8 bit);
    void op_RES(u16 src, u8 bit);

    void op_CB();

    void op_UNDEFINED();

    void serviceInterrupt(int interrupt);

    bool clockEnabled();
    void doDivideRegister(int cycles);
public:
     c_cpu();
    ~c_cpu();
    void getComponents(c_mem* memPtr);

    void reset(bool biosLoaded, bool cgb);
    int  tick();

    void requestInterrupt(u8 interrupt);
    void doInterrupts();
    void endStop();
    bool isHalted();

    void updateTimers(int cycles);
    void setClockFrequency();

};

#endif // cpu_h
