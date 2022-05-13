// FOR TEST
// DELETE BEFORE EDITOR

#include <Windows.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef FILE *stream;
typedef int bool;
typedef char *string;
// init
bool AddStrToClipBoard(string str);
string GetStrFromClipBoard();
bool OpenTheFile(string filepath);
// register

////////////////////////////////////////
#define INIT_FILEBUF_SIZE 2048

#define UNSAVED_FILE "unsaved_new_file"

typedef struct {
  string filename;
  bool is_saved;
  int TLength, UnderL;
  char OriginText[0];
} TEXT_FILE;
#define test_file_path "F:\\WTF\\cs\\thorn-jmh\\pjc\\noTepad\\temp.txt"
#define sttt "abcdefghijklmn"
#define init_si 4
///////////////////////////////////////

int Winmain() {
  // WideCharToMultiByte()
  wchar_t *s=L"asz 中文 🎈";
  printf("qaq\n");
  stream f;
  bool err=fopen_s(&f,"temp.🎈","w");
  fwprintf(f,L"%ls",s);
  fclose(f);
  err=fopen_s(&f,"temp.qaq","w, ccs=utf-8");
  fwprintf(f,L"%ls",s);
  fclose(f);
  return 0;
}