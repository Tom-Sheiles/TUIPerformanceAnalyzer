
#ifndef _PROCESS_WINDOW_H
#define _PROCESS_WINDOW_H

#include <CConsole.h>
#include <TlHelp32.h>


typedef struct Process
{
    char processName[255];
    DWORD processID;

} Process;


void CleanupProcessMemory(char **menuItems, Process *processes, int numberOfItems);
char **GetProcesseslist(int *numberOfProcesses, Process **out_porcesess);
void RunProcessList();

#endif /*_PROCESS_WINDOW_H */