
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
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
        k = CHIP_8_KEY_NOT_PRESSED;
    // Clear V Registers. 
    for(auto reg : V)
        reg = 0;
    // Load hex font sprites in the reserved memory area of Interpreter.
    for(size_t i = 0; i < 5*16; i++)
        Memory[i] = chip8_hex_sprites[i];

    // Reset Timers.
    DT = 0;
    ST = 0;

    // Clear & update Display.
    clearDisplay();
    updateDisplay();

    // Seed thr random generator with time.
    srand(time(NULL));
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
    for(size_t r = 0; r < CHIP_8_DISPLAY_HEIGHT; r++)
    {    
        for(size_t c = 0; c < CHIP_8_DISPLAY_WIDTH; c++)
            if(GFX[r][c] == CHIP_8_PIXEL_ON)
                std::cout << '*';
            else
                std::cout << ' ';
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Chip_8::clearDisplay()
{
    for(size_t r = 0; r < CHIP_8_DISPLAY_HEIGHT; r++)
        for(size_t c = 0; c < CHIP_8_DISPLAY_WIDTH; c++)
            GFX[r][c] = CHIP_8_PIXEL_OFF;
}

/*!
 * According to the opcode `Dxyn` , The function does these jobs.
 * The pixel value is XORed onto the existing screen.
 * Handles the pixel wrapping around the display.
 * Set V[F] = 1 if collision is detected (A pixel is erased).
 * */
void Chip_8::drawPixel( const unsigned char& x,
                        const unsigned char& y,
                        const unsigned char& pixelValue)
{
        if(GFX[x % CHIP_8_DISPLAY_HEIGHT][y % CHIP_8_DISPLAY_WIDTH] == CHIP_8_PIXEL_ON
        && pixelValue > CHIP_8_PIXEL_OFF)
            this->V[0xF] = 0x01;
        GFX[x % CHIP_8_DISPLAY_HEIGHT][y % CHIP_8_DISPLAY_WIDTH] ^= 
                                                    (pixelValue > CHIP_8_PIXEL_OFF ? CHIP_8_PIXEL_ON : CHIP_8_PIXEL_OFF);
}