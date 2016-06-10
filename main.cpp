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

// lighting
GLfloat LightAmbient[]= { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 5.0f, -10.0f, 1.0f };
GLfloat mat_specular[] = { 0.2, 0.2, 0.2, 1.0 };
 
int window_1, window_2;
 
static int view_state = 0, light_state = 0;
 
int spin;
 
int shape;

int WindowSize = 600;
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

float mouse_x = 0, mouse_y = 0, mouse_z = 0, trans_x = 0, trans_y = 0, angle = 0;
float axis[3];
float zoom = 0.0;
// Buffer
GLuint buffer_vertices,buffer_normals,buffer_faces;


// Functions
void trackball_ptov(int x, int y, float width, float height, float v[3]);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);
// Objects
model myLoader;
GLuint* texture;
//std::vector< glm::vec3 > vertices;
//std::vector< glm::vec2 > faces;
//std::vector< glm::vec3 > normals; // Won't be used at the moment.
//bool res = loadOBJ("../mesh-data/cube.obj", vertices, faces, normals);

void draw_cube(){
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

	//myLoader.Load("../mesh-data/apple/Apple.obj", "../mesh-data/apple/Apple.mtl");

	// Generate buffer
	//glGenBuffers(1, &buffer_vertices);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer_vertices);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &buffer_normals);
	//glBindBuffer(GL_ARRAY_BUFFER, buffer_normals);
	//glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &buffer_faces);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_faces);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(glm::vec2), &faces[0], GL_STATIC_DRAW);
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


	if (shape == 0) Sprint(-3, -7 ,"Solid Cube");
	if (shape == 1) Sprint(-3, -7 ,"Solid Cone");
	if (shape == 2) Sprint(-3, -7 ,"Solid Sphere");
	if (shape == 3) Sprint(-3, -7 ,"Solid Torus");
	if (shape == 4) Sprint(-3, -7 ,"Solid Dodecahedron");
	if (shape == 5) Sprint(-3, -7 ,"Solid Octahedron");
	if (shape == 6) Sprint(-3, -7 ,"Solid Tetrahedron");
	if (shape == 7) Sprint(-3, -7 ,"Solid Icosahedron");
	if (shape == 8) Sprint(-3, -7 ,"Solid Teapot");
 
	// Setup view, and print view state on screen
	if (view_state == 1)
		{
		glColor3f( 1.0, 1.0, 1.0);
		Sprint(-2, 4, "Perspective view");
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 1, 1, 30);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		}else
		{
		glColor3f( 1.0, 1.0, 1.0);
		Sprint(-2, 4, "Ortho view");
		}
 
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
		glColor4f(0.65, 0.65, 0.65, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(0.5, 0.5, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(0.85, 0.85, 0.85, 0.4);
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
	glPushMatrix();
	

	//glRotatef( 45, 1.0, 1.0, 0.0); // rotate cube
	//glRotatef( spin++, 1.0, 1.0, 1.0); // spin cube
 
	if (shape == 0) glutSolidCube(10); // Draw a cube
	//if (shape == 0) myLoader.draw(); //myLoader.loadBmpTexture("../mesh-data/cube.bmp", texture);
	if (shape == 1) glutSolidCone(5,10, 16,16);  // Draw a Cone
	if (shape == 2) glutSolidSphere(5, 16,16 );  // Draw a Sphere
	if (shape == 3) glutSolidTorus( 2.5, 5, 16, 16);
	if (shape == 4)
	   {
		glScalef( 3.5, 3.5, 3.5);
		glutSolidDodecahedron();
	   }
 
	if (shape == 5)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutSolidOctahedron();
	   }
	if (shape == 6)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutSolidTetrahedron();
	   }
 
	if (shape == 7)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutSolidIcosahedron();
	   }
	if (shape == 8) glutSolidTeapot( 5 );
 
	glPopMatrix();
	glutSwapBuffers();
}
 
void display_2(void)
{
	char *p;
 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen
 
	glMatrixMode (GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix
 
 
	glColor3f(1.0, 1.0, 1.0);
	if (shape == 0) Sprint(-3, -7 ,"Wire Cube");
	if (shape == 1) Sprint(-3, -7 ,"Wire Cone");
	if (shape == 2) Sprint(-3, -7 ,"Wire Sphere");
	if (shape == 3) Sprint(-3, -7 ,"Wire Torus");
	if (shape == 4) Sprint(-3, -7 ,"Wire Dodecahedron");
	if (shape == 5) Sprint(-3, -7 ,"Wire Octahedron");
	if (shape == 6) Sprint(-3, -7 ,"Wire Tetrahedron");
	if (shape == 7) Sprint(-3, -7 ,"Wire Icosahedron");
	if (shape == 8) Sprint(-3, -7 ,"Wire Teapot");
 
	// Setup view, and print view state on screen
	if (view_state == 1)
		{
		glColor3f( 1.0, 1.0, 1.0);
		Sprint(-2, 4, "Perspective view");
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, 1, 1, 30);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		}else
		{
		glColor3f( 1.0, 1.0, 1.0);
		Sprint(-2, 4, "Ortho view");
		}
 
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
		glColor4f(0.65, 0.65, 0.65, 0.4);
		glColorMaterial(GL_FRONT, GL_EMISSION);
		glColor4f(0.10, 0.10, 0.10, 0.0);
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		glColor4f(0.5, 0.5, 0.5, 0.4);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColor4f(0.85, 0.85, 0.85, 0.4);
		}
 
	gluLookAt( 0, 0, 20, 0, 0, 0, 0, 1, 0);

	if(zoom!=0)
		glTranslatef(0.0,0.0,-zoom);
	
	if(trans_x != 0||trans_y!=0)
		glTranslatef(trans_x, trans_y, 0);
	
	glMultMatrixf((GLfloat *)m_transform);
	glPushMatrix();

	//glRotatef( 45, 1.0, 1.0, 0.0); // rotate cube
	//glRotatef( spin++, 1.0, 1.0, 1.0); // spin cube
 
	if (shape == 0) glutWireCube(10); // Draw a cube
	if (shape == 1) glutWireCone(5,10, 16,16);  // Draw a Cone
	if (shape == 2) glutWireSphere(5, 16,16 );  // Draw a Sphere
	if (shape == 3) glutWireTorus( 2.5, 5, 16, 16);
	if (shape == 4)
	   {
		glScalef( 3.5, 3.5, 3.5);
		glutSolidDodecahedron();
	   }
 
	if (shape == 5)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutWireOctahedron();
	   }
	if (shape == 6)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutWireTetrahedron();
	   }
 
	if (shape == 7)
	   {
		glScalef( 5.0, 5.0, 5.0);
		glutWireIcosahedron();
	   }
	if (shape == 8) glutWireTeapot( 5 );
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
	  case 's':
	  case 'S':
		  shape++;
		  break;
 	  case 27:
         exit(0); // exit program when [ESC] key presseed
         break;
      default:
         break;
   }
 
if (shape > 8) shape = 0;
 
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
	glutSetWindowTitle("GlutWindow 1");

	init ();
	glutDisplayFunc(display_1);
	glutReshapeFunc(reshape_1);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
 
	window_2 = glutCreateWindow (argv[0]);
	glutSetWindowTitle("GlutWindow 2");
	init ();
	glutDisplayFunc(display_2);
	glutReshapeFunc(reshape_2);
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

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			printf("v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			printf("uv %f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			printf("vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each triangle
	for( unsigned int v=0; v<vertexIndices.size(); v+=3 ){
		// For each vertex of the triangle
		for ( unsigned int i=0; i<3; i+=1 ){

			unsigned int vertexIndex = vertexIndices[v+i];
			glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
			
			unsigned int uvIndex = uvIndices[v+i];
			glm::vec2 uv = temp_uvs[ uvIndex-1 ];
			
			unsigned int normalIndex = normalIndices[v+i];
			glm::vec3 normal = temp_normals[ normalIndex-1 ];
			
			out_vertices.push_back(vertex);
			out_uvs     .push_back(uv);
			out_normals .push_back(normal);
		}
	}

	return true;
}
