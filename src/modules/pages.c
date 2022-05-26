#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "pages.h"

#define Epsilon 0.001

static PAGE_T thePage; //定义静态页面布局变量

static double toph = 0.24;   //顶部高度
static double sliperw = 0.1; //滑块宽度
static double margin = 0.1;  //页边留白
static double notih = 2;
static double notiw = 1.5;
static double notinum = 5;

PAGE_T *GetPageInfo()
{ //返回页面布局指针
    PAGE_T *pp = &thePage;
    return pp;
};

void UpdatePageInfo()
{
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();

    thePage.PAGE.LT.X = 0;
    thePage.PAGE.LT.Y = windowh;
    thePage.PAGE.RB.X = windoww;
    thePage.PAGE.RB.Y = 0;

    thePage.SLIPER.LT.X = windoww - sliperw;
    thePage.SLIPER.LT.Y = windowh - toph;
    thePage.SLIPER.RB.X = windoww;
    thePage.SLIPER.RB.Y = 0;

    thePage.TOPBAR.LT.X = 0;
    thePage.TOPBAR.LT.Y = windowh;
    thePage.TOPBAR.RB.X = windoww;
    thePage.TOPBAR.RB.Y = windowh - toph;

    thePage.TEXT.LT.X = margin + toph;
    thePage.TEXT.LT.Y = windowh - toph;
    thePage.TEXT.RB.X = windoww - margin;
    thePage.TEXT.RB.Y = 0;

    thePage.LEFTBAR.LT.X = 0;
    thePage.LEFTBAR.LT.Y = thePage.TOPBAR.RB.Y - toph / 20;
    thePage.LEFTBAR.RB.X = toph;
    thePage.LEFTBAR.RB.Y = 0;
}; //这个函数会获取当前的窗口大小，对文本区进行调整

// from LT
void SetNotiArea(double x, double y, double w, double h)
{
    thePage.NOTI.LT.X = x;
    thePage.NOTI.LT.Y = y;
    thePage.NOTI.RB.X = x + w;
    thePage.NOTI.RB.Y = y - h;
}

void ResetNotiArea()
{
    thePage.NOTI.LT.X = -1;
    thePage.NOTI.LT.Y = -1;
    thePage.NOTI.RB.X = -1;
    thePage.NOTI.RB.Y = -1;
}

bool InNotiArea(double x, double y)
{
    if (thePage.NOTI.LT.X < 0)
        return FALSE;
    if (x >= thePage.NOTI.LT.X && x <= thePage.NOTI.RB.X && y <= thePage.NOTI.LT.Y && y >= thePage.NOTI.RB.Y)
        return TRUE;
    else
        return FALSE;
}

///////// OLD ////////////////

// void Noti(double mx, double my)
//{ //会按照传入的鼠标画一个长方体框框
//     if (mx <= margin || mx >= thePage.TEXT.RB.X || my >= thePage.TEXT.RB.Y)
//         return;
//     DefineColor("gray", 0.8, 0.8, 0.8);
//     SetPenColor("gray");
//     int LorR, UporD;
//     double inch = GetNotiInch();
//     double windowh = GetWindowHeight();
//     double windoww = GetWindowWidth();
//     if (mx <= windoww / 2)
//         LorR = 1;
//     else
//         LorR = -1;
//     if (my <= windowh / 2)
//         UporD = 1;
//     else
//         UporD = -1;
//     drawRect(mx, my, LorR * notiw, UporD * notih);
//     int i;
//     for (i = 1; i <= notinum; i++)
//     {
//         MovePen(mx, my + i * UporD * inch);
//         DrawLine(notiw * LorR, 0);
//     }
//
//     thePage.NOTI.LT.X = mx < mx + LorR * notiw ? mx : mx + LorR * notiw;
//     thePage.NOTI.LT.Y = my < my + UporD * notih ? my : my + UporD * notih;
//     thePage.NOTI.RB.X = mx > mx + LorR * notiw ? mx : mx + LorR * notiw;
//     thePage.NOTI.RB.Y = my > my + UporD * notih ? my : my + UporD * notih;
// }
//
// double GetNotiInch()
//{
//     return notih / notinum;
// }

/*void testText(void)
{
    drawRect(thePage.TEXT.LT.X, thePage.TEXT.LT.Y,
             thePage.TEXT.RB.X - thePage.TEXT.LT.X,
             thePage.TEXT.RB.Y - thePage.TEXT.LT.Y);
}

void testTop(void)
{
    SetPenColor("red");
    drawRect(thePage.TOPBUTTON.LT.X, thePage.TOPBUTTON.LT.Y,
             thePage.TOPBUTTON.RB.X - thePage.TOPBUTTON.LT.X,
             thePage.TOPBUTTON.RB.Y - thePage.TOPBUTTON.LT.Y);
}

void testNoti(void){
    SetPenColor("yellow");
    drawRect(thePage.NOTI.LT.X, thePage.NOTI.LT.Y,
             thePage.NOTI.RB.X - thePage.NOTI.LT.X,
             thePage.NOTI.RB.Y - thePage.NOTI.LT.Y);
}*/
