#include <cstdlib>
#include <cstring>

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
#include "MeshManager.h"
#include "GraphicsMath.h"


//
// ─── MACROS AND DEFINES ─────────────────────────────────────────────────────────
using BYTE = unsigned char;
const auto frame_wide = 1280;
const auto frame_high = 720;

//
// ─── GLOBAL VARIABLES ───────────────────────────────────────────────────────────
BYTE	pFrameL[frame_wide * frame_high * 3];
BYTE	pFrameR[frame_wide * frame_high * 3];
int		shade = 0;
auto    xypos = Point(0, 0);
int		stereo = 0;
int		eyes = 10;
bool _stereoDisplay = false;

//
// ─── FPS TIMING ─────────────────────────────────────────────────────────────────
clock_t current_ticks, delta_ticks;
clock_t fps = 0;

/**
 * Render instance for the graphics application 
 */
auto _render = Render(frame_wide, frame_high, 1200, pFrameR);
auto _meshManager = MeshManager(frame_wide, frame_high);

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
void OnKeypress(unsigned char key, int x, int y);
void SpecialInput(int key, int x, int y);

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
//	glutMouseFunc(OnMouse);
	glutSpecialFunc(SpecialInput);

	//
	// ─── RUN THE PROGRAM ────────────────────────────────────────────────────────────
	glutMainLoop();
	return 0;
}

void OnIdle()
{
	DrawFrame();
	glutPostRedisplay();
}

void OnDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPixelZoom( 1, -1 );
    glRasterPos2i(0, frame_high - 1);

	glDrawPixels(frame_wide, frame_high, GL_RGB, GL_UNSIGNED_BYTE, static_cast<GLubyte *>(pFrameR));

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

void OnKeypress(const unsigned char key, int x, int y)
{
	switch (key) 
	{ 
		case ' ': xypos.x = xypos.y = 0; break;
		case ']': eyes++;	break;
		case '[': eyes--;	break;
		case 'w': _meshManager.Rotate(GraphicsMath::Degrees(-10), GraphicsMath::Degrees(0),   GraphicsMath::Degrees(0));   break;
		case 's': _meshManager.Rotate(GraphicsMath::Degrees(10),  GraphicsMath::Degrees(0),   GraphicsMath::Degrees(0));   break;
		case 'x': _meshManager.Rotate(GraphicsMath::Degrees(0),   GraphicsMath::Degrees(0),   GraphicsMath::Degrees(10));  break;
		case 'z': _meshManager.Rotate(GraphicsMath::Degrees(0),   GraphicsMath::Degrees(0),   GraphicsMath::Degrees(-10)); break;
		case 'd': _meshManager.Rotate(GraphicsMath::Degrees(0),   GraphicsMath::Degrees(-10), GraphicsMath::Degrees(0));   break;
		case 'a': _meshManager.Rotate(GraphicsMath::Degrees(0),   GraphicsMath::Degrees(10),  GraphicsMath::Degrees(0));   break;
		case '1': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case '2': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case '3': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case '4': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case '5': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case '6': _meshManager.SwitchMesh(key); /* Play the action sound */ break;
		case 'r': _render.SwitchRenderMode(); break;
		case 'b': _render.ToggleZBuffer(); break;
		case 'v': _stereoDisplay = !_stereoDisplay;
		default: ;
	}

	_meshManager.PrintStatus();
}

void SpecialInput(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)     _meshManager.Translate(40, 0, 0);
	if (key == GLUT_KEY_LEFT)      _meshManager.Translate(-40, 0, 0);
	if (key == GLUT_KEY_UP)        _meshManager.Translate(0, -40, 0);
	if (key == GLUT_KEY_DOWN)      _meshManager.Translate(0, 40, 0);
	if (key == GLUT_KEY_PAGE_UP)   _meshManager.Scale(0.1);
	if (key == GLUT_KEY_PAGE_DOWN) _meshManager.Scale(-0.1);
	_meshManager.PrintStatus();
}

//
// ─── UTILITY FUNCTIONS ──────────────────────────────────────────────────────────
//void DrawText(const int x, const int y, const std::string text)
//{
//	
//}

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
	if (!_stereoDisplay) 
	{
		BuildFrame(pFrameR, 0);
	}
	else 
	{
		BuildFrame(pFrameL, -eyes);
		BuildFrame(pFrameR, +eyes);
		Interlace(static_cast<BYTE *>(pFrameL), static_cast<BYTE *>(pFrameR));
	}
}


//
// ─── MAIN LOOP ───────────────────────────────────────────────────────────
void BuildFrame(BYTE *pFrame, int view)
{
	// TIMING --- Log the start of the frame draw
	current_ticks = clock();

	// ─────────────────────────────────────────────────────────────────────
	// START RENDER
	// ─────────────────────────────────────────────────────────────────────

//    // Point generator for convex polygon creation
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
//	_render.DrawPolygon(points);

	if (_stereoDisplay)
	{
		for (auto& mesh: _meshManager.GetMeshes())
		{
			mesh.Translate(view, 0, 0);
		}
	}

	for (const auto& mesh: _meshManager.GetMeshes())
	{
		_render.DrawMesh(mesh);
	}

	_render.ClearZBuffer();

	// ─────────────────────────────────────────────────────────────────────
	// END RENDER
	// ─────────────────────────────────────────────────────────────────────

	// TIMING --- Log the end of the frame draw and calculate the FPS
	delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
	if (delta_ticks > 0)
	{
		fps = CLOCKS_PER_SEC / delta_ticks;
	}

//	auto ss = std::ostringstream();
	std::cout << "FPS: " << static_cast<int>(fps) << "\n";

//	basic = ss.str();
}


