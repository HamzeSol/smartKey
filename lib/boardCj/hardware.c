
#include "stm8s.h"
#include "hardware.h"
#include "mcuPinOut.h"
#include "cFunc.h"
char u1RxBuff;
char u1TxBuff;

static void CLK_Config(void);
static void UART_Config(void);
static void TIM4_Config(void);

#define U1_RX_BUFF_SIZE 60
#define U1_TX_BUFF_SIZE 20

#define CODE_SIZE 16

unsigned char tagCodeAr[CODE_SIZE];
unsigned char codeGetCnt = 0;
unsigned char data;

void HwInit(void)
{
  u1RxBuff = RingBuffer8Create(U1_RX_BUFF_SIZE);
  u1TxBuff = RingBuffer8Create(U1_TX_BUFF_SIZE);

  /* Initialize I/Os in Output Mode */
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SW_GPIO_PORT, (GPIO_Pin_TypeDef)SW_GPIO_PINS, GPIO_MODE_IN_PU_IT);

  /* CLK configuration -----------------------------------------*/
  CLK_Config();

  /* UART configuration -----------------------------------------*/
  UART_Config();

  /* TIM4 configuration -----------------------------------------*/
  //TIM4_Config();
  enableInterrupts();
  //TIM4_Cmd(ENABLE);
  /* Define FLASH programming time */
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
}

// void Delay(unsigned short nCount)
// {
//   /* Decrement nCount value */
//   int i = 0xFFFF;

//   while (nCount != 0)
//   {
//     nCount--;
//     i = 0xFF;
//     while (i)
//     {
//       i--;
//     }
//   }
// }

void EEpromWriteByte(uint32_t _address, uint8_t _data)
{
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte((_address), _data);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
}

uint8_t EEpromReadByte(uint32_t _address)
{
  uint8_t data;
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  data = FLASH_ReadByte(_address);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  return data;
}

void SetPcbPC3(char _val)
{
  if (_val)
    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  else
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}

char GetPcbPD4(void)
{
  return GPIO_ReadInputPin(SW_GPIO_PORT, (GPIO_Pin_TypeDef)SW_GPIO_PINS);
}

void SendUart1(unsigned char *_ar, unsigned char _size)
{
  int i = 0;
  while (i < _size)
  {
    RingBuffer8Write(u1TxBuff, _ar[i]);
    i++;
  }
  UART1_ITConfig(UART1_IT_TXE, ENABLE);
}

char GetUart1(unsigned char *_ar)
{

  char digit = 0;
  char i = 0;
  unsigned char checkXor = 0;

  if (RingBuffer8Read(u1RxBuff, &digit))
  {
    switch (digit)
    {
    case 2: // we are sure that number 2 just received in header, not in data. other wise change the algorithem
      codeGetCnt = 1;
      tagCodeAr[0] = 2;

      break;
    case 3:
      tagCodeAr[codeGetCnt] = 3;

      // UART1_SendData8(codeAr[0]);
      if ((tagCodeAr[0] == 2) && (codeGetCnt == 13))
      {
        for (i = 0; i <= (codeGetCnt); i++)
        {
          _ar[i] = tagCodeAr[i];
        }
        codeGetCnt = 0;
        return 1;
      }
      else
      {
        codeGetCnt = 0;
        return 0;
      }
      break;
    default:
      tagCodeAr[codeGetCnt] = digit;
      codeGetCnt++;
      if (codeGetCnt >= CODE_SIZE)
      {
        codeGetCnt = 0;
      }
      return 0;
      break;
    }
  }
  else
  {
    return 0;
  }
  return 0;
}

void Uart1TXIRQcallback()
{

  if (RingBuffer8Read(u1TxBuff, &data))
  {
    UART1_SendData8(data);
  }
  else
  {
    UART1_ITConfig(UART1_IT_TXE, DISABLE);
  }
}

void Uart1RXIRQcallback(unsigned char _val)
{
  RingBuffer8Write(u1RxBuff, _val);
}

/**
 * @brief  Configure system clock to run at 16Mhz
 * @param  None
 * @retval None
 */
static void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
 * @brief  UART1 and UART3 Configuration for interrupt communication
 * @param  None
 * @retval None
 */
static void UART_Config(void)
{
  /* Deinitializes the UART1 and UART3 peripheral */
  UART1_DeInit();
  // UART3_DeInit();
  /* UART1 and UART3 configuration -------------------------------------------------*/
  /* UART1 and UART3 configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - UART1 Clock disabled
   */
  /* Configure the UART1 */
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

  /* Enable UART1 Transmit interrupt*/
  UART1_ITConfig(UART1_IT_RXNE, ENABLE);
  // UART1_ITConfig(UART1_IT_TXE, ENABLE);

  /* Enable general interrupts */
}

#define TIM4_PERIOD 124
/**
 * @brief  Configure TIM4 to generate an update interrupt each 1ms
 * @param  None
 * @retval None
 */
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  // disableInterrupts();
  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);

  /* enable interrupts */
  // enableInterrupts();

  /* Enable TIM4 */
}

__IO uint32_t delayCounter = 50;
;
// volatile unsigned int delayCounter=10;
void Delay(__IO uint32_t _val)
{
  delayCounter = _val;

  while (delayCounter > 0)
    ;
}
char ledStatus = 0;
unsigned int ledStatusCounter = 0;
void LedStatus(char _val)
{
  ledStatus = _val;
  ledStatusCounter = 0;
}


void TIM4_callBackIrq(void)
{

  switch (ledStatus)
  {
  case LED_OFF:

    GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    break;
  case LED_ON:
    GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    break;
  case LED_OK:
    if (ledStatusCounter > 1)
      GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else if (ledStatusCounter > 0)
    {
      GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
      ledStatus = 0;
    }
    else
      ledStatusCounter = 1500;
  case LED_ERROR:
    if (ledStatusCounter > 1)
      GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else if (ledStatusCounter > 0)
    {
      GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
      ledStatus = 0;
    }
    else
      ledStatusCounter = 60;

    break;
  case LED_REG_USER:
    if (ledStatusCounter > 200)
      GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else if (ledStatusCounter > 0)
      GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else
      ledStatusCounter = 400;
    break;
  case LED_REG_MASTER:
    if (ledStatusCounter > 200)
      GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else if (ledStatusCounter > 0)
      GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
    else
      ledStatusCounter = 1000;
    break;

  // case 10:
  //   if (ledStatusCounter > 200)
  //     GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  //   else if (ledStatusCounter > 100)
  //     GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  //   else if (ledStatusCounter > 1)
  //     GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  //   else if (ledStatusCounter > 0)
  //   {
  //     GPIO_WriteHigh(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  //     ledStatus = 0;
  //   }
  //   else
  //     ledStatusCounter = 300;

    break;

  default:
    break;
  }

  if (ledStatusCounter)
    ledStatusCounter--;

  if (delayCounter > 0)
  {
    delayCounter--;
  }

  // if (delayCounter > 0)
  // {
  //   delayCounter--;
  // }
  // else
  // {
  //   delayCounter = 1000;
  //   GPIO_WriteReverse(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
  // }
  //

  // GPIO_WriteLow(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PINS);
}
