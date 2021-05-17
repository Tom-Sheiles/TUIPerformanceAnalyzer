#include "ProcessWindow.h"
#include "include.h"


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


Process RunProcessList(Console *console)
{
    Window processWindow = {console->bufferWidth * 0.3, 0, (console->bufferWidth - (console->bufferWidth*0.3) -console->bufferWidth*0.3), console->bufferHeight-2, BWHITE, "Processes", console};

    int numberOfProcesses = 0;
    Process *processes = NULL;
    char **menuItems = GetProcesseslist(&numberOfProcesses, &processes);

    qsort(menuItems, numberOfProcesses, sizeof(char *), alphaSort);
    qsort(processes, numberOfProcesses, sizeof(Process), processSort);

    Menu processesMenu = {5, 0, BWHITE, BACKGROUND_GREEN, numberOfProcesses};
    ConsoleCreateMenu(&processesMenu, menuItems, console, MENU_NOWRAP);

    processesMenu.selected = 14;

    int windowShowing = 1;
    int ticks = 0;
    while(windowShowing)
    {
        Clear(console);
        UpdateKeyState(console);

        if(console->keys['Q'])
        {
            console->running = FALSE;
            windowShowing = 0;
            CleanupProcessMemory(menuItems, processes, numberOfProcesses);
            FreeConsoleMemory(console);
            exit(1);
        }

        if(console->keys['D'] && processesMenu.selected <= numberOfProcesses-2)
            {processesMenu.selected ++; processesMenu.y--;}
        if(console->keys['E'] && processesMenu.selected > 0)
            {processesMenu.selected --; processesMenu.y++;}

        if(console->keys['A'])
        {
            windowShowing = 0;
        }

        if(ticks % 50 == 0){
            if(GetAsyncKeyState('S') && processesMenu.selected <= numberOfProcesses-2)
            {processesMenu.selected ++; processesMenu.y--;}
            if(GetAsyncKeyState('W') && processesMenu.selected > 0)
            {processesMenu.selected --; processesMenu.y++;}
        }

        DrawString(console, "[ ] Select Process", 3, 2, BWHITE);
        DrawString(console, "[ ] Move Selection Up", 3, 4, BWHITE);
        DrawString(console, "[ ] Move Selection Down", 3, 6, BWHITE);
        DrawString(console, "[ ] Quit", 3, 8, BWHITE);

        PlaceChar(console, 'A', 4, 2, LGREEN);
        PlaceChar(console, 'W', 4, 4, LBLUE);
        PlaceChar(console, 'S', 4, 6, LBLUE);
        PlaceChar(console, 'Q', 4, 8, LRED);

        DrawWindow(processWindow);
        DrawMenuWindow(&processWindow, &processesMenu);

        Draw(console);
        ticks++;
    }

    Process selectedProc = processes[processesMenu.selected];
    CleanupProcessMemory(menuItems, processes, numberOfProcesses);
    return selectedProc;
        
}







