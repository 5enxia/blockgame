#include <ncurses.h>

#include <random>
#include <future>
#include <chrono>

#include "Vec2d.hpp"
#include "Vec2i.hpp"

#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)

using namespace std;

const char *BALL = "*";
const char *PADDLE = "-----";
const int W = 80;
const int H = 24;

bool isPlaying, hasBall;
MEVENT me;
Vec2i p(40, 23);
Vec2d b(0, 0), v(0, 0);
mt19937 mt;
uniform_real_distribution<double> ud(0.0, 1.0);
const Vec2i BLOCK_START(10, 2), BLOCK_SIZE(60, 5);
const int BSTX = 10, BSTY = 2;
const int BSZX = 60, BSZY = 5;
bool blocks[BSZY][BSZX];

void init();

void setup();
void update();
void draw();
void loop();

void keyPressed();

void checkPaddleCollision();
void checkBlockCollision();
void moveBall();
void drawBlock();


int main(void)
{
    init();
    setup();
    thread game(loop);
    while (isPlaying) keyPressed();
    game.join();
    endwin();
    exit(1);
}

void init() {
    initscr();
    noecho(); // NO SHOW INPUT KEY
    curs_set(0); // NO SHOW INPUT KEY
    keypad(stdscr, TRUE); // GET MOUSE EVENT (XTERM)
    mousemask(ALL_MOUSE_EVENTS, NULL); // GET MOUSE EVENT (XTERM)
}

void setup() {
    isPlaying = true;
    hasBall = true;
    rep(i,BSZY) {
        rep(j,BSZX) {
            blocks[i][j] = true;
        }
    }
}

void update() {
    moveBall();
    checkPaddleCollision();
    checkBlockCollision();
}

void draw() {
    clear();
    if (hasBall) mvprintw(p.y - 1, p.x, BALL);
    mvprintw(p.y , p.x - 2 , PADDLE);
    int x = static_cast<int>(b.x);
    int y = static_cast<int>(b.y);
    if (!hasBall) mvprintw(y, x, BALL);
    drawBlock();
    refresh();
}

void loop() {
    int fps = 30;
    int ms = static_cast<int>(1000 / fps);
    while (isPlaying) {
        update();
        draw();
        this_thread::sleep_for(chrono::milliseconds(ms));
    }
}

void keyPressed() {
    switch (getch()) {
        case 'q':
            isPlaying = false;
            break;
        case ' ':
            if(hasBall) {
                hasBall = false;
                b.x = p.x;
                b.y = p.y - 1;
                double theta = (ud(mt) / 2 + 0.25) * M_PI;
                v.x = cos(theta) / 2;
                v.y = -sin(theta) / 2;
            }
            break;
        case KEY_LEFT:
            p.x -= 1;
            if (p.x < 2) p.x = 2;
            break;
        case KEY_RIGHT:
            p.x += 1;
            if (p.x > 77) p.x = 77;
            break;
        case KEY_MOUSE:
            switch (getmouse(&me)) {
                case OK:
                    p.x = me.x;
                    if (p.x < 2) p.x = 2;
                    if (p.x > 77) p.x = 77;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void checkPaddleCollision() {
    if (b.y < 23 || b.x < (p.x - 2) || (p.x + 3) < b.x) return;
    b.y = 23;
    double x = static_cast<double>(p.x);
    double theta = M_PI * ((x - b.x + 1.5) / 8 + 0.25);
    v.x = cos(theta) / 2;
    v.y = -sin(theta) / 2;
}

void checkBlockCollision() {
    if (b.x < BSTX || BSTX + BSZX < b.x) return;
    if (b.y < BSTY || BSTY + BSZY < b.y) return;
    int y = static_cast<int>(b.y) - BSTY;
    int x = static_cast<int>(b.x) - BSTX;
    if(!blocks[y][x]) return;
    blocks[y][x] = false;
    double dx = b.x - x + 0.5;
    double dy = b.y - y + 0.5;
    if (abs(dx) < abs(dy)) v.x = -v.x;
    else v.y = -v.y;
}

void moveBall() {
    if (hasBall) return;
    b += v;
    // bx
    if (b.x < 0) {
        b.x = 0;
        v.x = -v.x;
    }
    // by
    if (b.y < 0) {
        b.y = 0;
        v.y = -v.y;
    }
    // bx
    if (b.x > W) {
        b.x = W;
        v.x = -v.x;
    }
    // by
    if (b.y > H){
        b.y = H;
        hasBall = true;
    }
}

void drawBlock() {
    rep(i,BSZY) {
        rep(j,BSZX) {
            if(blocks[i][j]) mvprintw(BSTY+i, BSTX+j, "+");
        }
    }
}
