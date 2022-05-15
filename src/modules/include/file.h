#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

//TODO: for debug
#ifndef _cursor_h
typedef struct {
	size_t PTR_1, PTR_2;
	double X, Y;
} CURSOR_T;

CURSOR_T* GetCurrentCursor();
#endif

bool InitFileSys();

int GetFilesNum();

bool ChangeCurrentFile(int ith);

bool OpenTheFile(string filepath, bool utf8);

bool SaveTheFile(string filepath);

bool CloseTheFile(bool force);




Ustring GetStrText();

void AddStrToText(Ustring newstr,size_t ptr);

void DeleteFromText(size_t ptr1,size_t ptr2);





void RedoHistory();

void UndoHistory();

#endif