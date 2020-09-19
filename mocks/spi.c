#include "board.h"
#include "stdio.h"
#include "spi.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

//#define LPC_SSP           LPC_SSP1
#define SSP_IRQ           SSP1_IRQn
#define SSPIRQHANDLER SSP1_IRQHandler

#define BUFFER_SIZE                         (0x100)
#define SSP_DATA_BITS                       (SSP_BITS_8)

int32_t spi_write(uint8_t *data, __attribute__((unused))   int32_t byte_count)
{
	printf("0x%x \n", (uint8_t) *data);
	return 1;
}
//
//int32_t spi_sync_transfer(struct spi_transfer *xfer,
//		__attribute__((unused))    int32_t byte_count) {
//	printf("0x%s \n", (char*) xfer);
//	return 1;
//}

int32_t spi_sync_transfer(Chip_SSP_DATA_SETUP_T *xfers, uint32_t num_xfers,
		void (*gpio_wr_fsync)(bool))
{
	uint32_t i;

	for (i = 0; i < num_xfers; ++i) {
		if (gpio_wr_fsync != NULL) {
//			gpio_wr_fsync(0);
		}
//		Chip_SSP_RWFrames_Blocking(LPC_SSP, &(xfers[i]));

		if (gpio_wr_fsync != NULL) {
//			gpio_wr_fsync(1);
		}

		if (i != num_xfers) {
			/* Delay WR/FSYNC falling edge to SCLK rising edge 3 ns min
			 Delay WR/FSYNC falling edge to SDO release from high-Z
			 VDRIVE = 4.5 V to 5.25 V 16 ns min */
//			__NOP();	// 83ns delay at 12MHz
		}
	}
	return 0;
}

int32_t spi_init(void)
{
	/* SSP initialization */
//	Board_SSP_Init(LPC_SSP);

//	Chip_SSP_Init(LPC_SSP);

//	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
//	ssp_format.bits = SSP_DATA_BITS;
//	ssp_format.clockMode = SSP_CLOCK_MODE3;
//	Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat,
//			ssp_format.clockMode);
//	Chip_SSP_Enable(LPC_SSP);

//	Chip_SSP_SetMaster(LPC_SSP, 1);

}
