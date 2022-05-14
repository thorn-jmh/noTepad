#ifndef _pages_h
#define _pages_h

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"
#include "genlib.h"
#include "printer.h"

typedef struct{
    double top,bottom,left,right;
} AREA;

typedef struct {
   AREA TEXT,TOPLINE,SLIPER,NOTI;
} PAGE_T;

void Updeate(); //undefined

PAGE_T *GetPageInfo();
/*

PAGE

- UpdatePage

- PAGE_T *GetPageInfo


SLIPER

- 通过行信息画滑条

- 返回inch和行数的映射

Noti

- 根据起始位置画弹窗（四个角）

- 返回相对纵坐标对应功能

Topline

- 画

- 返回横坐标对应

*/

#endif