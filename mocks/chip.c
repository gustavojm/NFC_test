#include "stdbool.h"
#include "chip.h"

void Chip_UART_Init(__attribute__((unused))  LPC_USART_T *pUART)
{
	return;
}

uint32_t Chip_UART_SetBaudFDR(__attribute__((unused))  LPC_USART_T *pUART,
		__attribute__((unused))  uint32_t baud)
{
	return 1;
}

void Chip_TIMER_Init(__attribute__((unused))  LPC_TIMER_T *pTMR)
{
	return;
}

void Chip_TIMER_Reset(__attribute__((unused))  LPC_TIMER_T *pTMR)
{
	return;
}

void Chip_SSP_Init(__attribute__((unused))  LPC_SSP_T *pSSP)
{
	return;
}

void Chip_SSP_SetMaster(__attribute__((unused))  LPC_SSP_T *pSSP,
		__attribute__((unused)) bool master)
{
	return;
}

uint32_t Chip_SSP_RWFrames_Blocking(__attribute__((unused))  LPC_SSP_T *pSSP,
		__attribute__((unused))  Chip_SSP_DATA_SETUP_T *xf_setup)
{
	return 1;
}

uint32_t Chip_Clock_GetRate(__attribute__((unused))  CHIP_CCU_CLK_T clk)
{
	return 1;
}
