#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "clipboard.h"
#include "pages.h"
#include "topbar.h"
#include "bar.h"

static BOTTON_T FILE_BOTTON = {
    .name = "文件",
    .hotkey = "Alt+F",
    .func = NULL,
};
static BOTTON_T OPEN_BOTTON = {
    .name = "打开",
    .hotkey = "Ctrl+O",
    .func = NULL,
};
static BOTTON_T CLOSE_BOTTON = {
    .name = "关闭",
    .hotkey = "Ctrl+T",
    .func = NULL,
};
static BOTTON_T NEW_BOTTON = {
    .name = "新建",
    .hotkey = "Ctrl+N",
    .func = NULL,
};
static BOTTON_T SAVE_BOTTON = {
    .name = "保存",
    .hotkey = "Ctrl+S",
    .func = NULL,
};
static BOTTON_T SAVEAS_BOTTON = {
    .name = "另存为",
    .hotkey = NULL,
    .func = NULL,
};

static BOTTON_T EDIT_BOTTON = {
    .name = "编辑",
    .hotkey = "Alt+E",
    .func = NULL,
};
static BOTTON_T SEARCH_BOTTON = {
    .name = "搜索",
    .hotkey = "Ctrl+F",
    .func = NULL,
};
static BOTTON_T PASTE_BOTTON = {
    .name = "粘贴",
    .hotkey = "Ctrl+V",
    .func = NULL,
};
static BOTTON_T COPY_BOTTON = {
    .name = "复制",
    .hotkey = "Ctrl+C",
    .func = NULL,
};
static BOTTON_T CUT_BOTTON = {
    .name = "剪切",
    .hotkey = "Ctrl+X",
    .func = NULL,
};
static BOTTON_T DELETE_BOTTON = {
    .name = "删除",
    .hotkey = "Ctrl+D",
    .func = NULL,
};
static BOTTON_T SELECT_BOTTON = {
    .name = "全选",
    .hotkey = "Ctrl+A",
    .func = NULL,
};

static BOTTON_T CONF_BOTTON = {
    .name = "设置",
    .hotkey = "Alt+C",
    .func = NULL,
};
static BOTTON_T FONT_BOTTON = {
    .name = "字体",
    .hotkey = NULL,
    .func = NULL,
};
static BOTTON_T SIZE_BOTTON = {
    .name = "字号",
    .hotkey = NULL,
    .func = NULL,
};
static BOTTON_T THEME_BOTTON = {
    .name = "主题",
    .hotkey = NULL,
    .func = NULL,
};

static BOTTON_T HELP_BOTTON = {
    .name = "帮助",
    .hotkey = "Alt+H",
    .func = NULL,
};

static BOTTON_T SET_BOTTON = {
    .name = "设定",
    .hotkey = NULL,
    .func = NULL,
};
static BOTTON_T NEXT_BOTTON = {
    .name = "NEXT",
    .hotkey = NULL,
    .func = NULL,
};
static BOTTON_T LAST_BOTTON = {
    .name = "LAST",
    .hotkey = NULL,
    .func = NULL,
};
static BOTTON_T CANCEL_BOTTON = {
    .name = "关闭",
    .hotkey = NULL,
    .func = NULL,
};

static string BARTEXT = NULL;
#define MAX_BARTEXT 120 //Byte
#define WPER 0.74
#define MARGIN 0.1

//will return pen to original place
static void drawRectangle(double x, double y, double w, double h, int fillflag, string color)
{
    double cx, cy;
    string ccolor;
    cx = GetCurrentX();
    cy = GetCurrentY();
    ccolor = GetPenColor();

    if (x < 0 || y < 0)
        x = cx, y = cy;
    MovePen(x, y);
    SetPenColor(color);
    if (fillflag)
        StartFilledRegion(1);
    {
        DrawLine(0, h);
        DrawLine(w, 0);
        DrawLine(0, -h);
        DrawLine(-w, 0);
    }
    if (fillflag)
        EndFilledRegion();

    MovePen(cx, cy);
    SetPenColor(ccolor);
}

void StartDrawTopBar()
{
    BAR_THEME theme_tp = GetBarTheme();
    SetPenColor(theme_tp.text);
    SetStyle(1);
    SetFont(theme_tp.bar_font);
    SetPointSize(13);
    SetPenSize(2);
}

// return pen to x,y
void drawTopBarText(double x, double y, double w, double h, BOTTON_T botton)
{
    if (botton.name == NULL)
        return;

    BAR_THEME theme_tp = GetBarTheme();
    double actF = GetFontAscent();
    double dctF = GetFontDescent();
    MovePen(x + actF / 2, y - h / 2 - actF / 2 + dctF / 2);
    DrawTextString(botton.name);
    MovePen(x, y);

    if (botton.hotkey == NULL)
        return;

    string hotk = NULL;
    if (strstr(botton.hotkey, "Ctrl+") != NULL)
        hotk = botton.hotkey;
    else
        hotk = botton.hotkey + strlen(botton.hotkey) - 1;
    MovePen(x + w - actF / 2 - TextStringWidth(hotk), y - h / 2 - actF / 2);
    DrawTextString(hotk);
    MovePen(x, y);
}

bool checkMouse(double x, double y, double w, double h, bool key)
{
    MOUSE_T *mst = GetCurrentMouse();
    if (key && mst->event == BUTTON_UP)
        return TRUE;
    if (mst->X > x && mst->X <= x + w && mst->Y >= y - h && mst->Y < y)
        return TRUE;

    return FALSE;
}

// return pen to x,y
void drawTopBar(double x, double y, double w, double h, BOTTON_T botton)
{
    StartDrawTopBar();

    BAR_THEME theme_tp = GetBarTheme();
    if (checkMouse(x, y, w, h, FALSE))
    {
        drawRectangle(x, y, w, -h, TRUE, theme_tp.high);
        if (checkMouse(0, 0, 0, 0, TRUE))
        {
            botton.func();
        }
    }
    else
        drawRectangle(x, y, w, -h, TRUE, theme_tp.barbk);
    SetPenSize(2);
    drawRectangle(x, y, w, -h, FALSE, theme_tp.frame);
    drawTopBarText(x, y, w, h, botton);
}

void drawTopBarList(double x, double y, double w, double h, int num, ...)
{
    va_list bottons;
    va_start(bottons, h);

    for (int i = 0; i < num; i++)
    {
        BOTTON_T botton_tp = va_arg(bottons, BOTTON_T);
        drawTopBar(x, y - h * i, w, h, botton_tp);
    }
}

void drawTopBars()
{
    PAGE_T *pgt = GetPageInfo();
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y;
    double startX = heightT;
    double Wper = WPER;
    BAR_THEME theme_tp = GetBarTheme();
    SetPenSize(1);
    drawRectangle(0, startY, widthT, -heightT, TRUE, theme_tp.barbk);
    SetPenSize(2);
    drawRectangle(0, startY, widthT, -heightT, FALSE, theme_tp.frame);

    drawTopBar(startX, startY, Wper, heightT, FILE_BOTTON);
    drawTopBar(startX + Wper, startY, Wper, heightT, EDIT_BOTTON);
    drawTopBar(startX + 2 * Wper, startY, Wper, heightT, CONF_BOTTON);
    drawTopBar(startX + 3 * Wper, startY, Wper, heightT, HELP_BOTTON);
}

void InitBarText()
{
    BARTEXT = (string)malloc(MAX_BARTEXT * sizeof(char));
    *BARTEXT = '\0';
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    crst->PTR_1 = crst->PTR_2 = 0;
}

string GetBarText()
{
    return BARTEXT;
}

void DeleteBarText()
{
    CURSOR_T *crst = GetCurrentCursor();
    size_t ptrf, ptrb;
    ptrf = min(crst->PTR_1, crst->PTR_2);
    ptrb = max(crst->PTR_1, crst->PTR_2);
    if (ptrf == ptrb && ptrf != 0)
    {
        while (OneCharLength(*(BARTEXT + ptrf - 1)) == -1)
        {
            ptrf--;
        }
        ptrf--;
    }

    string tpstr = (string)malloc((strlen(BARTEXT + ptrb) + 1) * sizeof(char));
    strcpy(tpstr, BARTEXT + ptrb);
    strcpy(BARTEXT + ptrf, tpstr);
    free(tpstr);

    crst->PTR_1 = crst->PTR_2 = ptrf;
}

void InputBarText(string newstr)
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
        DeleteBarText();

    crst = GetCurrentCursor();
    ptrf = min(crst->PTR_1, crst->PTR_2);
    ptrb = max(crst->PTR_1, crst->PTR_2);

    string tpstr = (string)malloc((strlen(BARTEXT + ptrb) + 1) * sizeof(char));
    strcpy(tpstr, BARTEXT + ptrb);
    strcpy(BARTEXT + ptrf, newstr);
    strcpy(BARTEXT + ptrf + newlen, tpstr);
    free(tpstr);
    crst->PTR_1=crst->PTR_2=ptrf+newlen;
}

void CopyBarText()
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

void PasteBarText()
{
    string newstr = GetStrFromClipBoard();
    InputBarText(newstr);
    free(newstr);
}

void CloseBarText()
{
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 0;
    free(BARTEXT);
    BARTEXT = NULL;
}

void printBarText(double x, double y, double w, double h)
{
    BAR_THEME theme_tp = GetBarTheme();
    CURSOR_T *crst = GetCurrentCursor();
    size_t ptrf, ptrb;
    ptrf = min(crst->PTR_1, crst->PTR_2);
    ptrb = max(crst->PTR_1, crst->PTR_2);

    SetStyle(0);
    SetPenColor(theme_tp.text_color);
    double actF = GetFontAscent();
    double dctF = GetFontDescent();
    double htF = GetFontHeight();
    MovePen(x + actF / 2, y - h / 2 - actF / 2 + dctF);
    double lenline = actF;
    string text = BARTEXT;
    while (TRUE)
    {
        size_t olen = OneCharLength(*text);
        string tpstr = (string)malloc((olen + 1) * sizeof(char));
        memcpy(tpstr, text, olen);
        *(tpstr + olen) = '\0';

        double one_width = TextStringWidth(tpstr);
        lenline += one_width;
        if (lenline > w)
        {
            free(tpstr);
            break;
        }
        PenMove(0, -dctF);
        if (crst->focus==2 && (text - BARTEXT) == ptrf && ptrf == ptrb)
            drawRectangle(-1, -1, -dctF / 3, htF, TRUE, theme_tp.cursor_color);
        if ((text - BARTEXT) >= ptrf && (text - BARTEXT) < ptrb)
            drawRectangle(-1, -1, one_width, htF, TRUE, theme_tp.text_select);
        else
            drawRectangle(-1, -1, one_width, htF, TRUE, theme_tp.textbk);
        PenMove(0, dctF);
        DrawTextString(tpstr);
        free(tpstr);

        if((*text) == '\0'){
            break;
        }
        text += olen;
    }
}

void drawTextBar(double x, double y, double w, double h)
{
    BAR_THEME theme_tp = GetBarTheme();

    drawRectangle(x, y, w, -h, TRUE, theme_tp.textbk);
    drawRectangle(x, y, w, -h, FALSE, theme_tp.frame);
    StartDrawTopBar();
    SetPenColor(theme_tp.text);
    printBarText(x, y, w, h);
}

void drawSearchBar()
{
    PAGE_T *pgt = GetPageInfo();
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - MARGIN / 2;
    double startX = pgt->TOPBAR.RB.X - 6.5 * WPER;

    drawTextBar(startX, startY, 2 * WPER, heightT);
    drawTopBar(startX + (7.0 / 3) * WPER, startY, WPER, heightT, LAST_BOTTON);
    drawTopBar(startX + (11.0 / 3) * WPER, startY, WPER, heightT, NEXT_BOTTON);
    drawTopBar(startX + (5) * WPER, startY, WPER, heightT, CANCEL_BOTTON);
}

void drawSettingBar()
{
    PAGE_T *pgt = GetPageInfo();
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - MARGIN / 2;
    double startX = pgt->TOPBAR.RB.X - 5 * WPER;
    drawTextBar(startX, startY, 2 * WPER, heightT);
    drawTopBar(startX + (7.0 / 3) * WPER, startY, WPER, heightT, SET_BOTTON);
    drawTopBar(startX + (11.0 / 3) * WPER, startY, WPER, heightT, CANCEL_BOTTON);
}
