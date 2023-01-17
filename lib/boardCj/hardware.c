
#include "stm8s.h"
#include "hardware.h"
#include "mcuPinOut.h"
#include "cFunc.h"
char u1RxBuff;
char u1TxBuff;

static void CLK_Config(void);
static void UART_Config(void);

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
}


void Delay(unsigned short nCount)
{
  /* Decrement nCount value */
  int i = 0xFFFF;

  while (nCount != 0)
  {
    nCount--;
    i = 0xFF;
    while (i)
    {
      i--;
    }
  }
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
  enableInterrupts();
}
