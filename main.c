#include <stdio.h>
#include <CConsole.h>
#include <TlHelp32.h>

#include "include.h"

typedef struct Process
{
    char processName[255];
    DWORD processID;
    //DWORD currentThreads;

} Process;



int getNRunningProcesses(HANDLE *hProcSnap, PROCESSENTRY32 *procEntry)
{
    procEntry->dwSize = sizeof(PROCESSENTRY32);
    *hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    int numberOfRunningProcesses = 0;
    while(Process32Next(*hProcSnap, procEntry))
    {
        numberOfRunningProcesses++;
    }
    numberOfRunningProcesses--;
    Process32First(*hProcSnap, procEntry);

    return numberOfRunningProcesses;
}


//
// Could be made more complex later. currently all processes are treated equally. 
// Windows process can be identified with the IsProcessCritical function and values
// sorted based on this
//
void listProcesses(int numberOfProcesses, HANDLE hProcSnap, PROCESSENTRY32 *procEntry, Process *processes)
{
    int i = 0;
    while(Process32Next(hProcSnap, procEntry))
    {
        strcpy(processes[i].processName, procEntry->szExeFile);
        processes[i].processID = procEntry->th32ProcessID;

        //strcpy(menuNames[i], procEntry->szExeFile);
        i++;
    }

    return;
}


void freeProcessLists(char **menuItems, int numberOfProcesses)
{
    for(int i = 0; i < numberOfProcesses; i++)
    {
        free(menuItems[i]);
    }
    free(menuItems);
    //free(processes);
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

//new
int main()
{
    Console console;
    CreateConsole(&console);

    int currentWindow = PROCESSES_LIST;

    // PROCESSES LIST
        Window processWindow = {console.bufferWidth * 0.3, 0, (console.bufferWidth - (console.bufferWidth*0.3) -console.bufferWidth*0.3), console.bufferHeight-2, BWHITE, "Processes", &console};

        HANDLE hProcSnap = NULL;
        PROCESSENTRY32 procEntry;

        int numberOfProcesses = getNRunningProcesses(&hProcSnap, &procEntry);
        
        Process *processes = calloc(numberOfProcesses, sizeof (Process *));
        char **menuItems = malloc(sizeof(char *) * numberOfProcesses);


        for(int i = 0; i < numberOfProcesses; i++){
            menuItems[i] = malloc(PROCESS_NAME_LENGTH * sizeof(char *));
            strcpy(menuItems[i], "hello world");
        }

        listProcesses(numberOfProcesses, hProcSnap, &procEntry, processes);
        

        // Processes are listed by PID by default
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

    //freeProcessLists(menuItems, numberOfProcesses);
    FreeConsoleMemory(&console);
}