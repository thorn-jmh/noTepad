#ifndef _sliper_h
#define _sliper_h

//在鼠标回调函数中按顺序调用这三个函数
void sliper();

//请注册一个时钟REDRAWSLIPER，刷新速度可以是50，在时钟里面按顺序调用这两个函数
void cleanSliper();
void DrawSliper();

#endif