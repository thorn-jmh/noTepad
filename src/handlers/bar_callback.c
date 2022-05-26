#include <stdio.h>

#include "bar_callback.h"
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
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    InputString("newfile.txt");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = openfile_set;
}
static void closecbk()
{
    CloseTheFile(FALSE); //TODO: 询问
}
static void newcbk()
{
    OpenTheFile(NULL);
}
static void savecbk()
{
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
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    string filename = GetFileName(CurrentFileI());
    InputString(filename);

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = openfile_set;
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
    CopyTheString();
}
static void pastecbk()
{
    PasteTheString();
}
static void cutcbk()
{
    CopyTheString();
    DeleteString(0);
}
static void deletecbk()
{
    DeleteString(0);
}
static void selectcbk()
{
    CURSOR_T *crst = GetCurrentCursor();
    crst->PTR_1 = crst->PTR_2 = 0;
    string text = GetStrText();
    while (*text != '\0')
    {
        text++;
        crst->PTR_2++;
    }
}
static void searchcbk();

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
    //TODO: SET FONT
    CloseBarText();

    BAR_STATUS status = GetBarStatus();
    status &= (~SETTING_EXT);
    SetBarStatus(status);
}
static void fontcbk()
{
    InitBarText();
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    InputString("System");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = font_set;
}
static void sizecbk()
{
    InitBarText();
    CURSOR_T *crst = GetCurrentCursor();
    crst->focus = 2;
    InputString("System");

    BAR_STATUS status = GetBarStatus();
    status &= (~CLEAR_EXT);
    status |= SETTING_EXT;
    SetBarStatus(status);

    set_botton_cbk = font_set;
}
static void themecbk();

static void helpcbk()
{
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
    .func = NULL,
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
    .func = NULL,
};
BOTTON_T SIZE_BOTTON = {
    .name = "字号",
    .hotkey = "",
    .func = NULL,
};
BOTTON_T THEME_BOTTON = {
    .name = "主题",
    .hotkey = "",
    .func = NULL,
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