#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

Ustring GetStrText();

void AddStrToText(Ustring newstr,size_t ptr);

void DeleteFromText(size_t ptr1,size_t ptr2);

bool InitFileSys();

int GetFilesNum();

bool ChangeCurrentFile(int ith);

bool OpenTheFile(Ustring filepath);

bool SaveTheFile(Ustring filepath);

bool CloseTheFile(bool force);

void RedoHistory();

void UndoHistory();

#endif