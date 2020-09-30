#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "board.h"
#include "relay.h"
#include "lift.h"

#ifdef TEST_GUI
#include "gtk/gtk.h"

extern GtkWidget *lift_dir_label;
extern GtkWidget *lift_motor_label;
#endif

void relay_init()
{
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 2, 4);		//DOUT0	P4_4	PIN9	GPIO2[4]  MAIN_PWR
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 2, 5);		//DOUT1 P4_5	PIN10	GPIO2[5]  LIFT_PWR
//	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 2, 6);		//DOUT2 P4_6 	PIN11	GPIO2[6]  LIFT_DIR
}

void relay_lift_dir(enum lift_direction dir)
{
	if (dir == LIFT_DIRECTION_UP) {
//		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 2, 4);
		gui_lift_dir_handler(dir);
#ifdef TEST_GUI
#endif
	} else {
//		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 2, 4);
		gui_lift_dir_handler(dir);
#ifdef TEST_GUI
#endif
	}
}

void relay_lift_pwr(bool state)
{
	if (state) {
//		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 2, 5);
#ifdef TEST_GUI
		gui_lift_pwr_handler(state);
#endif
	} else {
//		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 2, 5);
#ifdef TEST_GUI
		gui_lift_pwr_handler(state);
#endif
	}
}

void relay_main_pwr(bool state)
{
//	if (state) {
//		Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 2, 6);
//	} else {
//		Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 2, 6);
//	}
}

