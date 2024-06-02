#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);

    init_milis();
}


int main(void)
{
    init();

    uint32_t time = 0;

    bool aktualni_stav_tlacitka;

    while(1) {
        if(milis() - time > 100) {
            time = milis();
            if (GPIO_ReadInputPin(GPIOB,GPIO_PIN_2) == RESET) {
                aktualni_stav_tlacitka = 1;
            }
            else {
                aktualni_stav_tlacitka = 0;
            }
            if (aktualni_stav_tlacitka == 1) {
                GPIO_WriteLow(GPIOB, GPIO_PIN_5);
            }
            else {
                GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
            }
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
