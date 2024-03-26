CHIP8
____________

CHIP-8 Interpreter written in C.

CHIP-8 is an interpreted minimalist programming language that was designed for use on the RCA COSMAC VIP computer.
Due to its hexadecimal format, it was best suited to machines with a scarcity of memory, as minimal text processing had to be performed by the interpreter before a program could be executed.

SETUP
___________
- Download SDL2 folder
- Install MinGW32 
- Add mingw32 into env variables
- Paste SDL2 Include and bin contents into MinGW32 contents folder respectively
- Paste SDL2.dll into Chip8 source folder 
- run cmd mingw32-make
- run ./chip8_emulator <rom> <debug>
- clean run mingw32-make clean (on windows system use del cmd if not use rm -f *.o)


Tutorial/References
_____________
- http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- www.emulator101.com
- https://en.wikipedia.org/wiki/CHIP-8
- https://lazyfoo.net/tutorials/SDL/
- https://wiki.libsdl.org/SDL2/FrontPage
- https://github.com/trapexit/chip-8_documentation
- https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file
