#include "cursor.h"
/* MovePen(width*(now_width-ch_width),height*(top-now_line*interval));
                if(position==crst->PTR_1){
                    if(position==crst->PTR_2){
                        crst->X=width*(now_width-ch_width);
                        crst->Y=height*(top-now_line*interval);
                    }else isFill=1;                              //如果两个坐标不同，那就是选中了
                }
                if(position==crst->PTR_2) isFill=0;             //结束涂色，PTR_2指向的字符不涂色
                if(isFill){                                      //将选中的地方涂色
                    SetPenColor("Blue");
                    drawRectangle(width*(now_width-ch_width),height*(top-now_line*interval),ch_width*width,ch_height*height,1);
                    SetPenColor("Black");
                }
                DrawTextString(str);*/
static CURSOR_T theCursor = { //定义静态全局变量光标
    0, 0, 0.0, 0.0};

void RenewCursorPTR(void)
{ //设置 PTR1=PTR2; 把PTR设置为(x,y)对应的字符位置
    theCursor.PTR_1 = PixieltoPTR(theCursor.X, theCursor.Y);
    theCursor.PTR_1 = theCursor.PTR_2;
}

double PTRtoPixelX(int PTR)
{   
    string text = GetStrText();
    //这里是计算x坐标的算法，还没有写，要和printer一致
    double PixelX;

    
    return PixelX;
}

double PTRtoPixelY(int PTR)
{
    string text = GetStrText();
    double PixelY;
    //这里是计算y坐标的算法，还没有写，要和printer一致
    return PixelY;
}

int PixieltoPTR(double x, double y)
{
    string text = GetStrText();
    int PTR;
    //这里是坐标到字符位置的算法，还没有写，要和printer一致
    return PTR;
}

void MoveCursorByMouse(CURSOR_T *crst, MOUSE_T *mst)
{
    if (mst->button == LEFT && mst->event == DOWN)
    { //左键按下，PTR1设置为当前鼠标位置对应的字符位置
        crst->PTR_1 = PixieltoPTR(mst->X, mst->Y);
    }
    if (mst->button == LEFT && mst->event == UP)
    {
        crst->PTR_2 = PixieltoPTR(mst->X, mst->Y); //左键抬起。PTR2设置为当前鼠标位置对应的字符位置
    }
};

void MoveCursorByDirect(CURSOR_T *crst, DIRECT_KEY drct)
{
    //抄的printer，定义行高度interval
    double width = GetWindowWidth();
    double ch_height = TextStringWidth("x") / width * 3;
    double interval = ch_height * 1.5;

    if (drct == LEFT)
    { //按左键，
        if (crst->PTR_1 == crst->PTR_2 && crst->PTR_1 && crst->PTR_2)
        { //光标的PTR1，PTR2相等且都为正数就同时减一
            crst->PTR_1--;
            crst->PTR_2--;
        }
        else
        {
            crst->PTR_2 = crst->PTR_1; //否则要么是在选中状态，要么是在最开头，把PTR2设置为PTR1就可以
        }
    }

    else if (drct == RIGHT)
    { //按右键
        if (crst->PTR_1 == crst->PTR_2)
        { //如果PTR1，PTR2相等就同时加一
            crst->PTR_1++;
            crst->PTR_2++;
        }
        else
        {
            crst->PTR_1 = crst->PTR_2; //否则是在选中状态，PTR1设置为PTR2
        }
    }
    else if (drct == UP)
    {
        crst->Y += interval; //光标的y加一间距
        RenewCursorPTR();    //更新PTR
    }

    else if (drct == DOWN)
    {
        crst->Y -= interval; //光标的y加一间距
        RenewCursorPTR();    //更新PTR
    }
};

CURSOR_T *GetCurrentCursor(void) //返回光标地址
{
    CURSOR_T *p = &theCursor;
    return p;
}

void SetCursorToPTR(int PTR) //把光标的PTR1，PTR2都设置到PTR
{
    if (PTR > 0)
    {
        theCursor.PTR_2 = PTR;
        theCursor.PTR_1 = PTR;
    }
}