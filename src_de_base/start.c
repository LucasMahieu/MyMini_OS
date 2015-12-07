#include <cpu.h>
#include <inttypes.h>
#include <gestion_ecran.h>
#include <stdio.h>
#include <time.h>
#include <processus.h>
//#include <check.h>

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

uint16_t cmpt=0,SECONDES=0,MINUTES=0,HEURES=0;
uint16_t CLOCK_FREQ = 50;

// declaration en globale de la table des processus
proc table_proc[TAILLE_TABLE_PROC];
proc* current_proc ;

void kernel_start(void)
{ 
    cli();
    init_traitant_IT(32,traitant_IT_32); 
    init_clock_freq();

    ecrit_car(0,0,0x41,0x0F); //lettre A blanche sur fond noir TEST 
    efface_ecran();
    masque_IRQ(0,0);

    // Initialisation des structure de processus
    init_proc(0, 0, "idle", ELU,0, idle);
    init_proc(1, 1, "proc1", ACTIVABLE, 0, proc1);
    init_proc(2, 2, "proc2", ACTIVABLE, 0, proc2);
    init_proc(3, 3, "proc3", ACTIVABLE, 0, proc3);

    // Appel de la fonction par default idle
    idle();
}



// ***********SEANCE 2 GESTION DU TEMPS = CA MARCHE *******************************
/*    cli();
    init_traitant_IT(32,traitant_IT_32); 
    init_clock_freq();

    ecrit_car(0,0,0x41,0x0F); //lettre A blanche sur fond noir
    efface_ecran();
    masque_IRQ(0,0);

//    sti();
    while(1){
        hlt();
    }

 }
*/


//############################ SCEANCE 1 EXEMPLE FACT ################

    //uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
   // (void)x;
    // on ne doit jamais sortir de kernel_start
 //   while (1) {
        // cette fonction arrete le processeur
//        hlt();
//    }
//}

