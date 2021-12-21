#include <math.h>

float E(int x) {
    float answer = 0;
    long long s = 1;
    for (int i = 0; i <= x; ++i) {
        if (i!=0) {
            s*=i;
        }
        answer += 1. / s;
    }
    return answer;
}

float Derivative(float A, float deltaX) {
    float ans;
    ans = (cos(A + deltaX) - cos(A - deltaX)) / (2 * deltaX);
    return ans;
}
