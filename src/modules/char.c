#include<Windows.h>
#include"graphics.h"
#include"cursor.h"
#include<winuser.h>
#include"genlib.h"
#include"cursor.h"
#include"printer.h"
#include<string.h>
#include "editor.h"
static char str[1000];
static int ptr;
void charevent(char key){
    if(key==VK_BACK) return;
    ptr=0;
    str[ptr++]=key;
    str[ptr]='\0';
    InputString(str);
}