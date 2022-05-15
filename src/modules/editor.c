#include "editor.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

static Ustring GetUstrFromClipBoard();
static bool AddUstrToClipBoard(Ustring str);

// TODO: focus

void InputUstring(Ustring newstr)
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  AddStrToText(newstr, ptrf);
}

void DeleteUstring()
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

void CopyUstring()
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  Ustring origText = GetStrText();
  int len = ptrb - ptrf;
  Ustring text = (Ustring)malloc((len + 1) * sizeof(Unicode));
  memcpy(text, origText + ptrf, len * sizeof(Unicode));
  // wcscpy(text, len, origText + ptrf);
  *(text + len) = L'\0';
  AddUstrToClipBoard(text);
  free(text);
}

void PasteUstring()
{
  CURSOR_T *crst = GetCurrentCursor();
  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  ptrb = ptrf;
  string newtext = GetUstrFromClipBoard();
  AddStrToText(newtext, ptrf);
  free(newtext);
}

static Ustring GetUstrFromClipBoard()
{
  Ustring clipboardbuf;
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
  Ustring pmem;
  clipboardbuf = Whatever2Ustring(GlobalLock(hmem));
  GlobalUnlock(hmem);

  return clipboardbuf;
}

static bool AddUstrToClipBoard(Ustring str)
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

  HGLOBAL hmem = GlobalAlloc(GHND, (wcslen(str) + 1) * sizeof(Unicode));
  if (hmem == NULL)
  {
    Error("clipboard: failed to alloc mem");
    return FALSE;
  }
  Ustring pmem = (Ustring)GlobalLock(hmem);
  wcscpy_s(pmem, wcslen(str) + 1, str);

  EmptyClipboard();
  SetClipboardData(CF_TEXT, hmem);
  CloseClipboard();
  GlobalFree(hmem);

  return TRUE;
}