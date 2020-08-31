#include <RTUcomHMI.h>
#include "board.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pole.h"
#include "lift.h"
#include "debug.h"

#define DEBUG
int debugLevel = Error;

int main(void)
{
	pole_init();
//	arm_init();
	lift_init();
	comm_init();

	/* Start the scheduler itself. */
	vTaskStartScheduler();

	return 0;
}

/*-----------------------------------------------------------*/
void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
	//volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	taskENTER_CRITICAL();
	{
		printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
		/* You can step out of this function to debug the assertion by using
		 the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		 value. */
		//while( ulSetToNonZeroInDebuggerToContinue == 0 )
		//{
		//}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/
