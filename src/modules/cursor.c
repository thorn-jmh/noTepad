#include "cursor.h"

static CURSOR_T theCursor ;//定义静态全局变量光标

static MOUSE_T theMouse ;//定义静态全局变量鼠标

CURSOR_T *GetCurrentCursor(void){//返回光标指针
    CURSOR_T *pc = &theCursor;
    return pc;
};

MOUSE_T *GetCurrentMouse(void){//返鼠标指针
    MOUSE_T *pm = &theMouse;
    return pm;
};
