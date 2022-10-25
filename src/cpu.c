#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
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
//ram 4k
unsigned char memory[0x1000];
//graphics screen
unsigned char gfx[64 * 32];


//set addressing registers to 0
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

void load_rom(chip8 *cpu, char *rom){
    FILE *fp;
    char path[100] = "./roms/";
    strcat(path, rom);
    printf("rom name %s\n", path);
    fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    unsigned char* temp = memory + 0x200;
    fread(temp, sizeof(sizeof(temp)), size, fp);
    cpu->program_counter = 0x200;
}
    
    
//fetch, decode, and execute cycle
void cycle(chip8 *cpu){
    //fetch
    cpu->opcode = memory[cpu->program_counter] << 8 | memory[cpu->program_counter + 1];
    //increment program counter by 2
    cpu->program_counter +=2;
    //decode and execute
    switch(cpu->opcode & 0xF000){
        
        case 0x0000:
            switch(cpu->opcode & 0x00FF){
                case 0x00E0:
                    printf("CLS, clear the display\n");
                    break;
                case 0x00EE:
                    printf("RET, return from subroutine\n");
                    break;
                default:
                    printf("Unkown opcode, exiting... \n");
            }
            break;
        case 0x1000: //... 0x1FFF:
            cpu->program_counter = cpu->opcode & 0x0FFF;
            printf("JP addr, jump to address that is 0x%03X\n",cpu->opcode & 0x0FFF);
            break;
        case 0x2000: //... 0x2FFF:
            cpu->stackptr++;
            cpu->stack[cpu->stackptr] = cpu->program_counter;
            cpu->program_counter = cpu->opcode & 0x0FFF;
            printf("CALL addr, increment stack pointer then place current PC on top of stack, then set PC to 0x%03X\n", cpu->opcode & 0x0FFF);
            break;
            //where x is the 2nd byte and kk is the 3rd and 4th byte
        case 0x3000: //... 0x3FFF:
            if(cpu->V[cpu->opcode & 0x0F00] == cpu->opcode & 0x00FF){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X == 0x%02X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00FF);
            break;
        case 0x4000: //... 0x4FFF:
        if(cpu->V[cpu->opcode & 0x0F00] != cpu->opcode & 0x00FF){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X != 0x%02X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00FF);
            break;
            //Vx is 2nd byte and Vy is third byte
        case 0x5000: //... 0x5FF0:
            if(cpu->V[cpu->opcode & 0x0F00] != cpu->V[cpu->opcode & 0x00F0]){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X == Vy 0x%01X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
            break;
        case 0x6000: //... 0x6FFF:
            cpu->V[cpu->opcode & 0x0F00] = (cpu->opcode & 0x00FF );
            printf("Load  0x%02X into Vx 0x%01X\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
            break;
        case 0x7000: //... 0x7FFF:
            cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] + (cpu->opcode & 0x00FF ); 
            printf("add 0x%02X to register Vx 0x%01X and store in Vx\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
            break;
        // case 0x8000 ... 0x8FF0:
        case 0x8000:
            switch(cpu->opcode & 0x000F){
            case 0x0000:
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x00F0];
                printf("store Vy 0x%01X into register Vx 0x%01X\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8001 ... 0x8FF1:
            case 0x0001:
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] | cpu->V[cpu->opcode & 0x00F0];
                printf("Vx 0x%01X OR Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8002 ... 0x8FF2:
            case 0x0002:
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] & cpu->V[cpu->opcode & 0x00F0];
                printf("Vx 0x%01X  AND Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8003 ... 0x8FF3:
            case 0x0003:
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] ^ cpu->V[cpu->opcode & 0x00F0];
                printf("Vx 0x%01X XOR Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8004 ... 0x8FF4:
            case 0x0004:
                if((cpu->V[cpu->opcode & 0x0F00] + cpu-> V[cpu->opcode & 0x00F0]) > 255){
                    cpu->V[0x0F] = 1;
                }
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] + cpu-> V[cpu->opcode & 0x00F0];
                
                printf("Vx 0x%01X + Vy 0x%01X, overflow set Vf to 1, store 8 lower bits in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8005 ... 0x8FF5:
            case 0x0005:
                if(cpu->V[cpu->opcode & 0x0F00] > cpu-> V[cpu->opcode & 0x00F0]){
                    cpu->V[0x0F] = 1;
                }
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] - cpu-> V[cpu->opcode & 0x00F0];
                printf("Vx 0x%01X = Vy 0x%01X - Vx, Vx>Vy set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8006 ... 0x8FF6:
            case 0x0006:
                if(cpu->V[cpu->opcode & 0x0F00] & 0x0001 == 0x0001){
                    cpu->V[0x0F] = 1;
                }
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] >> 1;
                
                printf("Vx 0x%01X >> Vy 0x%01X, if LSB of Vx is 1, set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8007 ... 0x8FF7:
            case 0x0007:
                if(cpu->V[cpu->opcode & 0x00F0] > cpu-> V[cpu->opcode & 0x0F00]){
                    cpu->V[0x0F] = 1;
                }
                cpu->V[cpu->opcode & 0x00F0] = cpu->V[cpu->opcode & 0x00F0] - cpu-> V[cpu->opcode & 0x0F00];
                printf("Vx 0x%01X = Vy 0x%01X - Vx, Vy>Vx set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x800E ... 0x8FFE:
            case 0x000E:
                if(cpu->V[cpu->opcode & 0x0F00] & 0x8000 == 0x8000){
                    cpu->V[0x0F] = 1;
                }
                cpu->V[cpu->opcode & 0x0F00] = cpu->V[cpu->opcode & 0x0F00] << 1;
                printf("Vx 0x%01X = Vx << Vy 0x%01X, if MSB of Vx is 1, set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            default:
                printf("Unkown opcode, exiting... \n");
            }
            break;
        // case 0x9000 ... 0x9FF0:
        case 0x9000:
            if(cpu->V[cpu->opcode & 0x00F0] != cpu-> V[cpu->opcode & 0x0F00]){
                    cpu->program_counter +=2;
            }
            printf("if Vx 0x%01X !=Vy 0x%01X, PC+=2\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
            break;
        // case 0xA000 ... 0xAFFF:
        case 0xA000:
            cpu->index = cpu->opcode & 0x0FFF;
            printf("Index register is set to addr  0x%03X in last three bytes\n", cpu->opcode & 0x0FFF);
            break;
        // case 0xB000 ... 0xBFFF:
        case 0xB000:
            cpu->program_counter = cpu->V[0] + (cpu->opcode & 0x0FFF);
            printf("Program counter register is set to V0 + last three bytes 0x%03X \n", cpu->opcode & 0x0FFF);
            break;
        // case 0xC000 ... 0xCFFF:
        case 0xC000:
            cpu->V[cpu->opcode & 0x0F00] = (rand() % 256) & (cpu->opcode & 0x00FF);
            printf("Generate a random number, then AND with last two bytes  0x%02X and store in Vx(0x%01X)\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
            break;
        // Draw
        case 0xD000: 
            printf("Read 0x%01X of bytes store in last byte starting at Index. Display at location (Vx 0x%01X,Vy 0x%01X), if collision Vf = 1, if off screen wrap around \n", cpu->opcode & 0x000F,
            cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
            break;
        // Key press
        case 0xE000:
            switch(cpu->opcode & 0x00FF){
                case 0x009E:
                    printf("Skip next instruction if key in Vx (0x%01X) is pressed, PC+=2\n", cpu->opcode & 0x0F00);
                    break;
                case 0x00A1:
                    printf("Skip next instruction if key in Vx (0x%01X) is not perssed, PC+=2\n", cpu->opcode & 0x0F00);
                    break;   
            }
            break;
        
        case 0xF000:
            switch(cpu->opcode & 0x00FF){
                case 0x0007:
                    printf("Vx 0x%01X  = Delay Timer\n", cpu->opcode & 0x0F00);
                    break;
                case 0x000A:
                    printf("Exection stops until a key is pressed, store value of key in Vx 0x%01X\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0015:
                    printf("Set delay timer to value of Vx 0x%01X (2nd byte)\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0018:
                    printf("Set sound timer to value of Vx 0x%01X (2nd byte)\n", cpu->opcode & 0x0F00);
                    break;
                case 0x001E:
                    printf("Index register + Vx 0x%01X (2nd byte) is stored in Index Register\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0029:
                    printf("Index register is set equal to the location for hexadecimal sprite corresponding to value in Vx 0x%01X\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0033:
                    printf("Store BCD of Vx 0x%01X in Index Register, Index Register+1, and Index Register+2\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0055:
                    printf("Store registers V0 - Vx 0x%01X (2nd byte) in in memory starting at address in Index register\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0065:
                    printf("Read values from memory starting at Index register and store them in V0 through Vx 0x%01X (2nd byte)\n", cpu->opcode & 0x0F00);
                    break;
            }
            break;
        }
    
}