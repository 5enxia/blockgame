#include <ncurses.h>

#include <random>
#include <future>

MEVENT me;
int px = 40;
const int P_Y = 23;
bool is_playing, has_ball;
double bx, by, vx, vy;

void setup();
void update();
void draw();

int main(void)
{
    initscr();
    setup();
    while (true) {
        update();
        draw();
    }
    endwin();
}

void setup() {
    // NO SHOW INPUT KEY
    noecho();
    // NO SHOW INPUT KEY
    curs_set(0);
    // GET MOUSE EVENT (XTERM)
    keypad(stdscr, TRUE);
    // GET MOUSE EVENT (XTERM)
    mousemask(ALL_MOUSE_EVENTS, NULL);
}

void update() {
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

void draw() {
}

