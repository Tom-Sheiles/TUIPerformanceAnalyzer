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

#endif /* _INCLUDES_H */