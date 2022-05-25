#ifndef _pages_h
#define _pages_h

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"
#include "genlib.h"
#include "printer.h"

typedef struct
{
    double X, Y;
} X_Y;

typedef struct
{
    X_Y LT, RB; //左上右下两个参数
} AREA;

typedef struct
{
    AREA PAGE, TEXT, TOPBAR, SLIPER, NOTI, LEFTBAR;
} PAGE_T;

//静态全局变量接口部分
PAGE_T *GetPageInfo(void); //获取页面布局变量指针

//整体page信息部分  注：上端的功能条的高度和右侧的滑条的宽度是固定不变的,具体大小可以到pages.c调整

void UpdatePageInfo(); //这个函数会获取当前的窗口大小，对文本区进行调整

void SetNotiArea(double x, double y, double w, double h);

void ResetNotiArea();

bool InNotiArea(double x, double y);

//noti
void Noti(double mx, double my); //会按照传入的鼠标画提示栏边框

double GetNotiInch(); //返回一个提示框的宽度，同时更新thepage属性

/*void testText();
void testTop();
void testNoti();*/

#endif