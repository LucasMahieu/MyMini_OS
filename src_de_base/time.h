#ifndef __START_H__
#define __START_H__

void print_top_right(char *chaine);
void tic_PIT(void);
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));
void traitant_IT_32(void);
void init_clock_freq(void);
void masque_IRQ(uint8_t num_IRQ, int  masque);

extern uint16_t cmpt;
extern uint16_t SECONDES;
extern uint16_t MINUTES;
extern uint16_t HEURES;

extern uint16_t CLOCK_FREQ;

#define QUARTZ 0x1234DD

#endif
