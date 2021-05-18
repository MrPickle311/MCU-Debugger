/*
 * menu.c
 *
 * Created: 15/05/2021 16:38:07
 *  Author: Damian Wójcik
 */ 

#include "menu.h"
#include "../drivers/core/timer.h"

volatile uint8_t start_flag = 0;
volatile uint8_t waiting_for_choice = 1;
volatile uint8_t current_breakpoint_nmbr = 0;

void MENU_Browser_printHeaderBody()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*3,0,"Breakpoint:",&Font16,BLUE,WHITE);
	Paint_DrawNum(16*10,0,current_breakpoint_nmbr,&Font16,BLUE,WHITE);
	Paint_DrawLine(0,16,LCD_WIDTH,16,BLUE,2,LINE_STYLE_SOLID);
}

void MENU_Browser_printBottomBody()
{
	Paint_DrawLine(0,LCD_HEIGHT - 32, LCD_WIDTH, LCD_HEIGHT - 32, WHITE,2,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
	Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
}

void MENU_Browser_printDataCell(uint8_t start_line)
{
	++start_line;
	Paint_DrawLine(     0,16*start_line*3      -8,LCD_WIDTH,16*start_line*3 -8,BLUE,2,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,16*start_line*3      -8, "Name:",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(0,16*start_line*3 + 16 -8, "Address:",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(0,16*start_line*3 + 32 -8, "Value:",&Font16,BLACK,WHITE);
}

void MENU_printBrowser_Menu()
{
	Paint_Clear(BLACK);
	MENU_Browser_printHeaderBody();
	for_N(i,5)
		MENU_Browser_printDataCell(i);
	MENU_Browser_printBottomBody();
}

void MENU_Browser_nextBreakpoint()
{
	TIMER_haltAsynchronousTimer();
	
	TIMER_releaseAsynchronousTimer();
}

void MENU_Browser_nextData()
{
	TIMER_haltAsynchronousTimer();
	
	TIMER_releaseAsynchronousTimer();
}

void MENU_browsingData()
{
	MENU_printBrowser_Menu();
	volatile uint8_t curr_pos = 0;
	while(1)
	{
		if(button_down(RIGHT_BUTTON_MASK))
		{
			if(curr_pos != 0)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos != 1)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,RED,WHITE);
				++curr_pos;
			}
		}
		else if(button_down(OK_BUTTON_MASK))
		{
			switch(curr_pos)
			{
				case 0:
					MENU_Browser_nextData();
					break;
				case 1:
					MENU_Browser_nextBreakpoint();
					break;
			}
		}
		else if(button_down(BACK_BUTTON_MASK))
		{
			MENU_printChoiceMenu();
			break;
		}
	}
}

void MENU_printUpdatingDataMessage()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*1,16*11,"Updating data...",&Font16,BLACK,WHITE);
}

void MENU_loadNextBreakpoint()
{
	DISABLE_BUTTONS();
	MENU_printUpdatingDataMessage();
	COM_sendDebugSignal();
	COM_commandProcessor();//musze siê switchowaæ
	goToNextSaveBreakpoint();
	MENU_printChoiceMenu();
	ENABLE_BUTTONS();
}

void MENU_serviceChoiceMenu()
{
	uint8_t curr_pos = 0;
	while(1)
	{
		if(button_down(RIGHT_BUTTON_MASK))
		{
			if(curr_pos != 0)
			{
				Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos != 1)
			{
				Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,RED,WHITE);
				++curr_pos;
			}
		}
		else if(button_down(OK_BUTTON_MASK))
		{
			switch(curr_pos)
			{
				case 0:
					MENU_browsingData();
					break;
				case 1:
					MENU_loadNextBreakpoint();
					break;
			}
		}
	}
}

void MENU_printChoiceMenu()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
	Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
	Paint_DrawRectangle(16*1 - 5, 16*10 - 5, 16*6+16*9, 16*10 + 16*2,WHITE,2,DRAW_FILL_EMPTY);
}

void MENU_printStartMenu()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*6,16*10,"Start",&Font16,RED,WHITE);
	Paint_DrawRectangle(16*6 - 5,16*10 - 5,16*6+16*4,16*10 + 16,WHITE,2,DRAW_FILL_EMPTY);
}

void MENU_serviceStartMenu()
{
	ENABLE_BUTTONS();
	while(!button_down(OK_BUTTON_MASK));
	DISABLE_BUTTONS();
	COM_sendDebugSignal();//receiving data
	COM_commandProcessor();// i have guarannce that data receiving will end until next line
	ENABLE_BUTTONS();
}


void MENU_navigate()
{
	if(start_flag == 0)
	{
		MENU_printStartMenu();
		MENU_serviceStartMenu();
		start_flag = 1;
	}
	else 
	{
		MENU_printChoiceMenu();
		MENU_serviceChoiceMenu();
	}
}
