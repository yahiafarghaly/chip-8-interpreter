#include <iostream>
#include <SFML/Graphics.hpp>
#include "Chip_8.hpp"


void KeyPressedToChip8(const sf::Event& keyEvent, Chip_8& mychip8);
void KeyReleasedToChip8(const sf::Event& keyEvent, Chip_8& mychip8);

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

    sf::RenderWindow window(sf::VideoMode(320, 320), "Chip 8 Interpreter By Yahia Farghaly");
    // Prevent repeated recording of a continous key press.
    window.setKeyRepeatEnabled(false);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                break;

                case sf::Event::KeyPressed:
                    KeyPressedToChip8(event,mychip8);
                break;

                case sf::Event::KeyReleased:
                    KeyReleasedToChip8(event,mychip8);
                break;

                case sf::Event::Resized:
                    std::cout << "new width: " << event.size.width << std::endl;
                    std::cout << "new height: " << event.size.height << std::endl;
                break;

                default:
                break;
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }


    return 0;
}


void KeyPressedToChip8(const sf::Event& keyEvent, Chip_8& mychip8)
{

    /*!   Original Layout   ==>     Todady's Mapped Keyboard Layout
     *      1	2	3	C   ==>     1   2   3   4
     *      4	5	6	D   ==>     Q   W   E   R
     *      7	8	9	E   ==>     A   S   D   F
     *      A	0	B	F   ==>     Z   X   C   V
     */

    switch(keyEvent.key.code)
    {
        case sf::Keyboard::Num1:
            mychip8.pressKey(0x01);
        break;

        case sf::Keyboard::Num2:
            mychip8.pressKey(0x02);
        break;

        case sf::Keyboard::Num3:
            mychip8.pressKey(0x03);
        break;

        case sf::Keyboard::Num4:
            mychip8.pressKey(0x0C);
        break;

        case sf::Keyboard::Q:
            mychip8.pressKey(0x04);
        break;

        case sf::Keyboard::W:
            mychip8.pressKey(0x05);
        break;

        case sf::Keyboard::E:
            mychip8.pressKey(0x06);
        break;

        case sf::Keyboard::R:
            mychip8.pressKey(0x0D);
        break;

        case sf::Keyboard::A:
            mychip8.pressKey(0x07);
        break;

        case sf::Keyboard::S:
            mychip8.pressKey(0x08);
        break;

        case sf::Keyboard::D:
            mychip8.pressKey(0x09);
        break;

        case sf::Keyboard::F:
            mychip8.pressKey(0x0E);
        break;

        case sf::Keyboard::Z:
            mychip8.pressKey(0x0A);
        break;

        case sf::Keyboard::X:
            mychip8.pressKey(0x00);
        break;

        case sf::Keyboard::C:
            mychip8.pressKey(0x0B);
        break;

        case sf::Keyboard::V:
            mychip8.pressKey(0x0F);
        break;

        default:
        break;
    }
}


void KeyReleasedToChip8(const sf::Event& keyEvent, Chip_8& mychip8)
{
    switch(keyEvent.key.code)
    {
        case sf::Keyboard::Num1:
            mychip8.releaseKey(0x01);
        break;

        case sf::Keyboard::Num2:
            mychip8.releaseKey(0x02);
        break;

        case sf::Keyboard::Num3:
            mychip8.releaseKey(0x03);
        break;

        case sf::Keyboard::Num4:
            mychip8.releaseKey(0x0C);
        break;

        case sf::Keyboard::Q:
            mychip8.releaseKey(0x04);
        break;

        case sf::Keyboard::W:
            mychip8.releaseKey(0x05);
        break;

        case sf::Keyboard::E:
            mychip8.releaseKey(0x06);
        break;

        case sf::Keyboard::R:
            mychip8.releaseKey(0x0D);
        break;

        case sf::Keyboard::A:
            mychip8.releaseKey(0x07);
        break;

        case sf::Keyboard::S:
            mychip8.releaseKey(0x08);
        break;

        case sf::Keyboard::D:
            mychip8.releaseKey(0x09);
        break;

        case sf::Keyboard::F:
            mychip8.releaseKey(0x0E);
        break;

        case sf::Keyboard::Z:
            mychip8.releaseKey(0x0A);
        break;

        case sf::Keyboard::X:
            mychip8.releaseKey(0x00);
        break;

        case sf::Keyboard::C:
            mychip8.releaseKey(0x0B);
        break;

        case sf::Keyboard::V:
            mychip8.releaseKey(0x0F);
        break;

        default:
        break;
    }
}