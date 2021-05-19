#ifndef _MEMORY_WINDOW_H
#define _MEMORY_WINDOW_H

#include <CConsole.h>
#include "ProcessWindow.h"


typedef struct MemoryInfo
{
    // Memory Values
    float maxMb;
    float currentMb;
    float minMb;
    double MbPercent;

    WORD pageFaultCount;
    float PeakMemory;
    float pagedPool; 

    // Memory Strings
    char maxMbStr[1024];
    char currentMbStr[1024];
    char minMbStr[1024];

    char pageFaultCountStr[1024];
    char PeakMemoryStr[1024];
    char pagedPoolStr[1024]; 

} MemoryInfo;

void RunMemoryWindow(Console *console, Process *process);

#endif