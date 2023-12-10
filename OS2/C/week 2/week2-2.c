#include <stdio.h>

int main(void)
{
    int i;
    char sentence[100];
    printf("Enter a sentence: ");
    scanf("%[^\n]s", sentence);
    for (i=0; sentence[i] != '\0'; i++);
    printf("Length of sentence: %d\n", i);
    return 0;
}