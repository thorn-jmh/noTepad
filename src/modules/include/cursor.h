#ifndef _cursor_h
#define _cursor_h

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "genlib.h"
#include "graphics.h"

typedef struct {
  int Tline, //T是总行数
  Fline, //F是窗口的第一行是第几行
  Cline;//C是窗口可以容纳的行数
} LINE_T;

typedef struct {
  size_t PTR_1, PTR_2;
  double X, Y;
  int Line;
  int focus; //0 空，1 文本，2 输入框
} CURSOR_T;

typedef struct {
  ACL_Mouse_Button button;
  ACL_Mouse_Event event;
  size_t PTR;
  double X, Y;
} MOUSE_T;


typedef enum { UP, LEFT, RIGHT, DOWN } DIRECT_KEY;//这个先不管它

LINE_T *GetCurrentLine(void);//获取行信息指针

CURSOR_T *GetCurrentCursor(void);//获取光标指针

MOUSE_T *GetCurrentMouse(void);//获取鼠标信息指针

#endif