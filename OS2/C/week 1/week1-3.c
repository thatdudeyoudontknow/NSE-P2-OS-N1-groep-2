
#include "week1-2.h"
// Write a program that does the following:


// take a user input and save it in a variable
// the user can chose between 3 options option 1, option 2, option 3
//if a option is chosen, print the chosen option
// if annything else is chosen, print "invalid option"
//if option 1 is chosen, print "option 1"
//if option 1 is chosen, run the main2 from week1-2.h and print the result
//if option 2 is chosen, print "option 2"
//if option 2 is chosen, run the main2 from week1-2.h and print the result times 2
//if option 3 is chosen, print "option 3"
//if option 3 is chosen, run the main2 from week1-2.h and print the result times 3




int main( void )
{
    int option;
    printf("Please enter a number between 1 and 3: ");
    scanf("%d", &option);
    if (option == 1)
    {
        printf("option 1\n");
        main2();
    }
    else if (option == 2)
    {
        printf("option 2\n");
        main2();
        main2();
    }
    else if (option == 3)
    {
        printf("option 3\n");
        main2();
        main2();
        main2();
    }
    else
    {
        printf("invalid option\n");
    }
    return 0;
}














