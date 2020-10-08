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
	TIMER0_IRQHandler();
}

void tmr_init(struct tmr *me)
{
	pole_tmr = xTimerCreate("TimerPole", pdMS_TO_TICKS(1), pdTRUE, NULL,
			pole_tmr_callback);
}

int32_t tmr_set_freq(struct tmr *me, uint32_t tick_rate_hz)
{
	int32_t period = (int32_t) ((1.f / tick_rate_hz) * 4000000);
	if ( xTimerChangePeriod( pole_tmr, pdMS_TO_TICKS(period), 100 ) == pdPASS) {
		return 0;
	}
	return 1;
}

void tmr_start(struct tmr *me)
{
	if ( xTimerStart( pole_tmr, 0 ) != pdPASS) {
		printf("pole: unable to start timer \n");
	}
}

void tmr_stop(struct tmr *me)
{
	if (pole_tmr != NULL) {
		xTimerStop(pole_tmr, 0);
	}
}

uint32_t tmr_started(struct tmr *me)
{
	if (xTimerIsTimerActive(pole_tmr) != pdFALSE) {
		/* xTimer is active, do something. */
		return 1;
	}
	return 1;
}

bool tmr_match_pending(struct tmr *me)
{
//	bool ret = false;
//	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1)) {
//		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
//		ret = true;
//	}
	return true;
}

