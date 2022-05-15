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
    double top,bottom,left,right;
} AREA;

typedef struct {
   AREA TEXT,TOPLINE,SLIPER,NOTI;
} PAGE_T;

PAGE_T *GetPageInfo(void);//获取页面布局变量指针

LINE_T *GetCurrentLine(void);//获取行信息指针

void SetPageInfo(PAGE_T PageInfo);//设置页面布局

void SetLineInfo(LINE_T LineInfo);//设置行信息

//滑条部分 注意！！!只有在Tline>0的才可正常绘制！！!
void drawRect(double x, double y,double w,double h);

double SliperLength();

double GetSliperInch();//获取每行对应的长度

void DrawSliperBase();//画滑条底

void DrawSliperBlock();//根据行信息画滑条

/*
PAGE
---done UpdatePage
---done PAGE_T *GetPageInfo

SLIPER
--done通过行信息画滑条
--done返回inch和行数的映射

Noti
- 根据起始位置画弹窗（四个角）
- 返回相对纵坐标对应功能

Topline
- 画
- 返回横坐标对应
*/

#endif