#include <windows.h>
#include <opencv/highgui.h> ///使用 OpenCV 2.1 比較簡單, 只要用 High GUI 即可
#include <opencv/cv.h>
#include <stdio.h>
#include <GL/glut.h>
#include "glm.h"
#include "CMP3_MCI.h"
CMP3_MCI myMP3;
GLMmodel * head = NULL,* headear = NULL,* headblack = NULL,* headred = NULL,* headmer = NULL,* eyeup = NULL,* eyewhite = NULL;
GLMmodel * body = NULL,* bot = NULL,* midside = NULL,* bodymid = NULL; ///GLMmodel * gundam = NULL;
GLMmodel * arm1 = NULL, * arm2 = NULL, * arm11 = NULL, * arm22 = NULL;
GLMmodel * arm3 = NULL, * arm4 = NULL;
GLMmodel * hand1 = NULL, * hand2 = NULL;
GLMmodel * leg1 = NULL, * leg2 = NULL;
GLMmodel * knee1 = NULL, * knee2 = NULL;
GLMmodel * foot1 = NULL, * foot2 = NULL;
GLuint text1,text2,text3,text4,text5,text6,text7,text8,text9,text10;

float teapotX = 0, teapotY = 0, oldX = 0, oldY = 0;
float angle[20] = {}, angle2[20] = {};///float angle = 0, angle2 = 0;
float NewAngle[20] = {}, NewAngle2[20] = {};
float OldAngle[20] = {}, OldAngle2[20] = {};
int ID = 0;
FILE * fout = NULL;
FILE * fin = NULL;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, -5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void timer(int t) {
    printf("現在timer(%d)\n", t);
    glutTimerFunc(20, timer, t+1); ///馬上設定下一個鬧鐘

    float alpha = (t%50) / 50.0; ///0.0 ~ 1.0

    if(t%50==0){
        if(fin == NULL) fin = fopen("motion.txt", "r");
        for(int i=0; i<20; i++){
            OldAngle[i] = NewAngle[i];
            OldAngle2[i] = NewAngle2[i];
            fscanf(fin, "%f", &NewAngle[i] );
            fscanf(fin, "%f", &NewAngle2[i] );
        }
    }
    for(int i=0; i<20; i++){
        angle[i] = NewAngle[i] * alpha + OldAngle[i] * (1-alpha);
        angle2[i] = NewAngle2[i] * alpha + OldAngle2[i] * (1-alpha);
    }

    glutPostRedisplay();
}
void myLight()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void keyboard(unsigned char key, int x, int y) {
    if(key=='0') ID = 0;
    if(key=='1') ID = 1;
    if(key=='2') ID = 2;
    if(key=='3') ID = 3;
    if(key=='4') ID = 4;
    if(key=='5') ID = 5;
    if(key=='6') ID = 6;
    if(key=='7') ID = 7;
    if(key=='8') ID = 8;
    if(key=='9') ID = 9;
    if(key=='-') ID = 10;
    if(key=='=') ID = 11;
    if(key=='[') ID = 12;
    if(key==']') ID = 13;
    if(key=='s'){ ///save存檔 也會動到檔案
        if(fout == NULL) fout = fopen("motion.txt", "w");
        for(int i=0; i<20; i++){
            fprintf(fout, "%.2f ", angle[i] );
            fprintf(fout, "%.2f ", angle2[i] );
        }
        fprintf(fout, "\n");
        printf("寫了一行\n");
    }
    if(key=='r'){ ///read讀檔 也會動到檔案
        if(fin == NULL) fin = fopen("motion.txt", "r");
        for(int i=0; i<20; i++){
            fscanf(fin, "%f", &angle[i] );
            fscanf(fin, "%f", &angle2[i] );
        }
        glutPostRedisplay();
    }
    if(key=='t'){ ///play播放 也會動到檔案
        glutTimerFunc(0, timer, 0);
        myMP3.Play();
    }
}

int myTexture(char * filename)
{
    IplImage * img = cvLoadImage(filename); ///OpenCV讀圖
    cvCvtColor(img,img, CV_BGR2RGB); ///OpenCV轉色彩 (需要cv.h)
    glEnable(GL_TEXTURE_2D); ///1. 開啟貼圖功能
    GLuint id; ///準備一個 unsigned int 整數, 叫 貼圖ID
    glGenTextures(1, &id); /// 產生Generate 貼圖ID
    glBindTexture(GL_TEXTURE_2D, id); ///綁定bind 貼圖ID
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖T, 就重覆貼圖
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖S, 就重覆貼圖
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /// 貼圖參數, 放大時的內插, 用最近點
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /// 貼圖參數, 縮小時的內插, 用最近點
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->imageData);
    return id;
}
void mouse(int button, int state, int x, int y) {
    oldX = x;
    oldY = y;
}
void motion(int x, int y) {
    teapotX += (x - oldX) / 1000.0 * 10; ///teapotX = (x-150)/150.0;
    teapotY += (oldY - y) / 1000.0 * 10; ///teapotY = (150-y)/150.0;
    angle[ID] += x - oldX;
    angle2[ID] += oldY - y;
    oldX = x;
    oldY = y;
    glutPostRedisplay();
    printf("  glTranslatef( %.2f, %.2f, 0 ); \n", teapotX, teapotY );
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    {
    glBindTexture(GL_TEXTURE_2D, text3);
    glTranslatef( 0, 0, 0.4 );
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, 1.0); // 左下角
    glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, -1.0); // 右下角
    glTexCoord2f(1.0, 1.0); glVertex2f(1.0, -1.0); // 右上角
    glTexCoord2f(1.0, 0.0); glVertex2f(1.0, 1.0); // 左上角
    glEnd();
    }


glEnable(GL_LIGHTING);
   glPopMatrix();
    glPushMatrix();
        glScalef(1.6, 1.6, 1.6);
        glTranslatef(0, -0.5,-0.2);///往下一半
        glBindTexture(GL_TEXTURE_2D , text6);
        glPushMatrix();
            glColor3f(1,1,1);
            glScalef(0.4, 0.4, 0.4);
            glRotatef(angle[0], 0, 1, 0); ///身體的轉動
            glmDraw(body, GLM_SMOOTH|GLM_TEXTURE);///glmDraw(gundam, GLM_MATERIAL|GLM_TEXTURE);



            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text4);
                glmDraw(bodymid, GLM_SMOOTH|GLM_TEXTURE);
            glPopMatrix();


            glPushMatrix();///左手
                glBindTexture(GL_TEXTURE_2D, text5);
                glTranslatef(+0.26, +1.36, 0 );
                glRotatef(angle[1], 0, 1, 0);
                glRotatef(angle2[1], 1, 0, 0);
                glTranslatef(-0.26, -1.36, 0 );///glTranslatef(teapotX, teapotY, 0);
                glmDraw(arm1, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();///左手
                glBindTexture(GL_TEXTURE_2D, text4);
                glmDraw(arm11, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text5);
                glTranslatef(+0.62, +1.10, 0 );
                glRotatef(angle[2], 0, 0, 1);
                glRotatef(angle2[2], 1, 0, 0);
                glTranslatef(-0.62, -1.10, 0 );
                glmDraw(arm3, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, text4);
                    glTranslatef(+0.68, +0.82, 0 );
                    glRotatef(angle[3], 0, 1, 0);
                    glRotatef(angle2[3], 1, 0, 0);
                    glTranslatef( -0.68, -0.82, 0 );///glTranslatef(teapotX, teapotY, 0);
                    glmDraw(hand1, GLM_SMOOTH|GLM_TEXTURE);
                glPopMatrix();
                glPopMatrix();
                glPopMatrix();
            glPopMatrix();




            glPushMatrix();///右手
                glBindTexture(GL_TEXTURE_2D, text5);
                glTranslatef(-0.24, +1.40, 0 );
                glRotatef(angle[4], 0, 1, 0);
                glRotatef(angle2[4], 1, 0, 0);
                glTranslatef(+0.24, -1.40, 0 );///glTranslatef(teapotX, teapotY, 0);
                glmDraw(arm2, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();///
                glBindTexture(GL_TEXTURE_2D, text4);
                glmDraw(arm22, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text5);
                glTranslatef(-0.56, +1.20, 0 );
                glRotatef(angle[5], 0, 0, 1);
                glRotatef(angle2[5], 1, 0, 0);
                glTranslatef(+0.56, -1.20, 0 );
                glmDraw(arm4, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, text4);
                    glTranslatef(-0.66, +0.88, 0 );
                    glRotatef(angle[6], 0, 1, 0);
                    glRotatef(angle2[6], 1, 0, 0);
                    glTranslatef(+0.66, -0.88, 0 );
                    glmDraw(hand2, GLM_SMOOTH|GLM_TEXTURE);
                    glPopMatrix();
                glPopMatrix();
                glPopMatrix();
            glPopMatrix();






            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text7);
                glTranslatef(-0.00, +1.35, 0 );
                glRotatef(angle[7], 0, 1, 0);
                glRotatef(angle2[7], 1, 0, 0);
                glTranslatef( 0.00, -1.35, 0 );///glTranslatef(teapotX, teapotY, 0);
                glmDraw( head, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text4);
                glmDraw( headmer, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text8);
                glmDraw( headear, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text6);
                glmDraw( headblack, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text5);
                glmDraw( headred, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text9);
                glmDraw( eyeup, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text10);
                glmDraw( eyewhite, GLM_SMOOTH|GLM_TEXTURE);

                glPopMatrix();

            glPopMatrix();














            glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text5);
                glmDraw(bot, GLM_SMOOTH|GLM_TEXTURE);

                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text4);
                    glTranslatef(+0.14, +0.80, 0 );
                    glRotatef(angle[8], 0, 1, 0);
                    glRotatef(angle2[8], 1, 0, 0);
                    glTranslatef(-0.14, -0.80, 0 );///glTranslatef(teapotX, teapotY, 0);
                    glmDraw(leg1, GLM_SMOOTH|GLM_TEXTURE);

                    glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, text5);
                        glTranslatef(+0.26, +0.56, 0 );
                        glRotatef(angle[9], 0, 1, 0);
                        glRotatef(angle2[9], 1, 0, 0);
                        glTranslatef( -0.26, -0.56, 0 );///glTranslatef(teapotX, teapotY, 0);
                        glmDraw(knee1, GLM_SMOOTH|GLM_TEXTURE);

                        glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, text5);
                            glTranslatef(+0.38, +0.22, 0 );
                            glRotatef(angle[10], 0, 1, 0);
                            glRotatef(angle2[10], 1, 0, 0);
                            glTranslatef(-0.38, -0.22, 0 );///glTranslatef(teapotX, teapotY, 0);
                            glmDraw(foot1, GLM_SMOOTH|GLM_TEXTURE);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();


                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, text4);
                    glTranslatef(-0.14, +0.82, 0 );
                    glRotatef(angle[11], 0, 1, 0);
                    glRotatef(angle2[11], 1, 0, 0);
                    glTranslatef(+0.14, -0.82, 0 );///glTranslatef(teapotX, teapotY, 0);
                    glmDraw(leg2, GLM_SMOOTH|GLM_TEXTURE);

                    glPushMatrix();
                    glBindTexture(GL_TEXTURE_2D, text5);
                        glTranslatef(-0.26, +0.56, 0 );
                        glRotatef(angle[12], 0, 1, 0);
                        glRotatef(angle2[12], 1, 0, 0);
                        glTranslatef(+0.26, -0.56, 0 );///glTranslatef(teapotX, teapotY, 0);
                        glmDraw(knee2, GLM_SMOOTH|GLM_TEXTURE);

                        glPushMatrix();
                        glBindTexture(GL_TEXTURE_2D, text5);
                            glTranslatef(-0.38, +0.24, 0 );
                            glRotatef(angle[13], 0, 1, 0);
                            glRotatef(angle2[13], 1, 0, 0);
                            glTranslatef(+0.38, -0.24, 0 );///glTranslatef(teapotX, teapotY, 0);
                            glmDraw(foot2, GLM_SMOOTH|GLM_TEXTURE);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
            glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    myMP3.Load("bgmver1.mp3");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(768,768);
    glutCreateWindow("week17");
    myLight();
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    headblack = glmReadOBJ("model/headblack.obj");
    headmer = glmReadOBJ("model/headmer.obj");
    headred = glmReadOBJ("model/headred.obj");
    headear = glmReadOBJ("model/headear.obj");
    eyeup = glmReadOBJ("model/eyeup.obj");
    eyewhite = glmReadOBJ("model/eyewhite.obj");
    head = glmReadOBJ("model/head.obj");
    body = glmReadOBJ("model/body.obj");
    bodymid = glmReadOBJ("model/bodymid.obj");
    midside = glmReadOBJ("model/midside.obj");
    bot = glmReadOBJ("model/bot.obj");
    arm1 = glmReadOBJ("model/uparmL.obj");
    arm11 = glmReadOBJ("model/uparmLL.obj");
    arm2 = glmReadOBJ("model/uparmR.obj");
    arm22 = glmReadOBJ("model/uparmRR.obj");
    arm3 = glmReadOBJ("model/lowarmL.obj");
    arm4 = glmReadOBJ("model/lowarmR.obj");
    hand1 = glmReadOBJ("model/handL.obj");
    hand2 = glmReadOBJ("model/handR.obj");
    leg1 = glmReadOBJ("model/uplegL.obj");
    leg2 = glmReadOBJ("model/uplegR.obj");
    knee1 = glmReadOBJ("model/lowlegL.obj");
    knee2 = glmReadOBJ("model/lowlegR.obj");
    foot1 = glmReadOBJ("model/footL.obj");
    foot2 = glmReadOBJ("model/footR.obj");

    text10 = myTexture("model/white.png");
    text9 = myTexture("model/eyeup.png");
    text8 = myTexture("model/ear.png");
    text7 = myTexture("model/headback.png");
    text6 = myTexture("model/black.png");
    text5 = myTexture("model/lowarm.png");
    text4 = myTexture("model/hand.png");
    text3 = myTexture("model/image/055.png");
    text2 = myTexture("model/Color.png");
    text1 = myTexture("model/blue.jpg");

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
}
