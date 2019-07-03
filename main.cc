#include <iostream>
#include "Chip_8.hpp"

int main()
{
    std::cout << "Welcome to Chip 8 Interpreter. " << std::endl;
    
    Chip_8 mychip8;
    mychip8.emulateCycle();
    
    return 0;
}