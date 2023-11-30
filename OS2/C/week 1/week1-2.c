#include <stdio.h>

void print_function( char letter );

int main( void )
{
    int ctr;
    char star = '*';
    char dash = '-';
    for (int ctr = 0 ; ctr < 10; ctr++ )
    {
        print_function( star );
        print_function( dash );
        printf("\n");
    }
    return 0;
}

void print_function ( char letter )
{
    int ctr;
    for ( ctr = 0; ctr < 10; ctr++) 
    {
        printf("%c", letter);
    }
}