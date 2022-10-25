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
} chip8;

void initialize(chip8 *cpu);
void load_rom(chip8 *cpu, char *rom);
void cycle(chip8 *cpu);


extern int font_reg[80];
extern unsigned char memory[0x1000];
extern unsigned char gfx[64 * 32];
extern unsigned char key[0x10];