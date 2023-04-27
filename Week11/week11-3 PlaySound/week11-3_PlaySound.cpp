#include<windows.h>
#include<stdio.h>
int main()
{
    PlaySound("do.wav",NULL,SND_SYNC);
    PlaySound("re.wav",NULL,SND_SYNC);
    PlaySound("mi.wav",NULL,SND_SYNC);
    PlaySound("fa.wav",NULL,SND_SYNC);
    PlaySound("so.wav",NULL,SND_SYNC);
    printf("Hello World");
}
