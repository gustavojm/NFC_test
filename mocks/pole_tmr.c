#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "dout.h"
#include "pole.h"
#include "debug.h"

#define POLE_SUPERVISOR_RATE    2		//2 calls to interrupt generates one pulse = one step

TimerHandle_t pole_tmr;
extern SemaphoreHandle_t pole_supervisor_semaphore;

static void pole_tmr_callback(TimerHandle_t tmr_handle)
{
	static uint32_t steps = 0;
	static bool On = false;
	BaseType_t xHigherPriorityTaskWoken;
	static enum mot_pap_direction last_dir = MOT_PAP_DIRECTION_CW;

//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);

		if (pole_get_status().dir != last_dir) {
			HERE;
			steps = 0;
			last_dir = pole_get_status().dir;
		}

	On = (bool) !On;
	// Generate waveform
	dout_pole_pulse(On);

	if (++steps == MOT_PAP_SUPERVISOR_RATE) {
		steps = 0;
		xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(pole_supervisor_semaphore,
				&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void pole_tmr_init(void)
{
	pole_tmr = xTimerCreate("TimerPole", pdMS_TO_TICKS(1), pdTRUE, NULL,
			pole_tmr_callback);
}

int32_t pole_tmr_set_freq(uint32_t tick_rate_hz)
{
	int32_t period = (int32_t) ((1.f / tick_rate_hz) * 4000000);
	if ( xTimerChangePeriod( pole_tmr, pdMS_TO_TICKS(period), 100 ) == pdPASS) {
		return 0;
	}
	return 1;
}

void pole_tmr_start(void)
{
	if ( xTimerStart( pole_tmr, 0 ) != pdPASS) {
		printf("pole: unable to start timer \n");
	}
}

void pole_tmr_stop(void)
{
	if (pole_tmr != NULL) {
		xTimerStop(pole_tmr, 0);
	}
}

uint32_t pole_tmr_started(void)
{
	if (xTimerIsTimerActive(pole_tmr) != pdFALSE) {
		/* xTimer is active, do something. */
	}
	return 1;
}
