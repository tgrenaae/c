/*Programmet er skrevet af Tommy Grenaae d. 7/12 2020, tgrena20@student.aau.dk, 
fra gruppe SW0001, Software 2020*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_KAMPE 182
#define MAX_HOLD 14

//enums anvendt i programmet
typedef enum holdnavne {FCM, EFB, SDR, RFC, ACH, FCN, LBK, AaB, OB, FCK, BIF, SIF, HOB, AGF} holdnavne;

//structs anvendt i programmet
typedef struct kamp {
    char hjemmehold[4];
    int hjemmemaal;
    char udehold[4];
    int udemaal;
    char ugedag[4];
    char dato[6];
    char klokkeslet[6];
    int tilskuertal;
} kamp;

typedef struct hold {
    char *holdnavn;
    int point;
    int egnemaal;
    int andresmaal;
} hold;

//prototyper af funktioner
kamp *indlaes_kampe(FILE *fil);
hold *indlaes_hold(kamp *kamp_arr);
char *strholdnavn(holdnavne holdnavn);
void updater_point(kamp *kamp, hold *hold);
void updater_point_ude(kamp *kamp, hold *hold);
void updater_maal(kamp *kamp, hold *hold);
void updater_maal_ude(kamp *kamp, hold *hold);
void sorterhold(hold *hold_arr);
int sammenlign_hold(const void *hold_et, const void *hold_to);
void printresultat(hold *hold_arr);

int main(void){
    kamp *kamp_arr;
    hold *hold_arr;
    FILE *fil = fopen("kampe-2019-2020.txt", "r");
    if(fil == NULL){
        printf("fil error, farvel.\n");
        exit(EXIT_FAILURE);
    }
    kamp_arr = indlaes_kampe(fil);
    fclose(fil);
    hold_arr = indlaes_hold(kamp_arr);
    sorterhold(hold_arr);
    printresultat(hold_arr);
    return 0;
}

//indlæser kampene i et array af kamp structs
kamp *indlaes_kampe(FILE *fil){
    kamp *kamp_arr = (kamp *) malloc(MAX_KAMPE * sizeof(kamp));
    if(kamp_arr == NULL){
        printf("array error, farvel.\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < MAX_KAMPE; i++){
        fscanf(fil, " %[A-Za-z]", kamp_arr[i].ugedag);
        fscanf(fil, " %[^ ]", kamp_arr[i].dato);
        fscanf(fil, " %[^ ]", kamp_arr[i].klokkeslet);
        fscanf(fil, " %[A-Za-z]", kamp_arr[i].hjemmehold);
        fscanf(fil, " - %[A-Za-z]", kamp_arr[i].udehold);
        fscanf(fil, " %d", &kamp_arr[i].hjemmemaal);
        fscanf(fil, " - %d", &kamp_arr[i].udemaal);
        fscanf(fil, " %d", &kamp_arr[i].tilskuertal);
    }
    return kamp_arr;
}

//indlæser hold og deres stats
hold *indlaes_hold(kamp *kamp_arr){
    hold *hold_arr = (hold *) malloc(MAX_HOLD * sizeof(hold));
    if(hold_arr == NULL){
        printf("array error, farvel.\n");
        exit(EXIT_FAILURE);
    }
    int counthjemme = 0;
    int countude = 0;
    //for hvert hold, tjek hjemmekampe
    for(int i = FCM; i <= AGF; i++){
        hold_arr[i].holdnavn = strholdnavn(i);
        hold_arr[i].point = 0;
        hold_arr[i].egnemaal = 0;
        hold_arr[i].andresmaal = 0;
        //for hver kamp
        for(int j = 0; j <= MAX_KAMPE; j++){
            //sæt antallet af chars der passer i navnet til 0
            counthjemme = 0;
            //tjek hele holdnavnet
            for(int k = 0; k < 3; k++){
                if(kamp_arr[j].hjemmehold[k] == hold_arr[i].holdnavn[k]){
                    counthjemme++;
                    //hvis hele hjemmeholdnavnet passer
                    if(counthjemme == 3){
                        updater_point(&kamp_arr[j], &hold_arr[i]);
                        updater_maal(&kamp_arr[j], &hold_arr[i]);
                    }
                } 
            }   
        }       
    }
    //for hvert hold, tjek udekampe
    for(int i = FCM; i <= AGF; i++){
        //for hver kamp
        for(int j = 0; j <= MAX_KAMPE; j++){
            //sæt antallet af chars der passer i navnet til 0
            countude = 0;
            //tjek hele holdnavnet
            for(int k = 0; k < 3; k++){
                if(kamp_arr[j].udehold[k] == hold_arr[i].holdnavn[k]){
                    countude++;
                    //hvis hele udeholdnavnet passer
                    if(countude == 3){
                        updater_point_ude(&kamp_arr[j], &hold_arr[i]);
                        updater_maal_ude(&kamp_arr[j], &hold_arr[i]);
                    }
                }
            }   
        }       
    }
    return hold_arr;    
}

//switch til enum holdnavnene
char *strholdnavn(holdnavne holdnavn){
    char *strholdnavn = (char *) malloc(4 * sizeof(char));
    switch (holdnavn){
    case FCM:
        strcpy(strholdnavn, "FCM");
        break;
    case EFB:
        strcpy(strholdnavn, "EFB");
        break;
    case SDR:
        strcpy(strholdnavn, "SDR");
        break;
    case RFC:
        strcpy(strholdnavn, "RFC");
        break;
    case ACH:
        strcpy(strholdnavn, "ACH");
        break;
    case FCN:
        strcpy(strholdnavn, "FCN");
        break;
    case LBK:
        strcpy(strholdnavn, "LBK");
        break;
    case AaB:
        strcpy(strholdnavn, "AaB");
        break;
    case OB:
        strcpy(strholdnavn, "OB");
        break;
    case FCK:
        strcpy(strholdnavn, "FCK");
        break;
    case BIF:
        strcpy(strholdnavn, "BIF");
        break;
    case SIF:
        strcpy(strholdnavn, "SIF");
        break;
    case HOB:
        strcpy(strholdnavn, "HOB");
        break;
    case AGF:
        strcpy(strholdnavn, "AGF");
        break;
    default:
        break;
    }
    return strholdnavn;
}

//updaterer hjemmeholdets point
void updater_point(kamp *kamp, hold *hold){
    if(kamp->hjemmemaal > kamp->udemaal){
        hold->point = hold->point + 3; 
    }
    else if(kamp->hjemmemaal == kamp->udemaal){
        hold->point = hold->point + 1; 
    }
    else{
        hold->point = hold->point; 
    }
    return;
}

//updaterer udeholdets point
void updater_point_ude(kamp *kamp, hold *hold){
    if(kamp->udemaal > kamp->hjemmemaal){
        hold->point = hold->point + 3; 
    }
    else if(kamp->udemaal == kamp->hjemmemaal){
        hold->point = hold->point + 1; 
    }
    else{
        hold->point = hold->point; 
    }
    return;
}

//updaterer hjemmeholdets mål
void updater_maal(kamp *kamp, hold *hold){
    hold->egnemaal = hold->egnemaal + kamp->hjemmemaal;
    hold->andresmaal = hold->andresmaal + kamp->udemaal;
    return;
}

//updaterer udeholdets mål
void updater_maal_ude(kamp *kamp, hold *hold){
    hold->egnemaal = hold->egnemaal + kamp->udemaal;
    hold->andresmaal = hold->andresmaal + kamp->hjemmemaal;
    return;
}

void sorterhold(hold *hold_arr){
    qsort(hold_arr, MAX_HOLD, sizeof(hold), sammenlign_hold);
    return;
}

int sammenlign_hold(const void *hold_et, const void *hold_to){
    hold *hold1 = (hold *) hold_et;
    hold *hold2 = (hold *) hold_to;
    if(hold1->point < hold2->point){
        return -1;
    } else if(hold1->point > hold2->point){
        return 1;
    }
    else{
        if(hold1->egnemaal < hold2->egnemaal){
            return -1;
        } else if (hold1->egnemaal > hold2->egnemaal){
            return 1;
        } else{
            return 0;
        }
    }
}

void printresultat(hold *hold_arr){
    printf("Hold | Point | Egne maal | Andres maal\n");
    for(int i = MAX_HOLD-1; i >= 0; i--){
        printf("%-3s  |  %d   |     %d    |     %d\n", hold_arr[i].holdnavn, hold_arr[i].point, hold_arr[i].egnemaal, hold_arr[i].andresmaal);
    }
    return;
}

