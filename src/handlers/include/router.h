#ifndef _router_h
#define _router_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"cursor.h"
#include"editor.h"
#include"file.h"
#include"pages.h"
#include"printer.h"
#include"genlib.h"
#include"graphics.h"

void InitStatus();

void keyboardManager (int key,int event);

void mouseManager(int x, int y, int button, int event);

void charManager (char c);

void timerManager (int timerID);


#endif