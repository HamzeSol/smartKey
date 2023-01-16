
#include "stm8s.h"
#include "board.h"
#include "mcuPinOut.h"

void hwInit(void)
{
  /* Initialize I/Os in Output Mode */
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SW_GPIO_PORT, (GPIO_Pin_TypeDef)SW_GPIO_PINS, GPIO_MODE_IN_PU_IT);

}



void set_PCB_PC3(char _val)
{
  if(_val)
    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  else
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}

char get_PCB_PD4(void)
{
  return GPIO_ReadInputPin(SW_GPIO_PORT, (GPIO_Pin_TypeDef)SW_GPIO_PINS);
}
