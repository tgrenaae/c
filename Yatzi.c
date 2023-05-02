/*Programmet er skrevet af Tommy Grenaae d. 16/11 2020, tgrena20@student.aau.dk, 
fra gruppe SW0001, Software 2020*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Globale konstanter til at ændre spillets natur
#define MAX_EYES 6
#define AVG_EYES 63
#define MAX_NUM_PAIRS 2
#define MAX_NUM_KINDS 4

//Enum navne til at iterere igennem til scoreboard
typedef enum yatzi_names {Ones, Twoes, Threes, Fours, Fives, Sixes, Bonus, OnePair, TwoPair, ThreeOfaKind, 
                FourOfaKind, SmallStraight, LargeStraight, FullHouse, Chance, Yatzi}
            names;

//prototyper af funktioner
void input_dice(int *number_of_dice);
int *upper_section(int number_of_dice);
int *roll_multiple_dice(int number_of_dice);
int roll_die(void);
int is_bonus(int *score_arr);
int check_same_dice(int *dice_arr, int number_of_dice, int check_number);
int *lower_section(int number_of_dice);
int check_pair(int *dice_arr, int number_of_dice, int pairs);
int of_a_kind(int *dice_arr, int number_of_dice, int of_a_kind);
int straight(int *dice_arr, int number_of_dice, int min_num);
int full_house(int *dice_arr, int number_of_dice);
int chance(int *dice_arr, int number_of_dice);
int yatzi(int *dice_arr, int number_of_dice);
void print_scoreboard(int *upper_score, int *lower_score, int bonus);
int calc_total_sum(int *scoreboard_arr);
char* write_names(names names);

int main(void){
    int sentinel = 1;
    int number_of_dice, bonus;
    int *upper_score;
    int *lower_score;

    //sentinel-loop så længe bruger indtaster 5 eller mere.
    while(sentinel){
        input_dice(&number_of_dice);
        if(number_of_dice >= 5){
            srand(time(NULL));
            //håndterer den øvre del af yatzi
            upper_score =  upper_section(number_of_dice);
            //vurderer hvorvidt bonus er opnået
            bonus = is_bonus(upper_score);
            //håndterer den nedre del af yatzi
            lower_score = lower_section(number_of_dice);
            //printer scoreboard
            print_scoreboard(upper_score, lower_score, bonus);
        }
        else{
            sentinel = 0;
        }
    }
    return 0;
}

//inputfunktion der tager imod n terninger fra bruger
void input_dice(int *number_of_dice){
    printf("This program lets computer play yatzi.\n");
    printf("Enter number of dice: ");
    scanf("%d", number_of_dice);
    return;
}

//håndterer øvre del af spillet, returnerer et array med resultater til main
int *upper_section(int number_of_dice){
    int *upper_arr = (int*) malloc(number_of_dice * sizeof(int));
    int *upper_scoreboard = (int*) calloc(MAX_EYES, sizeof(int));
    if(upper_scoreboard == NULL || upper_arr == NULL){
        printf("Array error. bb");
        exit(EXIT_FAILURE);
    }
    int count = 0;

    //ruller for ens terninger
    for(int i = 1; i <= MAX_EYES; i++){
        upper_arr = roll_multiple_dice(number_of_dice);
        upper_scoreboard[count] = check_same_dice(upper_arr, number_of_dice, i);
        count++;
        free(upper_arr);
    }
    return upper_scoreboard;
}

int *roll_multiple_dice(int number_of_dice){
    static int times = 1;
    int *die_arr = (int*) malloc(number_of_dice * sizeof(int));
    if(die_arr == NULL){
        printf("Array error. bb");
        exit(EXIT_FAILURE);
    }
    for(int j = 0; j < number_of_dice; j++){
        die_arr[j] = roll_die();
    }
    
    printf("Roll#%2d: ", times);
    for(int s = 0; s < number_of_dice; s++){
        printf("%d  ", die_arr[s]);
    }
    printf("\n");
    times++;    
    return die_arr;
}

int roll_die(void){
  return (rand() % MAX_EYES) + 1;
}

int is_bonus(int *score_arr){
    int sum = 0;
    for(int m = 0; m < MAX_EYES; m++){
        sum = sum + score_arr[m];
    }
    if(sum >= AVG_EYES){
        return 50;
    }
    else{
        return 0;
    }
}

//Kaldes i øvre del af spillet til at findes ens terninger
int check_same_dice(int *dice_arr, int number_of_dice, int check_number){
    int chosen_dice = 0;
    int result;
    for(int k = 0; k < number_of_dice; k++){
        if(chosen_dice < 5){
            if(dice_arr[k] == check_number){
            chosen_dice++;
            }
        }
    }
    result = chosen_dice * check_number;
    //returnerer resultat hvis man fik 3 eller mere af samme slags, ellers 0
    if(result >= 3 * check_number){
        return result;
    }
    else{
        return 0;
    }
}

//Håndterer den nedre del af spillet, returnerer et array med resultater til main
int *lower_section(int number_of_dice){
    int *lower_arr = (int*) malloc(number_of_dice * sizeof(int));
    int *lower_scoreboard = (int*) calloc(10, sizeof(int));
        if(lower_scoreboard == NULL || lower_arr == NULL){
            printf("Array error. bb");
            exit(EXIT_FAILURE);
        }
    int count = 0;
    //ruller for 1 og 2 par
    for(int p = 1; p <= MAX_NUM_PAIRS; p++){
        lower_arr = roll_multiple_dice(number_of_dice);
        lower_scoreboard[count] = check_pair(lower_arr, number_of_dice, p);
        count++;
        free(lower_arr);    
    }
    //ruller for 3 og 4 ens
    for(int q = 3; q <= MAX_NUM_KINDS; q++){
        lower_arr = roll_multiple_dice(number_of_dice);
        lower_scoreboard[count] = of_a_kind(lower_arr, number_of_dice, q);
        count++;
        free(lower_arr);
    }
    //ruller for lille og stor straight
    for(int r = 1; r <= 2; r++){
        lower_arr = roll_multiple_dice(number_of_dice);
        lower_scoreboard[count] = straight(lower_arr, number_of_dice, r);
        count++;
        free(lower_arr);
    }
    //ruller for fuldt hus
    lower_arr = roll_multiple_dice(number_of_dice);
    lower_scoreboard[count] = full_house(lower_arr, number_of_dice);
    count++;
    free(lower_arr);

    //ruller for chance
    lower_arr = roll_multiple_dice(number_of_dice);
    lower_scoreboard[count] = chance(lower_arr, number_of_dice);
    count++;
    free(lower_arr);

    //ruller for yatzi
    lower_arr = roll_multiple_dice(number_of_dice);
    lower_scoreboard[count] = yatzi(lower_arr, number_of_dice);
    free(lower_arr);

    return lower_scoreboard;
}

int check_pair(int *dice_arr, int number_of_dice, int pairs){
    int chosen_dice = 0;
    int *chosen_arr = (int*) calloc(pairs, sizeof(int));
    if(chosen_arr == NULL){
            printf("Array error. bb");
            exit(EXIT_FAILURE);
    }
    int index = 0;
    int result = 0;
    //itererer terningerne igennem
    for(int eyes = MAX_EYES; eyes >= 1; eyes--){
        chosen_dice = 0;
        for(int i = 0; i < number_of_dice; i++){
            if(dice_arr[i] == eyes){
                chosen_dice++;
            }
            //hvis vi har fundet to terninger, har vi fundet et par
            if(chosen_dice == 2){
                chosen_arr[index] = eyes;
                index++;
                i = number_of_dice;    
            }
            //hvis vi har gemt antal par vi leder efter = break
            if(index == pairs){
                eyes = 0;
                i = number_of_dice;
                break;
            }
        }
    }
    //resultat vil altid være 2*m + 2*n +...
    for(int m = 0; m < pairs; m++){
        result = result + (2 * chosen_arr[m]);
    }
    free(chosen_arr);
    return result;
}

int of_a_kind(int *dice_arr, int number_of_dice, int of_a_kind){
    int chosen_dice = 0;
    int chosen_arr[1] = {0};
    //kører så længe vi leder efter øjne mellem 6 og 1
    for(int eyes = MAX_EYES; eyes >= 1; eyes--){
        chosen_dice = 0;
        for(int i = 0; i < number_of_dice; i++){
            if(dice_arr[i] == eyes){
                chosen_dice++;
            }
            //hvis vi har fundet antal ens gemmes værdien og loop afsluttes
            if(chosen_dice == of_a_kind && chosen_arr[0] == 0){
                chosen_arr[0] = eyes;
                eyes = 0;
                i = number_of_dice;
                break;    
            }            
        }
    }
    return chosen_arr[0] * of_a_kind;
}

int straight(int *dice_arr, int number_of_dice, int min_num){
    int chosen_dice = 0;
    //der itereres 5 gange gennem terningerne
    for(int i = 0; i < MAX_EYES - 1; i++){
        for(int j = 0; j < number_of_dice; j++){
            //hvis terningen viser det næste tal vi leder efter
            if(dice_arr[j] == min_num + chosen_dice){
                chosen_dice++;
                j = number_of_dice;
            }
            if(chosen_dice == 5){
                //værdien af 5 på hinanden følgende terninger vil altid være 5*n + 10
                return 5 * min_num + 10;
            }
        }
    }
    return 0;
}

int full_house(int *dice_arr, int number_of_dice){
    int *check_of_kind = (int*) calloc(MAX_EYES, sizeof(int));
    int *check_if_pair = (int*) calloc(MAX_EYES, sizeof(int));
    if(check_of_kind == NULL || check_if_pair == NULL){
            printf("Array error. bb");
            exit(EXIT_FAILURE);
        }
    int result = 0;
    int count = 0;
    int indexkind = 0;
    int indexpair = 0;

    //samler alle øjne der er 3 ens af i array
    for(int eyes = MAX_EYES; eyes >= 1; eyes--){
        count = 0;
        for(int i = 0; i < number_of_dice; i++){
            if(dice_arr[i] == eyes){
                count++;
            }
            if(count == 3){
                check_of_kind[indexkind] = eyes;
                indexkind++;
                i = number_of_dice;
                break;    
            }            
        }
    }
    //samler alle øjne der er par af i array
    for(int eyes = MAX_EYES; eyes >= 1; eyes--){
        count = 0;
        for(int i = 0; i < number_of_dice; i++){
            if(dice_arr[i] == eyes){
                count++;
            }
            if(count == 2){
                check_if_pair[indexpair] = eyes;
                indexpair++;
                i = number_of_dice;
                break;    
            }
        }
    }
    //itererer igennem alle muligheder af 3 ens med alle fundne par
    for (int i = 0; i < MAX_EYES; i++){
        for(int j = 0; j < MAX_EYES; j++){
            //hvis kombinationen af ens og par er større, og hvis de 3 ens vi kigger på er forskellig fra parret
            if((3 * check_of_kind[i] + 2 * check_if_pair[j] > result) && check_of_kind[i] != check_if_pair[j]){
                result = 3 * check_of_kind[i] + 2 * check_if_pair[j];
            }
        }
    }
    //returnerer det størst fundne fulde hus
    return result;
}

int chance(int *dice_arr, int number_of_dice){
    int chosen_dice = 0;
    int chosen_arr[5];
    int result = 0;
    //itererer gennem terningerne fra 6 til 1
    for(int k = MAX_EYES; k >= 1; k--){
        for(int i = 0; i < number_of_dice; i++){
            //de højeste terninger gemmes, indtil 5 terninger er valgt
            if(dice_arr[i] == k){
                chosen_arr[chosen_dice] = k;
                chosen_dice++;
            }
            if(chosen_dice == 5){
                k = 0;
                i = number_of_dice;
                break;
            }
        }
    }
    for(int m = 0; m < 5; m++){
        result = result + chosen_arr[m];
    }
    return result;
}

int yatzi(int *dice_arr, int number_of_dice){
    int result = of_a_kind(dice_arr, number_of_dice, 5);
    //hvis der blev fundet 5 ens får man 50 i bonus
    if(result != 0){
        return result + 50;
    }
    else{
        return result;
    }
    
}

//håndterer print i terminal
void print_scoreboard(int *upper_score, int *lower_score, int bonus){
    int final_scoreboard[16];
    for(int i = 0; i < MAX_EYES; i++){
            final_scoreboard[i] = upper_score[i];
        }
    final_scoreboard[MAX_EYES] = bonus;
        
    for(int k = MAX_EYES + 1; k <= 16; k++){    
        final_scoreboard[k] = lower_score[k - (MAX_EYES + 1)];
    }
    
    printf("--------SCOREBOARD:--------\n");
    for(int k = 0; k < 16; k++){
        printf("%-17s %3d\n", write_names(k), final_scoreboard[k]);
    }
    printf("---------------------------\nTotal sum:        %3d\n\n", calc_total_sum(final_scoreboard));

    free(upper_score);
    free(lower_score);

    return;
}

int calc_total_sum(int *scoreboard_arr){
    int total_sum = 0;
    for(int i = 0; i < 16; i++){
        total_sum = total_sum + scoreboard_arr[i];
    }
    return total_sum;
}

//hjælpefunktion til at udskrive yatzinavne i scoreboard
char* write_names(names names){
    char *yatzi_name = (char*) malloc(17 * sizeof(char));
    switch (names)
    {
    case Ones:
        strcpy(yatzi_name, "Ones:");
        break;
    case Twoes:
        strcpy(yatzi_name, "Twoes:");
        break;
    case Threes:
        strcpy(yatzi_name, "Threes:");
        break;
    case Fours:
        strcpy(yatzi_name, "Fours:");
        break;
    case Fives:
        strcpy(yatzi_name, "Fives:");
        break;
    case Sixes:
        strcpy(yatzi_name, "Sixes:");
        break;
    case Bonus:
        strcpy(yatzi_name, "Bonus:");
        break;
    case OnePair:
        strcpy(yatzi_name, "One Pair:");
        break;
    case TwoPair:
        strcpy(yatzi_name, "Two Pair:");
        break;
    case ThreeOfaKind:
        strcpy(yatzi_name, "Three of a Kind:");
        break;
    case FourOfaKind:
        strcpy(yatzi_name, "Four of a Kind:");
        break;
    case SmallStraight:
        strcpy(yatzi_name, "Small Straight:");
        break;
    case LargeStraight:
        strcpy(yatzi_name, "Large Straight:");
        break;
    case FullHouse:
        strcpy(yatzi_name, "Full House:");
        break;
    case Chance:
        strcpy(yatzi_name, "Chance:");
        break;
    case Yatzi:
        strcpy(yatzi_name, "Yatzi:");
        break;
    default:
        break;
    }
    return yatzi_name;
}

