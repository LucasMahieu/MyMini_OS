#ifndef __PROCESSUS_H__
#define __PROCESSUS_H__

#include <inttypes.h>
#define TAILLE_NOM_PROC 20
#define TAILLE_CTX 5
#define TAILLE_PILE 1024
#define TAILLE_TABLE_PROC 4


enum Etat {
    ELU, ACTIVABLE, ENDORMI, MORT
};
typedef enum Etat Etat;

typedef struct Proc {
    int16_t pid;
    char nom[TAILLE_NOM_PROC];
    Etat etat;
    uint32_t reveil;
    uint32_t ctx[TAILLE_CTX];
    uint32_t pile[TAILLE_PILE];    
} proc;

void init_proc(int, int16_t, char*, Etat, uint32_t, void*);
void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);
void ctx_sw(uint32_t* ,uint32_t*);
void ordonnance(void);
void dors(uint32_t nb_sec);
uint32_t nb_secondes(void);
void maj_table(void);
void fin_proc(void);
void print_proc_stat(void);

extern proc table_proc[];
extern proc* current_proc;
extern uint16_t SECONDES,MINUTES,HEURES;
#endif
