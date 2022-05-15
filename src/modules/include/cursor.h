#ifndef _cursor_h
#define _cursor_h

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "genlib.h"
#include "graphics.h"

typedef struct {
  size_t PTR_1, PTR_2;
  double X, Y;
} CURSOR_T;

typedef struct {
  ACL_Mouse_Button button;
  ACL_Mouse_Event event;
  size_t PTR;
  int X, Y;
} MOUSE_T;


typedef enum { UP, LEFT, RIGHT, DOWN } DIRECT_KEY;//这个先不管它

CURSOR_T *GetCurrentCursor(void);//获取光标指针

MOUSE_T *GetCurrentMouse(void);//获取鼠标信息指针

#endif