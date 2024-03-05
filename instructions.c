#include "instructions.h"
// 00E0 CLS - Clear the screen CLS.
void cls(Chip8State *state)
{
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            state->screen[i][j] = 0;
        }
    }
    state->PC += 2;
}

// 1NNN JMP NNN - Set PC to NNN.
void jmp_nnn(Chip8State *state, uint8_t *code)
{
    uint8_t nnn = ((code[0] & 0xF) << 8) | code[1];
    if (nnn == state->PC) 
    {
        printf("Infinite Loop Detected \n");
        state->pause = 1;
    }
    state->PC = nnn;
}

// 6XNN LD VX, NN - Store number NN in register VX.
void ld_vx_nn(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t nn = code[1];
    state->V[x] = nn;
    state->PC += 2;
}

// ANNN LD I, NNN - Set I equal to NNN.
void ld_i_nnn(Chip8State *state, uint8_t *code)
{
    uint16_t nnn = ((code[0] << 8) | code[1]) & 0x0FFF;
    state->I = nnn;
    state->PC += 2;
}

// DXYN	DRW VX, VY, N - Display N-byte sprite starting at memory location I at (VX, VY).
// Each set bit of xored with what's already drawn.
// VF is set to 1 if a collision occurs. 0 otherwise.
// https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ implementation for reference
void drw_vx_vy_n(Chip8State *state, uint8_t *code)
{
    int x = state->V[code[0] & 0xF];
    int y = state->V[code[1] & 0xF0 >> 4];
    int n = code[1] & 0xF;
    state->V[0xF] = 0; // collision 0
    // read n bytes of data from memory I as rows of sprite
    // bytes then displayed on state-> screen
    // xored if pixels already on screen set VF flag 1
    for (int i = 0; i < n; i++) // y rows
    {
        uint8_t sprite = state->memory[state->I+i];
        for (int j = 0; j < SPRITE_WIDTH; j++) // x columns
        {
            if ((sprite & (SPRITE_BIT >> j) != 0)) // check if current bit in sprite set to 1, sprites are 8 bits long ******** if yes draw
            {
                if (state->screen[y + i][x + j] == 1) // check VF collision flag
                {
                    state->V[0xF] = 1;
                }
                state->screen[y + i][x + j] ^= 1; //XORed
            }
        }
    }
    state->drawflag = 1;
    state->PC += 2;
}