
#include<opencv/highgui.h>
int main()
{
    IplImage * img = cvLoadImage(".jpg");
    scShowImahe("week07",img);
    svWaitKey(0);
}
