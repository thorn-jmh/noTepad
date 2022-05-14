#ifndef _printer_h
#define _printer_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"
#include"cursor.h"

void PrintText(string text,int offset,CURSOR_T *crst,MOUSE_T *mst,LINE_T *lnt);


#endif