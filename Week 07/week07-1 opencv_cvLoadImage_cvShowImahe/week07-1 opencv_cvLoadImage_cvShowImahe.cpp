#include<opencv/highgui.h>
int main()
{
    IplImage * img = cvLoadImage("MC.jpg");
    cvShowImage("week07",img);
    cvWaitKey(0);
}
