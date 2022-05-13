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

#define UNSAVED_FILE "unsaved_new_file"

typedef struct {
  Ustring filename;
  int TLength, UnderL;
  bool is_save;
  Unicode OriginText[0];
} * TEXT_FILE;

TEXT_FILE CurrentFile;

linkedlistADT FILES_LIST, FILE_NODE;

Ustring GetStrText();

int AddStrToText(Ustring newstr, int ptr);

int DeleteFromText(int ptr1, int ptr2);

static int AddStrToTextWithOutHis(Ustring newstr, int ptr) {}

static int DeleteFromTextWithOutHis(int ptr1, int ptr2);

bool InitFileSys() {
  FILES_LIST = NewLinkedList();
  FILE_NODE = NULL;
}

int GetFilesNum() { return LinkedListLen(FILES_LIST); }

bool ChangeCurrentFile(int ith) {
  FILE_NODE = (TEXT_FILE)ithNodeobj(FILES_LIST, ith);
}

bool OpenTheFile(Ustring filepath) {
  // TODO: different file sys
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    return FALSE;
  }

  Ustring ts;
  CurrentFile = (TEXT_FILE)malloc(sizeof(*(TEXT_FILE)NULL) +
                                  (INIT_FILEBUF_SIZE + 1) * sizeof(Unicode));
  FILE_NODE = InsertNode(FILES_LIST, NULL, CurrentFile);
  CurrentFile->TLength = 0, CurrentFile->UnderL = INIT_FILEBUF_SIZE;
  CurrentFile->filename = UNSAVED_FILE;
  ts = (Ustring)malloc((strlen(filepath) + 1) * sizeof(Unicode));
  strcpy(ts, filepath);
  if (filepath != "") CurrentFile->filename = ts;

  int ch;
  ts = (Ustring)malloc(2 * sizeof(Unicode));
  *(ts + 1) = '\0';
  while ((ch = fgetc(file)) != EOF) {
    *ts = (Unicode)ch;
    AddStrToTextWithOutHis(ts, CurrentFile->TLength);
  }

  return TRUE;
}

bool SaveTheFile(Ustring filepath) {
  if (filepath == L"") {
    filepath = CurrentFile->filename;
  }
  FILE *file;
  bool err = fopen_s(&file,filepath
  if (file == NULL) {
    return FALSE;
  }

  if (fputws((Ustring)CurrentFile->OriginText, file) < 0) {
    return FALSE;
  }

  CurrentFile->is_save = TRUE;
  return TRUE;
}

bool CloseTheFile(bool force) {
  if (!(CurrentFile->is_save || force)) {
    return FALSE;
  }

  free(CurrentFile->filename);
  DeleteCurrentNode(FILES_LIST, FILE_NODE);
  FILE_NODE = FILES_LIST->next;

  return TRUE;
}

//////////////////////////////////////////

typedef enum { ADD_HIS, DEL_HIS } HISTORY;

typedef struct DLIST_HIS *DLIST_HIS;
struct DLIST_HIS {
  HISTORY type;
  int ptrs;
  DLIST_HIS nxt, frt;
  Unicode content[0];
};

/////////// history /////////////

static DLIST_HIS NewHistoryList() {
  DLIST_HIS headN = addNewNode(NULL, 0);
  headN->ptrs = -1;
  return headN;
}

// return new head node, also new current node
static DLIST_HIS AddNewHistory(DLIST_HIS headN, DLIST_HIS currentN,
                               HISTORY type, int ptrs, Ustring content) {
  headN = reverseUntil(headN, currentN);
  headN = addNewNode(headN, strlen(content));
  headN->ptrs = ptrs;
  headN->type = type;
  strcpy((Ustring)headN->content, content);

  return headN;
}

// return new current node
static DLIST_HIS RedoHistory(DLIST_HIS headN, DLIST_HIS currentN) {
  if (currentN->frt == NULL) {
    return currentN;
  }
  currentN = currentN->frt;
  HISTORY undoT = currentN->type;
  if (undoT == ADD_HIS) {
    AddStrToTextWithOutHis(currentN->content, currentN->ptrs);
  } else if (undoT == DEL_HIS) {
    DeleteFromTextWithOutHis(currentN->ptrs,
                             currentN->ptrs + strlen(currentN->content));
  }

  return currentN;
}

// return new current node
static DLIST_HIS UndoHistory(DLIST_HIS headN, DLIST_HIS currentN) {
  if (currentN->ptrs == -1) {
    return currentN;
  }
  HISTORY undoT = currentN->type ^ 1;
  if (undoT == ADD_HIS) {
    AddStrToTextWithOutHis(currentN->content, currentN->ptrs);
  } else if (undoT == DEL_HIS) {
    DeleteFromTextWithOutHis(currentN->ptrs,
                             currentN->ptrs + strlen(currentN->content));
  }

  return currentN->nxt;
}

/////////////////////////////////

/////////////// dl mod ////////////////

// reverse all before current node
// return new head
static DLIST_HIS reverseUntil(DLIST_HIS headN, DLIST_HIS currentN) {
  if ((headN == NULL && currentN == NULL) || currentN == headN) {
    return NULL;
  }
  DLIST_HIS rstHead = currentN;
  currentN = currentN->frt;
  DLIST_HIS newHead = currentN, cur = currentN;
  currentN = currentN->frt;
  cur->frt = NULL;
  cur->type ^= 1;
  while (currentN != NULL) {
    cur->nxt = currentN;
    currentN = currentN->frt;
    cur->nxt->frt = cur;
    cur = cur->nxt;
    cur->type ^= 1;
  }
  cur->nxt = rstHead;

  return newHead;
}

// return new head
static DLIST_HIS addNewNode(DLIST_HIS headN, int contentL) {
  DLIST_HIS np = (DLIST_HIS)malloc(sizeof(*(DLIST_HIS)NULL) +
                                   (contentL + 1) * sizeof(Unicode));
  np->frt = NULL;
  np->nxt = headN;
  if (headN != NULL) {
    headN->frt = np;
  }
  return np;
}

// return new current.
// if current is head, return new head(maybe NULL);
// remember to change your head!
static DLIST_HIS freeOneNode(DLIST_HIS headN, DLIST_HIS currentN) {
  if (headN == currentN && headN == NULL) {
    return NULL;
  }
  DLIST_HIS np = NULL;
  if (currentN->frt != NULL) {
    np = currentN->frt;
    np->nxt = currentN->nxt;
  } else {
    np = currentN->nxt;
  }
  if (currentN->nxt != NULL) {
    currentN->nxt->frt = currentN->frt;
  }
  free(currentN);
  currentN = np;
  return currentN;
}

static DLIST_HIS nextNode(DLIST_HIS headN, DLIST_HIS currentN) {
  if (currentN == NULL && headN == NULL) {
    return NULL;
  }
  return currentN->nxt;
}

static DLIST_HIS lastNode(DLIST_HIS headN, DLIST_HIS currentN) {
  if (currentN == NULL && headN == NULL) {
    return NULL;
  }
  return currentN->frt;
}

//////////////////////////////////////