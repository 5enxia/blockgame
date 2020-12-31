#include <ncurses.h>

#include <random>
#include <future>


int main(void)
{
    initscr();
    // NO SHOW INPUT KEY
    noecho();
    // NO SHOW INPUT KEY
    curs_set(0);
    // GET MOUSE EVENT (XTERM)
    keypad(stdscr, TRUE);
    // GET MOUSE EVENT (XTERM)
    mousemask(ALL_MOUSE_EVENTS, NULL);

    MEVENT me;
    int px = 2;

    while (true) {
        int ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_MOUSE) {
            if (getmouse(&me) == OK) {
                clear();
                px = me.x;
                if (px < 2)px = 2;
                if (px > 77)px = 77;
                mvprintw(23, px - 2, "-----");
                refresh();
            }
        }
    }
    endwin();
}

void setup () {
}
