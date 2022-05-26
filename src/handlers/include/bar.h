#ifndef _bar_h
#define _bar_h

#include "genlib.h"

typedef struct _BAR_THEME BAR_THEME;
struct _BAR_THEME
{
    string frame;
    string barbk;
    string selected;
    string text;
    string high;
    string bar_font;

    string textbk;
    string text_color;
    string text_high;
    string text_select;
    string cursor_color;
};

typedef enum
{
    ORIGIN = 0,       //0
    FILE_EXT = 1,     //1
    EDIT_EXT = 2,     //10
    CONF_EXT = 4,     //100
    SEARCH_EXT = 8,   //1000
    SETTING_EXT = 16, //10000
    CLEAR_EXT = 24,   // 11000
} BAR_STATUS;

void SetBarStatus(BAR_STATUS status);

BAR_STATUS GetBarStatus();


BAR_THEME GetBarTheme();
void ChangeThemeByName(string name);
string GetThemeName();



void UpdateAllBar();

void  mouse_test(int x, int y, int button, int event);

#endif