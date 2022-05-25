#include"graphics.h"
#include"cursor.h"
#include<winuser.h>
#include"genlib.h"
#include"cursor.h"
#include"printer.h"
static string *text_string;


void leftkey(){
    CURSOR_T *cursor;
    cursor =GetCurrentCursor();
    int chwidth=OneCharLength(text_string[cursor->PTR_1]);

    if(cursor->PTR_1==cursor->PTR_2){
        if(cursor->PTR_1!=0){
            if(OneCharLength(text_string[cursor->PTR_1-chwidth])){
            cursor->PTR_1-=chwidth;
            cursor->PTR_2=cursor->PTR_1;
            }
        }
    }else{
        if(OneCharLength(text_string[cursor->PTR_1-chwidth])){
            cursor->PTR_1-=chwidth;
            cursor->PTR_2=cursor->PTR_1;
        }
    }
    PrintTheText(1);
}

void rightkey(){
    CURSOR_T *cursor;
    cursor =GetCurrentCursor();
    int chwidth=OneCharLength(text_string[cursor->PTR_1]);

    if(cursor->PTR_1==cursor->PTR_2){
        if(cursor->PTR_2!=0){
            cursor->PTR_2+=chwidth;
            cursor->PTR_1=cursor->PTR_2;
        }
    }else{
        cursor->PTR_2+=chwidth;
        cursor->PTR_1=cursor->PTR_2;
    }
    PrintTheText(1);
}

void upkey(){
    CURSOR_T *cursor;
    MOUSE_T *mouse;
    LINE_T *line;
    cursor=GetCurrentCursor();
    line=GetCurrentLine();
    mouse=GetCurrentMouse();    
    double true_x=mouse->X,true_y=mouse->Y;                                 //记录下mouse的x和y
    int    true_ptr=mouse->PTR;
    double height=GetFontHeight();                                          //有问题，height和mouse坐标以及cursor坐标的单位是否相同？

    PrintTheText(1);
    
    if(cursor->Line<line->Fline){                                           //cursor超出输入的框了，在上面
        FocusLine(cursor->Line,0);                                          //把cursor移到显示的第二行
    }else if(cursor->Line>=line->Cline+line->Fline){                        //cursor超出输入的框了，在下面
        FocusLine(cursor->Line,1);                                          //把cursor移到显示的导数第二行
    }
    if(cursor->Line==1) return;                                             //cursor就在第一行，别移动了

    PrintTheText(1);
    mouse->X=cursor->X;
    mouse->Y=cursor->Y+height;                                              //把鼠标移到cursor的上一行

    PrintTheText(1);
    //if(mouse->PTR<=len_text&&mouse->PTR>=0){                              //如果ptr大于text的长度，或者小于0，就不变
        cursor->PTR_1=mouse->PTR;
        cursor->PTR_2=mouse->PTR;
    //}

    mouse->X=true_x;                                                        //恢复鼠标的参数
    mouse->Y=true_y;
    mouse->PTR=true_ptr;

    PrintTheText(1);
}

void downkey(){
    CURSOR_T *cursor;
    MOUSE_T *mouse;
    LINE_T *line;
    cursor=GetCurrentCursor();
    line=GetCurrentLine();
    mouse=GetCurrentMouse();    
    double true_x=mouse->X,true_y=mouse->Y;                                 //记录下mouse的x和y
    int    true_ptr=mouse->PTR;
    double height=GetFontHeight();                                          //有问题，height和mouse坐标以及cursor坐标的单位是否相同？

    PrintTheText(1);
    if(cursor->Line<line->Fline){                                           //cursor超出输入的框了，在上面
        FocusLine(cursor->Line,0);                                          //把cursor移到显示的第二行
    }else if(cursor->Line>=line->Cline+line->Fline){                        //cursor超出输入的框了，在下面
        FocusLine(cursor->Line,1);                                          //把cursor移到显示的导数第二行
    }
    if(cursor->Line==line->Tline) return;                                   //cursor就在最后一行，别移动了
    
    PrintTheText(1);
    mouse->X=cursor->X;
    mouse->Y=cursor->Y-height;                                              //把鼠标移到cursor的下一行

    PrintTheText(1);
    //if(mouse->PTR<=len_text&&mouse->PTR>=0){                              //如果ptr大于text的长度，或者小于0，就不变
        cursor->PTR_1=mouse->PTR;
        cursor->PTR_2=mouse->PTR;
    //}

    mouse->X=true_x;                                                        //恢复鼠标的参数
    mouse->Y=true_y;
    mouse->PTR=true_ptr;

    PrintTheText(1);
}

void keyboardevnt(int key, int event){
    text_string=GetStrText();
    if(key==VK_LEFT&&event==KEY_DOWN) leftkey();
    if(key==VK_RIGHT&&event==KEY_DOWN) rightkey();
    if(key==VK_UP&&event==KEY_DOWN) upkey();
    if(key==VK_DOWN&&event==KEY_DOWN) dnowkey();
}