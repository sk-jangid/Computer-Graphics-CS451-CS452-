#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
GLfloat plane[4];
GLfloat v0[3], v1[3], v2[3];
 
GLuint tex1;


void create_tex(void)
{   
    unsigned char temp[256][256][3];
    for (int j = 0; j < 255; j++) {
	for (int i = 0; i < 255; i++) {
	    unsigned char *t = temp[j][i];
	    t[0] = t[1] = t[2] = (i ^ j) & 8 ? 255 : 0;
	}
    }
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) temp);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



GLfloat *create_texture(int maxj, int maxi)
{
    static GLfloat *texture;
    texture = (GLfloat *)malloc(maxi * maxj * sizeof(GLfloat));
    for(int j = 0; j < maxj; j++) {
	for(int i = 0; i < maxi; i++) {
	    texture[i + maxi * j] = ((i >> 4) & 0x1) ^ ((j >> 4) & 0x1);
	}
    }
    return texture;
}
enum {SPHERE = 1, LIGHT, SPHERE_PLASTIC, LEFTWALL, FLOOR, TEX_SPHERE};





void shadowmatrix(GLfloat shadowMat[4][4], GLfloat plane_normal[4], GLfloat light_pos[4]) {
  GLfloat dot;
  /* find dot product between light position vector and ground plane normal */
  dot = plane_normal[0] * light_pos[0] +
        plane_normal[1] * light_pos[1] +
        plane_normal[2] * light_pos[2] +
        plane_normal[3] * light_pos[3];
  shadowMat[0][0] = dot - light_pos[0] * plane_normal[0];
  shadowMat[1][0] = 0.f - light_pos[0] * plane_normal[1];
  shadowMat[2][0] = 0.f - light_pos[0] * plane_normal[2];
  shadowMat[3][0] = 0.f - light_pos[0] * plane_normal[3];
  shadowMat[0][1] = 0.f - light_pos[1] * plane_normal[0];
  shadowMat[1][1] = dot - light_pos[1] * plane_normal[1];
  shadowMat[2][1] = 0.f - light_pos[1] * plane_normal[2];
  shadowMat[3][1] = 0.f - light_pos[1] * plane_normal[3];
  shadowMat[0][2] = 0.f - light_pos[2] * plane_normal[0];
  shadowMat[1][2] = 0.f - light_pos[2] * plane_normal[1];
  shadowMat[2][2] = dot - light_pos[2] * plane_normal[2];
  shadowMat[3][2] = 0.f - light_pos[2] * plane_normal[3];
  shadowMat[0][3] = 0.f - light_pos[3] * plane_normal[0];
  shadowMat[1][3] = 0.f - light_pos[3] * plane_normal[1];
  shadowMat[2][3] = 0.f - light_pos[3] * plane_normal[2];
  shadowMat[3][3] = dot - light_pos[3] * plane_normal[3];
}
/* find the plane equation given 3 points */
void get_plane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]){
  GLfloat temp1[3], temp2[3];

  temp1[0] = v1[0] - v0[0];
  temp1[1] = v1[1] - v0[1];
  temp1[2] = v1[2] - v0[2];
  temp2[0] = v2[0] - v0[0];
  temp2[1] = v2[1] - v0[1];
  temp2[2] = v2[2] - v0[2];
  

  plane[0] = temp1[1] * temp2[2] - temp1[2] * temp1[1];
  plane[1] = -(temp1[0] * temp2[2] - temp1[2] * temp2[0]);
  plane[2] = temp1[0] * temp2[1] - temp1[1] * temp2[0];
  plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}


void sphere(int sphere_id){
	glPushMatrix();

	switch(sphere_id){
		case(0): glTranslatef(50.0f, -50.0f, -400.f);glCallList(SPHERE);break;
		case(1): glEnable(GL_TEXTURE_2D); create_tex(); glBindTexture(GL_TEXTURE_2D, tex1); glTranslatef(-70.0f, -60.f, -400.0f); glCallList(TEX_SPHERE); glDisable(GL_TEXTURE_2D);break;
		case(2): glTranslatef(-10.0f, -70.f, -400.f); glCallList(SPHERE_PLASTIC);
	}
	glPopMatrix();

}




GLfloat left_rect_shadow[4][4];
GLfloat floor_shadow[4][4];
GLfloat back_rect_shadow[4][4];
GLfloat lightx = 100;
GLfloat light_pos[] = {0.0f, 100.f, -400.f, 1.f};


void reshape(void){

	static GLfloat rect_mat[] = {1.f, 1.f, 1.f, 1.f};
	static GLfloat sphere_mat[] = {1.f, 2.f, 0.f, 1.f};
	static GLfloat cone_mat[] = {0.f, .5f, 1.f, 1.f};
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

 
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rect_mat);
 
	glEnable(GL_TEXTURE_2D);
	    
	glBegin(GL_QUADS);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2i(0, 0);
	glVertex3f(-100.f, -100.f, -320.f);
	glTexCoord2i(1, 0);
	glVertex3f( 100.f, -100.f, -320.f);
	glTexCoord2i(1, 1);
	glVertex3f( 100.f, -100.f, -520.f);
	glTexCoord2i(0, 1);
	glVertex3f(-100.f, -100.f, -520.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
 
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glColor3f(0.f, 0.f, 0.f); 
      glPushMatrix();
      glMultMatrixf((GLfloat *)floor_shadow);
      //glMultMatrixf((GLfloat *)left_rect_shadow);

      sphere(1);
      glPopMatrix();
      glPushMatrix();
      glMultMatrixf((GLfloat *)floor_shadow);
      //glMultMatrixf((GLfloat *)left_rect_shadow);
      sphere(0);
      glPopMatrix();
      glPushMatrix();
      glMultMatrixf((GLfloat *)floor_shadow);
      //
      sphere(2);
      glPopMatrix();
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);

      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 1, 0);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glBegin(GL_QUADS);
    /* left wall */
    glNormal3f(1.f, 0.f, 0.f);
    glVertex3f(-100.f, -100.f, -320.f);
    glVertex3f(-100.f, -100.f, -520.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glVertex3f(-100.f,  100.f, -320.f);
    glEnd();
      glStencilFunc(GL_EQUAL, 1, 1);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glColor3f(0.f, 0.f, 0.f); 
      glDisable(GL_DEPTH_TEST);
      glPushMatrix();

      glMultMatrixf((GLfloat *)left_rect_shadow);
      sphere(1);
      glPopMatrix();
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);


    glBegin(GL_QUADS);

    glNormal3f(-1.f, 0.f, 0.f);
    glVertex3f( 100.f, -100.f, -320.f);
    glVertex3f( 100.f,  100.f, -320.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f( 100.f, -100.f, -520.f);

    glNormal3f(0.f, -1.f, 0.f);
    glVertex3f(-100.f,  100.f, -320.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f( 100.f,  100.f, -320.f);

    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(-100.f, -100.f, -520.f);
    glVertex3f( 100.f, -100.f, -520.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glEnd();

    
    glPushMatrix();
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glDisable(GL_LIGHTING);
    glColor3f(1.f, 1.f, 1.f);
    glCallList(LIGHT);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
    sphere(0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cone_mat);
    sphere(1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cone_mat);
    sphere(2);
    
    
    glutSwapBuffers(); 
}


void keyboard_func(unsigned char key, int x, int y)
{
    if(key == '0')
	exit(0);
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glFrustum(-100., 100., -100., 100., 320., 640.); 
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

const int TEXDIM = 256;

int main(int argc, char *argv[])
{
    GLfloat *tex;
    GLUquadricObj *sphere;
    GLfloat plane[4];
    GLfloat point1[3], point2[3], point3[3];
    
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL|GLUT_DOUBLE);
    
    
    (void)glutCreateWindow("170101064 Assignment3: Scene");
    
    
    glutDisplayFunc(reshape);
    glutKeyboardFunc(keyboard_func);
    init();

    /* make shadow matricies */
      point1[0] = -100.f; point1[1] = -100.f; point1[2] = -320.f;
      point2[0] =  100.f; point2[1] = -100.f; point1[2] = -320.f;
      point3[0] =  100.f; point3[1] = -100.f; point1[2] = -520.f;
      
      get_plane(plane, point1, point2, point3);
      shadowmatrix(floor_shadow, plane, light_pos);

      point1[0] = -100.f; point1[1] = -100.f; point1[2] = -320.f;
      point2[0] = -100.f; point2[1] = -190.f; point2[2] = -520.f;
      point3[0] = -100.f; point3[1] =  100.f; point3[2] = -520.f;
      get_plane(plane, point1, point2, point3);
      shadowmatrix(left_rect_shadow, plane, light_pos);

      point1[0] = -100.f; point1[1] = -100.f; point1[2] = -520.f;
      point2[0] =  100.f; point2[1] = -100.f; point2[2] = -520.f;
      point3[0] =  100.f; point3[1] = 100.f; point3[2] = -520.f;
      
      get_plane(plane, point1, point2, point3);
      shadowmatrix(back_rect_shadow, plane, light_pos);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glCullFace(GL_BACK);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    /* make display lists for sphere for efficiency */
    glNewList(SPHERE, GL_COMPILE);
    sphere = gluNewQuadric();
    gluSphere(sphere, 25.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();
    
    glNewList(LIGHT, GL_COMPILE);
    sphere = gluNewQuadric();
    gluSphere(sphere, 25.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();
    
    glNewList(TEX_SPHERE, GL_COMPILE);
    sphere = gluNewQuadric();
    create_tex();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 25.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();

    glNewList(SPHERE_PLASTIC, GL_COMPILE);
    sphere = gluNewQuadric();
    gluSphere(sphere, 25.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();
    
    glNewList(FLOOR, GL_COMPILE);
    glEndList();
    
    glNewList(LEFTWALL, GL_COMPILE);
    glEndList();

    tex = create_texture(TEXDIM, TEXDIM);
    glTexImage2D(GL_TEXTURE_2D, 0, 1, TEXDIM, TEXDIM, 0, GL_RED, GL_FLOAT, tex);
    free(tex);
    glutMainLoop();

    return 0;
}
