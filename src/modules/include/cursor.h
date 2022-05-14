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

typedef struct {
  int Tline, Fline, Cline;
} LINE_T;

typedef enum { UP, LEFT, RIGHT, DOWN } DIRECT_KEY;

CURSOR_T *GetCurrentCursor(void);

LINE_T *GetCurrentLine(void);

MOUSE_T *GetCurrentMouse(void);

#endif