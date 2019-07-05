
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <iterator>
#include <vector>
#include "Chip_8.hpp"
#include "chip8_sprites.hpp"


void Chip_8::reset()
{
    this->PC = 0x200;
    this->I = 0;
    this->opcode = 0;
    this->SP = 0;

    // Clear Memory.
    for(size_t i = 0; i < 4*1024; i++)
        Memory[i] = 0;
    // Clear Stack.
    for(size_t i = 0; i < 16; i++)
        Stack[i] = 0;
    // Clear Keys Status.
    for(size_t i = 0; i < 16; i++)
        key_status[i] = CHIP_8_KEY_NOT_PRESSED;
    // Clear V Registers. 
    for(size_t i = 0; i < 16; i++)
        V[i] = 0;
    // Load hex font sprites in the reserved memory area of Interpreter.
    for(size_t i = 0; i < 5*16; i++)
        Memory[i] = chip8_hex_sprites[i];

    // Clear & update Display.
    clearDisplay();
    updateDisplay();

    // Seed thr random generator with time.
    srand(time(NULL));

    // Make sure that these two threads have finished.
    if(Thread_DelayTimer.joinable())
        Thread_DelayTimer.join();
    if(Thread_SoundTimer.joinable())
        Thread_SoundTimer.join();

    // Reset Timers.
    DT = 0;
    ST = 0;
}

Chip_8::Chip_8()
{
    this->reset();
}

Chip_8::~Chip_8()
{
    if(Thread_DelayTimer.joinable())
        Thread_DelayTimer.join();
    if(Thread_SoundTimer.joinable())
        Thread_SoundTimer.join();
}

void Chip_8::StartDelayTimer()
{
    Thread_DelayTimer = std::thread(&Chip_8::DelayTimer,this);
}

void Chip_8::StartSoundTimer()
{
    Thread_SoundTimer = std::thread(&Chip_8::SoundTimer,this);
}

void Chip_8::DelayTimer()
{
    // Decrease The timer value by 1 at rate 60 Hz (1/60 ~ 17 ms)
    while(this->DT > 0)
    {
        this->DT -= 1;
        printf("Timer Value %d\n",this->DT);
        std::this_thread::sleep_for(std::chrono::milliseconds(17));     
    }
}

void Chip_8::SoundTimer()
{
    // Decrease The timer value by 1 at rate 60 Hz (1/60 ~ 17 ms)
    while(this->ST > 0)
    {
        this->ST -= 1;
        printf("BEEP !");
        std::this_thread::sleep_for(std::chrono::milliseconds(17));     
    }
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


bool Chip_8::load_application(const char* file_full_path)
{
    printf("Reseting Chip 8 Interpreter\n");
    this->reset();

    printf("Loading: %s\n",file_full_path);
    
    std::ifstream c8_app(file_full_path, std::ios::binary );
    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(c8_app), {});
    
    if(buffer.empty())
    {
        printf("Cannot allocate buffer for the read data\n");
        return false;
    }

    // Copy buffer into the chip 8 memory.
    if(buffer.size() > 4096-512)
    {
        printf("Application ROM is bigger than chip 8 memory\n");
        printf("Application ROM: %lu\n",buffer.size());
        return false;
    }
    else
    {
        for(int i = 0; i < buffer.size(); i++)
            this->Memory[i+512] = buffer[i];
    }
    
    c8_app.close();
	return true;
}



void Chip_8::pressKey(const unsigned char& keyIdx)
{
    this->key_status[keyIdx] = CHIP_8_KEY_PRESSED;
}
void Chip_8::releaseKey(const unsigned char& keyIdx)
{
    this->key_status[keyIdx] = CHIP_8_KEY_NOT_PRESSED;
}

void Chip_8::printKeypadStatus()
{
    printf("Keypad Status: \n");
    printf("-- -- -- --\n");
    printf("%d %d %d %d\n",
        key_status[1],key_status[2],key_status[3],key_status[0x0C]);
    printf("-- -- -- --\n");
    
    printf("%d %d %d %d\n",
    key_status[4],key_status[5],key_status[6],key_status[0x0D]);
    printf("-- -- -- --\n");

        printf("%d %d %d %d\n",
    key_status[7],key_status[8],key_status[9],key_status[0x0E]);
    printf("-- -- -- --\n");
    
        printf("%d %d %d %d\n",
    key_status[0x0A],key_status[0],key_status[0x0B],key_status[0x0F]);

    printf("-- -- -- --\n");
    printf("\n");
}