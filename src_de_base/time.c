#include <inttypes.h> 
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <gestion_ecran.h>
#include <cpu.h>
#include <stdio.h>
#include <segment.h>
#include <processus.h>


/**
 * fonction executé par le traitant à chaque interuption
 */
void tic_PIT(void){
    outb(0x20,0x20);
    char to_print[20];
    uint8_t n=0;
    
    if(cmpt>49){
        cmpt=0;
        if(SECONDES>58){
            if(MINUTES>58){
                HEURES++;
                MINUTES=0;
                SECONDES=0;
            }
            else{
                MINUTES++;
                SECONDES=0;
            }
        }
        else SECONDES++;
    }
    else {
        cmpt++;
    }
    n=sprintf(to_print,"%02d:%02d:%02d",HEURES,MINUTES,SECONDES);
    print_top_right(to_print);
    ordonnance();
}

/**
 * Initialisation du traitant d'init
 */
void init_traitant_IT(int32_t num_IT, void (*traitant)(void)){
    uint32_t *addr;
    uint32_t mot1,mot2;
    uint32_t lsb_traitant , msb_traitant;
    uint32_t cpy_traitant = (uint32_t)traitant;

    lsb_traitant = cpy_traitant & 0x0000FFFF ;
    msb_traitant = cpy_traitant & 0xFFFF0000 ;
    mot1 = 0x00100000 | lsb_traitant  ;
    mot2 = 0x00008E00 | msb_traitant;
    // Calcule de l'adr où l'on va ecrire les 2 mots
    addr=(uint32_t*)(0x1000 + (uint32_t)num_IT*8);
    // cpy du 1er mot 
    *(addr) = mot1;
    // cpy du second mot
    *(addr+1) = mot2;
}
/**
 * init du générateur d'interuption
 * horloge matérielle
 */
void init_clock_freq(void)
{
    uint8_t freq_lsb=0;
    uint8_t freq_msb=0;

    freq_lsb =(uint16_t)((QUARTZ/CLOCK_FREQ)%256) ;
    freq_msb =(uint16_t)((QUARTZ/CLOCK_FREQ)/256) ;
    outb(0x34,0x43); 
    // Informe que l'on va envoyer le nouvelle valeur de freq d'horloge

    outb(freq_lsb,0x40); 
    outb(freq_msb,0x40); 
}

/**
 * Fonction qui permet de masquer ou de demasquer (en fonction de masque) 
 * l'IRQ numero num_IRQ
 */
void masque_IRQ(uint8_t num_IRQ,int  masque)
{
    uint8_t state_irq;
    uint8_t m;
    if(masque!=0)
    { // on desactive les it de irq => on place un 1 sur le bit num_IRQ
        state_irq = inb(0x21);
        m =( (uint8_t)(0x1<<(7-num_IRQ)) ) | state_irq ;

        outb(m,0x21); 
    }
    if(masque==0)
    { // On active les it de num_irqirq => on place un 0 sur le bit num_IRQ
        state_irq = inb(0x21);
        m = (0b11111111 & (uint8_t)(0<<(7-num_IRQ)) ) & state_irq ;

        outb(m,0x21) ;
    }
}

/**
 * Retourne le nombre de seconde qu'il s'est écoulé depuis le debut du lancement de l'os 
 */
uint32_t nb_secondes(void){
    return SECONDES+60*MINUTES+60*60*HEURES;
}
