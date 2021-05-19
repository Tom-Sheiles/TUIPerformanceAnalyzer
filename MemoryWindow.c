#include "MemoryWindow.h"
#include <psapi.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct MemoryInfo
{
    // Memory Values
    float maxMb;
    float currentMb;
    float minMb;
    double MbPercent;

    // Memory Strings
    char maxMbStr[1024];
    char currentMbStr[1024];
    char minMbStr[1024];

} MemoryInfo;



int UpdateMemoryInfo(MemoryInfo *memory, HANDLE hProc, PROCESS_MEMORY_COUNTERS *pmc)
{
    if( !(GetProcessMemoryInfo(hProc, pmc, sizeof(*pmc))) ) return 1;
    
     // Calculate initial Memory
    memory->maxMb = (pmc->PeakWorkingSetSize / 1000000) + 50;
    memory->currentMb = pmc->WorkingSetSize / 1000000;
    memory->minMb = 0;
    memory->MbPercent = memory->currentMb / memory->maxMb;

    // Get memory Strings
    sprintf(memory->maxMbStr, "%.2f MB ", memory->maxMb);
    sprintf(memory->currentMbStr, "%.2f MB ", memory->currentMb);
    sprintf(memory->minMbStr, "%.2f MB ", memory->minMb);

    return 0;
}



void RunMemoryWindow(Console *console, Process *process)
{
    int width = console->bufferWidth-10;
    int height = console->bufferHeight*0.30;

    Window memoryWindow = {1, 0, width, height, BWHITE, process->processName, console};
    Window CPUWindow = {1+width*0.25, height+1, width-(width*0.25), height, BWHITE, "CPU Usage", console};
    Window win3 = {1, height+1, width - (width*0.75)-2, height*0.75/2, BWHITE, "Page Faults", console};
    Window win4 = {1, ((height+1)+height*0.75/2)+1, width - (width*0.75)-2, height*0.75/2, BWHITE, "Temperatures", console};

    //Window memoryWindow2 = {console->bufferWidth*0.6+12, 0, (console->bufferWidth-(console->bufferWidth*0.6+5))-11, console->bufferHeight-1, BWHITE, "Two", console};

    // Read Process memory
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    MemoryInfo processMemory;

    int status = 0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process->processID);
    status = UpdateMemoryInfo(&processMemory, hProcess, &pmc);

   

    int index = 0;
    int windowShowing = 1;
    int ticks = 0;
    
    while(windowShowing){

        UpdateKeyState(console);

        if(console->keys['Q']) windowShowing = 0;


        if(ticks % 1000 == 0){
            if(!index) Clear(console);

            if(status == 0){
                DrawWindow(memoryWindow);
                DrawWindow(CPUWindow);
                DrawWindow(win3);
                DrawWindow(win4);

                DrawString(console, process->processName, 2, 0, YELLOW);

                DrawString(console, processMemory.maxMbStr, (memoryWindow.x + memoryWindow.w)+1, 1, YELLOW);
                DrawString(console, processMemory.currentMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h/2, YELLOW);
                DrawString(console, processMemory.minMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h-1, YELLOW);

                WindowFillRect(&memoryWindow, FULL_PIXEL, index, memoryWindow.h -(memoryWindow.h* (processMemory.MbPercent)), 2, memoryWindow.h, LGREEN);
            }
            else{
                DrawString(console, "Error: Could not open process!", 5, 1, LRED);
            }


            status = UpdateMemoryInfo(&processMemory, hProcess, &pmc);


            index += 3;
            if(index >= width) index = 0;
        }

        ticks++;
        Draw(console);
    }
        
    return;
}
