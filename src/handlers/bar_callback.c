#include <stdio.h>

#include "bar_callback.h"
#include "printer.h"
#include "bar.h"
#include "file.h"
#include "cursor.h"
#include "editor.h"

static void (*set_botton_cbk)();

static void fileextcbk()
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
static void opencbk()
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
static void closecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    CloseTheFile(FALSE); //TODO: 询问
}
static void newcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    OpenTheFile(NULL);
}
static void savecbk()
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
static void saveascbk()
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

static void editextcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    status |= EDIT_EXT;
    SetBarStatus(status);
}
static void copycbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    CopyTheString();
}
static void pastecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);
    PasteTheString();
}
static void cutcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    CopyTheString();
    DeleteString(0);
}
static void deletecbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    DeleteString(0);
}
static void selectcbk()
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
static void searchcbk(){
    InitBarText();
    InputString("");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SEARCH_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = openfile_set;
}

static void confextcbk()
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
    CURSOR_T *crst=GetCurrentCursor();
    crst->focus=2;
    crst->PTR_1=0,crst->PTR_2=ptrb;
    DeleteString(0);

    InputString(GetTextFont());    
}
static void fontcbk()
{
    InitBarText();
    InputString("System");

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
    CURSOR_T *crst=GetCurrentCursor();
    crst->focus=2;
    crst->PTR_1=0,crst->PTR_2=ptrb;
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
static void theme_set(){
   string text = GetBarText();
    ChangeThemeByName(text);

    size_t ptrb = 0;
    while (*(text) != '\0')
    {
        ptrb++, text++;
    }
    CURSOR_T *crst=GetCurrentCursor();
    crst->focus=2;
    crst->PTR_1=0,crst->PTR_2=ptrb;
    DeleteString(0);

    InputString(GetThemeName()); 
}
static void themecbk(){
    InitBarText();
    InputString(GetThemeName());

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    status &= CLEAR_EXT;
    SetBarStatus(status);

    set_botton_cbk = theme_set;
}

static void helpcbk()
{
    BAR_STATUS status = GetBarStatus();
    status &= CLEAR_EXT;
    SetBarStatus(status);

    system("cmd /c start https://github.com/");
}

static void setcbk()
{
    set_botton_cbk();
}
static void nextcbk();
static void lastcbk();
static void cancelcbk()
{
    CloseBarText();

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    SetBarStatus(status);
}

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
    .func = NULL,
};
BOTTON_T LAST_BOTTON = {
    .name = "LAST",
    .hotkey = NULL,
    .func = NULL,
};
BOTTON_T CANCEL_BOTTON = {
    .name = "关闭",
    .hotkey = NULL,
    .func = cancelcbk,
};