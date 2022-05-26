#ifndef _bar_callback_h
#define _bar_callback_h

#include "genlib.h"

typedef struct
{
    string name;
    string hotkey;
    void (*func)();
} BOTTON_T;


#endif