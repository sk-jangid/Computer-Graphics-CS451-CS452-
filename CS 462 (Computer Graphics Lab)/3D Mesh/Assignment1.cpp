#include<iostream>
#include<vector>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

struct vertex{

	double x;
	double y;
	double z;

};

struct face{

	int v1;
	int v2;
	int v3;

};


static int z_angle = 0, y_angle=0, x_angle=0;
std::vector<vertex>vertexes;
std::vector<face>faces;


bool readMesh(string objFile){
	string line;
   	ifstream file(objFile);
   	if(!file)
         	return false;
	while(file>>line)
   	{
         	if(*line.c_str()=='v'){
         		vertex v;
                    	file>>v.x>>v.y>>v.z;
                    	vertexes.push_back(v);
         	}
         	else if(*line.c_str()=='f'){
         		face f;
               	file>>f.v1>>f.v2>>f.v3;
               	if(f.v1>0&&f.v2>0&&f.v3>0){
               	faces.push_back(f);}
         	
         	}
        }
        return true;
}

void init(){
	glColor3f (0.2, 0.1, 0.5);
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_FLAT);

}

void displayMesh(){

	
   	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   	glLoadIdentity();
   	
   	glTranslated(0.0, -0.2, -2.0);
   	glColor3f (0.2, 0.1, 0.5);
	
   	glRotatef ((GLfloat) x_angle, 1.0, 0.0, 0.0);
   	glRotatef ((GLfloat) y_angle, 0.0, 1.0, 0.0);
   	glRotatef ((GLfloat) z_angle, 0.0, 0.0, 1.0);
   	
   
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3d(0.5,0.5,0.5);
	//glEnable(GL_EDGE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   	for(int i=0;i<faces.size();i++)
	{            
		
		//glEdgeFlag(GL_TRUE);
		vertex v1= vertexes[faces[i].v1-1];
		vertex v2= vertexes[faces[i].v2-1];
		vertex v3= vertexes[faces[i].v3-1];
		//if(
		
		glVertex3d(v1.x,v1.y,v1.z);
		glVertex3d(v3.x,v3.y,v3.z);
		glVertex3d(v2.x,v2.y,v2.z);
		
		
	}
	glEnd();
	//glutSwapBuffers();	
	//
	glLineWidth(1.5);
	glBegin(GL_LINES);
	
	//glDisable(GL_LINE_STIPPLE);
	
	glColor3f(0.1,0.3,0.0);	
   	for(int i=0;i<faces.size();i++)
	{            
		
		 
		vertex v1= vertexes[faces[i].v1-1];
		vertex v2=vertexes[faces[i].v2-1];
		vertex v3=vertexes[faces[i].v3-1];
		
		
		glVertex3d(v1.x,v1.y,v1.z);
		glVertex3d(v2.x,v2.y,v2.z);
		glVertex3d(v2.x,v2.y,v2.z);
		glVertex3d(v3.x,v3.y,v3.z);
		glVertex3d(v3.x,v3.y,v3.z);
		glVertex3d(v1.x,v1.y,v1.z);
		
		
	}
	glEnd();
	
	
	
	
   	glutSwapBuffers();
	
	

}
void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);

	y_angle = (y_angle + 5) % 360;
	
}

void change_shape (int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1, 1.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
}



void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'z':
         z_angle = (z_angle + 5) % 360;
         glutPostRedisplay();
         break;
      case 'Z':
         z_angle = (z_angle - 5) % 360;
         glutPostRedisplay();
         break;
      case 'x':
         x_angle = (x_angle + 5) % 360;
         glutPostRedisplay();
         break;
      case 'X':
         x_angle = (x_angle - 5) % 360;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}


int main(int argc, char** argv){
	string file = "lowpolybunny.obj";
	
	if(vertexes.size()==0){
		readMesh(file);
		printf("Press x to rotate along X-axis\nPress z to rotate along Z-axis\n");
	}
	glutInit(&argc, argv);
	
  	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  	 glutInitWindowSize(1500, 1500);
  	 glutCreateWindow("170101064 Assignment1 3D Mesh Bunny ");
  	 
  	 init();
  	 
   	glutDisplayFunc(displayMesh);
   	
   	glutReshapeFunc(change_shape);
  	glutKeyboardFunc(keyboard);
  	
	glutTimerFunc(0, timer, 0);
	
	glutMainLoop();
}
