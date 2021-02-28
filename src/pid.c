#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "pid.h"
#include "debug.h"

/**
 * @brief	constrains the out value to limit
 * @param 	out
 * @param 	limit
 * @return	out value
 * @return	limit if out > limit
 * @return	-limit if out < limit
 */
static double abs_limit(const double out, const int32_t limit)
{
	if (out > (double) limit)
		return (double) limit;
	if (out < (double) -limit)
		return (double) -limit;
	return out;
}

/**
 * @brief				: initializes the PID structure.
 * @param 	me			: pointer to struct pid
 * @param 	kp			: proportional constant
 * @param 	sample_time	: sample time
 * @param 	ti			: integrative time
 * @param 	td			: derivative time
 * @param 	limit		: output limiter value
 */
void pid_controller_init(struct pid *me, double kp, double ki, double kd,
		int32_t sample_time, int32_t min_limit, int32_t max_limit)
{
	me->kp = kp;
	me->ki = ki;
	me->kd = kd;
	me->sample_time_in_ticks = pdMS_TO_TICKS(sample_time);
	me->min_limit = min_limit;
	me->max_limit = max_limit;
	me->setpoint = 0;
	me->sloped_setpoint = 0;
	me->integral_term = 0;
}

/**
 * @brief	calculates the incremental PID algorithm.
 * @param 	me			: pointer to struct pid
 * @param 	setpoint	: the position to reach
 * @param 	input		: the current position
 * @return	the calculated PID output or the specified limits for output
 * @note 	if this function was called before the sample time has elapsed will return the
 * 			previously calculated value, unless a setpoint change is detected.
 */
int32_t pid_controller_calculate(struct pid *me, int32_t setpoint,
		int32_t input)
{
	double error;
	TickType_t now = xTaskGetTickCount();
	TickType_t elapsed = now - me->last_time_in_ticks;

	if (setpoint != me->setpoint) {
		// Slope calculation
		me->slope = MAX(abs(setpoint - input), 1) / 100;
		me->setpoint = setpoint;
		me->sloped_setpoint = input;				// Start increasing setpoint from current input value
	}

	if ((me->sloped_setpoint != setpoint) | (elapsed > me->sample_time_in_ticks)) {
		lDebug(Info, "///// SLOPE: %i", me->slope);

		if (me->slope != 0) {
			if (setpoint > me->sloped_setpoint) {
				me->sloped_setpoint += me->slope;
				if (me->sloped_setpoint > setpoint) {
					me->sloped_setpoint = setpoint;
					me->slope = 0;
				}
			} else {
				me->sloped_setpoint -= me->slope;
				if (me->sloped_setpoint < setpoint) {
					me->sloped_setpoint = setpoint;
					me->slope = 0;
				}
			}
		}
//			me->setpoint = setpoint;
		me->integral_term = 0;

		lDebug(Info, "///// SETPOINT: %i", me->sloped_setpoint);

		// Compute error and normalize it to 100%
		error = abs(me->sloped_setpoint - input) * (double) 100 / 65536;
		lDebug(Info, "///// ERROR: %f", error);

		// Compute integral term
		me->integral_term += me->ki * error;

		// Compute differential on input
		double dinput = (input - me->last_input) * (double) 100 / 65535;
		lDebug(Info, "///// DINPUT: %f", dinput);

		// Compute PID output
		me->output = me->kp * error + me->integral_term - me->kd * dinput;

		lDebug(Info, "///// UNLIMITED: %f", me->output);

		// Anti wind-up
		if (me->output > me->max_limit) {
			me->integral_term -= (me->output - me->max_limit);
			me->output = me->max_limit;
		} else if (me->output < me->min_limit) {
			me->integral_term += (me->min_limit - me->output);
			me->output = me->min_limit;
		}

		//me->output = abs_limit(me->output, 100);

		me->last_input = input;
		me->last_time_in_ticks = now;

		lDebug(Info, "///// proporcional: %f", me->kp * error);
		lDebug(Info, "///// integrativo: %f", me->integral_term);
		lDebug(Info, "///// derivativo: %f", me->kd * dinput);
	}
	return (int32_t) floor(me->output);
}
