#ifndef Instructions_H
#define Instructions_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "chip8.h"

// remaps keys from keyboard to keypad value 0-15
int remap(SDL_KeyCode key);

// instructions Each CHIP-8 instruction is exactly 2 bytes long so PC will progess by 2 every instruction unless skipped
// 00E0	Clear the screen CLS
void cls(Chip8State *state);
// 00EE	Return from a subroutine
void ret(Chip8State *state);
// 1NNN	Jump to address NNN
void jmp_nnn(Chip8State *state, uint8_t *code);
// 2NNN	Execute subroutine starting at address NNN
void call_nnn(Chip8State *state, uint8_t *code);
// 3XNN	Skip the following instruction if the value of register VX equals NN
void se_vx_nn(Chip8State *state, uint8_t *code);
// 4XNN	Skip the following instruction if the value of register VX is not equal to NN
void sne_vx_nn(Chip8State *state, uint8_t *code);
// 5XY0	Skip the following instruction if the value of register VX is equal to the value of register VY
void se_vx_vy(Chip8State *state, uint8_t *code);
// 6XNN	Store number NN in register VX
void ld_vx_nn(Chip8State *state, uint8_t *code);
// 7XNN	Add the value NN to register VX
void add_vx_nn(Chip8State *state, uint8_t *code);
// 8XY0	Store the value of register VY in register VX
void ld_vx_vy(Chip8State *state, uint8_t *code);
// 8XY1	Set VX to VX OR VY
void or_vx_vy(Chip8State *state, uint8_t *code);
// 8XY2	Set VX to VX AND VY
void and_vx_vy(Chip8State *state, uint8_t *code);
// 8XY3	Set VX to VX XOR VY
void xor_vx_vy(Chip8State *state, uint8_t *code);
// 8XY4	Add the value of register VY to register VX
// Set VF to 01 if a carry occurs
// Set VF to 00 if a carry does not occur
void add_vx_vy(Chip8State *state, uint8_t *code);
// 8XY5	Subtract the value of register VY from register VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void sub_vx_vy(Chip8State *state, uint8_t *code);
// 8XY6	Store the value of register VY shifted right one bit in register VX¹
// Set register VF to the least significant bit prior to the shift
// VY is unchanged
void shr_vx_vy(Chip8State *state, uint8_t *code);
// 8XY7	Set register VX to the value of VY minus VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void subn_vx_vy(Chip8State *state, uint8_t *code);
// 8XYE	Store the value of register VY shifted left one bit in register VX¹
// Set register VF to the most significant bit prior to the shift
// VY is unchanged
void shl_vx_vy(Chip8State *state, uint8_t *code);
// 9XY0	Skip the following instruction if the value of register VX is not equal to the value of register VY
void sne_vx_vy(Chip8State *state, uint8_t *code);
// ANNN	Store memory address NNN in register I
void ld_i_nnn(Chip8State *state, uint8_t *code);
// BNNN	Jump to address NNN + V0
void jmp_nnn_v0(Chip8State *state, uint8_t *code);
// CXNN	Set VX to a random number with a mask of NN
void rnd_vx_nn(Chip8State *state, uint8_t *code);
// DXYN	Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
// Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
void drw_vx_vy_n(Chip8State *state, uint8_t *code);
// EX9E	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
void skp_vx(Chip8State *state, uint8_t *code);
// EXA1	Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
void sknp_vx(Chip8State *state, uint8_t *code);
// FX07	Store the current value of the delay timer in register VX
void ld_dt_vx(Chip8State *state, uint8_t *code);
// FX0A	Wait for a keypress and store the result in register VX
void ld_vx_key(Chip8State *state, uint8_t *code);
// FX15	Set the delay timer to the value of register VX
void ld_vx_dt(Chip8State *state, uint8_t *code);
// FX18	Set the sound timer to the value of register VX
// FX1E	Add the value stored in register VX to register I
void add_i_vx(Chip8State *state, uint8_t *code);
// FX29	Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
void ld_i_font(Chip8State *state, uint8_t *code);
// FX33	Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
void bcd_vx(Chip8State *state, uint8_t *code);
// FX55	Store the values of registers V0 to VX inclusive in memory starting at address I // I is set to I + X + 1 after operation
void ld_i_vx(Chip8State *state, uint8_t *code);
// FX65	Copy values from memory location I through I + X into registers V0 through VX. I does not change.
void ld_vx_i(Chip8State *state, uint8_t *code);
#endif