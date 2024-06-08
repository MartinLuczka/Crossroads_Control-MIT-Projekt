#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
#include "delay.h"

#define segment_a GPIOE, GPIO_PIN_0
#define segment_b GPIOC, GPIO_PIN_1
#define segment_c GPIOC, GPIO_PIN_3
#define segment_d GPIOC, GPIO_PIN_2
#define segment_e GPIOG, GPIO_PIN_0
#define segment_f GPIOD, GPIO_PIN_5
#define segment_g GPIOD, GPIO_PIN_6

#define button GPIOA, GPIO_PIN_6

#define crossing_lights_green GPIOB, GPIO_PIN_7
#define crossing_lights_red GPIOB, GPIO_PIN_6

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    // 7 segment

    GPIO_Init(segment_a, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_b, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_c, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_d, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_e, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_f, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_g, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // tlačítko

    GPIO_Init(button, GPIO_MODE_IN_PU_NO_IT);

    // LED semaforu přechodu

    GPIO_Init(crossing_lights_green, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(crossing_lights_red, GPIO_MODE_OUT_PP_LOW_SLOW);

    // povolení milis()

    init_milis();
}

const uint8_t numbers[] =
{
    0b11000000, // číslo 0
    0b11111001, // číslo 1
    0b10100100, // číslo 2
    0b10110000, // číslo 3
    0b10011001, // číslo 4
    0b10010010, // číslo 5
    0b10000010, // číslo 6
    0b11111000, // číslo 7
    0b10000000, // číslo 8
    0b10010000, // číslo 9
    0b11111111, // 7segmentová obrazovka je zhasnuta
};

void show_number(uint8_t number) {
    if (number & 1<<0) {
        HIGH(segment_a);
    }
    else {
        LOW(segment_a);
    }
    if (number & 1<<1) {
        HIGH(segment_b);
    }
    else {
        LOW(segment_b);
    }
    if (number & 1<<2) {
        HIGH(segment_c);
    }
    else {
        LOW(segment_c);
    }
    if (number & 1<<3) {
        HIGH(segment_d);
    }
    else {
        LOW(segment_d);
    }
    if (number & 1<<4) {
        HIGH(segment_e);
    }
    else {
        LOW(segment_e);
    }
    if (number & 1<<5) {
        HIGH(segment_f);
    }
    else {
        LOW(segment_f);
    }
    if (number & 1<<6) {
        HIGH(segment_g);
    }
    else {
        LOW(segment_g);
    }
}

void crossing_activated(void) {
    HIGH(crossing_lights_red);
    LOW(crossing_lights_green);
    uint8_t crossing_time_number = 8; // může se zde upravit
    uint32_t time = 0;
    while (crossing_time_number > 0) {
        if(milis() - time > 1000) {
            time = milis();
            show_number(numbers[crossing_time_number]);
            if (crossing_time_number == 1) {
                delay_ms(1000);
            }
            crossing_time_number -= 1;
        }
    }
    show_number(numbers[10]); // zhasnutí 7segmentovky
    LOW(crossing_lights_red);
    HIGH(crossing_lights_green);
}


int main(void)
{
    init();

    uint32_t time = 0;
    bool stisk = 0;

    while(1) {
        if(milis() - time > 100) {
            time = milis();
            if(PUSH(button)) {
                stisk = 1;
            }
            else {
                if(stisk == 1) {
                    crossing_activated();
                    stisk = 0;
                }
            }
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
