#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_V_REGISTERS 16
#define TOTAL_RAM 4096
#define STACK_SIZE 16
#define STACK_START 0xea0 // (0xEA0-0xEFF) were reserved for the call stack,
#define DISPLAY_BUFFER 0Xf00 //Display buffer is in RAM at 0xF00.
#define PC_START 0x200
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32


/* 
V - 16 8 bit registers v0 v1 ... vF
I - Memory address
SP - stack pointer
PC - program counter
RAM starts at 0x200
 */
typedef struct Chip8State {
    uint8_t V[NUM_V_REGISTERS];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;
    uint8_t delay;
    uint8_t sound;
    uint8_t *memory;
    uint8_t *screen;
} Chip8State;
