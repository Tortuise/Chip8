#include "chip8.h"
#include "instructions.h"

void Disassembler(uint8_t *codebuffer, int pc, FILE *fptr) 
{
    uint8_t *code = &codebuffer[pc];
    uint8_t firstnib = (code[0] >> 4);
    uint8_t lastnib = code[1]>>4;
    
    printf("%04x %02x %02x ", pc, code[0], code[1]);
    if (fptr)
    {
        fprintf(fptr, "%04x %02x %02x \n", pc, code[0], code[1]);
    }
    
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

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

//SDL functions
int sdlInit()
{
	//Initialization flag
	int success = 1;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chip8",
     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10, 0);
    if (!window){
        fprintf(stderr, "Could not create %s: %s\n", "window", SDL_GetError());
        return 0;
    }
     uint8_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    if (!renderer){
        fprintf(stderr, "Could not create %s: %s\n", "renderer", SDL_GetError());
        return 0;
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!texture){
        fprintf(stderr, "Could not create %s: %s\n", "texture", SDL_GetError());
        return 0;
    }
    return success;
}

void renderScreen(Chip8State *state)
{
    FILE *file = fopen("draw.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    // log render
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            fprintf(file, "%6x ", state->screen[index]); 
        }
        fprintf(file, "\n");
    }

    fclose(file);

    SDL_UpdateTexture(texture, NULL, state->screen, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clearSDL() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

static void opCode0(Chip8State *state, uint8_t *code)
{
    switch (code[1])
    {
        case 0xE0: cls(state); break;
        case 0xEE: break;
        default: printf("Not implemented 0"); state->pause = 1; break;

    }
}

static void opCode1(Chip8State *state, uint8_t *code)
{
    jmp_nnn(state, code);
}

static void opCode6(Chip8State *state, uint8_t *code)
{
    ld_vx_nn(state, code);
}

static void opCodeA(Chip8State *state, uint8_t *code)
{
    ld_i_nnn(state, code);
}

static void opCodeD(Chip8State *state, uint8_t *code)
{
    drw_vx_vy_n(state, code);
}

static void opCode7(Chip8State *state, uint8_t *code)
{
    add_vx_nn(state, code);
}

Chip8State* initiate(void)
{
    Chip8State* state = calloc(sizeof(Chip8State), 1);
    state->memory = calloc(TOTAL_RAM, 1);
    //clear screen
    // for (int i = 0; i < SCREEN_HEIGHT; i++) {
    //     for (int j = 0; j < SCREEN_WIDTH; j++) {
    //         state->screen[i][j] = 0;
    //     }
    // }
    memset(state->screen, 0, sizeof(state->screen));
    state->PC = PC_START;
    state->SP = STACK_START;
    state->drawflag = 0;
    state->pause = 0;
    return state;

}

void EmulateChip8(Chip8State *state) {
    uint8_t *opcode = &state->memory[state->PC];
    uint8_t highnib = (opcode[0]) >> 4; // mask last 4 bits and bit shift right 4 times
    Disassembler(state->memory, state->PC, NULL);
    printf(" Emulating %04x %02x %02x \n", state->PC, opcode[0], opcode[1]);
    switch (highnib)
    {
        case 0x00: opCode0(state, opcode); break;    
        case 0x01: opCode1(state, opcode); break;    
        case 0x02: printf("2 not handled yet"); state->pause = 1; break;    
        case 0x03: printf("3 not handled yet"); state->pause = 1; break;    
        case 0x04: printf("4 not handled yet"); state->pause = 1; break;    
        case 0x05: printf("5 not handled yet"); state->pause = 1; break;    
        case 0x06: opCode6(state, opcode); break;    
        case 0x07: opCode7(state, opcode); break;    
        case 0x08: printf("8 not handled yet"); state->pause = 1; break;    
        case 0x09: printf("9 not handled yet"); state->pause = 1; break;    
        case 0x0a: opCodeA(state, opcode); break;    
        case 0x0b: printf("b not handled yet"); state->pause = 1; break;    
        case 0x0c: printf("c not handled yet"); state->pause = 1; break;    
        case 0x0d: opCodeD(state, opcode); break;    
        case 0x0e: printf("e not handled yet"); state->pause = 1; break;    
        case 0x0f: printf("f not handled yet"); state->pause = 1; break;   
    }
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
    printf("Iniating \n");
    Chip8State* chip8 = initiate();
   
    // load rom into memory
    printf("Loading to ROM \n");
    for(int i = 0; i < fsize; ++i) 
    {
        chip8->memory[i + PC_START] = buffer[i + PC_START];
    }
    printf("Checking Debug Mode \n");
    int debugMode = 0;
    if (argc > 2 && strcmp(argv[2], "debug") == 0) 
    {
        debugMode = 1;
        printf("DEBUG MODE ON \n");
    }
    printf("Iniating SDL\n");
    sdlInit();
    printf("Starting Execution Cycles \n");
    int cycles = 0;
    while ( chip8->pause == 0 )
    {
        if (debugMode == 1)
        {
            char input;
            printf("Debug mode enabled. Press Enter to execute each cycle (q + Enter to quit).\n");
            while ((input = getchar()) != 'q')
            {
                if (input == '\n') 
                {
                    EmulateChip8(chip8);
                    printf("Cycle executed. Press Enter to execute the next cycle (q + Enter to quit).\n");
                }
            }
        }
        else 
        {
            printf("emulateing \n");
            EmulateChip8(chip8); // 1 cycle
        }
        
        if (chip8->drawflag)
        {
            renderScreen(chip8);
            chip8->drawflag = 0;
        }

        printf("end cycle %d \n", cycles);
        
        if (((strcmp(argv[1],"1-chip8-logo.ch8") == 0) & (cycles == 39))
        | ((strcmp(argv[1],"2-ibm-logo.ch8") == 0) & (cycles == 20))) 
        {
            printf("end test rom \n");
            // pause indef
            
            printf("q + Enter to quit.\n");
            char input = getchar();
            if (input == 'q')
            {
                chip8->pause = 1;
            }
        }
        cycles += 1;
    }
    clearSDL();
    return 0;
}