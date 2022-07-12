#include "BGM.h"
//new
#include<stdio.h>
//#include<graphic.h>
#include<Windows.h>
#include<conio.h>
#include<mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include<iostream>

/*void display_cover()
{
    initgraph(800,600);
    LoadImage(NULL,"snack_cover.jpg",800,600);
    SetBkMode(TRANSPARENT);
}*/


void play_BGM()
{
    mciSendString("open BGM.mp3",NULL,0,NULL);
    mciSendString("play BGM.mp3 repeat",NULL,0,NULL);
    system("pause");
    char c;
    std::cout<<"输入Z或z以关闭背景音乐,输入其他键继续"<<std::endl;
    std::cin>>c;
    if (c=='Z'||c=='z')
    {
        mciSendString("close BGM.mp3",NULL,0,NULL);
    }
}
