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
    uint16_t res = state->V[x] + state->V[y];
    state->V[x] += state->V[y];
    if (res > OVERFLOW)
    {
        state->V[0xF] = 1;
    }
    else 
    {
        state->V[0xF] = 0;
    }

    state->PC += 2;
}

// 8XY5	Subtract the value of register VY from register VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void sub_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    int res = (state->V[x] < state->V[y]);
    state->V[x] -= state->V[y];
    if (res)
    {
        state->V[0xF] = 0;
    }
    else 
    {
        state->V[0xF] = 1;
    }

    state->PC += 2;
}

// 8XY6	Set VX equal to VX bitshifted right 1. 
// VF is set to the least significant bit of VX prior to the shift.
void shr_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    //uint8_t y = (code[1] & 0xF0) >> 4;
    uint8_t lsb = state->V[x] & 0x01;
    state->V[x] = state->V[x] >> 1;
    state->V[0xF] = lsb;
    state->PC += 2;
}

// 8XY7	Set register VX to the value of VY minus VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void subn_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    uint8_t y = (code[1] & 0xF0) >> 4;
    state->V[x] = state->V[y] - state->V[x];
    if (state->V[x] > state->V[y])
    {
        state->V[0xF] = 0;
    }
    else 
    {
        state->V[0xF] = 1;
    }

    state->PC += 2;
}

// 8XYE	Set VX equal to VX bitshifted left 1. 
// VF is set to the most significant bit of VX prior to the shift.
void shl_vx_vy(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    //uint8_t y = (code[1] & 0xF0) >> 4;
    uint8_t msb = state->V[x] >> 7;
    state->V[x] = state->V[x] << 1;
    state->V[0xF] = msb;

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

// remaps keys from keyboard to keypad value 0-15
int remap(SDL_KeyCode key)
{
    switch (key)
    {
        case SDLK_1: return 1; 
        case SDLK_2: return 2;
        case SDLK_3: return 3;
        case SDLK_4: return 12;
        case SDLK_q: return 4;
        case SDLK_w: return 5;
        case SDLK_e: return 6;
        case SDLK_r: return 13;
        case SDLK_a: return 7;
        case SDLK_s: return 8;
        case SDLK_d: return 9;
        case SDLK_f: return 14;
        case SDLK_z: return 10;
        case SDLK_x: return 0;
        case SDLK_c: return 11;
        case SDLK_v: return 15;
        default: return -1;
    }
}

// EX9E	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
void skp_vx(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    if (state->keys[state->V[x]] == 1)
    {
        state->PC += 4;
    }
    else
    {
        state->PC += 2;
    }
}

// EXA1	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
void sknp_vx(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    if (state->keys[state->V[x]] == 0) // if key = 0 returns -1
    {
        state->PC += 4;
    }
    else
    {
        state->PC += 2;
    }
}

// FX07	Store the current value of the delay timer in register VX
void ld_vx_dt(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    state->V[x] = state->delay_timer;
    state->PC += 2;
}

// FX0A	Wait for a keypress and store the result in register VX
void ld_vx_key(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    state->key_pressed = 0;

    for (int i = 0; i < NUM_KEYS; i++)
    {
        if (state->keys[i])
        {
            state->keys[i] = 0;
            state->V[x] = i;
            state->key_pressed = 1;
            break;
        }
    }

    if (state->key_pressed == 0)
    {
        return;
    }
    state->PC += 2;
}

// FX15	Set the delay timer to the value of register VX
void ld_dt_vx(Chip8State *state, uint8_t *code)
{
    uint8_t x = code[0] & 0xF;
    state->delay_timer = state->V[x];
    state->PC += 2;
}

// FX1E	ADD I VX - Add the value stored in register VX to register I
void add_i_vx(Chip8State *state, uint8_t *code)
{
    uint8_t value = state->V[code[0] & 0xF];
    uint16_t res = state->I + value;
    state->I = res;
    if (res > 0x0FFF) // check if res would overflow past addressing range
    {
        state->V[0xf] = 1;
    }
    else
    {
        state->V[0xf] = 0;
    }
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