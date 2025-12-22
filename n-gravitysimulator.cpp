#include <raylib.h>
#include <vector>
#include <string>
#include <cmath>

const int WIDTH = 1400;
const int HEIGHT = 900;

struct Body {
    Vector2 pos;
    Vector2 vel;
    float mass;
    float radius;
    Color color;
    std::string name;
    std::vector<Vector2> trail;
    bool drawTrail;

    Body(Vector2 p, Vector2 v, float m, float r, Color c, const std::string& n, bool trail = true)
        : pos(p), vel(v), mass(m), radius(r), color(c), name(n), drawTrail(trail) {}

    void update(float dt, const std::vector<Body*>& others) {
        Vector2 accel = {0.0f, 0.0f};

        for (const auto* other : others) {
            if (other == this) continue;

            Vector2 diff = {other->pos.x - pos.x, other->pos.y - pos.y};
            float distSq = diff.x * diff.x + diff.y * diff.y;
            if (distSq < 1.0f) continue;

            float force = 4000.0f * other->mass / distSq;
            float dist = sqrtf(distSq);
            accel.x += force * diff.x / dist;
            accel.y += force * diff.y / dist;
        }

        vel.x += accel.x * dt;
        vel.y += accel.y * dt;
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        if (drawTrail) {
            trail.push_back(pos);
            if (trail.size() > 600) trail.erase(trail.begin());
        }
    }

    void draw() const {
        if (drawTrail && trail.size() > 1) {
            for (size_t i = 1; i < trail.size(); ++i) {
                float alpha = (float)i / trail.size();
                Color trailCol = color;
                trailCol.a = (unsigned char)(alpha * 80);
                DrawLineV(trail[i-1], trail[i], trailCol);
            }
        }

        DrawCircleV(pos, radius, color);

        if (!name.empty()) {
            int textW = MeasureText(name.c_str(), 14);
            DrawText(name.c_str(), pos.x - textW / 2, pos.y - radius - 20, 14, WHITE);
        }
    }
};

int main() {
    InitWindow(WIDTH, HEIGHT, "N-Body Gravity Simulator - Sun + Planets");
    SetTargetFPS(60);

    std::vector<Body> bodies;

    bodies.emplace_back(Vector2{WIDTH/2.0f, HEIGHT/2.0f},
                        Vector2{0, 0},
                        30000.0f, 40, YELLOW, "Sun", false);

    bodies.emplace_back(Vector2{WIDTH/2.0f + 120, HEIGHT/2.0f},
                        Vector2{0, 90.0f},
                        8.0f, 10, GRAY, "Mercury");

    bodies.emplace_back(Vector2{WIDTH/2.0f + 200, HEIGHT/2.0f},
                        Vector2{0, 65.0f},
                        15.0f, 16, ORANGE, "Venus");

    bodies.emplace_back(Vector2{WIDTH/2.0f + 300, HEIGHT/2.0f},
                        Vector2{0, 55.0f},
                        18.0f, 16, BLUE, "Earth");

    bodies.emplace_back(Vector2{WIDTH/2.0f + 420, HEIGHT/2.0f},
                        Vector2{0, 42.0f},
                        10.0f, 12, RED, "Mars");

    std::vector<Body*> bodyPtrs;
    for (auto& b : bodies) bodyPtrs.push_back(&b);

    bool paused = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        if (IsKeyPressed(KEY_R)) {
            for (auto& b : bodies) {
                if (b.drawTrail) b.trail.clear();
            }
        }

        if (!paused) {
            for (auto& body : bodies) {
                body.update(dt, bodyPtrs);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < 200; ++i) {
            int x = GetRandomValue(0, WIDTH);
            int y = GetRandomValue(0, HEIGHT);
            DrawPixel(x, y, WHITE);
        }

        for (const auto& body : bodies) {
            body.draw();
        }

        DrawText("SPACE - Pause / Resume", 10, 10, 20, LIME);
        DrawText("R - Reset trails", 10, 40, 20, LIME);
        DrawText(paused ? "PAUSED" : "RUNNING", WIDTH - 150, 10, 24, paused ? RED : GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}