#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "genlib.h"

//TODO: for debug
//#ifndef _cursor_h
//typedef struct {
//	size_t PTR_1, PTR_2;
//	double X, Y;
//} CURSOR_T;
//
//CURSOR_T* GetCurrentCursor();
//#endif

void InitFileSys();

int GetFilesNum();

void ChangeCurrentFile(int ith);

bool OpenTheFile(string filepath);

bool SaveTheFile(string filepath);

bool CloseTheFile(bool force);

string GetStrText();

int CurrentFileI();

string GetFileName(int ith);

void AddStrToText(string newstr, size_t ptr);

void DeleteFromText(size_t ptr1, size_t ptr2);

void RedoHistory();

void UndoHistory();

#endif