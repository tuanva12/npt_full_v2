/*



*/

#include "effect.h"
#include "cd4094.h"
/****************************** variable ***************************************/
static uint8_t byte_chanel;             /* so byte luu du lieu on or off cua cac kenh, chieu ngang*/
static uint16_t long_data_eff;          /* chieu dai cua hieu ung */
static uint8_t   repeat;                /* So lan lap lai cua mot hieu ung */


extern uint8_t byte_data[100];  /* save data eff*/
/*********************************CODE******************************************/

/*   init element of file effect */
void eff_init(uint8_t num_byte_chanel, uint16_t num_long_eff, uint8_t nnum_repeat)
{
    byte_chanel = num_byte_chanel;
    long_data_eff = num_long_eff;
    repeat = nnum_repeat;
}


/*     */

void shadow_data(void)
{
    uint8_t index;
    for(index = 0;index<10;index++)
    {
        CD4094_InByte(0x00);
    }
}

/* function check 4 input and return what value chanel input */
uint8_t input_chanel(void)
{
    uint8_t data = 0x00;
    if(BT1_Is_Low())
    {
        data |= 0x01;
    }
    if(BT2_Is_Low())
    {
        data |= 0x02;
    }
    if(BT3_Is_Low())
    {
        data |= 0x04;
    }
    if(BT4_Is_Low())
    {
        data |= 0x08;
    }
    return data;
}


/* run eff */
void run_step_eff()
{
    uint16_t value_delay = 0;
    uint8_t index = 0;
    value_delay = (((uint16_t)byte_data[0])<<8|((uint16_t)byte_data[1]));
    shadow_data();                          /* đẩy các byte trước đi xa, tránh bị trùng khi nó nhiều module*/
    for(index = (byte_chanel -1);index >=0;--index)
    {
        CD4094_InByte(byte_data[index+2]);
    }
    CD4094_Latch();
    HAL_Delay(value_delay);/* delay step epp*/
}








