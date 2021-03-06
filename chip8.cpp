#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <ios>
#include <fstream>

using std::streamsize;
using std::vector;
using std::ifstream;
using std::ios;


unsigned char chip8_fontset[80] = { 
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

chip8::chip8(){
	// empty
}

chip8::~chip8() {
	// empty
}

void chip8::initialize(){
        pc = 0x200; //Program counter starts at 0x200
        opcode = 0; //Reset current opcode
        I = 0; //Reset index register
        sp = 0; //Reset stack pointer
        
        //clear display
        for(int i = 0; i< 64*32; ++i)
            gfx[i] = 0;
            
        //clear stack
        for(int i = 0; i < 16; ++i)
            stack[i] = 0;
            
        //clear registers V0-VF
        for(int i = 0; i < 16; ++i)
            key[i] = V[i] = 0;
            
        //clear memory
        for(int i = 0; i< 4096; ++i)
            memory[i] = 0;
            
        //load fontset
        for(int i = 0; i < 80; ++i)
            memory[i] = chip8_fontset[i];
            
        //Reset timers
        delay_timer = 0;
        sound_timer = 0;
        
        drawFlag = true;
        
        srand(time(NULL));
        
}

void chip8::emulateCycle(){
   
    //Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    
    //Decode Opcode
    switch(opcode & 0xF000){
        case 0x0000:
            switch (opcode & 0x000F){
                case 0x0000: //opcode 00e0
                  for(int i = 0; i< 64*32; ++i){
                      gfx[i] = 0;
                  }
                  drawFlag = true;
                  pc += 2;
                break;
                    
                case 0x000E: //opcode 00ee
                  --sp;
                  pc = stack[sp];
                  pc += 2;
                break;
                
                default:
                    printf ("Unkown opcode: 0x%X\n", opcode);
            }
        break;
      
        case 0x1000: //opcode 1nnn
          pc = opcode & 0x0FFF;
        break;
      
        case 0x2000: //opcode 2nnn
          stack[sp] = pc;
          ++sp;
          pc = opcode & 0x0FFF;
        break;
      
        case 0x3000: //opcode 3xkk
          if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            pc += 4;
          
          else
            pc += 2;
        break;
      
        case 0x4000: //opcode 4xkk
          if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            pc += 4;
          
          else
            pc += 2;
        break;
      
        case 0x5000: //opcode 5xy0
          if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            pc += 4;
            
          else
            pc +=2;
        break;
      
        case 0x6000: //opcode 6xkk
          V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
          pc += 2;
        break;
      
        case 0x7000: //opcode 7xkk
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] +  (opcode & 0x00FF) ;
          pc += 2;
        break;
      
        case 0x8000:
            switch (opcode & 0x000F){
              
              case 0x0000: //opcode 8xy0
                V[(opcode & 0x0F00) >> 8] = V[ (opcode & 0x00F0) >> 4];
                pc += 2;
              break;
            
              case 0x0001: //opcode 8xy1
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                pc += 2;
              break;
            
              case 0x0002: //opcode 8xy2
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                pc += 2;
              break;
            
              case 0x0003: //opcode 8xy3
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                pc += 2;
              break;
              
              case 0x0004: // opcode 8xy4
                if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                  V[0xF] = 1; //carry
                  
                else
                  V[0xF] = 0;
                  
                V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                pc += 2;
              break;
            
              case 0x0005: //opcode 8xy5
                if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                  V[0xF] = 1;
                  
                else
                  V[0xF] = 0;
                  
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                pc += 2;
              break;
            
              case 0x0006: //opcode 8xy6
                V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                V[(opcode & 0x0F00) >> 8] >>= 1;
                pc += 2;
              break;
            
              case 0x0007: //opcode 8xy7
                if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
                  V[0xF] = 0;
                  
                else
                  V[0xF] = 1;
                  
                V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x00F0) >> 8];
                pc += 2;
              break;
            
              case 0x000E: //opcode 8xye
                V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                V[(opcode & 0x0F00) >> 8] <<= 1;
                pc += 2;
              break;
            
              default:
              printf ("Unkown opcode: 8x%X\n", opcode);
            }
        break;
            
        case 0x9000: //opcode 9xy0
          if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            pc +=4;
            
          else
            pc +=2;
        break; 
        
        case 0xA000: //opcode annn
            I = opcode & 0x0FFF;
            pc +=2;
        break;
      
        case 0xB000: //opcode bnnn
            pc = (opcode & 0x0FFF) + V[0x0];
        break;
      
        case 0xC000: //opcode cxkk
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            pc += 2;
        break;
      
        case 0xD000:{ // opcode dxyn
              unsigned short x = V[(opcode & 0x0F00) >> 8];
              unsigned short y = V[(opcode & 0x00F0) >> 4];
              unsigned short height = opcode & 0x000F;
              unsigned short pixel;
 
              V[0xF] = 0;
              for (int yline = 0; yline < height; yline++) {
                  pixel = memory[I + yline];
                  for(int xline = 0; xline < 8; xline++) {
                     if((pixel & (0x80 >> xline)) != 0) {
                        if(gfx[(x + xline + ((y + yline) * 64))] == 1){
                            V[0xF] = 1;                                   
                        }
                      gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                  }
                }
              drawFlag = true;
              pc += 2;
            }
        break;
      
        case 0xE000:{ 
            switch (opcode & 0x00FF){
              case 0x009E: //opcode ex9e
                if(key[V[(opcode & 0x0F00) >> 8]] == 1)
                  pc += 4;
                
                else
                  pc +=2;
              break;
            
              case 0x00A1: //opcode exa1
                if(key[V[(opcode & 0x0F00) >> 8]] == 0)
                  pc += 4;
                
                else
                  pc +=2;
              break;
            
              default:
              printf ("Unkown opcode: Ex%X\n", opcode);
            }
        }
        break;
        
        case 0xF000:
            switch (opcode & 0x00FF){
              case 0x0007: //opcode fx07
                V[(opcode & 0x0F00) >> 8] = delay_timer;
                pc += 2;
              break;
            
              case 0x000A:{ //opcode fx0A
                bool keypress = false;
                for(int i = 0; i< 16; ++i){
                    if (key[i] == 1) {
                        V[(opcode & 0x0F00) >> 8] = i;
                        keypress = true;
                    }
                }
                
                if(!keypress)
                    return; 
                pc += 2;
              } 
              break;
            
              case 0x0015: // opcode fx15
                 delay_timer = V[(opcode & 0x0F00) >> 8];
                 pc += 2;
              break;
                  
              case 0x0018: //opcode fx18
                sound_timer = V[(opcode & 0x0F00) >> 8];
                pc += 2;
              break;
                  
              case 0x001E: //opcode fx1e
                if( I + V[(opcode & 0x0F00) >> 8] > 0xFFF){
                  V[0xF] = 1;
                }
                else {
                  V[0xF] = 0;
                }
                I = I + V[(opcode & 0x0F00) >> 8];
                pc += 2;
              break;
                
              case 0x0029: //opcode fx29
                I = V[(opcode & 0x0F00) >> 8] *5;
                pc += 2;
              break;
            
              case 0x0033: //opcode fx33
                memory[I]  = V[(opcode & 0x0F00) >> 8] / 100;
                memory[I+1] = (V[(opcode & 0x0F00) >> 8] /10) % 10;
                memory[I+2] = (V[(opcode & 0xF00) >> 8] % 100) % 10;
                pc += 2;
              break;
            
              case 0x0055:{  //opcode fx55
                unsigned char X = ((opcode & 0x0F00) >> 8);
                for (unsigned char r = 0; r <= X; r++) {
                    memory[I+r] = V[r];
                }
                
                I = I + X + 1;
                pc += 2;
              }
              break;
            
              case 0x0065:{ //opcode fx65
                unsigned char X = ((opcode & 0x0F00) >> 8);
                for (unsigned char r = 0; r <= X; r++) {
                    V[r] = memory [I+r];  
                }
                
                I = I + X + 1;
                pc += 2;
              }
              break;
            
              default:
              printf ("Unkown opcode: Fx%X\n", opcode);
                
            }
        break;
      
        default:
        printf ("Unkown opcode: 0x%X\n", opcode);
    }
        
  //Update Timers
  if(delay_timer > 0)
    --delay_timer;
        
  if(sound_timer > 0){
    if (sound_timer == 1)
      --sound_timer;
  }
    
}


bool chip8::loadGame(const char* filename){
    initialize();
    ifstream rom(filename, ios::in | ios::binary | ios::ate);
    streamsize size = rom.tellg();
    rom.seekg(0, ios::beg);
    vector<char> buffer(size);
    
    if (rom.read(buffer.data(), size)){

      for (int i = 0; i <= buffer.size(); ++i) {
      	memory[0x200 + i] = buffer[i];
      }
      
    } else {
        printf("Couldn't load file");
        return false;
      }
      
    rom.close();
    return true;
}






