#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "genlib.h"
#include "file.h"
#include "linkedlist.h"

typedef struct
{
    size_t ptr;
} * REC_NODE;

static linkedlistADT FOUND_LIST, FOUND_NODE;
static int REC_LEN, REC_NUM;

void FindSubStr(Ustring target)
{
    FreeLinkedList(FOUND_LIST);
    FOUND_LIST = NewLinkedList();
    Ustring originText = GetStrText();
    Ustring fstr = NULL;
    size_t optr = 0;
    while (fstr = wcsstr(originText + optr, target))
    {
        REC_NODE tp = (REC_NODE)malloc(sizeof(*(REC_NODE)NULL));
        tp->ptr = fstr - originText;
        optr = tp->ptr + wcslen(target);
        InsertNode(FOUND_LIST, NULL, tp);
    }
    FOUND_NODE = FOUND_LIST->next;
    REC_LEN = LinkedListLen(FOUND_LIST);
    REC_NUM = 0;
}

int TotalFound()
{
    return REC_LEN;
}

int NowFound()
{
    return REC_NUM;
}

size_t WhereFoundStr()
{
    if (FOUND_NODE == NULL)
        return wcslen(GetStrText());
    return ((REC_NODE)FOUND_NODE->dataptr)->ptr;
}

void NextFoundStr()
{
    if (FOUND_NODE == NULL) return;
    if (FOUND_NODE->next == NULL)
        FOUND_NODE = FOUND_LIST;
    FOUND_NODE = FOUND_NODE->next;
    REC_NUM = (REC_NUM + 1) % REC_LEN;
}

void LastFoundStr()
{
    if (REC_LEN == 0) return;
    REC_NUM = (REC_NUM - 1 + REC_LEN) % REC_LEN;
    FOUND_NODE = ithNode(FOUND_LIST, REC_NUM + 1);
}