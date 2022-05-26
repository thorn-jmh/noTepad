#ifndef _sliper_h
#define _sliper_h

//在鼠标回调函数中加入下面这个函数
void sliper();

//请注册一个时钟REDRAWSLIPER，刷新速度应该在10以下，在时钟里面按顺序调用这两个函数
cleanSliper();
DrawSliper();

#endif