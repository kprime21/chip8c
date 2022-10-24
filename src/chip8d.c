#include "stdio.h"
#include "stdlib.h"

//dissassemble chip8 roms and then print their two byte addresses out 
int main(){


    
    FILE *fp, *fr;
    fp = fopen("./roms/PONG", "rb");
    fseek(fp, 0 , SEEK_END);
    long romSize = ftell(fp);
    unsigned char buffer[romSize]; 
    rewind(fp);
    printf("%d\n", romSize);
    fread(buffer,sizeof(buffer),1,fp);

    int addr = 0x200;
    fr = fopen("pong.txt", "w+");
    for(int i = 0; i < romSize; i+=2){
        fprintf(fr, "0x%04X - 0x%02X%02X\n", addr, buffer[i], buffer[i+1]);
        addr+=16;
    }
    fclose(fr);

    return 0;
}