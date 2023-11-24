#include <stdio.h>

void calculate(float diameter, float *omtrek ) {

    const float pi = 3.14159;
    *omtrek = pi * diameter;

}

int main(){

    float diameter;

    printf("Voer hier de diameter van de cirkel in: ");
    scanf("%f", &diameter);

    float omtrek;

    calculate(diameter, &omtrek);

    printf("De omtrek van de cirkel= %.2f\n",omtrek);

    return 0;
}