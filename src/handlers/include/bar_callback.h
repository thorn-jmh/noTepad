#ifndef _bar_callback_h
#define _bar_callback_h

#include "genlib.h"

// button tpye
typedef struct
{
    string name;    // button name
    string hotkey;  // button hotkey
    void (*func)(); // button callback function
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