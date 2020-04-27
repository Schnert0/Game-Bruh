#include <stdio.h>

#include "memory.h"
#include "cpu.h"

const char* opcodeNames[256] = {
    "NOP",
    "LD BC,nn",
    "LD (BC),A",
    "INC BC",
    "INC B",
    "DEC B",
    "LD B,n",
    "RLCA",
    "LD (nn),SP",
    "ADD HL,BC",
    "LD A,(BC)",
    "DEC BC",
    "INC C",
    "DEC C",
    "LD C,n",
    "RRCA",

    "STOP",
    "LD DE,nn",
    "LD (DE),A",
    "INC DE",
    "INC D",
    "DEC D",
    "LD D,n",
    "RLA",
    "JR n",
    "ADD HL,DE",
    "LD A,(DE)",
    "DEC DE",
    "INC E",
    "DEC E",
    "LD E,n",
    "RRA",

    "JR NZ,n",
    "LD HL,nn",
    "LD (HLI),A",
    "INC HL",
    "INC H",
    "DEC H",
    "LD H,n",
    "DAA",
    "JR Z,n",
    "ADD HL,HL",
    "LD A,(HLI)",
    "DEC HL",
    "INC L",
    "DEC L",
    "LD L,n",
    "CPL",

    "JR NC,n",
    "LD SP,nn",
    "LD (HLD),A",
    "INC SP",
    "INC (HL)",
    "DEC (HL)",
    "LD (HL),n",
    "SCF",
    "JR C,n",
    "ADD HL,SP",
    "LD A,(HLD)",
    "DEC SP",
    "INC A",
    "DEC A",
    "LDA,n",
    "CCF",

    "LD B,B",
    "LD B,C",
    "LD B,D",
    "LD B,E",
    "LD B,H",
    "LD B,L",
    "LD B,(HL)",
    "LD B,A",
    "LD C,B",
    "LD C,C",
    "LD C,D",
    "LD C,E",
    "LD C,H",
    "LD C,L",
    "LD C,(HL)",
    "LD C,A",

    "LD D,B",
    "LD D,C",
    "LD D,D",
    "LD D,E",
    "LD D,H",
    "LD D,L",
    "LD D,(HL)",
    "LD D,A",
    "LD E,B",
    "LD E,C",
    "LD E,D",
    "LD E,E",
    "LD E,H",
    "LD E,L",
    "LD E,(HL)",
    "LD E,A",

    "LD H,B",
    "LD H,C",
    "LD H,D",
    "LD H,E",
    "LD H,H",
    "LD H,L",
    "LD H,(HL)",
    "LD H,A",
    "LD L,B",
    "LD L,C",
    "LD L,D",
    "LD L,E",
    "LD L,H",
    "LD L,L",
    "LD L,(HL)",
    "LD L,A",

    "LD (HL),B",
    "LD (HL),C",
    "LD (HL),D",
    "LD (HL),E",
    "LD (HL),H",
    "LD (HL),L",
    "HALT",
    "LD (HL),A",
    "LD A,B",
    "LD A,C",
    "LD A,D",
    "LD A,E",
    "LD A,H",
    "LD A,L",
    "LD A,(HL)",
    "LD A,A",

    "ADD A,B",
    "ADD A,C",
    "ADD A,D",
    "ADD A,E",
    "ADD A,H",
    "ADD A,L",
    "ADD A,(HL)",
    "ADD A,A",
    "ADC A,B",
    "ADC A,C",
    "ADC A,D",
    "ADC A,E",
    "ADC A,H",
    "ADC A,L",
    "ADC A,(HL)",
    "ADC A,A",

    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB (HL)",
    "SUB A",
    "SBC A,B",
    "SBC A,C",
    "SBC A,D",
    "SBC A,E",
    "SBC A,H",
    "SBC A,L",
    "SBC A,(HL)",
    "SBC A,A",

    "AND B",
    "AND C",
    "AND D",
    "AND E",
    "AND H",
    "AND L",
    "AND (HL)",
    "AND A",
    "XOR B",
    "XOR C",
    "XOR D",
    "XOR E",
    "XOR H",
    "XOR L",
    "XOR (HL)",
    "XOR A",

    "OR B",
    "OR C",
    "OR D",
    "OR E",
    "OR H",
    "OR L",
    "OR (HL)",
    "OR A",
    "CP B",
    "CP C",
    "CP D",
    "CP E",
    "CP H",
    "CP L",
    "CP (HL)",
    "CP A",

    "RET NZ",
    "POP BC",
    "JP NZ,nn",
    "JP nn",
    "CALL NZ,nn",
    "PUSH BC",
    "ADD A,n",
    "RST ",
    "RET Z",
    "RET",
    "JP Z,nn",
    "cb opcode",
    "CALL Z,nn",
    "CALL nn",
    "ADC A,n",
    "RST 0x08",

    "RET NC",
    "POP DE",
    "JP NC,nn",
    "unused opcode",
    "CALL NC,nn",
    "PUSH DE",
    "SUB n",
    "RST 0x10",
    "RET C",
    "RETI",
    "JP C,nn",
    "unused opcode",
    "CALL C,nn",
    "unused opcode",
    "SBC A,n",
    "RST 0x18",

    "LD (0xFF00+n),A",
    "POP HL",
    "LD (0xFF00+C),A",
    "unused opcode",
    "unused opcode",
    "PUSH HL",
    "AND n",
    "RST 0x20",
    "ADD SP,n",
    "JP (HL)",
    "LD (nn),A",
    "unused opcode",
    "unused opcode",
    "unused opcode",
    "XOR n",
    "RST 0x28",

    "LD A,(0xFF00+n)",
    "POP AF",
    "LD A,(0xFF00+C)",
    "DI",
    "unused opcode",
    "PUSH AF",
    "OR n",
    "RST 0x30",
    "LD HL,SP",
    "LD SP,HL",
    "LD A,(nn)",
    "EI",
    "unused opcode",
    "unused opcode",
    "CP n",
    "RST 0x38"
};

c_cpu::c_cpu(){
    debug = false;
}

c_cpu::~c_cpu(){
}

void c_cpu::getComponents(c_mem* memPtr){
    mem = memPtr;
}

void c_cpu::reset(bool biosLoaded, bool cgb){
    isCGB = cgb;

    if(isCGB)
        reg.af = 0x11b0;
    else;
        reg.af = 0x01b0;

    reg.bc = 0x0013;
    reg.de = 0x00d8;
    reg.hl = 0x014d;
    reg.sp = 0xfffe;

    if(biosLoaded){
        reg.pc = 0x0000;
        mem->store8(0xff50, 0);
    }else{
        reg.pc = 0x0100;
        mem->store8(0xff50, 1);
    }

    memset(&reg, 0, sizeof(reg));

    timing.cyclesRun = 0;
    timing.cycles = 0;
    timing.divideReg = 0;
    timing.timerCounter = 0;

    interruptMasterEnable = true;

    haltBug = false;

    currentInstruction = 0;
    cbInstruction = 0;
}

void c_cpu::push8(u8 data){
    reg.sp--;
    mem->store8(reg.sp, data);
}

u8 c_cpu::pop8(){
    u8 data = mem->load8(reg.sp);
    reg.sp++;

    return data;
}

void c_cpu::push16(u16 data){
    push8(data >> 8);
    push8(data & 0xff);
}

u16 c_cpu::pop16(){
    u16 data = pop8();
    data |= pop8() << 8;

    return data;
}

bool c_cpu::isHalted(){
    return halted;
}

void c_cpu::requestInterrupt(u8 interrupt){
    u8 request = mem->load8(0xff0f);
    request = setBit(request, interrupt, true);
    mem->store8(0xff0f, request);
}

void c_cpu::doInterrupts(){
    if(interruptMasterEnable){
        u8 request = mem->load8(0xff0f);
        u8 enabled = mem->load8(0xffff);
        if(request != 0){
            for(int i = 0; i < 5; i++){
                if(getBit(request, i) && getBit(enabled, i))
                    serviceInterrupt(i);
            }
        }

    }
}

void c_cpu::serviceInterrupt(int interrupt){
    interruptMasterEnable = false;

    // turns off the interrupt being serviced
    u8 request = mem->load8(0xff0f);
    request = setBit(request, interrupt, false);
    mem->store8(0xff0f, request);

    push16(reg.pc);

    switch(interrupt){
    case 0: reg.pc = 0x40; break;
    case 1: reg.pc = 0x48; break;
    case 2: reg.pc = 0x50; break;
    case 4: reg.pc = 0x60; break;
    }

    halted = false;
}


void c_cpu::endStop(){
    stopped = false;
}


void c_cpu::updateTimers(int cycles){
    doDivideRegister(cycles);

    if(clockEnabled()){
        timing.timaCycles += cycles;
        setClockFrequency();
        while(timing.timaCycles >= timing.timerCounter){
            timing.timaCycles -= timing.timerCounter;
            u8 tima = mem->load8(TIMA);
            if(tima == 0xff){
              tima = mem->load8(TMA);
              requestInterrupt(2);
            }else{
              tima++;
            }
            mem->store8(TIMA, tima);
        }
    }
}


void c_cpu::doDivideRegister(int cycles){
    timing.divideReg += cycles;
    u32 divCycles = 256;
    while(timing.divideReg >= divCycles){
        timing.divideReg -= divCycles;
        mem->incDivider();
    }
}

bool c_cpu::clockEnabled(){
    return getBit(mem->load8(TMC), 2);
}

void c_cpu::setClockFrequency(){
    u8 freq = mem->load8(TMC) & 3;

    switch(freq){
    case 0: timing.timerCounter = 1024; break;
    case 1: timing.timerCounter =   16; break;
    case 2: timing.timerCounter =   64; break;
    case 3: timing.timerCounter =  256; break;
    }

    //timing.timerCounter <<= 2;
}

int c_cpu::tick(){
    if(stopped || halted) return 4;

    currentInstruction = mem->load8(reg.pc);

    switch(currentInstruction){
    case 0x00: op_NOP();                              break;
    case 0x01: op_LD(&reg.bc, mem->load16(reg.pc+1)); break;
    case 0x02: op_LD(reg.bc, reg.a);                  break;
    case 0x03: op_INC(&reg.bc);                       break;
    case 0x04: op_INC(&reg.b);                        break;
    case 0x05: op_DEC(&reg.b);                        break;
    case 0x06: op_LD(&reg.b, mem->load8(reg.pc+1));   break;
    case 0x07: op_RLC(&reg.a); reg.fz = 0;            break;
    case 0x08: op_LD(mem->load16(reg.pc+1), reg.sp);  break;
    case 0x09: op_ADD_HL(reg.bc);                     break;
    case 0x0a: op_LD(&reg.a, reg.bc);                 break;
    case 0x0b: op_DEC(&reg.bc);                       break;
    case 0x0c: op_INC(&reg.c);                        break;
    case 0x0d: op_DEC(&reg.c);                        break;
    case 0x0e: op_LD(&reg.c, mem->load8(reg.pc+1));   break;
    case 0x0f: op_RRC(&reg.a); reg.fz = 0;            break;

    case 0x10: op_STOP();                             break;
    case 0x11: op_LD(&reg.de, mem->load16(reg.pc+1)); break;
    case 0x12: op_LD(reg.de, reg.a);                  break;
    case 0x13: op_INC(&reg.de);                       break;
    case 0x14: op_INC(&reg.d);                        break;
    case 0x15: op_DEC(&reg.d);                        break;
    case 0x16: op_LD(&reg.d, mem->load8(reg.pc+1));   break;
    case 0x17: op_RL(&reg.a); reg.fz = 0;             break;
    case 0x18: op_JR(true);                           break;
    case 0x19: op_ADD_HL(reg.de);                     break;
    case 0x1a: op_LD(&reg.a, reg.de);                 break;
    case 0x1b: op_DEC(&reg.de);                       break;
    case 0x1c: op_INC(&reg.e);                        break;
    case 0x1d: op_DEC(&reg.e);                        break;
    case 0x1e: op_LD(&reg.e, mem->load8(reg.pc+1));   break;
    case 0x1f: op_RR(&reg.a); reg.fz = 0;             break;

    case 0x20: op_JR(!reg.fz);                        break;
    case 0x21: op_LD(&reg.hl, mem->load16(reg.pc+1)); break;
    case 0x22: op_LD(reg.hl, reg.a); reg.hl++;        break;
    case 0x23: op_INC(&reg.hl);                       break;
    case 0x24: op_INC(&reg.h);                        break;
    case 0x25: op_DEC(&reg.h);                        break;
    case 0x26: op_LD(&reg.h, mem->load8(reg.pc+1));   break;
    case 0x27: op_DAA();                              break;
    case 0x28: op_JR(reg.fz);                         break;
    case 0x29: op_ADD_HL(reg.hl);                     break;
    case 0x2a: op_LD(&reg.a, reg.hl); reg.hl++;       break;
    case 0x2b: op_DEC(&reg.hl);                       break;
    case 0x2c: op_INC(&reg.l);                        break;
    case 0x2d: op_DEC(&reg.l);                        break;
    case 0x2e: op_LD(&reg.l, mem->load8(reg.pc+1));   break;
    case 0x2f: op_CPL();                              break;

    case 0x30: op_JR(!reg.fc);                        break;
    case 0x31: op_LD(&reg.sp, mem->load16(reg.pc+1)); break;
    case 0x32: op_LD(reg.hl, reg.a); reg.hl--;        break;
    case 0x33: op_INC(&reg.sp);                       break;
    case 0x34: op_INC(reg.hl);                        break;
    case 0x35: op_DEC(reg.hl);                        break;
    case 0x36: op_LD(reg.hl, mem->load8(reg.pc+1));   break;
    case 0x37: op_SCF();                              break;
    case 0x38: op_JR(reg.fc);                         break;
    case 0x39: op_ADD_HL(reg.sp);                     break;
    case 0x3a: op_LD(&reg.a, reg.hl); reg.hl--;       break;
    case 0x3b: op_DEC(&reg.sp);                       break;
    case 0x3c: op_INC(&reg.a);                        break;
    case 0x3d: op_DEC(&reg.a);                        break;
    case 0x3e: op_LD(&reg.a, mem->load8(reg.pc+1));   break;
    case 0x3f: op_CCF();                              break;

    case 0x40: op_LD(&reg.b, reg.b);  break;
    case 0x41: op_LD(&reg.b, reg.c);  break;
    case 0x42: op_LD(&reg.b, reg.d);  break;
    case 0x43: op_LD(&reg.b, reg.e);  break;
    case 0x44: op_LD(&reg.b, reg.h);  break;
    case 0x45: op_LD(&reg.b, reg.l);  break;
    case 0x46: op_LD(&reg.b, reg.hl); break;
    case 0x47: op_LD(&reg.b, reg.a);  break;
    case 0x48: op_LD(&reg.c, reg.b);  break;
    case 0x49: op_LD(&reg.c, reg.c);  break;
    case 0x4a: op_LD(&reg.c, reg.d);  break;
    case 0x4b: op_LD(&reg.c, reg.e);  break;
    case 0x4c: op_LD(&reg.c, reg.h);  break;
    case 0x4d: op_LD(&reg.c, reg.l);  break;
    case 0x4e: op_LD(&reg.c, reg.hl); break;
    case 0x4f: op_LD(&reg.c, reg.a);  break;

    case 0x50: op_LD(&reg.d, reg.b);  break;
    case 0x51: op_LD(&reg.d, reg.c);  break;
    case 0x52: op_LD(&reg.d, reg.d);  break;
    case 0x53: op_LD(&reg.d, reg.e);  break;
    case 0x54: op_LD(&reg.d, reg.h);  break;
    case 0x55: op_LD(&reg.d, reg.l);  break;
    case 0x56: op_LD(&reg.d, reg.hl); break;
    case 0x57: op_LD(&reg.d, reg.a);  break;
    case 0x58: op_LD(&reg.e, reg.b);  break;
    case 0x59: op_LD(&reg.e, reg.c);  break;
    case 0x5a: op_LD(&reg.e, reg.d);  break;
    case 0x5b: op_LD(&reg.e, reg.e);  break;
    case 0x5c: op_LD(&reg.e, reg.h);  break;
    case 0x5d: op_LD(&reg.e, reg.l);  break;
    case 0x5e: op_LD(&reg.e, reg.hl); break;
    case 0x5f: op_LD(&reg.e, reg.a);  break;

    case 0x60: op_LD(&reg.h, reg.b);  break;
    case 0x61: op_LD(&reg.h, reg.c);  break;
    case 0x62: op_LD(&reg.h, reg.d);  break;
    case 0x63: op_LD(&reg.h, reg.e);  break;
    case 0x64: op_LD(&reg.h, reg.h);  break;
    case 0x65: op_LD(&reg.h, reg.l);  break;
    case 0x66: op_LD(&reg.h, reg.hl); break;
    case 0x67: op_LD(&reg.h, reg.a);  break;
    case 0x68: op_LD(&reg.l, reg.b);  break;
    case 0x69: op_LD(&reg.l, reg.c);  break;
    case 0x6a: op_LD(&reg.l, reg.d);  break;
    case 0x6b: op_LD(&reg.l, reg.e);  break;
    case 0x6c: op_LD(&reg.l, reg.h);  break;
    case 0x6d: op_LD(&reg.l, reg.l);  break;
    case 0x6e: op_LD(&reg.l, reg.hl); break;
    case 0x6f: op_LD(&reg.l, reg.a);  break;

    case 0x70: op_LD(reg.hl, reg.b);  break;
    case 0x71: op_LD(reg.hl, reg.c);  break;
    case 0x72: op_LD(reg.hl, reg.d);  break;
    case 0x73: op_LD(reg.hl, reg.e);  break;
    case 0x74: op_LD(reg.hl, reg.h);  break;
    case 0x75: op_LD(reg.hl, reg.l);  break;
    case 0x76: op_HALT();             break;
    case 0x77: op_LD(reg.hl, reg.a);  break;
    case 0x78: op_LD(&reg.a, reg.b);  break;
    case 0x79: op_LD(&reg.a, reg.c);  break;
    case 0x7a: op_LD(&reg.a, reg.d);  break;
    case 0x7b: op_LD(&reg.a, reg.e);  break;
    case 0x7c: op_LD(&reg.a, reg.h);  break;
    case 0x7d: op_LD(&reg.a, reg.l);  break;
    case 0x7e: op_LD(&reg.a, reg.hl); break;
    case 0x7f: op_LD(&reg.a, reg.a);  break;

    case 0x80: op_ADD(&reg.a, reg.b);              break;
    case 0x81: op_ADD(&reg.a, reg.c);              break;
    case 0x82: op_ADD(&reg.a, reg.d);              break;
    case 0x83: op_ADD(&reg.a, reg.e);              break;
    case 0x84: op_ADD(&reg.a, reg.h);              break;
    case 0x85: op_ADD(&reg.a, reg.l);              break;
    case 0x86: op_ADD(&reg.a, mem->load8(reg.hl)); break;
    case 0x87: op_ADD(&reg.a, reg.a);              break;
    case 0x88: op_ADC(&reg.a, reg.b);              break;
    case 0x89: op_ADC(&reg.a, reg.c);              break;
    case 0x8a: op_ADC(&reg.a, reg.d);              break;
    case 0x8b: op_ADC(&reg.a, reg.e);              break;
    case 0x8c: op_ADC(&reg.a, reg.h);              break;
    case 0x8d: op_ADC(&reg.a, reg.l);              break;
    case 0x8e: op_ADC(&reg.a, mem->load8(reg.hl)); break;
    case 0x8f: op_ADC(&reg.a, reg.a);              break;

    case 0x90: op_SUB(&reg.a, reg.b);              break;
    case 0x91: op_SUB(&reg.a, reg.c);              break;
    case 0x92: op_SUB(&reg.a, reg.d);              break;
    case 0x93: op_SUB(&reg.a, reg.e);              break;
    case 0x94: op_SUB(&reg.a, reg.h);              break;
    case 0x95: op_SUB(&reg.a, reg.l);              break;
    case 0x96: op_SUB(&reg.a, mem->load8(reg.hl)); break;
    case 0x97: op_SUB(&reg.a, reg.a);              break;
    case 0x98: op_SBC(&reg.a, reg.b);              break;
    case 0x99: op_SBC(&reg.a, reg.c);              break;
    case 0x9a: op_SBC(&reg.a, reg.d);              break;
    case 0x9b: op_SBC(&reg.a, reg.e);              break;
    case 0x9c: op_SBC(&reg.a, reg.h);              break;
    case 0x9d: op_SBC(&reg.a, reg.l);              break;
    case 0x9e: op_SBC(&reg.a, mem->load8(reg.hl)); break;
    case 0x9f: op_SBC(&reg.a, reg.a);              break;

    case 0xa0: op_AND(&reg.a, reg.b);              break;
    case 0xa1: op_AND(&reg.a, reg.c);              break;
    case 0xa2: op_AND(&reg.a, reg.d);              break;
    case 0xa3: op_AND(&reg.a, reg.e);              break;
    case 0xa4: op_AND(&reg.a, reg.h);              break;
    case 0xa5: op_AND(&reg.a, reg.l);              break;
    case 0xa6: op_AND(&reg.a, mem->load8(reg.hl)); break;
    case 0xa7: op_AND(&reg.a, reg.a);              break;
    case 0xa8: op_XOR(&reg.a, reg.b);              break;
    case 0xa9: op_XOR(&reg.a, reg.c);              break;
    case 0xaa: op_XOR(&reg.a, reg.d);              break;
    case 0xab: op_XOR(&reg.a, reg.e);              break;
    case 0xac: op_XOR(&reg.a, reg.h);              break;
    case 0xad: op_XOR(&reg.a, reg.l);              break;
    case 0xae: op_XOR(&reg.a, mem->load8(reg.hl)); break;
    case 0xaf: op_XOR(&reg.a, reg.a);              break;

    case 0xb0: op_OR(&reg.a, reg.b);              break;
    case 0xb1: op_OR(&reg.a, reg.c);              break;
    case 0xb2: op_OR(&reg.a, reg.d);              break;
    case 0xb3: op_OR(&reg.a, reg.e);              break;
    case 0xb4: op_OR(&reg.a, reg.h);              break;
    case 0xb5: op_OR(&reg.a, reg.l);              break;
    case 0xb6: op_OR(&reg.a, mem->load8(reg.hl)); break;
    case 0xb7: op_OR(&reg.a, reg.a);              break;
    case 0xb8: op_CP(&reg.a, reg.b);              break;
    case 0xb9: op_CP(&reg.a, reg.c);              break;
    case 0xba: op_CP(&reg.a, reg.d);              break;
    case 0xbb: op_CP(&reg.a, reg.e);              break;
    case 0xbc: op_CP(&reg.a, reg.h);              break;
    case 0xbd: op_CP(&reg.a, reg.l);              break;
    case 0xbe: op_CP(&reg.a, mem->load8(reg.hl)); break;
    case 0xbf: op_CP(&reg.a, reg.a);              break;

    case 0xc0: op_RET(!reg.fz);                       break;
    case 0xc1: op_POP(&reg.bc);                       break;
    case 0xc2: op_JP(!reg.fz, mem->load16(reg.pc+1)); break;
    case 0xc3: op_JP(true, mem->load16(reg.pc+1));    break;
    case 0xc4: op_CALL(!reg.fz);                      break;
    case 0xc5: op_PUSH(reg.bc);                       break;
    case 0xc6: op_ADD(&reg.a, mem->load8(reg.pc+1));  break;
    case 0xc7: op_RST(0x00);                          break;
    case 0xc8: op_RET(reg.fz);                        break;
    case 0xc9: op_RET(true);                          break;
    case 0xca: op_JP(reg.fz, mem->load16(reg.pc+1));  break;
    case 0xcb: op_CB();                               break;
    case 0xcc: op_CALL(reg.fz);                       break;
    case 0xcd: op_CALL(true);                         break;
    case 0xce: op_ADC(&reg.a, mem->load8(reg.pc+1));  break;
    case 0xcf: op_RST(0x08);                          break;

    case 0xd0: op_RET(!reg.fc);                       break;
    case 0xd1: op_POP(&reg.de);                       break;
    case 0xd2: op_JP(!reg.fc, mem->load16(reg.pc+1)); break;

    case 0xd4: op_CALL(!reg.fc);                      break;
    case 0xd5: op_PUSH(reg.de);                       break;
    case 0xd6: op_SUB(&reg.a, mem->load8(reg.pc+1));  break;
    case 0xd7: op_RST(0x10);                          break;
    case 0xd8: op_RET(reg.fc);                        break;
    case 0xd9: op_RETI();                             break;
    case 0xda: op_JP(reg.fc, mem->load16(reg.pc+1));  break;

    case 0xdc: op_CALL(reg.fc);                       break;

    case 0xde: op_SBC(&reg.a, mem->load8(reg.pc+1));  break;
    case 0xdf: op_RST(0x18);                          break;

    case 0xe0: op_LDH(mem->load8(reg.pc+1), reg.a);  break;
    case 0xe1: op_POP(&reg.hl);                      break;
    case 0xe2: op_LD(0xff00 | reg.c, reg.a);         break;


    case 0xe5: op_PUSH(reg.hl);                      break;
    case 0xe6: op_AND(&reg.a, mem->load8(reg.pc+1)); break;
    case 0xe7: op_RST(0x20);                         break;
    case 0xe8: op_ADD_SP(mem->load8(reg.pc+1));      break;
    case 0xe9: op_JP(true, reg.hl);                  break;
    case 0xea: op_LD(mem->load16(reg.pc+1), reg.a);  break;



    case 0xee: op_XOR(&reg.a, mem->load8(reg.pc+1)); break;
    case 0xef: op_RST(0x28);                         break;

    case 0xf0: op_LDH(&reg.a, mem->load8(reg.pc+1));            break;
    case 0xf1: op_POP(&reg.af);                                 break;
    case 0xf2: op_LD(&reg.a, mem->load8(0xff00 | reg.c));       break;
    case 0xf3: op_DI();                                         break;

    case 0xf5: op_PUSH(reg.af);                                 break;
    case 0xf6: op_OR(&reg.a, mem->load8(reg.pc+1));             break;
    case 0xf7: op_RST(0x30);                                    break;
    case 0xf8: op_LD_HL_SP_n();                                 break;
    case 0xf9: op_LD(&reg.sp, reg.hl);                          break;
    case 0xfa: op_LD(&reg.a, mem->load16(reg.pc+1));            break;
    case 0xfb: op_EI();                                         break;


    case 0xfe: op_CP(&reg.a, mem->load8(reg.pc+1));             break;
    case 0xff: op_RST(0x38);                                    break;

    default: printf("OPCODE ERROR: unimplemented opcode 0x%02x at 0x%04x\n", currentInstruction, reg.pc); getchar();
    }

    if(!isBranch){
        if(currentInstruction != 0xcb){
            reg.pc += instructionLength[currentInstruction];
            if(haltBug){
                printf("halt bug\n");
                reg.pc--;
                haltBug = false;
            }
        }
        else{
            reg.pc += 2;
        }

        if(currentInstruction != 0xcb){
            timing.cycles = instructionTiming[currentInstruction];
        }
        else{
            timing.cycles = CBinstructionTiming[cbInstruction];
        }
    }

    isBranch = false;

    if(timing.cycles == 0) timing.cycles = 1;

    if(debug){
        //for(int i = 0; i < instructionLength[currentInstruction]; i++){
        //    printf("0x%02x ", mem->load8(prevPC+i));
        //}
        printf("\n");
        printf("%s\n\n", opcodeNames[currentInstruction]);
        printf("F:\t0b");
        for(int i = 0; i < 4; i ++)
            printf("%d", (reg.f >> (7-i))&1);
        printf("(ZNHC)\n");
        printf("AF:\t0x%04x\n", reg.af);
        printf("BC:\t0x%04x\n", reg.bc);
        printf("DE:\t0x%04x\n", reg.de);
        printf("HL:\t0x%04x\n", reg.hl);
        printf("SP:\t0x%04x\n", reg.sp);
        printf("PC:\t0x%04x\n", reg.pc);
        printf("cycles:\t%d\n", (int)timing.cyclesRun);
        printf("\n");
        printf("LCDC:\t0x%02x\n", mem->load8(0xff40));
        printf("STAT:\t0x%02x\n", mem->load8(0xff41));
        printf("LY:\t0x%02x\n",   mem->load8(0xff44));
        printf("LYC:\t0x%02x\n",  mem->load8(0xff45));
        getchar();
    }

    timing.cyclesRun++;

    return timing.cycles << 2;
}

void c_cpu::op_CB(){
    cbInstruction = mem->load8(reg.pc+1);
    //printf("0x%02x\n", cbInstruction);
    switch(cbInstruction){
    case 0x00: op_RLC(&reg.b); break;
    case 0x01: op_RLC(&reg.c); break;
    case 0x02: op_RLC(&reg.d); break;
    case 0x03: op_RLC(&reg.e); break;
    case 0x04: op_RLC(&reg.h); break;
    case 0x05: op_RLC(&reg.l); break;
    case 0x06: op_RLC(reg.hl); break;
    case 0x07: op_RLC(&reg.a); break;
    case 0x08: op_RRC(&reg.b); break;
    case 0x09: op_RRC(&reg.c); break;
    case 0x0a: op_RRC(&reg.d); break;
    case 0x0b: op_RRC(&reg.e); break;
    case 0x0c: op_RRC(&reg.h); break;
    case 0x0d: op_RRC(&reg.l); break;
    case 0x0e: op_RRC(reg.hl); break;
    case 0x0f: op_RRC(&reg.a); break;

    case 0x10: op_RL(&reg.b); break;
    case 0x11: op_RL(&reg.c); break;
    case 0x12: op_RL(&reg.d); break;
    case 0x13: op_RL(&reg.e); break;
    case 0x14: op_RL(&reg.h); break;
    case 0x15: op_RL(&reg.l); break;
    case 0x16: op_RL(reg.hl); break;
    case 0x17: op_RL(&reg.a); break;
    case 0x18: op_RR(&reg.b); break;
    case 0x19: op_RR(&reg.c); break;
    case 0x1a: op_RR(&reg.d); break;
    case 0x1b: op_RR(&reg.e); break;
    case 0x1c: op_RR(&reg.h); break;
    case 0x1d: op_RR(&reg.l); break;
    case 0x1e: op_RR(reg.hl); break;
    case 0x1f: op_RR(&reg.a); break;

    case 0x20: op_SLA(&reg.b); break;
    case 0x21: op_SLA(&reg.c); break;
    case 0x22: op_SLA(&reg.d); break;
    case 0x23: op_SLA(&reg.e); break;
    case 0x24: op_SLA(&reg.h); break;
    case 0x25: op_SLA(&reg.l); break;
    case 0x26: op_SLA(reg.hl); break;
    case 0x27: op_SLA(&reg.a); break;
    case 0x28: op_SRA(&reg.b); break;
    case 0x29: op_SRA(&reg.c); break;
    case 0x2a: op_SRA(&reg.d); break;
    case 0x2b: op_SRA(&reg.e); break;
    case 0x2c: op_SRA(&reg.h); break;
    case 0x2d: op_SRA(&reg.l); break;
    case 0x2e: op_SRA(reg.hl); break;
    case 0x2f: op_SRA(&reg.a); break;

    case 0x30: op_SWAP(&reg.b); break;
    case 0x31: op_SWAP(&reg.c); break;
    case 0x32: op_SWAP(&reg.d); break;
    case 0x33: op_SWAP(&reg.e); break;
    case 0x34: op_SWAP(&reg.h); break;
    case 0x35: op_SWAP(&reg.l); break;
    case 0x36: op_SWAP(reg.hl); break;
    case 0x37: op_SWAP(&reg.a); break;
    case 0x38: op_SRL(&reg.b);  break;
    case 0x39: op_SRL(&reg.c);  break;
    case 0x3a: op_SRL(&reg.d);  break;
    case 0x3b: op_SRL(&reg.e);  break;
    case 0x3c: op_SRL(&reg.h);  break;
    case 0x3d: op_SRL(&reg.l);  break;
    case 0x3e: op_SRL(reg.hl);  break;
    case 0x3f: op_SRL(&reg.a);  break;

    case 0x40: op_BIT(&reg.b, 0); break;
    case 0x41: op_BIT(&reg.c, 0); break;
    case 0x42: op_BIT(&reg.d, 0); break;
    case 0x43: op_BIT(&reg.e, 0); break;
    case 0x44: op_BIT(&reg.h, 0); break;
    case 0x45: op_BIT(&reg.l, 0); break;
    case 0x46: op_BIT(reg.hl, 0); break;
    case 0x47: op_BIT(&reg.a, 0); break;
    case 0x48: op_BIT(&reg.b, 1); break;
    case 0x49: op_BIT(&reg.c, 1); break;
    case 0x4a: op_BIT(&reg.d, 1); break;
    case 0x4b: op_BIT(&reg.e, 1); break;
    case 0x4c: op_BIT(&reg.h, 1); break;
    case 0x4d: op_BIT(&reg.l, 1); break;
    case 0x4e: op_BIT(reg.hl, 1); break;
    case 0x4f: op_BIT(&reg.a, 1); break;

    case 0x50: op_BIT(&reg.b, 2); break;
    case 0x51: op_BIT(&reg.c, 2); break;
    case 0x52: op_BIT(&reg.d, 2); break;
    case 0x53: op_BIT(&reg.e, 2); break;
    case 0x54: op_BIT(&reg.h, 2); break;
    case 0x55: op_BIT(&reg.l, 2); break;
    case 0x56: op_BIT(reg.hl, 2); break;
    case 0x57: op_BIT(&reg.a, 2); break;
    case 0x58: op_BIT(&reg.b, 3); break;
    case 0x59: op_BIT(&reg.c, 3); break;
    case 0x5a: op_BIT(&reg.d, 3); break;
    case 0x5b: op_BIT(&reg.e, 3); break;
    case 0x5c: op_BIT(&reg.h, 3); break;
    case 0x5d: op_BIT(&reg.l, 3); break;
    case 0x5e: op_BIT(reg.hl, 3); break;
    case 0x5f: op_BIT(&reg.a, 3); break;

    case 0x60: op_BIT(&reg.b, 4); break;
    case 0x61: op_BIT(&reg.c, 4); break;
    case 0x62: op_BIT(&reg.d, 4); break;
    case 0x63: op_BIT(&reg.e, 4); break;
    case 0x64: op_BIT(&reg.h, 4); break;
    case 0x65: op_BIT(&reg.l, 4); break;
    case 0x66: op_BIT(reg.hl, 4); break;
    case 0x67: op_BIT(&reg.a, 4); break;
    case 0x68: op_BIT(&reg.b, 5); break;
    case 0x69: op_BIT(&reg.c, 5); break;
    case 0x6a: op_BIT(&reg.d, 5); break;
    case 0x6b: op_BIT(&reg.e, 5); break;
    case 0x6c: op_BIT(&reg.h, 5); break;
    case 0x6d: op_BIT(&reg.l, 5); break;
    case 0x6e: op_BIT(reg.hl, 5); break;
    case 0x6f: op_BIT(&reg.a, 5); break;

    case 0x70: op_BIT(&reg.b, 6); break;
    case 0x71: op_BIT(&reg.c, 6); break;
    case 0x72: op_BIT(&reg.d, 6); break;
    case 0x73: op_BIT(&reg.e, 6); break;
    case 0x74: op_BIT(&reg.h, 6); break;
    case 0x75: op_BIT(&reg.l, 6); break;
    case 0x76: op_BIT(reg.hl, 6); break;
    case 0x77: op_BIT(&reg.a, 6); break;
    case 0x78: op_BIT(&reg.b, 7); break;
    case 0x79: op_BIT(&reg.c, 7); break;
    case 0x7a: op_BIT(&reg.d, 7); break;
    case 0x7b: op_BIT(&reg.e, 7); break;
    case 0x7c: op_BIT(&reg.h, 7); break;
    case 0x7d: op_BIT(&reg.l, 7); break;
    case 0x7e: op_BIT(reg.hl, 7); break;
    case 0x7f: op_BIT(&reg.a, 7); break;

    case 0x80: op_RES(&reg.b, 0); break;
    case 0x81: op_RES(&reg.c, 0); break;
    case 0x82: op_RES(&reg.d, 0); break;
    case 0x83: op_RES(&reg.e, 0); break;
    case 0x84: op_RES(&reg.h, 0); break;
    case 0x85: op_RES(&reg.l, 0); break;
    case 0x86: op_RES(reg.hl, 0); break;
    case 0x87: op_RES(&reg.a, 0); break;
    case 0x88: op_RES(&reg.b, 1); break;
    case 0x89: op_RES(&reg.c, 1); break;
    case 0x8a: op_RES(&reg.d, 1); break;
    case 0x8b: op_RES(&reg.e, 1); break;
    case 0x8c: op_RES(&reg.h, 1); break;
    case 0x8d: op_RES(&reg.l, 1); break;
    case 0x8e: op_RES(reg.hl, 1); break;
    case 0x8f: op_RES(&reg.a, 1); break;

    case 0x90: op_RES(&reg.b, 2); break;
    case 0x91: op_RES(&reg.c, 2); break;
    case 0x92: op_RES(&reg.d, 2); break;
    case 0x93: op_RES(&reg.e, 2); break;
    case 0x94: op_RES(&reg.h, 2); break;
    case 0x95: op_RES(&reg.l, 2); break;
    case 0x96: op_RES(reg.hl, 2); break;
    case 0x97: op_RES(&reg.a, 2); break;
    case 0x98: op_RES(&reg.b, 3); break;
    case 0x99: op_RES(&reg.c, 3); break;
    case 0x9a: op_RES(&reg.d, 3); break;
    case 0x9b: op_RES(&reg.e, 3); break;
    case 0x9c: op_RES(&reg.h, 3); break;
    case 0x9d: op_RES(&reg.l, 3); break;
    case 0x9e: op_RES(reg.hl, 3); break;
    case 0x9f: op_RES(&reg.a, 3); break;

    case 0xa0: op_RES(&reg.b, 4); break;
    case 0xa1: op_RES(&reg.c, 4); break;
    case 0xa2: op_RES(&reg.d, 4); break;
    case 0xa3: op_RES(&reg.e, 4); break;
    case 0xa4: op_RES(&reg.h, 4); break;
    case 0xa5: op_RES(&reg.l, 4); break;
    case 0xa6: op_RES(reg.hl, 4); break;
    case 0xa7: op_RES(&reg.a, 4); break;
    case 0xa8: op_RES(&reg.b, 5); break;
    case 0xa9: op_RES(&reg.c, 5); break;
    case 0xaa: op_RES(&reg.d, 5); break;
    case 0xab: op_RES(&reg.e, 5); break;
    case 0xac: op_RES(&reg.h, 5); break;
    case 0xad: op_RES(&reg.l, 5); break;
    case 0xae: op_RES(reg.hl, 5); break;
    case 0xaf: op_RES(&reg.a, 5); break;

    case 0xb0: op_RES(&reg.b, 6); break;
    case 0xb1: op_RES(&reg.c, 6); break;
    case 0xb2: op_RES(&reg.d, 6); break;
    case 0xb3: op_RES(&reg.e, 6); break;
    case 0xb4: op_RES(&reg.h, 6); break;
    case 0xb5: op_RES(&reg.l, 6); break;
    case 0xb6: op_RES(reg.hl, 6); break;
    case 0xb7: op_RES(&reg.a, 6); break;
    case 0xb8: op_RES(&reg.b, 7); break;
    case 0xb9: op_RES(&reg.c, 7); break;
    case 0xba: op_RES(&reg.d, 7); break;
    case 0xbb: op_RES(&reg.e, 7); break;
    case 0xbc: op_RES(&reg.h, 7); break;
    case 0xbd: op_RES(&reg.l, 7); break;
    case 0xbe: op_RES(reg.hl, 7); break;
    case 0xbf: op_RES(&reg.a, 7); break;

    case 0xc0: op_SET(&reg.b, 0); break;
    case 0xc1: op_SET(&reg.c, 0); break;
    case 0xc2: op_SET(&reg.d, 0); break;
    case 0xc3: op_SET(&reg.e, 0); break;
    case 0xc4: op_SET(&reg.h, 0); break;
    case 0xc5: op_SET(&reg.l, 0); break;
    case 0xc6: op_SET(reg.hl, 0); break;
    case 0xc7: op_SET(&reg.a, 0); break;
    case 0xc8: op_SET(&reg.b, 1); break;
    case 0xc9: op_SET(&reg.c, 1); break;
    case 0xca: op_SET(&reg.d, 1); break;
    case 0xcb: op_SET(&reg.e, 1); break;
    case 0xcc: op_SET(&reg.h, 1); break;
    case 0xcd: op_SET(&reg.l, 1); break;
    case 0xce: op_SET(reg.hl, 1); break;
    case 0xcf: op_SET(&reg.a, 1); break;

    case 0xd0: op_SET(&reg.b, 2); break;
    case 0xd1: op_SET(&reg.c, 2); break;
    case 0xd2: op_SET(&reg.d, 2); break;
    case 0xd3: op_SET(&reg.e, 2); break;
    case 0xd4: op_SET(&reg.h, 2); break;
    case 0xd5: op_SET(&reg.l, 2); break;
    case 0xd6: op_SET(reg.hl, 2); break;
    case 0xd7: op_SET(&reg.a, 2); break;
    case 0xd8: op_SET(&reg.b, 3); break;
    case 0xd9: op_SET(&reg.c, 3); break;
    case 0xda: op_SET(&reg.d, 3); break;
    case 0xdb: op_SET(&reg.e, 3); break;
    case 0xdc: op_SET(&reg.h, 3); break;
    case 0xdd: op_SET(&reg.l, 3); break;
    case 0xde: op_SET(reg.hl, 3); break;
    case 0xdf: op_SET(&reg.a, 3); break;

    case 0xe0: op_SET(&reg.b, 4); break;
    case 0xe1: op_SET(&reg.c, 4); break;
    case 0xe2: op_SET(&reg.d, 4); break;
    case 0xe3: op_SET(&reg.e, 4); break;
    case 0xe4: op_SET(&reg.h, 4); break;
    case 0xe5: op_SET(&reg.l, 4); break;
    case 0xe6: op_SET(reg.hl, 4); break;
    case 0xe7: op_SET(&reg.a, 4); break;
    case 0xe8: op_SET(&reg.b, 5); break;
    case 0xe9: op_SET(&reg.c, 5); break;
    case 0xea: op_SET(&reg.d, 5); break;
    case 0xeb: op_SET(&reg.e, 5); break;
    case 0xec: op_SET(&reg.h, 5); break;
    case 0xed: op_SET(&reg.l, 5); break;
    case 0xee: op_SET(reg.hl, 5); break;
    case 0xef: op_SET(&reg.a, 5); break;

    case 0xf0: op_SET(&reg.b, 6); break;
    case 0xf1: op_SET(&reg.c, 6); break;
    case 0xf2: op_SET(&reg.d, 6); break;
    case 0xf3: op_SET(&reg.e, 6); break;
    case 0xf4: op_SET(&reg.h, 6); break;
    case 0xf5: op_SET(&reg.l, 6); break;
    case 0xf6: op_SET(reg.hl, 6); break;
    case 0xf7: op_SET(&reg.a, 6); break;
    case 0xf8: op_SET(&reg.b, 7); break;
    case 0xf9: op_SET(&reg.c, 7); break;
    case 0xfa: op_SET(&reg.d, 7); break;
    case 0xfb: op_SET(&reg.e, 7); break;
    case 0xfc: op_SET(&reg.h, 7); break;
    case 0xfd: op_SET(&reg.l, 7); break;
    case 0xfe: op_SET(reg.hl, 7); break;
    case 0xff: op_SET(&reg.a, 7); break;

    default: printf("ERROR: unimplemented CB opcode 0x%02x at 0x%04x\n", cbInstruction, reg.pc); getchar();
    }
}

void c_cpu::op_NOP(){
}

void c_cpu::op_LD(u16* dst, u16 data){
    (*dst) = data;
}

void c_cpu::op_LD(u16 dst, u16 src){
    mem->store16(dst, src);
}

void c_cpu::op_LD(u16 dst, u8 src){
    mem->store8(dst, src);
}

void c_cpu::op_LD(u8* dst, u8 data){
    (*dst) = data;
}

void c_cpu::op_LD(u8* dst, u16 addr){
    (*dst) = mem->load8(addr);
}

void c_cpu::op_LDH(u8* dst, u8 addr){
    (*dst) = mem->load8(0xff00 | addr);
}

void c_cpu::op_LD_HL_SP_n(){
    s8 n = mem->load8(reg.pc+1);
    s16 result = reg.sp + n;

    reg.fz = 0;
    reg.fn = 0;
    reg.fh = ((reg.sp ^ n ^ result) & 0x10)  == 0x10;
    reg.fc = ((reg.sp ^ n ^ result) & 0x100) == 0x100;

    reg.hl = result;
}

void c_cpu::op_LDH(u8 addr, u8 src){
    mem->store8(0xff00 | addr, src);
}

void c_cpu::op_PUSH(u16 src){
    push16(src);

}

void c_cpu::op_POP(u16* dst){
    u16 data = pop16();
    if(dst == &reg.af)
        data &= 0xfff0;
    (*dst) = data;
}

void c_cpu::op_INC(u16* src){
    (*src)++;
}

void c_cpu::op_INC(u16 addr){
    u8 data = mem->load8(addr)+1;
    mem->store8(addr, data);

    reg.fz =  data == 0x00;
    reg.fn =  0;
    reg.fh = (data & 0x0f) == 0x00;
}

void c_cpu::op_INC(u8* src){
    (*src)++;

    reg.fz = (*src) == 0x00;
    reg.fn = 0;
    reg.fh = ((*src) & 0x0f) == 0x00;
}

void c_cpu::op_DEC(u16* src){
    (*src)--;
}

void c_cpu::op_DEC(u16 addr){
    u8 data = mem->load8(addr)-1;
    mem->store8(addr, data);

    reg.fz =  data == 0x00;
    reg.fn =  1;
    reg.fh = (data & 0x0f) == 0x0f;
}

void c_cpu::op_DEC(u8* src){
    (*src)--;

    reg.fz =  (*src) == 0x00;
    reg.fn =  1;
    reg.fh = ((*src) & 0x0f) == 0x0f;
}

void c_cpu::op_RLC(u8* src){
    reg.fz = (((*src) << 1) | ((*src) >> 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = (*src) >> 7;

    (*src) = ((*src) << 1) | ((*src) >> 7);
}

void c_cpu::op_RLC(u16 src){
    u8 data = mem->load8(src);

    reg.fz = ((data << 1) | (data >> 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = data >> 7;

    mem->store8(src, (data << 1) | (data >> 7));
}

void c_cpu::op_RL(u8* src){
    bool carry = reg.fc;
    u8 result = (*src);
    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;
    if((result & 0x80) != 0){
        reg.fc = 1;
    }
    result <<= 1;
    result |= carry;

    (*src) = result;

    reg.fz = result == 0;
}

void c_cpu::op_RL(u16 src){
    bool carry = reg.fc;
    u8 result = mem->load8(src);
    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;
    if((result & 0x80) != 0){
        reg.fc = 1;
    }
    result <<= 1;
    result |= carry;

    mem->store8(src, result);

    reg.fz = result == 0;
}

void c_cpu::op_RRC(u8* src){
    reg.fz = (((*src) >> 1) | ((*src) << 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = (*src) & 0x01;

    (*src) = ((*src) >> 1) | ((*src) << 7);
}

void c_cpu::op_RRC(u16 src){
    u8 data = mem->load8(src);

    reg.fz = ((data >> 1) | (data << 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = data & 0x01;

    mem->store8(src, (data >> 1) | (data << 7));
}

void c_cpu::op_RR(u8* src){
    bool carry = reg.fc;

    reg.fz = (((*src) >> 1) | (carry << 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = (*src) & 1;

    (*src) = ((*src) >> 1) | (carry << 7);
}

void c_cpu::op_RR(u16 src){
    bool carry = reg.fc;
    u8 data = mem->load8(src);

    reg.fz = ((data >> 1) | (carry << 7)) == 0x00;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = data & 1;

    mem->store8(src, (data >> 1) | (carry << 7));
}

void c_cpu::op_SLA(u8* src){
    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    if(((*src) & 0x80) != 0){
        reg.fc = 1;
    };
    u8 result = (*src) << 1;
    (*src) = result;
    reg.fz = result == 0;
}

void c_cpu::op_SLA(u16 src){
    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    u8 data = mem->load8(src);

    if((data & 0x80) != 0){
        reg.fc = 1;
    };
    u8 result = data << 1;
    mem->store8(src, result);
    reg.fz = result == 0;
}

void c_cpu::op_SRA(u8* src){
    u8 result = (*src);

    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    if((result & 0x01) != 0)
        reg.fc = 1;

    if ((result & 0x80) != 0){
        result >>= 1;
        result |= 0x80;
    }else{
        result >>= 1;
    }
    (*src) = result;
    reg.fz = result == 0;
}

void c_cpu::op_SRA(u16 src){
    u8 result = mem->load8(src);

    reg.fz = 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    if((result & 0x01) != 0)
        reg.fc = 1;

    if ((result & 0x80) != 0){
        result >>= 1;
        result |= 0x80;
    }else{
        result >>= 1;
    }
    mem->store8(src, result);
    reg.fz = result == 0;
}

void c_cpu::op_SRL(u8* src){
    reg.fz = ((*src) >> 1) == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = ((*src) & 1);

    (*src) >>= 1;
}

void c_cpu::op_SRL(u16 src){
    u8 data = mem->load8(src);

    reg.fz = (data >> 1) == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = (data & 1);

    mem->store8(src, data >> 1);
}

void c_cpu::op_STOP(){
    //printf("STOP\n");
    stopped = true;
    if((mem->load8(0xff4d) & 1) == 1){
        mem->store8(0xff4d, mem->load8(0xff4d) ^ 0x80);
        if(mem->load8(0xff4d)>>7)
            printf("double speed mode enabled\n");
        else
            printf("double speed mode disabled\n");
        stopped = false;
    }
    //getchar();
}

void c_cpu::op_HALT(){
    halted = true;
    //if(!interruptMasterEnable){
        //haltBug = true;
        //debug = true;
    //}
}

void c_cpu::op_JR(bool cond){
    if(cond){
        u16 offset = mem->load8(reg.pc+1);
        if(offset > 0x7f) offset |= 0xff00;
        reg.pc = reg.pc + 2 + offset;
        isBranch = true;
        timing.cycles = 12;

    }else{
        timing.cycles = 8;
    }
}

void c_cpu::op_JP(bool cond, u16 addr){
    if(cond){
        reg.pc = addr;
        isBranch = true;
        timing.cycles = 16;
    }else{
        timing.cycles = 12;
    }
}

void c_cpu::op_CALL(bool cond){
    if(cond){
        push16(reg.pc+3);
        reg.pc = mem->load16(reg.pc+1);
        isBranch = true;
        timing.cycles = 24;
    }else{
        timing.cycles = 12;
    }
}

void c_cpu::op_RET(bool cond){
    if(cond){
        reg.pc = pop16();
        isBranch = true;
        timing.cycles = 20;
    }else{
        timing.cycles = 8;
    }
}

void c_cpu::op_RETI(){
    reg.pc = pop16();
    interruptMasterEnable = true;
    isBranch = true;
    timing.cycles = 16;
    //getchar();
}

void c_cpu::op_RST(u8 addr){
    push16(reg.pc+1);
    reg.pc = addr;
    isBranch = true;
    timing.cycles = 16;
}

void c_cpu::op_DAA(){
    int a = reg.a;

    if(!reg.fn){
        if(reg.fc || (a > 0x99)){
            a += 0x60;
            reg.fc = 1;
        }
        if(reg.fh || (a & 0xf)>9){
            a += 0x06;
        }
    }else if(reg.fc && reg.fh){
        a += 0x9a;
    }
    else if(reg.fc){
        a += 0xa0;
    }
    else if(reg.fh){
        a += 0xfa;
    }

    reg.a = a;

    reg.fh = 0;
    reg.fz = reg.a == 0;
}

void c_cpu::op_ADD(u8* dst, u8 src){
    reg.fn = 0;
    reg.fh = (((*dst)&0xf)+(src&0xf)) > 0xf;
    reg.fc = ((*dst)+src) > 0xff;

    (*dst) += src;

    reg.fz = (*dst) == 0;
}

void c_cpu::op_ADD_HL(u16 src){
    reg.fn = 0;
    reg.fh = (reg.hl ^ src ^ ((reg.hl+src) & 0xffff)) & 0x1000;
    reg.fc = reg.hl+src > 0xffff;

    reg.hl += src;
}

void c_cpu::op_ADD_SP(s8 offset){
    int result = reg.sp + offset;

    reg.fz = 0;
    reg.fn = 0;
    reg.fh = ((reg.sp ^ offset ^ (result & 0xFFFF)) & 0x10) == 0x10;
    reg.fc = ((reg.sp ^ offset ^ (result & 0xFFFF)) & 0x100) == 0x100;

    reg.sp = (u16)result;
}

void c_cpu::op_SUB(u8* dst, u8 src){
    int result = (*dst) - src;
    int carry  = (*dst) ^ src ^ result;

    reg.fz = (*dst) == src;
    reg.fn = 1;
    reg.fh = (carry & 0x10)  != 0;
    reg.fc = (carry & 0x100) != 0;

    (*dst) -= src;
}

void c_cpu::op_ADC(u8* dst, u8 src){
    bool carry = reg.fc;

    reg.fn = 0;
    reg.fh = (((*dst)&0xf)+(src&0xf)+carry) > 0xf;
    reg.fc = ((*dst)+src+carry) > 0xff;

    (*dst) = (*dst)+src+carry;

    reg.fz = (*dst) == 0;
}

void c_cpu::op_SBC(u8* dst, u8 src){
    bool carry = reg.fc;

    int result = reg.a - src - carry;

    reg.fz = (u8)result == 0;
    reg.fn = 1;
    reg.fh = ((reg.a & 0x0f) - (src & 0x0f) - carry) < 0;
    reg.fc = result < 0;

    reg.a = (u8)result;
}

void c_cpu::op_SCF(){
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 1;
}

void c_cpu::op_CCF(){
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = !reg.fc;
}

void c_cpu::op_EI(){
    interruptMasterEnable = true;
}

void c_cpu::op_DI(){
    interruptMasterEnable = false;
}

void c_cpu::op_CPL(){
    reg.a = ~reg.a;

    reg.fn = 1;
    reg.fh = 1;
}

void c_cpu::op_AND(u8* dst, u8 src){
    u8 result = (*dst) & src;
    reg.fz = result == 0;
    reg.fn = 0;
    reg.fh = 1;
    reg.fc = 0;

    (*dst) = result;
}

void c_cpu::op_XOR(u8* dst, u8 src){
    u8 result = (*dst) ^ src;
    reg.fz = result == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    (*dst) = result;
}

void c_cpu::op_OR(u8* dst, u8 src){
    u8 result = (*dst) | src;
    reg.fz = result == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    (*dst) = result;
}

void c_cpu::op_CP(u8* dst, u8 src){
    reg.fz = (*dst) == src;
    reg.fn = 1;
    reg.fh = (((*dst)-src)&0xf) > ((*dst)&0xf);
    reg.fc = (*dst) < src;
}

void c_cpu::op_SWAP(u8* src){
    reg.fz = (*src) == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    (*src) = ((*src) >> 4) | ((*src) << 4);
}

void c_cpu::op_SWAP(u16 src){
    u8 data = mem->load8(src);

    reg.fz = data == 0;
    reg.fn = 0;
    reg.fh = 0;
    reg.fc = 0;

    mem->store8(src, (data >> 4) | (data<< 4));
}

void c_cpu::op_BIT(u8* src, u8 bit){
    reg.fz = ((*src) & (1 << bit)) == 0;
    reg.fn = 0;
    reg.fh = 1;
}

void c_cpu::op_BIT(u16 src, u8 bit){
    reg.fz = (mem->load8(src) & (1 << bit)) == 0;
    reg.fn = 0;
    reg.fh = 1;
}

void c_cpu::op_SET(u8* src, u8 bit){
    (*src) |= (1 << bit);
}

void c_cpu::op_SET(u16 src, u8 bit){
    mem->store8(src, mem->load8(src) | (1 << bit));
}

void c_cpu::op_RES(u8* src, u8 bit){
    (*src) &= ~(1 << bit);
}

void c_cpu::op_RES(u16 src, u8 bit){
    mem->store8(src, mem->load8(src) & ~(1 << bit));
}
