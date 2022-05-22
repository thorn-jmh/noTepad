#include "editor.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "cursor.h"

static string GetStrFromClipBoard();
static bool AddStrToClipBoard(string str);

// TODO: focus

void InputString(string newstr)
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  AddStrToText(newstr, ptrf);
}

void DeleteString()
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  if (ptrf == ptrb)
  {
    ptrf--;
    if (ptrf < 0)
      ptrf++;
  }

  DeleteFromText(ptrf, ptrb);
}

void CopyTheString()
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  string origText = GetStrText();
  int len = ptrb - ptrf;
  string text = (string)malloc((len + 1) * sizeof(char));
  memcpy(text, origText + ptrf, len * sizeof(char));
  *(text + len) = '\0';
  AddStrToClipBoard(text);
  free(text);
}

void PasteTheString()
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  ptrb = ptrf;
  string newtext = GetStrFromClipBoard();
  AddStrToText(newtext, ptrf);
  free(newtext);
}

static string GetStrFromClipBoard()
{
  string clipboardbuf;
  int t = 5;
  bool flag = FALSE;
  do
  {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag)
  {
    Error("failed to open clipboard");
    return "";
  }
  if (!IsClipboardFormatAvailable(CF_TEXT))
  {
    Error("clipboard format invalid");
    return "";
  }

  HGLOBAL hmem = GetClipboardData(CF_TEXT);
  if (hmem == NULL)
  {
    return "";
  }
  string pmem;
  pmem = (string)GlobalLock(hmem);
  clipboardbuf = (string)malloc((strlen(pmem)+1)*sizeof(char));
  strcpy(clipboardbuf,pmem);
  GlobalUnlock(hmem);

  return clipboardbuf;
}

static bool AddStrToClipBoard(string str)
{
  int t = 5;
  bool flag = FALSE;
  do
  {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag)
  {
    Error("failed to open clipboard");
    return FALSE;
  }

  HGLOBAL hmem = GlobalAlloc(GHND, (strlen(str) + 1) * sizeof(char));
  if (hmem == NULL)
  {
    Error("clipboard: failed to alloc mem");
    return FALSE;
  }
  string pmem = (string)GlobalLock(hmem);
  strcpy(pmem, str);

  EmptyClipboard();
  SetClipboardData(CF_TEXT, hmem);
  CloseClipboard();
  GlobalFree(hmem);

  return TRUE;
}