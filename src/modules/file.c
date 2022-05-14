#include "file.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"
#include "genlib.h"
#include "linkedlist.h"

CURSOR_T GetCursorStatus();

#define INIT_FILEBUF_SIZE 2048

#define UNSAVED_NEW_FILE "new_file"

typedef enum { ADD_HIS, DEL_HIS } HISTORY_T;

typedef struct DLIST_HIS* DLIST_HIS;
struct DLIST_HIS {
  HISTORY_T type;
  size_t ptrs;
  DLIST_HIS nxt, frt;
  Unicode content[0];
};

typedef struct {
  size_t TLength, UnderL;
  string filename;
  DLIST_HIS* history_head;
  bool is_save;
  Unicode OriginText[0];
} * TEXT_FILE;

TEXT_FILE CurrentFile;

linkedlistADT FILES_LIST, FILE_NODE;

DLIST_HIS HIS_LIST, HIS_NODE;

static void AddStrToTextWithOutHis(Ustring newstr, size_t ptr);
static void DeleteFromTextWithOutHis(size_t ptr1, size_t ptr2);
static void UpdateFileSys();
static void readTextFromFileUTF8(stream file);
static void readTextFromFile(stream file);

static DLIST_HIS NewHisList();
static DLIST_HIS NewHisList();

//// history doublelinked-list module ////
static void freeHisList(DLIST_HIS headN);
static void deleteUntil(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS addNewNode(DLIST_HIS headN, DLIST_HIS currentN,
                            Ustring content);
static void deleteOneNode(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS nextNode(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS lastNode(DLIST_HIS headN, DLIST_HIS currentN);
/////////////////////////////////////////

Ustring GetStrText() { return CurrentFile->OriginText; }

void AddStrToText(Ustring newstr, size_t ptr) {
  AddNewHistory(ADD_HIS, ptr, newstr);
  AddStrToTextWithOutHis(newstr, ptr);
}

void DeleteFromText(size_t ptr1, size_t ptr2) {
  Ustring tpstr = (Ustring)malloc((ptr2 - ptr1 + 1) * sizeof(Unicode));
  Ustring cstr = CurrentFile->OriginText + ptr1;
  wcscpy_s(tpstr, ptr2 - ptr1, cstr);
  *(tpstr + ptr2 - ptr1) = L'\0';
  AddNewHistory(DEL_HIS, ptr1, tpstr);
  free(tpstr);
  DeleteFromTextWithOutHis(ptr1, ptr2);
}

bool InitFileSys() {
  FILES_LIST = NewLinkedList();
  FILE_NODE = NULL;
}

int GetFilesNum() { return LinkedListLen(FILES_LIST); }

bool ChangeCurrentFile(int ith) {
  FILE_NODE = (TEXT_FILE)ithNodeobj(FILES_LIST, ith);
  UpdateFileSys();
}

bool OpenTheFile(string filepath, bool utf8) {
  if (filepath == "") {
    filepath = UNSAVED_NEW_FILE;
  }

  stream file = NULL;
  if (utf8) {
    fopen_S(&file, filepath, "r, ccs=uft-8");
  } else {
    fopen_s(&file, filepath, "r");
  }
  if (file == NULL) {
    return FALSE;
  }

  Ustring ts;
  CurrentFile = (TEXT_FILE)malloc(sizeof(*(TEXT_FILE)NULL) +
                                  (INIT_FILEBUF_SIZE + 1) * sizeof(Unicode));

  FILE_NODE = InsertNode(FILES_LIST, NULL, CurrentFile);
  CurrentFile->TLength = 0, CurrentFile->UnderL = INIT_FILEBUF_SIZE;
  int len = strlen(filepath) + 1;
  string filename = (string)malloc(len * sizeof(char));
  strcpy_s(filename, len, filepath);
  CurrentFile->filename = filename;
  CurrentFile->history_head = NewHisList();
  UpdateFileSys();

  if (utf8) {
    readTextFromFileUTF8(file);
  } else {
    readTextFromFile(file);
  }

  fclose(file);
  return TRUE;
}

bool SaveTheFile(string filepath) {
  if (filepath == "") {
    filepath = CurrentFile->filename;
  }
  stream file = NULL;
  fopen_s(&file, filepath, "w, ccs=utf-8");
  if (file == NULL) {
    return FALSE;
  }

  if (fputws(CurrentFile->OriginText, file) < 0) {
    return FALSE;
  }

  fclose(file);
  CurrentFile->is_save = TRUE;
  return TRUE;
}

bool CloseTheFile(bool force) {
  if (!(CurrentFile->is_save || force)) {
    return FALSE;
  }

  free(CurrentFile->filename);
  freeHisList(HIS_LIST);
  DeleteCurrentNode(FILES_LIST, FILE_NODE);
  FILE_NODE = FILES_LIST->next;
  UpdateFileSys();

  return TRUE;
}

void RedoHistory() {
  if (HIS_NODE->frt == HIS_LIST) return;
  HIS_NODE = HIS_NODE->frt;
  HISTORY_T undoT = HIS_NODE->type;
  if (undoT == ADD_HIS) {
    AddStrToTextWithOutHis(HIS_NODE->content, HIS_NODE->ptrs);
  } else if (undoT == DEL_HIS) {
    DeleteFromTextWithOutHis(HIS_NODE->ptrs,
                             HIS_NODE->ptrs + wcslen(HIS_NODE->content));
  }
}

void UndoHistory() {
  if (HIS_NODE == NULL || HIS_NODE->nxt == NULL) return;
  HISTORY_T undoT = HIS_NODE->type ^ 1;
  if (undoT == ADD_HIS) {
    AddStrToTextWithOutHis(HIS_NODE->content, HIS_NODE->ptrs);
  } else if (undoT == DEL_HIS) {
    DeleteFromTextWithOutHis(HIS_NODE->ptrs,
                             HIS_NODE->ptrs + strlen(HIS_NODE->content));
  }
  HIS_NODE = HIS_NODE->nxt;
}


//////////////////////////////////////////


/////////////////  R&W FILE  /////////////////

static void AddStrToTextWithOutHis(Ustring newstr, size_t ptr) {
  size_t len = wcslen(newstr);

  while (CurrentFile->TLength + len > CurrentFile->UnderL) {
    CurrentFile = (TEXT_FILE)realloc(
        CurrentFile, sizeof(*(TEXT_FILE)NULL) +
                         (CurrentFile->UnderL * 2 + 1) * sizeof(Unicode));
    CurrentFile->UnderL *= 2;
  }
  FILE_NODE->dataptr = (void*)CurrentFile;

  Ustring tptr = CurrentFile->OriginText + ptr;
  Ustring cstr =
      (Ustring)malloc((CurrentFile->TLength - ptr + 1) * sizeof(Unicode));
  wcscpy_s(cstr, CurrentFile->TLength - ptr + 1, tptr);
  wcscpy_s(tptr, len, newstr);
  tptr = tptr + len;
  wcscpy_s(tptr, CurrentFile->TLength - ptr + 1, cstr);
  free(cstr);
  CurrentFile->TLength += len;

  CURSOR_T* cursor = GetCurrentCursor();
  cursor->PTR_1 = cursor->PTR_2 = ptr + len;
}

static void DeleteFromTextWithOutHis(size_t ptr1, size_t ptr2) {
  Ustring tptr = CurrentFile->OriginText + ptr2;
  Ustring cstr =
      (Ustring)malloc((CurrentFile->TLength - ptr2 + 1) * sizeof(Unicode));
  wcscpy_s(cstr, CurrentFile->TLength - ptr2 + 1, tptr);
  tptr = CurrentFile->OriginText + ptr1;
  wcscpy_s(tptr, CurrentFile->TLength - ptr2 + 1, cstr);
  free(cstr);
  CurrentFile->TLength -= ptr2 - ptr1;

  CURSOR_T* cursor = GetCurrentCursor();
  cursor->PTR_1 = cursor->PTR_2 = ptr1;
}

static void readTextFromFileUTF8(stream file) {
  wint_t ch;
  Ustring ts = (Ustring)malloc(2 * sizeof(Unicode));
  *(ts + 1) = L'\0';
  while ((ch = fgetwc(file)) != WEOF) {
    *ts = (Unicode)ch;
    AddStrToTextWithOutHis(ts, CurrentFile->TLength);
  }
}

static void readTextFromFile(stream file) {
  int ch;
  Ustring ts = (Ustring)malloc(2 * sizeof(Unicode));
  *(ts + 1) = L'\0';
  while ((ch = fgetc(file)) != EOF) {
    *ts = (Unicode)ch;
    AddStrToTextWithOutHis(ts, CurrentFile->TLength);
  }
}

////////////////////////////////////////////


//////////////// file system /////////////////


static void UpdateFileSys() {
  if (FILE_NODE == NULL) {
    HIS_LIST = HIS_NODE = NULL;
    CurrentFile = NULL;
  }
  CurrentFile = (TEXT_FILE)FILE_NODE;
  deleteUntil(HIS_LIST, HIS_NODE);
  HIS_LIST = CurrentFile->history_head;
  HIS_NODE = HIS_LIST->nxt;
}


/////////// history /////////////

// a new doublelinked-list!
static DLIST_HIS NewHisList() {
  DLIST_HIS np = (DLIST_HIS)malloc(sizeof(*(DLIST_HIS)NULL));
  np->frt = NULL;
  np->nxt = NULL;
}

// return new head node, also new current node
static void AddNewHistory(HISTORY_T type, int ptrs, Ustring content) {
  HIS_NODE = addNewNode(HIS_LIST, HIS_NODE, content);
  HIS_NODE->ptrs = ptrs;
  HIS_NODE->type = type;
}
/////////////////////////////////

/////////////// dl mod ////////////////

// delete all before current node
static void deleteUntil(DLIST_HIS headN, DLIST_HIS currentN) {
  DLIST_HIS tp = NULL;
  while (headN->nxt != currentN && headN->nxt != NULL) {
    deleteOneNode(headN, headN->nxt);
    free(tp);
  }
}

// Free a dlist
static void freeHisList(DLIST_HIS headN) {
  deleteUntil(headN, NULL);
  free(headN);
}

// return new node
static DLIST_HIS addNewNode(DLIST_HIS headN, DLIST_HIS currentN,
                            Ustring content) {
  size_t len = wcslen(content);
  DLIST_HIS np =
      (DLIST_HIS)malloc(sizeof(*(DLIST_HIS)NULL) + (len + 1) * sizeof(Unicode));
  wcscpy_s(np->content, len, content);
  *(np->content + len) = L'\0';

  deleteUntil(headN, currentN);
  headN->nxt = np;
  if (currentN != NULL) currentN->frt = np;
  np->frt = headN;
  np->nxt = currentN;
  return np;
}

// after delete,
// currentN is invalid
static void deleteOneNode(DLIST_HIS headN, DLIST_HIS currentN) {
  currentN->frt->nxt = currentN->nxt;
  if (currentN->nxt != NULL) currentN->nxt->frt = currentN->frt;

  free(currentN);
}

static DLIST_HIS nextNode(DLIST_HIS headN, DLIST_HIS currentN) {
  return currentN->nxt;
}

static DLIST_HIS lastNode(DLIST_HIS headN, DLIST_HIS currentN) {
  return currentN->frt;
}

//////////////////////////////////////