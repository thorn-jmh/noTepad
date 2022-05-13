#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

Ustring GetStrText();

int AddStrToText(Ustring newstr,int ptr);

int DeleteFromText(int ptr1,int ptr2);

bool OpenTheFile(Ustring filepath);

bool SaveTheFile(Ustring filepath);



#endif