#ifndef _printer_h
#define _printer_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

typedef struct {
    int Tline,Fline,Cline;
} LINE_T;

void PrePrintText(string text,int offset,CURSOR_T *crst,MOUSE_T *mst,LINE_T *lnt);

void PrintText(string text,int offset);


#endif