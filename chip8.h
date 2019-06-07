class chip8{
    private:
        unsigned short opcode;
        unsigned short I; //Index Register
        unsigned short pc; //Program counter
        unsigned short sp; //stack pointer
        
        unsigned char delay_timer;
        unsigned char sound_timer;
        
        unsigned char memory[4096];
        unsigned char V[16]; //cpu registers
        unsigned short stack[16];
        
        void initialize();
      
        
    public:
        chip8();
        ~chip8();
        
        bool drawFlag;
        
        void emulateCycle();
        void debugRender();
        bool loadGame(const char* filename);
        
        unsigned char gfx[64*32];
        unsigned char key[16];
           
};
