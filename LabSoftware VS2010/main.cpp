#include <stdlib.h>			//- for exit()
#include <stdio.h>			//- for sprintf()
#include <string.h>			//- for memset()
#include "Colour.h"

#ifdef _WIN32
	#include "libs/glut.h"
	#include <windows.h>
	#pragma comment(lib, "winmm.lib")			//- not required but have it in anyway
	#pragma comment(lib, "libs/glut32.lib")
#elif __APPLE__
	#include <GLUT/glut.h>
#elif __unix__		// all unices including  __linux__
	#include <GL/glut.h>
#endif

//====== Macros and Defines =========

#define FRAME_WIDE	1000
#define FRAME_HIGH	600
#define ROUND(x) ((int)(x+0.5)) 

//====== Structs & typedefs =========
typedef unsigned char BYTE;
struct POINT2D {int x, y;};


// Utility Classes
class Point {
public:
	int x;
	int y;

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Colour {
public:
	int r;
	int g;
	int b;

	Colour(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

};



//====== Global Variables ==========
BYTE	pFrameL[FRAME_WIDE * FRAME_HIGH * 3];
BYTE	pFrameR[FRAME_WIDE * FRAME_HIGH * 3];
int		shade = 0;
POINT2D	xypos = {0,0};
int		stereo = 0;
int		eyes = 10;

//===== Forward Declarations ========
void ClearScreen();
void DrawFrame();
void Interlace(BYTE* pL, BYTE* pR);
void PlaySoundEffect(char * filename);
void BuildFrame(BYTE *pFrame, int view);
void OnIdle(void);
void OnDisplay(void);
void reshape(int w, int h);
void OnMouse(int button, int state, int x, int y);
void OnKeypress(unsigned char key, int x, int y);

////////////////////////////////////////////////////////
// Program Entry Poinr
////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	//-- setup GLUT --
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//GLUT_3_2_CORE_PROFILE |
	glutInitWindowSize(FRAME_WIDE, FRAME_HIGH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
/*
#ifdef WIN32
	//- eliminate flashing --
	typedef void (APIENTRY * PFNWGLEXTSWAPCONTROLPROC) (int i);
	PFNWGLEXTSWAPCONTROLPROC wglSwapControl = NULL;
	wglSwapControl = (PFNWGLEXTSWAPCONTROLPROC) wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapControl != NULL) wglSwapControl(1); 
#endif
*/

	//--- set openGL state --
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//-- register call back functions --
	glutIdleFunc(OnIdle);
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(OnKeypress);
	glutMouseFunc(OnMouse);

	//-- run the program
	glutMainLoop();
	return 0;
}


////////////////////////////////////////////////////////
// Event Handers
////////////////////////////////////////////////////////
      
void OnIdle(void)
{
	DrawFrame();
	glutPostRedisplay();
}


void OnDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom( 1, -1 );
    glRasterPos2i(0, FRAME_HIGH-1);
    glDrawPixels(FRAME_WIDE, FRAME_HIGH, GL_RGB,GL_UNSIGNED_BYTE, (GLubyte*)pFrameR);
    glutSwapBuffers();
    glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		PlaySoundEffect("Laser.wav"); 
		if (++shade > 16) shade = 0;	
	}
}

void OnKeypress(unsigned char key, int x, int y)
{
	switch (key) 
	{ 
	case ' ': xypos.x = xypos.y = 0; break;
	case 's': stereo ^= 1, eyes = 10;break;
	case ']': eyes++;	break;
	case '[': eyes--;	break;
	case 27 : exit(0);
	}
	PlaySoundEffect("Whoosh.wav"); 
}


////////////////////////////////////////////////////////
// Utility Functions
////////////////////////////////////////////////////////


void ClearScreen()
{
	memset(pFrameL, 0, FRAME_WIDE * FRAME_HIGH * 3);
	memset(pFrameR, 0, FRAME_WIDE * FRAME_HIGH * 3);
}


void Interlace(BYTE* pL, BYTE* pR)
{
	int rowlen = 3 * FRAME_WIDE;
	for (int y = 0; y < FRAME_HIGH; y+=2)
	{
		for (int x = 0; x < rowlen; x++) *pR++ = *pL++;
		pL += rowlen;
		pR += rowlen;
	}
}


void DrawFrame()
{
	ClearScreen();
	
	if (!stereo) BuildFrame(pFrameR, 0);
	else {
		BuildFrame(pFrameL, -eyes);
		BuildFrame(pFrameR, +eyes);
		Interlace((BYTE*)pFrameL, (BYTE*)pFrameR);
	}
}

void	PlaySoundEffect(char * filename)		
{
#ifdef _WIN32
	PlaySound(filename, NULL, SND_ASYNC | SND_FILENAME ); 
#else
	char command[80];
	#ifdef __APPLE__
		sprintf(command, "afplay %s &", filename);
	#else
		sprintf(command, "play %s &", filename);
	#endif	
	system(command);
#endif
}

void SetPixel(BYTE *screen, Point point, Colour colour)
{
	// Calculate the position in the 1D Array
	int position = (point.x + point.y * FRAME_WIDE) * 3;

	screen[position] = colour.r;
	screen[position + 1] = colour.g;
	screen[position + 2] = colour.b;
}

void DDA_DrawLine(Point p0, Point p1, Colour colour, BYTE* screen)
{
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x; 
	int y1 = p1.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int steps;
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	} else {
		steps = abs(dy);
	}

	double x_inc = dx / (double) steps;
	double y_inc = dy / (double) steps;

	double x = x0;
	double y = y0;

	SetPixel(screen, Point(ROUND(x), ROUND(y)), colour);

	for (int i = 0; i < steps; i++, x += x_inc, y += y_inc) {
		SetPixel(screen, Point(ROUND(x), ROUND(y)), colour);
	}

}

////////////////////////////////////////////////////////
// Drawing Function
////////////////////////////////////////////////////////

void BuildFrame(BYTE *pFrame, int view)
{
	BYTE*	screen = (BYTE*)pFrame;		// use copy of screen pointer for safety

	// for (int i = 0; i < 10000; i++) {
	// 	int x = rand() % FRAME_WIDE;
	// 	int y = rand() % FRAME_HIGH;
	// 	unsigned char r = rand() % 256;
	// 	unsigned char g = rand() % 256;
	// 	unsigned char b = rand() % 256;

	// 	SetPixel(screen, x, y, r, g, b);

	// }
	auto p0 = Point(0, 0);
	auto p1 = Point(500, 500);
	auto colour_white = Colour(255, 255, 255);

	DDA_DrawLine(p0, p1, colour_white, screen);
}


