#ifndef __gestion_ecran_h__ 
#define __gestion_ecran_h__ 

#include <inttypes.h>
uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c,uint8_t format);
void efface_ecran(void);
void place_curseur(uint32_t , uint32_t col);
void defilement(void);
void traite_car(char );
void console_putbytes(char*,int32_t);
#endif
