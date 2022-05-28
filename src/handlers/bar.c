#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "cursor.h"
#include "printer.h"
#include "sliper.h"
#include "pages.h"
#include "leftbar.h"
#include "topbar.h"
#include "bar.h"

static int FindThemeName(string name);

// Theme num
static int theme_num = 2;
// Theme name
static string theme_tables[] = {
    "Original",
    "Plain",};

// Themes
static BAR_THEME ui_themes[] = {
    {"Cyan", "Cyan", "Violet", "White", "Deep Cyan", "Consolas", "White", "Black", "Gray Blue", "Gray", "Black"},
    {"Black", "White", "Blue", "Black", "Gray", "Consolas", "White", "Black", "Yellow", "Light Gray", "Black"},
};

static int bar_theme_id = 0;          // current theme id
static BAR_STATUS BarStatus = ORIGIN; //initial bar status

// mouse lock
// lock if key status doesn't update
static bool MOUSE_MUX = FALSE;

// get mouse lock status
bool getmux()
{
    return MOUSE_MUX;
}
// lock mouse
void lockmux()
{
    MOUSE_MUX = TRUE;
}

// change theme by name
void ChangeThemeByName(string name)
{
    int id = FindThemeName(name);
    if (id >= 0)
        bar_theme_id = id;
}

// get current theme name
string GetThemeName()
{
    return theme_tables[bar_theme_id];
}

// get current theme info
BAR_THEME GetBarTheme()
{
    return ui_themes[bar_theme_id];
}

// set current bar status
void SetBarStatus(BAR_STATUS status)
{
    BarStatus = status;
    if (BarStatus & FILE_EXT)
    {
        preDrawFileExt();
    }
    else if (BarStatus & EDIT_EXT)
    {
        preDrawEditExt();
    }
    else if (BarStatus & CONF_EXT)
    {
        preDrawConfExt();
    }
    else
    {
        ResetNotiArea();
    }
    // (!(BarStatus & ~CLEAR_EXT))
}

// get current barstatus
BAR_STATUS GetBarStatus()
{
    return BarStatus;
}

// redraw all bars
void UpdateAllBar()
{
    drawTopBars();
    UpdateFileBars();

    if (BarStatus & FILE_EXT)
    {
        drawFileExt();
    }
    else if (BarStatus & EDIT_EXT)
    {
        drawEditExt();
    }
    else if (BarStatus & CONF_EXT)
    {
        drawConfExt();
    }

    if (BarStatus & SEARCH_EXT)
    {
        drawSearchBar();
    }
    else if (BarStatus & SETTING_EXT)
    {
        drawSettingBar();
    }
}

void MouseEvent(int x, int y, int button, int event)
{
    MOUSE_T *mst = GetCurrentMouse();
    CURSOR_T *crst = GetCurrentCursor();
    mst->X = ScaleXInches(x);
    mst->Y = ScaleYInches(y);
    mst->button = button;
    mst->event = event;
    MOUSE_MUX = FALSE;

    if (!InTopArea(mst->X, mst->Y) && !InNotiArea(mst->X, mst->Y) && event == BUTTON_DOWN)
    {
        BAR_STATUS status = GetBarStatus();
        status &= CLEAR_EXT;
        SetBarStatus(status);
    }

    DisplayClear();
    UpdatePageInfo();
    PrintTheText(1);
    UpdateAllBar();
    sliper();
    DrawSliper();
}

/////////////////////////////////

// find theme by name
static int FindThemeName(string name)
{
    string tpptr = name;
    while (*tpptr != '\0')
    {
        if ((*tpptr - 'a') < 0)
            *tpptr += 32;
        tpptr++;
    }
    *name -= 32;
    for (int i = 0; i < theme_num; i++)
    {
        if (strcmp(theme_tables[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}