#include "MemMap.h"
#include "StdTypes.h"
#include "Tasks_Int.h"
#include "DIO_Int.h"
#include "LCD_Int.h"

static u16 num=1;

void Task_1s_Lcd(void)
{
	LCD_SetCursor(1,1);
	LCD_WriteString("seconds: ");
	LCD_WriteNumber(num);
	num++;
}

void Task_2s_Blink(void)
{
	DIO_TogglePin(PINB6);
}

void Task_3s_Blink(void)
{
	DIO_TogglePin(PINB7);
}