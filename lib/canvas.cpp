#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <emscripten.h>

using namespace std;

const int MX_SIZE = 55;

enum Direction {
    FORWARD,
    BACKWARD
};

struct Circle {
    int x; // x coordinate
    int y; // y coordinate
    int r; // radius
    
    int _r; // color - RED
    int _g; // color - GREEN
    int _b; // color - BLUE
    
    int xv; // x - axis velocity
    int yv; // y - axis velocity
    
    Direction xd; // x - axis direction 
    Direction yd; // y - axis direction 
} circles[MX_SIZE];

void _init_circles() {
    // Seed random number generator
    srand(time(NULL));

    auto _rand = [&](int n) {
        return (rand() % n);
    };

    // Create circles
    for (int i = 0; i < MX_SIZE; i++) {
        int radius = _rand(50);
        int x = _rand(1000) + radius;
        int y = _rand(1000) + radius;

        circles[i].x = x;
        circles[i].y = y;
        circles[i].r = radius;
        circles[i]._r = _rand(255);
        circles[i]._g = _rand(255);
        circles[i]._b = _rand(255);
        circles[i].xv = _rand(10);
        circles[i].yv = _rand(10);
        circles[i].xd = FORWARD;
        circles[i].yd = FORWARD;  
    }
}

// Initialize circls and call render function in JS
int main() {
    _init_circles();

    int sz = sizeof(Circle) / sizeof(int);

    // Run JS code
    EM_ASM({ render($0, $1); }, MX_SIZE * sz, sz);
}

extern "C" {
    // Return updated circles to JS
    struct Circle * get_circles(int canvas_width, int canvas_height) {
        // Update circles
        for (int i = 0; i < MX_SIZE; i++) {
            // Collision RIGHT - set x direction backwards 
            if (circles[i].x + circles[i].r >= canvas_width) {
                circles[i].xd = BACKWARD;
            }

            // Collision LEFT - set x direction forwards 
            if (circles[i].x - circles[i].r <= 0) {
                circles[i].xd = FORWARD;
            }

            // Collision TOP - set y direction forwards 
            if (circles[i].y - circles[i].r <= 0) {
                circles[i].yd = FORWARD;
            }

            // Collision BOTTOM - set y direction backwards 
            if (circles[i].y + circles[i].r >= canvas_height) {
                circles[i].yd = BACKWARD;
            }

            // Move circle in specificed direction
            int xv = circles[i].xv;
            circles[i].x += circles[i].xd == FORWARD ? xv : -xv;

            int yv = circles[i].yv;
            circles[i].y += circles[i].yd == FORWARD ? yv : -yv;
        }

        return circles;
    }
}