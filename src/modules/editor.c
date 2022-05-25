#include "editor.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "topbar.h"
#include "cursor.h"
#include "clipboard.h"

// TODO: focus

void InputString(string newstr)
{
  CURSOR_T *crst = GetCurrentCursor();
  if(crst->focus==0) return;
  else if(crst->focus==2){
    InputBarText(newstr);
    return;
  }


  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  AddStrToText(newstr, ptrf);
}

void DeleteString()
{
  CURSOR_T *crst = GetCurrentCursor();
  if(crst->focus==0) return;
  else if(crst->focus==2){
    DeleteBarText();
    return;
  }


  size_t ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);
  string Otext = GetStrText();
  if (ptrf == ptrb && ptrf != 0)
  {
    while (OneCharLength(Otext + ptrf - 1) == -1)
    {
      ptrf--;
    }
    ptrf--;
  }

  DeleteFromText(ptrf, ptrb);
}

void CopyTheString()
{
  CURSOR_T *crst = GetCurrentCursor();
  if(crst->focus==0) return;
  else if(crst->focus==2){
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
  if(crst->focus==0) return;
  else if(crst->focus==2){
    PasteBarText();
    return;
  }

  int ptrf, ptrb;
  ptrf = min(crst->PTR_1, crst->PTR_2);
  ptrb = max(crst->PTR_1, crst->PTR_2);

  DeleteFromText(ptrf, ptrb);
  ptrb = ptrf;
  string newtext = GetStrFromClipBoard();
  AddStrToText(newtext, ptrf);
  free(newtext);
}