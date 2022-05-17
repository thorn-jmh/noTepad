#include "pages.h"

static LINE_T theLine; //定义静态全局变量行信息

static PAGE_T thePage; //定义静态页面布局变量

static double toph = 0.2;    //顶部高度
static double sliperw = 0.1; //滑块宽度
static double margin = 0.1;  //页边留白
static double notih = 2;
static double notiw = 1.5;
static double notinum = 5;

//静态全局变量接口部分
LINE_T *GetCurrentLine(void)
{ //返回行信息指针
    LINE_T *pl = &theLine;
    return pl;
};

PAGE_T *GetPageInfo()
{ //返回页面布局指针
    PAGE_T *pp = &thePage;
    return pp;
};

void SetLineInfo(LINE_T LineInfo)
{ //设置行信息
    theLine = LineInfo;
};

void SetPageInfo(PAGE_T PageInfo)
{ //设置页面布局
    thePage = PageInfo;
};

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
    double windowh = GetWindowHeight() - toph;
    double sl = ((double)theLine.Cline / (double)theLine.Tline) * windowh;
    return sl;
};

double GetSliperInch()
{ //获取每一行对应在滑条的长度
    double inch;
    double windowh = GetWindowHeight() - toph;
    double sliperl = SliperLength();
    inch = (windowh - sliperl) / (double)(theLine.Tline - 1);
    return inch;
};

void DrawSliperBase()
{ //先画滑条底

    DefineColor("gray", 0.8, 0.8, 0.8);
    SetPenColor("gray");
    StartFilledRegion(0.9);
    drawRect(thePage.SLIPER.LB.X,
             thePage.SLIPER.LB.Y,
             thePage.SLIPER.RT.X - thePage.SLIPER.LB.X,
             thePage.SLIPER.RT.Y - thePage.SLIPER.LB.Y);
    EndFilledRegion();
}

void DrawSliperBlock()
{ //画滑块
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();
    double inch = GetSliperInch();
    double blockl = SliperLength();

    double blockTop = toph + (theLine.Fline - 1) * inch;
    SetPenColor("black");
    StartFilledRegion(0.9);
    drawRect(windoww - sliperw, windowh - (blockTop + blockl), sliperw, blockl);
    EndFilledRegion();
}

void PageSuitWindow()
{
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();

    thePage.SLIPER.LB.X = windoww - sliperw;
    thePage.SLIPER.LB.Y = 0;
    thePage.SLIPER.RT.X = windoww;
    thePage.SLIPER.RT.Y = windowh - toph;

    thePage.TOPBUTTON.LB.X = 0;
    thePage.TOPBUTTON.LB.Y = windowh - toph;
    thePage.TOPBUTTON.RT.X = windoww;
    thePage.TOPBUTTON.RT.Y = windowh;

    thePage.TEXT.LB.X = margin;
    thePage.TEXT.LB.Y = 0;
    thePage.TEXT.RT.X = windoww - margin;
    thePage.TEXT.RT.Y = windowh - toph;
}; //这个函数会获取当前的窗口大小，对文本区进行调整

void Noti(double mx, double my)
{ //会按照传入的鼠标画一个长方体框框
    if (mx <= margin || mx >= thePage.TEXT.RT.X || my >= thePage.TEXT.RT.Y)
        return;
    DefineColor("gray", 0.8, 0.8, 0.8);
    SetPenColor("gray");
    int LorR, UporD;
    double inch = GetNotiInch();
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();
    if (mx <= windoww / 2)
        LorR = 1;
    else
        LorR = -1;
    if (my <= windowh / 2)
        UporD = 1;
    else
        UporD = -1;
    drawRect(mx, my, LorR * notiw, UporD * notih);
    int i;
    for (i = 1; i <= notinum; i++)
    {
        MovePen(mx, my + i * UporD * inch);
        DrawLine(notiw * LorR, 0);
    }

    
    thePage.NOTI.LB.X = mx<mx+LorR*notiw?mx:mx+LorR*notiw;
    thePage.NOTI.LB.Y = my<my+UporD*notih?my:my+UporD*notih;
    thePage.NOTI.RT.X = mx>mx+LorR*notiw?mx:mx+LorR*notiw;
    thePage.NOTI.RT.Y = my>my+UporD*notih?my:my+UporD*notih;
}

double GetNotiInch()
{
    return notih / notinum;
}

/*void testText(void)
{
    drawRect(thePage.TEXT.LB.X, thePage.TEXT.LB.Y,
             thePage.TEXT.RT.X - thePage.TEXT.LB.X,
             thePage.TEXT.RT.Y - thePage.TEXT.LB.Y);
}

void testTop(void)
{
    SetPenColor("red");
    drawRect(thePage.TOPBUTTON.LB.X, thePage.TOPBUTTON.LB.Y,
             thePage.TOPBUTTON.RT.X - thePage.TOPBUTTON.LB.X,
             thePage.TOPBUTTON.RT.Y - thePage.TOPBUTTON.LB.Y);
}

void testNoti(void){
    SetPenColor("yellow");
    drawRect(thePage.NOTI.LB.X, thePage.NOTI.LB.Y,
             thePage.NOTI.RT.X - thePage.NOTI.LB.X,
             thePage.NOTI.RT.Y - thePage.NOTI.LB.Y);
}*/
