#include <stdio.h>

int glob_balance= 1000;
int pin, keuze;

void Check_balance();
void Deposit_money();
void Withdraw_money();
void Exit();

void main(){
    while (pin =! 2020){
        printf("geef uw pincode");
        scanf ('%d', %pin);
        if (pin =! 2020)
            printf("geef uw correcte code alstublieft");
    }
    do{
        printf("welkom bij uw pinapparaat");
        printf("optie 1. check uw balans\n");
        printf("optie 2. geld pinnen\n");
        printf("optie 3. geld storten\n");
        printf("optie 4. exit\n");
        printf("\n");
        printf("maak alstublieft uw keuze");
        scanf("%d", &keuze);

    } while (/* condition */);
    
        