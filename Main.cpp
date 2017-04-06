#include "glut.h"

GLfloat xRotated, yRotated, zRotated;
// Torus
GLdouble innerRadius = 0.3;
GLdouble outterRadius = 1;
GLint sides = 50;
GLint rings = 50;


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
	glTranslatef(0.0, 0.0, -6);
	
	// Set Materials properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat qaWhite[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat qaShininess[] = { 120.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, qaGreen);
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

	
	glutSolidTorus(innerRadius, outterRadius, sides, rings);
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
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Lighting Set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// Set light position
	GLfloat qaLightPosition[] = { 0.5, 0.5, 2.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

	//Assign  the function used in events
	glutDisplayFunc(displayTorus);
	glutReshapeFunc(reshapeTorus);
	glutIdleFunc(idleTorus);

	// 2D textures
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
}

int main(int argc, char **argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	//double buffering used to avoid flickering problem in animation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// window size
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 600);
	// create the window 
	glutCreateWindow("Torus Rotating Animation");

	initRendering();

	//Let start glut loop
	glutMainLoop();
	return 0;
} 