#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "genlib.h"
#include "file.h"
#include "finder.h"

typedef struct
{
    size_t ptr;
} * REC_NODE;

static linkedlistADT FOUND_LIST, FOUND_NODE;
static int REC_LEN=-1, REC_NUM=-1;
static linkedlistADT HIGHLIGHT;

linkedlistADT GetCurrentHighlight()
{
    return CopyLinkedList(HIGHLIGHT);
}

void FindSubStr(string target)
{
    FreeLinkedList(FOUND_LIST);
    FreeLinkedList(HIGHLIGHT);

    if (!strcmp(target,"")) return;

    HIGHLIGHT = NewLinkedList();
    FOUND_LIST = NewLinkedList();
    string originText = GetStrText();
    string fstr = NULL;
    size_t optr = 0;
    while (fstr = strstr(originText + optr, target))
    {
        REC_NODE tp = (REC_NODE)malloc(sizeof(*(REC_NODE)NULL));
        tp->ptr = fstr - originText;
        optr = fstr - originText + strlen(target);
        InsertNode(FOUND_LIST, NULL, tp);

        //HIGH LIGHT
        size_t *tp2 = (size_t *)malloc(sizeof(size_t));
        *tp2 = tp->ptr;
        InsertNode(HIGHLIGHT, NULL, tp2);
        tp2 = (size_t *)malloc(sizeof(size_t));
        tp->ptr = optr;
        *tp2 = tp->ptr;
        InsertNode(HIGHLIGHT, NULL, tp2);
    }
    FOUND_NODE = FOUND_LIST->next;
    REC_LEN = LinkedListLen(FOUND_LIST);
    REC_NUM = 0;
}

void FreeFoundList()
{
    if(HIGHLIGHT == NULL) return;

    REC_LEN = REC_NUM = -1;
    FreeLinkedList(FOUND_LIST);
    FreeLinkedList(HIGHLIGHT);
    FOUND_LIST = NULL;
    HIGHLIGHT = NULL;
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
        return strlen(GetStrText());
    return ((REC_NODE)FOUND_NODE->dataptr)->ptr;
}

void NextFoundStr()
{
    if (FOUND_NODE == NULL)
        return;
    if (FOUND_NODE->next == NULL)
        FOUND_NODE = FOUND_LIST;
    FOUND_NODE = FOUND_NODE->next;
    REC_NUM = (REC_NUM + 1) % REC_LEN;
}

void LastFoundStr()
{
    if (REC_LEN == 0)
        return;
    REC_NUM = (REC_NUM - 1 + REC_LEN) % REC_LEN;
    FOUND_NODE = ithNode(FOUND_LIST, REC_NUM + 1);
}