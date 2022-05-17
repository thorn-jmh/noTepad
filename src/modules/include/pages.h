#ifndef _pages_h
#define _pages_h

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"
#include "genlib.h"
#include "printer.h"

typedef struct {
  int Tline, //T是总行数
  Fline, //F是窗口的第一行是第几行
  Cline;//C是窗口可以容纳的行数
} LINE_T;

typedef struct{
    double X,Y;
} X_Y;

typedef struct{
    X_Y LB,RT;//左上右下两个参数
} AREA;

typedef struct {
   AREA TEXT,TOPBUTTON,SLIPER,NOTI;
} PAGE_T;

//静态全局变量接口部分
PAGE_T *GetPageInfo(void);//获取页面布局变量指针

LINE_T *GetCurrentLine(void);//获取行信息指针

void SetPageInfo(PAGE_T PageInfo);//设置页面布局

void SetLineInfo(LINE_T LineInfo);//设置行信息

//滑条部分 注意：只有在Tline>0的才可正常绘制！！!
void drawRect(double x, double y,double w,double h);

double SliperLength();

double GetSliperInch();//获取每行对应的长度

void DrawSliperBase();//画滑条底

void DrawSliperBlock();//根据行信息画滑块

//整体page信息部分  注：上端的功能条的高度和右侧的滑条的宽度是固定不变的,具体大小可以到pages.c调整

void PageSuitWindow();//这个函数会获取当前的窗口大小，对文本区进行调整

//noti
void Noti(double mx, double my);//会按照传入的鼠标画提示栏边框

double GetNotiInch ();//返回一个提示框的宽度，同时更新thepage属性

/*void testText();
void testTop();
void testNoti();*/

#endif