#include <stdio.h>



int main() {
    FILE *inputFile, *outputFile;
    char ch;

    // Open het originele bestand in leesmodus
    inputFile = fopen("input.txt", "r");

    // Controleer of het bestand succesvol is geopend
    if (inputFile == NULL) {
        printf("Kan het bestand niet openen.\n");
        return 1;
    }

    // Open een nieuw bestand in schrijfmodus
    outputFile = fopen("output.txt", "w");

    // Controleer of het bestand succesvol is geopend
    if (outputFile == NULL) {
        printf("Kan het uitvoerbestand niet maken.\n");
        fclose(inputFile);
        return 1;
    }

    int charCount = 0;

    // Lees elk karakter van het originele bestand
    while ((ch = fgetc(inputFile)) != EOF) {
        // Tel het aantal karakters (niet LF en EOF)
        if (ch != '\n' && ch != EOF) {
            charCount++;
        }

        // Verhoog de ASCII-waarde van het karakter
        if (ch != '\n' && ch != EOF) {
            ch++;
        }

        // Schrijf het verhoogde karakter naar het nieuwe bestand
        fputc(ch, outputFile);
    }

    // Sluit beide bestanden
    fclose(inputFile);
    fclose(outputFile);

    // Druk het aantal karakters af
    printf("Aantal karakters: %d\n", charCount);

    return 0;
}








