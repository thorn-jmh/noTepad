#include <Windows.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.h"
#include "genlib.h"
#include "cursor.h"
#include "printer.h"
#include "editor.h"
#include "file.h"
#include "bar_callback.h"
#include "keyboard.h"


// static bool isAltDown = FALSE;

static void leftkey();
static void rightkey();
static void upkey();
static void downkey();

static void Tab();
static void Enter();
static void Backspace();
static void Delete();


void CharInputEvent(char key)
{
    switch (key)
    {
    case 15:
        opencbk();
        return;
    case 20:
        closecbk();
        return;
    case 14:
        newcbk();
        return;
    case 19:
        savecbk();
        return;
    case 26:
        undocbk();
        return;
    case 25:
        redocbk();
        return;
    case 6:
        searchcbk();
        return;
    case 22:
        pastecbk();
        return;
    case 3:
        copycbk();
        return;
    case 24:
        cutcbk();
        return;
    case 4:
        deletecbk();
        return;
    case 1:
        selectcbk();
        return;

    default:
        break;
    }
    if (key < 32 && key >= 0)
        return;
    string s = (string)malloc(2 * sizeof(char));
    *s = key;
    *(s + 1) = 0;
    InputString(s);
    free(s);
}

void KeyBoardEvent(int key, int event)
{
    string text_string = GetStrText();
    if (key == VK_LEFT && event == KEY_DOWN)
        leftkey();
    if (key == VK_RIGHT && event == KEY_DOWN)
        rightkey();
    if (key == VK_UP && event == KEY_DOWN)
        upkey();
    if (key == VK_DOWN && event == KEY_DOWN)
        downkey();

    if (key == VK_ESCAPE && event == KEY_DOWN)
        cancelcbk();
    if (key == VK_RETURN && event == KEY_DOWN)
        Enter();
    if (key == VK_TAB && event == KEY_DOWN)
        Tab();
    if (key == VK_BACK && event == KEY_DOWN)
        Backspace();
    if (key == VK_DELETE && event == KEY_DOWN)
        Delete();

    // if (key == VK_MENU && event == KEY_DOWN)
    //     isAltDown = TRUE;
    // if (key == VK_MENU && event == KEY_UP)
    //     isAltDown = FALSE;
}

///////////////////////////////////////


static void leftkey()
{
    CURSOR_T *crst = GetCurrentCursor();
    size_t ptrf, ptrb;
    ptrf = min(crst->PTR_1, crst->PTR_2);
    ptrb = max(crst->PTR_1, crst->PTR_2);
    string Otext = NULL;
    if (crst->focus == 0)
        return;
    if (crst->focus == 1)
        Otext = GetStrText();
    else
        Otext = GetBarText();
    if (ptrf != ptrb)
    {
        ptrb = ptrf;
    }
    else if (ptrf != 0)
    {
        if (OneCharLength(*(Otext + ptrf - 1)) == 2)
        {
            ptrf--, ptrb--;
        }
        ptrf--, ptrb--;
    }

    crst->PTR_1 = ptrf;
    crst->PTR_2 = ptrb;
}

static void rightkey()
{
    CURSOR_T *crst = GetCurrentCursor();
    size_t ptrf, ptrb;
    ptrf = min(crst->PTR_1, crst->PTR_2);
    ptrb = max(crst->PTR_1, crst->PTR_2);
    string Otext = NULL;
    if (crst->focus == 0)
        return;
    if (crst->focus == 1)
        Otext = GetStrText();
    else
        Otext = GetBarText();
    if (ptrf != ptrb)
    {
        ptrf = ptrb;
    }
    else if (*(Otext + ptrb) != '\0')
    {
        if (OneCharLength(*(Otext + ptrb + 1)) == 2)
        {
            ptrf++, ptrb++;
        }
        ptrf++, ptrb++;
    }

    crst->PTR_1 = ptrf;
    crst->PTR_2 = ptrb;
}

static void upkey()
{
    CURSOR_T *crst = GetCurrentCursor();
    MOUSE_T *mst = GetCurrentMouse();
    MOUSE_T tpmst = *mst;
    double heightT = GetFontHeight() + GetFontDescent() / 6;

    if (crst->focus != 1)
        return;
    if (crst->PTR_1 != crst->PTR_2)
    {
        crst->PTR_2 = crst->PTR_1;
        PrintTheText(FALSE);
    }
    if (crst->Line <= 1)
        return;

    crst = GetCurrentCursor();
    LINE_T *lnt = GetCurrentLine();
    if (crst->Line > lnt->Fline + lnt->Cline - 1 || crst->Line <= lnt->Fline)
        FocusLine(crst->Line, 0);

    // get x,y
    PrintTheText(FALSE);
    crst = GetCurrentCursor();

    mst->X = crst->X;
    mst->Y = crst->Y + heightT;
    mst->button = LEFT_BUTTON;
    mst->event = BUTTON_DOWN;

    PrintTheText(FALSE);
    mst->event = BUTTON_UP;
    PrintTheText(FALSE);

    *mst = tpmst;
}

static void downkey()
{
    CURSOR_T *crst = GetCurrentCursor();
    MOUSE_T *mst = GetCurrentMouse();
    MOUSE_T tpmst = *mst;
    double heightT = GetFontHeight() + GetFontDescent() / 6;

    if (crst->focus != 1)
        return;
    if (crst->PTR_1 != crst->PTR_2)
    {
        crst->PTR_1 = crst->PTR_2;
        PrintTheText(FALSE);
    }

    crst = GetCurrentCursor();
    LINE_T *lnt = GetCurrentLine();
    if (crst->Line >= lnt->Tline)
        return;
    if (crst->Line >= lnt->Fline + lnt->Cline - 2 || crst->Line < lnt->Fline)
        FocusLine(crst->Line, 1);

    // get x,y
    PrintTheText(FALSE);
    crst = GetCurrentCursor();

    mst->X = crst->X;
    mst->Y = crst->Y - heightT;
    mst->button = LEFT_BUTTON;
    mst->event = BUTTON_DOWN;

    PrintTheText(FALSE);
    mst->event = BUTTON_UP;
    PrintTheText(FALSE);

    *mst = tpmst;
}

static void Tab()
{
    string s = (string)malloc(5 * sizeof(char));
    for (int i = 0; i < 4; i++)
        *(s + i) = ' ';
    *(s + 4) = '\0';
    InputString(s);
    free(s);
}

static void Enter()
{
    CURSOR_T *crst = GetCurrentCursor();
    if (crst->focus == 2)
    {
        nextcbk();
        return;
    }
    else if (crst->focus == 0)
        return;
    string s = (string)malloc(2 * sizeof(char));
    *s = '\n';
    *(s + 1) = 0;
    InputString(s);
    free(s);
}

static void Backspace()
{
    DeleteString(1);
}

static void Delete()
{
    DeleteString(2);
}
