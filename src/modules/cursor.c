#include "cursor.h"

static LINE_T theLine; //���徲̬ȫ�ֱ�������Ϣ

static CURSOR_T theCursor; //���徲̬ȫ�ֱ������

static MOUSE_T theMouse; //���徲̬ȫ�ֱ������

//��������Ϣָ��
LINE_T *GetCurrentLine(void)
{
    LINE_T *pl = &theLine;
    return pl;
};

//���ع��ָ��
CURSOR_T *GetCurrentCursor(void)
{
    CURSOR_T *pc = &theCursor;
    return pc;
};

//�����ָ��
MOUSE_T *GetCurrentMouse(void)
{
    MOUSE_T *pm = &theMouse;
    return pm;
};
