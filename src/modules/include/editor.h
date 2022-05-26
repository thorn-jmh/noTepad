#ifndef _editor_h
#define _editor_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

void InitBarText();

void CloseBarText();

string GetBarText();

void InputString(string newstr);

void DeleteString(int direct);

void CopyTheString();

void PasteTheString();

#endif