#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <cpu.h>
#include "processus.h"
#include "gestion_ecran.h"

/**
 * Cette fonction permet d'initialiser n'importe quel processus dans la table de processus
 * Elle peut aussi être utilisé pour remplacer un processus dans la table
 */
void init_proc(int indice, int16_t ipid, char* inom, Etat ietat,uint32_t sommeil, void* fonction) 
{
	//pid 
    table_proc[indice].pid = ipid;
	//nom
    strcpy(table_proc[indice].nom,inom);        
	//Etat
    table_proc[indice].etat = ietat;
	//Heure de reveil
    if(ietat == ENDORMI){
        table_proc[indice].reveil = nb_secondes() + sommeil;
    }
	else{
		table_proc[indice].reveil = 0;
	}
	// Dans l'avant derniere case de la pile : @ de la fonction à exec
    table_proc[indice].pile[TAILLE_PILE-2] = (uint32_t)fonction;
	// Dans la derniere case de la pile : @ de la fonction de fin de proc
    table_proc[indice].pile[TAILLE_PILE-1] = (uint32_t)fin_proc;
	// Dans la 2eme case du contexte 
    table_proc[indice].ctx[1] =(uint32_t) &(table_proc[indice].pile[TAILLE_PILE-2]);
	// Maj du proc courrant
    if(ietat == ELU){
        current_proc = &table_proc[indice]; 
    }
}

/**
 * Cette fonction créer une nouveau processus
 * En fait il permet de remplacer un processus mort UNIQUEMENT 
 * Si le processus n'a pas pu être créer (pas de proc MORT)
 * La fonction retourne -1
 * Sinon, si la création à réussi, il retourne 0
 */
int  create_proc(char* nom, void* func){
    uint8_t i=0;
    for(i=0; i<TAILLE_TABLE_PROC; i++){
        if(table_proc[i].etat == MORT){
            init_proc(i,i, nom, ACTIVABLE, 0, func);
            return 0;
        }
    }
    // Pas de processus Mort pour en creer un nouveau
    // Table pleine
    return -1;
}

/**
 * Scan tous les processus pour savoir si ils doivent être réveillés 
 */
void maj_table(void){
    int i=0;
    for(i=0; i<TAILLE_TABLE_PROC; i++){
        if( (table_proc[i].etat==ENDORMI) && (table_proc[i].reveil <= nb_secondes()) ){
            table_proc[i].etat = ACTIVABLE;
        }
    }
}

/**
 * Fonction de terminaison de processus 
 */
void fin_proc(void) {
    (*current_proc).etat = MORT;
    ordonnance();
}


/**
 * Ordonnanceur du noyau
 */
void ordonnance(void){
    proc* old_proc = current_proc;
    // Pour distingué du proc qui vient de mourir et de celui qui est préempté 
    if ((*old_proc).etat==ELU){
        (*old_proc).etat = ACTIVABLE;
    }
    int i=1;
    // Mis a jour des états de la table de proc
    // => Reveille des eventuels proc à l'heure du levé
    // Driiiinnnnnnngggg !!!!! 
    maj_table();
    // Et mtn on va choisir le 1er proc activable de la liste 
    while( table_proc[((*old_proc).pid+i)%TAILLE_TABLE_PROC].etat != ACTIVABLE ){
        i++;
    }
    // On met a jour le proc courant et son etat 
    current_proc = &table_proc[((*old_proc).pid+i)%TAILLE_TABLE_PROC];
    (*current_proc).etat = ELU;
    //On affiche l'état de tous les processus avant le chgt de ctx
    print_proc_stat();
    // chgt de ctx
    ctx_sw((*old_proc).ctx, (*current_proc).ctx);
}


/**
 * Fait dormir les processus
 */
void dors(uint32_t som)
{
    (*current_proc).etat = ENDORMI;
    (*current_proc).reveil = nb_secondes() + som;
    ordonnance();
}

/**
 * Cette fonction permet d'afficher l'etat de tous les processus.
 */
void print_proc_stat(void)
{
    uint8_t i=0;
    uint8_t j=0;
    uint16_t taille=20;
    char toPrint[20];
    for(i=0; i<TAILLE_TABLE_PROC; i++){
        // Calcul la taille de la chaine a afficher (E = 1ere lettre de l'etat)
        switch(table_proc[i].etat){
            case ACTIVABLE:
                sprintf(toPrint,"%s: %s",table_proc[i].nom,"ACTIVABLE");
                break;
            case ELU:
                sprintf(toPrint,"%s: %s",table_proc[i].nom,"ELU :)");
                break;
            case MORT:
                sprintf(toPrint,"%s: %s",table_proc[i].nom,"MORT x_x");
                break;
            case ENDORMI:
                sprintf(toPrint,"%s: %s",table_proc[i].nom,"ENDORMI Zzz!");
                break;
            default:
                break;
        }
        for(j=0; j<taille; j++){
            // on écrit sur la ligne i+1 a droite de l'ecran (en 0 il y a l'heure)
            // 0x14 = rouge sur fond bleu
            if(j>strlen(toPrint)-1){
                ecrit_car(i+1,80-taille+j,' ',0x00);
            }
            else{
                ecrit_car(i+1,80-taille+j,toPrint[j],0x14);
            }
        }
    }
}

/**
 * LES 4 PROCESSUS CRéER
 */
void idle(void){ 
    for(;;) {
        sti();
        hlt();
        cli();
    }
    printf("[idle] j'arrete le systeme\n");
    hlt();

}
void proc1(void)
{
    dors(1);
    printf("[time=%d]: %s pid=%d\n",nb_secondes(),(*current_proc).nom,(*current_proc).pid);
    printf(" Ł>essaye de creer procC: %d\n",create_proc("procC", proc3));
}
void proc2(void)
{
    dors(2);
    printf("[time=%d]: %s pid=%d\n",nb_secondes(),(*current_proc).nom,(*current_proc).pid);
    printf(" Ł> essaye de creer procA: %d\n",create_proc("procA", proc1));
}
void proc3(void)
{
    dors(3);
    printf("[time=%d]: %s pid=%d\n",nb_secondes(),(*current_proc).nom,(*current_proc).pid);
    printf(" Ł> essaye de creer procB: %d\n",create_proc("procB", proc2));
}
