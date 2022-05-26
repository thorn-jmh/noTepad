#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "pages.h"
static double slipermargin = 0.05;
enum MODE{
    NONE,
    SHIFT,
    FOLLOW,
};

typedef struct{//静态全局记录滑块顶端的位置
    double  top;
    double  buttom;
} Block;


static Block theblock;


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


    double blockTop = toph + (theLine->Fline - 1) * inch;//距离顶端的距离
    theblock.buttom =  windowh - (blockTop + blockl);
    theblock.top = windowh - blockTop;
    SetPenColor("black");
    StartFilledRegion(0.9);
    drawRect(windoww - sliperw, theblock.buttom, sliperw, blockl);
    EndFilledRegion();
}

void DrawSliper()
{
    DrawSliperBase();
    DrawSliperBlock();
}


int InSliperZoom(){
    MOUSE_T *mp =GetCurrentMouse();

    double mx = mp->X;
    double my = mp->Y;

    PAGE_T *pgt = GetPageInfo();
    double windowsh = GetWindowHeight();

    if(mx>=pgt->SLIPER.LT.X&&mx<=pgt->SLIPER.RB.X-0.005) return 1;
    else return 0;
}

int InSliperblock(){
    double blockL = SliperLength();
    MOUSE_T *mp =GetCurrentMouse();
    double mx = mp->X;
    double my = my = mp->Y;
    PAGE_T *pgt = GetPageInfo();
    double windowh = pgt->PAGE.LT.Y;
    
    if(mx>=pgt->SLIPER.LT.X&&
    mx<=pgt->SLIPER.RB.X-0.005&&
    my<=theblock.top &&
    my>=theblock.buttom) return 1;
    else return 0;
}

static int MODE;
static double last_my, current_my;

int * testMode(){
    int*p =&MODE;
    return p;
}

static int testshiftL;
int * testShiftL(){
    int *p = &testshiftL;
    return p;
}


void sliper(){
    //判断是跟随鼠标滑动还是瞬移
    MOUSE_T* MousePtr = GetCurrentMouse();
    double mx = MousePtr->X;
    double my = MousePtr->Y;

    if(MousePtr->button == LEFT_BUTTON&&MousePtr->event==BUTTON_DOWN){
        if(InSliperblock()) MODE = FOLLOW;
        else if(InSliperZoom()){
            MODE = SHIFT;
            last_my = my;//记录点击下去的时候的y坐标
        }
    }

    LINE_T* LinePtr = GetCurrentLine();
    double inch = GetSliperInch();
    double length = SliperLength();

    //滚轮
    if(MousePtr->button==MIDDLE_BUTTON&&MousePtr->event==ROLL_DOWN){
        LinePtr->Fline+=1;
        if(LinePtr->Fline<=1)LinePtr->Fline=1;
        if(LinePtr->Fline>=LinePtr->Tline)LinePtr->Fline=LinePtr->Tline;
    }

    if(MousePtr->button==MIDDLE_BUTTON&&MousePtr->event==ROLL_UP){
        LinePtr->Fline-=1;
        if(LinePtr->Fline<=1)LinePtr->Fline=1;
        if(LinePtr->Fline>=LinePtr->Tline)LinePtr->Fline=LinePtr->Tline;
    }

    //点击滑块之外，瞬移
    if(MODE == SHIFT){
        int ShiftL =-((my-theblock.top+(length/2))/inch) ;
        LinePtr->Fline +=ShiftL;
        if(LinePtr->Fline<=1)LinePtr->Fline=1;
        if(LinePtr->Fline>=LinePtr->Tline)LinePtr->Fline=LinePtr->Tline;
        MODE = NONE;
    }

    //点击滑块，跟随
    if(MODE == FOLLOW){
        current_my = my;
        
        if(current_my-last_my>=inch
        ||current_my-last_my<=-inch){
            testshiftL =-((my-theblock.top+(length/2))/inch) ;
            LinePtr->Fline+=testshiftL;
            if(LinePtr->Fline<=1)LinePtr->Fline=1;
            if(LinePtr->Fline>=LinePtr->Tline)LinePtr->Fline=LinePtr->Tline;
            last_my = current_my;
        }

        if(MousePtr->button == LEFT_BUTTON&&
        MousePtr->event==BUTTON_UP) MODE=NONE;
    }
}

void cleanSliper(){
    PAGE_T *pgt = GetPageInfo();
    SetPenColor("white");
    StartFilledRegion(0);
    drawRect(pgt->SLIPER.LT.X,
             pgt->SLIPER.RB.Y,
             pgt->SLIPER.RB.X - pgt->SLIPER.LT.X,
             pgt->SLIPER.LT.Y - pgt->SLIPER.RB.Y);
    EndFilledRegion();
}

