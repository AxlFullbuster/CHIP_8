#include  <stdio.h>
#include "chip8.h"

chip8 mychip8;

int main(int argc, char**argv) {
    //set up render system and register input callbacks
    setupGraphics();
    setupInput();
    
    //Initialize the chip8 system and load the game into memory
    mychip8.initialize();
    mychip8.loadGame("pong");
    
    //Emulation loop
    for(;;){
        //Emulate one cycle
        mychip8.emulateCycle();
        
        //If the draw flag is set, update the screen
        if(mychip8.drawFlag)
            drawGraphics();
        
        //Store key press state (press and release)
        mychip8.setKeys();
    }
    return 0;
}

