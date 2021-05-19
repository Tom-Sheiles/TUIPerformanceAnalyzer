#ifndef _INCLUDES_H
#define _INCLUDES_H

#include "ProcessWindow.h"

enum Windows{
    PROCESSES_LIST,
    MEMORY_VIEWER
};

//
// Passed as pointer to qsort() to sort alphabetically
//
inline int alphaSort(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}

inline int processSort(const void *a, const void *b)
{
    Process *proca = (Process *)a;
    Process *procb = (Process *)b;
    return strcmp(proca->processName, procb->processName);
}


inline void chopStr(char *str, size_t n)
{
    size_t len = strlen(str);
    if(n > len) return;
    memmove(str, str+n, len - n+1);
}


inline static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
   static unsigned long long _previousTotalTicks = 0;
   static unsigned long long _previousIdleTicks = 0;

   unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
   unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;

   float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);

   _previousTotalTicks = totalTicks;
   _previousIdleTicks  = idleTicks;
   return ret;
}

inline unsigned long long FileTimeToInt64(FILETIME *ft)
{
    return (((unsigned long long)(ft->dwHighDateTime))<<32)|((unsigned long long)ft->dwLowDateTime);
}


inline float GetCPULoad()
{
   FILETIME idleTime, kernelTime, userTime;
   return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(&idleTime), FileTimeToInt64(&kernelTime)+FileTimeToInt64(&userTime)) : -1.0f;
}

#endif /* _INCLUDES_H */