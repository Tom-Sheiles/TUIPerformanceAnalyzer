#include "include.h"
#include "ProcessWindow.h"
#include "MemoryWindow.h"


int main(int argc, char **argv)
{
    Console console;
    CreateConsole(&console);
    
    RunProcessList(&console);
    RunMemoryWindow(&console);

    FreeConsoleMemory(&console);
}