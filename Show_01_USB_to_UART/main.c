//--------------------------------------------------------------
// File     : main.c
// Datum    : 16.06.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : USB to UART Bridge
//              Die CPU kann per USB-OTG-Buchse
//              an einen PC angeschlossen werden
//              (Virtueller COM-Port)
//              Alle eingehenden Daten vom PC werden an einer
//              UART der CPU ausgegeben und alle eingehenden
//              Daten an der UART werden an den PC gesendet
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//
// Librarys : Die eingesetzen Library fьr USB-CDC und UART
//            sind abgeдnderte Versionen und tragen daher
//            eine Alpha-Versions-Nr (V:0.1)
//
// Portpins :
//              PA8   -> USB_OTG_SOF (wird nicht benutzt)
//              PA9   -> USB_OTG_VBUS
//              PA10  -> USB_OTG_ID
//              PA11  -> USB_OTG_DM
//              PA12  -> USB_OTG_DP
//
//              PC10  -> UART_TX  [115200 Baud, 8N1]
//              PC11  -> UART_RX
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_usb_cdc.h"
//#include "stm32_ub_uart.h"
#include <string.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
//#include <stdlib.h>

void delay_ms(uint32_t ms)
{
volatile uint32_t nCount; //переменная для счета
RCC_ClocksTypeDef RCC_Clocks; //переменная для считывания текущей частоты
RCC_GetClocksFreq (&RCC_Clocks); //считываем текущую тактовую частоту

nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms; //пересчитываем мс в циклы
for (; nCount!=0; nCount--); //гоняем пустые циклы
}


int main(void)
{
  USB_CDC_STATUS_t check_usb;

  //uint16_t rx_uart;
  char buf[64];
  char* string_buf = malloc(20);
  uint16_t rx_usb = strlen(string_buf);
  SystemInit(); // Quarz Einstellungen aktivieren

  // init vom CDC
  UB_USB_CDC_Init();



  	GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);



  // init vom UART
  //UB_Uart_Init();

  while(1)
  {
	  // test ob USB mit PC verbunden ist
	  check_usb=UB_USB_CDC_GetStatus();
	  if(check_usb==USB_CDC_CONNECTED) {
		  int x=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		  string_buf = (x==1)?(char*)"BUTTON PRESSED\r\n":(char*)"BUTTON UNPRESSED\r\n";

		   UB_USB_CDC_SendData(string_buf,strlen(string_buf));
		   delay_ms(1000);




	}
  }
}

