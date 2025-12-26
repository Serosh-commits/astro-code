#include <iostream>
#include <cmath>
#include <vector>
#include <cstdio>
using namespace std;
struct State {
    double theta, phi;
};

State deriv(double xi, State s, double n) {
    if (xi == 0) return {0, 0};
    return {s.phi, -pow(s.theta, n) - (2.0 / xi) * s.phi};
}
void solve(double n, double h = 0.01) {
    double xi = 1e-10;
    State s = {1.0, 0.0};
    printf("\nLane-Emden (n = %.1f)\n", n);
    printf("%-10s %-10s %-10s\n", "xi", "theta", "phi");
    while (s.theta > 0 && xi < 20.0) {
        if (fmod(xi, 0.5) < h) 
        {
            printf("%-10.4f %-10.4f %-10.4f\n", xi, s.theta, s.phi);
        }
        State k1 = deriv(xi, s, n);
        State k2 = deriv(xi + h/2, {s.theta + h/2 * k1.theta, s.phi + h/2 * k1.phi}, n);
        State k3 = deriv(xi + h/2, {s.theta + h/2 * k2.theta, s.phi + h/2 * k2.phi}, n);
        State k4 = deriv(xi + h, {s.theta + h * k3.theta, s.phi + h * k3.phi}, n);

        s.theta += (h/6.0) * (k1.theta + 2*k2.theta + 2*k3.theta + k4.theta);
        s.phi += (h/6.0) * (k1.phi + 2*k2.phi + 2*k3.phi + k4.phi);
        xi += h;
    }
    printf("Surface at xi_1 = %.4f\n", xi);
}
int main() {
    solve(0.0);
    solve(1.0);
    solve(1.5);
    solve(3.0);
    return 0;
}
