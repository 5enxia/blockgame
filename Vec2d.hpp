class Vec2d {
public:
    double x, y;

public:
    Vec2d(double x, double y) {
       this->x = x;
       this->y = y;
    }

    void operator +=(Vec2d &p) {
        this->x += p.x;
        this->y += p.y;
    }
};
