#include "pages.h"

static LINE_T theLine; //定义静态全局变量行信息

static PAGE_T thePage; //定义静态页面布局变量

//滑条的宽度是常量，不会因为页面改变而变化。宽度定义为：
    //先画滑条底部
static double sliperw = 0.1;
static double toph = 0.1;//顶部高度

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
void drawRect(double x, double y,double w,double h){//xy是左下角
    MovePen(x, y);
    DrawLine(0, h);
    DrawLine(w,0);
    DrawLine(0,-h);
    DrawLine(-w,0);
}

double SliperLength()
{ //滑条的长度
    double windowh = GetWindowHeight()-toph;
    double sl = ((double)theLine.Cline / (double)theLine.Tline) * windowh;
    return sl;
};

double GetSliperInch()
{ //获取每一行对应在滑条的长度
    double inch;
    double windowh = GetWindowHeight()-toph;
    double sliperl = SliperLength();
    inch = (windowh - sliperl) / (double)(theLine.Tline-1);
    return inch;
};

void DrawSliperBase()
{//先画滑条底
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();
    
    DefineColor("gray",0.8, 0.8, 0.8);
    SetPenColor("gray");
    StartFilledRegion(0.9);
    drawRect(windoww-sliperw,0,sliperw,windowh-toph);
    EndFilledRegion();
}

void DrawSliperBlock(){
    double windowh = GetWindowHeight();
    double windoww = GetWindowWidth();
    double inch = GetSliperInch();
    double blockl = SliperLength();
    double blockTop = toph + (theLine.Fline-1)*inch;
    SetPenColor("black");
    StartFilledRegion(0.9);
    drawRect(windoww - sliperw,windowh-(blockTop+blockl),sliperw,blockl);
    EndFilledRegion();
}


