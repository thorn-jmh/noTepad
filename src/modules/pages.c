#include "pages.h"

static PAGE_T thePage; //定义静态页面布局变量

static double toph = 0.2;    //顶部高度
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
    LINE_T *theLine = GetCurrentLine();
    double windowh = GetWindowHeight() - toph;
    double sl = ((double)theLine->Cline / (double)theLine->Tline);
    if (sl > 1.0)
        sl = 1.0;
    sl *= windowh;
    return sl;
};

double GetSliperInch()
{ //获取每一行对应在滑条的长度
    LINE_T *theLine = GetCurrentLine();
    double inch;
    double windowh = GetWindowHeight() - toph;
    double sliperl = SliperLength();
    if (theLine->Cline >= theLine->Tline)
        inch = 0;
    else
        inch = (windowh - sliperl) / (double)(theLine->Tline - 1);
    return inch;
};

void DrawSliperBase()
{ //先画滑条底

    DefineColor("gray", 0.8, 0.8, 0.8);
    SetPenColor("gray");
    StartFilledRegion(0.9);
    drawRect(thePage.SLIPER.LT.X,
             thePage.SLIPER.LT.Y,
             thePage.SLIPER.RB.X - thePage.SLIPER.LT.X,
             thePage.SLIPER.RB.Y - thePage.SLIPER.LT.Y);
    EndFilledRegion();
}

void DrawSliperBlock()
{ //画滑块
    LINE_T *theLine = GetCurrentLine();
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();
    double inch = GetSliperInch();
    double blockl = SliperLength();

    double blockTop = toph + (theLine->Fline - 1) * inch;
    SetPenColor("black");
    StartFilledRegion(0.9);
    drawRect(windoww - sliperw, windowh - (blockTop + blockl), sliperw, blockl);
    EndFilledRegion();
}

void PageSuitWindow()
{
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();

    thePage.SLIPER.LT.X = windoww - sliperw;
    thePage.SLIPER.LT.Y = 0;
    thePage.SLIPER.RB.X = windoww;
    thePage.SLIPER.RB.Y = windowh - toph;

    thePage.TOPBUTTON.LT.X = 0;
    thePage.TOPBUTTON.LT.Y = windowh - toph;
    thePage.TOPBUTTON.RB.X = windoww;
    thePage.TOPBUTTON.RB.Y = windowh;

    thePage.TEXT.LT.X = margin;
    thePage.TEXT.LT.Y = 0;
    thePage.TEXT.RB.X = windoww - margin;
    thePage.TEXT.RB.Y = windowh - toph;
}; //这个函数会获取当前的窗口大小，对文本区进行调整

void Noti(double mx, double my)
{ //会按照传入的鼠标画一个长方体框框
    if (mx <= margin || mx >= thePage.TEXT.RB.X || my >= thePage.TEXT.RB.Y)
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

    thePage.NOTI.LT.X = mx < mx + LorR * notiw ? mx : mx + LorR * notiw;
    thePage.NOTI.LT.Y = my < my + UporD * notih ? my : my + UporD * notih;
    thePage.NOTI.RB.X = mx > mx + LorR * notiw ? mx : mx + LorR * notiw;
    thePage.NOTI.RB.Y = my > my + UporD * notih ? my : my + UporD * notih;
}

double GetNotiInch()
{
    return notih / notinum;
}

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
