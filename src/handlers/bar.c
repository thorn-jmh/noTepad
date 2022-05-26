#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "pages.h"
#include "file.h"
#include "leftbar.h"
#include "topbar.h"
#include "bar.h"


static BAR_THEME ui_themes[] = {
    {"Cyan", "Cyan", "Violet", "White", "Deep Cyan", "Hack", "White", "Black", "Blue", "Gray", "Black"},
};

static int bar_theme_id = 0;

BAR_THEME GetBarTheme()
{
    return ui_themes[bar_theme_id];
}

static BAR_STATUS BarStatus = ORIGIN;

void SetBarStatus(BAR_STATUS status)
{
    BarStatus = status;
}

BAR_STATUS GetBarStatus(){
    return BarStatus;
}

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


void  mouse_test(int x, int y, int button, int event){
    MOUSE_T *mst=GetCurrentMouse();
    mst->X=ScaleXInches(x);
    mst->Y=ScaleYInches(y);
    mst->button=button;
    mst->event=event;
    DisplayClear();
    UpdateAllBar();
    PrintTheText(1);
}
