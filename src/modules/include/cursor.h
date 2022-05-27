#ifndef _cursor_h
#define _cursor_h

#include "genlib.h"
#include "graphics.h"

// 行信息类型
typedef struct
{
  int Tline, //T是总行数
      Fline, //F是窗口的第一行是第几行
      Cline; //C是窗口可以容纳的行数
} LINE_T;

// 光标信息
typedef struct
{
  size_t PTR_1, PTR_2; // 两个光标指针
  double X, Y;         //坐标
  int Line;            // 所在行数
  int focus;           //0 空，1 文本，2 输入框
} CURSOR_T;

// 鼠标信息
typedef struct
{
  ACL_Mouse_Button button;
  ACL_Mouse_Event event;
  size_t PTR;  //对应指针
  double X, Y; //坐标
} MOUSE_T;

LINE_T *GetCurrentLine(void); //获取行信息指针

CURSOR_T *GetCurrentCursor(void); //获取光标指针

MOUSE_T *GetCurrentMouse(void); //获取鼠标信息指针

#endif