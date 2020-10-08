#include <stdint.h>
#include <errno.h>

#include "board.h"
#include "ad2s1210.h"
#include "debug.h"
#include "gui.h"

//xf_setup.length = BUFFER_SIZE;
//xf_setup.tx_data = Tx_Buf;
//xf_setup.rx_data = Rx_Buf;
//
//xf_setup.rx_cnt = xf_setup.tx_cnt = 0;

const uint32_t ad2s1210_resolution_value[] = { 10, 12, 14, 16 };

int32_t spi_write(uint8_t *data, __attribute__((unused))      int32_t byte_count);
int32_t spi_sync_transfer(Chip_SSP_DATA_SETUP_T *xfers,
		__attribute__((unused))      int32_t byte_count);

/* write 1 bytes (address or data) to the chip */
int32_t ad2s1210_config_write(struct ad2s1210 *me, uint8_t data)
{
	int32_t ret = 0;
//
//	me->tx[0] = data;
//	ret = spi_write(me->tx, 1);
//
	return ret;
}

/* read value from one of the registers */
int32_t ad2s1210_config_read(struct ad2s1210 *me, uint8_t address)
{
//	struct spi_transfer xfers[] = { { .xf_setup = { .length = 1, .rx_data =
//			&me->rx[0], .tx_data = &me->tx[0], .rx_cnt = 0, .tx_cnt = 0 },
//			.cs_change = 1, }, { .xf_setup = { .length = 1, .rx_data =
//			&me->rx[1], .tx_data = &me->tx[1], .rx_cnt = 0, .tx_cnt = 0 }, }, };
//
//	int32_t ret = 0;
//
//	me->tx[0] = address | AD2S1210_MSB_IS_HIGH;
//	me->tx[1] = AD2S1210_REG_FAULT;
//	ret = spi_sync_transfer(xfers, 2);
//	if (ret < 0)
//		return ret;
//
//	return me->rx[1];
	return 0;
}

int32_t ad2s1210_update_frequency_control_word(struct ad2s1210 *me)
{
//	int32_t ret = 0;
//	uint8_t fcw;
//
//	fcw = (uint8_t) (me->fexcit * (1 << 15) / me->fclkin);
//	if (fcw < AD2S1210_MIN_FCW || fcw > AD2S1210_MAX_FCW) {
//		lDebug(Error, "ad2s1210: FCW out of range");
//		return -ERANGE;
//	}
//
//	ret = ad2s1210_config_write(me, AD2S1210_REG_EXCIT_FREQ);
//	if (ret < 0)
//		return ret;
//
//	return ad2s1210_config_write(me, fcw);
	return 0;
}

int32_t ad2s1210_soft_reset(struct ad2s1210 *me)
{
//	int32_t ret;
//
//	ret = ad2s1210_config_write(me, AD2S1210_REG_SOFT_RESET);
//	if (ret < 0)
//		return ret;
//
//	return ad2s1210_config_write(me, 0x0);
	return 0;
}

void ad2s1210_hard_reset(struct ad2s1210 *me)
{
//	me->gpios.reset(0);
//	vTaskDelay(pdMS_TO_TICKS(1));
//	me->gpios.reset(1);
}

uint32_t ad2s1210_get_fclkin(struct ad2s1210 *me)
{

	return me->fclkin;
}

int32_t ad2s1210_set_fclkin(struct ad2s1210 *me, uint32_t fclkin)
{

	int32_t ret = 0;

	if (fclkin < AD2S1210_MIN_CLKIN || fclkin > AD2S1210_MAX_CLKIN) {
		lDebug(Error, "ad2s1210: fclkin out of range");
		return -EINVAL;
	}


//			me->fclkin = fclkin;
//
//			ret = ad2s1210_update_frequency_control_word(me);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_soft_reset(me);
	return ret;
}

uint32_t ad2s1210_get_fexcit(struct ad2s1210 *me)
{

	return me->fexcit;
}

int32_t ad2s1210_set_fexcit(struct ad2s1210 *me, uint32_t fexcit)
{
	int32_t ret = 0;

	if (fexcit < AD2S1210_MIN_EXCIT || fexcit > AD2S1210_MAX_EXCIT) {
		lDebug(Error, "ad2s1210: excitation frequency out of range");
		return -EINVAL;
	}
//			me->fexcit = fexcit;
//			ret = ad2s1210_update_frequency_control_word(me);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_soft_reset(me);
	return ret;
}

int32_t ad2s1210_get_control(struct ad2s1210 *me)
{
	int32_t ret = 0;

//			ret = ad2s1210_config_read(me, AD2S1210_REG_CONTROL);
	return ret;
}

int32_t ad2s1210_set_control(struct ad2s1210 *me, uint8_t udata)
{

//	uint8_t data;
	int32_t ret = 0;

//			ret = ad2s1210_config_write(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//			data = udata & AD2S1210_MSB_IS_LOW;
//			ret = ad2s1210_config_write(me, data);
//			if (ret < 0)
//				return ret;
//
//			ret = ad2s1210_config_read(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//			if (ret & AD2S1210_MSB_IS_HIGH) {
//				ret = -EIO;
//				lDebug(Error, "ad2s1210: write control register fail");
//				return ret;
//			}
//			me->resolution = ad2s1210_resolution_value[data
//					& AD2S1210_SET_RESOLUTION];
//			me->hysteresis = !!(data & AD2S1210_ENABLE_HYSTERESIS);
//
	return ret;
}

uint8_t ad2s1210_get_resolution(struct ad2s1210 *me)
{

	return me->resolution;
}

int32_t ad2s1210_set_resolution(struct ad2s1210 *me, uint8_t udata)
{
//	uint8_t data;
	int32_t ret = 0;

	if (udata < 10 || udata > 16) {
		lDebug(Error, "ad2s1210: resolution out of range");
		return -EINVAL;
	}

//			ret = ad2s1210_config_read(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				goto error_ret;
//			data = ret;
//			data &= ~AD2S1210_SET_RESOLUTION;
//			data |= (udata - 10) >> 1;
//			ret = ad2s1210_config_write(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_config_write(me, data & AD2S1210_MSB_IS_LOW);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_config_read(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//			data = ret;
//			if (data & AD2S1210_MSB_IS_HIGH) {
//				ret = -EIO;
//				lDebug(Error, "ad2s1210: setting resolution fail");
//				goto error_ret;
//			}
//			me->resolution = ad2s1210_resolution_value[data
//					& AD2S1210_SET_RESOLUTION];
	return ret;
}

/* read the fault register since last sample */
int32_t ad2s1210_get_fault(struct ad2s1210 *me)
{
	int32_t ret = 0;

	ret = ad2s1210_config_read(me, AD2S1210_REG_FAULT);
	return ret;
}

int32_t ad2s1210_clear_fault(struct ad2s1210 *me)
{
	int32_t ret = 0;

//			me->gpios.sample(0);
//			/* delay (2 * tck + 20) nano seconds */
//			vTaskDelay(pdMS_TO_TICKS(0.02));
//			me->gpios.sample(1);
//			ret = ad2s1210_config_read(me, AD2S1210_REG_FAULT);
//			if (ret < 0)
//				return ret;
//
//			me->gpios.sample(0);
//			me->gpios.sample(1);
//
	return ret;
}

int32_t ad2s1210_get_reg(struct ad2s1210 *me, uint8_t address)
{
	int32_t ret = 0;

//			ret = ad2s1210_config_read(me, address);
	return ret;
}

int32_t ad2s1210_set_reg(struct ad2s1210 *me, uint8_t address,
		uint8_t data)
{

	int32_t ret = 0;

//			ret = ad2s1210_config_write(me, address);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_config_write(me, data & AD2S1210_MSB_IS_LOW);
	return ret;
}

int32_t ad2s1210_init(struct ad2s1210 *me)
{
//	uint8_t data;
	int32_t ret = 0;

//			ret = ad2s1210_config_write(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//			data = AD2S1210_DEF_CONTROL & ~(AD2S1210_SET_RESOLUTION);
//			data |= (me->resolution - 10) >> 1;
//			ret = ad2s1210_config_write(me, data);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_config_read(me, AD2S1210_REG_CONTROL);
//			if (ret < 0)
//				return ret;
//
//			if (ret & AD2S1210_MSB_IS_HIGH) {
//				ret = -EIO;
//				return ret;
//			}
//
//			ret = ad2s1210_update_frequency_control_word(me);
//			if (ret < 0)
//				return ret;
//			ret = ad2s1210_soft_reset(me);
	return ret;
}

uint16_t ad2s1210_read_position(struct ad2s1210 *me)
{
	uint16_t ret = 0;

			//ret = ad2s1210_config_read(me, AD2S1210_REG_POSITION);
#ifdef TEST_GUI
		ret = gui_pole_cur_pos();
#endif
	return ret;
}

int16_t ad2s1210_read_velocity(struct ad2s1210 *me)
{
	int16_t ret = 0;

//			ret = ad2s1210_config_read(me, AD2S1210_REG_VELOCITY);
	return ret;
}

//Truncar a la resolución configurada
//if (me->hysteresis)
//	pos >>= 16 - me->resolution;
//*val = pos;
//
