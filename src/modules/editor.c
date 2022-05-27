#include "editor.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "cursor.h"
#include "finder.h"
#include "printer.h"

static string GetStrFromClipBoard();
static bool AddStrToClipBoard(string str);
static void PasteBarText();
static void CopyBarText();
static void InputBarText(string newstr);
static void DeleteBarText(int direct);

static string BARTEXT = NULL;
#define MAX_BARTEXT 120 //Byte

void InitBarText()
{
  if (BARTEXT != NULL)
    CloseBarText();
  BARTEXT = (string)malloc(MAX_BARTEXT * sizeof(char));
  if (BARTEXT == NULL)
    Error("null ptr");
  *BARTEXT = '\0';
  CURSOR_T *crst = GetCurrentCursor();
  crst->focus = 2;
  crst->PTR_1 = crst->PTR_2 = 0;
}

string GetBarText()
{
  return BARTEXT;
}

void CloseBarText()
{
  CURSOR_T *crst = GetCurrentCursor();
  crst->focus = 0;
  free(BARTEXT);
  BARTEXT = NULL;
}

static void checkCursor()
{
  CURSOR_T *crst = GetCurrentCursor();
  LINE_T *lnt = GetCurrentLine();

  int nowline = crst->Line;
  int fline = lnt->Fline + 1;
  int lline = lnt->Fline + lnt->Cline - 2;

  if (nowline < fline)
    FocusLine(nowline, 0);
  else if (nowline > lline)
    FocusLine(nowline, 1);
}

void InputString(string newstr)
{
  CURSOR_T *crst = GetCurrentCursor();
  if (crst->focus == 0)
    return;
  else if (crst->focus == 2)
  {
    InputBarText(newstr);
    return;
  }

  FreeFoundList();
  checkCursor();

  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  AddStrToText(newstr, ptrf);
}

//0不移动，1向前，2向后
void DeleteString(int direct)
{
  CURSOR_T *crst = GetCurrentCursor();
  if (crst->focus == 0)
    return;
  else if (crst->focus == 2)
  {
    DeleteBarText(direct);
    return;
  }

  FreeFoundList();
  checkCursor();

  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  string Otext = GetStrText();
  if (direct == 1 && ptrf == ptrb && ptrf != 0)
  {
    if (OneCharLength(*(Otext + ptrf - 1)) == 2)
    {
      ptrf--;
    }
    ptrf--;
  }
  else if (direct == 2 && ptrf == ptrb && *(Otext + ptrb) != '\0')
  {
    if (OneCharLength(*(Otext + ptrb + 1)) == 2)
    {
      ptrb++;
    }
    ptrb++;
  }

  DeleteFromText(ptrf, ptrb);
}

void CopyTheString()
{
  CURSOR_T *crst = GetCurrentCursor();
  if (crst->focus == 0)
    return;
  else if (crst->focus == 2)
  {
    CopyBarText();
    return;
  }

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
  if (crst->focus == 0)
    return;
  else if (crst->focus == 2)
  {
    PasteBarText();
    return;
  }

  FreeFoundList();
  checkCursor();

  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  string newtext = GetStrFromClipBoard();
  AddStrToText(newtext, ptrf);
  crst->PTR_1 = ptrf;
  crst->PTR_2 = ptrf + strlen(newtext);
  free(newtext);
}

static void DeleteBarText(int direct)
{
  CURSOR_T *crst = GetCurrentCursor();
  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  if (direct == 1 && ptrf == ptrb && ptrf != 0)
  {
    if (OneCharLength(*(BARTEXT + ptrf - 1)) == 2)
    {
      ptrf--;
    }
    ptrf--;
  }
  else if (direct == 2 && ptrf == ptrb && *(BARTEXT + ptrb) != '/0')
  {
    if (OneCharLength(*(BARTEXT + ptrb + 1)) == 2)
    {
      ptrb++;
    }
    ptrb++;
  }

  string tpstr = (string)malloc((strlen(BARTEXT + ptrb) + 1) * sizeof(char));
  strcpy(tpstr, BARTEXT + ptrb);
  strcpy(BARTEXT + ptrf, tpstr);
  free(tpstr);

  crst->PTR_1 = crst->PTR_2 = ptrf;
}

static void InputBarText(string newstr)
{
  size_t oldlen, newlen;
  oldlen = strlen(BARTEXT);
  newlen = strlen(newstr);
  if (newlen + oldlen >= MAX_BARTEXT)
    return;

  CURSOR_T *crst = GetCurrentCursor();
  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  if (ptrf != ptrb)
    DeleteBarText(0);

  crst = GetCurrentCursor();
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  string tpstr = (string)malloc((strlen(BARTEXT + ptrb) + 1) * sizeof(char));
  strcpy(tpstr, BARTEXT + ptrb);
  strcpy(BARTEXT + ptrf, newstr);
  strcpy(BARTEXT + ptrf + newlen, tpstr);
  free(tpstr);
  crst->PTR_1 = crst->PTR_2 = ptrf + newlen;
}

static void CopyBarText()
{
  CURSOR_T *crst = GetCurrentCursor();
  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  if (ptrf == ptrb)
    return;

  string tpstr = (string)malloc((ptrb - ptrf + 1) * sizeof(char));
  memcpy(tpstr, BARTEXT + ptrf, ptrb - ptrf);
  *(tpstr + ptrb - ptrf) = '\0';

  AddStrToClipBoard(tpstr);
  free(tpstr);
}

static void PasteBarText()
{
  CURSOR_T *crst = GetCurrentCursor();
  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  string newstr = GetStrFromClipBoard();
  InputBarText(newstr);

  crst->PTR_1 = ptrf;
  crst->PTR_2 = ptrf + strlen(newstr);

  free(newstr);
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
  clipboardbuf = (string)malloc((strlen(pmem) + 1) * sizeof(char));
  strcpy(clipboardbuf, pmem);
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