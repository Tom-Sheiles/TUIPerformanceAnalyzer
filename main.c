#include <stdio.h>
#include <CConsole.h>
#include <TlHelp32.h>

#include "include.h"

typedef struct Process
{
    char processName[255];
    DWORD processID;

} Process;

//
// Could be made more complex later. currently all processes are treated equally. 
// Windows process can be identified with the IsProcessCritical function and values
// sorted based on this
//
char **GetProcesseslist(Console *console, int *numberOfProcesses, Process **out_porcesess)
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


void CleanupProcessMemory(char **menuItems, Process *processes, int numberOfItems)
{
    for(int i = 0; i < numberOfItems; i++)
    {
        free(menuItems[i]);
    }
    free(menuItems);
    free(processes);
}


//
// Passed as pointer to qsort() to sort alphabetically
//
int alphaSort(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}



int main()
{
    Console console;
    CreateConsole(&console);

    int currentWindow = PROCESSES_LIST;

    // PROCESSES LIST
        Window processWindow = {console.bufferWidth * 0.3, 0, (console.bufferWidth - (console.bufferWidth*0.3) -console.bufferWidth*0.3), console.bufferHeight-2, BWHITE, "Processes", &console};

        int numberOfProcesses = 0;
        Process *processes = NULL;
        char **menuItems = GetProcesseslist(&console, &numberOfProcesses, &processes);

        qsort(menuItems, numberOfProcesses, sizeof(char *), alphaSort);

        Menu processesMenu = {5, 0, BWHITE, BACKGROUND_GREEN, numberOfProcesses};
        ConsoleCreateMenu(&processesMenu, menuItems, &console, MENU_NOWRAP);
        processesMenu.selected = 14;

    // 

    Window memoryWindow = {1, 0, console.bufferWidth * 0.6, console.bufferHeight*0.70, BWHITE, "Memory", &console};
    Window memoryWindow2 = {console.bufferWidth*0.6+4, 0, (console.bufferWidth-(console.bufferWidth*0.6+5))-3, console.bufferHeight-1, BWHITE, "Two", &console};


    int ticks = 0;

    //
    // Maybe should break the rules and split into a seperate program loop and draw call for each window to make logic more seperated.
    //
    while(console.running)
    {
        Clear(&console);
        UpdateKeyState(&console);

        // Handle user input
            if(console.keys['Q']) console.running = FALSE;

            if(console.keys['D'] && processesMenu.selected <= numberOfProcesses-2)
                {processesMenu.selected ++; processesMenu.y--;}
            if(console.keys['E'] && processesMenu.selected > 0)
                {processesMenu.selected --; processesMenu.y++;}

            if(console.keys['A']) currentWindow = MEMORY_VIEWER;

            if(ticks % 50 == 0){
                if(GetAsyncKeyState('S') && processesMenu.selected <= numberOfProcesses-2)
                {processesMenu.selected ++; processesMenu.y--;}
                if(GetAsyncKeyState('W') && processesMenu.selected > 0)
                {processesMenu.selected --; processesMenu.y++;}
            }
        //

        if(currentWindow == PROCESSES_LIST)
        {
            DrawString(&console, "[ ] Select Process", 3, 2, BWHITE);
            DrawString(&console, "[ ] Move Selection Up", 3, 4, BWHITE);
            DrawString(&console, "[ ] Move Selection Down", 3, 6, BWHITE);
            DrawString(&console, "[ ] Quit", 3, 8, BWHITE);

            PlaceChar(&console, 'A', 4, 2, LGREEN);
            PlaceChar(&console, 'W', 4, 4, LBLUE);
            PlaceChar(&console, 'S', 4, 6, LBLUE);
            PlaceChar(&console, 'Q', 4, 8, LRED);

            DrawWindow(processWindow);
            DrawMenuWindow(&processWindow, &processesMenu);
        }
        else if(currentWindow == MEMORY_VIEWER)
        {
            DrawWindow(memoryWindow);
            DrawWindow(memoryWindow2);

            WindowFillRect(&memoryWindow, FULL_PIXEL, 0, memoryWindow.h/2, 3, memoryWindow.h, LGREEN);
            WindowFillRect(&memoryWindow, FULL_PIXEL, 3, memoryWindow.h/2-1, 6, memoryWindow.h, LGREEN);
            WindowFillRect(&memoryWindow, FULL_PIXEL, 9, memoryWindow.h/2-5, 3, memoryWindow.h, LGREEN);
        }

        Draw(&console);
        ticks++;
    }

    CleanupProcessMemory(menuItems, processes, numberOfProcesses);
    FreeConsoleMemory(&console);
}