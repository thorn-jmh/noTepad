#ifndef _finder_h
#define _finder_h

#include "linkedlist.h"
#include "genlib.h"

linkedlistADT GetCurrentHighlight();

void FindSubStr(string target);

void FreeFoundList();

int TotalFound();

int NowFound();

size_t WhereFoundStr();

void NextFoundStr();

void LastFoundStr();

#endif