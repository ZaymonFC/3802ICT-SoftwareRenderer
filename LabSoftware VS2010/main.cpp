#include <cstdlib>			//- for exit()
#include <cstdio>			//- for sprintf()
#include <cstring>			//- for memset()
#include <cmath>


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

//
// ─── CLASSES AND TYPES ──────────────────────────────────────────────────────────
//
#include "Point.h"
#include "Colour.h"

//
// ─── MACROS AND DEFINES ─────────────────────────────────────────────────────────
//
#define FRAME_WIDE	1000
#define FRAME_HIGH	600
//#define ROUND(x) ((int)(x+0.5)) 

//
// ─── GLOBAL VARIABLES ───────────────────────────────────────────────────────────
//
BYTE	pFrameL[FRAME_WIDE * FRAME_HIGH * 3];
BYTE	pFrameR[FRAME_WIDE * FRAME_HIGH * 3];
int		shade = 0;
auto    xypos = Point(0, 0);
int		stereo = 0;
int		eyes = 10;

//
// ─── FORWARD DECLARATIONS ───────────────────────────────────────────────────────
//
void ClearScreen();
void DrawFrame();
void Interlace(BYTE* pL, BYTE* pR);
void BuildFrame(BYTE *pFrame, int view);
void OnIdle(void);
void OnDisplay(void);
void Reshape(int w, int h);
void OnMouse(int button, int state, int x, int y);
void OnKeypress(unsigned char key, int x, int y);


//
// ───────────────────────────────────────────────────────────────────────────────────────────
//   :::::: P R O G R A M   E N T R Y   P O I N T : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────────────────────────────────
//
int main(int argc, char** argv)
{
	// ─── SETUP GLUT ─────────────────────────────────────────────────────────────────
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

	// ─── SET OPENGL STATE ───────────────────────────────────────────────────────────
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// ─── REGISTER CALL BACK FUNCTIONS ───────────────────────────────────────────────
	glutIdleFunc(OnIdle);
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(OnKeypress);
	glutMouseFunc(OnMouse);

	//
	// ─── RUN THE PROGRAM ────────────────────────────────────────────────────────────
	glutMainLoop();
	return 0;
}


//
// ─── EVENT HANDERS ──────────────────────────────────────────────────────────────
//  
void OnIdle()
{
	DrawFrame();
	glutPostRedisplay();
}


void OnDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom( 1, -1 );
    glRasterPos2i(0, FRAME_HIGH-1);
    glDrawPixels(FRAME_WIDE, FRAME_HIGH, GL_RGB,GL_UNSIGNED_BYTE, static_cast<GLubyte *>(pFrameR));
    glutSwapBuffers();
    glFlush();
}

void Reshape(const int w, const int h)
{
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, static_cast<GLdouble>(w), 0.0, static_cast<GLdouble>(h));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void OnMouse(int button, int state, int x, int y)
{
	//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	//	{
	//		PlaySoundEffect("Laser.wav"); 
	//		if (++shade > 16) shade = 0;	
	//	}
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
		default: ;
	}
	// PlaySoundEffect("Whoosh.wav"); 
}


//
// ─── UTILITY FUNCTIONS ──────────────────────────────────────────────────────────
void ClearScreen()
{
	memset(pFrameL, 0, FRAME_WIDE * FRAME_HIGH * 3);
	memset(pFrameR, 0, FRAME_WIDE * FRAME_HIGH * 3);
}


void Interlace(BYTE* pL, BYTE* pR)
{
	const auto rowlen = 3 * FRAME_WIDE;
	for (auto y = 0; y < FRAME_HIGH; y+=2)
	{
		for (auto x = 0; x < rowlen; x++) *pR++ = *pL++;
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
		Interlace(static_cast<BYTE *>(pFrameL), static_cast<BYTE *>(pFrameR));
	}
}


// void	PlaySoundEffect(LPWCSTR filename)		
// {
// 	#ifdef _WIN32
// 		PlaySound(filename, NULL, SND_ASYNC | SND_FILENAME ); 
// 	#else
// 		char command[80];
// 		#ifdef __APPLE__
// 			sprintf(command, "afplay %s &", filename);
// 		#else
// 			sprintf(command, "play %s &", filename);
// 		#endif	
// 		system(command);
// 	#endif
// }


void SetPixel(BYTE *screen, Point point, Colour colour)
{
	// Calculate the position in the 1D Array
	const auto position = (point.x + point.y * FRAME_WIDE) * 3;

	screen[position] = colour.r;
	screen[position + 1] = colour.g;
	screen[position + 2] = colour.b;
}


void DrawLine_Dda(const Point p0, const Point p1, const Colour colour, BYTE* screen)
{
	const auto x0 = p0.x;
	const auto y0 = p0.y;
	const auto x1 = p1.x; 
	const auto y1 = p1.y;

	const auto dx = abs(x1 - x0);
	const auto dy = abs(y1 - y0);

	int steps;
	if (abs(dx) > abs(dy)) {
		steps = abs(dx);
	} else {
		steps = abs(dy);
	}

	const auto x_inc = dx / static_cast<double>(steps);
	const auto y_inc = dy / static_cast<double>(steps);

	double x = x0;
	double y = y0;

	SetPixel(screen, Point(lround(x), lround(y)), colour);

	for (auto i = 0; i < steps; i++, x += x_inc, y += y_inc) {
		SetPixel(screen, Point(lround(x), lround(y)), colour);
	}
}


//
// ─── DRAWING FUNCTION ───────────────────────────────────────────────────────────
void BuildFrame(BYTE *pFrame, int view)
{
	const auto screen = static_cast<BYTE *>(pFrame);

	//
	// ─── CODE FOR PIXEL SNOW ────────────────────────────────────────────────────────
	for (auto i = 0; i < 20000; i++) {
		const auto point = Point(rand() % FRAME_WIDE, rand() % FRAME_HIGH);
		const auto colour = Colour(rand() % 256, rand() % 256, rand() % 256);

		SetPixel(screen, point, colour);
	}

	for (auto i = 0; i < 100; i ++)
	{
		const auto p0 = Point(rand() % 500, rand() % 300);
		const auto p1 = Point(rand() % 300, rand() % 300);
		const auto colour_white = Colour(rand() % 256, rand() % 256, rand() % 256);

		DrawLine_Dda(p0, p1, colour_white, screen);
	}
	Sleep(10);

}


