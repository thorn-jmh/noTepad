#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "pages.h"
#include "cursor.h"
#include "file.h"
#include "bar.h"
#include "leftbar.h"

// basic height of a leftbar
#define HPER 1

static void drawFiletag(double y, double w, double h);
static void drawEnd(double y, double w);
static void drawFileBar(double y, double w, double h, string filename, bool high);
static void drawFileBarName(double y, double w, double h, string filename);
static void StartDrawFileBar();
static void drawRectangle(double x, double y, double w, double h, int fillflag, string color);


// Redraw leftbar area
void UpdateFileBars()
{
    PAGE_T *pgt = GetPageInfo();
    MOUSE_T *mst = GetCurrentMouse();
    double startY = pgt->LEFTBAR.LT.Y;
    double widthT = pgt->LEFTBAR.RB.X;
    double heightT = startY - widthT;
    int filenum = GetFilesNum();
    double Hper = min(heightT / filenum, 1);
    for (int i = 1; i <= filenum; i++)
    {
        string filename = GetFileName(i);
        if (mst->X <= widthT && mst->Y >= (startY - i * Hper) && mst->Y < (startY - (i - 1) * Hper))
        {
            drawFileBar(startY - (i - 1) * Hper, widthT, Hper, filename, TRUE);
            if (mst->event == BUTTON_DOWN && mst->button == LEFT_BUTTON)
            {
                ChangeCurrentFile(i);
            }
        }
        else
            drawFileBar(startY - (i - 1) * Hper, widthT, Hper, filename, FALSE);
    }
    drawFiletag(startY - (CurrentFileI() - 1) * Hper, widthT, Hper);
    drawEnd(startY - filenum * Hper, widthT);
}

/////////////////////////////////////

/////////// draw modules /////////////


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

// prepare to draw leftbar
static void StartDrawFileBar()
{
    BAR_THEME theme_tp = GetBarTheme();

    SetPenSize(2);
    SetStyle(1);
    SetFont(theme_tp.bar_font);
    SetPenColor(theme_tp.text);
    SetPointSize(13);
}

// draw text in a leftbar
static void drawFileBarName(double y, double w, double h, string filename)
{
    double actF = GetFontAscent();
    double dctF = GetFontDescent();
    MovePen(0 + w / 2 - actF / 2 + dctF, y - actF / 2);

    DrawAngleTextString(filename, h - actF);
}

// draw a leftbar
static void drawFileBar(double y, double w, double h, string filename, bool high)
{
    StartDrawFileBar();
    BAR_THEME theme_tp = GetBarTheme();
    if (!high)
        drawRectangle(0, y, w, -h, TRUE, theme_tp.barbk);
    else
        drawRectangle(0, y, w, -h, TRUE, theme_tp.high);
    drawRectangle(0, y, w, -h, FALSE, theme_tp.frame);
    drawFileBarName(y, w, h, filename);
}

// draw end tag at leftbar area
static void drawEnd(double y, double w)
{
    double dctF = GetFontDescent();
    BAR_THEME theme_tp = GetBarTheme();
    SetPenSize(1);
    drawRectangle(0, y, w, -dctF, TRUE, theme_tp.frame);
    drawRectangle(0, y - dctF, w, dctF - y, TRUE, theme_tp.barbk);
}

// fraw now file tag at leftbar area
static void drawFiletag(double y, double w, double h)
{
    double cx, cy, dctF;
    dctF = GetFontDescent();
    cx = GetCurrentX();
    cy = GetCurrentY();
    BAR_THEME theme_tp = GetBarTheme();

    MovePen(w, y);
    SetPenSize(6);
    SetPenColor(theme_tp.selected);
    DrawLine(0, dctF - h);

    MovePen(cx, cy);
}
