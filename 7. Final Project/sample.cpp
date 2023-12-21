//	Final Project / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Chiayu Tu
//
// Objective:
//		1. To create the Earth and the Moon using texture mapping.
//		2. To utilize lighting techniques to simulate the effect of sunlight, casting shadows on the Earth and the Moon.
//		3. To animate a rocket's journey, demonstrating changes in size and angle as it travels from Earth, 
//		   lands on the Moon, and returns.


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef F_PI
#define F_PI		((float)(M_PI))
#define F_2_PI		((float)(2.f*F_PI))
#define F_PI_2		((float)(F_PI/2.f))
#endif


#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

// title of these windows:

const char *WINDOWTITLE = "OpenGL / GLUT Sample -- Chiayu Tu";
const char *GLUITITLE   = "Final Project";

// what the glui package defines as true and false:

const int GLUITRUE  = true;
const int GLUIFALSE = false;

// the escape key:

const int ESCAPE = 0x1b;

// initial window size:

const int INIT_WINDOW_SIZE = 600; //600

// size of the 3d box to be drawn:

const float BOXSIZE = 2.f; //2

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;

// minimum allowable scale factor:

const float MINSCALE = 0.05f;

// scroll wheel button values:

const int SCROLL_WHEEL_UP   = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = 4;
const int MIDDLE = 2;
const int RIGHT  = 1;

// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH   = 3.;

// the color numbers:
// this order must match the radio button order, which must match the order of the color names,
// 	which must match the order of the color RGB values

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta"
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE     = GL_LINEAR;
const GLfloat FOGDENSITY  = 0.30f;
const GLfloat FOGSTART    = 1.5f;
const GLfloat FOGEND      = 4.f;

// for lighting:

const float	WHITE[ ] = { 1.,1.,1.,1. };

// for animation:

const int MS_PER_CYCLE = 10000;		


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong
//#define DEMO_Z_FIGHTING
//#define DEMO_DEPTH_BUFFER


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
//GLuint	BoxList;				// object display list
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
int		MainWindow;				// window id for main graphics window
int		NowColor;				// index into Colors[ ]
int		NowProjection;		// ORTHO or PERSP
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
float	Time;					// used for animation, this has a value between 0. and 1.
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
//-------------------Texture-------------------
GLuint  SphereDL, EarthDL, MoonDL; // Display lists
GLuint  EarthTex, MoonTex;         // Texture object
//---------------------Obj---------------------
GLuint  RocketObj;
GLuint  FireObj;
//-------------------Rotate--------------------
GLfloat Angle = 0.0f;
GLfloat Speed = 1.0f;
GLfloat Radius = 4.0f;
//-------------------Frozen--------------------
bool    Frozen;
//-----------------LookAtPoint------------------
bool    LookAtPointOn = false;

// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void			Axes( float );
void			HsvRgb( float[3], float [3] );
void			Cross(float[3], float[3], float[3]);
float			Dot(float [3], float [3]);
float			Unit(float [3], float [3]);
float			Unit(float [3]);


// utility to create an array from 3 separate values:

float *
Array3( float a, float b, float c )
{
	static float array[4];

	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

// utility to create an array from a multiplier and an array:

float *
MulArray3( float factor, float array0[ ] )
{
	static float array[4];

	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}


float *
MulArray3(float factor, float a, float b, float c )
{
	static float array[4];

	float* abc = Array3(a, b, c);
	array[0] = factor * abc[0];
	array[1] = factor * abc[1];
	array[2] = factor * abc[2];
	array[3] = 1.;
	return array;
}

// these are here for when you need them -- just uncomment the ones you need:

//#include "setmaterial.cpp"
//#include "setlight.cpp"
#include "osusphere.cpp"
//#include "osucone.cpp"
//#include "osutorus.cpp"
#include "bmptotexture.cpp"
#include "loadobjfile.cpp"
#include "keytime.cpp"
//#include "glslprogram.cpp"

//-------------------Keytime-------------------
// a defined value:
const int MSEC = 30000;		// 10000 milliseconds = 10 seconds

// Rocket and Fire coordinate
Keytimes RFX, RFY, RFZ;
// Fire coordinate
// Keytimes FX, FY, FZ;
// Rocket and Fire scale
Keytimes RFscale;
// Fire scale
// Keytimes Fscale;
// Rocket angle
Keytimes Rangle;
// Fire angle;
Keytimes Fangle;
// Rocket and Fire rotate position
Keytimes RXR, RYR, RZR;
Keytimes FXR, FYR, FZR;
// Eye position
Keytimes eyePosX, eyePosY, eyePosZ;
// UFO color
Keytimes ColorR, ColorG, ColorB;
Keytimes LightPos, LightColR, LightColG, LightColB;

// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since glutInit might
	// pull some command line arguments out)

	glutInit( &argc, argv );

	// setup all the graphics stuff:

	InitGraphics( );

	// create the display lists that **will not change**:

	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );

	// setup all the user interface stuff:

	InitMenus( );

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// glutMainLoop( ) never actually returns
	// the following line is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutPostRedisplay( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables for Display( ) to find:

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;							// makes the value of ms between 0 and MS_PER_CYCLE-1
	Time = (float)ms / (float)MS_PER_CYCLE;		// makes the value of Time between 0. and slightly less than 1.

	//-------------------Rotate--------------------
	Angle += Speed * 0.01f;
	if (Angle >= 2 * M_PI) {
		Angle -= 2 * M_PI;
	}

	// for example, if you wanted to spin an object in Display( ), you might call: glRotatef( 360.f*Time,   0., 1., 0. );

	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting Display.\n");

	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
#ifdef DEMO_DEPTH_BUFFER
	if( DepthBufferOn == 0 )
		glDisable( GL_DEPTH_TEST );
#endif


	// specify shading to be flat:

	glShadeModel( GL_FLAT );

	// set the viewport to be a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( NowProjection == ORTHO )
		glOrtho( -2.f, 2.f,     -2.f, 2.f,     0.1f, 1000.f );
	else
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:

	//gluLookAt( 0.f, 30.f, 120.f,     0.f, 0.f, 0.f,     0.f, 1.f, 0.f );

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0.f, 1.f, 0.f );
	glRotatef( (GLfloat)Xrot, 1.f, 0.f, 0.f );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}

	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[NowColor][0] );
		glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure the normals get unitized:

	glEnable( GL_NORMALIZE );
	//-------------------Keytime-------------------
	int msec = glutGet(GLUT_ELAPSED_TIME) % MSEC; // MSEC should be the total duration of the animation in milliseconds
	float nowTime = (float)msec / 1000.f; // Convert milliseconds to seconds

	//-------------------Rotate--------------------
	// The moon revolves around the earth 
	GLfloat LightX = Radius * cos(Angle);
	GLfloat LightZ = Radius * sin(Angle);	
	// Earth and Moon rotation
	GLfloat angle = Time * 360.0f;

	// draw the box object by calling up its display list:
	//-------------------Texture-------------------
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//-----------------LookAtPoint------------------
	if (!LookAtPointOn) {
		gluLookAt(RFX.GetValue(nowTime), RFY.GetValue(nowTime) + 3, RFZ.GetValue(nowTime), 1.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}
	else {
		gluLookAt(0.f, 30.f, 120.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}
	


	//--------------------Earth---------------------
	glPushMatrix();
		glRotatef(angle, 0.2f, -1.f, 0.f);
		glCallList(EarthDL);
	glPopMatrix();

	//--------------------Moon----------------------
	glPushMatrix();
		//glTranslatef(LightX, 0.0f, LightZ);	
		glRotatef(angle, 0.f, 1.f, 0.f);
		glTranslatef(60., 0., 0.);
		glCallList(MoonDL);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//float light_position[3] = { LightPos.GetValue(nowTime), LightPos.GetValue(nowTime), LightPos.GetValue(nowTime) };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	////glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);

	//float light_color[3] = { LightColR.GetValue(nowTime), LightColG.GetValue(nowTime), LightColB.GetValue(nowTime) };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

	//-----------------Obj Rocket-------------------
	glPushMatrix();
		//-------------------Keytime-------------------
		//glColor3fv(light_color);
		glColor3f(LightColR.GetValue(nowTime), LightColG.GetValue(nowTime), LightColB.GetValue(nowTime));
		glTranslatef(RFX.GetValue(nowTime), RFY.GetValue(nowTime), RFZ.GetValue(nowTime));
		glScalef(RFscale.GetValue(nowTime), RFscale.GetValue(nowTime), RFscale.GetValue(nowTime));
		glRotatef(90, 1., 0., 0.);
		glRotatef(Rangle.GetValue(nowTime), RXR.GetValue(nowTime), RYR.GetValue(nowTime), RZR.GetValue(nowTime));

		glColor3f(ColorR.GetValue(nowTime), ColorG.GetValue(nowTime), ColorB.GetValue(nowTime));
		// Rocket's coordinates on Earth
		/*glTranslatef(-30, -15, 0);
		glScalef(0.7, 0.7, 0.7);
		glRotatef(90, 1., 0., 0.);
		glRotatef(116.31, -0.148, 0, 0.989);*/
		//glTranslatef(0, 0, 18);
		//glScalef(0.3, 0.3, 0.3);
		//glRotatef(90, 1., 0., 0.);
		// Rocket's coordinates on Moon 340
		//glTranslatef(340., 0, 0);

		// Rocket's rotation angle on Earth
		//glRotatef(63, 1., 0., 0.);
		// Rocket's rotation angle on Moon

		glCallList(RocketObj);
	glPopMatrix();

	//-----------------Obj Fire---------------------
	//glPushMatrix();
	////-------------------Keytime-------------------
	//	glTranslatef(RFX.GetValue(nowTime), RFY.GetValue(nowTime), RFZ.GetValue(nowTime));
	//	glScalef(RFscale.GetValue(nowTime), RFscale.GetValue(nowTime), RFscale.GetValue(nowTime));
	//	glRotatef(265, 1., 0., 0.);
	//	glRotatef(Fangle.GetValue(nowTime), FXR.GetValue(nowTime), FYR.GetValue(nowTime), FZR.GetValue(nowTime));
	//	
	//	// Fire's coordinates on Earth
	//	/*glTranslatef(0, 0, 18);
	//	glScalef(0.7, 0.7, 0.7);
	//	glRotatef(265, 1., 0., 0.);*/
	//	// Fire's coordinates on Moon
	//	//glTranslatef(340., 0, 0);

	//	//glRotatef(180, 1, 0., 0.);
	//	// Fire's rotation angle on Moon
	//	//glRotatef(65, 1., 0., 0.);
	//	// Fire's rotation angle on Moon
	//	//glRotatef(90, 0., 0., 1.);
	//	glCallList(FireObj);
	//glPopMatrix();


	
	

#ifdef DEMO_Z_FIGHTING
	if( DepthFightingOn != 0 )
	{
		glPushMatrix( );
			glRotatef( 90.f,   0.f, 1.f, 0.f );
			glCallList( BoxList );
		glPopMatrix( );
	}
#endif


	// draw some gratuitous text that just rotates on top of the scene:
	// i commented out the actual text-drawing calls -- put them back in if you have a use for them
	// a good use for thefirst one might be to have your name on the screen
	// a good use for the second one might be to have vertex numbers on the screen alongside each vertex

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.f, 1.f, 1.f );
	//DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0.f, 100.f,     0.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1.f, 1.f, 1.f );
	//DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	NowColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	NowProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitMenus.\n");

	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(float) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Axis Colors",   colormenu);

#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
#endif

	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup callback functions

void
InitGraphics( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitGraphics.\n");

	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	//-------------------Keytime-------------------
	// Rocket and Fire' X Y and Z coordinate for keytime animation(RX)
	RFX.Init();
	RFY.Init();
	RFZ.Init();

	// Earth to Moon
	RFX.AddTimeValue(0.0, 0.0);
	RFY.AddTimeValue(0.0, 0.0);
	RFZ.AddTimeValue(0.0, 16.0);

	RFX.AddTimeValue(0.3, 0.0);
	RFY.AddTimeValue(0.3, 0.0);
	RFZ.AddTimeValue(0.3, 18.0);

	RFX.AddTimeValue(0.5, -8.0);
	RFY.AddTimeValue(0.5, -5.0);
	RFZ.AddTimeValue(0.5, 20.0);

	RFX.AddTimeValue(1.0, -20.0);
	RFY.AddTimeValue(1.0, -10.0);
	RFZ.AddTimeValue(1.0, 15.0);

	RFX.AddTimeValue(2.5, -30.0);
	RFY.AddTimeValue(2.5, -15.0);
	RFZ.AddTimeValue(2.5, 0.0);

	RFX.AddTimeValue(4.0, -20.0);
	RFY.AddTimeValue(4.0, -10.0);
	RFZ.AddTimeValue(4.0, -15.0);

	RFX.AddTimeValue(5.5, 0.0);
	RFY.AddTimeValue(5.5, 0.0);
	RFZ.AddTimeValue(5.5, -25.0);

	RFX.AddTimeValue(6.5, 20.0);
	RFY.AddTimeValue(6.5, 10.0);
	RFZ.AddTimeValue(6.5, -15.0);

	RFX.AddTimeValue(7.5, 30.0);
	RFY.AddTimeValue(7.5, 15.0);
	RFZ.AddTimeValue(7.5, 0.0);

	RFX.AddTimeValue(8.5, 30.0);
	RFY.AddTimeValue(8.5, 5.0);
	RFZ.AddTimeValue(8.5, 60.0);

	RFX.AddTimeValue(9.0, 55.0);
	RFY.AddTimeValue(9.0, -5.0);
	RFZ.AddTimeValue(9.0, 53.0);

	RFX.AddTimeValue(9.4, 66.0);
	RFY.AddTimeValue(9.4, 0.0);
	RFZ.AddTimeValue(9.4, 30.0);

	RFX.AddTimeValue(9.8, 68.0);
	RFY.AddTimeValue(9.8, 0.0);
	RFZ.AddTimeValue(9.8, 5.0);

	RFX.AddTimeValue(10.0, 65.0);
	RFY.AddTimeValue(10.0, 0.0);
	RFZ.AddTimeValue(10.0, 0.0);

	// Stay on Moon
	RFX.AddTimeValue(10.5, 62.53);
	RFY.AddTimeValue(10.5, 0.0);
	RFZ.AddTimeValue(10.5, -17.75);

	RFX.AddTimeValue(11.0, 53.66);
	RFY.AddTimeValue(11.0, 0.0);
	RFZ.AddTimeValue(11.0, -36.68);

	RFX.AddTimeValue(11.5, 39.69);
	RFY.AddTimeValue(11.5, 0.0);
	RFZ.AddTimeValue(11.5, -51.47);

	RFX.AddTimeValue(12.0, 20.07);
	RFY.AddTimeValue(12.0, 0.0);
	RFZ.AddTimeValue(12.0, -61.83);

	RFX.AddTimeValue(12.5, 0.0);
	RFY.AddTimeValue(12.5, 0.0);
	RFZ.AddTimeValue(12.5, -65.0);

	RFX.AddTimeValue(13.0, -20);
	RFY.AddTimeValue(13.0, 0.0);
	RFZ.AddTimeValue(13.0, -61.85);

	RFX.AddTimeValue(13.5, -39.12);
	RFY.AddTimeValue(13.5, 0.0);
	RFZ.AddTimeValue(13.5, -51.91);

	RFX.AddTimeValue(14.0, -53.44);
	RFY.AddTimeValue(14.0, 0.0);
	RFZ.AddTimeValue(14.0, -37.0);

	RFX.AddTimeValue(14.5, -62.45);
	RFY.AddTimeValue(14.5, 0.0);
	RFZ.AddTimeValue(14.5, -18.02);

	RFX.AddTimeValue(15.0, -65.0);
	RFY.AddTimeValue(15.0, 0.0);
	RFZ.AddTimeValue(15.0, 0.0);
	
	// Moon to Earth
	RFX.AddTimeValue(17.5, -55.0);
	RFY.AddTimeValue(17.5, 10.0);
	RFZ.AddTimeValue(17.5, 20.0);

	RFX.AddTimeValue(18.5, -45.0);
	RFY.AddTimeValue(18.5, 15.0);
	RFZ.AddTimeValue(18.5, 30.0);

	RFX.AddTimeValue(19.5, -35.0);
	RFY.AddTimeValue(19.5, 10.0);
	RFZ.AddTimeValue(19.5, 20.0);

	RFX.AddTimeValue(21.0, -25.0);
	RFY.AddTimeValue(21.0, 0.0);
	RFZ.AddTimeValue(21.0, 0.0);

	RFX.AddTimeValue(22.5, -15.0);
	RFY.AddTimeValue(22.5, -10.0);
	RFZ.AddTimeValue(22.5, -20.0);

	RFX.AddTimeValue(23.5, -5.0);
	RFY.AddTimeValue(23.5, -15.0);
	RFZ.AddTimeValue(23.5, -30.0);

	RFX.AddTimeValue(24.5, 5.0);
	RFY.AddTimeValue(24.5, -10.0);
	RFZ.AddTimeValue(24.5, -20.0);

	RFX.AddTimeValue(26.0, 20.0);
	RFY.AddTimeValue(26.0, 0.0);
	RFZ.AddTimeValue(26.0, 0.0);

	RFX.AddTimeValue(27.0, 15.0);
	RFY.AddTimeValue(27.0, 8.0);
	RFZ.AddTimeValue(27.0, 15.0);

	RFX.AddTimeValue(27.5, 10.0);
	RFY.AddTimeValue(27.5, 6.0);
	RFZ.AddTimeValue(27.5, 18.0);

	RFX.AddTimeValue(28.3, 0.0);
	RFY.AddTimeValue(28.3, 0.0);
	RFZ.AddTimeValue(28.3, 18.0);

	RFX.AddTimeValue(29.0, 0.0);
	RFY.AddTimeValue(29.0, 0.0);
	RFZ.AddTimeValue(29.0, 16.0);

	// Rocket and Fire' scale
	RFscale.Init();
	RFscale.AddTimeValue(0.0, 0.3);
	RFscale.AddTimeValue(0.3, 0.7);
	RFscale.AddTimeValue(0.5, 0.9);
	RFscale.AddTimeValue(9.0, 1.0);
	RFscale.AddTimeValue(8.5, 0.9);
	RFscale.AddTimeValue(9.8, 0.7);
	RFscale.AddTimeValue(10.0, 0.3);
	RFscale.AddTimeValue(10.0, 0.6);
	RFscale.AddTimeValue(18.5, 0.9);
	RFscale.AddTimeValue(21.0, 0.7);
	RFscale.AddTimeValue(26.0, 0.6);
	RFscale.AddTimeValue(27.0, 0.7);
	RFscale.AddTimeValue(28.3, 0.6);
	RFscale.AddTimeValue(29.0, 0.3);

	// Rocket's rotate angle
	// Initialize Keytime variables
	Rangle.Init();
	RXR.Init();
	RYR.Init();
	RZR.Init();

	// Set the rotation angle and rotation axis at each time point

	// Time = 0.0
	Rangle.AddTimeValue(0.0, 0.00);
	RXR.AddTimeValue(0.0, 0.000);
	RYR.AddTimeValue(0.0, 0.000);
	RZR.AddTimeValue(0.0, 0.000);

	// Time = 0.3
	Rangle.AddTimeValue(0.3, 25.25);
	RXR.AddTimeValue(0.3, 0.226);
	RYR.AddTimeValue(0.3, -0.362);
	RZR.AddTimeValue(0.3, 0.000);

	// Time = 0.5
	Rangle.AddTimeValue(0.5, 31.07);
	RXR.AddTimeValue(0.5, 0.210);
	RYR.AddTimeValue(0.5, -0.470);
	RZR.AddTimeValue(0.5, -0.034);

	// Time = 1.0
	Rangle.AddTimeValue(1.0, 33.85);
	RXR.AddTimeValue(1.0, 0.249);
	RYR.AddTimeValue(1.0, -0.498);
	RZR.AddTimeValue(1.0, 0.000);

	// Time = 2.5
	Rangle.AddTimeValue(2.5, 33.85);
	RXR.AddTimeValue(2.5, 0.249);
	RYR.AddTimeValue(2.5, -0.498);
	RZR.AddTimeValue(2.5, 0.000);

	// Time = 4.0
	Rangle.AddTimeValue(4.0, 56.15);
	RXR.AddTimeValue(4.0, 0.371);
	RYR.AddTimeValue(4.0, -0.743);
	RZR.AddTimeValue(4.0, 0.000);

	// Time = 5.5
	Rangle.AddTimeValue(5.5, 56.15);
	RXR.AddTimeValue(5.5, 0.371);
	RYR.AddTimeValue(5.5, -0.743);
	RZR.AddTimeValue(5.5, 0.000);

	// Time = 6.5
	Rangle.AddTimeValue(6.5, 33.85);
	RXR.AddTimeValue(6.5, 0.249);
	RYR.AddTimeValue(6.5, -0.498);
	RZR.AddTimeValue(6.5, 0.000);

	// Time = 7.5
	Rangle.AddTimeValue(7.5, 64.40);
	RXR.AddTimeValue(7.5, 0.399);
	RYR.AddTimeValue(7.5, -0.798);
	RZR.AddTimeValue(7.5, -0.133);

	// Time = 8.5
	Rangle.AddTimeValue(8.5, 21.06);
	RXR.AddTimeValue(8.5, 0.110);
	RYR.AddTimeValue(8.5, 0.332);
	RZR.AddTimeValue(8.5, -0.083);

	// Time = 9.0
	Rangle.AddTimeValue(9.0, 19.84);
	RXR.AddTimeValue(9.0, -0.027);
	RYR.AddTimeValue(9.0, 0.333);
	RZR.AddTimeValue(9.0, 0.059);

	// Time = 9.4
	Rangle.AddTimeValue(9.4, 20.24);
	RXR.AddTimeValue(9.4, 0.000);
	RYR.AddTimeValue(9.4, 0.346);
	RZR.AddTimeValue(9.4, 0.000);

	// Time = 9.8
	Rangle.AddTimeValue(9.8, 4.21);
	RXR.AddTimeValue(9.8, 0.000);
	RYR.AddTimeValue(9.8, 0.073);
	RZR.AddTimeValue(9.8, 0.000);

	// Time = 
	Rangle.AddTimeValue(10.0, 15.85);
	RXR.AddTimeValue(10.0, -0.000);
	RYR.AddTimeValue(10.0, 0.273);
	RZR.AddTimeValue(10.0, 0.000);

	// Time = 10.5
	Rangle.AddTimeValue(10.5, 18.51);
	RXR.AddTimeValue(10.5, 0.000);
	RYR.AddTimeValue(10.5, 0.317);
	RZR.AddTimeValue(10.5, 0.000);

	// Time = 11.0
	Rangle.AddTimeValue(11.0, 18.01);
	RXR.AddTimeValue(11.0, 0.000);
	RYR.AddTimeValue(11.0, 0.309);
	RZR.AddTimeValue(11.0, 0.000);

	// Time = 11.5
	Rangle.AddTimeValue(11.5, 19.65);
	RXR.AddTimeValue(11.5, 0.000);
	RYR.AddTimeValue(11.5, 0.336);
	RZR.AddTimeValue(11.5, 0.000);

	// Time = 12.0
	Rangle.AddTimeValue(12.0, 17.98);
	RXR.AddTimeValue(12.0, 0.000);
	RYR.AddTimeValue(12.0, 0.309);
	RZR.AddTimeValue(12.0, 0.000);

	// Time = 12.5
	Rangle.AddTimeValue(12.5, 17.92);
	RXR.AddTimeValue(12.5, 0.000);
	RYR.AddTimeValue(12.5, 0.308);
	RZR.AddTimeValue(12.5, 0.000);

	// Time = 13.0
	Rangle.AddTimeValue(13.0, 19.08);
	RXR.AddTimeValue(13.0, 0.000);
	RYR.AddTimeValue(13.0, 0.327);
	RZR.AddTimeValue(13.0, 0.000);

	// Time = 13.5
	Rangle.AddTimeValue(13.5, 18.30);
	RXR.AddTimeValue(13.5, 0.000);
	RYR.AddTimeValue(13.5, 0.314);
	RZR.AddTimeValue(13.5, 0.000);

	// Time = 14.0
	Rangle.AddTimeValue(14.0, 18.60);
	RXR.AddTimeValue(14.0, 0.000);
	RYR.AddTimeValue(14.0, 0.319);
	RZR.AddTimeValue(14.0, 0.000);

	// Time = 14.5
	Rangle.AddTimeValue(14.5, 16.10);
	RXR.AddTimeValue(14.5, 0.000);
	RYR.AddTimeValue(14.5, 0.277);
	RZR.AddTimeValue(14.5, 0.000);

	// Time = 15.0
	Rangle.AddTimeValue(15.0, 22.12);
	RXR.AddTimeValue(15.0, 0.000);
	RYR.AddTimeValue(15.0, 0.337);
	RZR.AddTimeValue(15.0, -0.168);

	// Time = 17.5
	Rangle.AddTimeValue(17.5, 14.57);
	RXR.AddTimeValue(17.5, 0.000);
	RYR.AddTimeValue(17.5, 0.225);
	RZR.AddTimeValue(17.5, -0.113);

	// Time = 18.5
	Rangle.AddTimeValue(18.5, 4.13);
	RXR.AddTimeValue(18.5, 0.000);
	RYR.AddTimeValue(18.5, -0.064);
	RZR.AddTimeValue(18.5, 0.032);

	// Time = 19.5
	Rangle.AddTimeValue(19.5, 32.57);
	RXR.AddTimeValue(19.5, 0.000);
	RYR.AddTimeValue(19.5, -0.482);
	RZR.AddTimeValue(19.5, 0.241);

	// Time = 21.0
	Rangle.AddTimeValue(21.0, 56.15);
	RXR.AddTimeValue(21.0, 0.000);
	RYR.AddTimeValue(21.0, -0.743);
	RZR.AddTimeValue(21.0, 0.371);

	// Time = 22.5
	Rangle.AddTimeValue(22.5, 25.38);
	RXR.AddTimeValue(22.5, 0.000);
	RYR.AddTimeValue(22.5, -0.383);
	RZR.AddTimeValue(22.5, 0.192);

	// Time = 23.5
	Rangle.AddTimeValue(23.5, 21.08);
	RXR.AddTimeValue(23.5, 0.000);
	RYR.AddTimeValue(23.5, -0.322);
	RZR.AddTimeValue(23.5, 0.161);

	// Time = 24.5
	Rangle.AddTimeValue(24.5, 77.40);
	RXR.AddTimeValue(24.5, 0.000);
	RYR.AddTimeValue(24.5, -0.873);
	RZR.AddTimeValue(24.5, 0.436);

	// Time = 26.0
	Rangle.AddTimeValue(26.0, 48.58);
	RXR.AddTimeValue(26.0, 0.000);
	RYR.AddTimeValue(26.0, -0.662);
	RZR.AddTimeValue(26.0, 0.353);

	// Time = 27.0
	Rangle.AddTimeValue(27.0, 15.75);
	RXR.AddTimeValue(27.0, 0.111);
	RYR.AddTimeValue(27.0, -0.247);
	RZR.AddTimeValue(27.0, 0.021);

	// Time = 27.5
	Rangle.AddTimeValue(27.5, 32.94);
	RXR.AddTimeValue(27.5, 0.280);
	RYR.AddTimeValue(27.5, -0.466);
	RZR.AddTimeValue(27.5, 0.000);

	// Time = 28.3
	Rangle.AddTimeValue(28.3, 0.0);
	RXR.AddTimeValue(28.3, 0.0);
	RYR.AddTimeValue(28.3, 0.0);
	RZR.AddTimeValue(28.3, 1.0);

	// Time = 28.3
	Rangle.AddTimeValue(29.0, 0.0);
	RXR.AddTimeValue(29.0, 0.0);
	RYR.AddTimeValue(29.0, 0.0);
	RZR.AddTimeValue(29.0, 1.0);

	// Eye X, Y, and Z eye position
	eyePosX.Init();
	eyePosX.AddTimeValue(8.5, 0.0);
	eyePosX.AddTimeValue(9.0, 65.0);
	eyePosX.AddTimeValue(12.0, 0.0);

	// UFO light postition and RGB
	ColorR.Init();
	ColorR.AddTimeValue(0.0, 77.0);
	ColorR.AddTimeValue(5.0, 130.0);
	ColorR.AddTimeValue(10.0, 43.0);
	ColorR.AddTimeValue(15.0, 60.0);
	ColorR.AddTimeValue(20.0, 222.0);
	ColorR.AddTimeValue(25.0, 127.0);
	ColorR.AddTimeValue(30.0, 57.0);

	ColorG.Init();
	ColorG.AddTimeValue(0.0, 12.0);
	ColorG.AddTimeValue(5.0, 104.0);
	ColorG.AddTimeValue(10.0, 211.0);
	ColorG.AddTimeValue(15.0, 68.0);
	ColorG.AddTimeValue(20.0, 81.0);
	ColorG.AddTimeValue(25.0, 189.0);
	ColorG.AddTimeValue(30.0, 116.0);

	ColorB.Init();
	ColorB.AddTimeValue(0.0, 107.0);
	ColorB.AddTimeValue(5.0, 14.0);
	ColorB.AddTimeValue(10.0, 54.0);
	ColorB.AddTimeValue(15.0, 237.0);
	ColorB.AddTimeValue(20.0, 25.0);
	ColorB.AddTimeValue(25.0, 33.0);
	ColorB.AddTimeValue(30.0, 129.0);

	LightPos.Init();
	LightPos.AddTimeValue(0.0, 1.0);
	LightPos.AddTimeValue(5.0, -1.0);
	LightPos.AddTimeValue(10.0, 2.0);
	LightPos.AddTimeValue(15.0, 1.5);
	LightPos.AddTimeValue(20.0, 5.0);
	LightPos.AddTimeValue(25.0, 3.0);
	LightPos.AddTimeValue(30.0, 1.0);

	LightColR.Init();
	LightColR.AddTimeValue(0.0, 0.0);
	LightColR.AddTimeValue(5.0, 1.0);
	LightColR.AddTimeValue(10.0, 0.13);
	LightColR.AddTimeValue(15.0, 0.3);
	LightColR.AddTimeValue(20.0, 1.0);
	LightColR.AddTimeValue(25.0, 1.0);
	LightColR.AddTimeValue(30.0, 1.0);

	LightColG.Init();
	LightColG.AddTimeValue(0.0, 0.8);
	LightColG.AddTimeValue(5.0, 0.0);
	LightColG.AddTimeValue(10.0, 0.8);
	LightColG.AddTimeValue(15.0, 1.0);
	LightColG.AddTimeValue(20.0, 0.6);
	LightColG.AddTimeValue(25.0, 0.8);
	LightColG.AddTimeValue(30.0, 0.8);

	LightColB.Init();
	LightColB.AddTimeValue(0.0, 0.4);
	LightColB.AddTimeValue(5.0, 1.0);
	LightColB.AddTimeValue(10.0, 0.0);
	LightColB.AddTimeValue(15.0, 1.0);
	LightColB.AddTimeValue(20.0, 0.67);
	LightColB.AddTimeValue(25.0, 0.4);
	LightColB.AddTimeValue(30.0, 0.4);

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );

	// setup glut to call Animate( ) every time it has
	// 	nothing it needs to respond to (which is most of the time)
	// we don't need to do this for this program, and really should set the argument to NULL
	// but, this sets us up nicely for doing animation

	glutIdleFunc( Animate );

	// init the glew package (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

	// all other setups go here, such as GLSLProgram and KeyTime setups:
	//-------------------Texture-------------------
	//--------------------Earth--------------------
	int Earthwidth, Earthheight;
	char* Earthfile = (char*)"worldtex.bmp";
	unsigned char* Earthtexture = BmpToTexture(Earthfile, &Earthwidth, &Earthheight);
	if (Earthtexture == NULL) {
		fprintf(stderr, "Cannot open texture '%s'\n", Earthfile);
	}
	else {
		fprintf(stderr, "Opened '%s': width = %d ; height = %d\n", Earthfile, Earthwidth, Earthheight);
	}

	glGenTextures(1, &EarthTex);
	glBindTexture(GL_TEXTURE_2D, EarthTex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Earthwidth, Earthheight, 0, GL_RGB, GL_UNSIGNED_BYTE, Earthtexture);

	//--------------------Moon---------------------
	int Moonwidth, Moonheight;
	char* Moonfile = (char*)"Moon.bmp";
	unsigned char* Moontexture = BmpToTexture(Moonfile, &Moonwidth, &Moonheight);
	if (Moontexture == NULL) {
		fprintf(stderr, "Cannot open texture '%s'\n", Moonfile);
	}
	else {
		fprintf(stderr, "Opened '%s': width = %d ; height = %d\n", Moonfile, Moonwidth, Moonheight);
	}

	glGenTextures(1, &MoonTex);
	glBindTexture(GL_TEXTURE_2D, MoonTex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Moonwidth, Moonheight, 0, GL_RGB, GL_UNSIGNED_BYTE, Moontexture);
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitLists.\n");

	//-------------------Texture-------------------
	//-------------------Sphere--------------------
	SphereDL = glGenLists(1);
	glNewList(SphereDL, GL_COMPILE);
		OsuSphere(15., 30. , 30.);
	glEndList();

	//--------------------Earth---------------------
	EarthDL = glGenLists(1);
	glNewList(EarthDL, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, EarthTex);	
		glPushMatrix();
			//glScalef(0.63f, 0.63f, 0.63f);
			//glRotatef(90., 1., 1., 0.);
			glCallList(SphereDL);		
		glPopMatrix();
	glEndList();

	//--------------------Moon----------------------
	MoonDL = glGenLists(1);
	glNewList(MoonDL, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, MoonTex);
		glPushMatrix();
			glScalef(0.3f, 0.3f, 0.3f);
			glCallList(SphereDL);
		glPopMatrix();
	glEndList();

	//-----------------Obj Rocket-------------------
	RocketObj = glGenLists(1);
	glNewList(RocketObj, GL_COMPILE);
		glPushMatrix();
		glScalef(0.07, 0.07, 0.07);
			//LoadObjFile("rocket.obj");
			LoadObjFile("ufo.obj");
		glPopMatrix();
	glEndList();

	//-----------------Obj Fire---------------------
	FireObj = glGenLists(1);
	glNewList(FireObj, GL_COMPILE);
		glPushMatrix();
			LoadObjFile("Fire.obj");
		glPopMatrix();
	glEndList();


	// create the axes:
	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 25 ); //1.5
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			NowProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			NowProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		//-----------------LookAtPoint------------------
		case 'l':
		case 'L':
			LookAtPointOn = !LookAtPointOn;
			break;

		//-------------------Frozen--------------------
		case 'f':
		case 'F':
			Frozen = !Frozen;
			if (Frozen)
				glutIdleFunc(NULL);
			else
				glutIdleFunc(Animate);
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}

	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}

	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	ShadowsOn = 0;
	NowColor = YELLOW;
	NowProjection = PERSP;
	Xrot = Yrot = 0.;
	//-------------------Frozen--------------------
	Frozen = false;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r=0., g=0., b=0.;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}


float
Unit( float v[3] )
{
	float dist = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		v[0] /= dist;
		v[1] /= dist;
		v[2] /= dist;
	}
	return dist;
}
