/*




*/
#include "control.h"
#include "fatfs.h"
#include "effect.h"
/****************************** variable ***************************************/

FATFS fatfs;
FIL  myfile;
FRESULT fresult;
UINT br; 

static uint8_t byte_init[5];    /* save byte init*/

 uint8_t byte_data[100];  /* save data eff*/
static uint8_t file_name[20];   /* save file name*/

static uint8_t ctr_byte_chanel;             /* so byte luu du lieu on or off cua cac kenh, chieu ngang*/
static uint16_t ctr_long_data_eff;          /* chieu dai cua hieu ung */
static uint8_t   ctr_repeat;                /* So lan lap lai cua mot hieu ung */


uint8_t chanel_current = 0;


/*********************************CODE******************************************/

void read_byte_init()
{
    f_read(&myfile,byte_init,4,&br);
    
    __NOP();__NOP();__NOP();__NOP();
    
    ctr_byte_chanel = byte_init[0];
    ctr_repeat = byte_init[1];
    ctr_long_data_eff = ((((uint16_t)byte_init[2])<<8)|(uint16_t)byte_init[3]);

    eff_init(ctr_byte_chanel,ctr_long_data_eff,ctr_repeat);
}


void read_data_eff(void)
{
    f_read(&myfile,byte_data,ctr_byte_chanel,&br);
}



/* main */

void control_main_eff(void)
{
    static uint16_t delay_count = 0;
    static uint8_t  count = 0;
    
    sprintf((char*)file_name,"eff1.bin");
    while(1)
    {
        HAL_Delay(20);
        HAL_GPIO_TogglePin(LED_BLINK_GPIO_Port,LED_BLINK_Pin);

        if(input_chanel() != chanel_current)// neu co su thay doi kenh dau vao
        {
            chanel_current = input_chanel();

        }

        if(chanel_current != 0)
        {
            sprintf((char*)file_name,"eff%d.bin",chanel_current);

            fresult = f_open(&myfile,(char*)file_name,FA_READ); /* opent file */
            if(fresult == FR_OK)
            {
                read_byte_init();    // khoi tao cac gia tri ban dau cua file hieu ung
                while((input_chanel() == chanel_current) && (ctr_long_data_eff >0))
                {
                    fresult = f_read(&myfile,byte_data,ctr_byte_chanel,&br);/* read data*/ 

                    delay_count = (((uint16_t)byte_data[0])<<8|((uint16_t)byte_data[1]));
                    shadow_data();                          /* đẩy các byte trước đi xa, tránh bị trùng khi nó nhiều module*/

                    for(count = 0;count <= ctr_byte_chanel ;count++)
                    {
                        CD4094_InByte(byte_data[(ctr_byte_chanel + 2) - count]);
                    }
                    CD4094_Latch();
                    HAL_Delay(delay_count);/* delay step epp*/
                    ctr_long_data_eff--;
                }
            }
            f_close(&myfile);   /* close file */
        }
        else /* end show eff*/
        {
            shadow_data();
            CD4094_Latch();
        }

    }
}










void control(void)
{
   if(BSP_SD_Init() == MSD_OK);
    {
        fresult = f_mount(&fatfs,"",1);
        if(fresult ==FR_OK)
        {

            control_main_eff();

        }
    }
}








