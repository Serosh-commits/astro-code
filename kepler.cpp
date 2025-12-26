#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>

using namespace std;

const double G = 6.67430e-11;
const double MSun = 1.989e30;
const double AU = 1.496e11;

struct Body {
    double a, e, i, lan, argp, m0;
};

double solve(double m, double e) {
    double E = m;
    for (int i = 0; i < 10; i++) {
        E = E - (E - e * sin(E) - m) / (1 - e * cos(E));
    }
    return E;
}

void pos(Body b, double days) {
    double mu = G * MSun;
    double n = sqrt(mu / pow(b.a * AU, 3));
    double M = fmod(b.m0 * M_PI / 180.0 + n * (days * 86400), 2 * M_PI);
    
    double E = solve(M, b.e);
    double v = 2 * atan2(sqrt(1 + b.e) * sin(E/2), sqrt(1 - b.e) * cos(E/2));
    double r = b.a * AU * (1 - b.e * cos(E));
    
    double x = r * cos(v) / AU;
    double y = r * sin(v) / AU;
    
    printf("t: %.1f | r: %.4f | pos: (%.4f, %.4f)\n", days, r/AU, x, y);
}

int main() {
    Body earth = {1.00000011, 0.01671022, 0.0, 0.0, 0.0, 100.46435};
    cout << "Earth Orbit Data:" << endl;
    for (double t = 0; t <= 270; t += 90) pos(earth, t);
    return 0;
}
