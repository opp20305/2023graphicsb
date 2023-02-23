#include <GL/glut.h>
#include <math.h>
void dp(float b,float x,float y)
{
    glBegin(GL_POLYGON);
    for(float a=0; a<=2*3.141592653589793238462632383279;a+=0.01)
    {
        glVertex2f(b*cos(a)+x,b*sin(a)+y);
    }
    glEnd();
}
void display()
{
        glColor3f(11/255.0,237/255.0,255/255.0);
        dp(0.5,0,0);
        glColor3f(1,0,0);
        dp(0.3,0.5,0.5);
        glColor3f(0,1,0);
        dp(0.3,-0.5,0.5);
        glColor3f(0,0,1);
        dp(0.3,-0.5,-0.5);
        glColor3f(0,1,1);
        dp(0.3,0.5,-0.5);
        glutSwapBuffers();
}
int main(int argc,char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("week02");
    glutDisplayFunc(display);
    glutMainLoop();
}
