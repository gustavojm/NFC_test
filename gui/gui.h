#include <stdbool.h>

#include "lift.h"
#include "mot_pap.h"

void gui_task(void * args);

void gui_init(void);

void gui_pole_dir_handler(enum mot_pap_direction dir);

void gui_pole_pulse_handler(bool state);

uint16_t gui_pole_cur_pos();

void gui_lift_dir_handler(enum lift_direction dir);

void gui_lift_pwr_handler(bool state);

