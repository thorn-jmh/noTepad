#include "cursor.h"

static CURSOR_T theCursor ;//定义静态全局变量光标

static MOUSE_T theMouse ;//定义静态全局变量鼠标

static LINE_T theLine;//定义静态全局变量行信息

CURSOR_T *GetCurrentCursor(void){//返回光标指针
    CURSOR_T *pc = &theCursor;
    return pc;
};

LINE_T *GetCurrentLine(void){//返回鼠标指针
    LINE_T * pl = &theLine;
    return pl;
};

MOUSE_T *GetCurrentMouse(void){//返回行信息指针
    MOUSE_T *pm = &theMouse;
    return pm;
};
