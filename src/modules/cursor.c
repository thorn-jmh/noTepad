#include "cursor.h"

static LINE_T theLine; //定义静态全局变量行信息

static CURSOR_T theCursor; //定义静态全局变量光标

static MOUSE_T theMouse; //定义静态全局变量鼠标

//返回行信息指针
LINE_T *GetCurrentLine(void)
{
    LINE_T *pl = &theLine;
    return pl;
};

//返回光标指针
CURSOR_T *GetCurrentCursor(void)
{
    CURSOR_T *pc = &theCursor;
    return pc;
};

//返鼠标指针
MOUSE_T *GetCurrentMouse(void)
{
    MOUSE_T *pm = &theMouse;
    return pm;
};
