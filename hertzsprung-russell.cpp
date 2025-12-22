#include <raylib.h>
#include <vector>
#include <string>
#include <cmath>

const int WIDTH = 1400;
const int HEIGHT = 900;

struct StarData {
    float temperature;
    float luminosity;
    Color color;
    std::string spectralType;
};

std::vector<StarData> stars = {
    {3000,   0.0001f, RED,     "M"},
    {4500,   0.01f,   ORANGE,  "K"},
    {6000,   1.0f,    YELLOW,  "G"},
    {7500,   5.0f,    WHITE,   "F"},
    {10000,  30.0f,   SKYBLUE, "A"},
    {20000,  1000.0f, BLUE,    "B"},
    {40000,  50000.0f,VIOLET,  "O"}
};

Color tempToColor(float temp) {
    float wl = 2.897e6f / temp;
    if (wl < 400) return VIOLET;
    if (wl < 450) return BLUE;
    if (wl < 500) return SKYBLUE;
    if (wl < 580) return WHITE;
    if (wl < 600) return YELLOW;
    return ORANGE;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Hertzsprung-Russell Diagram");
    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    bool showLabels = true;
    bool logScale = true;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) camera.target.x -= 400 * GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) camera.target.x += 400 * GetFrameTime();
        if (IsKeyDown(KEY_UP)) camera.target.y += 400 * GetFrameTime();
        if (IsKeyDown(KEY_DOWN)) camera.target.y -= 400 * GetFrameTime();
        if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) camera.zoom += GetMouseWheelMove() * 0.1f;
        camera.zoom = fmaxf(camera.zoom, 0.5f);
        if (IsKeyPressed(KEY_L)) showLabels = !showLabels;
        if (IsKeyPressed(KEY_S)) logScale = !logScale;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        int originX = 100;
        int originY = HEIGHT - 100;
        int diagW = WIDTH - 200;
        int diagH = HEIGHT - 200;

        DrawLine(originX, originY, originX + diagW, originY, WHITE);
        DrawLine(originX, originY, originX, originY - diagH, WHITE);

        for (int i = 0; i <= 10; ++i) {
            float temp = 40000 * powf(10, -i / 10.0f);
            float x = originX + (log10f(40000) - log10f(temp)) / (log10f(40000) - log10f(3000)) * diagW;
            DrawLine(x, originY, x, originY + 10, WHITE);
            DrawText(TextFormat("%.0f", temp), x - 20, originY + 15, 14, WHITE);
        }

        for (int i = -2; i <= 6; ++i) {
            float lum = powf(10, i);
            float y = originY - (log10f(lum) - log10f(0.0001f)) / (log10f(50000) - log10f(0.0001f)) * diagH;
            DrawLine(originX - 10, y, originX, y, WHITE);
            DrawText(TextFormat("%g", lum), originX - 60, y - 10, 14, WHITE);
        }

        DrawText("Temperature (K) → decreasing", originX + diagW / 2 - 100, originY + 30, 20, LIGHTGRAY);
        DrawText("Luminosity (L_sun) ↑ increasing", originX - 100, originY - diagH / 2 - 20, 20, Rotatef(LIGHTGRAY, -90));

        DrawText("Main Sequence", originX + diagW / 3, originY - diagH / 4, 24, Fade(YELLOW, 0.7f));
        DrawText("Giants", originX + diagW / 2, originY - diagH * 0.8f, 24, Fade(RED, 0.7f));
        DrawText("White Dwarfs", originX + diagW / 5, originY - diagH * 0.1f, 24, Fade(WHITE, 0.7f));

        for (const auto& s : stars) {
            float logTemp = log10f(s.temperature);
            float x = originX + (log10f(40000) - logTemp) / (log10f(40000) - log10f(3000)) * diagW;

            float logLum = log10f(s.luminosity);
            float y = originY - (logLum - log10f(0.0001f)) / (log10f(50000) - log10f(0.0001f)) * diagH;

            float size = 10 + 30 * powf(s.luminosity / 50000.0f, 0.3f);
            size = fminf(size, 60);

            Color col = tempToColor(s.temperature);
            DrawCircle(x, y, size, col);

            if (showLabels) {
                DrawText(s.spectralType.c_str(), x - 10, y - size - 25, 20, WHITE);
            }
        }

        EndMode2D();

        DrawText("Arrows / Middle mouse wheel - Pan & Zoom", 10, 10, 20, LIME);
        DrawText("L - Toggle spectral labels", 10, 40, 20, LIME);
        DrawText("S - Toggle log scale hint (visual only)", 10, 70, 20, LIME);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}