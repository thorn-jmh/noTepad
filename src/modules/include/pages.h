#ifndef _pages_h
#define _pages_h

#include "genlib.h"

// 定义 X_Y 表示点
typedef struct
{
    double X, Y;
} X_Y;

// 定义方形区域，记录左上右下两点
typedef struct
{
    X_Y LT, RB; //左上右下两个参数
} AREA;

// 定义页面区域
typedef struct
{
    AREA PAGE;    // 全页面
    AREA TEXT;    //文字区
    AREA TOPBAR;  //顶栏区
    AREA SLIPER;  //滑条区
    AREA NOTI;    //noti区
    AREA LEFTBAR; //左栏区
} PAGE_T;

PAGE_T *GetPageInfo(void);


void UpdatePageInfo();

void SetNotiArea(double x, double y, double w, double h);

void ResetNotiArea();

bool InNotiArea(double x, double y);

bool InTopArea(double x, double y);

#endif