#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include <time.h>

//ram 4k
unsigned char memory[RAM];
//graphics screen
unsigned char gfx[WIDTH * HEIGHT];


//set addressing registers to 0
void initialize(chip8 *cpu){
    //clear pc, index, opcode
    cpu->program_counter = 0x200;
    cpu->index = 0x00;
    cpu->opcode = 0x00;
     //clear stack
    for(int i = 0; i< 0x10; i++){
        cpu->stack[i] = 0;
    }
    //clear stack pointer
    cpu->stackptr = 0;
    //clear graphics
    for(int i = 0; i < PIXEL_MATRIX; i++){
        gfx[i] = 0;
    }
    //clear ram
    for(int i = 0; i < RAM; i++){
        memory[i] = 0;
    }
    //clear GP registers
    for(int i = 0; i < V_REG_NUM; i++){
        cpu->V[i] = 0;
    }
    //set keyboard
    for(int i = 0; i < NUM_KEY; i++){
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
    for(int i = 0; i < FONT_SIZE; i++){
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
    unsigned char* temp = memory + PC_START;
    fread(temp, sizeof(sizeof(temp)), size, fp);
    fclose(fp);
}
    
    
//fetch, decode, and execute cycle
void cycle(chip8 *cpu){
    //fetch
    cpu->opcode = memory[cpu->program_counter] << 8 | memory[cpu->program_counter + 1];
    //decode and execute
    switch(cpu->opcode & 0xF000){
        
        case 0x0000:
            switch(cpu->opcode & 0x00FF){
                case 0x00E0: //0x00E0 Clear Display
                    for(int i = 0; i<PIXEL_MATRIX; i++){
                        gfx[i] = 0;
                    }
                    cpu->draw_flag = 1;
                    cpu->program_counter +=2;
                    break;
                case 0x00EE: //0x00EE return from subroutine
                    --cpu->stackptr;
                    cpu->program_counter = cpu->stack[cpu->stackptr];
                    cpu->program_counter +=2;
                    break;
                default:
                    break;
            }
            break;
        case 0x1000: //0x1NNN Jump to 0x0NNN
            cpu->program_counter = cpu->opcode & 0x0FFF;
            break;
        case 0x2000: //0x2NNN store PC in stack, increment stackptr, store 0x0NNN in PC:
            cpu->stack[cpu->stackptr] = cpu->program_counter;
            cpu->stackptr++;
            cpu->program_counter = cpu->opcode & 0x0FFF;
            break;
        case 0x3000: //0x3XKK skip next instruction if register VX == 0x00KK:
            if(cpu->V[(cpu->opcode & 0x0F00)>>8] == (cpu->opcode & 0x00FF)){
                cpu->program_counter+=2;
            }
            cpu->program_counter +=2;
            break;
        case 0x4000: //0x4XKK skip next instruction if VX != 0x00KK:
        if(cpu->V[(cpu->opcode & 0x0F00)>>8] != (cpu->opcode & 0x00FF)){
                cpu->program_counter+=2;
            }
            cpu->program_counter +=2;
            break;
            case 0x5000: //0x5XY0 skip next instruction if VX == VY:
            if(cpu->V[(cpu->opcode & 0x0F00) >> 8] == cpu->V[(cpu->opcode & 0x00F0) >> 4]){
                cpu->program_counter+=2;
            }
            cpu->program_counter +=2;
            break;
        case 0x6000: //0x6XKK load 0x0NNN into VX
            cpu->V[(cpu->opcode & 0x0F00) >> 8] = (cpu->opcode & 0x00FF );
            cpu->program_counter +=2;
            break;
        case 0x7000: //0x7XKK VX+=0x00KK
            cpu->V[(cpu->opcode & 0x0F00) >> 8] += (cpu->opcode & 0x00FF );
            cpu->program_counter +=2;
            break;
        case 0x8000:
            switch(cpu->opcode & 0x000F){
            case 0x0000: //0x8XY0 VX=VY 
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4];
                cpu->program_counter +=2;
                break;
            case 0x0001: //0x8XY1 VX|=VY
                cpu->V[(cpu->opcode & 0x0F00) >> 8] |= cpu->V[(cpu->opcode & 0x00F0) >> 4];
                cpu->program_counter +=2;
                break;
            case 0x0002: //0x8XY2  VX&=VY
                cpu->V[(cpu->opcode & 0x0F00) >> 8] &= cpu->V[(cpu->opcode & 0x00F0) >> 4];
                cpu->program_counter +=2;
                break;
            case 0x0003: //0x8XY3 VX^=VY
                cpu->V[(cpu->opcode & 0x0F00) >> 8] ^= cpu->V[(cpu->opcode & 0x00F0) >> 4];
                cpu->program_counter +=2;
                break;
            case 0x0004: //0x8XY4 sum=VX+VY, if sum>255 -> VF=1, VX=sum*0x00FF 
                int sum = (cpu->V[(cpu->opcode & 0x0F00) >> 8] + cpu-> V[(cpu->opcode & 0x00F0) >> 4]);
                if( sum > 255){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = sum & 0xFF;
                cpu->program_counter +=2;
                break;
            case 0x0005: //0x8XY5 VX=VX-VY, if VX>VY -> VF=1 else VF=0
                if(cpu->V[(cpu->opcode & 0x0F00) >> 8] > cpu-> V[(cpu->opcode & 0x00F0) >>4] ){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] -= cpu-> V[(cpu->opcode & 0x00F0) >> 4];
                cpu->program_counter +=2;
                break;
            case 0x0006: //0x8XY6 VX>>=1, if VX*0x0001==1 -> VF=1 else VF=0
                if(cpu->V[(cpu->opcode & 0x0F00) >>8] & 0x0001 == 0x0001){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] >> 1;
                cpu->program_counter +=2;
                break;
            case 0x0007: //0x8XY7 VX=VX-VY, if VY>VX -> VF=1 else VF=0
                if(cpu->V[(cpu->opcode & 0x00F0) >> 4] > cpu-> V[(cpu->opcode & 0x0F00) >> 8]){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x00F0) >> 4] - cpu-> V[(cpu->opcode & 0x0F00) >> 8];
                cpu->program_counter +=2;
                break;
            case 0x000E: //0x8XYE VX>>=1, if VX*0x1000 -> VF=1 else VF=0
                if(cpu->V[(cpu->opcode & 0x0F00) >> 8] & 0x8000 == 0x8000){
                    cpu->V[0x0F] = 1;
                }
                else{
                    cpu->V[0x0F] = 0;
                }
                cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->V[(cpu->opcode & 0x0F00) >> 8] << 1;
                cpu->program_counter+=2;
                break;
            default:
                break;
            }
            break;
        case 0x9000: //0x9XY0 if VX==VY -> PC+=2
            if(cpu->V[(cpu->opcode & 0x00F0) >> 4] != cpu-> V[(cpu->opcode & 0x0F00) >> 8]){
                    cpu->program_counter +=2;
            }
            cpu->program_counter +=2;
            break;
        case 0xA000: //0xANNN IR=0x0NNN
            cpu->index = cpu->opcode & 0x0FFF;
            cpu->program_counter +=2;
            break;
        case 0xB000: //0xBNNN PC=0x0NNN+V0
            cpu->program_counter = cpu->V[0] + (cpu->opcode & 0x0FFF);
            break;
        case 0xC000: //0xCXKK VX=RAND()&0x00KK
            srand(time(NULL)); //generate random blocks with time 
            cpu->V[(cpu->opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (cpu->opcode & 0x00FF);
            cpu->program_counter +=2;
            break;
        // Draw
        case 0xD000: //0xDXYN for N Bytes, draw Memory[IR+0x000N] at (VX,VY)
            unsigned short posx = cpu->V[(cpu->opcode & 0x0F00)>>8];
            unsigned short posy = cpu->V[(cpu->opcode & 0x00F0)>>4];
            unsigned short height = cpu->opcode & 0x000F;
            unsigned short pixel;
            cpu->V[0xF] = 0;
           for(int y=0; y<height; y++){
                pixel = memory[cpu->index + y];
                for(int x=0; x < 8; x++){
                    
                    if((pixel & (0x80 >> x)) != 0){
                        if(gfx[WIDTH*(posy+y) + (posx+x)] == 1){
                            cpu->V[0xF] = 1;
                        }
                        gfx[WIDTH*(posy+y) + (posx+x)] ^=1;
                    }
                }
           }
            cpu->draw_flag = 1;
            cpu->program_counter +=2; 
            break;
        // Key press
        case 0xE000: 
            switch(cpu->opcode & 0x00FF){
                case 0x009E: //0xEX9E if VX=KeyPress -> PC+=2
                     if (cpu->key[cpu->V[(cpu->opcode & 0x0F00) >> 8]] != 0){
                        cpu->program_counter += 2;
                     }
                     cpu->program_counter +=2;
                    break;
                case 0x00A1: //0xEXA1 if VX!=KeyPress -> PC+=2
                    if (cpu->key[cpu->V[(cpu->opcode & 0x0F00) >> 8]] == 0){
                            cpu->program_counter += 2;
                        }
                    cpu->program_counter +=2;
                    break;   
                default:
                    break;
            }
            break;
        case 0xF000:
            switch(cpu->opcode & 0x00FF){
                case 0x0007: //0xFX07 VX=DelayTimer
                    cpu->V[(cpu->opcode & 0x0F00) >> 8] = cpu->delay_timer;
                    cpu->program_counter +=2;
                    break;
                case 0x000A: //0xFX0A Pause, if KeyPress -> VX=KeyPress and Unpause
                    cpu->key_flag = 0;
                    for(int i = 0; i < 0x10; i++){
                        if(cpu->key[i]){
                            cpu->V[(cpu->opcode & 0x0F00) >> 8] = i;
                            cpu->key_flag = 1;
                        }     
                    }
                    if(cpu->key_flag == 0){
                        break;
                    }
                    cpu->program_counter +=2;
                    break;
                case 0x0015: //0xFX15 DelayTimer=VX
                    cpu->delay_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    cpu->program_counter +=2;
                    break;
                case 0x0018: //0xFX18 SoundTimer=VX
                    cpu->sound_timer = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    cpu->program_counter +=2;
                    break;
                case 0x001E: //0xFX1E IR+=VX
                    cpu->index = cpu->index + cpu->V[(cpu->opcode & 0x0F00) >> 8];
                    cpu->program_counter +=2;
                    break;
                case 0x0029: //0xFX29 IR=Sprite at VX
                    cpu->index =  cpu->V[(cpu->opcode & 0x0F00) >> 8] * 0x5;
                    cpu->program_counter +=2;
                    break;
                case 0x0033: //0xFX33 (IR, IR+1, IR+2)=BCD(VX), 
                    memory[cpu->index] = cpu->V[(cpu->opcode & 0x0F00) >> 8]/100;
                    memory[cpu->index + 1] = ((cpu->V[(cpu->opcode & 0x0F00) >> 8])/10)%10;
                    memory[cpu->index + 2] = ((cpu->V[(cpu->opcode & 0x0F00) >> 8])%100)%10;
                    cpu->program_counter +=2;
                    break;
                case 0x0055: //FX55 For N from 0 to X, Memory[IR+N]=VN
                    for(int i = 0; i <= ((cpu->opcode & 0x0F00)>>8); i++){
                        memory[cpu->index + i] = cpu->V[i];
                    }
                    cpu->index = ((cpu->opcode & 0x0F00) >> 8) + 1;
                    cpu->program_counter +=2;
                    break;
                case 0x0065: //FX65 For N from 0 to X, VN=Memory[IR+N]
                    for(int i = 0; i <= ((cpu->opcode & 0x0F00)>>8); i++){
                        cpu->V[i] = memory[cpu->index + i];
                    }
                    cpu->index = ((cpu->opcode & 0x0F00) >> 8) + 1;
                    cpu->program_counter +=2;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        }
        
}

void dec_timer(chip8* cpu){
    if (cpu->delay_timer > 0)
    {
            --cpu->delay_timer;
    }
    if (cpu->sound_timer > 0)
    {
        if (cpu->sound_timer == 1){
            printf("\a");
        }
        --cpu->sound_timer;
    } 
}

void process_key(chip8* cpu){
    SDL_Event event;
    while(SDL_PollEvent(&event)){   
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    cpu->run_flag = 0;
                    break;      
                case SDLK_SPACE:
                    cpu->pause_flag = cpu->pause_flag == 1 ? 0 : 1;
                    break;
                default:
                    break;
            }

            for(int i = 0; i < 16; i++){
                if(event.key.keysym.sym == keyboard[i]){
                    cpu->key[i] = 1;
                }
            }
        }
        if(event.type == SDL_KEYUP){
            for(int i = 0;i<16; i++){
                if(event.key.keysym.sym == keyboard[i]){
                    cpu->key[i] = 0;
                }
            }
        }
    }
}