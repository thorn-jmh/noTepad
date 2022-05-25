#ifndef _bartheme_h
#define _bartheme_h

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
    ORIGIN,
    FILE_EXT,
    EDIT_EXT,
    CONF_EXT,
    FONT_EXT,
    THEME_EXT,
    SEARCH_EXT
} BAR_STATUS;

void SetBarStatus(BAR_STATUS status);

BAR_THEME GetBarTheme();

#endif