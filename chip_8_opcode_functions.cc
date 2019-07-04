
#include <iostream>
#include "chip_8_opcode_functions.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time *

/* All The opcodes execution is performed according
 * to the chip-8 standarizaton mentioned here in this link
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0
 */

const opcode_function_executer opcode_table [] = 
{
    opcode_0_fn_sets,
    opcode_1_fn_sets,
    opcode_2_fn_sets,
    opcode_3_fn_sets,
    opcode_4_fn_sets,
    opcode_5_fn_sets,
    opcode_6_fn_sets,
    opcode_7_fn_sets,
    opcode_8_fn_sets,
    opcode_9_fn_sets,
    opcode_A_fn_sets,
    opcode_B_fn_sets,
    opcode_C_fn_sets,
    opcode_D_fn_sets,
    opcode_E_fn_sets,
    opcode_F_fn_sets
};


//0x0nnn - 0x00E0 - 0x00EE
void opcode_0_fn_sets(Chip_8 & chip8){

    switch(chip8.opcode & 0x0fff)
    {   
        case 0x00E0:    // Clear The display
                chip8.clearDisplay();
                chip8.updateDisplay();
                chip8.PC_increment(); // By default it's 2 bytes As Chip 8 does.
        break;

        case 0x00EE:    // Return from a subroutine.
            chip8.SP -= 1;
            chip8.PC = chip8.Stack[chip8.SP];
            chip8.PC_increment();
        break;

        default:        // 0nnn => SYS addr (Jump to a machine code routine at nnn.) 
            chip8.PC = chip8.opcode & 0x0fff;
        break;
    }
}

// 0x1nnn - JP addr
void opcode_1_fn_sets(Chip_8 & chip8){
        chip8.PC = chip8.opcode & 0x0fff;
}

// 0x2nnn - CALL addr
void opcode_2_fn_sets(Chip_8 & chip8){
     chip8.Stack[chip8.SP] = chip8.PC;
     chip8.SP += 1;
     chip8.PC = chip8.opcode & 0x0fff;
}

// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk.
void opcode_3_fn_sets(Chip_8 & chip8){

    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto byte = (chip8.opcode & 0x00ff);

    if(chip8.V[x] == byte)
        chip8.PC_increment(4);
    else
        chip8.PC_increment(2);
}

// 4xkk - SNE Vx, byte
// Skip next instruction if Vx != kk.
void opcode_4_fn_sets(Chip_8 & chip8){
    
    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto byte = (chip8.opcode & 0x00ff);

    if(chip8.V[x] != byte)
        chip8.PC_increment(4);
    else
        chip8.PC_increment(2);
}

// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy.
void opcode_5_fn_sets(Chip_8 & chip8){
    
    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto y = (chip8.opcode & 0x00f0) >> 4;

    if(chip8.V[x] == chip8.V[y])
        chip8.PC_increment(4);
    else
        chip8.PC_increment(2);
}

// 6xkk - LD Vx, byte
// Set Vx = kk.
void opcode_6_fn_sets(Chip_8 & chip8){
    chip8.V[(chip8.opcode & 0x0f00) >> 8] = (chip8.opcode & 0x00ff);
    chip8.PC_increment();
}

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
void opcode_7_fn_sets(Chip_8 & chip8){
    chip8.V[(chip8.opcode & 0x0f00) >> 8] += (chip8.opcode & 0x00ff);
    chip8.PC_increment();
}

// 8xy0 - 8xy1 - 8xy2 - 8xy3 - 8xy4 - 8xy5 - 8xy6 - 8xy7 - 8xyE
void opcode_8_fn_sets(Chip_8 & chip8){
    auto code_select = chip8.opcode & 0x000f;
    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto y = (chip8.opcode & 0x00f0) >> 4;

    switch(code_select)
    {
        // Set Vx = Vy
        case 0x00:
            chip8.V[x] = chip8.V[y];
            chip8.PC_increment();
        break;
        
        // Set Vx = Vx | Vy
        case 0x01:
            chip8.V[x] |= chip8.V[y];
            chip8.PC_increment();  
        break;
        
        // Set Vx = Vx & Vy
        case 0x02:
            chip8.V[x] &= chip8.V[y];
            chip8.PC_increment();
        break;
        
        // Set Vx = Vx xor Vy
        case 0x03:
            chip8.V[x] ^= chip8.V[y];
            chip8.PC_increment();        
        break;

        // Set Vx = Vx + Vy , VF = carry
        case 0x04:
        {   // Create an enclosing scope so not violating the switch case rule
            // while creating variables.
            unsigned short result = (unsigned short)chip8.V[x] + (unsigned short)chip8.V[y];
            chip8.V[0xF] = (result > 0xFF) ? 0x01 : 0x00;
            chip8.V[x] = result & 0x000F;
            chip8.PC_increment();            
        }
        break;

        // Set Vx = Vx - Vy, set VF = NOT borrow.
        case 0x05:
            chip8.V[0xF] = (chip8.V[x] > chip8.V[y]) ? 0x01 : 0x00;
            chip8.V[x] = chip8.V[x] - chip8.V[y];
            chip8.PC_increment();
        break;

        // Set Vx = Vx SHR(Shift Right by one bit) 1
        case 0x06:
            chip8.V[0xF] = (chip8.V[x] & 0x01);
            chip8.V[x] >>= 0x01;
            chip8.PC_increment();
        break;

        // Set Vx = Vy - Vx, set VF = NOT borrow
        case 0x07:
            chip8.V[0xF] = (chip8.V[y] > chip8.V[x]) ? 0x01 : 0x00;
            chip8.V[x] = chip8.V[y] - chip8.V[x];
            chip8.PC_increment();        
        break;

        // Set Vx = Vx SHL(Shift Left) 1.
        case 0x0E:
            chip8.V[0xF] = chip8.V[x] >> 0x07;
            chip8.V[x] <<= 0x01;
            chip8.PC_increment();        
        break;

        default:
            printf("Unknown opcode [0x8000]: 0x%X\n", chip8.opcode);
            exit(-1);
        break;
    }
}

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
void opcode_9_fn_sets(Chip_8 & chip8){
    auto code_select = chip8.opcode & 0x000f;
    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto y = (chip8.opcode & 0x00f0) >> 4;
    switch(code_select)
    {
        case 0x0000:
            if(chip8.V[x] != chip8.V[y])
                chip8.PC_increment(4);
            else
                chip8.PC_increment(2);
        break;
        default:
            printf("Unknown opcode [0x9000]: 0x%X\n", chip8.opcode);
            exit(-1);
        break;
    }
}

// Annn, Set I = nnn.
void opcode_A_fn_sets(Chip_8 & chip8){
    chip8.I = chip8.opcode & 0x0FFF;
    chip8.PC_increment();
}

// Bnnn, Jump to location nnn + V0.
void opcode_B_fn_sets(Chip_8 & chip8){
    chip8.PC = (chip8.opcode & 0x0FFF) + chip8.V[0];
}

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
void opcode_C_fn_sets(Chip_8 & chip8){
    auto x = (chip8.opcode & 0x0F00) >> 8;
    auto max_byte = (chip8.opcode) & 0x00FF;
    chip8.V[x] = (rand() % 0xFF) & max_byte;
    chip8.PC_increment();
}

// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
void opcode_D_fn_sets(Chip_8 & chip8){
    auto n = chip8.opcode & 0x000F;
    auto x = (chip8.opcode & 0x0f00) >> 8;
    auto y = (chip8.opcode & 0x00f0) >> 4;
    unsigned char row_sprite = 0;
    chip8.V[0xF] = 0;
    
    for(size_t y_offest = 0; y_offest < n; y_offest++)
    {
        row_sprite = chip8.Memory[chip8.I + y_offest];
        for(size_t x_offest = 0; x_offest < 8; x_offest++)
        {
            chip8.drawPixel(x + x_offest,
                            y + y_offest,
                            row_sprite & (0x80 >> x_offest) );
        }
    }
    chip8.updateDisplay();
    chip8.PC_increment();
}

void opcode_E_fn_sets(Chip_8 & chip8){}

void opcode_F_fn_sets(Chip_8 & chip8){}