#include <ncurses.h>

#include <random>
#include <future>
#include <chrono>

using namespace std;

struct vec2i {
    int x;
    int y;
};

struct vec2d {
    double x;
    double y;
};

const string BALL = "*";
const string PADDLE = "-----";

bool isPlaying, hasBall;

MEVENT me;

vec2i p;
vec2d b, v;

void setup();
void update();
void draw();
void loop();

void keyPressed();

auto game = thread([] { loop(); });

mt19937 mt;
uniform_real_distribution<double> ud(0.0, 1.0);

int main(void)
{
    initscr();

    setup();
    while (true) {
        update();
        draw();
    }

    isPlaying = false;
    game.join();
    endwin();
    exit(1);
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

    p = { 40, 23 };
}

void update() {
    keyPressed();
}

void draw() {
    clear();

    int x = static_cast<int>(b.x);
    int y = static_cast<int>(b.y);
    mvprintw(p.y , p.x - 2 , PADDLE);

    if (hasBall) mvprintw(b.y, b.x, BALL);
    else mvprintw(y, x, BALL);

    refresh();
}

void loop() {
    while (isPlaying) {
        update();
        draw();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void keyPressed() {
    int ch = getch();
    if (ch == 'q') break;
    switch (ch) {
        case ' ':
            if(hasBall) {
                hasBall = false;
                b.x = p.x;
                b.y = p.y - 1;
                double theta = (ud(mt) / 2 + 0.25) * M_PI;
                v.x = cos(theta) / 2;
                v.y = sin(theta) / 2;
            }
            break;
        case KEY_MOUSE:
            switch (getmouse(&me)) {
                case OK:
                    p.x = me.x;
                    if (px < 2) px = 2;
                    if (px > 2) px = 77;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
