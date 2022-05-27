#ifndef _file_h
#define _file_h

#include "genlib.h"


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