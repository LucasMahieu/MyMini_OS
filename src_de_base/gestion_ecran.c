#include <gestion_ecran.h>
#include <cpu.h>
#include <string.h>
#include <inttypes.h>

static int LIGNE=0;
static int COLONNE=0;
#define FORMAT 0x0F


/**
 * retour un pointeur vers la zone memoire corespondant
 * a la ligne et colonne donné en entrée.
 *
 */
uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    uint16_t* ptr;
    // Oxb8000 = adresse de base de la zone memoire qui corespond a l'affichage
    // la zone mem est contigue et ligne apres ligne.
    // chaque position est codée sur 2 octets 
    // ainsi pour avoir la lig ieme ligne il faut aller dans la ligne lig*80*2
    // pour tomber sur la colonne col, il faut donc ajouter 80*2
    // en ajoutant cela a l'@ de base on arrive sur la zone mem de (lig, col)
    ptr = (uint16_t*)(0xb8000 + 2*(lig*80 + col));
    return ptr;
}

/**
 * Ecrit le caractere 'c' dans le format 'format'
 * à la ligne 'lig' et la colonne 'col'
 */
void ecrit_car(uint32_t lig, uint32_t col, char c,uint8_t format){
    uint16_t* pos_mem;
    // Calcule de la position en mémoire corespondant à lig col
    // et on remplie les 2 octets en fonction du caractere et du format d'entrée
    pos_mem = ptr_mem(lig, col);
    *pos_mem = (((uint16_t)(format)<<8) | (uint16_t)(c));
}

/**
 * Ecrit la chaine de caractere en haut à droite de l'ecran
 * Peu etre utilisé pour afficher l'heure
 */
void print_top_right(char *chaine){
    int taille=0;
    int i=0;
    taille = strlen(chaine);
    for (i=taille-1;i>-1;i--){
        ecrit_car(0,80-taille+i,chaine[i],0x0F);
    } 
}

/**
 * Place le curseur sur la position (ligne,col)
 *
 */
void place_curseur (uint32_t ligne, uint32_t col){
    uint16_t pos=0;
    uint8_t pos_basse=0;
    uint8_t pos_haute=0;

    // Calcul de la position du curseur
    pos=col+ligne*80;
    pos_basse =(uint8_t) pos;
    pos_haute =(uint8_t) (pos >> 8);
    outb(0x0F,0x3D4); //On va envoyer la partie basse juste après
    outb(pos_basse,0x3D5);
    outb(0x0E,0x3D4); //On va envoyer la partie haute juste après
    outb(pos_haute,0x3D5);
}

/**
 * Va afficher un espace sur fond noir sur toutes les cases de l'ecran
 */
void efface_ecran(void){
    
    int lig=0,col=0;

    for(lig=0;lig<25;lig++){
        for(col=0;col<80;col++){
            ecrit_car(lig,col,0x20,0x0F);    
        }
    }
}

/*
 * Traite les caracteres spéciaux en fonction de leru signification  
 */
void traite_car(char c){
    int8_t div;
    if (c>31 && c<127){
            ecrit_car(LIGNE,COLONNE,c,FORMAT);
            COLONNE+=1;
            place_curseur(LIGNE,COLONNE);           
    }
    
    switch(c){
        case '\b':
            if(COLONNE>0) {
                COLONNE-=1;
                place_curseur(LIGNE,COLONNE);
            }
        break;
        case '\t':
            div=(COLONNE/8)+1;
            if (div<10) COLONNE = div*8;
            else {
                COLONNE=0;
                LIGNE+=1;
            }
                place_curseur(LIGNE,COLONNE);
        break;
        case '\n':
            if (LIGNE<24) {
            LIGNE+=1;
            COLONNE=0;
            place_curseur(LIGNE,COLONNE);
            }
            else {
                defilement();
                traite_car(c);
            }
        break;
        case '\f':
            efface_ecran();
            place_curseur(0,0);
        break;
        case '\r':
            COLONNE=0;
            place_curseur(LIGNE,COLONNE);
        break;
        default:
        break;
    } 
}

/**
 * Faire 'remonter' d'une ligne tout ce qui est affiché à l'ecran 
 */
void defilement(void){
    uint8_t i=0;
    memmove(ptr_mem(0,0),ptr_mem(1,0),sizeof(uint16_t)*80*24); //memmove(*dest,*src,size)
    for (i=0;i<80;i++) ecrit_car(24,i,0x20,0x0F);
    LIGNE--;
    place_curseur(LIGNE,COLONNE);
}

/*
 * Fonction appelé par printf() 
 */
void console_putbytes(char *chaine, int32_t taille){
uint32_t i=0;
    for(i=0;i<taille;i++){
        traite_car(chaine[i]);
    }
}









