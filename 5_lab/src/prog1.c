#include<stdio.h>

float E(int x);
float Derivative(float A, float deltaX);

int main(){

    int key;
    while(scanf("%d", &key) > 0){

        if (key == 1) {

            float A, deltaX;
            scanf("%f%f", &A, &deltaX);
            printf("%s(%f, %f) = %f\n", "Derivative", A, deltaX, Derivative(A, deltaX));

        }
        else if (key == 2) {

            int x;
            scanf("%d", &x);
            printf("%s(%d) = %f\n", "E", x, E(x));

        } else {

            printf("ERROR\n");

        }
    }
    
}