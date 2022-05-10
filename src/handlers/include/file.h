#ifndef _file_h
#define _file_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include"genlib.h"

//TODO: INIT_FILEBUF_SIZE 2048

// typedef struct{
//     int TLength,UnderL;
//     string OriginText;
// } TEXT_FILE;

int GetStrText(string ptr);

void AddStrToText(string newstr,int ptr);

void DeleteFromText(int ptr1,int ptr2);

bool OpenFile(string filepath);

bool SaveFile(string filepath);



#endif