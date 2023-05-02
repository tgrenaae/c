/*Programmet er skrevet af Tommy Grenaae d. 16/10 2020, tgrena20@student.aau.dk, 
fra gruppe SW0001, Software 2020*/

#include <stdio.h>
#include <math.h>

/*prototyper af funktionerne anvendt*/
void scan_data(char *operator, double *operand);
void run_calculator(double *akkumolator, char *operator, double *operand);
void do_next_op(double *akkumolator, char *operator, double *operand);
void addition(double *akkumolator, double *operand);
void subtraction(double *akkumolator, double *operand);
void multiplication(double *akkumolator, double *operand);
void division(double *akkumolator, double *operand);
void power(double *akkumolator, double *operand);
void squareroot(double *akkumolator);
void changesign(double *akkumolator);
void invert(double *akkumolator);


int main(void){
    /*variable initialiseres*/
    double akkumolator = 0.0;
    double operand = 0.0;
    char operator = 'A';

    printf("Akkumolator starts at 0.\n");
    /*loop der kører indtil operator er q*/
    while (operator != 'q'){
        scan_data(&operator, &operand);
        run_calculator(&akkumolator, &operator, &operand);
    }
    printf("Program terminated. Goodbye.\n");
    return 0;
}

/*funktionen indlæser data fra bruger, operand promptes kun hvis operator er binær infix*/
void scan_data(char *operator, double *operand){
    printf("Enter operator: \n");
    scanf("%c", operator);
    if(*operator == '+' || *operator == '-' || *operator == '*' || *operator == '/' || *operator == '^'){
        printf("Enter operand: \n");
        scanf("%lf", operand);
    }
    return;
}

/*funktionen står for kørsel af beregning, samt resultat*/
void run_calculator(double *akkumolator, char *operator, double *operand){
    /*laver næste operation, såfremt bruger ikke ønsker at stoppe*/
    if(*operator != 'q'){
        do_next_op(akkumolator, operator, operand);
    }
    printf("Akkumolator: %f\n", *akkumolator);
    return;
}

/*funktionen anvender switch til at bestemme hvilken case vi skal udføre*/
void do_next_op(double *akkumolator, char *operator, double *operand){
    switch (*operator)
    {
    case '+':
        addition(akkumolator, operand);
        break;
    case '-':
        subtraction(akkumolator, operand);
        break;
    case '*':
        multiplication(akkumolator, operand);
        break;
    case '/':
        division(akkumolator, operand);
        break;
    case '^':
        power(akkumolator, operand);
        break;
    case '#':
        squareroot(akkumolator);
        break;
    case '%':
        changesign(akkumolator);
        break;
    case '!':
        invert(akkumolator);
        break;
    default:
        break;
    }
    return;
}

/*følgende funktioners brugbarhed kan diskuteres, men er programmeret
for at vise konceptet i Top-down programmering med trinvis forfinelse*/
void addition(double *akkumolator, double *operand){
    *akkumolator = *akkumolator + *operand;
    return;
}

void subtraction(double *akkumolator, double *operand){
    *akkumolator = *akkumolator - *operand;
    return;
}

void multiplication(double *akkumolator, double *operand){
    *akkumolator = *akkumolator * *operand;
    return;
}

void division(double *akkumolator, double *operand){
    /*sikrer at divisor ikke er 0*/
    if(*operand != 0){
        *akkumolator = *akkumolator / *operand;
    }
    return;
}

void power(double *akkumolator, double *operand){
        *akkumolator = pow(*akkumolator, *operand);
    return;
}

void squareroot(double *akkumolator){
    /*sikrer at kvadratroden kan udføres (uden tanke på imaginære tal)*/
    if(*akkumolator >= 0){
        *akkumolator = sqrt(*akkumolator);
    }
    return;
}

void changesign(double *akkumolator){
    *akkumolator = *akkumolator * (-1);
    return;
}

void invert(double *akkumolator){
    /*sikrer at akkumolator ikke er nul*/
    if(*akkumolator != 0){
        *akkumolator = 1 / *akkumolator;
    }
    return;
}