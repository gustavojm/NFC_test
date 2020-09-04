#include "board.h"
#include "pid.h"
#include <stddef.h>
#include "FreeRTOS.h"
#include "task.h"
#include "math.h"

int32_t abs_limit(const int32_t out, const int32_t limit)
{
	if (out > limit)
		return limit;
	if (out < -limit)
		return -limit;
	return out;
}

void pid_controller_init(struct pid *pid, double kp, double ki, double kd,
		int32_t sample_time_in_ms, int32_t limit, int32_t rate)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->sample_time_in_ticks = pdMS_TO_TICKS(sample_time_in_ms);
	pid->limit = limit;
	pid->rate = rate;
	pid->errors[0] = 0;
	pid->errors[1] = 0;
	pid->errors[2] = 0;
	pid->input = 0;
	pid->setpoint = 0;
	pid->prop_out = 0;
	pid->int_out = 0;
	pid->der_out = 0;
	pid->output = 0;
	pid->last_time_in_ticks = xTaskGetTickCount();
}

static int32_t rate_limit(struct pid *pid, int32_t setpoint)
{
	// Implementar rate limiter para evitar arranques a la máxima velocidad del motor;
	// generar una rampa desde pid->input hasta setpoint
	int32_t out, limited;
	if (pid->rate == 0) {
		out = setpoint;
	} else {
		if (setpoint > pid->setpoint) {
			limited = pid->setpoint + (pid->rate * pid->sample_time_in_ticks / 1000);
			out = MIN(limited, setpoint);
		} else {
			limited = pid->setpoint - (pid->rate * pid->sample_time_in_ticks / 1000);
			out = MAX(limited, setpoint);
		}
	}

	return out;
}

int32_t pid_controller_calculate(struct pid *pid, int32_t setpoint,
		int32_t input)
{
	TickType_t current_ticks = xTaskGetTickCount();

	if ((current_ticks - pid->last_time_in_ticks) >= pid->sample_time_in_ticks) {
		pid->input = input;
		if (pid->setpoint != setpoint) {
			pid->setpoint = rate_limit(pid, setpoint);
		}
		pid->errors[0] = pid->setpoint - pid->input;
		pid->prop_out = pid->kp * (pid->errors[0] - pid->errors[1]);
		pid->int_out = pid->ki * pdTICKS_TO_MS(pid->sample_time_in_ticks) * pid->errors[0];
		pid->der_out = (pid->kd / pdTICKS_TO_MS(pid->sample_time_in_ticks))
				* (pid->errors[0] - 2 * pid->errors[1] + pid->errors[2]);
		pid->output += (pid->prop_out + pid->int_out + pid->der_out);

		pid->errors[2] = pid->errors[1];
		pid->errors[1] = pid->errors[0];
		pid->last_time_in_ticks = current_ticks;
	}

	return abs_limit((int32_t) floorl(pid->output), pid->limit);
}
