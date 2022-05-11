#include "editor.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

void InputString(string newstr, CURSOR_T *crst) {
  // history

  // TODO: focus
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  ptrf=AddStrToText(newstr, ptrf);

  crst->PTR_1 = ptrf;
  crst->PTR_2 = ptrf;
}

void DeletString(CURSOR_T *crst) {
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  if (ptrf == ptrb) {
    ptrf--;
    if (ptrf < 0) ptrf++;
  }

  DeleteFromText(ptrf, ptrb);

  crst->PTR_1 = ptrf;
  crst->PTR_2 = ptrf;
}

void CopyString(CURSOR_T *crst){
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  string origText=GetStrText();
  int len=ptrb-ptrf;
  string text=(string)malloc(len);
  memcpy(text,origText+ptrf,len);
  AddStrToClipBoard(text);
  free(text);

  crst->PTR_1=ptrf;
  crst->PTR_2=ptrb;
}

void PasteString(CURSOR_T *crst) {
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf,ptrb);
  ptrb = ptrf;
  string newtext = GetStrFromClipBoard();
  ptrf=AddStrToText(newtext,ptrf);
  free(newtext);

  crst->PTR_1=ptrf;
  crst->PTR_2=ptrf;
}

static string GetStrFromClipBoard() {
  string clipboardbuf;
  int t = 5;
  bool flag = FALSE;
  do {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag) {
    Error("failed to open clipboard");
    return "";
  }
  if(!IsClipboardFormatAvailable(CF_TEXT)){
    Error("clipboard format invalid");
    return "";
  }

  HGLOBAL hmem=GetClipboardData(CF_TEXT);
  if (hmem==NULL){
    return "";
  }
  string pmem;
  pmem=(string)GlobalLock(hmem);
  clipboardbuf = (string)melloc(strlen(pmem));
  memcpy(clipboardbuf,pmem,strlen(pmem));
  GlobalUnlock(hmem);

  return clipboardbuf;
}

static bool AddStrToClipBoard(string str) {
  int t = 5;
  bool flag = FALSE;
  do {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag) {
    Error("failed to open clipboard");
    return FALSE;
  }

  HGLOBAL hmem = GlobalAlloc(GHND, strlen(str) + 1);
  if (hmem == NULL) {
    Error("clipboard: failed to alloc mem");
    return FALSE;
  }
  string pmem = (string)GlobalLock(hmem);
  memcpy(pmem, str, strlen(str) + 1);

  EmptyClipboard();
  SetClipboardData(CF_TEXT, hmem);
  CloseClipboard();
  GlobalFree(hmem);

  return TRUE;
}