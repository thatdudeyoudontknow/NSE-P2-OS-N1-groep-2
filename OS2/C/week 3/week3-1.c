#include <stdio.h>
#include <string.h>

void shiftText(FILE *input, FILE *output) {
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if (ch != '\n') {
            // Shift ASCII value unless it's an LF character
            fputc(ch + 1, output);
        } else {
            // Preserve line formatting
            fputc('\n', output);
        }
    }
}

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[100], outputFileName[100], modifiedFileName[100];

    // Ask the user for the file name
    printf("Enter the file name (.txt): ");
    scanf("%s", inputFileName);

    // Open the input file for reading
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    // Generate the output file name by adding '-changed' before the extension
    char *dot = strrchr(inputFileName, '.');
    if (dot != NULL) {
        *dot = '\0';  // Remove the extension temporarily
    }
    sprintf(modifiedFileName, "%s-changed", inputFileName);

    // Concatenate the original extension to the modified file name
    if (dot != NULL) {
        strcat(modifiedFileName, dot);
    }

    // Open the output file for writing
    outputFile = fopen(modifiedFileName, "w");
    if (outputFile == NULL) {
        printf("Could not create the output file.\n");
        fclose(inputFile);
        return 1;
    }

    // Shift text and preserve line formatting
    shiftText(inputFile, outputFile);

    // Close the files
    fclose(inputFile);
    fclose(outputFile);

    printf("Operation completed. The output file is %s.\n", modifiedFileName);

    return 0;
}
