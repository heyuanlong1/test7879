#include "printfc.h"

int main(void)
{
	printfc(FG_RED, "red color text\n");
	printfc(FG_GREEN, "green color text\n");
	printfc(FG_YELLOW, "yellow color text\n");
	printfc(FG_BLUE, "blue color text\n");
	printfc(FG_PURPLE, "purple color text\n");
	printfc(FG_DARKGREEN, "dark green color text\n");
	printfc(FG_WHITE, "white color text\n\n");

	printfc(FG_BLACK, BG_WHITE, "black color text on white");
	printfc(FG_RED, BG_BLACK, "red color text on black\n");
	printfc(FG_GREEN, BG_YELLOW, "green color text on yellow\n");
	printfc(FG_YELLOW, BG_BLUE, "yellow color text on blue\n");
	printfc(FG_BLUE, BG_WHITE, "blue color text\n on white\n");
	printfc(FG_PURPLE, BG_WHITE, "purple color text\n on white\n");
	printfc(FG_DARKGREEN, BG_DARKRED, "dark green color text\n on dark red\n");
	printfc(FG_WHITE, BG_BLUE, "white color text\n on blue\n");

	puts("\nPress any key to continue...");
	getchar();
	return 0;
}
