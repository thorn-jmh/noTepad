#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "finder.h"
#include "editor.h"
#include "cursor.h"
#include "pages.h"
#include "topbar.h"
#include "bar.h"
#include "bar_callback.h"

////////////// BUTTONS //////////////////
extern BOTTON_T FILE_BOTTON;
extern BOTTON_T OPEN_BOTTON;
extern BOTTON_T CLOSE_BOTTON;
extern BOTTON_T NEW_BOTTON;
extern BOTTON_T SAVE_BOTTON;
extern BOTTON_T SAVEAS_BOTTON;

extern BOTTON_T EDIT_BOTTON;
extern BOTTON_T Undo_BOTTON;
extern BOTTON_T Redo_BOTTON;
extern BOTTON_T SEARCH_BOTTON;
extern BOTTON_T PASTE_BOTTON;
extern BOTTON_T COPY_BOTTON;
extern BOTTON_T CUT_BOTTON;
extern BOTTON_T DELETE_BOTTON;
extern BOTTON_T SELECT_BOTTON;

extern BOTTON_T CONF_BOTTON;
extern BOTTON_T FONT_BOTTON;
extern BOTTON_T SIZE_BOTTON;
extern BOTTON_T THEME_BOTTON;

extern BOTTON_T HELP_BOTTON;

extern BOTTON_T SET_BOTTON;
extern BOTTON_T NEXT_BOTTON;
extern BOTTON_T LAST_BOTTON;
extern BOTTON_T CANCEL_BOTTON;

///////////////////////////////////////

static void drawTextBar(double x, double y, double w, double h);
static void printBarText(double x, double y, double w, double h);
static void drawTopBar(double x, double y, double w, double h, BOTTON_T botton);
static bool checkMouse(double x, double y, double w, double h, bool key);
static void drawTopBarText(double x, double y, double w, double h, BOTTON_T botton);
static void StartDrawTopBar();
static void drawRectangle(double x, double y, double w, double h, int fillflag, string color);
static void drawTopBarFrame(double y, double w, double h);
static void drawTopBarList(double x, double y, double w, double h, int num, ...);
static void drawSearchStatus(double x, double y, double w, double h);

#define WPER 0.74  //width per button
#define MARGIN 0.1 // margin between topbar and text area

///////// Interface module drawers ////////////

// draw searchbar
void drawSearchBar()
{
    PAGE_T *pgt = GetPageInfo();
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - MARGIN / 2;
    double startX = pgt->TOPBAR.RB.X - 8 * WPER;

    drawSearchStatus(startX, startY, WPER, heightT);
    drawTextBar(startX + (4.0 / 3) * WPER, startY, 2 * WPER, heightT);
    drawTopBar(startX + (11.0 / 3) * WPER, startY, WPER, heightT, LAST_BOTTON);
    drawTopBar(startX + 5 * WPER, startY, WPER, heightT, NEXT_BOTTON);
    drawTopBar(startX + (19.0 / 3) * WPER, startY, WPER, heightT, CANCEL_BOTTON);
}

// drawsettingbar
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

// drawtopbars
void drawTopBars()
{
    PAGE_T *pgt = GetPageInfo();
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y;
    double startX = heightT;
    double Wper = WPER;
    drawTopBarFrame(startY, widthT, heightT);

    drawTopBar(startX, startY, Wper, heightT, FILE_BOTTON);
    drawTopBar(startX + Wper, startY, Wper, heightT, EDIT_BOTTON);
    drawTopBar(startX + 2 * Wper, startY, Wper, heightT, CONF_BOTTON);
    drawTopBar(startX + 3 * Wper, startY, Wper, heightT, HELP_BOTTON);
}

// draw file extend
void drawFileExt()
{
    BAR_THEME theme_tp = GetBarTheme();
    PAGE_T *pgt = GetPageInfo();
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT;
    double Wper = WPER;

    drawRectangle(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper, -5 * heightT, TRUE, theme_tp.textbk);
    drawTopBarList(startX, startY, 2 * Wper, heightT, 5,
                   NEW_BOTTON, OPEN_BOTTON, CLOSE_BOTTON, SAVE_BOTTON, SAVEAS_BOTTON);
}

// set file extend area
void preDrawFileExt()
{
    PAGE_T *pgt = GetPageInfo();
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT;
    double Wper = WPER;

    SetNotiArea(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper + MARGIN / 4, 5 * heightT + MARGIN / 4);
}

// draw edit extend
void drawEditExt()
{
    BAR_THEME theme_tp = GetBarTheme();
    PAGE_T *pgt = GetPageInfo();
    double Wper = WPER;
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT + Wper;
    drawRectangle(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper, -8 * heightT, TRUE, theme_tp.textbk);
    drawTopBarList(startX, startY, 2 * Wper, heightT, 8,
                   Undo_BOTTON, Redo_BOTTON, SEARCH_BOTTON, CUT_BOTTON, COPY_BOTTON, PASTE_BOTTON, DELETE_BOTTON, SELECT_BOTTON);
}

// set edit extend area
void preDrawEditExt()
{
    PAGE_T *pgt = GetPageInfo();
    double Wper = WPER;
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT + Wper;

    SetNotiArea(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper + MARGIN / 4, 8 * heightT + MARGIN / 4);
}

// draw config extend
void drawConfExt()
{
    BAR_THEME theme_tp = GetBarTheme();
    PAGE_T *pgt = GetPageInfo();
    double Wper = WPER;
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT + 2 * Wper;
    drawRectangle(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper, -3 * heightT, TRUE, theme_tp.textbk);
    drawTopBarList(startX, startY, 2 * Wper, heightT, 3,
                   FONT_BOTTON, SIZE_BOTTON, THEME_BOTTON);
}

// set config extend area
void preDrawConfExt()
{
    PAGE_T *pgt = GetPageInfo();
    double Wper = WPER;
    double widthT = pgt->TOPBAR.RB.X - pgt->TOPBAR.LT.X;
    double heightT = pgt->TOPBAR.LT.Y - pgt->TOPBAR.RB.Y;
    double startY = pgt->TOPBAR.LT.Y - heightT;
    double startX = heightT + 2 * Wper;

    SetNotiArea(startX - MARGIN / 4, startY + MARGIN / 4, 2 * Wper + MARGIN / 4, 3 * heightT + MARGIN / 4);
}

/////////////////////////////////////////////////

/////////// draw module functions   /////////////

// draw a rectangle
// will return pen to original place
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

// prepare draw topbar
static void StartDrawTopBar()
{
    BAR_THEME theme_tp = GetBarTheme();
    SetPenColor(theme_tp.text);
    SetStyle(1);
    SetFont(theme_tp.bar_font);
    SetPointSize(13);
    SetPenSize(2);
}

// draw text int topbar
// return pen to x,y
static void drawTopBarText(double x, double y, double w, double h, BOTTON_T botton)
{
    if (botton.name == NULL)
        return;

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
    else if (strlen(botton.hotkey) <= 0)
        hotk = "nil";
    else
        hotk = botton.hotkey + strlen(botton.hotkey) - 1;
    MovePen(x + w - actF / 2 - TextStringWidth(hotk), y - h / 2 - actF / 2);
    DrawTextString(hotk);
    MovePen(x, y);
}

// check if mouse in bar area
static bool checkMouse(double x, double y, double w, double h, bool key)
{
    MOUSE_T *mst = GetCurrentMouse();
    if (x < 0)
    {
        x = GetCurrentX();
        y = GetCurrentY() + h;
    }

    if (mst->X > x && mst->X <= x + w && mst->Y >= y - h && mst->Y < y)
        if (mst->event == BUTTON_UP)
            return TRUE;
        else if (!key)
            return TRUE;

    return FALSE;
}

// print text in textbar
static void printBarText(double x, double y, double w, double h)
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
    MovePen(x + actF / 2, y - h / 2 - actF / 2 + dctF / 2);
    double lenline = actF;
    string text = GetBarText();
    string Btext = GetBarText();
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
        if (crst->focus == 2 && (text - Btext) == ptrf && ptrf == ptrb)
            drawRectangle(-1, -1, -dctF / 3, htF, TRUE, theme_tp.cursor_color);
        if ((text - Btext) >= ptrf && (text - Btext) < ptrb)
            drawRectangle(-1, -1, one_width, htF, TRUE, theme_tp.text_select);
        else
            drawRectangle(-1, -1, one_width, htF, TRUE, theme_tp.textbk);

        PenMove(0, dctF);
        DrawTextString(tpstr);
        free(tpstr);

        if ((*text) == '\0')
        {
            break;
        }
        text += olen;
    }
}

// draw textbar
static void drawTextBar(double x, double y, double w, double h)
{
    BAR_THEME theme_tp = GetBarTheme();

    drawRectangle(x, y, w, -h, TRUE, theme_tp.textbk);
    drawRectangle(x, y, w, -h, FALSE, theme_tp.frame);
    if (checkMouse(x, y, w, h, FALSE))
    {
        MOUSE_T *mst = GetCurrentMouse();
        if (mst->event == BUTTON_DOWN)
        {
            CURSOR_T *crst = GetCurrentCursor();
            crst->PTR_1 = 0, crst->PTR_2 = strlen(GetBarText());
            crst->focus = 2;
        }
    }
    StartDrawTopBar();
    SetPenColor(theme_tp.text);
    printBarText(x, y, w, h);
}

// draw a top bar
// return pen to x,y
static void drawTopBar(double x, double y, double w, double h, BOTTON_T botton)
{
    StartDrawTopBar();

    BAR_THEME theme_tp = GetBarTheme();
    if (checkMouse(x, y, w, h, FALSE))
    {
        drawRectangle(x, y, w, -h, TRUE, theme_tp.high);
        if (checkMouse(x, y, w, h, TRUE))
        {
            if (botton.func != NULL)
                botton.func();
        }
    }
    else
        drawRectangle(x, y, w, -h, TRUE, theme_tp.barbk);
    SetPenSize(2);
    drawRectangle(x, y, w, -h, FALSE, theme_tp.frame);
    drawTopBarText(x, y, w, h, botton);
}

// draw search status
static void drawSearchStatus(double x, double y, double w, double h)
{
    StartDrawTopBar();
    BAR_THEME theme_tp = GetBarTheme();
    drawRectangle(x, y, w, -h, TRUE, theme_tp.barbk);
    SetPenSize(2);
    drawRectangle(x, y, w, -h, FALSE, theme_tp.frame);

    char tpstr[20];
    sprintf(tpstr, "%d/%d", NowFound() + 1, TotalFound());

    double actF = GetFontAscent();
    double dctF = GetFontDescent();
    MovePen(x + actF / 2, y - h / 2 - actF / 2 + dctF / 2);
    DrawTextString(tpstr);
    MovePen(x, y);
}

// draw topbar frame
static void drawTopBarFrame(double y, double w, double h)
{
    BAR_THEME theme_tp = GetBarTheme();
    SetPenSize(1);
    drawRectangle(0, y, w, -h, TRUE, theme_tp.barbk);
    SetPenSize(2);
    drawRectangle(0, y, w, -h, FALSE, theme_tp.frame);
}

// draw a list of bar
static void drawTopBarList(double x, double y, double w, double h, int num, ...)
{
    va_list bottons;
    va_start(bottons, num);

    for (int i = 0; i < num; i++)
    {
        BOTTON_T botton_tp = va_arg(bottons, BOTTON_T);
        drawTopBar(x, y - h * i, w, h, botton_tp);
    }
}
