

#ifndef OS_H_
#define OS_H_


typedef void (*fptr)(void);

void Init_Scheduler(void);

typedef enum
{
	BLOCKED,
	SUSPENDED,
	READY
}OS_State_t;

typedef struct
{
	fptr function;           // Func
	u8 task_periodicity;     // periodicity 
	OS_State_t task_state;   // Task State
	u8 Task_Counter;         // each task has its owm counter to check if its time has come or not yet in task_handler
}OS_Struct_t;  // TCB

typedef enum
{
	OK_feedback,
	NOK_UNKNOWN,
	NOK_NUll_PTR,
	NOK_HYPER_PERIOD,
	NOK_COUNT_LIMIT	
}OS_feedback_t;

OS_feedback_t OS_Task_Create(fptr func, u8 periodicity, OS_State_t task_state);
void OS_Task_Handler(void);
void OS_Task_Executer(void);

void vIncremental (void);

#endif /* OS_H_ */