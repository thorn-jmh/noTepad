#ifndef _topbar_h
#define _topbar_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "genlib.h"

typedef struct
{
    string name;
    string hotkey;
    void (*func)();
} BOTTON_T;

void DeleteBarText();

void InputBarText(string newstr);

void PasteBarText();

void CopyBarText();

void InitBarText();

void CloseBarText();

string GetBarText();

void drawTopBars();

void drawSettingBar();

void drawSearchBar();

#endif