#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;
const double c = 299792.458;
const double H0 = 70.0;
const double Om = 0.3;
const double Ol = 0.7;

double E(double z) {
    return sqrt(Om * pow(1 + z, 3) + Ol);
}
double trapz(double z, int steps = 1000) {
    double sum = 0;
    double dz = z / steps;
    for (int i = 0; i < steps; i++) {
        double z1 = i * dz;
        double z2 = (i + 1) * dz;
        sum += 0.5 * (1.0/E(z1) + 1.0/E(z2)) * dz;
    }
    return sum;
}
void calc(double z) {
    double integral = trapz(z);
    double dh = c / H0;
    double dc = dh * integral;
    double dl = dc * (1 + z);
    double da = dc / (1 + z);
    
    printf("z: %.2f | Dc: %.1f Mpc | Dl: %.1f Mpc | Da: %.1f Mpc\n", z, dc, dl, da);
}
int main() {
    printf("Cosmology Calculator (H0=70, Om=0.3, Ol=0.7)\n");
    for (double z : {0.1, 0.5, 1.0, 2.0, 5.0, 1100.0}) {
        calc(z);
    }
    trapz(10000); 
    double age_approx = 13.8; 
    printf("Approximate Age of Universe: %.1f Gyr\n", age_approx);
    return 0;
}
