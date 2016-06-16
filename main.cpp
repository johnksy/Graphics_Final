// Glutdualwindow.c
// By Eric Stringer 2002
// Simple examples of OpenGL and Glut usage.
// Keyboard input
// 'v' = view ortho/perspective
// 'l' = lighting on/off
 
 
#include <windows.h>  // This header file will be needed for some windows compilers
//#include <GL/gl.h>   // gl.h and glu.h also maybe needed for some compilers
//#include <GL/glu.h>
#include <GL/glew.h>
#include <GL/glut.h>  // glut (gl utility toolkit) basic windows functions, keyboard, mouse.
#include <stdio.h>    // standard (I/O library)
#include <stdlib.h>   // standard library (set of standard C functions
#include <math.h>     // Math library (Higher math functions )
#include <iostream>
#include <fstream>
#include <cstring>
#include <cfloat>
#include "glm\glm.hpp"
#include <vector>
#include "ObjLoader.h"
#include "Mesh_viewer.h"
#include "textfile.cpp"

// lighting
GLfloat LightAmbient[]= { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 5.0f, -10.0f, 1.0f };
GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 1.0 };
 
float lpos[4] = { 1,0.5,1,0 };

// light color

const float FrontMaterialDiffuse[4] = { 0.55f, 0.55f, 0.55f, 1 };
const float FrontMaterialAmbient[4] = { 0.1f, 0.1f, 0.1f, 1 };
const float FrontMaterialSpecular[4] = { 0.4f, 0.4f, 0.4f, 1 };
const float LightSourceDiffuse[4] = { 1.0, 0, 0, 1 };
const float LightSourceAmbient[4] = { 0.1f, 0.1f, 0.1f, 1 };
const float LightSourceSpecular[4] = { 1.0f, 1.0f, 1.0f, 1 };

const GLfloat LightModelDiffuse[4] = { 1.0, 0, 0, 1 };
const GLfloat LightModelAmbient[4] = { 0.5, 0.5, 0.5, 1 };
const GLfloat LightModelSpecular[4] = { 1.0, 1.0, 1.0, 1 };

// value location in the shader
GLint loc_projection, loc_model, loc_view, loc_normalmat;
GLint loc_k, loc_alpha;
GLint loc_LightSourcePosition;
GLint loc_LightSourceDiffuse, loc_LightSourceAmbient, loc_LightSourceSpecular;
GLint loc_LightModelDiffuse, loc_LightModelAmbient, loc_LightModelSpecular;
GLint loc_FrontMaterialDiffuse, loc_FrontMaterialAmbient, loc_FrontMaterialSpecular;

// coefficient parameters
vec3 k;
float alpha = 4.0;
int range = 4;
float thickness = 0.001;

int window_1, window_2;
 
static int view_state = 1, light_state = 0;
 
int spin;
 
int mode;

int WindowSize = 600;
GLfloat rotation = 0.0;
GLfloat translation = 0.0;
GLfloat theta = 0, phi = 0;
GLfloat eyeX = 0.0, eyeY = 0.0, eyeZ = 20.0;
GLfloat objX = 0.0, objY = 0.0, objZ = 0.0;
GLfloat length_move = 0.0;
/*
	Parameters
*/
  // Virtual trackball parameters
enum Status{
	LEFT_DOWN,
	MIDDLE_DOWN,
	RIGHT_DOWN,
	UP
};

enum Status S;
GLfloat m_transform[16];
GLfloat m_view[16];
GLfloat m_modelview[16];
GLfloat m_projection[16];
GLfloat m_normal[16];

float mouse_x = 0, mouse_y = 0, mouse_z = 0, trans_x = 0, trans_y = 0, angle = 0;
float axis[3];
float zoom = 0.0;

  // title params
const char* title[] = { "../Letters/3.off", "../Letters/D.off", "../Letters/S.off", "../Letters/P.off",
						"../Letters/O.off", "../Letters/T.off", "../Letters/D.off", "../Letters/I.off",
						"../Letters/F.off", "../Letters/F.off", "../Letters/E.off", "../Letters/R.off",
						"../Letters/E.off", "../Letters/N.off", "../Letters/C.off", "../Letters/E.off" };
GLfloat move_x[] = {-4.5, -2.5, -2.5, -1.0, 0.5, 2.0, -2.5, -1.7, -0.9, -0.1, 0.7, 1.5, 2.3, 3.1, 3.9, 4.7};
GLfloat move_y[] = { 1.5, 1.5, -0.8, -0.8, -0.8, -0.8, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0 };
GLfloat scale[] = {0.6, 0.6, 0.4, 0.4, 0.4, 0.4, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};
GLfloat spin_cont = -10;
GLfloat alpha_cont = 1.0;
GLfloat alpha_stat = 1.0;

  //GLSL params
GLuint p[2];
GLuint CurrProg;
// Buffer
GLuint buffer_vertices,buffer_normals,buffer_faces;

// cub map
GLuint color_tex;
int width[6], height[6];
uchar4 *dst[6];


GLuint vBuffer_title[16];
GLuint nBuffer_title[16];
GLuint elemBuffer_title[16];
//GLuint vBuffer_s, nBuffer_s, elemBuffer_s;


// Functions
void trackball_ptov(int x, int y, float width, float height, float v[3]);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void inverseMatrix4x4(const float m[16], float invOut[16]);
void draw_cube();
void draw_objects();
void draw_objects2();
void draw_commons();
// Objects
model The_City0, The_City1, The_City2, The_City3;

model cat;
model The_Dog;
model Box;
model Gorilla;

model The_City0_2, The_City1_2, The_City2_2, The_City3_2;
model cat_2;
model The_Dog_2;
model Box_2;
model Gorilla_2;


void load_data(){
	//myLoader.Load("../mesh-data/The_City/The_City.obj", "../mesh-data/The_City/The_City.mtl");
	//myLoader.Load("../mesh-data/capsule.obj", "../mesh-data/capsule.mtl");
#define STATIC_CUBEMAP
#ifdef STATIC_CUBEMAP
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	LoadBMPFile(&dst[0], &width[0], &height[0], "../sky_box/skybox_back.bmp");
	LoadBMPFile(&dst[1], &width[1], &height[1], "../sky_box/skybox_left.bmp");
	LoadBMPFile(&dst[2], &width[2], &height[2], "../sky_box/skybox_bottom.bmp");
	LoadBMPFile(&dst[3], &width[3], &height[3], "../sky_box/skybox_front.bmp");
	LoadBMPFile(&dst[4], &width[4], &height[4], "../sky_box/skybox_right.bmp");
	LoadBMPFile(&dst[5], &width[5], &height[5], "../sky_box/skybox_top.bmp");
#endif
	The_City0.Load("../mesh-data/The_City0.obj", "../mesh-data/The_City0.mtl");
	The_City1.Load("../mesh-data/The_City1.obj", "../mesh-data/The_City1.mtl");
	The_City2.Load("../mesh-data/The_City2.obj", "../mesh-data/The_City2.mtl");
	The_City3.Load("../mesh-data/The_City3.obj", "../mesh-data/The_City3.mtl");
	cat.Load("../mesh-data/cat.obj", "../mesh-data/cat.mtl");

	The_Dog.Load("../mesh-data/The_Dog/The_Dog.obj", "../mesh-data/The_Dog/The_Dog.mtl");
	Box.Load("../mesh-data/Mr_Handy_Box/Mr_Handy_Box.obj", "../mesh-data/Mr_Handy_Box/Mr_Handy_Box.mtl");
	Gorilla.Load("../mesh-data/Gorilla/Gorilla.obj", "../mesh-data/Gorilla/Gorilla.mtl");
}
void load_data2(){

	The_City0_2.Load("../mesh-data/The_City0.obj", "../mesh-data/The_City0.mtl");
	The_City1_2.Load("../mesh-data/The_City1.obj", "../mesh-data/The_City1.mtl");
	The_City2_2.Load("../mesh-data/The_City2.obj", "../mesh-data/The_City2.mtl");
	The_City3_2.Load("../mesh-data/The_City3.obj", "../mesh-data/The_City3.mtl");
	cat_2.Load("../mesh-data/cat.obj", "../mesh-data/cat.mtl");

	The_Dog_2.Load("../mesh-data/The_Dog/The_Dog.obj", "../mesh-data/The_Dog/The_Dog.mtl");
	Box_2.Load("../mesh-data/Mr_Handy_Box/Mr_Handy_Box.obj", "../mesh-data/Mr_Handy_Box/Mr_Handy_Box.mtl");
	Gorilla_2.Load("../mesh-data/Gorilla/Gorilla.obj", "../mesh-data/Gorilla/Gorilla.mtl");
}

void init_title(int order);

// Objects
//model myLoader;
Mesh_viewer *mesh_title = new Mesh_viewer[16];
//Mesh_viewer mesh_s("../Letters/S.off");
//Mesh_viewer mesh_p("../Letters/P.off");
//Mesh_viewer mesh_o("../Letters/O.off");
//Mesh_viewer mesh_t("../Letters/T.off");



//std::vector< glm::vec3 > vertices;
//std::vector< glm::vec2 > faces;
//std::vector< glm::vec3 > normals; // Won't be used at the moment.
//bool res = loadOBJ("../mesh-data/cube.obj", vertices, faces, normals);

void draw_cube_(){

	glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_normals);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_faces);

	glIndexPointer(GL_UNSIGNED_INT, 0, 0);

	//glDrawElements(GL_TRIANGLES, 3*faces.size(), GL_UNSIGNED_INT, 0);
	/*
	glDrawElements(GL_TRIANGLES, 3*faces.size(), GL_UNSIGNED_INT, 0);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//printf("v size: %d\n", vertices.size());
	//printf("nv size: %d\n", normals.size());
	//printf("f size: %d\n", faces.size());
	*/
}
void draw_block() {
	glBegin(GL_QUADS);
	glVertex3f(-4.7, -2.7, -8);
	glVertex3f(-4.7, -1.5, -8);
	glVertex3f(4.5, -1.5, -8);
	glVertex3f(4.5, -2.7, -8);
	glEnd();

}

// I use this to put text on the screen
void Sprint( int x, int y, char *st)
{
	int l,i;
 
	l=strlen( st ); // see how many characters are in text string.
	glRasterPos2i( x, y); // location to start printing text
	for( i=0; i < l; i++)  // loop until i is greater then l
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
	}
 
}
 
 
// This creates the spinning of the cube.
static void TimeEvent(int te)
{
 
    spin++;  // increase cube rotation by 1
	if (spin > 360) spin = 0; // if over 360 degress, start back at zero.
 
	glutSetWindow( window_1 );
	glutPostRedisplay();  // Update screen with new rotation data
 
	glutSetWindow( window_2 );
	glutPostRedisplay();  // Update screen with new rotation data
 
	glutTimerFunc( 100, TimeEvent, 1);  // Reset our timmer.
}
 
 
// Setup our Opengl world, called once at startup.
void init(void)
{
 
 
	glClearColor (0.0, 0.0, 0.0, 0.0);  // When screen cleared, use black.
	glShadeModel (GL_SMOOTH);  // How the object color will be rendered smooth or flat
	glEnable(GL_DEPTH_TEST);   // Check depth when rendering
	// Lighting is added to scene
	glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
	glEnable(GL_LIGHTING);  // Turn on lighting
	glEnable(GL_LIGHT1);    // Turn on light 1
	
	glewInit();
	glGenTextures(1, &color_tex);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	

	for (int i = 0; i < 16; i++) {
		mesh_title[i].init(title[i]);
		std::cout << mesh_title[i].vSize << " / " << mesh_title[i].iSize << endl;
		init_title(i);
	}


}

void init_title(int order) {
	glGenBuffers(1, &vBuffer_title[order]);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer_title[order]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_title[order].vSize, mesh_title[order].vCoords, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glGenBuffers(1, &nBuffer_title[order]);
	glBindBuffer(GL_ARRAY_BUFFER, nBuffer_title[order]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh_title[order].vSize, mesh_title[order].vsNormal, GL_STATIC_DRAW);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, 0);

	glGenBuffers(1, &elemBuffer_title[order]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer_title[order]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * mesh_title[order].iSize, mesh_title[order].iArray, GL_STATIC_DRAW);

}
 
void title_display(int order) {
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer_title[order]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, nBuffer_title[order]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer_title[order]);
	glTranslatef(move_x[order], move_y[order], -10);
	glRotatef(spin_cont, 0, 1, 0);
	glRotatef(-10, 0, 0, 1);

	spin_cont = spin_cont += 0.5;
	glScalef(scale[order], scale[order], scale[order]);
	glMultMatrixf((GLfloat*)m_transform);
	glDrawElements(GL_TRIANGLES, 3 * mesh_title[order].iSize, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
// Draw our world
void display_1(void)
{
	char *p;
 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen
 
	glMatrixMode (GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix
 
	glColor3f(1.0, 1.0, 1.0);


	if (mode == 0) {
		Sprint(1, -6, "20121092 Sol-A Kim");
		Sprint(1, -7, "20131392 Wonjun Yoon");
	}
 
	// Setup view, and print view state on screen

	glColor3f( 1.0, 1.0, 1.0);
		
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 
	glColor3f( 0.0, 0.0, 1.0);  // Cube color
 
	// Lighting on/off
	if (light_state == 1)
		{
		glDisable(GL_LIGHTING);  // Turn off lighting
		glDisable(GL_COLOR_MATERIAL); // Turn off material, which needs lighting to work
		}else
		{
		glEnable(GL_LIGHTING); // Turn on lighting
		glEnable(GL_COLOR_MATERIAL); // Turn on material settings
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(1.0, 0.65, 0.0, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 1.0, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(1.0, 1.0, 0.0, 0.4);
		}
 
	gluLookAt( 0, 0, 20, 0, 0, 0, 0, 1, 0);
	// only for the first window (display)
	if(angle != 0){
		glPushMatrix();	
		glLoadIdentity();
		glRotatef(angle, axis[0], axis[1], axis[2]);
		glMultMatrixf((GLfloat*)m_transform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*) m_transform);
		glPopMatrix();
		angle = 0;
	}
	if(zoom!=0)
		glTranslatef(0.0,0.0,-zoom);
	
	if(trans_x != 0||trans_y!=0)
		glTranslatef(trans_x, trans_y, 0);
	
	
	glMultMatrixf((GLfloat *)m_transform);
	



	if (mode == 0) {
		//glutSolidCube(10); // Draw a cube
		for (int i = 0; i < 2; i++) {
			title_display(i); //3D
		}
		for (int i = 2; i <6 ; i++) {
			title_display(i); //SPOT
		}
		glLoadIdentity();
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(0.6, 0.1, 1.0, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 0.5, 1.0, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(1.0f, 1.0f, 1.0f, alpha_cont);
	
		draw_block();
		glDisable(GL_BLEND);
		/*
		for (int i = 6; i <16; i++) {
			title_display(i); //DIFFERENCE
		}*/
		/*glLoadIdentity;
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer_title[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, nBuffer_title[0]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer_title[0]);
		glDrawElements(GL_TRIANGLES, 3 * mesh_title[0].iSize, GL_UNSIGNED_INT, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);*/
		
	}
	//if (shape == 0) myLoader.draw(); //myLoader.loadBmpTexture("../mesh-data/cube.bmp", texture);
	if (mode != 0){
		glLoadIdentity();
		//glDisable(GL_LIGHTING);
		gluLookAt(eyeX, eyeY, eyeZ-9.5, objX, objY, objZ, 0, 1, 0);
		//glTranslatef(translation*sin(theta), 0.0, 9.5+ translation*cos(theta));
		//glRotatef(rotation,0,1,0);

		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(1.0f, 1.0f, 1.0f, 0.0);
		glEnable(GL_TEXTURE_2D);

		draw_cube();
		draw_objects();
		draw_commons();
		glPushMatrix();
		glScalef(0.01,0.01,0.01);
		//glTranslatef(-The_City0.midx(), -The_City0.midy(), -The_City0.midz());
		glTranslatef(-The_City0.midx(), -100, -The_City0.midz());
		
		The_City0.draw(); 
		The_City1.draw(); 
		The_City2.draw(); 
		The_City3.draw();
		
		
		glPopMatrix();
	}
	
 
	
	glutSwapBuffers();
}
 
void display_2(void)
{
	char *p;
 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen
 
	glMatrixMode (GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrixl
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix
 
 
	glColor3f(1.0, 1.0, 1.0);
	if (mode == 0) {
		Sprint(1, -6, "20121092 Sol-A Kim");
		Sprint(1, -7, "20131392 Wonjun Yoon");
	}
 
	// Setup view, and print view state on screen

	glColor3f( 1.0, 1.0, 1.0);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 30);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 
	glColor3f( 1.0, 0.8, 0.0);  // Cube color

	
	// Lighting on/off
	if (light_state == 1)
		{
		glDisable(GL_LIGHTING);  // Turn off lighting
		glDisable(GL_COLOR_MATERIAL); // Turn off material, which needs lighting to work
		}else
		{
		glEnable(GL_LIGHTING); // Turn on lighting
		glEnable(GL_COLOR_MATERIAL); // Turn on material settings
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(1.0, 0.65, 0.0, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 1.0, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);

		//glColor4f(0.85, 0.85, 0.85, 0.4);
		//glColorMaterial(GL_FRONT, GL_AMBIENT);
		//glColor4f(0,0,0, 0.4);
		//glColorMaterial(GL_FRONT, GL_EMISSION);
		//glColor4f(0,0,0, 0.0);
		//glColorMaterial(GL_FRONT, GL_SPECULAR);
		//glColor4f(0,0,0, 0.4);
		//glColorMaterial(GL_FRONT, GL_DIFFUSE);
		//glColor4f(0.5880, 0.5880, 0.5880,0.4);

		glColor4f(1.0, 1.0, 0.0, 0.4);

		}
 
	gluLookAt( 0, 0, 20, 0, 0, 0, 0, 1, 0);

	if(zoom!=0)
		glTranslatef(0.0,0.0,-zoom);
	
	if(trans_x != 0||trans_y!=0)
		glTranslatef(trans_x, trans_y, 0);
	
	glMultMatrixf((GLfloat *)m_transform);
	glPushMatrix();

	if (mode == 0) {
		for (int i = 0; i < 2; i++) {
			//glUseProgram(p[0]);
			title_display(i); //3D
		}
		for (int i = 2; i <6; i++) {
			title_display(i); //SPOT
		}
		//glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
		for (int i = 6; i <16; i++) {
		 if(i % 2 ==0){
			 glColorMaterial(GL_FRONT, GL_AMBIENT);
			 glColor4f(0.6, 0.1, 1.0, 0.4);
			 glColorMaterial(GL_FRONT, GL_EMISSION);
			 glColor4f(0.10, 0.10, 0.10, 0.0);
			 glColorMaterial(GL_FRONT, GL_SPECULAR);
			 glColor4f(1.0, 0.5, 1.0, 0.4);
			 glColorMaterial(GL_FRONT, GL_DIFFUSE);
			 glColor4f(0.6, 0.1, 1.0, 0.4);
		 }
		 else {
			 glColorMaterial(GL_FRONT, GL_AMBIENT);
			 glColor4f(1.0, 0.65, 0.0, 0.4);
			 glColorMaterial(GL_FRONT, GL_EMISSION);
			 glColor4f(0.10, 0.10, 0.10, 0.0);
			 glColorMaterial(GL_FRONT, GL_SPECULAR);
			 glColor4f(1.0, 1.0, 0.5, 0.4);
			 glColorMaterial(GL_FRONT, GL_DIFFUSE);
			 glColor4f(1.0, 1.0, 0.0, 0.4);
		 }
		 title_display(i); //DIFFERENCE
		}
		glEnable(GL_BLEND);

		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(0.6, 0.1, 1.0, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 0.5, 1.0, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(0.6, 0.1, 1.0, 0.4);
		glLoadIdentity();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, alpha_cont);
		if (alpha_cont >= 0.55) {
			alpha_cont = alpha_cont / alpha_stat;
		}

		draw_block();
		glDisable(GL_BLEND);
	}
	
	if (mode != 0){
		glLoadIdentity();
		gluLookAt(eyeX, eyeY, eyeZ - 9.5, objX, objY, objZ, 0, 1, 0);

		//glRotatef(90, 0, 1, 0);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(1.0f, 1.0f, 1.0f, 0.0);
		glEnable(GL_TEXTURE_2D);
		draw_cube();
		draw_objects2();
		draw_commons();
		glPushMatrix();
		glScalef(0.01, 0.01, 0.01);

		//glTranslatef(-The_City0.midx(), -The_City0.midy(), -The_City0.midz());
		glTranslatef(-The_City0_2.midx(), -100, -The_City0_2.midz());

		The_City0_2.draw(); 
		The_City1_2.draw(); 
		The_City2_2.draw(); 
		The_City3_2.draw();
		glPopMatrix();
	}

	glPopMatrix();
	glutSwapBuffers();
}
 
 
// This is called when the window has been resized.
void reshape_1 (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
   
   glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*) m_transform);
   k[0] = 0.8;
   k[1] = 0.1;
   k[2] = 0.5;

}
 
// This is called when the window has been resized.
void reshape_2 (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
   //gluLookAt( 0, 0, 20, 0, 0, 0, 0, 1, 0);

}
 
// Read the keyboard
void keyboard (unsigned char key, int x, int y)
{
   switch (key)
   {
 
	  case 'v':
	  case 'V':
		  view_state = abs(view_state -1);
		  break;
	  case 'l':
	  case 'L':
		  light_state = abs(light_state -1);
		  break;
	  case 'm':
	  case 'M':
		  mode++;
		  break;
	  case '1':
		  alpha_stat = alpha_stat + 0.01;
		  break;
 	  case 27:
         exit(0); // exit program when [ESC] key presseed
         break;
	  case 'q':
	  case 'Q':
		  rotation = rotation - 180;
		  break;
	  case 'w':
	  case 'W':
		  translation =  + 0.05;
		  eyeX = eyeX - translation*sin(theta);
		  eyeZ = eyeZ - translation*cos(theta);
		  objX = objX - translation*sin(theta);
		  objZ = objZ - translation*cos(theta);
		  cout << "EYE" << eyeX << " " << eyeZ << endl;
		  cout << "OBJ" << objX << " " << objZ << endl;
		  break;
	  case 's':
	  case 'S':
		  translation =  - 0.05;
		  eyeX = eyeX - translation*sin(theta);
		  eyeZ = eyeZ - translation*cos(theta);
		  objX = objX - translation*sin(theta);
		  objZ = objZ - translation*cos(theta);
		  cout << "EYE" << eyeX << " " << eyeZ << endl;
		  cout << "OBJ" << objX << " " << objZ << endl;
		  break;
	  case 'a':
	  case 'A':
		  theta += 0.03;
		  length_move = sqrt((objX - eyeX)*(objX - eyeX) + (objZ - eyeZ)*(objZ - eyeZ));
		  objX = 20*sin(theta+135);
		  objZ = 20*cos(theta+135);
		  cout << "LENGHT" << length_move << endl;
		  cout << "EYE" << eyeX << " " << eyeZ << endl;
		  cout << "OBJ" << objX << " " << objZ << endl;
		  break;
	  case 'd':
	  case 'D':
		  theta -= 0.03;
		  length_move = sqrt((objX - eyeX)*(objX - eyeX) + (objZ - eyeZ)*(objZ - eyeZ));
		  objX = 20*sin(theta+135);
		  objZ = 20*cos(theta+135);
		  cout << "LENGHT" << length_move << endl;
		  cout << "EYE" << eyeX << " " << eyeZ << endl;
		  cout << "OBJ" << objX << " " << objZ << endl;
		  break;
      default:
         break;
   }
   glutPostRedisplay();
   if (mode > 1) mode = 0;
 
}
void getUniformLoc() {
	loc_projection = glGetUniformLocation(p[CurrProg], "projection");
	loc_view = glGetUniformLocation(p[CurrProg], "view");
	loc_model = glGetUniformLocation(p[CurrProg], "model");
	loc_normalmat = glGetUniformLocation(p[CurrProg], "normalmat");
	loc_LightSourcePosition = glGetUniformLocation(p[CurrProg], "LightSourcePosition");
	loc_LightSourceDiffuse = glGetUniformLocation(p[CurrProg], "LightSourceDiffuse");
	loc_LightSourceAmbient = glGetUniformLocation(p[CurrProg], "LightSourceAmbient");
	loc_LightSourceSpecular = glGetUniformLocation(p[CurrProg], "LightSourceSpecular");
	loc_LightModelDiffuse = glGetUniformLocation(p[CurrProg], "LightModelDiffuse");
	loc_LightModelAmbient = glGetUniformLocation(p[CurrProg], "LightModelAmbient");
	loc_LightModelSpecular = glGetUniformLocation(p[CurrProg], "LightModelSpecular");
	loc_FrontMaterialDiffuse = glGetUniformLocation(p[CurrProg], "FrontMaterialDiffuse");
	loc_FrontMaterialAmbient = glGetUniformLocation(p[CurrProg], "FrontMaterialAmbient");
	loc_FrontMaterialSpecular = glGetUniformLocation(p[CurrProg], "FrontMaterialSpecular");

	loc_alpha = glGetUniformLocation(p[CurrProg], "alpha");
	loc_k = glGetUniformLocation(p[CurrProg], "k");
}
void sendUniformVal() {
	// matrix
	/*glUniformMatrix4fv(loc_projection, 1, false, m_projection);
	glUniformMatrix4fv(loc_view, 1, false, m_view);
	glUniformMatrix4fv(loc_model, 1, false, m_transform);
	inverseMatrix4x4(m_transform, m_normal);
	glUniformMatrix4fv(loc_normalmat, 1, true, m_normal);*/
	// color
	glUniform4fv(loc_LightSourcePosition, 1, lpos);
	glUniform4fv(loc_LightSourceDiffuse, 1, LightSourceDiffuse);
	glUniform4fv(loc_LightSourceAmbient, 1, LightSourceAmbient);
	glUniform4fv(loc_LightSourceSpecular, 1, LightSourceSpecular);
	glUniform4fv(loc_LightModelDiffuse, 1, LightModelDiffuse);
	glUniform4fv(loc_LightModelAmbient, 1, LightModelAmbient);
	glUniform4fv(loc_LightModelSpecular, 1, LightModelSpecular);
	glUniform4fv(loc_FrontMaterialDiffuse, 1, FrontMaterialDiffuse);
	glUniform4fv(loc_FrontMaterialAmbient, 1, FrontMaterialAmbient);
	glUniform4fv(loc_FrontMaterialSpecular, 1, FrontMaterialSpecular);
	// parameter
	glUniform3fv(loc_k, 1, k);
	glUniform1f(loc_alpha, alpha);
}
 
// Main program
int main(int argc, char** argv)
{
	//glutInit(&amp;argc, argv);
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (WindowSize, WindowSize);
	glutInitWindowPosition (10, 10);
	glutTimerFunc( 10, TimeEvent, 1);

	window_1 = glutCreateWindow (argv[0]);
	glutSetWindowTitle("Left_Window");

	init ();
	load_data();
	glutDisplayFunc(display_1);
	glutReshapeFunc(reshape_1);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	//p[0] = createGLSLProgram("../phong.vert", NULL, "../phong.frag");

	window_2 = glutCreateWindow (argv[0]);
	glutSetWindowTitle("Right_Window");
	init ();
	load_data2();
	glutDisplayFunc(display_2);
	glutReshapeFunc(reshape_2);
	//p = createGLSLProgram("../phong.vert", NULL, "../phong.frag");
	glutMainLoop();
	return 0;
}

void trackball_ptov(int x, int y, float width, float height, float v[3]){
	float d, a;
	v[0] = (2.0*x - width) / width;
	v[1] = (height - 2.0*y) / height;
	d = sqrt(v[0]*v[0] + v[1]*v[1]);
	v[2] = cos((3.141592/2.0)*((d<1.0) ? d: 1.0));
	a = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= a;
	v[1] /= a;
	v[2] /= a;
}
void mouse(int button, int state, int x, int y){

	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		S = LEFT_DOWN;
		float prev[3];
		trackball_ptov(x,y, WindowSize,WindowSize, prev);
		mouse_x = prev[0];
		mouse_y = prev[1];
		mouse_z = prev[2];
	}
	if(button==GLUT_LEFT_BUTTON && state == GLUT_UP){
		S = UP;
		angle = 0;
		mouse_x = 0;
		mouse_y = 0;
		mouse_z = 0;
	}
	if(button==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
		S = MIDDLE_DOWN;
		mouse_x = x;
		mouse_y = y;
	}
	if(button==GLUT_MIDDLE_BUTTON && state == GLUT_UP){
		S = UP;
		mouse_x = 0;
		mouse_y = 0;
		mouse_z = 0;
	}
	if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		S = RIGHT_DOWN;
		mouse_x = x;
		mouse_y = y;
	}
	if(button==GLUT_RIGHT_BUTTON && state == GLUT_UP){
		S = UP;
		mouse_x = 0;
		mouse_y = 0;
		mouse_z = 0;
	}
}
void motion(int x, int y){
	switch(S){
		case LEFT_DOWN:
			float curr[3], dx, dy, dz;
			trackball_ptov(x, y, WindowSize, WindowSize, curr);
			dx = curr[0] - mouse_x;
			dy = curr[1] - mouse_y;
			dz = curr[2] - mouse_z;

			if(dx||dy||dz){
				angle = 90*sqrt(dx*dx + dy*dy + dz*dz);
				axis[0] = (mouse_y*curr[2] - mouse_z*curr[1])*0.25;
				axis[1] = (mouse_z*curr[0] - mouse_x*curr[2])*0.25;
				axis[2] = (mouse_x*curr[1] - mouse_y*curr[0])*0.25;
			
				mouse_x = curr[0];
				mouse_y = curr[1];
				mouse_z = curr[2];
			}
			break;
		case MIDDLE_DOWN:
			trans_x += (x - mouse_x)*0.01;
			trans_y += (mouse_y - y)*0.01;
			mouse_x = x;
			mouse_y = y;
			break;
		case RIGHT_DOWN:
			zoom += (mouse_y - y)/100;
			mouse_x = x;
			mouse_y = y;
			break;
	};
	glutPostRedisplay();
}
void inverseMatrix4x4(const float m[16], float invOut[16])
{

	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

}
void draw_cube()
{
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glScalef(2277.354980/200.0, 2000/200.0, 2124.265045/200.0);
	//glTranslatef(-381.2476, 435.447510, -776.047546);
	glPushMatrix();
	//glTranslatef(-776.047546/50.0, 0, -381.2476/50.0);

	glScalef(2000/100.0, 2000/100.0, 2000/100.0);
	glTranslatef(-0.5,-0.1,-0.5);
	//glTranslatef(-381.2476, 435.447510, -776.047546);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[0], height[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[0]);
	glBegin(GL_QUADS);
		// Back
		glNormal3f(0,0,-1);
		glTexCoord2f(0,0);
		glVertex3f(1,1,0);
		glTexCoord2f(1,0);
		glVertex3f(0,1,0);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
		glTexCoord2f(0,1);
		glVertex3f(1,0,0);
		
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[1], height[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[1]);
	glBegin(GL_QUADS);
		// Left	
		glNormal3f(-1,0,0);
		glTexCoord2f(0,0);
		glVertex3f(0,1,0);
		glTexCoord2f(1,0);
		glVertex3f(0,1,1);
		glTexCoord2f(1,1);
		glVertex3f(0,0,1);
		glTexCoord2f(0,1);
		glVertex3f(0,0,0);
		
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[2], height[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[2]);
	glBegin(GL_QUADS);
		// Bottom
		glNormal3f(0,-1,0);
		glTexCoord2f(0,0);
		glVertex3f(0,0,1);
		glTexCoord2f(1,0);
		glVertex3f(1,0,1);
		glTexCoord2f(1,1);
		glVertex3f(1,0,0);
		glTexCoord2f(0,1);
		glVertex3f(0,0,0);
		
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[3], height[3], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[3]);
	glBegin(GL_QUADS);
		// Front
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);
		glVertex3f(0,1,1);
		glTexCoord2f(1,0);
		glVertex3f(1,1,1);
		glTexCoord2f(1,1);
		glVertex3f(1,0,1);
		glTexCoord2f(0,1);
		glVertex3f(0,0,1);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[4], height[4], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[4]);
	glBegin(GL_QUADS);
		// Right
		glNormal3f(1,0,0);
		glTexCoord2f(0,0);
		glVertex3f(1,1,1);
		glTexCoord2f(1,0);
		glVertex3f(1,1,0);
		glTexCoord2f(1,1);
		glVertex3f(1,0,0);
		glTexCoord2f(0,1);
		glVertex3f(1,0,1);
		
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, color_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[5], height[5], 0, GL_RGBA, GL_UNSIGNED_BYTE, dst[5]);
	glBegin(GL_QUADS);
		// Top
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		glVertex3f(0,1,0);
		glTexCoord2f(1,0);
		glVertex3f(1,1,0);
		glTexCoord2f(1,1);
		glVertex3f(1,1,1);
		glTexCoord2f(0,1);
		glVertex3f(0,1,1);
	glEnd();
	glPopMatrix();
}
void draw_objects(){

	//p0
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(17.33, 0, 36.52);
	cat.draw();
	glPopMatrix();

	//p1
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(13.96, 0, -9.55);
	cat.draw();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(-2,-3,1);
	glTranslatef(15.92, 0, -10.58);
	Gorilla.draw();
	glPopMatrix();

	//p2
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(2,-3,0);
	glTranslatef(-17.42, 0, -6.26);
	The_Dog.draw();
	glPopMatrix();

	//p3
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(-2,-3,1);
	glTranslatef(-8.93, 0, 7.78);
	Gorilla.draw();
	glPopMatrix();

	//p4
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	glTranslatef(4,-6,1);
	glTranslatef(22.58, 0, 14.66);
	Box.draw();
	glPopMatrix();

	//p5
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(0.62, 0, -27.05);
	glRotatef(-45,0,1,0);
	cat.draw();
	glPopMatrix();




}
void draw_objects2(){
	//p0
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(17.33, 0, 36.52);
	glRotatef(180,0,1,0);
	cat.draw();
	glPopMatrix();
	//p1
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(-2,-3,1);
	glTranslatef(15.92, 0, -10.58);
	Gorilla.draw();
	glPopMatrix();

	//p2
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(2,-3,0);
	glTranslatef(-17.37, 0, -6.63);
	cat.draw();
	glPopMatrix();
	//p3


	//p4
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	glTranslatef(4,-6,1);
	glTranslatef(22.58, 0, 14.66);
	Box.draw();
	glPopMatrix();
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	glTranslatef(4,-6,1);
	glTranslatef(22.58, 2, 14.66);
	Box.draw();
	glPopMatrix();

	//p5
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(0.62, 0, -27.05);
	cat.draw();
	glPopMatrix();


		
	//glPushMatrix();
	//glScalef(0.2,0.2,0.2);
	//glTranslatef(0,-3,0);
	//glTranslatef(points_x[0], 0, -points_z[0]);
	//if(mode == 1){
	//	if(trans_x != 0||trans_y!=0)
	//		glTranslatef(trans_x, 0, -trans_y);
	//}
	//cat.draw();
	//glPopMatrix();

	//glPushMatrix();
	//glScalef(0.2,0.2,0.2);
	//glTranslatef(2,-3,0);
	//glTranslatef(points_x[1], 0, -points_z[1]);
	//if(mode == 2){
	//	if(trans_x != 0||trans_y!=0)
	//		glTranslatef(trans_x, 0, -trans_y);
	//}
	//The_Dog.draw();
	//glPopMatrix();

	//glPushMatrix();
	//glScalef(0.1,0.1,0.1);
	//glTranslatef(4,-6,1);
	//glTranslatef(points_x[2], 0, -points_z[2]);
	//if(mode == 3){
	//	if(trans_x != 0||trans_y!=0)
	//		glTranslatef(trans_x, 0, -trans_y);
	//}
	//Box.draw();
	//glPopMatrix();

	//glPushMatrix();
	//glScalef(0.2,0.2,0.2);
	//glTranslatef(-2,-3,1);
	//glTranslatef(points_x[3], 0, -points_z[3]);
	//if(mode == 4){
	//	if(trans_x != 0||trans_y!=0)
	//		glTranslatef(trans_x, 0, -trans_y);
	//}
	////glTranslatef(points_x[3], 0, -points_z[3]);
	//Gorilla.draw();
	//glPopMatrix();


}
void draw_commons(){
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(-2,-3,1);
	glRotatef(20,0,1,0);
	Gorilla.draw();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(0,-3,0);
	glTranslatef(-8, 0, -7);
	glRotatef(-20,0,1,0);
	cat.draw();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	glTranslatef(4,-6,1);
	glTranslatef(-1.38, 0, 14.24);
	Box.draw();
	glTranslatef(0, 2, 0);
	Box.draw();
	glTranslatef(0, 2, 0);
	Box.draw();
	glPopMatrix();

}