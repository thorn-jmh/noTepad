#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "pages.h"

//滑条部分
void drawRect(double x, double y, double w, double h)
{ // xy是左下角
    MovePen(x, y);
    DrawLine(0, h);
    DrawLine(w, 0);
    DrawLine(0, -h);
    DrawLine(-w, 0);
}

double SliperLength()
{ //滑块的长度
    PAGE_T *pgt = GetPageInfo();
    double toph = pgt->PAGE.LT.Y - pgt->SLIPER.LT.Y;
    LINE_T *theLine = GetCurrentLine();
    double windowh = pgt->PAGE.LT.Y - toph;
    double sl = ((double)theLine->Cline / (double)theLine->Tline);
    if (sl > 1.0)
        sl = 1.0;
    sl *= windowh;
    return sl;
};

double GetSliperInch()
{ //获取每一行对应在滑条的长度

    PAGE_T *pgt = GetPageInfo();
    double toph = pgt->PAGE.LT.Y - pgt->SLIPER.LT.Y;
    LINE_T *theLine = GetCurrentLine();
    double inch;
    double windowh = pgt->PAGE.LT.Y - toph;
    double sliperl = SliperLength();
    if (theLine->Cline >= theLine->Tline)
        inch = 0;
    else
        inch = (windowh - sliperl) / (double)(theLine->Tline - 1);
    return inch;
};

void DrawSliperBase()
{ //先画滑条底
    PAGE_T *pgt = GetPageInfo();

    DefineColor("gray", 0.8, 0.8, 0.8);
    SetPenColor("gray");
    StartFilledRegion(0.9);
    drawRect(pgt->SLIPER.LT.X,
             pgt->SLIPER.RB.Y,
             pgt->SLIPER.RB.X - pgt->SLIPER.LT.X,
             pgt->SLIPER.LT.Y - pgt->SLIPER.RB.Y);
    EndFilledRegion();
}

void DrawSliperBlock()
{ //画滑块
    PAGE_T *pgt = GetPageInfo();
    double toph = pgt->PAGE.LT.Y - pgt->SLIPER.LT.Y;
    double sliperw = pgt->SLIPER.RB.X - pgt->SLIPER.LT.X;

    LINE_T *theLine = GetCurrentLine();
    double windowh = pgt->PAGE.LT.Y;
    double windoww = pgt->PAGE.RB.X;
    double inch = GetSliperInch();
    double blockl = SliperLength();

    double blockTop = toph + (theLine->Fline - 1) * inch;
    SetPenColor("black");
    StartFilledRegion(0.9);
    drawRect(windoww - sliperw, windowh - (blockTop + blockl), sliperw, blockl);
    EndFilledRegion();
}

void DrawSliper()
{
    DrawSliperBase();
    DrawSliperBlock();
}