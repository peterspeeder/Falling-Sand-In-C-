#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define WIDTH 50
#define HEIGHT 50
#define CELL_SIZE 10

int* buffer = NULL;
#define EMPTY 0
#define WALL 1
#define SAND 2
#define STATIC 3

Color colors[] = {
    (Color){ 153, 153, 153, 255 },  // EMPTY
    (Color){ 68, 68, 68, 255 },     // WALL
    (Color){ 255, 255, 68, 255 },   // SAND
    (Color){ 128, 128, 128, 255 },  // STATIC
};

void setBuf(int x, int y, int val) {
    buffer[x + y * WIDTH] = val;
}

int getBuf(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return EMPTY;
    return buffer[x + y * WIDTH];
}

void allocateBuffer() {
    if (buffer == NULL) {
        buffer = (int*)malloc(WIDTH * HEIGHT * sizeof(int));
    }
}

void freeBuffer() {
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
}

void placeSand() {
    int x = GetMouseX() / CELL_SIZE;
    int y = GetMouseY() / CELL_SIZE;

    if (IsKeyDown(KEY_SPACE)) {
        if (getBuf(x, y) == EMPTY) {
            setBuf(x, y, SAND);
        }
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && getBuf(x, y) == EMPTY) {
        setBuf(x, y, STATIC);
    }
}

void think() {
    int dir = GetRandomValue(0, 1) * 2 - 1; // randomly set dir to +1 or -1

    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {
            if (getBuf(x, y) == SAND) {
                if (getBuf(x, y + 1) == EMPTY) {
                    setBuf(x, y, EMPTY);
                    setBuf(x, y + 1, SAND);
                } else if (getBuf(x + dir, y + 1) == EMPTY) {
                    setBuf(x, y, EMPTY);
                    setBuf(x + dir, y + 1, SAND);
                }
            }
        }
    }
}

void draw() {
    BeginDrawing();
    ClearBackground((Color){ 0, 0, 0, 255 });

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int index = getBuf(x, y);
            DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, colors[index]);
        }
    }

    EndDrawing();
}

int main(void) {
    InitWindow(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, "Falling Sand");
    SetTargetFPS(60);

    allocateBuffer();

    while (!WindowShouldClose()) {
        placeSand();

        think();
        draw();

        // Check if particles have left the screen border and free the buffer
        int minX = WIDTH, maxX = 0, minY = HEIGHT, maxY = 0;
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (getBuf(x, y) != EMPTY) {
                    if (x < minX) minX = x;
                    if (x > maxX) maxX = x;
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }

        if (minX == 0 && maxX == WIDTH - 1 && minY == 0 && maxY == HEIGHT - 1) {
            freeBuffer();
        }
    }

    freeBuffer();
    CloseWindow();

    return 0;
}
