#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cursor.h"
#include "genlib.h"
#include "linkedlist.h"
#include "finder.h"
#include "file.h"

#define MAX_TIME_INTERVAL 1             // max time interval between two correlated history
#define INIT_FILEBUF_SIZE 2048          // initial file buffer bytes
#define UNSAVED_NEW_FILE "new_file.txt" // new file name

// define type of history
typedef enum
{
  ADD_HIS,
  DEL_HIS
} HISTORY_T;

// define history double linked list cdt
typedef struct _DLIST_HIS *DLIST_HIS;
struct _DLIST_HIS
{
  HISTORY_T type;
  int order;
  time_t time;
  size_t ptrs;
  DLIST_HIS nxt, frt;
  char content[0];
};

// define file type
typedef struct
{
  size_t TLength, UnderL;
  size_t filenameL;
  DLIST_HIS history_head;
  bool is_save;
  char OriginText[0];
} * TEXT_FILE;

// files num and current file id
int FILES_NUM, FILE_I;

// file which is activate
// all works will be done in this file
static TEXT_FILE CurrentFile;

// opened files list and current file node
static linkedlistADT FILES_LIST, FILE_NODE;

// history of all files and current file's history
static DLIST_HIS HIS_LIST, HIS_NODE;

// a timestamp use to record history
static time_t timestp;

static void AddStrToTextWithOutHis(string newstr, size_t ptr);
static void DeleteFromTextWithOutHis(size_t ptr1, size_t ptr2);
static void UpdateFileSys(bool DelHis);
static void readTextFromFile(stream file);
static string updateFilePath(string filepath, bool DelHis);

static DLIST_HIS NewHisList();
static void AddNewHistory(HISTORY_T type, int ptrs, string content);

//// history doublelinked-list module ////
static void freeHisList(DLIST_HIS headN);
static void deleteUntil(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS addNewNode(DLIST_HIS headN, DLIST_HIS currentN,
                            string content);
static void deleteOneNode(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS nextNode(DLIST_HIS headN, DLIST_HIS currentN);
static DLIST_HIS lastNode(DLIST_HIS headN, DLIST_HIS currentN);
/////////////////////////////////////////

// init file system
void InitFileSys()
{
  timestp = time(NULL);
  FILES_LIST = NewLinkedList();
  FILE_NODE = NULL;
  FILES_NUM = FILE_I = 0;
}

// return current file node id
int CurrentFileI()
{
  return FILE_I;
}

// get opened files num
int GetFilesNum()
{
  if (LinkedListLen(FILES_LIST) != FILES_NUM)
    Error("file system error!!!");
  return FILES_NUM;
}

// change current file
void ChangeCurrentFile(int ith)
{
  FILE_I = ith;
  FILE_NODE = ithNode(FILES_LIST, ith);
  UpdateFileSys(TRUE);

  CURSOR_T *crst = GetCurrentCursor();
  size_t ptr = 0;
  string text = CurrentFile->OriginText;
  while (*text != '\0')
  {
    ptr++;
    text++;
  }
  crst->PTR_1 = crst->PTR_2 = ptr;
}

// get ith file's filename
string GetFileName(int ith)
{
  linkedlistADT tpNode = FILES_LIST;
  while (ith--)
  {
    tpNode = tpNode->next;
  }
  TEXT_FILE tpFile = (TEXT_FILE)(tpNode->dataptr);
  string ptr = tpFile->OriginText + tpFile->filenameL + tpFile->UnderL;
  int tp = tpFile->filenameL;
  while ((*ptr != '\0' || *ptr != '\\' || *ptr != '/' || *ptr != ':') && tp)
  {
    tp--;
    ptr--;
  }
  return ++ptr;
}

// get raw text of current file
string GetStrText() { return CurrentFile->OriginText; }

// add string to current file
void AddStrToText(string newstr, size_t ptr)
{
  AddNewHistory(ADD_HIS, ptr, newstr);
  AddStrToTextWithOutHis(newstr, ptr);
}

// delete string from current file
void DeleteFromText(size_t ptr1, size_t ptr2)
{
  //从原文本copy下删除部分
  string tpstr = (string)malloc((ptr2 - ptr1 + 1) * sizeof(char));
  string cstr = CurrentFile->OriginText + ptr1;
  memcpy(tpstr, cstr, (ptr2 - ptr1) * sizeof(char));
  *(tpstr + ptr2 - ptr1) = '\0';
  AddNewHistory(DEL_HIS, ptr1, tpstr);
  free(tpstr);
  DeleteFromTextWithOutHis(ptr1, ptr2);
}

// open a file
bool OpenTheFile(string filepath)
{
  if (filepath == NULL)
  {
    filepath = UNSAVED_NEW_FILE;
  }

  stream file = NULL;
  file=fopen(filepath,"r");
  // int err = fopen_s(&file, filepath, "r");
  // if (err == 2)
  // {
  //   fopen_s(&file, filepath, "w");
  //   if (file != NULL)
  //     fclose(file);
  //   err = fopen_s(&file, filepath, "r");
  // }
  // if (err != 0)
  //   return FALSE;
  //读入
  CurrentFile = (TEXT_FILE)malloc(sizeof(*(TEXT_FILE)NULL) +
                                  (INIT_FILEBUF_SIZE + 1) * sizeof(char) +
                                  (strlen(filepath) + 1) * sizeof(char));

  FILE_NODE = InsertNode(FILES_LIST, NULL, CurrentFile);
  CurrentFile->TLength = 0, CurrentFile->UnderL = INIT_FILEBUF_SIZE;
  CurrentFile->history_head = NewHisList();
  CurrentFile->is_save = TRUE;
  *(CurrentFile->OriginText) = '\0';
  string temp = updateFilePath(filepath, TRUE);
  free(temp);

  readTextFromFile(file);
  fclose(file);
  FILES_NUM++;
  FILE_I = FILES_NUM;
  return TRUE;
}

// save a file
// Do not change filepath if pass NULL
bool SaveTheFile(string filepath)
{
  if (filepath == NULL)
  {
    filepath = CurrentFile->OriginText + CurrentFile->UnderL + 1;
  }
  string tpptr = updateFilePath(filepath, FALSE);
  free(tpptr);
  stream file = NULL;
  file=fopen(filepath,"w");
  // int err = fopen_s(&file, filepath, "w");
  // if (err != 0)
  //   return FALSE;

  if (fputs(CurrentFile->OriginText, file) < 0)
  {
    return FALSE;
  }

  fclose(file);
  CurrentFile->is_save = TRUE;
  return TRUE;
}

// close current file
// - if force is false
// - will not close if current file isn't saved
bool CloseTheFile(bool force)
{
  if (!(CurrentFile->is_save || force))
  {
    return FALSE;
  }

  freeHisList(HIS_LIST);
  HIS_LIST = NULL;
  DeleteCurrentNode(FILES_LIST, FILE_NODE);
  FILES_NUM--;
  FILE_I = (FILE_I == 1 && FILES_NUM >= FILE_I) ? FILE_I : FILE_I - 1;
  if (FILE_I == 0)
  {
    OpenTheFile(NULL);
  }
  else
  {
    ChangeCurrentFile(FILE_I);
  }

  UpdateFileSys(TRUE);

  return TRUE;
}

// redo history
void RedoHistory()
{
  if (HIS_NODE->frt == HIS_LIST)
    return;

  do
  {
    HIS_NODE = HIS_NODE->frt;
    HISTORY_T undoT = HIS_NODE->type;
    if (undoT == ADD_HIS)
    {
      AddStrToTextWithOutHis(HIS_NODE->content, HIS_NODE->ptrs);
    }
    else if (undoT == DEL_HIS)
    {
      DeleteFromTextWithOutHis(HIS_NODE->ptrs,
                               HIS_NODE->ptrs + strlen(HIS_NODE->content));
    }
  } while (HIS_NODE->frt != HIS_LIST && HIS_NODE->frt->order == HIS_NODE->order);
}

// undo
void UndoHistory()
{
  if (HIS_NODE->nxt == NULL)
    return;
  do
  {
    HISTORY_T undoT = HIS_NODE->type ^ 1;
    if (undoT == ADD_HIS)
    {
      AddStrToTextWithOutHis(HIS_NODE->content, HIS_NODE->ptrs);
    }
    else if (undoT == DEL_HIS)
    {
      DeleteFromTextWithOutHis(HIS_NODE->ptrs,
                               HIS_NODE->ptrs + strlen(HIS_NODE->content));
    }
    HIS_NODE = HIS_NODE->nxt;
  } while (HIS_NODE->nxt != NULL && HIS_NODE->order == HIS_NODE->frt->order);
}

//////////////////////////////////////////

/////////////////  R&W FILE  /////////////////

static void AddStrToTextWithOutHis(string newstr, size_t ptr)
{
  CurrentFile->is_save = FALSE;
  size_t len = strlen(newstr);
  string filepath = updateFilePath(NULL, FALSE);
  while (CurrentFile->TLength + len > CurrentFile->UnderL)
  {
    CurrentFile = (TEXT_FILE)realloc(
        CurrentFile, sizeof(*(TEXT_FILE)NULL) +
                         (CurrentFile->UnderL * 2 + 1) * sizeof(char) +
                         (CurrentFile->filenameL + 1) * sizeof(char));
    CurrentFile->UnderL *= 2;
  }
  string tpstr = updateFilePath(filepath, FALSE);
  free(tpstr);
  free(filepath);

  // add new string
  string tptr = CurrentFile->OriginText + ptr;
  string cstr =
      (string)malloc((CurrentFile->TLength - ptr + 1) * sizeof(char));
  strcpy(cstr, tptr);
  strcpy(tptr, newstr);
  tptr = tptr + len;
  strcpy(tptr, cstr);
  free(cstr);
  CurrentFile->TLength += len;

  CURSOR_T *cursor = GetCurrentCursor();
  cursor->PTR_1 = cursor->PTR_2 = ptr + len;
}

static void DeleteFromTextWithOutHis(size_t ptr1, size_t ptr2)
{
  CurrentFile->is_save = FALSE;
  string tptr = CurrentFile->OriginText + ptr2;
  string cstr =
      (string)malloc((CurrentFile->TLength - ptr2 + 1) * sizeof(char));
  strcpy(cstr, tptr);
  tptr = CurrentFile->OriginText + ptr1;
  strcpy(tptr, cstr);
  free(cstr);
  CurrentFile->TLength -= ptr2 - ptr1;

  CURSOR_T *cursor = GetCurrentCursor();
  cursor->PTR_1 = cursor->PTR_2 = ptr1;
}

static void readTextFromFile(stream file)
{
  char ch;
  string ts = (string)malloc(2 * sizeof(char));
  *(ts + 1) = '\0';
  CURSOR_T *crst = GetCurrentCursor();
  crst->focus = 1;
  crst->PTR_1 = crst->PTR_2 = 0;
  while ((ch = fgetc(file)) != EOF)
  {
    *ts = ch;
    AddStrToTextWithOutHis(ts, CurrentFile->TLength);
  }
  free(ts);
}

////////////////////////////////////////////

//////////////// file system /////////////////

static void UpdateFileSys(bool DelHis)
{
  FreeFoundList();
  if (FILE_NODE == NULL)
  {
    HIS_LIST = HIS_NODE = NULL;
    CurrentFile = NULL;
  }
  CurrentFile = (TEXT_FILE)FILE_NODE->dataptr;
  if (DelHis)
    deleteUntil(HIS_LIST, HIS_NODE);
  HIS_LIST = CurrentFile->history_head;
  HIS_NODE = HIS_LIST->nxt;
}

//please receive the return value and free it
//btw, update filesystem
static string updateFilePath(string filepath, bool DelHis)
{

  if (filepath != NULL)
  {
    size_t len = strlen(filepath);
    if (len != CurrentFile->filenameL)
    {
      CurrentFile = (TEXT_FILE)realloc(
          CurrentFile, sizeof(*(TEXT_FILE)NULL) +
                           (CurrentFile->UnderL + 1) * sizeof(char) +
                           (len + 1) * sizeof(char));
      FILE_NODE->dataptr = (void *)CurrentFile;
      UpdateFileSys(DelHis);
    }
    CurrentFile->filenameL = len;
    memcpy(CurrentFile->OriginText + CurrentFile->UnderL + 1, filepath, len + 1);
  }

  string ret = (string)malloc(CurrentFile->filenameL + 1);
  memcpy(ret, CurrentFile->OriginText + CurrentFile->UnderL + 1, CurrentFile->filenameL + 1);
  return ret;
}

/////////// history /////////////

// a new doublelinked-list!
static DLIST_HIS NewHisList()
{
  DLIST_HIS np = (DLIST_HIS)malloc(sizeof(*(DLIST_HIS)NULL));
  np->frt = NULL;
  np->nxt = NULL;
  np->order = -1;
  np->time = -1;
  addNewNode(np, NULL, "");
  return np;
}

// return new head node, also new current node
static void AddNewHistory(HISTORY_T type, int ptrs, string content)
{
  if (strlen(content) <= 0)
    return;
  HIS_NODE = addNewNode(HIS_LIST, HIS_NODE, content);
  HIS_NODE->ptrs = ptrs;
  HIS_NODE->type = type;
}
/////////////////////////////////

/////////////// dl mod ////////////////

// delete all before current node
static void deleteUntil(DLIST_HIS headN, DLIST_HIS currentN)
{
  if (headN == NULL)
    return;
  while (headN->nxt != currentN && headN->nxt->nxt != NULL)
  {
    deleteOneNode(headN, headN->nxt);
  }
}

// Free a dlist
static void freeHisList(DLIST_HIS headN)
{
  deleteUntil(headN, NULL);
  free(headN->nxt);
  free(headN);
}

// return new node
static DLIST_HIS addNewNode(DLIST_HIS headN, DLIST_HIS currentN,
                            string content)
{
  size_t len = strlen(content);
  DLIST_HIS np =
      (DLIST_HIS)malloc(sizeof(*(DLIST_HIS)NULL) + (len + 1) * sizeof(char));
  strcpy(np->content, content);

  deleteUntil(headN, currentN);
  headN->nxt = np;
  np->frt = headN;
  np->nxt = currentN;
  if (currentN == NULL)
  {
    np->order = 0;
    np->time = -1;
    return np;
  }
  currentN->frt = np;

  time_t ttt = time(&timestp);
  np->time = ttt;
  np->order = currentN->order + 1;
  if (np->time - currentN->time < MAX_TIME_INTERVAL)
  {
    np->order--;
  }
  return np;
}

// after delete,
// currentN is invalid
static void deleteOneNode(DLIST_HIS headN, DLIST_HIS currentN)
{
  currentN->frt->nxt = currentN->nxt;
  if (currentN->nxt != NULL)
    currentN->nxt->frt = currentN->frt;

  free(currentN);
}

static DLIST_HIS nextNode(DLIST_HIS headN, DLIST_HIS currentN)
{
  return currentN->nxt;
}

static DLIST_HIS lastNode(DLIST_HIS headN, DLIST_HIS currentN)
{
  return currentN->frt;
}

//////////////////////////////////////