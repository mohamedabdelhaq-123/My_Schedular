#include "MemMap.h"
#include "StdTypes.h"
#include "OS.h"
#include "OS_Cfg.h"
#include "OS_Priv.h"
#include "Timers_Int.h"
#include "Tasks_Int.h"

static u8 TaskCounter_OF_CreatedTasks =0;

void Init_Scheduler(void)
{
	TIMER0_Init(TIMER0_NORMAL_MODE,TIMER0_SCALER_8);   
	TIMER0_OC0Mode(OC0_DISCONNECTED);
	TIMER0_OV_InterruptEnable();                 // in ISR the tcnt0=156
	Timer0_OVF_SetCallBack(vIncremental);

	OS_Task_Create(Task_1s_Lcd,  1,BLOCKED);
	OS_Task_Create(Task_2s_Blink,2,BLOCKED);
	OS_Task_Create(Task_3s_Blink,3,BLOCKED);
}

void vIncremental (void)   // comes every 100us
{
	Timer0_Interrupt_counter++;
	if(Timer0_Interrupt_counter>= 10)   // change form us to ms
	{
		Timer0_Interrupt_counter=1;
		Millis_Counter++;
	}
	if (Millis_Counter>=1000)  // change form ms to sec
	{
		Millis_Counter=1;
		OS_Task_Handler();  // it is Tick Time
	}
}

OS_feedback_t OS_Task_Create(fptr func, u8 periodicity, OS_State_t task_state)  // create all tasks
{
	OS_feedback_t feedback = NOK_UNKNOWN;
	if(func==NOK_NUll_PTR)
	{
		feedback=NOK_NUll_PTR;
	}
	else if(periodicity > HYPER_PERIOD)
	{
		feedback=NOK_HYPER_PERIOD;
	}
	else if(TaskCounter_OF_CreatedTasks >= NO_OF_Tasks)
	{
		feedback=NOK_COUNT_LIMIT;
	}
	else
	{
		// create the task
		tasksArray[TaskCounter_OF_CreatedTasks].function=func;
		tasksArray[TaskCounter_OF_CreatedTasks].task_periodicity=periodicity;
		tasksArray[TaskCounter_OF_CreatedTasks].task_state=task_state;
		tasksArray[TaskCounter_OF_CreatedTasks].Task_Counter=Init_Value;
		TaskCounter_OF_CreatedTasks++;
		feedback=OK_feedback;
	}
	return feedback;
}


void OS_Task_Handler(void)  // comes with tick
{
	u8 T;
	for(T=0;T<TaskCounter_OF_CreatedTasks;T++)                                               // loop to check the time limit of every task "counter"
	{
		if(tasksArray[T].task_state != SUSPENDED)
		{
			if(tasksArray[T].Task_Counter >= tasksArray[T].task_periodicity) // if met so blocked to ready
			{                                                                   // ex: T2 in first tick Task_Counter=1 and its task_periodicity=2 & still blocked
				tasksArray[T].task_state = READY;                               // so no but inc its task counter so in next tick they become equal and be ready
				tasksArray[T].Task_Counter=Init_Value;                             // return counter to initial condition
			}
			else                                                             // else so counter ++ and still blocked
			{
				tasksArray[T].Task_Counter++;
			}
		}
		else
		{
			//nothing
		}
	}
}


void OS_Task_Executer(void)
{
	u8 i;
	for(i=0;i<TaskCounter_OF_CreatedTasks;i++)
	{
		if (tasksArray[i].task_state == READY)
		{
			tasksArray[i].function();
			tasksArray[i].task_state = BLOCKED;
		}
	}
}
