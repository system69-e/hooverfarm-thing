#pragma once
#include <cstdio>

void wait() {
	printf("Press enter to continue...\n");
	getchar(); // wait for the user to press enter
}

void clear() {
	// clear console without system("cls")
	printf("\033[2J\033[1;1H");
}