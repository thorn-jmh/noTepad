#ifndef _editor_h
#define _editor_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

void InputString(CURSOR_T *crst);

void DeletString(CURSOR_T *crst);

void CopyString(CURSOR_T *crst);

void PasteString(CURSOR_T *crst);


#endif