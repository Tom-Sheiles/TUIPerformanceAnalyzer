#include "MemoryWindow.h"

void RunMemoryWindow(Console *console)
{
    Window memoryWindow = {1, 0, console->bufferWidth * 0.6, console->bufferHeight*0.70, BWHITE, "Memory", console};
    Window memoryWindow2 = {console->bufferWidth*0.6+4, 0, (console->bufferWidth-(console->bufferWidth*0.6+5))-3, console->bufferHeight-1, BWHITE, "Two", console};

    int windowShow = 1;
    while(windowShow){

        Clear(console);
        UpdateKeyState(console);

        if(console->keys['Q']) windowShow = 0;

        DrawWindow(memoryWindow);
        DrawWindow(memoryWindow2);

        WindowFillRect(&memoryWindow, FULL_PIXEL, 0, memoryWindow.h/2, 3, memoryWindow.h, LGREEN);
        WindowFillRect(&memoryWindow, FULL_PIXEL, 3, memoryWindow.h/2-1, 6, memoryWindow.h, LGREEN);
        WindowFillRect(&memoryWindow, FULL_PIXEL, 9, memoryWindow.h/2-5, 3, memoryWindow.h, LGREEN);

        Draw(console);
    }
        
    return;
}
