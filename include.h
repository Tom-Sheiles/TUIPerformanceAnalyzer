#ifndef _INCLUDES_H
#define _INCLUDES_H


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

#endif /* _INCLUDES_H */