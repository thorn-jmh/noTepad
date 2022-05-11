// FOR TEST
// DELETE BEFORE EDITOR

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef int bool;
typedef char *string;
// init
bool AddStrToClipBoard(string str);
string GetStrFromClipBoard();
// register

int main() {
  string a = "hihihi";
  printf("%s", a);
  int f = AddStrToClipBoard(a);
  if (!f) {
    printf("qaq");
  }
  string b = GetStrFromClipBoard();
  printf("%s", b);
  return 0;
}

string GetStrFromClipBoard() {
  string clipboardbuf;
  int t = 5;
  bool flag = FALSE;
  do {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag) {
    // Error("failed to open clipboard");
    return "";
  }
  if (!IsClipboardFormatAvailable(CF_TEXT)) {
    // Error("clipboard format invalid");
    return "";
  }

  HGLOBAL hmem = GetClipboardData(CF_TEXT);
  if (hmem == NULL) {
    return "";
  }
  clipboardbuf = (string)GlobalLock(hmem);
  GlobalUnlock(hmem);
  return clipboardbuf;
}

bool AddStrToClipBoard(string str) {
  int t = 5;
  bool flag = FALSE;
  do {
    flag = OpenClipboard(NULL);
    Sleep(50);
  } while (--t && !flag);

  if (!flag) {
    // Error("failed to open clipboard");
    return FALSE;
  }

  HGLOBAL hmem = GlobalAlloc(GHND, strlen(str) + 1);
  if (hmem == NULL) {
    // Error("clipboard: failed to alloc mem");
    return FALSE;
  }
  char *pmem = (char *)GlobalLock(hmem);
  memcpy(pmem, str, strlen(str) + 1);

  printf("%d", (int)*(str + strlen(str)));
  printf("%d", (int)*(pmem + strlen(str)));
  
  EmptyClipboard();
  SetClipboardData(CF_TEXT, hmem);
  CloseClipboard();
  GlobalFree(hmem);

  return TRUE;
}