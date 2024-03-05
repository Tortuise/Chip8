#include "chip8.h"

#include "instructions.h"



void Disassembler(uint8_t *codebuffer, int pc, FILE *fptr) 
{
    uint8_t *code = &codebuffer[pc];
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);
    fprintf(fptr, "%04x %02x %02x \n", pc, code[0], code[1]);
    switch(firstnib)
    {
        case 0x0: 
            switch (code[1])
            {
                case 0xe0: printf("%10s", "CLS"); break;
                case 0xee: printf("%10s", "RET"); break;
                default: printf("Not Implemented 0"); break;
            }
            break;
        case 0x1: printf("%10s $%01x%02x", "JMP NNN", code[0]&0xf, code[1]); break;
        case 0x2: printf("%10s $%01x%02x", "CALL NNN", code[0]&0xf, code[1]); break;
        case 0x3: printf("%10s $%01x%02x", "SE VX, NN", code[0]&0xf, code[1]); break;
        case 0x4: printf("%10s $%01x%02x", "SNE VX, NN", code[0]&0xf, code[1]); break;
        case 0x5: printf("%10s $%01x%02x", "SE VX, VY", code[0]&0xf, code[1]); break; // 5XY0
        case 0x6: printf("%10s $%01x%02x", "LD VX, NN", code[0]&0xf, code[1]); break; //6XNN Store number NN in register VX
        case 0x7: printf("%10s $%01x%02x", "ADD VX, NN", code[0]&0xf, code[1]); break; // 7XNN Add value NN to registe VX
        case 0x8: 
            uint8_t lastnib = code[1]>>4;
            switch (lastnib) // last nibble of instruction
            {
                case 0: printf("%10s $%01x%02x", "LD VX VY", code[0]&0xf, code[1]>>4); break;
                case 1: printf("%10s $%01x%02x", "OR VX, VY", code[0]&0xf, code[1]>>4); break;
                case 2: printf("%10s $%01x%02x", "AND VX, VY", code[0]&0xf, code[1]>>4); break;
                case 3: printf("%10s $%01x%02x", "XOR VX, VY", code[0]&0xf, code[1]>>4); break;
                case 4: printf("%10s $%01x%02x", "ADD VX, VY", code[0]&0xf, code[1]>>4); break;
                case 5: printf("%10s $%01x%02x", "SUB VX, VY", code[0]&0xf, code[1]>>4); break;
                case 6: printf("%10s $%01x%02x", "SHR VX, VY", code[0]&0xf, code[1]>>4); break;
                case 7: printf("%10s $%01x%02x", "SUBN VX, VY", code[0]&0xf, code[1]>>4); break;
                case 0xe: printf("%10s $%01x%02x", "SHL VX, VY", code[0]&0xf, code[1]>>4); break;
                default: printf("Not Implemented 8"); break;
            }
            break;
        case 0x9: printf("%10s $%01x%02x", "SNE VX, VY", code[0]&0xf, code[1]); break; // 9XY0
        case 0xa: printf("%10s $%01x%02x", "LD I, NNN", code[0]&0xf, code[1]); break;
        case 0xb: printf("%10s $%01x%02x", "JMP V0, NNN", code[0]&0xf, code[1]); break;
        case 0xc: printf("%10s $%01x%02x", "RND VX, NN", code[0]&0xf, code[1]); break; // CXNN
        case 0xd: printf("%10s $%01x%02x", "DRW VX, VY, N", code[0]&0xf, code[1]); break; // DXYN
        case 0xe: 
            switch (code[1])
            {
                case 0x9e: printf("%10s $%01x%02x", "SKP VX", code[0]&0xf, code[1]); break; 
                case 0xa1: printf("%10s $%01x%02x", "SKNP VX", code[0]&0xf, code[1]); break; 
            }
            break;
        case 0xf: 
            switch(code[1])
            {
                case 0x07: printf("%10s $%01x%02x", "LD VX, DT", code[0]&0xf, code[1]); break;
                case 0x0a: printf("%10s $%01x%02x", "LD VX, KEY", code[0]&0xf, code[1]); break; 
                case 0x15: printf("%10s $%01x%02x", "LD DT, VX", code[0]&0xf, code[1]); break; 
                case 0x18: printf("%10s $%01x%02x", "LD ST, VX", code[0]&0xf, code[1]); break; 
                case 0x1e: printf("%10s $%01x%02x", "ADD I, VX", code[0]&0xf, code[1]); break; 
                case 0x29: printf("%10s $%01x%02x", "LD I, FONT(VX)", code[0]&0xf, code[1]); break; 
                case 0x33: printf("%10s $%01x%02x", "BCD VX", code[0]&0xf, code[1]); break; 
                case 0x55: printf("%10s $%01x%02x", "LD [I], VX", code[0]&0xf, code[1]); break; 
                case 0x65: printf("%10s $%01x%02x", "LD VX, [I]", code[0]&0xf, code[1]); break; 
                default: printf("Not implemented F"); break;
            }
            break;

    }
}

static void opCode0(Chip8State *state, uint8_t *code){
    switch (code[1])
    {
        case 0xE0: cls(state); break;
        case 0xEE: break;

    }
}

static void opCode1(Chip8State *state, uint8_t *code){
    jmp_nnn(state, code);
}

static void opCode6(Chip8State *state, uint8_t *code){
    ld_vx_nn(state, code);
}

static void opCodeA(Chip8State *state, uint8_t *code){
    ld_i_nnn(state, code);
}

static void opCodeD(Chip8State *state, uint8_t *code){
    drw_vx_vy_n(state, code);
}


int main (int argc, char *argv[])
{
    char *filename = argv[1];
    FILE *f = fopen(filename, "rb");
    if (f==NULL)
    {
        printf("ERROR: File %s does not exist\n",filename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buffer = malloc(fsize + PC_START);
    fread(buffer + PC_START, fsize, 1, f);
    fclose(f);
    int pc = PC_START;

    FILE *fptr;
    fptr = fopen("filename.txt", "w");
    while (pc < (fsize + PC_START))
    {
        Disassembler(buffer, pc, fptr);
        pc += 2;
        printf("\n");
    }
    fclose(fptr);

    // Chip8State* chip8 = initiate();
    // while ( chip8->PC < (fsize + PC_START))
    // {
    //     EmulateChip8(chip8); // 1 cycle
    //     if (chip8->drawflag)
    //     {
    //         printf("drawing graphics \n");
    //         chip8->drawflag = 0;
    //     }
    // }

    return 0;
}

Chip8State* initiate(void)
{
    Chip8State* state = calloc(sizeof(Chip8State), 1);
    state->memory = calloc(TOTAL_RAM, 1);
    //clear screen
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            state->screen[i][j] = 0;
        }
    }
    state->PC = PC_START;
    state->SP = STACK_START;
    state->drawflag = 0;
    state->pause = 0;
    return state;

}

void EmulateChip8(Chip8State *state) {
    uint8_t *opcode = &state->memory[state->PC];
    int firstnib = (*opcode & 0xf0) >> 4; // mask last 4 bits and bit shift right 4 times
    printf("Emulating ");
    printf("%04x %02x %02x ", state->PC, opcode[0], opcode[1]);
    switch (firstnib)
    {
        case 0x0: opCode0(state, opcode); break;    
        case 0x1: opCode1(state, opcode); break;    
        case 0x2: printf("2 not handled yet"); break;    
        case 0x3: printf("3 not handled yet"); break;    
        case 0x4: printf("4 not handled yet"); break;    
        case 0x5: printf("5 not handled yet"); break;    
        case 0x6: opCode6(state, opcode); break;    
        case 0x7: printf("7 not handled yet"); break;    
        case 0x8: printf("8 not handled yet"); break;    
        case 0x9: printf("9 not handled yet"); break;    
        case 0xa: opCodeA(state, opcode); break;    
        case 0xb: printf("b not handled yet"); break;    
        case 0xc: printf("c not handled yet"); break;    
        case 0xd: opCodeD(state, opcode); break;    
        case 0xe: printf("e not handled yet"); break;    
        case 0xf: printf("f not handled yet"); break;   
    }
}