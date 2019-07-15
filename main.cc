#include <iostream>
#include <GL/glut.h>
#include "Chip_8.hpp"


// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

Chip_8 myChip8;
int modifier = 10;

// Window size
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

void display();
void reshape_window(GLsizei w, GLsizei h);
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);

typedef unsigned char u8;
u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3]; 
void setupTexture();

int main(int argc, char **argv)
{
    if(argc < 2)
	{
		printf("Usage: ./chip8-app chip8-application\n\n");
		return -1;
	}

    if(!myChip8.load_application(argv[1]))
    {
        printf("Failed to Load ` %s ` as a chip 8 application\n",argv[1]);
        return -2;
    }

    printf("Welcome to Chip 8 Interpreter. \n");
    printf("%s is loaded successfully\n",argv[1]);

	// Setup OpenGL
	glutInit(&argc, argv);          
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);


	glutInitWindowSize(display_width, display_height);
    glutInitWindowPosition(320, 320);
	glutCreateWindow("myChip8 by Yahia Farghaly");
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
    glutReshapeFunc(reshape_window);        
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp); 
	

	glutMainLoop(); 

    return 0;
}

// Old GFX code
void drawPixel(int x, int y)
{
	glBegin(GL_QUADS);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + 0.0f,	 0.0f);
		glVertex3f((x * modifier) + 0.0f,     (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + modifier, 0.0f);
		glVertex3f((x * modifier) + modifier, (y * modifier) + 0.0f,	 0.0f);
	glEnd();
}

void updateQuads(const Chip_8& c8)
{
	// Draw
	for(int y = 0; y < 32; ++y)		
		for(int x = 0; x < 64; ++x)
		{
			if(c8.GFX[y][x] == 0) 
				glColor3f(0.0f,0.0f,0.0f);			
			else 
				glColor3f(1.0f,1.0f,1.0f);

			drawPixel(x, y);
		}
}

void display()
{
	myChip8.emulateCycle();
		
	if(myChip8.drawFlag)
	{
		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);

		updateQuads(myChip8);		

		// Swap buffers!
		glutSwapBuffers();    

		// Processed frame
		myChip8.drawFlag = false;
	}
}

void reshape_window(GLsizei w, GLsizei h)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);        
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

	// Resize quad
	display_width = w;
	display_height = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == '1')		myChip8.pressKey(0x01);
	else if(key == '2')	myChip8.pressKey(0x02);
	else if(key == '3')	myChip8.pressKey(0x03);
	else if(key == '4')	myChip8.pressKey(0x0C);

	else if(key == 'q')	myChip8.pressKey(0x04);
	else if(key == 'w')	myChip8.pressKey(0x05);
	else if(key == 'e')	myChip8.pressKey(0x06);
	else if(key == 'r')	myChip8.pressKey(0x0D);

	else if(key == 'a')	myChip8.pressKey(0x07);
	else if(key == 's')	myChip8.pressKey(0x08);
	else if(key == 'd')	myChip8.pressKey(0x09);
	else if(key == 'f')	myChip8.pressKey(0x0E);

	else if(key == 'z')	myChip8.pressKey(0x0A);
	else if(key == 'x')	myChip8.pressKey(0x00);
	else if(key == 'c')	myChip8.pressKey(0x0B);
	else if(key == 'v')	myChip8.pressKey(0x0F);
}

void keyboardUp(unsigned char key, int x, int y)
{
	if(key == '1')		myChip8.releaseKey(0x01);
	else if(key == '2')	myChip8.releaseKey(0x02);
	else if(key == '3')	myChip8.releaseKey(0x03);
	else if(key == '4')	myChip8.releaseKey(0x0C);

	else if(key == 'q')	myChip8.releaseKey(0x04);
	else if(key == 'w')	myChip8.releaseKey(0x05);
	else if(key == 'e')	myChip8.releaseKey(0x06);
	else if(key == 'r')	myChip8.releaseKey(0x0D);

	else if(key == 'a')	myChip8.releaseKey(0x07);
	else if(key == 's')	myChip8.releaseKey(0x08);
	else if(key == 'd')	myChip8.releaseKey(0x09);
	else if(key == 'f')	myChip8.releaseKey(0x0E);

	else if(key == 'z')	myChip8.releaseKey(0x0A);
	else if(key == 'x')	myChip8.releaseKey(0x00);
	else if(key == 'c')	myChip8.releaseKey(0x0B);
	else if(key == 'v')	myChip8.releaseKey(0x0F);
}