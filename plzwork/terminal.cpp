#include "terminal.h"
#include <Windows.h>

void wait() {
	printf("Press enter to continue...\n");
	getchar(); // wait for the user to press enter
}

void clear() {
	// clear screen
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, ' ', cells, {0,0}, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, { 0,0 }, &written);
    SetConsoleCursorPosition(console, {0,0});
	
}