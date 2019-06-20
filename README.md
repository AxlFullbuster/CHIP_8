This is a small project that focuses on creating a CHIP-8 Emulator.

It uses SDL to display the games and currently only works on linux machines. Windows support will be added sometime in the future.

Compiling:  
It uses Cmake to compile so this is what you'll need to do  
```
cmake .
make
./CHIP_8 *Rom file name*
```

The rom file has to be in the same directory as the executable.

Current Status:  
Debugging and Testing

References:  
How to write an emulator: http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
Cowgod's Chip8 Techinical Reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Fx0A
LazyFoo Productions: http://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL
