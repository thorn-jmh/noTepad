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

void CLearTextBar();

void PrintTheText();

// void PrintText(Ustring text, int offset);

bool FocusLine(int line, bool Mode);

#endif