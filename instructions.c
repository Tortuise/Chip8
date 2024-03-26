#include "instructions.h"

// 00E0 CLS - Clear the screen CLS.
void cls(Chip8State *state)
{
    // for (int i = 0; i < SCREEN_HEIGHT; i++) {
    //     for (int j = 0; j < SCREEN_WIDTH; j++) {
    //         state->screen[i][j] = 0;
    //     }
    // }
    memset(state->screen, 0, sizeof(state->screen) );
    state->PC += 2;
}
// 00EE RET - Return from subroutine. Set the PC to the address at the top of the stack and subtract 1 from the SP.
void ret(Chip8State *state)
{
    state->SP -= 1;
    state->PC = state->stack[state->SP];
    state->PC += 2;
}

// 1NNN JMP NNN - Set PC to NNN.
void jmp_nnn(Chip8State *state, uint8_t *code)
{
    uint16_t nnn = ((code[0] & 0xF) << 8) | code[1];
    if (nnn == state->PC) 
    {
        printf("Infinite Loop Detected \n");
        state->pause = 1;
    }
    state->PC = nnn;
}

// 2NNN CALL NNN - 	Call subroutine a NNN. Increment the SP and put the current PC value on the top of the stack. 
// Then set the PC to NNN. Generally there is a limit of 16 successive calls.
void call_nnn(Chip8State *state, uint8_t *code)
{
    state->stack[state->SP] = state->PC;
    state->SP += 1;
    state->PC = ((code[0] & 0xF) << 8) | code[1];

}

// 3XNN	SE VX, NN - Skip the next instruction if register VX is equal to NN.
void se_vx_nn(Chip8State *state, uint8_t *code)
{
    if (state->V[(code[0] & 0xF)] == code[1])
    {
        state->PC += 2;
    }
    state->PC += 2;
}

// 4XNN	SNE VX, NN - Skip the next instruction if register VX is not equal to NN.
void sne_vx_nn(Chip8State *state, uint8_t *code)
{
    if (state->V[(code[0] & 0xF)] != code[1])
    {
        state->PC += 2;
    }
    state->PC += 2;
}

// 5XY0	SE VX VY - Skip the following instruction if the value of register VX is equal to the value of register VY
void se_vx_vy(Chip8State *state, uint8_t *code)
{
    if (state->V[(code[0] & 0xF)] == state->V[(code[1] & 0xF0) >> 4])
    {
        state->PC += 2;
    }
    state->PC += 2;
}

// 6XNN LD VX, NN - Store number NN in register VX.
void ld_vx_nn(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t nn = code[1];
    state->V[x] = nn;
    state->PC += 2;
}

// 7XNN ADD VX, NN - Add the value NN to register VX
void add_vx_nn(Chip8State *state, uint8_t *code)
{
    uint8_t nn = code[1];
    uint8_t x = code[0] & 0xF;
    state->V[x] += nn;
    state->PC += 2;
}

// 8XY0	LD VX VY - Store the value of register VY in register VX
void ld_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    state->V[x] = state->V[y];
    state->PC += 2;
}

// 8XY1	Set VX to VX OR VY
void or_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    state->V[x] |= state->V[y];
    state->PC += 2;
}

// 8XY2	Set VX to VX AND VY
void and_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    state->V[x] &= state->V[y];
    state->PC += 2;
}

// 8XY3	Set VX to VX XOR VY
void xor_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    state->V[x] ^= state->V[y];
    state->PC += 2;
}

// 8XY4	Add the value of register VY to register VX
// Set VF to 01 if a carry occurs
// Set VF to 00 if a carry does not occur
void add_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    if (state->V[x] + state->V[y] > OVERFLOW)
    {
        state->V[0xF] = 1;
    }
    else 
    {
        state->V[0xF] = 0;
    }
    state->V[x] += state->V[y];

    state->PC += 2;
}

// 8XY5	Subtract the value of register VY from register VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void sub_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    if (state->V[x] < state->V[y])
    {
        state->V[0xF] = 1;
    }
    else 
    {
        state->V[0xF] = 0;
    }
    state->V[x] -= state->V[y];

    state->PC += 2;
}

// 8XY6	Store the value of register VY shifted right one bit in register VX¹
// Set register VF to the least significant bit prior to the shift
// VY is unchanged
void shr_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;

    state->V[0xF] = state->V[y] & 0x1;
    state->V[x] = state->V[y] >> 1;

    state->PC += 2;
}

// 8XY7	Set register VX to the value of VY minus VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void subn_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    if (state->V[x] > state->V[y])
    {
        state->V[0xF] = 1;
    }
    else 
    {
        state->V[0xF] = 0;
    }
    state->V[x] = state->V[y] - state->V[x];

    state->PC += 2;
}

// 8XYE	Store the value of register VY shifted left one bit in register VX¹
// Set register VF to the most significant bit prior to the shift
// VY is unchanged
void shl_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;

    state->V[0xF] = state->V[y] >> 7;
    state->V[x] = state->V[y] << 1;

    state->PC += 2;
}

// 9XY0	SNE VX VY - Skip the following instruction if the value of register VX is not equal to the value of register VY
void sne_vx_vy(Chip8State *state, uint8_t *code)
{
    if (state->V[(code[0] & 0xF)] != state->V[(code[1] & 0xF0) >> 4])
    {
        state->PC += 2;
    }
    state->PC += 2;
}

// ANNN LD I, NNN - Set I equal to NNN.
void ld_i_nnn(Chip8State *state, uint8_t *code)
{
    uint16_t nnn = (((code[0] & 0xF) << 8) | code[1]);
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
    int y = state->V[(code[1] & 0xF0) >> 4];
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
            if ((sprite & (SPRITE_BIT >> j)) != 0) // check if current bit in sprite set to 1, sprites are 8 bits long ******** if yes draw
            {
                int coord = ((x + j) % SCREEN_WIDTH) + ((y + i) % SCREEN_HEIGHT) * SCREEN_WIDTH;
                //printf("draw %d %d \n",x+j,y+i);
                if (state->screen[coord] == 0xFFFFFF) // check VF collision flag
                {
                    state->V[0xF] = 1;
                    state->screen[coord] = 0;
                }
                else 
                {
                    state->screen[coord] = 0xFFFFFF; 
                }
                
            }
        }
    }
    printf("Completed draw command \n");
    state->drawflag = 1;
    state->PC += 2;
}

// FX1E	ADD I VX - Add the value stored in register VX to register I
void add_i_vx(Chip8State *state, uint8_t *code)
{
    uint8_t value = state->V[code[0] & 0xF];
    state->I += value;
    state->PC += 2;
}

// FX33	BCD VX - Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
// most significant decimal digit is then stored in at the address found in I, the next in I + 1, and the least significant digit in I + 2
void bcd_vx(Chip8State *state, uint8_t *code)
{
    uint8_t value = state->V[code[0] & 0xF];

    state->memory[state->I] = value / 100;
    state->memory[state->I+1] = (value / 10) % 10;
    state->memory[state->I+2] = (value % 100) % 10;
    state->PC += 2;
}

// FX55	LD I VX - Store the values of registers V0 to VX inclusive in memory starting at address I
// I is set to I + X + 1 after operation²
void ld_i_vx(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;

    for (int i = 0; i <= x; i++)
    {
        state->memory[state->I+i] = state->V[i];
    }
    state->I += x + 1;
    state->PC += 2;   
}

// FX65	LD VX I - Copy values from memory location I through I + X into registers V0 through VX. I does not change.
void ld_vx_i(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    for (int i = 0; i <= x; i++)
    {
        state->V[i] = state->memory[state->I+i];
    }
    state->PC += 2; 
}