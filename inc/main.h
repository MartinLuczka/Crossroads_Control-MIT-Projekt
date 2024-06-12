#ifndef _MAIN_H_
#define _MAIN_H_ 1

#define LOW(PORT, PIN) GPIO_WriteLow(PORT, PIN)
#define HIGH(PORT, PIN) GPIO_WriteHigh(PORT, PIN)
#define REVERSE(PORT, PIN) GPIO_WriteReverse(PORT, PIN)

#define READ(PORT, PIN) (GPIO_ReadInputPin(PORT, PIN)!=RESET) 
#define PUSH(PORT, PIN) (GPIO_ReadInputPin(PORT, PIN)==RESET) 

// Discovery Board
#ifdef STM8S003
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#define BTN_PORT GPIOB
#define BTN_PIN  GPIO_PIN_7
#endif

// Dero Board
#ifdef STM8S103
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_4
#endif

// Discovery Board
#ifdef STM8S105
#define LED_PORT GPIOD
#define LED_PIN  GPIO_PIN_0
#endif

// Nucleo Kit
#ifdef STM8S208
#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#endif

// functions
void init(void);

#endif // !_MAIN_H_
