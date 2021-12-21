#include <math.h>

float E(int x) {
    return pow(1 + 1. / x, x);
}

float Derivative(float A, float deltaX) {
    float ans;
    ans = (cos(A + deltaX) - cos(A)) / deltaX;
    return ans;
}