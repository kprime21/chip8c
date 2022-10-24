#include <stdio.h>
//registers here


int font_reg[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

unsigned int program_counter[0x10];
unsigned int index[0x10];
unsigned int stack[0x10];
//60Hz
unsigned int delay_timer[0x8];
unsigned int sound_timer[0x8];
//GP registers
unsigned int V0[0x8];
unsigned int V1[0x8];
unsigned int V2[0x8];
unsigned int V3[0x8];
unsigned int V4[0x8];
unsigned int V5[0x8];
unsigned int V6[0x8];
unsigned int V7[0x8];
unsigned int V8[0x8];
unsigned int V9[0x8];
unsigned int VA[0x8];
unsigned int VB[0x8];
unsigned int VC[0x8];
unsigned int VD[0x8];
unsigned int VE[0x8];
unsigned int VF[0x8];