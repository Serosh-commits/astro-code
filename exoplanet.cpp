#include <raylib.h>
#include <vector>
#include <string>
#include <cmath>

const int SCREEN_W = 1400;
const int SCREEN_H = 900;

float depth(float pr, float sr) {
    return (pr*pr)/(sr*sr);
}

std::vector<float> make_curve(float per, float dur, float dep, int pts=1200) {
    std::vector<float> lc(pts);
    float hd = dur/2;

    for(int i=0;i<pts;i++) {
        float t = (float)i/pts*per - per/2;
        if(fabsf(t)<hd) lc[i]=1-dep;
        else lc[i]=1;
    }
    return lc;
}

int main() {
    InitWindow(SCREEN_W,SCREEN_H,"Exoplanet Transit Tool");
    SetTargetFPS(60);

    float star_r = 60;
    float planet_r = 12;
    float a = 180;
    float period = 8;
    float dur_h = 4;
    float inc = 89.5;

    float ang = 0;
    bool view = true;
    bool pause = false;

    Vector2 cen = {SCREEN_W/2.0f, SCREEN_H/2.0f + 100};

    auto curve = make_curve(period, dur_h/24.0f, depth(planet_r,star_r));

    while(!WindowShouldClose()) {
        float dt = GetFrameTime();
        if(!pause) ang += dt/period * 2*PI;

        if(IsKeyDown(KEY_UP)) planet_r += 15*dt;
        if(IsKeyDown(KEY_DOWN)) planet_r = fmaxf(5,planet_r-15*dt);
        if(IsKeyDown(KEY_RIGHT)) a += 80*dt;
        if(IsKeyDown(KEY_LEFT)) a = fmaxf(100,a-80*dt);
        if(IsKeyPressed(KEY_SPACE)) view=!view;
        if(IsKeyPressed(KEY_P)) pause=!pause;

        float d = depth(planet_r,star_r);
        curve = make_curve(period, dur_h/24.0f, d);

        Vector2 ppos = {cen.x + a*cosf(ang),
                        cen.y + a*sinf(inc*DEG2RAD)*sinf(ang)};

        BeginDrawing();
        ClearBackground(BLACK);

        if(view) {
            DrawCircleV(cen,star_r,YELLOW);
            DrawCircleLines(cen.x,cen.y,a,Fade(WHITE,0.3f));
            DrawCircleV(ppos,planet_r,BLUE);
        }

        int gx=100, gy=SCREEN_H-320, gw=SCREEN_W-200, gh=220;

        DrawRectangle(gx-10,gy-10,gw+20,gh+60,Fade(BLACK,0.7f));
        DrawRectangleLines(gx,gy,gw,gh,WHITE);

        DrawText("Flux vs Phase",gx+gw/2-100,gy-40,24,WHITE);
        DrawText("Phase",gx+gw/2-50,gy+gh+20,20,LIGHTGRAY);
        DrawText("Flux ↑",gx-80,gy+20,20,LIGHTGRAY);

        float phase = fmodf(ang/(2*PI)+0.5f,1);
        float mx = gx + phase*gw;

        for(size_t i=1;i<curve.size();i++) {
            float x1 = gx + (i-1)/(float)curve.size()*gw;
            float x2 = gx + i/(float)curve.size()*gw;
            float y1 = gy + gh*(1 - (curve[i-1]-(1-d))/d);
            float y2 = gy + gh*(1 - (curve[i]-(1-d))/d);
            DrawLineEx({x1,y1},{x2,y2},3.5f,SKYBLUE);
        }

        DrawLineEx({mx,(float)gy},{mx,(float)(gy+gh)},3,RED);

        DrawText("Controls",20,20,22,LIME);
        DrawText("UP/DOWN  planet size",40,60,20,WHITE);
        DrawText("L/R      orbit size",40,90,20,WHITE);
        DrawText("SPACE    toggle view",40,120,20,WHITE);
        DrawText("P        pause",40,150,20,WHITE);

        DrawText(TextFormat("Depth %.5f (%.1f ppm)",d,d*1e6),40,200,22,YELLOW);
        DrawText(TextFormat("Planet R %.1f Earth",planet_r/12),40,240,20,WHITE);
        DrawText(TextFormat("Period %.1f d",period),40,270,20,WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
