#include <ncurses.h>
#include <string.h>

void myMvprintw(int row, int col, char* message) {
    mvprintw(row, col, message);
}
