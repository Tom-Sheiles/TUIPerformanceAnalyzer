#include <CConsole.h>

#include <stdio.h>

int main()
{
    Console console;
    CreateConsole(&console);

    Window window = {2, 0, console.bufferWidth-(console.bufferWidth*0.4), console.bufferHeight-(console.bufferHeight*0.4), BWHITE, "Memory", &console};

    Menu menu = {5, 5, BWHITE, BACKGROUND_GREEN | BACKGROUND_INTENSITY, 5};
    const char *items[] = {"ONE", "TWO", "THREE", "FOUR", "FIVE"};

    ConsoleCreateMenu(&menu, items, &console, MENU_WRAP);

    while(console.running)
    {
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(console.hConsole, &cci);
        cci.bVisible = FALSE;
        SetConsoleCursorInfo(console.hConsole, &cci);

        Clear(&console);
        UpdateKeyState(&console);
        if(console.keys['Q']) console.running = FALSE;

        if(console.keys['S'])
        {
            //if(menu.selected < menu.numberOfItems-1)menu.y --;
            menu.selected ++;
        }
        if(console.keys['W']){
            //if(menu.selected > 0)menu.y++;
            menu.selected --;
        }

        //DrawWindow(window);

        DrawMenu(&menu);

        Draw(&console);
    }

    FreeConsoleMemory(&console);
}