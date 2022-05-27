#ifndef _printer_h
#define _printer_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"
#include"graphics.h"
#include"extgraph.h"
#include"cursor.h"
#include"pages.h"

void ChangeCursor();

string GetTextSize();
string GetTextFont();

void SetTextFont(string font);

void SetTextSize(string size);


void CLearTextBar();

void PrintTheText(bool print);

bool FocusLine(int line, bool Mode);

#endif