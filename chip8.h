#ifndef Chip8_H
#define Chip8_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define NUM_V_REGISTERS 16
#define STACK_SIZE 16
#define TOTAL_RAM 4096
#define STACK_SIZE 16
#define STACK_START 0xea0 // (0xEA0-0xEFF) were reserved for the call stack,
#define DISPLAY_BUFFER 0Xf00 //Display buffer is in RAM at 0xF00.
#define PC_START 0x200
#define DECIMAL_PC_START 512
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SPRITE_WIDTH 8
#define SPRITE_BIT 0x80
#define MONITOR_WIDTH 640
#define MONITOR_HEIGHT 480
#define OVERFLOW 255

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
    uint16_t stack[STACK_SIZE];
    uint32_t screen[SCREEN_HEIGHT * SCREEN_WIDTH];
    uint8_t drawflag;
    uint8_t pause;
} Chip8State;

Chip8State* initiate(void);
void EmulateChip8(Chip8State *state);
#endif 