#include "MemoryWindow.h"
#include "include.h"
#include <psapi.h>
#include <stdio.h>
#include <inttypes.h>


int UpdateMemoryInfo(MemoryInfo *memory, HANDLE hProc, PROCESS_MEMORY_COUNTERS *pmc)
{
    if( !(GetProcessMemoryInfo(hProc, pmc, sizeof(*pmc))) ) return 1;
    
     // Calculate initial Memory
    memory->maxMb = (pmc->PeakWorkingSetSize/1024/1024) + 50;
    memory->currentMb = pmc->WorkingSetSize/1024/1024;
    memory->minMb = 0;
    memory->MbPercent = memory->currentMb / memory->maxMb;

    memory->pageFaultCount = pmc->PageFaultCount/1024/1024;
    memory->PeakMemory = pmc->PagefileUsage/1024/1024;
    memory->pagedPool = pmc->QuotaPeakPagedPoolUsage/1024/1024;

    // Get memory Strings
    sprintf(memory->maxMbStr, "%.2f MB ", memory->maxMb);
    sprintf(memory->currentMbStr, "%.2f MB ", memory->currentMb);
    sprintf(memory->minMbStr, "%.2f MB ", memory->minMb);

    sprintf(memory->pageFaultCountStr, "%d", memory->pageFaultCount);
    sprintf(memory->PeakMemoryStr, "%.2f MB", memory->PeakMemory);
    sprintf(memory->pagedPoolStr, "%.2f MB", memory->pagedPool);

    return 0;
}



void RunMemoryWindow(Console *console, Process *process)
{
    int width = console->bufferWidth-10;
    int height = console->bufferHeight*0.30;

    Window memoryWindow = {1, 0, width, height, BWHITE, process->processName, console};
    Window CPUWindow = {1+width*0.25, height+1, width-(width*0.25), height+1, BWHITE, "CPU Usage", console};
    Window PageFaultWindow = {1, height+1, width - (width*0.75)-2, height/2, BWHITE, "Page Faults", console};
    Window TemperatureWindow = {1, height+2+height/2, width - (width*0.75)-2, height/2, BWHITE, "Temperatures", console};


    // Read Process memory
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    MemoryInfo processMemory;

    int status = 0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process->processID);
    status = UpdateMemoryInfo(&processMemory, hProcess, &pmc);

   

    int memoryIndex = 0;
    int CPUIndex = 0;
    int windowShowing = 1;
    int ticks = 0;
    
    while(windowShowing){

        UpdateKeyState(console);

        if(console->keys['Q']) windowShowing = 0;


        if(ticks % 1000 == 0){
            if(!memoryIndex) Clear(console);

            if(status == 0){
                DrawWindow(memoryWindow);
                DrawWindow(CPUWindow);
                DrawWindow(PageFaultWindow);
                DrawWindow(TemperatureWindow);

                DrawString(console, processMemory.maxMbStr, (memoryWindow.x + memoryWindow.w)+1, 1, YELLOW);
                DrawString(console, processMemory.currentMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h/2, YELLOW);
                DrawString(console, processMemory.minMbStr, (memoryWindow.x + memoryWindow.w)+1, memoryWindow.h-1, YELLOW);

                //DrawString(console, "Page Faults", )
                DrawStringWindow(&PageFaultWindow, "Page Faults", 0, 1, YELLOW);
                DrawStringWindow(&PageFaultWindow, "Peak Memory", 0, 3, YELLOW);
                DrawStringWindow(&PageFaultWindow, "Paged Pool", 0, 5, YELLOW);

                DrawStringWindow(&PageFaultWindow, processMemory.pageFaultCountStr, PageFaultWindow.w-5, 1, YELLOW);
                DrawStringWindow(&PageFaultWindow, processMemory.PeakMemoryStr, PageFaultWindow.w-10, 3, YELLOW);
                DrawStringWindow(&PageFaultWindow, processMemory.pagedPoolStr, PageFaultWindow.w-10, 5, YELLOW);


                float cpuPercent = GetCPULoad();
                char cpuStr[1024];
                sprintf(cpuStr, "%.2f%%", cpuPercent*100);

                PlaceCharWindow(&CPUWindow, '.', CPUIndex, CPUWindow.h - (CPUWindow.h * cpuPercent), LRED);
                DrawString(console, cpuStr, CPUWindow.x + CPUWindow.w+1, CPUWindow.y + (CPUWindow.h/2), YELLOW);

                WindowFillRect(&memoryWindow, FULL_PIXEL, memoryIndex, memoryWindow.h -(memoryWindow.h* (processMemory.MbPercent)), 2, memoryWindow.h, LGREEN);
            }
            else{
                DrawString(console, "Error: Could not open process!", 5, 1, LRED);
            }

            status = UpdateMemoryInfo(&processMemory, hProcess, &pmc);

            memoryIndex += 3;
            CPUIndex ++;
            if(memoryIndex >= width){
                memoryIndex = 0;
                CPUIndex = 0;
            }
        }

        ticks++;
        Draw(console);
    }
        
    return;
}
