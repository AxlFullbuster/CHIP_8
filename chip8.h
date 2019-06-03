#ifdef CHIP8_H
#define CHIP8_H

class chip8{
    private:
        unsigned short opcode;
        unsigned short I;
        unsigned short pc;
        unsigned short sp;
        
        unsigned char delay_timer;
        unsigned char sound_timer;
        
        unsigned char memory[4096];
        unsigned char V[16];
        unsigned short stack[16];
        
        void initialize();
      
        
    public:
        chip8();
        ~chip8();
        
        bool drawFlag;
        
        void emulateCycle();
        void debugRender();
        bool loadApplication(const char* filename)
        
        unsigned char gfx[64*32];
        unsigned char key[16];
           
};

#endif