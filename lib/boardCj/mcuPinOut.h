/**
 ******************************************************************************
 * @file    mcuPinOut.h
 * @author  h.soleimanipour@gmail.com
 * @version V1
 * @date    30-Jan-2023
 * @brief   Define microcontroller pins for our purposes
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Public define -------------------------------------------------------------*/
#define LED_GPIO_PORT (GPIOC)
#define LED_GPIO_PINS (GPIO_PIN_3)

#define BUZ_GPIO_PORT (GPIOD)
#define BUZ_GPIO_PINS (GPIO_PIN_3)

#define RELAY_GPIO_PORT (GPIOD)
#define RELAY_GPIO_PINS (GPIO_PIN_2)

#define SW_GPIO_PORT (GPIOD)
#define SW_GPIO_PINS (GPIO_PIN_4)