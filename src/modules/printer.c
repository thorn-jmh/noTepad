#include "printer.h"
#include "cursor.h"
#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genlib.h"
#include <stddef.h>
#include "file.h"

int judge(float mouse_x,float mouse_y,float x,float y,float width,float height){
    if(mouse_x>=x&&mouse_x<=x+width&&mouse_y>=y&&mouse_y<=y+height) return 1;
    return 0;
}

void PrintText(string text,int offset,CURSOR_T *crst,MOUSE_T *mst,LINE_T *lnt){
    int isFill=0;                  //是否填充字体
    Unicode ch,str[2];                 //一个字符一个字符地读,由str输出,真就是一个字符一个字符输出
    str[1]='\0';                    //str后面一个字符始终为结束符  
    float width=GetWindowWidth(),height=GetWindowHeight();
    float top=1,bottom=0,left=0,right=0.8;      //文本输出占的区域，百分比
    float ch_width,ch_height=TextStringWidth("x")/width*2;          //字体宽度,高度就当整个字符串的高度了,百分比
    float interval=ch_height*1.5;                    //字体间距(含字体),字符串的高度,百分比,我初始化为1.5倍
    float now_width=left;                   //现在输出位置的宽度,百分比
    int position=0,mouse_position=-1;      //读取的字符在数组的位置,鼠标的位置
    lnt->Cline=(top-bottom)/interval;
    int now_line=1;                     //和当前行数
    InitGraphics();

    while((ch=text[position++])!=0){

        str[0]=ch;
        ch_width=TextStringWidth(str)/width;
        now_width+=ch_width;

        if(judge(mst->X/width,mst->Y/height,now_width,top-now_line*interval,ch_width,ch_height)) mouse_position=position-1;  // 判断mouse的位置

        
        if(now_line>=lnt->Fline&&now_line<lnt->Cline+lnt->Fline){                                      //当前行数没超过最大行数，能输出
            if(ch=='\r'||ch=='\n'){                              //遇到换行符
                now_line++;
                now_width=left;
            }else if(now_width>right){              //遇到边界情况，该字符不输出
                now_line++;
                now_width=left;
                position--;                                     //这个是回拨一个position,因为这个字符没输出，所以想让下一次录入仍为这个字符                                 
            }else{                                              //继续走
                MovePen(width*(now_width-ch_width),height*(top-now_line*interval));
                if(position==crst->PTR_1){
                    if(position==crst->PTR_2){
                        crst->X=width*(now_width-ch_width);      //传输光标的位置，（光标左下角的位置），绝对位置（非百分比）
                        crst->Y=height*(top-now_line*interval);  
                    }else isFill=1;                              //如果两个坐标不同，那就是选中了
                }
                if(position==crst->PTR_2) isFill=0;             //结束涂色，PTR_2指向的字符不涂色
                if(isFill){                                      //将选中的地方涂色
                    SetPenColor("Blue");
                    drawRectangle(width*(now_width-ch_width),height*(top-now_line*interval),ch_width*width,ch_height*height,1);
                    SetPenColor("Black");
                }
                DrawTextString(str);
            }
        }else{                                                //之前的行数，不能输出，只计算
            if(ch=='\r'||ch=='\n'){          
                now_line++;
                now_width=left;
            }else if(now_width>width*(right-left)){ 
                now_line++;
                now_width=left;
                position--;                                  
            }else{}
        }
    }

    lnt->Tline=now_line;
    return;
}