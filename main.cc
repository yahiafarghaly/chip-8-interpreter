#include <iostream>
#include "Chip_8.hpp"

int main(int argc, char **argv)
{
    if(argc < 2)
	{
		printf("Usage: ./chip8-app chip8-application\n\n");
		return -1;
	}
    
    Chip_8 mychip8;
    if(!mychip8.load_application(argv[1]))
    {
        printf("Failed to Load ` %s ` as a chip 8 application\n",argv[1]);
        return -2;
    }

    printf("Welcome to Chip 8 Interpreter. \n");
    printf("%s is loaded successfully\n",argv[1]);

    while(true)
    {
        mychip8.emulateCycle();
        mychip8.updateDisplay();
       // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    
    return 0;
}