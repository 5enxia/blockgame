class Vec2i {
public:
    int x, y;

public:
    Vec2i(int x, int y) {
       this->x = x;
       this->y = y;
    }

    void operator +=(Vec2i &p) {
        this->x += p.x;
        this->y += p.y;
    }
};
