#include "MemoryWindow.h"


void RunMemoryWindow(Console *console, Process *process)
{
    Window memoryWindow = {1, 0, console->bufferWidth * 0.6, console->bufferHeight*0.70, BWHITE, process->processName, console};
    //Window memoryWindow2 = {console->bufferWidth*0.6+4, 0, (console->bufferWidth-(console->bufferWidth*0.6+5))-3, console->bufferHeight-1, BWHITE, "Two", console};

    int windowShowing = 1;
    while(windowShowing){

        Clear(console);
        UpdateKeyState(console);

        if(console->keys['Q']) windowShowing = 0;


        DrawWindow(memoryWindow);
        //DrawWindow(memoryWindow2);


        Draw(console);
    }
        
    return;
}
