#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definieer de Persoon structuur
struct Persoon {
    char naam[50];
    int leeftijd;
};

int main() {
    // Initialisatie van variabelen
    struct Persoon *personen = NULL;
    int aantalPersonen = 0;
    char invoer[50];

    // Voer het programma uit totdat de gebruiker aangeeft te stoppen
    while (1) {
        // Dynamisch geheugen reserveren voor een nieuwe persoon
        struct Persoon *temp = realloc(personen, (aantalPersonen + 1) * sizeof(struct Persoon));

        if (temp == NULL) {
            printf("Geheugen kon niet worden toegewezen. Programma wordt afgesloten.\n");
            free(personen);
            return 1;
        }

        personen = temp;

        // Invoer van de naam van de persoon
        printf("Voer de naam van de persoon in (stop om te eindigen): ");
        fgets(invoer, sizeof(invoer), stdin);
        if (strcmp(invoer, "stop\n") == 0) {
            // Stop het programma als "stop" wordt ingevoerd
            break;
        }
        strcpy(personen[aantalPersonen].naam, invoer);

        // Invoer van de leeftijd van de persoon
        printf("Voer de leeftijd van de persoon in: ");
        scanf("%d", &personen[aantalPersonen].leeftijd);
        fflush(stdin); // Leeg de stdin-buffer

        // Verhoog het aantal personen
        aantalPersonen++;

        // Druk alle ingevoerde personen af
        printf("\nAlle ingevoerde personen:\n");
        for (int i = 0; i < aantalPersonen; i++) {
            printf("Persoon %d:\n", i + 1);
            printf("Naam: %sLeeftijd: %d\n\n", personen[i].naam, personen[i].leeftijd);
        }
    }

    // Druk alle ingevoerde personen af voordat het programma wordt afgesloten
    printf("\nAlle ingevoerde personen voordat het programma wordt afgesloten:\n");
    for (int i = 0; i < aantalPersonen; i++) {
        printf("Persoon %d:\n", i + 1);
        printf("Naam: %sLeeftijd: %d\n\n", personen[i].naam, personen[i].leeftijd);
    }

    // Geheugen vrijgeven bij het beëindigen van het programma
    free(personen);

    return 0;
}
