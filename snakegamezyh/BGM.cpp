#include "BGM.h"
//new
#include<stdio.h>
#include<graphics.h>
#include<Windows.h>
#include<conio.h>
#include<MMSystem.h>
#pragma comment(lib,"Winmm.lib")
#include<iostream>
#include<unistd.h>


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

void display_cover()
{
    initgraph(800, 600);
	loadimage(NULL, "snack_cover.jpg", 800, 600);
	setbkmode(TRANSPARENT);
	settextstyle(30, 20,"Times New Roman");
	outtextxy(280, 400, "Loading...");
	settextcolor(RGB(50, 128, 255));
	sleep(2);
    closegraph();
}

void start_game()
{
    display_cover();
    play_BGM();
}
