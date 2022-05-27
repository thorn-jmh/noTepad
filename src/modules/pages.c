#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "cursor.h"
#include "pages.h"


static PAGE_T thePage; //定义静态页面布局变量

static double toph = 0.24;   //顶部高度
static double sliperw = 0.1; //滑块宽度
static double margin = 0.1;  //页边留白

//返回页面布局指针
PAGE_T *GetPageInfo()
{
    PAGE_T *pp = &thePage;
    return pp;
};

// 更新页面位置信息
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
};

// from LT
// 设置 noti 区域
void SetNotiArea(double x, double y, double w, double h)
{
    thePage.NOTI.LT.X = x;
    thePage.NOTI.LT.Y = y;
    thePage.NOTI.RB.X = x + w;
    thePage.NOTI.RB.Y = y - h;
}

// 重置 noti 区域
void ResetNotiArea()
{
    thePage.NOTI.LT.X = -1;
    thePage.NOTI.LT.Y = -1;
    thePage.NOTI.RB.X = -1;
    thePage.NOTI.RB.Y = -1;
}

// 判断是否在 topbar 区域
bool InTopArea(double x, double y)
{
    if (x >= thePage.TOPBAR.LT.X && x <= thePage.TOPBAR.RB.X && y <= thePage.TOPBAR.LT.Y && y >= thePage.TOPBAR.RB.Y)
        return TRUE;
    else
        return FALSE;
}

// 判断是否在 noti 区域
bool InNotiArea(double x, double y)
{
    if (thePage.NOTI.LT.X < 0)
        return FALSE;
    if (x >= thePage.NOTI.LT.X && x <= thePage.NOTI.RB.X && y <= thePage.NOTI.LT.Y && y >= thePage.NOTI.RB.Y)
        return TRUE;
    else
        return FALSE;
}
