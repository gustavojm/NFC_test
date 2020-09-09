#include "stdint.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "stdbool.h"
#include "dout.h"
#include "pole.h"
#include "debug.h"

#define POLE_SUPERVISOR_RATE    10		//every 10 steps call supervisor task

TimerHandle_t pole_tmr;
extern SemaphoreHandle_t pole_supervisor_semaphore;

static void pole_tmr_callback(TimerHandle_t tmr_handle)
{
	uint32_t static steps = 0;
	static bool On = false;
	BaseType_t xHigherPriorityTaskWoken;

//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
	On = (bool) !On;
	// Generate waveform
	dout_pole_pulse(On);

	if (++steps == POLE_SUPERVISOR_RATE) {
		steps = 0;
		xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(pole_supervisor_semaphore,
				&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void pole_tmr_init(void)
{
	HERE;
	pole_tmr = xTimerCreate("TimerPole", pdMS_TO_TICKS(1), pdTRUE, NULL,
			pole_tmr_callback);
	HERE;
}

int32_t pole_tmr_set_freq(int32_t tick_rate_hz)
{
	int32_t period = (int32_t) ((1.f / tick_rate_hz) * 1000000);
	HERE;
	if ( xTimerChangePeriod( pole_tmr, pdMS_TO_TICKS(period), 100 ) == pdPASS) {
		HERE;
		return 0;
	}
	HERE;
	return 1;
}

void pole_tmr_start(void)
{
	HERE;
	if ( xTimerStart( pole_tmr, 0 ) != pdPASS) {
		printf("pole: unable to start timer \n");
	}
	HERE;
}

void pole_tmr_stop(void)
{
	HERE;
	if (pole_tmr != NULL) {
		xTimerStop(pole_tmr, 0);
	}
	HERE;
}

uint32_t pole_tmr_started(void)
{
	HERE;
	if (xTimerIsTimerActive(pole_tmr) != pdFALSE) {
		/* xTimer is active, do something. */
	}
	HERE;
	return 1;
}
