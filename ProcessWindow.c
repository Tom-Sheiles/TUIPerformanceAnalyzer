#include "ProcessWindow.h"


void CleanupProcessMemory(char **menuItems, Process *processes, int numberOfItems)
{
    for(int i = 0; i < numberOfItems; i++)
    {
        free(menuItems[i]);
    }
    free(menuItems);
    free(processes);
}


char **GetProcesseslist(int *numberOfProcesses, Process **out_porcesess)
{
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    while(Process32Next(hProcSnap, &procEntry))
    {
        (*numberOfProcesses)++;
    }
    (*numberOfProcesses) -= 1;
    Process32First(hProcSnap, &procEntry);

    //
    // MEMORY LEAKS FIX THIS
    //
    Process *processes = (Process *)malloc(sizeof(Process) * (*numberOfProcesses));
    char **menuNames = malloc(sizeof(char *) * (*numberOfProcesses));

    int i = 0;
    while(Process32Next(hProcSnap, &procEntry))
    {
        strcpy(processes[i].processName, procEntry.szExeFile);
        processes[i].processID = procEntry.th32ProcessID;

        menuNames[i] = malloc(255 * sizeof(char));
        strcpy(menuNames[i], procEntry.szExeFile);
        i++;
    }

    *out_porcesess = processes;

    return menuNames;
}