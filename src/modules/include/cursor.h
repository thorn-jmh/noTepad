#ifndef _cursor_h
#define _cursor_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"
#include"graphics.h"

typedef struct{
    int PTR_1,PTR_2;
    double X,Y;
} CURSOR_T;

typedef struct{
    ACL_Mouse_Button button;
    ACL_Mouse_Event event;
    int X,Y;
} MOUSE_T;

typedef enum{
    UP,LEFT,RIGHT,DOWN
} DIRECT_KEY;

CURSOR_T *GetCurrentCursor(void);

void SetCursorToPTR(int PTR);

void MoveCursorByMouse(CURSOR_T *crst, MOUSE_T *mst);

void MoveCursorByDirect(CURSOR_T *crst, DIRECT_KEY drct);

#endif