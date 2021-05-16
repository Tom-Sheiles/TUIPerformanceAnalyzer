#include <stdio.h>

#include "include.h"
#include "ProcessWindow.h"
#include "MemoryWindow.h"


int main(int argc, char **argv)
{
    Process selected;

    if(argc > 1)
    {
        STARTUPINFO info={sizeof(info)};
        PROCESS_INFORMATION processInfo;
        if (CreateProcess(argv[1], "", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &info, &processInfo))
        {
            printf("opened with pid %d\n", processInfo.dwProcessId);
            chopStr(argv[1], 2);
            strcpy(selected.processName, argv[1]);
            selected.processID = processInfo.dwProcessId;
        }
        else{
            printf("could not open %s", argv[1]);
            return;
        }

    }

    Console console;
    CreateConsole(&console);

    if(argc <= 1) selected = RunProcessList(&console);
    RunMemoryWindow(&console, &selected);

    FreeConsoleMemory(&console);
}