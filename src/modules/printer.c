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
static bool checkMouse();

static void SetHighColor(string color);
static void DelHightColor();
static string CurrentHighColor();
static void HighlightStart();
static void HighlightEnd();

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

void PrintTheText()
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

        //换行
        if (lenLine >= rightX - leftX || !strcmp(chs, "\n"))
        {
            lenLine = 0;
            nowline++;
            if (printFlag)
            {
                print_line++;
                MovePen(leftX, topY + dctF - (htF + dctF / 6) * print_line);
            }
            if (!strcmp(chs, "\n"))
                text += olen;
            continue;
        }

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

        //输出及画面内处理
        if (printFlag)
        {
            if (!mLocated && checkMouse())
            {
                mLocated = TRUE;
                mst->PTR = text - Otext;
            }
            drawBK(one_width, CurrentHighColor());
            DrawTextString(chs);
            if (crst->focus == 1)
                if (CURSOR_PRINT && cursor_ptr == text - Otext && another_c == cursor_ptr)
                    printCursor();
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
    FreeLinkedList(HList);
    HighlightEnd();
    lnt->Tline = nowline;
    lnt->Cline = print_line - 1;
    if (htF * print_line > topY - bottomY)
        lnt->Cline--;
}

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

static bool checkMouse()
{
    MOUSE_T *mst = GetCurrentMouse();
    double mx, my, cx, cy;
    mx = GetMouseX();
    my = GetMouseY();
    cx = GetCurrentX();
    cy = GetCurrentY();
    mst->X = mx, mst->Y = my;
    double actF, dctF;
    actF = GetFontAscent();
    dctF = GetFontDescent();

    if (mx < cx - Epsilon && my > cy - dctF)
        return FALSE;
    else if (my > cy + actF)
        return FALSE;
    else
        return FALSE;
}

///////////////////////////////////////////

//// abandon ////

// int judge(float mouse_x, float mouse_y, float x, float y, float width, float height, Unicode ch)
// {
//     if (ch == '\r' || ch == '\n')
//     {
//         if (mouse_y >= y && mouse_y <= y + height && mouse_x >= x)
//             return 1;
//     }
//     else
//     {
//         if (mouse_x >= x && mouse_x <= x + width && mouse_y >= y && mouse_y <= y + height)
//             return 1;
//     }
//     return 0;
// }

// void PrintText(Ustring text, int offset)
// { //后面三个删掉，用get获得
//     CURSOR_T *crst;
//     crst = GetCurrentCursor();
//     MOUSE_T *mst;
//     mst = GetCurrentMouse();
//     LINE_T *lnt;
//     lnt = GetCurrentLine();
//     int isFill = 0;     //是否填充字体
//     int isCursor = 0;   //光标是否在显示的行内
//     int notJudged = 1;  //鼠标坐标还未赋值
//     Unicode ch, str[2]; //一个字符一个字符地读,由str输出,真就是一个字符一个字符输出
//     str[1] = '\0';      //str后面一个字符始终为结束符

//     float width = GetWindowWidth(), height = GetWindowHeight();
//     float ascent = GetFontAscent() / width, descent = GetFontDescent() / width, ch_height = GetFontHeight() / width; //字体的下部分，中间部分，整个高度
//     float top = 1, bottom = 0, left = 0, right = 0.8;                                                                //文本输出占的区域，百分比
//     float ch_width;                                                                                                  //字体宽度,高度就当整个字符串的高度了,百分比
//     float interval = ch_height * 1.5;                                                                                //字体间距(含字体),字符串的高度,百分比,我初始化为1.5倍
//     float now_width = left;                                                                                          //现在输出位置的宽度,百分比
//     int position = 0, mouse_position = -1;                                                                           //读取的字符在数组的位置,鼠标的位置
//     lnt->Cline = (top - bottom) / interval;
//     int now_line = 1; //和当前行数
//     //InitGraphics();  不要init了

//     while ((ch = text[position++]) != 0)
//     {

//         str[0] = ch;
//         ch_width = TextStringWidth(str);
//         ch_width /= width;
//         now_width += ch_width;

//         if (crst->PTR_1 > crst->PTR_2)
//         { //保证PTR_1不大于PTR_2
//             size_t temp;
//             temp = crst->PTR_1;
//             crst->PTR_1 = crst->PTR_2;
//             crst->PTR_2 = temp;
//         }

//         if (judge(mst->X / width, mst->Y / height, now_width, top - now_line * interval - descent, ch_width, ch_height, ch) && notJudged)
//         {
//             mst->PTR = position - 1; // 判断mouse的位置
//             notJudged = 0;
//         }

//         if (now_line >= lnt->Fline && now_line < lnt->Cline + lnt->Fline)
//         { //当前行数没超过最大行数，能输出
//             if (ch == '\r' || ch == '\n')
//             { //遇到换行符
//                 now_line++;
//                 now_width = left;
//             }
//             else if (now_width > right)
//             { //遇到边界情况，该字符不输出
//                 now_line++;
//                 now_width = left;
//                 position--; //这个是回拨一个position,因为这个字符没输出，所以想让下一次录入仍为这个字符
//             }
//             else
//             { //继续走
//                 MovePen(width * (now_width - ch_width), height * (top - now_line * interval));
//                 if (position == crst->PTR_1)
//                 {
//                     isCursor = 1;
//                     if (position == crst->PTR_2)
//                     {
//                         crst->X = width * (now_width - ch_width); //传输光标的位置，（光标左下角的位置），绝对位置（非百分比）
//                         crst->Y = height * (top - now_line * interval - descent);
//                     }
//                     else
//                         isFill = 1; //如果两个坐标不同，那就是选中了
//                 }
//                 if (position == crst->PTR_2)
//                     isFill = 0; //结束涂色，PTR_2指向的字符不涂色
//                 if (isFill)
//                 { //将选中的地方涂色
//                     SetPenColor("Blue");
//                     drawRectangle(width * (now_width - ch_width), height * (top - now_line * interval - descent), ch_width * width, ch_height * height, 1);
//                     SetPenColor("Black");
//                 }
//                 MovePen(width * (now_width - ch_width), height * (top - now_line * interval));
//                 DrawTextString(str);
//             }
//         }
//         else
//         { //之前的行数，不能输出，只计算
//             if (ch == '\r' || ch == '\n')
//             {
//                 now_line++;
//                 now_width = left;
//             }
//             else if (now_width > width * (right - left))
//             {
//                 now_line++;
//                 now_width = left;
//                 position--;
//             }
//             else
//             {
//             }
//         }
//     }

//     if (isCursor == 0)
//     { //此时光标不再可显示的范围内
//         crst->X = -1;
//         crst->Y == 1;
//     }

//     lnt->Tline = now_line;
//     return;
// }