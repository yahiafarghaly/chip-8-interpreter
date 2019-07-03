
#include <iostream>
#include <exception>
#include "chip_8_opcode_functions.hpp"


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

void opcode_8_fn_sets(Chip_8 & chip8){}

void opcode_9_fn_sets(Chip_8 & chip8){}

void opcode_A_fn_sets(Chip_8 & chip8){}

void opcode_B_fn_sets(Chip_8 & chip8){}

void opcode_C_fn_sets(Chip_8 & chip8){}

void opcode_D_fn_sets(Chip_8 & chip8){}

void opcode_E_fn_sets(Chip_8 & chip8){}

void opcode_F_fn_sets(Chip_8 & chip8){}