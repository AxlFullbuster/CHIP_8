#include <stdio.h>
#include //graphics
#include "chip8.h"

0x000-0x1FF //Chip8 interpreter
0x050-0x0A0 // 4x5 font set
0x200-0xFFF //Program ROM and work RAM

unsigned char chip8_fontset[80] =
{ 
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

void chip8::initialize(){
        pc = 0x200; //Program counter starts at 0x200
        opcode = 0; //Reset current opcode
        I = 0; //Reset index register
        sp = 0; //Reset stack pointer
        
        //clear display
        //clear stack
        //clear registers V0-VF
        //clear memory
        
        //load fontset
        for(int i = 0; i < 80; ++i)
            memory[i] = chip8_fontset[i];
            
        //Reset timers
}

void chip8::emulateCycle(){
    //Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    //Decode Opcode
    switch(opcode & 0xF000){
        case 0x0A000: //ANNN sets I to the register NNN
            //Execute Opcode
            I = opcode & 0x0FFF;
            pc +=2;
        break;
            
        case 0x0000:
            switch (opcode & 0x000F){
                case 0x0000: //0x00E0: clears the screen
                     //Execute Opcode
                break;
                    
                case 0x000E: //0x00EE: returns from subroutine
                        //Execute Opcode
                break;
                
                default:
                    printf("Unkwn opcode [0x0000]: 0x%X\n", opcode);
            }
        break;
        
        //more opcodes
            
        default:
            printf ("Unkown opcode: 0x%X\n", opcode);
    }
        
    //Update Timers
    if(delay_timer > 0)
        --delay_timer;
        
    if(sound_timer > 0){
        if (sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }

void loadApplication(const char* filename){
    
}
}