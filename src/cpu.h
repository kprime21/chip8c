#include <SDL2/SDL.h>
typedef struct chip8{
    //hold the 2 byte opcode
    unsigned short opcode;
    //addressing registers
    unsigned short program_counter;
    unsigned short index;
    //return from subroutine
    unsigned short stack[0x10];
    unsigned short stackptr;
    //60Hz timer registers 
    unsigned char delay_timer;
    unsigned char sound_timer;
    //GP registers
    unsigned char V[0x10];
    //track key press
    unsigned char key[0x10];
    //flags
    unsigned char key_flag;
    unsigned char run_flag;
    unsigned char draw_flag;
    unsigned char pause_flag;
} chip8;

void initialize(chip8 *cpu);
void reset(chip8 *cpu); //implement
void load_rom(chip8 *cpu, char *rom);
void cycle(chip8 *cpu);
void clock(chip8 *cpu); //decrement sound and delay timers
void key_event(chip8 *cpu); //process the user keyboard event 

const static int font_reg[80] = {
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
const static short keyboard[0x10] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};
extern unsigned char memory[0x1000];
extern unsigned char gfx[64 * 32];