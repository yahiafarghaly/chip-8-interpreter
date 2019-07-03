#ifndef _CHIP_8_H__
#define _CHIP_8_H__

#include <iostream>
#include "chip_8_opcode_functions.hpp"

class Chip_8
{

private:

// Memory Map:
// +---------------+= 0xFFF (4095) End of Chip-8 RAM
// |               |
// |               |
// |               |
// |               |
// |               |
// | 0x200 to 0xFFF|
// |     Chip-8    |
// | Program / Data|
// |     Space     |
// |               |
// |               |
// |               |
// +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
// |               |
// |               |
// |               |
// +---------------+= 0x200 (512) Start of most Chip-8 programs
// | 0x000 to 0x1FF|
// | Reserved for  |
// |  interpreter  |
// +---------------+= 0x000 (0) Start of Chip-8 RAM

    unsigned char   Memory[4*1024];      // 4 Kbytes of memory size.
    unsigned short  Stack[16];           // Stack of 16 nested levels.
    unsigned char   GFX[32][64];          // a 64x32-pixel monochrome display used by Chip 8
    unsigned char   key_status[16];      // The computers which originally used the Chip-8 Language had a 16-key hexadecimal keypad.
    unsigned char   V[16];               // V general purpose registers.(V0 --> VF,VF is used as a flag for some Instructions).
    unsigned short  I;                   // I Register.
    unsigned short  SP;                  // Stack Pointer (Can be 8 bit width).
    unsigned short  PC;                  // Program Counter.

    unsigned short  opcode;              // Current fetched opcode.
    unsigned char   delay_timer;
    unsigned char   sound_timer; 

    void PC_increment(size_t count = 2){
        this->PC += count;
    }

    void decode_and_execute_opcode(const unsigned short& opcode);

    // Friends opcode operations.
    friend void opcode_0_fn_sets(Chip_8 &);
    friend void opcode_1_fn_sets(Chip_8 &);
    friend void opcode_2_fn_sets(Chip_8 &);
    friend void opcode_3_fn_sets(Chip_8 &);
    friend void opcode_4_fn_sets(Chip_8 &);
    friend void opcode_5_fn_sets(Chip_8 &);
    friend void opcode_6_fn_sets(Chip_8 &);
    friend void opcode_7_fn_sets(Chip_8 &);
    friend void opcode_8_fn_sets(Chip_8 &);
    friend void opcode_9_fn_sets(Chip_8 &);
    friend void opcode_A_fn_sets(Chip_8 &);
    friend void opcode_B_fn_sets(Chip_8 &);
    friend void opcode_C_fn_sets(Chip_8 &);
    friend void opcode_D_fn_sets(Chip_8 &);
    friend void opcode_E_fn_sets(Chip_8 &);
    friend void opcode_F_fn_sets(Chip_8 &);

public:
    Chip_8();
    ~Chip_8();

    bool load_application(const char* file_full_path);
    void reset();
    void emulateCycle();
    void updateDisplay();
    void clearDisplay();
};


#endif /* _CHIP_8_H__ */
