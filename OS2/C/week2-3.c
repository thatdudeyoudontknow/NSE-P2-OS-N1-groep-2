// Schrijf een C programma die de gebruiker een array van structures laat invullen (3 elementen). De gewenste structure beschrijft een student en is als volgt gedefinieerd:
// •	De structure heeft als naam “student”
// •	De “student” structure bevat een string om de “naam” van de “student” op te slaan
// •	De “student” structure bevat een variabele om de “leeftijd” van de “student” op te slaan
// •	De “student” structure bevat een structure “opleiding”
// •	De “opleiding” structure bevat een string om de “naamOpleiding” van de “opleiding” op te slaan
// •	De “opleiding” structure bevat een variabele om het “instroomJaar” van de “student” aan deze “opleiding” in op te slaan.
// De gebruiker kan de informatie van drie studenten invoeren en deze informatie wordt uitgeprint wanneer de gebruiker klaar is met het invullen van de informatie.

// Lever het bestand in met de naam: “week2-3.c”.

#include <stdio.h>
#include <string.h>

struct opleiding
{
    char naamOpleiding[100];
    int instroomJaar;
};

struct student
{
    char naam[100];
    int leeftijd;
    struct opleiding opleiding;
};
#include <ctype.h>
#include <stdbool.h>

bool isValidName(char *name);

int main(void)
{
    struct student studenten[3];

    for (int i = 0; i < 3; i++)
    {
        printf("Student %d\n", i + 1);
        printf("Naam: ");
        // In your main function or wherever you're getting the input:
        do {
            scanf("%s", studenten[i].naam);
            if (!isValidName(studenten[i].naam)) {
                printf("Error: Onjuiste invoer. Voer alstublieft een correcte naam in.\n");
                printf("Naam: ");
            }
        } while (!isValidName(studenten[i].naam));
        printf("Leeftijd: ");
        // scanf("%d", &studenten[i].leeftijd);
        do{
            scanf("%d", &studenten[i].leeftijd);
            if (!test(studenten[i].leeftijd))
            {
                printf("Error: Ongeldige leeftijd. Voer alstublieft een leeftijd tussen 0 en 130 in.\n");
                printf("Leeftijd: ");
            }
        } while (!test(studenten[i].leeftijd));




        printf("Opleiding: ");
        scanf("%s", studenten[i].opleiding.naamOpleiding);
        printf("Instroomjaar: ");
        scanf("%d", &studenten[i].opleiding.instroomJaar);
        printf("\n");
    }

    printf("\n");
    printf("\n");

    for (int i = 0; i < 3; i++)
    {
        printf("Student %d\n", i + 1);
        printf("Naam: %s\n", studenten[i].naam);
        printf("Leeftijd: %d\n", studenten[i].leeftijd);
        printf("Opleiding: %s\n", studenten[i].opleiding.naamOpleiding);
        printf("Instroomjaar: %d\n", studenten[i].opleiding.instroomJaar);
        printf("\n");
    }

    return 0;
}


bool isValidName(char *name) {
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool test (int leeftijd){
    if (leeftijd >= 0 && leeftijd <= 130){
        return true;
    }
    else{
        return false;
    }
}
