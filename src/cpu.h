
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
    //flags
    unsigned char key_flag;
    unsigned char run_flag;
    unsigned char draw_flag;
    unsigned char pause_flag;
} chip8;

void initialize(chip8 *cpu);
void load_rom(chip8 *cpu, char *rom);
void cycle(chip8 *cpu);


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
extern unsigned char memory[0x1000];
extern unsigned char gfx[64 * 32];
extern unsigned char key[0x10];