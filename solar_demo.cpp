#include <raylib.h>
#include <vector>
#include <string>
#include <cmath>

const int WIDTH = 1200;
const int HEIGHT = 800;

struct Body {
    Vector2 pos;
    float size;
    Color col;
    std::string label;

    Body(Vector2 p, float s, Color c, const std::string& l)
        : pos(p), size(s), col(c), label(l) {}

    virtual void update(float) {}
    virtual void draw() {
        DrawCircleV(pos, size, col);
        int textWidth = MeasureText(label.c_str(), 12);
        DrawText(label.c_str(), pos.x - textWidth / 2, pos.y + size + 8, 12, WHITE);
    }
};

struct Star : public Body {
    float temp;

    Star(Vector2 p, float s, float t)
        : Body(p, s, WHITE, "Sun"), temp(t) {
        updateColor();
    }

    void updateColor() {
        float peak = 2898000.0f / temp;          // Wien's law in nm (approx constant)

        if (peak < 400)      col = BLUE;
        else if (peak < 500) col = WHITE;
        else if (peak < 600) col = YELLOW;
        else if (peak < 700) col = ORANGE;
        else                 col = RED;
    }
};

struct Planet : public Body {
    float orbitDistance;
    float speed;
    float angle = 0.0f;

    Planet(Vector2 center, float dist, float spd, float sz, Color c, const std::string& l)
        : Body({center.x + dist, center.y}, sz, c, l),
          orbitDistance(dist), speed(spd) {}

    void update(float dt) override {
        angle += speed * dt;
        pos.x = WIDTH / 2 + orbitDistance * cosf(angle);
        pos.y = HEIGHT / 2 + orbitDistance * sinf(angle);
    }

    void draw() override {
        DrawCircleLines(WIDTH / 2, HEIGHT / 2, orbitDistance, Fade(GRAY, 0.3f));
        Body::draw();
    }
};

int main() {
    InitWindow(WIDTH, HEIGHT, "Solar System - Blackbody Demo");
    SetTargetFPS(60);

    Vector2 center = { float(WIDTH / 2), float(HEIGHT / 2) };

    Star sun(center, 50, 5772);

    std::vector<Planet> planets;
    planets.emplace_back(center, 100, 0.04f, 10, GRAY,   "Mercury");
    planets.emplace_back(center, 180, 0.025f, 15, YELLOW,"Venus");
    planets.emplace_back(center, 260, 0.018f, 15, BLUE,   "Earth");
    planets.emplace_back(center, 360, 0.012f, 12, RED,    "Mars");

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        for (auto& p : planets) {
            p.update(delta);
        }

        if (IsKeyDown(KEY_UP))   sun.temp += 100;
        if (IsKeyDown(KEY_DOWN)) sun.temp -= 100;
        sun.updateColor();

        BeginDrawing();
        ClearBackground(BLACK);

        // simple starfield
        for (int i = 0; i < 150; ++i) {
            int x = GetRandomValue(0, WIDTH);
            int y = GetRandomValue(0, HEIGHT);
            DrawPixel(x, y, WHITE);
        }

        sun.draw();
        for (auto& p : planets) {
            p.draw();
        }

        DrawText("UP / DOWN arrows → change star temperature", 10, 10, 20, LIME);
        DrawText(TextFormat("Temperature: %.0f K", sun.temp), 10, 40, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}