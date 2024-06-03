#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"

#define segment_a GPIOE, GPIO_PIN_0
#define segment_b GPIOC, GPIO_PIN_1
#define segment_c GPIOC, GPIO_PIN_3
#define segment_d GPIOC, GPIO_PIN_2
#define segment_e GPIOG, GPIO_PIN_0
#define segment_f GPIOD, GPIO_PIN_5
#define segment_g GPIOD, GPIO_PIN_6

int turnON (int cislo) {

}

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB,GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);

    // 7 segment

    GPIO_Init(segment_a, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_b, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_c, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_d, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_e, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_f, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_g, GPIO_MODE_OUT_PP_HIGH_SLOW);

    init_milis();
}


int main(void)
{
    init();

    uint32_t time = 0;

    while(1) {
        if(milis() - time > 500) {
            time = milis();
            REVERSE(segment_a);
            REVERSE(segment_b);
            REVERSE(segment_c);
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
