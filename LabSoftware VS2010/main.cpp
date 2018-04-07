#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef _WIN32
	#include "libs/glut.h"
//	#include <windows.h>
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
#include "Render.h"
#include "MeshLoader.h"
#include <iostream>


//
// ─── MACROS AND DEFINES ─────────────────────────────────────────────────────────
//
using BYTE = unsigned char;
const auto frame_wide = 1280;
const auto frame_high = 720;


 constexpr float Degrees(const float f)
 {
	 return  f * (3.141592653589793238462643383279502884f / 180.0f);
 }

//
// ─── GLOBAL VARIABLES ───────────────────────────────────────────────────────────
//
BYTE	pFrameL[frame_wide * frame_high * 3];
BYTE	pFrameR[frame_wide * frame_high * 3];
int		shade = 0;
auto    xypos = Point(0, 0);
int		stereo = 0;
int		eyes = 10;

/**
 * \brief Render instance for the graphics application 
 */
auto _render = Render(frame_wide, frame_high, 1200, pFrameR);

//
// ─── FORWARD DECLARATIONS ───────────────────────────────────────────────────────
//
void ClearScreen();
void DrawFrame();
void Interlace(BYTE* pL, BYTE* pR);
void BuildFrame(BYTE *pFrame, int view);
void OnIdle();
void OnDisplay();
void Reshape(int w, int h);
void OnMouse(int button, int state, int x, int y);
void OnKeypress(unsigned char key, int x, int y);
void SpecialInput(int key, int x, int y);


//
// ─── Meshes ───────────────────────────────────────────────────────
//
auto mesh = MeshLoader::LoadMesh("Objects/cube.json");
int frameCount = 0;

//
// ───────────────────────────────────────────────────────────────────────────────────────────
//   :::::: P R O G R A M   E N T R Y   P O I N T : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────────────────────────────────
//
int main(int argc, char** argv)
{
//	auto game = new Game();

	// ─── SETUP GLUT ─────────────────────────────────────────────────────────────────
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//GLUT_3_2_CORE_PROFILE |
	glutInitWindowSize(frame_wide, frame_high);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

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
	glutSpecialFunc(SpecialInput);

	mesh.Translate(500, 500, 1000);


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
    glRasterPos2i(0, frame_high-1);
    glDrawPixels(frame_wide, frame_high, GL_RGB,GL_UNSIGNED_BYTE, static_cast<GLubyte *>(pFrameR));
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

void OnKeypress(const unsigned char key, int x, int y)
{
	switch (key) 
	{ 
		case ' ': xypos.x = xypos.y = 0; break;
//		case 's': stereo ^= 1, eyes = 10;break;
		case ']': eyes++;	break;
		case '[': eyes--;	break;
		case 'w': mesh.Rotate(Degrees(-10), Degrees(0), Degrees(0)); break;
		case 's': mesh.Rotate(Degrees(10), Degrees(0), Degrees(0)); break;
		case 'a': mesh.Rotate(Degrees(0), Degrees(0), Degrees(10)); break;
		case 'd': mesh.Rotate(Degrees(0), Degrees(0), Degrees(-10)); break;
		case 'x': mesh.Rotate(Degrees(0), Degrees(-10), Degrees(0)); break;
		case 'z': mesh.Rotate(Degrees(0), Degrees(10), Degrees(0)); break;
		case 27 : exit(0);
		default: ;
	}
	mesh.PrintStatus();
}

void SpecialInput(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)     mesh.Translate(40, 0, 0);
	if (key == GLUT_KEY_LEFT)      mesh.Translate(-40, 0, 0);
	if (key == GLUT_KEY_UP)        mesh.Translate(0, -40, 0);
	if (key == GLUT_KEY_DOWN)      mesh.Translate(0, 40, 0);
	if (key == GLUT_KEY_PAGE_UP)   mesh.Scale(0.1);
	if (key == GLUT_KEY_PAGE_DOWN) mesh.Scale(-0.1);
	mesh.PrintStatus();
}

//
// ─── UTILITY FUNCTIONS ──────────────────────────────────────────────────────────
void ClearScreen()
{
	memset(pFrameL, 0, frame_wide * frame_high * 3);
	memset(pFrameR, 0, frame_wide * frame_high * 3);
}


void Interlace(BYTE* pL, BYTE* pR)
{
	const auto rowlen = 3 * frame_wide;
	for (auto y = 0; y < frame_high; y+=2)
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

//
// ─── MAIN LOOP ───────────────────────────────────────────────────────────
void BuildFrame(BYTE *pFrame, int view)
{
//	const auto colour_white = Colour(100, 255, 100);
//	const auto colour_black = Colour(0, 0, 0);
//
//	auto colour_new = Colour();

//	const auto p1 = Point(300, 200, Colour(255, 0, 0));
//	const auto p2 = Point(100, 400, Colour(0, 255, 0));
//	const auto p3 = Point(500, 400, Colour(0, 0, 255));
//	auto red = Colour(255, 0, 0);
//	auto green = Colour(0, 255, 0);
//	auto blue = Colour(0, 0, 255);

//	const auto points = std::vector<Point>{ Point(0,0, red), Point(200, 0, blue), Point(200, 200), Point(0, 200, green) };
//	const auto points = std::vector<Point>{
//		Point(0, 0, red),
//		Point(100, 0, green),
//		Point(100, 100),
//		Point(300, 100),
//		Point(300, 0),
//		Point(400, 0),
//		Point(400, 400),
//		Point(0, 400, blue)
//	};
//
//	auto points = std::vector<Point>();
//
//	const auto pointCount = 100;
//	float a = 0, x = 0, y = 0;
//	const auto angle = (360.0f / pointCount);
//
//	for (auto i = 0; i < pointCount; i++) {
//		const auto r = rand() % 250;
//
//		a -= angle * (3.14159262 / 180);
//
//		x = r * cos(a) + static_cast<float>(frame_wide) / 2;
//		y = r * sin(a) + static_cast<float>(frame_high) / 2;
//
//		points.emplace_back(x, y, 100, Colour(rand() % 255, rand() % 255, rand() % 255));
//	}
//
//	_render.DrawPolygon(points);

	_render.DrawMesh(mesh);

//	std::cout << "Frame: " << frameCount++ << "\n";
//	_render.ClearZBuffer();
}


