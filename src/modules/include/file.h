#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

string GetStrText();

int AddStrToText(string newstr,int ptr);

int DeleteFromText(int ptr1,int ptr2);

bool OpenTheFile(string filepath);

bool SaveTheFile(string filepath);



#endif