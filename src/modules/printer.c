#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <locale.h>

#include "genlib.h"
#include "printer.h"
#include "linkedlist.h"
#include "finder.h"
#include "cursor.h"
#include "file.h"

#define Epsilon 0.00000000001

static string FONT_COLOR = "Black";        //字体颜色
static string BK_GROUND_COLOR = "White";   //背景颜色
static string HIGH_LIGHT_COLOR = "Orange"; //高亮颜色
static string SELECTED_COLOR = "Gray";     //选中颜色
static string FONT_NAME = "System";        //字体
static int POINT_SIZE = 13;                //字号
static int FONT_STYLE = 0;                 //字体样式
static double CURSOR_WID = 0.017;          //光标粗细
static string CURSOR_COLOR = "Black";      //光标颜色
static bool CURSOR_PRINT = TRUE;           //是否打印光标（和timer衔接）
static bool BUTTON_IN = FALSE;             //是否左键点击到文字区

//左右上下间隔
static double LeftMargin = 0.1;
static double RightMargin = 0.1;
static double TopMargin = 0.1;
static double BottomMargin = 0.1;

static linkedlistADT STACK; //背景色栈

static void drawRectangle(double x, double y, double w, double h, int fillflag, string color);

static void setTextPen();
static void drawBK(double wid, string color);
static void printCursor();
static bool checkMouse_Line();
static bool checkMouseNoti();
static bool checkMouse();

static mouseWork();

static void SetHighColor(string color);
static void DelHightColor();
static string CurrentHighColor();
static void HighlightStart();
static void HighlightEnd();

void SetTextFont(string font)
{
    SetFont(FONT_NAME);
    SetFont(font);
    FONT_NAME = GetFont();
}

string GetTextFont()
{
    return FONT_NAME;
}

void SetTextSize(string size)
{
    int fsize = 0;
    while (*size != '\0')
    {
        fsize *= 10;
        fsize += *size - '0';
        size++;
    }

    SetPointSize(POINT_SIZE);
    SetPointSize(fsize % 100);
    POINT_SIZE = GetPointSize();
}

static char sizestr[3];
string GetTextSize()
{
    sizestr[2] = '\0';
    sizestr[1] = POINT_SIZE % 10 + '0';
    sizestr[0] = POINT_SIZE / 10 + '0';
    if (POINT_SIZE < 10)
        return sizestr + 1;
    return sizestr;
}

//清楚文字区域内容
void CLearTextBar()
{
    PAGE_T *pgt = GetPageInfo();
    double topY, leftX, rightX, bottomY;
    topY = pgt->TEXT.LT.Y;
    leftX = pgt->TEXT.LT.X;
    rightX = pgt->TEXT.RB.X;
    bottomY = pgt->TEXT.RB.Y;

    drawRectangle(leftX, bottomY, rightX - leftX, topY - bottomY, TRUE, BK_GROUND_COLOR);
}

//将指定行移动到行内，0为最开头，1为最后
bool FocusLine(int line, bool Mode)
{
    LINE_T *lnt = GetCurrentLine();
    if (line <= 0 || line > lnt->Tline)
        return FALSE;
    // printText();
    // CLearTextBar();
    if (Mode)
    {
        lnt->Fline = ((line == lnt->Tline) ? lnt->Tline : line + 1) - lnt->Cline + 1;
    }
    else
    {
        lnt->Fline = (line == 1) ? 1 : line - 1;
    }
    return TRUE;
}

void PrintTheText(bool print)
{
    string Otext = GetStrText();
    string text = Otext;
    CURSOR_T *crst = GetCurrentCursor();
    MOUSE_T *mst = GetCurrentMouse();
    LINE_T *lnt = GetCurrentLine();
    PAGE_T *pgt = GetPageInfo();
    linkedlistADT HList = GetCurrentHighlight();
    HighlightStart();
    setTextPen();

    double topY, leftX, rightX, bottomY;
    topY = pgt->TEXT.LT.Y - TopMargin;
    leftX = pgt->TEXT.LT.X + LeftMargin;
    rightX = pgt->TEXT.RB.X - RightMargin;
    bottomY = pgt->TEXT.RB.Y + BottomMargin;

    bool mLocated = FALSE;
    bool printFlag = FALSE;
    bool highed = FALSE;

    size_t cursor_ptr = min(crst->PTR_1, crst->PTR_2);
    size_t another_c = max(crst->PTR_1, crst->PTR_2);
    size_t char_num = 0;
    int nowline = 1;
    int print_line = 1;
    double lenLine = 0;
    double actF, dctF, htF;
    actF = GetFontAscent();
    dctF = GetFontDescent();
    htF = GetFontHeight();

    MovePen(leftX, topY + dctF - htF * print_line);
    crst->X = crst->Y = -1;
    lnt->Cline = lnt->Tline = 0;
    lnt->Fline = (lnt->Fline > 0) ? lnt->Fline : 1;
    while (TRUE)
    {
        //从多字节中处理出单字符
        printFlag = FALSE;
        size_t olen = OneCharLength(*text);
        string chs = (string)malloc((olen + 1) * sizeof(char));
        memcpy(chs, text, olen);
        *(chs + olen) = '\0';

        //计算行长度
        double one_width = TextStringWidth(chs);
        lenLine += one_width;
        if (one_width > 0)
            char_num++;

        //判断是否输出
        if (nowline >= lnt->Fline && htF * (print_line - 1) < topY - bottomY)
            printFlag = TRUE;

        //处理HighLight信息
        while (HList != NULL && HList->next != NULL)
        {
            if (*((size_t *)(HList->next->dataptr)) > (text - Otext))
            {
                break;
            }
            if (highed)
                DelHightColor();
            else
                SetHighColor(HIGH_LIGHT_COLOR);
            highed ^= 1;
            DeleteCurrentNodeWithOutFree(HList, HList->next);
        }

        //处理光标信息
        if (crst->focus == 1 && cursor_ptr == text - Otext)
        {
            crst->Line = nowline;
            if (printFlag)
            {
                crst->X = GetCurrentX();
                crst->Y = GetCurrentY();
            }
            SetHighColor(SELECTED_COLOR);
        }
        if (crst->focus == 1 && another_c == text - Otext)
        {
            DelHightColor();
        }

        //换行1
        if (lenLine >= rightX - leftX)
        {
            lenLine = 0;
            nowline++;
            if (printFlag)
            {
                print_line++;
                MovePen(leftX, topY + dctF - (htF + dctF / 6) * print_line);
            }
            continue;
        }

        //输出及画面内处理
        if (printFlag)
        {
            if (InNotiArea(GetCurrentX(), GetCurrentY()))
                PenMove(one_width, 0);
            else
            {
                if (!mLocated && checkMouse() && checkMouseNoti())
                {
                    mLocated = TRUE;
                    mst->PTR = text - Otext;
                    if (text - Otext > 0 && *(text - 1) == '\n' && checkMouse_Line())
                    {
                        mst->PTR--;
                    }
                    mouseWork();
                }
                if (print)
                {
                    if (crst->focus == 1)
                        if (CURSOR_PRINT && cursor_ptr == text - Otext && another_c == cursor_ptr)
                            printCursor();
                    drawBK(one_width, CurrentHighColor());
                    DrawTextString(chs);
                }
            }
        }

        //换行2
        if (!strcmp(chs, "\n"))
        {
            lenLine = 0;
            nowline++;
            if (printFlag)
            {
                print_line++;
                MovePen(leftX, topY + dctF - (htF + dctF / 6) * print_line);
            }
        }

        free(chs);
        if (*text == 0)
        {
            break;
        }
        text += olen;
    }

    //处理完Cline信息
    while (printFlag)
    {
        printFlag = FALSE;
        if (htF * (print_line - 1) < topY - bottomY)
            printFlag = TRUE;
        print_line++;
    }

    if (!mLocated)
    {
        mst->PTR = text - Otext;
        if (checkMouseNoti())
            mouseWork();
    }

    FreeLinkedList(HList);
    HighlightEnd();
    lnt->Tline = nowline;
    lnt->Cline = print_line - 1;
    if (htF * print_line > topY - bottomY)
        lnt->Cline--;
}

/////////////////////////////////////////

//////////// Mouse Module ///////////////

static mouseWork()
{
    MOUSE_T *mst = GetCurrentMouse();
    CURSOR_T *crst = GetCurrentCursor();
    if (BUTTON_IN)
    {
        crst->PTR_2 = mst->PTR;
    }
    if (mst->button == LEFT_BUTTON)
    {
        if (mst->event == BUTTON_DOWN)
        {
            BUTTON_IN = TRUE;
            crst->focus = 1;
            crst->PTR_1 = crst->PTR_2 = mst->PTR;
        }
        else if (mst->event == BUTTON_UP && BUTTON_IN)
        {
            crst->focus = 1;
            crst->PTR_2 = mst->PTR;
            BUTTON_IN = FALSE;
        }
    }
    if (mst->button == RIGHT_BUTTON)
    {
        if (mst->event == BUTTON_UP)
        {
        }
    }
}

////////////////////////////////////////

////////// High Light Module ////////////

static void HighlightStart()
{
    STACK = NewLinkedList();

    string tp = (string)malloc((strlen(BK_GROUND_COLOR) + 1) * sizeof(char));
    strcpy(tp, BK_GROUND_COLOR);
    InsertNode(STACK, STACK, (void *)tp);
}

static void HighlightEnd()
{
    FreeLinkedList(STACK);
}

static void SetHighColor(string color)
{
    string tp = (string)malloc((strlen(color) + 1) * sizeof(char));
    strcpy(tp, color);
    InsertNode(STACK, STACK, (void *)tp);
}

static void DelHightColor()
{
    DeleteCurrentNode(STACK, STACK->next);
}

static string CurrentHighColor()
{
    return (string)(STACK->next->dataptr);
}

///////////////////////////////////////////

/////////////// Draw Module ////////////////

//will return pen to original place
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

static void setTextPen()
{
    SetPenSize(0.001);
    SetPenColor(FONT_COLOR);
    SetPointSize(POINT_SIZE);
    SetFont(FONT_NAME);
    SetStyle(FONT_STYLE);
}

static void drawBK(double wid, string color)
{
    double dctF, htF;
    dctF = GetFontDescent();
    htF = GetFontHeight();
    PenMove(0, -dctF);
    drawRectangle(-1, -1, wid, htF, TRUE, color);
    PenMove(0, dctF);
}

//显示光标
static void printCursor()
{
    double actF, dctF, htF;
    actF = GetFontAscent();
    dctF = GetFontDescent();
    htF = GetFontHeight();
    PenMove(0, -dctF);
    drawRectangle(-1, -1, -CURSOR_WID, htF, TRUE, CURSOR_COLOR);
    PenMove(0, dctF);
}

static bool checkMouse_Line()
{
    MOUSE_T *mst = GetCurrentMouse();
    double mx, my, cx, cy;
    cx = GetCurrentX();
    cy = GetCurrentY();
    mx = mst->X, my = mst->Y;
    double actF, dctF;
    actF = GetFontAscent();
    dctF = GetFontDescent();

    if (my > cy + actF)
        return TRUE;
    return FALSE;
}

static bool checkMouseNoti()
{
    PAGE_T *pgt = GetPageInfo();
    MOUSE_T *mst = GetCurrentMouse();
    double mx, my, cx, cy;
    cx = GetCurrentX();
    cy = GetCurrentY();
    mx = mst->X, my = mst->Y;

    if (mx < pgt->TEXT.LT.X + LeftMargin || mx > pgt->TEXT.RB.X - RightMargin)
        return FALSE;
    if (my > pgt->TEXT.LT.Y - TopMargin || my < pgt->TEXT.RB.Y + BottomMargin)
        return FALSE;
    if (InNotiArea(mx, my))
        return FALSE;

    return TRUE;
}
static bool checkMouse()
{
    MOUSE_T *mst = GetCurrentMouse();
    double mx, my, cx, cy;
    cx = GetCurrentX();
    cy = GetCurrentY();
    mx = mst->X, my = mst->Y;

    double actF, dctF;
    actF = GetFontAscent();
    dctF = GetFontDescent();

    if (mx < cx && my > cy - dctF)
        return TRUE;
    else if (my > cy + actF)
        return TRUE;
    else
        return FALSE;
}
