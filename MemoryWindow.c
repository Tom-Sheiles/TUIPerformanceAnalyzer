#include "MemoryWindow.h"
#include <psapi.h>
#include <stdio.h>


void RunMemoryWindow(Console *console, Process *process)
{
    int width = console->bufferWidth * 0.6;
    int height = console->bufferHeight*0.70;

    Window memoryWindow = {1, 0, width, height, BWHITE, process->processName, console};
    //Window memoryWindow2 = {console->bufferWidth*0.6+4, 0, (console->bufferWidth-(console->bufferWidth*0.6+5))-3, console->bufferHeight-1, BWHITE, "Two", console};

    // Read Process memory
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    int status = 0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process->processID);
    if(hProcess == NULL) status = 1;
    if( !(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) ) status = 1;

    // Calculate initial Memory
    float maxMb = (pmc.PeakWorkingSetSize / 1000000) + 50;
    float currentMb = pmc.WorkingSetSize / 1000000;
    float minMb = 0;
    double MbPercent = currentMb / maxMb;

    // Get memory Strings
    char maxMbStr[1024];
    char currentMbStr[1024];
    char minMbStr[1024];
    sprintf(maxMbStr, "%.2f MB", maxMb);
    sprintf(currentMbStr, "%.2f MB", currentMb);
    sprintf(minMbStr, "%.2f MB", minMb);


    int index = 0;
    int windowShowing = 1;
    
    while(windowShowing){

        if(!index) Clear(console);
        UpdateKeyState(console);

        if(console->keys['Q']) windowShowing = 0;

        if(status == 0){
            DrawWindow(memoryWindow);

            DrawString(console, maxMbStr, (memoryWindow.x + memoryWindow.w)+1, 1, LGREEN);
            DrawString(console, currentMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h/2, LGREEN);
            DrawString(console, minMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h-1, LGREEN);

            WindowFillRect(&memoryWindow, FULL_PIXEL, index, memoryWindow.h -(memoryWindow.h* (MbPercent)), 2, memoryWindow.h, LGREEN);
        }
        else{
            DrawString(console, "Error: Could not open process!", 5, 1, LRED);
        }

        if( !(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) ) status = 1;

         // Calculate initial Memory
        maxMb = (pmc.PeakWorkingSetSize / 1000000) + 50;
        currentMb = pmc.WorkingSetSize / 1000000;
        MbPercent = currentMb / maxMb;

        // Get memory Strings
        sprintf(maxMbStr, "%.2f MB  ", maxMb);
        sprintf(currentMbStr, "%.2f MB   ", currentMb);
        sprintf(minMbStr, "%.2f MB    ", minMb);


        index += 3;
        if(index >= width) index = 0;

        Draw(console);
        Sleep(1000);
    }
        
    return;
}
