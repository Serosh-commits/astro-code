#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cstdio>
using namespace std;
struct Star {
    double x, y, z;
    double vx, vy, vz;
    double ax, ay, az;
    double m;
};
const double G = 1.0;
const double Softening = 0.1;
void accel(vector<Star>& stars) {
    for (auto& s : stars) {
        s.ax = s.ay = s.az = 0;
    }
    for (size_t i = 0; i < stars.size(); i++) {
        for (size_t j = i + 1; j < stars.size(); j++) {
            double dx = stars[j].x - stars[i].x;
            double dy = stars[j].y - stars[i].y;
            double dz = stars[j].z - stars[i].z;
            double r2 = dx*dx + dy*dy + dz*dz + Softening*Softening;
            double invR3 = G / (r2 * sqrt(r2)); 
            double fx = dx * invR3;
            double fy = dy * invR3;
            double fz = dz * invR3;
            stars[i].ax += stars[j].m * fx;
            stars[i].ay += stars[j].m * fy;
            stars[i].az += stars[j].m * fz;

            stars[j].ax -= stars[i].m * fx;
            stars[j].ay -= stars[i].m * fy;
            stars[j].az -= stars[i].m * fz;
        }
    }
}

void step(vector<Star>& stars, double dt) {
    for (auto& s : stars) {
        s.vx += s.ax * 0.5 * dt;
        s.vy += s.ay * 0.5 * dt;
        s.vz += s.az * 0.5 * dt;
        s.x += s.vx * dt;
        s.y += s.vy * dt;
        s.z += s.vz * dt;
    }
    
    accel(stars);

    for (auto& s : stars) {
        s.vx += s.ax * 0.5 * dt;
        s.vy += s.ay * 0.5 * dt;
        s.vz += s.az * 0.5 * dt;
    }
}

int main() {
    int N = 100;
    double dt = 0.01;
    vector<Star> stars(N);
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-10, 10);
    uniform_real_distribution<double> vdist(-0.1, 0.1);

    for (int i = 0; i < N; i++) {
        stars[i] = {dist(gen), dist(gen), dist(gen), vdist(gen), vdist(gen), vdist(gen), 0, 0, 0, 1.0};
    }

    accel(stars);

    printf("N-Body Simulation (N=%d)\n", N);
    for (int i = 0; i < 100; i++) {
        step(stars, dt);
        if (i % 20 == 0) {
            double tx = 0, ty = 0, tz = 0;
            for (auto& s : stars) { tx += s.x; ty += s.y; tz += s.z; }
            printf("Step %d | Center of Mass: (%.4f, %.4f, %.4f)\n", i, tx/N, ty/N, tz/N);
        }
    }

    return 0;
}
