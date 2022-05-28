#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "graphics.h"
#include "extgraph.h"
#include "pages.h"
#include "printer.h"
#include "sliper.h"
#include "file.h"
#include "bar.h"
#include "keyboard.h"
#include "router.h"

static void timeManager(int id);

void InitStatus()
{
    InitFileSys();
    OpenTheFile("readme.txt");
    SaveTheFile(NULL);

    InitGraphics();
    SetWindowTitle("noTepad.exe");
    startTimer(114, 10);
    startTimer(514, 500);

    registerTimerEvent(timeManager);
    registerMouseEvent(MouseEvent);
    registerKeyboardEvent(KeyBoardEvent);
    registerCharEvent(CharInputEvent);
}

static void timeManager(int id)
{
    if (id == 114)
    {
        DisplayClear();
        UpdatePageInfo();
        PrintTheText(1);
        UpdateAllBar();
        DrawSliper();
    }
    if (id == 514)
    {
        ChangeCursor();
    }
}