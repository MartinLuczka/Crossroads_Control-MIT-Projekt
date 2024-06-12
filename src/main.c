#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
#include "delay.h"

// makra rozdělit na porty a piny (abychom mohli posílat do funkcí, psát to řádek po řádku je MOR, hnus), tím pádem se bude měnit i ty makra v main.h, jak na to ve stažených souborech

#define segment_a_PORT GPIOE
#define segment_a_PIN  GPIO_PIN_0

#define segment_b_PORT GPIOC
#define segment_b_PIN  GPIO_PIN_1

#define segment_c_PORT GPIOC
#define segment_c_PIN  GPIO_PIN_3

#define segment_d_PORT GPIOC
#define segment_d_PIN  GPIO_PIN_2

#define segment_e_PORT GPIOG
#define segment_e_PIN  GPIO_PIN_0

#define segment_f_PORT GPIOD
#define segment_f_PIN  GPIO_PIN_5

#define segment_g_PORT GPIOD
#define segment_g_PIN  GPIO_PIN_6

#define button_PORT GPIOA
#define button_PIN  GPIO_PIN_6

#define speaker_PORT GPIOG
#define speaker_PIN GPIO_PIN_6

#define crossing_lights_green_PORT GPIOB
#define crossing_lights_green_PIN  GPIO_PIN_7

#define crossing_lights_red_PORT GPIOB
#define crossing_lights_red_PIN  GPIO_PIN_6

#define A_straight_green_PORT GPIOB
#define A_straight_green_PIN  GPIO_PIN_0

#define A_straight_orange_PORT GPIOB
#define A_straight_orange_PIN  GPIO_PIN_1

#define A_straight_red_PORT GPIOB
#define A_straight_red_PIN  GPIO_PIN_2

#define A_right_green_PORT GPIOB
#define A_right_green_PIN  GPIO_PIN_3

#define A_right_orange_PORT GPIOB
#define A_right_orange_PIN  GPIO_PIN_4

#define A_right_red_PORT GPIOB
#define A_right_red_PIN  GPIO_PIN_5

#define B_left_green_PORT GPIOE
#define B_left_green_PIN  GPIO_PIN_2

#define B_left_orange_PORT GPIOE
#define B_left_orange_PIN  GPIO_PIN_1

#define B_left_red_PORT GPIOD
#define B_left_red_PIN  GPIO_PIN_7

#define B_right_green_PORT GPIOE
#define B_right_green_PIN  GPIO_PIN_5

#define B_right_orange_PORT GPIOC
#define B_right_orange_PIN  GPIO_PIN_6

#define B_right_red_PORT GPIOC
#define B_right_red_PIN  GPIO_PIN_7

#define C_left_green_PORT GPIOF
#define C_left_green_PIN  GPIO_PIN_5

#define C_left_orange_PORT GPIOF
#define C_left_orange_PIN  GPIO_PIN_6

#define C_left_red_PORT GPIOF
#define C_left_red_PIN  GPIO_PIN_7

#define C_straight_green_PORT GPIOF
#define C_straight_green_PIN  GPIO_PIN_0

#define C_straight_orange_PORT GPIOF
#define C_straight_orange_PIN  GPIO_PIN_3

#define C_straight_red_PORT GPIOF
#define C_straight_red_PIN  GPIO_PIN_4

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    // 7 segment
    GPIO_Init(segment_a_PORT, segment_a_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_b_PORT, segment_b_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_c_PORT, segment_c_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_d_PORT, segment_d_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_e_PORT, segment_e_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_f_PORT, segment_f_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(segment_g_PORT, segment_g_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // reproduktor

    GPIO_Init(speaker_PORT, speaker_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    // tlačítko
    GPIO_Init(button_PORT, button_PIN, GPIO_MODE_IN_PU_NO_IT);

    // LED semaforu přechodu
    GPIO_Init(crossing_lights_green_PORT, crossing_lights_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(crossing_lights_red_PORT, crossing_lights_red_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    // směr křižovatky A
    GPIO_Init(A_straight_red_PORT, A_straight_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_straight_orange_PORT, A_straight_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_straight_green_PORT, A_straight_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_red_PORT, A_right_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_orange_PORT, A_right_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(A_right_green_PORT, A_right_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);


    // směr křižovatky B
    GPIO_Init(B_left_red_PORT, B_left_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_left_orange_PORT, B_left_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_left_green_PORT, B_left_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_red_PORT, B_right_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_orange_PORT, B_right_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(B_right_green_PORT, B_right_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // směr křižovatky C
    GPIO_Init(C_left_red_PORT, C_left_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_left_orange_PORT, C_left_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_left_green_PORT, C_left_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_red_PORT, C_straight_red_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_orange_PORT, C_straight_orange_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(C_straight_green_PORT, C_straight_green_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);

    // povolení milis()
    init_milis();
}

// Pole pro porty
const GPIO_TypeDef* all_ports[] = {
    A_straight_red_PORT,
    A_straight_orange_PORT,
    A_straight_green_PORT,
    A_right_red_PORT,
    A_right_orange_PORT,
    A_right_green_PORT,
    B_left_red_PORT,
    B_left_orange_PORT,
    B_left_green_PORT,
    B_right_red_PORT,
    B_right_orange_PORT,
    B_right_green_PORT,
    C_left_red_PORT,
    C_left_orange_PORT,
    C_left_green_PORT,
    C_straight_red_PORT,
    C_straight_orange_PORT,
    C_straight_green_PORT
};

// Pole pro piny
const uint16_t all_pins[] = {
    A_straight_red_PIN,
    A_straight_orange_PIN,
    A_straight_green_PIN,
    A_right_red_PIN,
    A_right_orange_PIN,
    A_right_green_PIN,
    B_left_red_PIN,
    B_left_orange_PIN,
    B_left_green_PIN,
    B_right_red_PIN,
    B_right_orange_PIN,
    B_right_green_PIN,
    C_left_red_PIN,
    C_left_orange_PIN,
    C_left_green_PIN,
    C_straight_red_PIN,
    C_straight_orange_PIN,
    C_straight_green_PIN
};


const GPIO_TypeDef* mode_1_PORT[] = {
    B_right_red_PORT,
    B_left_red_PORT,
    A_right_red_PORT,
    B_right_orange_PORT,
    B_left_orange_PORT,
    A_right_orange_PORT,
    B_right_green_PORT,
    B_left_green_PORT,
    A_right_green_PORT,
};

const uint8_t mode_1_PIN[] = {
    B_right_red_PIN,
    B_left_red_PIN,
    A_right_red_PIN,
    B_right_orange_PIN,
    B_left_orange_PIN,
    A_right_orange_PIN,
    B_right_green_PIN,
    B_left_green_PIN,
    A_right_green_PIN,
};

const GPIO_TypeDef* mode_2_PORT[] = {
    A_straight_red_PORT,
    A_right_red_PORT,
    C_straight_red_PORT,
    A_straight_orange_PORT,
    A_right_orange_PORT,
    C_straight_orange_PORT,
    A_straight_green_PORT,
    A_right_green_PORT,
    C_straight_green_PORT,
};

const uint8_t mode_2_PIN[] = {
    A_straight_red_PIN,
    A_right_red_PIN,
    C_straight_red_PIN,
    A_straight_orange_PIN,
    A_right_orange_PIN,
    C_straight_orange_PIN,
    A_straight_green_PIN,
    A_right_green_PIN,
    C_straight_green_PIN,
};

const GPIO_TypeDef* mode_3_PORT[] = {
    C_straight_red_PORT,
    C_left_red_PORT,
    B_right_red_PORT,
    C_straight_orange_PORT,
    C_left_orange_PORT,
    B_right_orange_PORT,
    C_straight_green_PORT,
    C_left_green_PORT,
    B_right_green_PORT,
};

const GPIO_TypeDef* mode_3_PIN[] = {
    C_straight_red_PIN,
    C_left_red_PIN,
    B_right_red_PIN,
    C_straight_orange_PIN,
    C_left_orange_PIN,
    B_right_orange_PIN,
    C_straight_green_PIN,
    C_left_green_PIN,
    B_right_green_PIN,
};

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
        HIGH(segment_a_PORT, segment_a_PIN);
    }
    else {
        LOW(segment_a_PORT, segment_a_PIN);
    }
    if (number & 1<<1) {
        HIGH(segment_b_PORT, segment_b_PIN);
    }
    else {
        LOW(segment_b_PORT, segment_b_PIN);
    }
    if (number & 1<<2) {
        HIGH(segment_c_PORT, segment_c_PIN);
    }
    else {
        LOW(segment_c_PORT, segment_c_PIN);
    }
    if (number & 1<<3) {
        HIGH(segment_d_PORT, segment_d_PIN);
    }
    else {
        LOW(segment_d_PORT, segment_d_PIN);
    }
    if (number & 1<<4) {
        HIGH(segment_e_PORT, segment_e_PIN);
    }
    else {
        LOW(segment_e_PORT, segment_e_PIN);
    }
    if (number & 1<<5) {
        HIGH(segment_f_PORT, segment_f_PIN);
    }
    else {
        LOW(segment_f_PORT, segment_f_PIN);
    }
    if (number & 1<<6) {
        HIGH(segment_g_PORT, segment_g_PIN);
    }
    else {
        LOW(segment_g_PORT, segment_g_PIN);
    }
}

void crossing_activated(void) {
    HIGH(crossing_lights_red_PORT, crossing_lights_red_PIN);
    LOW(crossing_lights_green_PORT, crossing_lights_green_PIN);
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
    LOW(crossing_lights_red_PORT, crossing_lights_red_PIN);
    HIGH(crossing_lights_green_PORT, crossing_lights_green_PIN);
}

void turn_on(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3) {
    LOW(port1, pin1);
    LOW(port2, pin2);
    LOW(port3, pin3);
}

void turn_off(GPIO_TypeDef* port1, uint8_t pin1, GPIO_TypeDef* port2, uint8_t pin2, GPIO_TypeDef* port3, uint8_t pin3) {
    HIGH(port1, pin1);
    HIGH(port2, pin2);
    HIGH(port3, pin3);
}

int main(void) {
    init();

    uint32_t time = 0;
    bool stisk = 0;
    uint8_t crossroad_mode = 1;
    uint32_t crossroad_mode_time = 0;
    bool crossing_request = 0;
    uint32_t sound_time = 0;
    bool sound_on = 0;
    uint32_t switching_sound_time = 0;

    while (1) {
        if(milis() - switching_sound_time > 667) {
            if(sound_on == 0) {
                sound_on = 1;
            }
            else{
                sound_on = 0;
            }
            switching_sound_time = milis();
        }
        if((milis() - sound_time > 1) & sound_on) {
                sound_time = milis();
                REVERSE(speaker_PORT, speaker_PIN);
            }
        if (milis() - time > 100) {
            time = milis();
            if (PUSH(button_PORT, button_PIN)) {
                stisk = 1;
            } else {
                if (stisk == 1) {
                    crossing_request = 1;
                    stisk = 0;
                }
            }
            if (crossroad_mode == 1) {
                if(crossing_request == 1) {
                    crossing_activated();
                    crossing_request = 0;
                }
            }
        }
    }
}


/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
