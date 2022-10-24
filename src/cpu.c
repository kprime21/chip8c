#include <stdio.h>
#include <stdlib.h>
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

unsigned char memory[4096];

typedef struct chip8{
    unsigned int opcode;
    unsigned int program_counter;
    unsigned int index;
    unsigned int stack[0x10];
    unsigned int*stack_ptr;
    //60Hz timer registers 
    unsigned int delay_timer;
    unsigned int sound_timer;
    //GP registers
    unsigned short V0;
    unsigned short V1;
    unsigned short V2;
    unsigned short V3;
    unsigned short V4;
    unsigned short V5;
    unsigned short V6;
    unsigned short V7;
    unsigned short V8;
    unsigned short V9;
    unsigned short VA;
    unsigned short VB;
    unsigned short VC;
    unsigned short VD;
    unsigned short VE;
    //used as a flag register
    unsigned short VF; 
} chip8;



void initialize(chip8 *cpu){
    for(int i = 0;i < 16; i++){
        cpu->program_counter = 0x00;
        cpu->index = 0x00;
        cpu->opcode = 0x00;
        for(int i = 0; i< 16; i++){
            cpu->stack[i] = 0;
        }
    }

    //load the fonts into memory

    for(int i = 0; i < 80; i++){
        memory[i] = font_reg[i];
    }
}


void cycle(chip8 *cpu){
    //fetch
    cpu->opcode = memory[cpu->program_counter] << 8 | memory[cpu->program_counter + 1];
    printf("0x%08x\n", cpu->opcode);
    //increment program counter by 2
    cpu->program_counter +=2;

    //decode and execute
    switch(cpu->opcode){
    case 0x00E0:
        printf("CLS, clear the display\n");
        break;
    case 0x00EE:
        printf("RET, return from subroutine\n");
        break;
    case 0x1000 ... 0x1FFF:
        printf("JP addr, jump to address that is nnn\n");
        break;
    case 0x2000 ... 0x2FFF:
        printf("CALL addr, increment stack pointer then place current PC on top of stack, then set PC to nnn\n");
        break;
        //where x is the 2nd byte and kk is the 3rd and 4th byte
    case 0x3000 ... 0x3FFF:
        printf("skip next instruction if Vx == kk\n");
        break;
    case 0x4000 ... 0x4FFF:
        printf("skip next instruction if Vx != kk\n");
        break;
        //Vx is 2nd byte and Vy is third byte
    case 0x5000 ... 0x5FF0:
        printf("skip next instruction if Vx == Vy\n");
        break;
    case 0x6000 ... 0x6FFF:
        printf("Load kk into Vx\n");
        break;
    case 0x7000 ... 0x7FFF:
        printf("add kk to register Vx and store in Vx\n");
        break;
    // case 0x8000 ... 0x8FF0:
    case 0x8FF0:
        printf("store Vy into register Vx\n");
        break;
    // case 0x8001 ... 0x8FF1:
    case 0x8FF1:
        printf("Vx OR Vy and store in Vx\n");
        break;
    // case 0x8002 ... 0x8FF2:
    case 0x8FF2:
        printf("Vx AND Vy and store in Vx\n");
        break;
    // case 0x8003 ... 0x8FF3:
    case 0x8FF3:
        printf("Vx XOR Vy and store in Vx\n");
        break;
    // case 0x8004 ... 0x8FF4:
    case 0x8FF4:
        printf("Vx + Vy, overflow set Vf to 1, store 8 lower bits in Vx");
        break;
    // case 0x8005 ... 0x8FF5:
    case 0x8FF5:
        printf("Vx = Vy - Vx, Vx>Vy set Vf to 1\n");
        break;
    // case 0x8006 ... 0x8FF6:
    case 0x8FF6:
        printf("Vx >> 1, if LSB of Vx is 1, set Vf to 1\n");
        break;
    // case 0x8007 ... 0x8FF7:
    case 0x8FF7:
        printf("Vx = Vy - Vx, Vy>Vx set Vf to 1\n");
        break;
    // case 0x800E ... 0x8FFE:
    case 0x8FFE:
        printf("Vx = Vx << 1, if MSB of Vx is 1, set Vf to 1");
        break;
    // case 0x9000 ... 0x9FF0:
    case 0x9FF0:
        printf("if Vx!=Vy, PC+=2");
        break;
    // case 0xA000 ... 0xAFFF:
    case 0xAFFF:
        printf("Index register is set to addr in last three bytes");
        break;
    // case 0xB000 ... 0xBFFF:
    case 0xBFFF:
        printf("Program counter register is set to V0 + last three bytes");
        break;
    // case 0xC000 ... 0xCFFF:
    case 0xCFFF:
        printf("Generate a random number, then AND with last two bytes and store in Vx(2nd byte)");
        break;
    // case 0xD000 ... 0xDFFF:
    case 0xDFFF:
        printf("Read number of bytes store in last byte starting at Index. Display at location (Vx -2nd byte,Vy - 3rd byte), if collision Vf = 1, if off screen wrap around \n");
        break;
    case 0xE09E:
    case 0xE19E:
    case 0xE29E:
    case 0xE39E:
    case 0xE49E:
    case 0xE59E:
    case 0xE69E:
    case 0xE79E:
    case 0xE89E:
    case 0xE99E:
    case 0xEA9E:
    case 0xEB9E:
    case 0xEC9E:
    case 0xED9E:
    case 0xEE9E:
    case 0xEF9E:
        printf("Skip next instruction if key in Vx (2nd byte) is pressed, PC+=2\n");
        break;
    case 0xE0A1:
    case 0xE1A1:
    case 0xE2A1:
    case 0xE3A1:
    case 0xE4A1:
    case 0xE5A1:
    case 0xE6A1:
    case 0xE7A1:
    case 0xE8A1:
    case 0xE9A1:
    case 0xEAA1:
    case 0xEBA1:
    case 0xECA1:
    case 0xEDA1:
    case 0xEEA1:
    case 0xEFA1:
        printf("Skip next instruction if key in Vx (2nd byte) is not perssed, PC+=2\n");
        break;
    case 0xF007:
    case 0xF107:
    case 0xF207:
    case 0xF307:
    case 0xF407:
    case 0xF507:
    case 0xF607:
    case 0xF707:
    case 0xF807:
    case 0xF907:
    case 0xFA07:
    case 0xFB07:
    case 0xFC07:
    case 0xFD07:
    case 0xFE07:
    case 0xFF07:
        printf("Vx = Delay Timer\n");
        break;
    case 0xF00A:
    case 0xF10A:
    case 0xF20A:
    case 0xF30A:
    case 0xF40A:
    case 0xF50A:
    case 0xF60A:
    case 0xF70A:
    case 0xF80A:
    case 0xF90A:
    case 0xFA0A:
    case 0xFB0A:
    case 0xFC0A:
    case 0xFD0A:
    case 0xFE0A:
    case 0xFF0A:
        printf("Exection stops until a key is pressed, store value of key in Vx\n");
        break;
    case 0xF015:
    case 0xF115:
    case 0xF215:
    case 0xF315:
    case 0xF415:
    case 0xF515:
    case 0xF615:
    case 0xF715:
    case 0xF815:
    case 0xF915:
    case 0xFA15:
    case 0xFB15:
    case 0xFC15:
    case 0xFD15:
    case 0xFE15:
    case 0xFF15:
        printf("Set delay timer to value of Vx (2nd byte)\n");
        break;
    case 0xF018:
    case 0xF118:
    case 0xF218:
    case 0xF318:
    case 0xF418:
    case 0xF518:
    case 0xF618:
    case 0xF718:
    case 0xF818:
    case 0xF918:
    case 0xFA18:
    case 0xFB18:
    case 0xFC18:
    case 0xFD18:
    case 0xFE18:
    case 0xFF18:
        printf("Set sound timer to value of Vx (2nd byte)\n");
        break;
    case 0xF01E:
    case 0xF11E:
    case 0xF21E:
    case 0xF31E:
    case 0xF41E:
    case 0xF51E:
    case 0xF61E:
    case 0xF71E:
    case 0xF81E:
    case 0xF91E:
    case 0xFA1E:
    case 0xFB1E:
    case 0xFC1E:
    case 0xFD1E:
    case 0xFE1E:
    case 0xFF1E:
        printf("Index register + Vx (2nd byte) is stored in Index Register\n");
        break;
    case 0xF029:
    case 0xF129:
    case 0xF229:
    case 0xF329:
    case 0xF429:
    case 0xF529:
    case 0xF629:
    case 0xF729:
    case 0xF829:
    case 0xF929:
    case 0xFA29:
    case 0xFB29:
    case 0xFC29:
    case 0xFD29:
    case 0xFE29:
    case 0xFF29:
        printf("Index register is set equal to the location for hexadecimal sprite corresponding to value in Vx\n");
        break;
    case 0xF033:
    case 0xF133:
    case 0xF233:
    case 0xF333:
    case 0xF433:
    case 0xF533:
    case 0xF633:
    case 0xF733:
    case 0xF833:
    case 0xF933:
    case 0xFA33:
    case 0xFB33:
    case 0xFC33:
    case 0xFD33:
    case 0xFE33:
    case 0xFF33:
        printf("Store BCD of Vx in Index Register, Index Register+1, and Index Register+2\n");
        break;
    case 0xF055:
    case 0xF155:
    case 0xF255:
    case 0xF355:
    case 0xF455:
    case 0xF555:
    case 0xF655:
    case 0xF755:
    case 0xF855:
    case 0xF955:
    case 0xFA55:
    case 0xFB55:
    case 0xFC55:
    case 0xFD55:
    case 0xFE55:
    case 0xFF55:
        printf("Store registers V0 - Vx (2nd byte) in in memory starting at address in Index register\n");
        break;
    case 0xF065:
    case 0xF165:
    case 0xF265:
    case 0xF365:
    case 0xF465:
    case 0xF565:
    case 0xF665:
    case 0xF765:
    case 0xF965:
    case 0xFA65:
    case 0xFB65:
    case 0xFC65:
    case 0xFD65:
    case 0xFE65:
    case 0xFF65:
        printf("Read values from memory starting at Index register and store them in V0 through Vx (2nd byte)");
        break;
    }
    
}
int main(){
//the display screen
unsigned char gfx[64 * 32];
//make cpu
chip8 test;
initialize(&test);
cycle(&test);


FILE *fp, *fr;

fp = fopen("./roms/PONG", "rb");
fseek(fp, 0 , SEEK_END);
long romSize = ftell(fp);
unsigned char buffer[romSize]; 
rewind(fp);
printf("%d\n", romSize);
fread(buffer,sizeof(buffer),1,fp);

fr = fopen("pong.txt", "w+");
for(int i = 0; i < romSize; i++){
    fprintf(fr, "0x%08x\n", buffer[i]);
}
fclose(fr);
}

