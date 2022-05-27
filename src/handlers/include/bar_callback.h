#ifndef _bar_callback_h
#define _bar_callback_h

#include "genlib.h"

typedef struct
{
    string name;
    string hotkey;
    void (*func)();
} BOTTON_T;

void fileextcbk();
void opencbk();
void closecbk();
void newcbk();
void savecbk();
void saveascbk();
void editextcbk();
void copycbk();
void pastecbk();
void cutcbk();
void deletecbk();
void selectcbk();
void undocbk();
void redocbk();
void searchcbk();
void confextcbk();
void helpcbk();
void nextcbk();
void cancelcbk();

#endif