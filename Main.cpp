#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "RgbImage.h"
#include "TextureTorus.h"
#include "glut.h"       // OpenGL Graphics Utility Library

const float PI2 = 2.0f*3.1415926535;

GLfloat xRotated, yRotated, zRotated;
// Torus
GLdouble innerRadius = 0.6;
GLdouble outterRadius = 1;
GLint sides = 25;
GLint rings = 25;

// A texture
int TextureWrapVert = 1;
int TextureWrapHoriz = 1;


/*
* Read a texture map from a BMP bitmap file.
*/
void loadTextureFromFile(char *filename)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap(filename);

	// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
	//    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(),
		GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());

}


void displayTorus(void)
{

	glMatrixMode(GL_MODELVIEW);

	glShadeModel(GL_SMOOTH);
	// clear the drawing buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// clear the identity matrix.
	glLoadIdentity();
	// traslate the draw by z = -4.0
	// Note this when you decrease z like -8.0 the drawing will looks far , or smaller.
	glTranslatef(0.0, 0.0, -7.5);

	glClearColor(0.92, 0.85, 0.0, 1.0);

	glEnable(GL_TEXTURE_2D);

	// Set Materials properties
	GLfloat qaBlack[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat qaBlue[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat qaShininess[] = { 20.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaWhite);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaWhite);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, qaWhite);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, qaShininess);


	// Red color used to draw.
	//glColor3f(0.8, 0.6, 0.1);
	// changing in transformation matrix.
	// rotation about X axis
	glRotatef(xRotated, 1.0, 0.0, 0.0);
	// rotation about Y axis
	glRotatef(yRotated, 0.0, 1.0, 0.0);
	// rotation about Z axis
	glRotatef(zRotated, 0.0, 0.0, 1.0);
	// scaling transfomation 
	glScalef(1.0, 1.0, 1.0);
	// built-in (glut library) function , draw you a Torus.


	//glutSolidTorus(innerRadius, outterRadius, sides, rings);
	// Draw the torus
	for (int i = 0; i<rings; i++) {
		glBegin(1 ? GL_QUAD_STRIP : GL_TRIANGLE_STRIP);
		for (int j = 0; j <= sides; j++) {
			putVertTexture(i, j);
			putVertTexture(i + 1, j);
		}
		glEnd();
	}
	// Flush buffers to screen

	glFlush();
	// sawp buffers called because we are using double buffering 
	// glutSwapBuffers();
}

void reshapeTorus(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
								   //Set a new projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0

	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);

	glViewport(0, 0, x, y);  //Use the whole window for rendering
}

void idleTorus(void)
{

	yRotated += 0.01;
	xRotated += 0.01;

	displayTorus();
}

void initRendering(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	xRotated = yRotated = zRotated = 30.0;
	xRotated = 0;
	yRotated = 90;
	glClearColor(1.0, 1.0, 0.0, 1.0);

	//Lighting Set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set light position
	GLfloat qaLightPosition[] = { 0.5, 0.5, 2.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);


	// Load the texture
	//loadTextureFromFile("future.bmp");
	loadTextureFromFile("donut.bmp");
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

int main(int argc, char **argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	//double buffering used to avoid flickering problem in animation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// window size
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 600);
	// create the window 
	glutCreateWindow("Torus Rotating Animation");

	initRendering();

	//Assign  the function used in events
	glutDisplayFunc(displayTorus);
	glutReshapeFunc(reshapeTorus);
	glutIdleFunc(idleTorus);

	//Let start glut loop
	glutMainLoop();
	return 0;
}


/*
* issue vertex command for segment number j of wrap number i.
*/
void putVertTexture(int i, int j) {
	float wrapFrac = (j%rings) / (float)rings;
	float wrapFracTex = (float)j / (float)rings;
	float phi = PI2*wrapFrac;
	float thetaFrac = ((float)(i%sides) + wrapFracTex) / (float)sides;
	float thetaFracTex = ((float)i + wrapFracTex) / (float)sides;
	float theta = PI2*thetaFrac;
	float sinphi = sin(phi);
	float cosphi = cos(phi);
	float sintheta = sin(theta);
	float costheta = cos(theta);
	float y = innerRadius*sinphi;
	float r = outterRadius + innerRadius*cosphi;
	float x = sintheta*r;
	float z = costheta*r;

	glTexCoord2f(wrapFracTex*(float)TextureWrapVert, thetaFracTex*(float)TextureWrapHoriz);
	glNormal3f(sintheta*cosphi, sinphi, costheta*cosphi);
	glVertex3f(x, y, z);
}


