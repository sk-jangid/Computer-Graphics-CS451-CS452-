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


static int z_angle = 0, y_angle=0, x_angle=0, is_pressed =0;

static  int mouse_x1 = 0,mouse_y1 = 0;
static float camera_x = 0,camera_y = 0,camera_z = -3,pos_x = 0,pos_y = 0;


std::vector<vertex>vertexes;
std::vector<face>faces;
GLuint polygons;

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
         }
        file.clear();
	file.seekg(0, ios::beg);
	
	while(file>>line){
         	if(*line.c_str()=='f'){
         		glBegin(GL_TRIANGLES);
         		face f;
               	file>>f.v1>>f.v2>>f.v3;
               	if(f.v1>0&&f.v2>0&&f.v3>0){
               	faces.push_back(f);}
               	else{continue;}
               	
               	vertex v1= vertexes[f.v1-1];
			vertex v2= vertexes[f.v2-1];
			vertex v3= vertexes[f.v3-1];
			
			glVertex3f(v1.x,v1.y,v1.z);
			glVertex3f(v2.x,v2.y,v2.z);
			glVertex3f(v3.x,v3.y,v3.z);
			
			glEnd();
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

	
   	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
   	
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
   	glColor3f(1.0,1.0,1.0);
   	
   	
   	glTranslatef(camera_x, camera_y, camera_z);
   	
	//glTranslatef(0,0,-2);
   	glRotatef ((GLfloat) x_angle, 1.0, 0.0, 0.0);
   	glRotatef ((GLfloat) y_angle, 0.0, 1.0, 0.0);
   	glRotatef ((GLfloat) z_angle, 0.0, 0.0, 1.0);
   	
   	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(0.5,0.5,0.5);
	
	
   	for(int i=0;i<faces.size();i++)
	{            
		
		//glEdgeFlag(GL_TRUE);
		vertex v1= vertexes[faces[i].v1-1];
		vertex v2= vertexes[faces[i].v2-1];
		vertex v3= vertexes[faces[i].v3-1];
		//if(
		
		glVertex3f(v1.x,v1.y,v1.z);
		glVertex3f(v3.x,v3.y,v3.z);
		glVertex3f(v2.x,v2.y,v2.z);
		
		
	}
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
	glColor3f(0.1,0.3,0.0);
	glLineWidth(1.5);
	glBegin(GL_LINES);

   	for(int i=0;i<faces.size();i++)
	{            
		
		 
		vertex v1= vertexes[faces[i].v1-1];
		vertex v2=vertexes[faces[i].v2-1];
		vertex v3=vertexes[faces[i].v3-1];
		
		
		glVertex3f(v1.x,v1.y,v1.z);
		glVertex3f(v2.x,v2.y,v2.z);
		glVertex3f(v2.x,v2.y,v2.z);
		glVertex3f(v3.x,v3.y,v3.z);
		glVertex3f(v3.x,v3.y,v3.z);
		glVertex3f(v1.x,v1.y,v1.z);
		
		
	}
	glEnd();
	
   	// enable double buffer
   	glutSwapBuffers();
   	

}


void change_shape (int w, int h)
{

   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(15.0, 1, 1.0, 50);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
	
}



void keyboard_func (unsigned char key, int x, int y)
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
      case 'y':
         y_angle = (y_angle + 5) % 360;
         glutPostRedisplay();
         break;
      case 'Y':
         y_angle = (y_angle - 5) % 360;
         glutPostRedisplay();
         break;
      case 'r':
         camera_x = 0.0; camera_y = 0.0; camera_z = -3.0;
         z_angle = 0, y_angle=0, x_angle=0, is_pressed =0;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}


void mouse_func(int button, int state, int x, int y) 
{
   
  	
   
      if(button== GLUT_LEFT_BUTTON){
         if (state == GLUT_DOWN) {
            mouse_x1=x;mouse_y1=y;
            is_pressed = 1;
         }else if(is_pressed==1){
         	y_angle = (y_angle + (x - mouse_x1)/20)%360;
         	x_angle = (x_angle + (y - mouse_y1)/20)%360;
         	is_pressed= 0;
         	
         	glutPostRedisplay();
         
         }
      }
      if(button== GLUT_RIGHT_BUTTON){
      	
         if (state == GLUT_DOWN){
         	mouse_x1=y;
            	is_pressed = 1;
         }else if(is_pressed==1){
         	z_angle = (z_angle - (x - mouse_x1)/20)%360;
         	is_pressed= 0;
         	
         	glutPostRedisplay();
         
         }
            
      }
      if(button == 3){
      	camera_z =  camera_z+0.1;
      	glutPostRedisplay();
      }
      if(button == 4){
      	camera_z =  camera_z-0.1;
      	glutPostRedisplay();
      }
}



void specialKey_func(int key, int x, int y){
	if(key == GLUT_KEY_UP){
		camera_y += 0.1;
		
		glutPostRedisplay();
	} 
   
	if(key == GLUT_KEY_DOWN) {
	   	camera_y -= 0.1;
		glutPostRedisplay();
	}

	if(key == GLUT_KEY_LEFT){
		camera_x -= 0.1;
		glutPostRedisplay();
	}
	if(key == GLUT_KEY_RIGHT){
		camera_x += 0.1;
		glutPostRedisplay();
	}
	  


}


int main(int argc, char** argv){
	string file = "lowpolybunny.obj";
	glutInit(&argc, argv);
	
  	 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  	 glutInitWindowSize(1500, 1500);
  	 glutInitWindowPosition(100, 100);
  	 glutCreateWindow("170101064 Assignment4 Rotate by mouse ");
  	 readMesh(file);  	 
  	 init();
   	glutDisplayFunc(displayMesh);
   	glutReshapeFunc(change_shape);
   	glutMouseFunc(mouse_func);
   	glutKeyboardFunc(keyboard_func);
   	glutSpecialFunc(specialKey_func);
  	
  	
	glutMainLoop();
}
