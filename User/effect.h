/*



*/
#ifndef __EFFECT_H
#define __EFFECT_H
#include "main.h"

/****************************** variable ***************************************/




/*********************************CODE******************************************/


/*   init element of file effect */
void eff_init(uint8_t num_byte_chanel, uint16_t num_long_eff, uint8_t nnum_repeat);

/*     */

void shadow_data(void);
/* function check 4 input and return what value chanel input */
uint8_t input_chanel(void);

/* run eff */
void run_step_eff();







#endif





