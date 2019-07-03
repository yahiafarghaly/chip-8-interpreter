
#include "Chip_8.hpp"
#include "chip8_sprites.hpp"


void Chip_8::reset()
{
    this->PC = 0x200;
    this->I = 0;
    this->opcode = 0;
    this->SP = 0;

    // Clear Memory.
    for(auto byte : Memory)
        byte = 0;
    // Clear Stack.
    for(auto i : Stack)
        i = 0;
    // Clear Keys Status.
    for(auto k : key_status)
        k = 0;
    // Clear V Registers. 
    for(auto reg : V)
        reg = 0;
    // Load hex font sprites in the reserved memory area of Interpreter.
    for(size_t i = 0; i < 5*16; i++)
        Memory[i] = chip8_hex_sprites[i];

    // Reset Timers.
    delay_timer = 0;
    sound_timer = 0;

    // Clear & update Display.
    clearDisplay();
    updateDisplay();
}

Chip_8::Chip_8()
{
    this->reset();
}

Chip_8::~Chip_8()
{
}


void Chip_8::emulateCycle()
{
    // Fetch opcode.
    this->opcode = Memory[PC] << 8 | Memory[PC + 1]; // opcode is 2 bytes on chip 8.
    decode_and_execute_opcode(opcode);
}


void Chip_8::decode_and_execute_opcode(const unsigned short& opcode)
{
    opcode_table[(opcode & 0xf000) >> 0x0C](*this);
}

void Chip_8::updateDisplay()
{
    for(size_t r = 0; r < 32; r++)
    {    
        for(size_t c = 0; c < 64; c++)
            std::cout << GFX[r][c];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Chip_8::clearDisplay()
{
    for(size_t r = 0; r < 32; r++)
        for(size_t c = 0; c < 64; c++)
            GFX[r][c] = 0;
}