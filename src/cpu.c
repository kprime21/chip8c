#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "graphics.h"
//registers here


//ram 4k
unsigned char memory[0x1000];
//graphics screen
unsigned char gfx[64 * 32];


//set addressing registers to 0
void initialize(chip8 *cpu){
    //clear pc, index, opcode
    cpu->program_counter = 0x000;
    cpu->index = 0x00;
    cpu->opcode = 0x00;
     //clear stack
    for(int i = 0; i< 0x10; i++){
        cpu->stack[i] = 0;
    }
    //clear stack pointer
    cpu->stackptr = 0;
    //clear graphics
    for(int i = 0; i < 2048; i++){
        gfx[i] = 0;
    }
    //clear ram
    for(int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    //clear GP registers
    for(int i = 0; i < 0x10; i++){
        cpu->V[i] = 0;
    }
    //set keyboard
    for(int i = 0; i < 0x10; i++){
        cpu->key[i] = 0;
    }
    //set flags
    cpu->draw_flag = 0;
    cpu->pause_flag = 0;
    cpu->key_flag = 0;
    cpu->run_flag = 1;
    //set timers
    cpu->delay_timer = 0;
    cpu->sound_timer = 0;
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
                    //2048 is size
                    for(int i = 0; i<2048; i++){
                        gfx[i] = 0;
                    }
                    printf("CLS, clear the display\n");
                    break;
                case 0x00EE:
                    cpu->program_counter = cpu->stack[cpu->stackptr];
                    cpu->stackptr--;
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
            if(cpu->V[(cpu->opcode & 0x0F00)>>8] == cpu->opcode & 0x00FF){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X == 0x%02X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00FF);
            break;
        case 0x4000: //... 0x4FFF:
        if(cpu->V[(cpu->opcode & 0x0F00)>>8] != cpu->opcode & 0x00FF){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X != 0x%02X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00FF);
            break;
            //Vx is 2nd byte and Vy is third byte
        case 0x5000: //... 0x5FF0:
            if(cpu->V[(cpu->opcode & 0x0F00) >> 8] != cpu->V[(cpu->opcode & 0x00F0) >> 4]){
                cpu->program_counter+=2;
            }
            printf("skip next instruction if Vx 0x%01X == Vy 0x%01X\n", cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
            break;
        case 0x6000: //... 0x6FFF:
           
            cpu->V[(cpu->opcode & 0x0F00) >> 8] = (cpu->opcode & 0x00FF );
            printf("Load  0x%02X into Vx 0x%01X\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
           
            break;
        case 0x7000: //... 0x7FFF:
            cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] + (cpu->opcode & 0x00FF ); 
            printf("add 0x%02X to register Vx 0x%01X and store in Vx\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
            break;
        // case 0x8000 ... 0x8FF0:
        case 0x8000:
            switch(cpu->opcode & 0x000F){
            case 0x0000:
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4];
                printf("store Vy 0x%01X into register Vx 0x%01X\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8001 ... 0x8FF1:
            case 0x0001:
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] | cpu->V[(cpu->opcode & 0x00F0) >> 4];
                printf("Vx 0x%01X OR Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8002 ... 0x8FF2:
            case 0x0002:
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] & cpu->V[(cpu->opcode & 0x00F0) >> 4];
                printf("Vx 0x%01X  AND Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8003 ... 0x8FF3:
            case 0x0003:
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] ^ cpu->V[(cpu->opcode & 0x00F0) >> 4];
                printf("Vx 0x%01X XOR Vy 0x%01X and store in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8004 ... 0x8FF4:
            case 0x0004:
                int sum = (cpu->V[(cpu->opcode & 0x0F00) >> 8] + cpu-> V[(cpu->opcode & 0x00F0) >> 4]);
                if( sum > 255){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = sum & 0xFF;
                
                printf("Vx 0x%01X + Vy 0x%01X, overflow set Vf to 1, store 8 lower bits in Vx\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8005 ... 0x8FF5:
            case 0x0005:
                if(cpu->V[(cpu->opcode & 0x0F00) >> 8] > cpu-> V[(cpu->opcode & 0x00F0) >>4] ){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] - cpu-> V[(cpu->opcode & 0x00F0) >> 4];
                printf("Vx 0x%01X = Vy 0x%01X - Vx, Vx>Vy set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8006 ... 0x8FF6:
            case 0x0006:
                if(cpu->V[(cpu->opcode & 0x0F00) >>8] & 0x0001 == 0x0001){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] >> 1;
                
                printf("Vx 0x%01X >> Vy 0x%01X, if LSB of Vx is 1, set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x8007 ... 0x8FF7:
            case 0x0007:
                if(cpu->V[(cpu->opcode & 0x00F0) >> 4] > cpu-> V[(cpu->opcode & 0x0F00) >> 8]){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x00F0) >> 4] = cpu->V[(cpu->opcode & 0x00F0) >> 4] - cpu-> V[(cpu->opcode & 0x0F00) >> 8];
                printf("Vx 0x%01X = Vy 0x%01X - Vx, Vy>Vx set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            // case 0x800E ... 0x8FFE:
            case 0x000E:
                if(cpu->V[(cpu->opcode & 0x0F00) >> 8] & 0x8000 == 0x8000){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] << 1;
                printf("Vx 0x%01X = Vx << Vy 0x%01X, if MSB of Vx is 1, set Vf to 1\n",cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
                break;
            default:
                printf("Unkown opcode, exiting... \n");
            }
            break;
        // case 0x9000 ... 0x9FF0:
        case 0x9000:
            if(cpu->V[(cpu->opcode & 0x00F0) >> 4] != cpu-> V[(cpu->opcode & 0x0F00) >> 8]){
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
            cpu->V[(cpu->opcode & 0x0F00) >> 8] = (rand() % 256) & (cpu->opcode & 0x00FF);
            printf("Generate a random number, then AND with last two bytes  0x%02X and store in Vx(0x%01X)\n", cpu->opcode & 0x00FF,cpu->opcode & 0x0F00);
            break;
        // Draw
        case 0xD000: 
            unsigned short posx = cpu->V[(cpu->opcode & 0x0F00)>>8];
            unsigned short posy = cpu->V[(cpu->opcode & 0x00F0)>>4];
            unsigned short height = cpu->opcode & 0x000F;
            unsigned short pixel;
            printf("these are the values 0x%01x 0x%01x %d \n", posx, posy, cpu->index);
            cpu->V[0xF] = 0;
            
            
           for(int y=0; y<height; y++){

                pixel = memory[cpu->index + y];
                for(int x=0; x < 8; x++){
                    
                    if((pixel & (0x80 >> x)) != 0){
                        if(gfx[64*(posy+y) + (posx+x)] == 1){
                            cpu->V[0xF] = 1;
                        }
                        gfx[64*(posy+y) + (posx+x)] ^=1;
                    }
                }
           }
            cpu->draw_flag = 1;
            printf("Read 0x%01X of bytes store in last byte starting at Index. Display at location (Vx 0x%01X,Vy 0x%01X), if collision Vf = 1, if off screen wrap around \n", cpu->opcode & 0x000F,
            cpu->opcode & 0x0F00, cpu->opcode & 0x00F0);
             
            break;
        // Key press
        case 0xE000:
            switch(cpu->opcode & 0x00FF){
                case 0x009E:
                     if (cpu->key[cpu->V[(cpu->opcode & 0x0F00) >> 8]] != 0){
                        cpu->program_counter += 2;
                     }
                        
                    printf("Skip next instruction if key in Vx (0x%01X) is pressed, PC+=2\n", cpu->opcode & 0x0F00);
                    break;
                case 0x00A1:
                    if (cpu->key[cpu->V[(cpu->opcode & 0x0F00) >> 8]] == 0){
                            cpu->program_counter += 2;
                        }
                    printf("Skip next instruction if key in Vx (0x%01X) is not perssed, PC+=2\n", cpu->opcode & 0x0F00);
                    break;   
            }
            break;
        case 0xF000:
            switch(cpu->opcode & 0x00FF){
                case 0x0007:
                    cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->delay_timer;
                    printf("Vx 0x%01X  = Delay Timer\n", cpu->opcode & 0x0F00);
                    break;
                case 0x000A:
                    cpu->key_flag = 0;
                    for(int i = 0; i < 0x10; i++){
                        if(cpu->key[i]){
                            cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->key[i];
                            cpu->key_flag = 1;
                        }
                    }
                    printf("Exection stops until a key is pressed, store value of key in Vx 0x%01X\n", cpu->V[cpu->opcode & 0x0F00]);
                    break;
                case 0x0015:
                    cpu->delay_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    printf("Set delay timer to cpu->value of Vx 0x%04X (2nd byte)\n", cpu->V[(cpu->opcode & 0x0F00) >> 8]);
                    break;
                case 0x0018:
                    cpu->sound_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    printf("Set sound timer to value of Vx 0x%01X (2nd byte)\n", cpu->opcode & 0x0F00);
                    break;
                case 0x001E:
                    cpu->index = cpu->index + cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    printf("Index register + Vx 0x%01X (2nd byte) is stored in Index Register\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0029:
                    cpu->index =  cpu->V[(cpu->opcode & 0x0F00) >> 8] * 0x5;
                    printf("Index register is set equal to the location for hexadecimal sprite corresponding to value in Vx 0x%01X\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0033:
                    memory[cpu->index] = cpu->V[(cpu->opcode & 0x0F00) >> 8]/100;
                    memory[cpu->index + 1] = ((cpu->V[(cpu->opcode & 0x0F00) >> 8])/10)%10;
                    memory[cpu->index + 2] = ((cpu->V[(cpu->opcode & 0x0F00) >> 8])/100)%10;
                    printf("Store BCD of Vx 0x%01X in Index Register, Index Register+1, and Index Register+2\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0055:
                    for(int i = 0; i < cpu->V[(cpu->opcode & 0x0F00)>>8]; i++){
                        memory[cpu->index + i] = cpu->V[i];
                    }
                    printf("Store registers V0 - Vx 0x%01X (2nd byte) in in memory starting at address in Index register\n", cpu->opcode & 0x0F00);
                    break;
                case 0x0065:
                    for(int i = 0; i < cpu->V[(cpu->opcode & 0x0F00)>>8]; i++){
                        cpu->V[i] = memory[cpu->index + i];
                    }
                    printf("Read values from memory starting at Index register and store them in V0 through Vx 0x%01X (2nd byte)\n", cpu->opcode & 0x0F00);
                    break;
            }
            break;
        }
        //decrement the delay timer 
        if (cpu->delay_timer > 0)
            --cpu->delay_timer;
            printf("this is delay timer %d\n", cpu->delay_timer);
        //decrement the sound timer
        if (cpu->sound_timer > 0)
        {
            if (cpu->sound_timer == 1){
                //fprintf(stdout, "\aBeep!\n" );
                printf("beep\n");
            }
            --cpu->sound_timer;
            printf("this is sound timer %d\n", cpu->sound_timer);
        } 
}
