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
#include "sliper.h"

static int theme_num=2;
static string theme_tables[]={
    "Test",
    "Plain"
};

static BAR_THEME ui_themes[] = {
    {"Cyan", "Cyan", "Violet", "White", "Deep Cyan", "Hack", "White", "Black", "Blue", "Gray", "Black"},
    {"Black","White","Blue","Black","Gray","JetBrains Mono","White","Black","Blue","Light Gray","Black"},
};

static int bar_theme_id = 0;

static int FindThemeName(string name){
    string tpptr=name;
    while(*tpptr != '\0'){
        if((*tpptr-'a') < 0 ) *tpptr+=32;
        tpptr++;
    }
    *name-=32;
    for(int i=0;i<theme_num;i++){
        if(strcmp(theme_tables[i],name) == 0){
            return i;
        }
    }
    return -1;
}

void ChangeThemeByName(string name){
    int id=FindThemeName(name);
    if(id >=0) bar_theme_id=id;
}

string GetThemeName(){
    return theme_tables[bar_theme_id];
}


BAR_THEME GetBarTheme()
{
    return ui_themes[bar_theme_id];
}

static BAR_STATUS BarStatus = ORIGIN;

void SetBarStatus(BAR_STATUS status)
{
    BarStatus = status;
    if(BarStatus & FILE_EXT){
        preDrawFileExt();
    }else if (BarStatus & EDIT_EXT){
        preDrawEditExt();
    }else if (BarStatus & CONF_EXT){
        preDrawConfExt();
    }else{
        ResetNotiArea();
    }
    // (!(BarStatus & ~CLEAR_EXT))
}

BAR_STATUS GetBarStatus(){
    return BarStatus;
}


static long long  tptptp;
void UpdateAllBar()
{
    printf("updating %lld\n", tptptp++);
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
    UpdatePageInfo();
    PrintTheText(1);
    UpdateAllBar();

}
