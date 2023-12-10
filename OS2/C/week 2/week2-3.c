#include <stdio.h>

// Definitie van de "opleiding" structure
struct Opleiding {
    char naamOpleiding[50];
    int instroomJaar;
};

// Definitie van de "student" structure met een "opleiding" structure als onderdeel
struct Student {
    char naam[50];
    int leeftijd;
    struct Opleiding opleiding;
};

int main() {
    // Array van structures om informatie over drie studenten op te slaan
    struct Student studenten[3];

    // Invoer van informatie voor elke student
    for (int i = 0; i < 3; ++i) 
    {
        printf("Voer de naam van student %d in: ", i + 1);
        scanf("%s", studenten[i].naam);

        printf("Voer de leeftijd van student %d in: ", i + 1);
        scanf("%d", &studenten[i].leeftijd);

        printf("Voer de naam van de opleiding van student %d in: ", i + 1);
        scanf("%s", studenten[i].opleiding.naamOpleiding);

        printf("Voer het instroomjaar van student %d in: ", i + 1);
        scanf("%d", &studenten[i].opleiding.instroomJaar);
        printf("\n");
    }
    printf("\n");
    // Afdrukken van de ingevoerde informatie
    printf("\nIngevoerde informatie:\n");

    for (int i = 0; i < 3; ++i) {
        printf("\nStudent %d:\n", i + 1);
        printf("Naam: %s\n", studenten[i].naam);
        printf("Leeftijd: %d\n", studenten[i].leeftijd);
        printf("Opleiding: %s\n", studenten[i].opleiding.naamOpleiding);
        printf("Instroomjaar: %d\n", studenten[i].opleiding.instroomJaar);
    }

    return 0;
}
