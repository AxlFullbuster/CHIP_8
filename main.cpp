#include "chip8.h"
#include <stdio.h>
#include <iostream>
#include<SDL2/SDL.h>

using std::cout;
using std::endl;

chip8 mychip8;
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Texture* texture = NULL;
SDL_Renderer* renderer = NULL;
const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;

bool init() {
    
    //Initalization flag
    bool success = true;
    
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf ( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
	}
    
    //create window
    window = SDL_CreateWindow("CHIP_8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    //create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    }
    
    return success;
}

void close() {
    SDL_DestroyTexture(texture);
    texture = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    
    SDL_Quit();
}

int main(int argc, char**argv) {
    
    if(argc < 2) {
        cout << "Please provide a rom file to open." << endl;
        return 1;
    }
    
    if(!init()){
        printf("Failed to initialize!");
    } else {
        bool quit = false;
        mychip8.loadGame(argv[1]);
        
        if(!mychip8.loadGame(argv[1]))
            return 1;
    
        SDL_Event e;
        while(!quit) {
            while( SDL_PollEvent( &e ) != 0 ) {
                //User requests quit
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN){
                    switch(e.key.keysym.sym){
                        case SDLK_KP_1:
                            mychip8.key[0x1] = 1;
                        break;
                
                        case SDLK_KP_2:
                            mychip8.key[0x2] = 1;
                        break;
            
                        case SDLK_KP_3:
                            mychip8.key[0x3] = 1;
                        break;
            
                        case SDLK_KP_4:
                            mychip8.key[0xC] = 1;
                        break;
            
                        case SDLK_q:
                            mychip8.key[0x4] = 1;
                        break;
            
                        case SDLK_w:
                            mychip8.key[0x5] = 1;
                        break;
            
                        case SDLK_e:
                            mychip8.key[0x6] = 1;
                        break;
            
                        case SDLK_r:
                            mychip8.key[0xD] = 1;
                        break;
            
                        case SDLK_a:
                            mychip8.key[0x7] = 1;
                        break;
            
                        case SDLK_s:
                            mychip8.key[0x8] = 1;
                        break;
            
                        case SDLK_d:
                            mychip8.key[0x9] = 1;
                        break;
            
                        case SDLK_f:
                            mychip8.key[0xE] = 1;
                        break;
            
                        case SDLK_z:
                            mychip8.key[0xA] = 1;
                        break;
                
                        case SDLK_x:
                            mychip8.key[0x0] = 1;
                        break;
            
                        case SDLK_c:
                            mychip8.key[0xB] = 1;
                        break;
            
                        case SDLK_v:
                            mychip8.key[0xF] = 1;
                        break;     
                    }
                }
                else if(e.type == SDL_KEYDOWN) {
                    switch(e.key.keysym.sym){
                        case SDLK_KP_1:
                            mychip8.key[0x1] = 0;
                        break;
            
                        case SDLK_KP_2:
                            mychip8.key[0x2] = 0;
                        break;
            
                        case SDLK_KP_3:
                            mychip8.key[0x3] = 0;
                        break;
            
                        case SDLK_KP_4:
                            mychip8.key[0xC] = 0;
                        break;
            
                        case SDLK_q:
                            mychip8.key[0x4] = 0;
                        break;
            
                        case SDLK_w:
                            mychip8.key[0x5] = 0;
                        break;
            
                        case SDLK_e:
                            mychip8.key[0x6] = 0;
                        break;
            
                        case SDLK_r:
                            mychip8.key[0xD] = 0;
                        break;
            
                        case SDLK_a:
                            mychip8.key[0x7] = 0;
                        break;
            
                        case SDLK_s:
                            mychip8.key[0x8] = 0;
                        break;
            
                        case SDLK_d:
                            mychip8.key[0x9] = 0;
                        break;
            
                        case SDLK_f:
                            mychip8.key[0xE] = 0;
                        break;
            
                        case SDLK_z:
                            mychip8.key[0xA] = 0;
                        break;
            
                        case SDLK_x:
                            mychip8.key[0x0] = 0;
                        break;
            
                        case SDLK_c:
                            mychip8.key[0xB] = 0;
                        break;
            
                        case SDLK_v:
                            mychip8.key[0xF] = 0;
                        break;     
                    }
                }
            }
        }
    
            mychip8.emulateCycle();
        
            //If the draw flag is set, update the screen
            if(mychip8.drawFlag){
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(renderer);
                int x = 0;
                int y = 0;
                for(int i = 0; i < 64*32; ++i){
                    if(i % 64 == 0){
                        x += 9;
                        y = 0;
                    }
                    if(mychip8.gfx[i] == 1){
                        SDL_Rect fillRect = {x, y, 10, 10};
                        SDL_SetRenderDrawColor(renderer, 0x00,0xFF,0x00,0xFF);
                        SDL_RenderFillRect(renderer, &fillRect);
                    }
                    y += 9;
                }
                SDL_RenderPresent(renderer);
                mychip8.drawFlag = false;
            }
      
    }
    close();
    return 0;
}


