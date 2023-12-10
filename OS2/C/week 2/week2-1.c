#include <stdio.h>

void maxArrays(float *array1, float *array2, float *array3, int length);

int main(void)
{
    float array1[] = {0.7, 3.3, 0.5, 10.3};
    float array2[] = {4.1, 1.5, 0.5, 2.3};
    float array3[4];

    printf("Array 1: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%.1f ", array1[i]);
    }
    printf("\n");

    printf("Array 2: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%.1f ", array2[i]);
    }
    printf("\n");

    maxArrays(array1, array2, array3, 4);

    printf("Array 3: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%.1f ", array3[i]);
    }
    printf("\n");

    return 0;
}

void maxArrays(float *array1, float *array2, float *array3, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (array1[i] > array2[i])
        {
            array3[i] = array1[i];
        }
        else
        {
            array3[i] = array2[i];
        }
    }
}



