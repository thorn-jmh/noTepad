#include"clipboard.h"

string GetStrFromClipBoard()
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

bool AddStrToClipBoard(string str)
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