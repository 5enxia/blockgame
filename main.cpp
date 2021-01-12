#include <ncurses.h>

#include <random>
#include <future>
#include <chrono>

#include "Vec2d.hpp"
#include "Vec2i.hpp"

#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)

using namespace std;

const char *BALL = "@";
const char *PADDLE = "-----";
const int W = 80;
const int H = 24;
const int FPS = 30;
const int PADDLE_SIZE = 5;
const int PADDLE_SIZE_HALF = 2;

bool isPlaying, hasBall;
MEVENT me;
Vec2i p(40, H - 1);
Vec2d b(0, 0), v(0, 0);
mt19937 mt;
uniform_real_distribution<double> ud(0.0, 1.0);
const int BSTX = 10, BSTY = 2;
const int BSZX = 60, BSZY = 5;
bool blocks[BSZY][BSZX];
enum {
    PADDLE_COLOR = 1,
    BALL_COLOR,
    BLOCK_COLOR,
    WALL_COLOR
};

void setup();
void update();
void draw();
void loop();

void init();
void end();

void initColorPair();

void keyPressed();

void checkPaddleCollision();
void checkBlockCollision();

void moveBall();

void drawPaddle();
void drawBall();
void drawBlock();
void drawWall();


int main(void)
{
    init();
    setup();
    thread game(loop);
    while (isPlaying) keyPressed();
    game.join();
    end();
    exit(1);
}

void init() {
    initscr();
    start_color(); // COLOR MODE ON
    initColorPair();
    noecho(); // NO SHOW INPUT KEY
    curs_set(0); // NO SHOW INPUT KEY
    keypad(stdscr, TRUE); // GET MOUSE EVENT (XTERM)
    mousemask(ALL_MOUSE_EVENTS, NULL); // GET MOUSE EVENT (XTERM)
}

void initColorPair() {
    init_pair(PADDLE_COLOR, COLOR_RED, COLOR_BLACK);
    init_pair(BALL_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLOCK_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(WALL_COLOR, COLOR_BLUE, COLOR_BLACK);
}

void end() {
    endwin();
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
    attron(COLOR_PAIR(PADDLE_COLOR));
    drawPaddle();
    attron(COLOR_PAIR(BALL_COLOR));
    drawBall();
    attron(COLOR_PAIR(BLOCK_COLOR));
    drawBlock();
    attron(COLOR_PAIR(WALL_COLOR));
    drawWall();
    refresh();
}

void loop() {
    int ms = static_cast<int>(1000 / FPS);
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
            if (p.x < PADDLE_SIZE_HALF + 1) p.x = PADDLE_SIZE_HALF + 1;
            break;
        case KEY_RIGHT:
            p.x += 1;
            if (p.x > W - PADDLE_SIZE_HALF - 2) p.x = W - PADDLE_SIZE_HALF - 2;
            break;
        case KEY_MOUSE:
            switch (getmouse(&me)) {
                case OK:
                    p.x = me.x;
                    if (p.x < PADDLE_SIZE_HALF) p.x = PADDLE_SIZE_HALF;
                    if (p.x > W - PADDLE_SIZE_HALF - 1) p.x = W - PADDLE_SIZE_HALF - 1;
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
    if (b.y < H - 1 || b.x < (p.x - PADDLE_SIZE_HALF) || (p.x + PADDLE_SIZE_HALF + 1) < b.x) return;
    b.y = H - 1;
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
    if (b.x < 1) {
        b.x = 1;
        v.x = -v.x;
    }
    // by
    if (b.y < 1) {
        b.y = 1;
        v.y = -v.y;
    }
    // bx
    if (b.x > W - 2) {
        b.x = W - 2;
        v.x = -v.x;
    }
    // by
    if (b.y > H){
        b.y = H;
        hasBall = true;
    }
}

void drawPaddle() {
    mvprintw(p.y , p.x - PADDLE_SIZE_HALF , PADDLE);
}

void drawBall() {
    int x = static_cast<int>(b.x);
    int y = static_cast<int>(b.y);
    if (hasBall) mvprintw(p.y - 1, p.x, BALL);
    else mvprintw(y, x, BALL);
}

void drawBlock() {
    rep(i,BSZY) {
        rep(j,BSZX) {
            if(blocks[i][j]) mvprintw(BSTY+i, BSTX+j, "+");
        }
    }
}

void drawWall() {
    rep(i, H) mvprintw(i, 0, "|");
    rep(i, H) mvprintw(i, W - 1, "|");
    rep(i, W) mvprintw(0, i, "-");
}
