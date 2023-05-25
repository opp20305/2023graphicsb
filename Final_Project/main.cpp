#include <stdio.h>
#include <GL/glut.h>
#include "glm.h"
GLMmodel * head = NULL;
GLMmodel * body = NULL;
GLMmodel * uparmR = NULL;
GLMmodel * lowerarmR = NULL;
GLMmodel * armR = NULL;
int show[5] = {1,1,0,0,0};
int ID=4;
float teapotX=0,teapotY=0;
float angle[20]={};
FILE * fout = NULL;
FILE * fin = NULL;
void keyboard(unsigned char key, int x,int y)
{
    if(key=='0')show[0]=!show[0];
    if(key=='1')show[1]=!show[1];
    if(key=='2')show[2]=!show[2];
    if(key=='3')show[3]=!show[3];
    if(key=='4')show[4]=!show[4];
    glutPostRedisplay();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    if(head==NULL)
    {
        head = glmReadOBJ("model/head.obj");
        body = glmReadOBJ("model/body.obj");
        uparmR = glmReadOBJ("model/uparmR.obj");
        lowerarmR = glmReadOBJ("model/lowarmR.obj");
        armR = glmReadOBJ("model/armR.obj");
    }
    glScalef(0.3,0.3,0.3);
    glPushMatrix();
        glTranslatef(teapotX,teapotY,0);
        if(show[0]) glmDraw(body,GLM_MATERIAL);
    glPopMatrix();
        if(show[1])glmDraw(head,GLM_MATERIAL);
        glPushMatrix();
        glTranslatef(-1.360000,+0.500000,0);
        glRotatef(angle[2],0,0,1);
        glTranslatef(1.360000,-0.500000,0);
        if(ID==2)glColor3f(1,0,0);
        else glColor3f(1,1,1);
        if(show[2])glmDraw(uparmR,GLM_MATERIAL);
        glPushMatrix();
        glTranslatef(-1.959999,+0.080000,0);
        glRotatef(angle[2],0,0,1);
        glTranslatef(1.959999,-0.080000,0);
        if(ID==3)glColor3f(1,0,0);
        else glColor3f(1,1,1);
        if(show[3])glmDraw(lowerarmR,GLM_MATERIAL);
        if(show[4])glmDraw(armR,GLM_MATERIAL);
        glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}
int oldX=0,oldY=0;
void mouse(int button,int state,int x,int y)
{
    if(state==GLUT_DOWN)
    {
        oldX=x;
        oldY=y;
    }
}
void motion(int x,int y)
{
    teapotX+=(x-oldX)/150.0*3;
    teapotY-=(y-oldY)/150.0*3;
    printf("glTranslatef(%f,%f,0)\n",teapotX,teapotY);
    angle[ID]+=x-oldX;
    oldX=x;
    oldY=y;
    glutPostRedisplay();
}
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
    glutCreateWindow("week13");
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
}
