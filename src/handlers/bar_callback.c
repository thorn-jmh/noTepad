#include <stdio.h>
#include <Windows.h>
#include "printer.h"
#include "finder.h"
#include "bar.h"
#include "file.h"
#include "cursor.h"
#include "editor.h"
#include "bar_callback.h"

// set_button's callback function
static void (*set_botton_cbk)();

// each button's callback function

void fileextcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    status |= FILE_EXT;
    SetBarStatus(status);
}
static void openfile_set()
{
    string text = GetBarText();
    OpenTheFile(text);
    CloseBarText();

    BAR_STATUS status = GetBarStatus();
    status &= (~SETTING_EXT);
    SetBarStatus(status);
}
void opencbk()
{
    InitBarText();
    InputString("newfile.txt");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status &= CLEAR_EXT;
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = openfile_set;
}
void closecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    CloseTheFile(FALSE); //TODO: 询问
}
void newcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    OpenTheFile(NULL);
}
void savecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    SaveTheFile(NULL);
}
static void saveas_set()
{
    string text = GetBarText();
    SaveTheFile(text);
    CloseBarText();

    BAR_STATUS status = GetBarStatus();
    status &= (~SETTING_EXT);
    SetBarStatus(status);
}
void saveascbk()
{
    InitBarText();
    string filename = GetFileName(CurrentFileI());
    InputString(filename);

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status &= CLEAR_EXT;
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = saveas_set;
}

void editextcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    status |= EDIT_EXT;
    SetBarStatus(status);
}
void copycbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    CopyTheString();
}
void pastecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    PasteTheString();
}
void cutcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    CopyTheString();
    DeleteString(0);
}
void deletecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    DeleteString(0);
}
void selectcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    CURSOR_T *crst = GetCurrentCursor();
    crst->PTR_1 = crst->PTR_2 = 0;
    string text = GetStrText();
    while (*text != '\0')
    {
        text++;
        crst->PTR_2++;
    }
}
void undocbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    UndoHistory();
}
void redocbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    RedoHistory();
}
void searchcbk()
{
    InitBarText();
    InputString("");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SEARCH_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = openfile_set;
}

void confextcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    status |= CONF_EXT;
    SetBarStatus(status);
}
static void font_set()
{
    string text = GetBarText();
    SetTextFont(text);

    size_t ptrb = 0;
    while (*(text) != '\0')
    {
        ptrb++, text++;
    }
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    crst->PTR_1 = 0, crst->PTR_2 = ptrb;
    DeleteString(0);

    InputString(GetTextFont());
}
static void fontcbk()
{
    InitBarText();
    InputString(GetTextFont());

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = font_set;
}
static void size_set()
{
    string text = GetBarText();
    SetTextSize(text);

    size_t ptrb = 0;
    while (*(text) != '\0')
    {
        ptrb++, text++;
    }
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    crst->PTR_1 = 0, crst->PTR_2 = ptrb;
    DeleteString(0);

    InputString(GetTextSize());
}
static void sizecbk()
{
    InitBarText();
    InputString(GetTextSize());

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = size_set;
}
static void theme_set()
{
    string text = GetBarText();
    ChangeThemeByName(text);

    size_t ptrb = 0;
    while (*(text) != '\0')
    {
        ptrb++, text++;
    }
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    crst->PTR_1 = 0, crst->PTR_2 = ptrb;
    DeleteString(0);

    InputString(GetThemeName());
}
static void themecbk()
{
    InitBarText();
    InputString(GetThemeName());

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = theme_set;
}

void helpcbk()
{
    if (getmux())
        return;
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    system("cmd /c start https://github.com/");
    lockmux();
}

static void setcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    if (set_botton_cbk != NULL)
        set_botton_cbk();
}
void nextcbk()
{
    if (getmux())
        return;
    if (TotalFound() == -1)
    {
        string text = GetBarText();
        FindSubStr(text);
    }
    else
    {
        NextFoundStr();
    }
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 1;
    crst->PTR_1 = crst->PTR_2 = WhereFoundStr();
    lockmux();
}
static void lastcbk()
{
    if (getmux())
        return;
    if (TotalFound() == -1)
    {
        string text = GetBarText();
        FindSubStr(text);
    }
    else
    {
        LastFoundStr();
    }
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 1;
    crst->PTR_1 = crst->PTR_2 = WhereFoundStr();
    lockmux();
}
void cancelcbk()
{
    CloseBarText();
    FreeFoundList();

    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    status &= (~CLEAR_EXT);
    SetBarStatus(status);
}

// define all buttons may used

BOTTON_T FILE_BOTTON = {
    .name = "文件",
    .hotkey = "Alt+F",
    .func = fileextcbk,
};
BOTTON_T OPEN_BOTTON = {
    .name = "打开",
    .hotkey = "Ctrl+O",
    .func = opencbk,
};
BOTTON_T CLOSE_BOTTON = {
    .name = "关闭",
    .hotkey = "Ctrl+T",
    .func = closecbk,
};
BOTTON_T NEW_BOTTON = {
    .name = "新建",
    .hotkey = "Ctrl+N",
    .func = newcbk,
};
BOTTON_T SAVE_BOTTON = {
    .name = "保存",
    .hotkey = "Ctrl+S",
    .func = savecbk,
};
BOTTON_T SAVEAS_BOTTON = {
    .name = "另存为",
    .hotkey = "",
    .func = saveascbk,
};

BOTTON_T EDIT_BOTTON = {
    .name = "编辑",
    .hotkey = "Alt+E",
    .func = editextcbk,
};
BOTTON_T Undo_BOTTON = {
    .name = "撤销",
    .hotkey = "Ctrl+Z",
    .func = undocbk,
};
BOTTON_T Redo_BOTTON = {
    .name = "重做",
    .hotkey = "Ctrl+Y",
    .func = redocbk,
};
BOTTON_T SEARCH_BOTTON = {
    .name = "搜索",
    .hotkey = "Ctrl+F",
    .func = searchcbk,
};
BOTTON_T PASTE_BOTTON = {
    .name = "粘贴",
    .hotkey = "Ctrl+V",
    .func = pastecbk,
};
BOTTON_T COPY_BOTTON = {
    .name = "复制",
    .hotkey = "Ctrl+C",
    .func = copycbk,
};
BOTTON_T CUT_BOTTON = {
    .name = "剪切",
    .hotkey = "Ctrl+X",
    .func = cutcbk,
};
BOTTON_T DELETE_BOTTON = {
    .name = "删除",
    .hotkey = "Ctrl+D",
    .func = deletecbk,
};
BOTTON_T SELECT_BOTTON = {
    .name = "全选",
    .hotkey = "Ctrl+A",
    .func = selectcbk,
};

BOTTON_T CONF_BOTTON = {
    .name = "设置",
    .hotkey = "Alt+C",
    .func = confextcbk,
};
BOTTON_T FONT_BOTTON = {
    .name = "字体",
    .hotkey = "",
    .func = fontcbk,
};
BOTTON_T SIZE_BOTTON = {
    .name = "字号",
    .hotkey = "",
    .func = sizecbk,
};
BOTTON_T THEME_BOTTON = {
    .name = "主题",
    .hotkey = "",
    .func = themecbk,
};

BOTTON_T HELP_BOTTON = {
    .name = "帮助",
    .hotkey = "Alt+H",
    .func = helpcbk,
};

BOTTON_T SET_BOTTON = {
    .name = "确定",
    .hotkey = NULL,
    .func = setcbk,
};
BOTTON_T NEXT_BOTTON = {
    .name = "NEXT",
    .hotkey = NULL,
    .func = nextcbk,
};
BOTTON_T LAST_BOTTON = {
    .name = "LAST",
    .hotkey = NULL,
    .func = lastcbk,
};
BOTTON_T CANCEL_BOTTON = {
    .name = "关闭",
    .hotkey = NULL,
    .func = cancelcbk,
};