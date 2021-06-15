/*
 * menu.c
 *
 * Created: 15/05/2021 16:38:07
 *  Author: Damian Wójcik
 */ 

#include "menu.h"
#include "../drivers/core/timer.h"

volatile uint8_t start_flag = 0;

void MENU_updateBreakpointNumber()
{
	Paint_DrawNum(16*10,0,current_breakpoint_to_load,&Font16,BLUE,WHITE);
}

void MENU_Browser_printHeaderBody()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*3,0,"Breakpoint:",&Font16,BLUE,WHITE);
	MENU_updateBreakpointNumber();
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
	//Paint_DrawLine(     0,16*start_line*3      -27 + (start_line - 1)*2,LCD_WIDTH,16*start_line*3      -27 + (start_line - 1)*2,BLUE,1,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,16*start_line*3      -27 + (start_line - 1)*3, "Name:",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(0,16*start_line*3 + 32 -27 + (start_line - 1)*3, "Value:",&Font16,BLACK,WHITE);
}

volatile char string_buffer[40];
volatile uint8_t sb_idx = 0;
volatile uint32_t temp_value = 0;
volatile uint8_t var_size = 0;
volatile uint8_t helper_var = 0;
volatile char number_str_buffer[10];

void MENU_printData(uint8_t line)
{
	//reset
	sb_idx = 0;
	temp_value =  0;
	string_buffer[0] = 0;
	var_size = 0;
	helper_var = 0;
	number_str_buffer[0] = 0;
	
	FRAM_CircularBuffer_pop(fram_buffer,&var_size);//get var size
	
	for_N(i,var_size)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&helper_var);
		temp_value |= helper_var << ( 8 * i ) ;
	}
	
	FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	while(string_buffer[sb_idx++])
		FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	Paint_DrawString_EN(16*4,16*line*3      -27 + (line - 1)*3, "                  ",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(16*4,16*line*3 + 32 -27 + (line - 1)*3,"                  ",&Font16,BLACK,WHITE);
	
	Paint_DrawString_EN(16*4,16*line*3      -27 + (line - 1)*3, string_buffer,&Font16,BLACK,WHITE);
	
	sprintf(number_str_buffer,"0x%08x", temp_value);
	
	Paint_DrawString_EN(16*4,16*line*3 + 32 -27 + (line - 1)*3 , number_str_buffer,&Font16,BLACK,WHITE);
	//Paint_DrawNum(16*4,16*line*3 + 32 -27 + (line - 1)*3 , temp_value,&Font16,BLACK,WHITE);
}

void MENU_printDataPage()
{
	for_N(i,5)
		MENU_printData(i + 1);
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
	DISABLE_BUTTONS();
	goToNextLoadBreakpoint();
	MENU_updateBreakpointNumber();
	MENU_printDataPage();
	ENABLE_BUTTONS();
}

void MENU_Browser_nextData()
{
	DISABLE_BUTTONS();
	MENU_printDataPage();
	ENABLE_BUTTONS();
}

void MENU_browsingData()
{
	resetLoadBreakpoint();
	MENU_printBrowser_Menu();
	MENU_printDataPage();
	volatile uint8_t curr_pos = 0;
	while(1)
	{
		if(button_down(RIGHT_BUTTON_MASK))
		{
			if(curr_pos == 1)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos == 0)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,RED,WHITE);
				++curr_pos;
			}
		}
		else if(button_down(OK_BUTTON_MASK))
		{
			Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
			Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
			switch(curr_pos)
			{
				case 0:
					MENU_Browser_nextBreakpoint();
					break;
				case 1:
					MENU_Browser_nextData();
					break;
			}
			curr_pos = 0;
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
	resetSaveBreakpoint();
	COM_sendDebugSignal();
	COM_commandProcessor();//data are injected to fram now 
	
	goToNextSaveBreakpoint();//so i can switch this buffer
	
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
			if(curr_pos == 1)
			{
				Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos == 0)
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
			curr_pos = 0;
		}
	}
}

void MENU_printChoiceMenu()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
	Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
	Paint_DrawRectangle(16*1 - 5, 16*10 - 5, 16*6+16*9, 16*10 + 16*2,WHITE,2,DRAW_FILL_EMPTY);
	
	Paint_DrawString_EN(0,16*15,"Breakpoints: ",&Font16,BLACK,WHITE);
	Paint_DrawNum(16*10,16*15,breakpoints_total_nmbr,&Font16,BLACK,WHITE);
}

void MENU_printString_NotSelected(const char* const str, MENU_Point* start_point )
{
	Paint_DrawString_EN(FONT_16_WIDTH * start_point->x_ ,
						FONT_16_HEIGHT * start_point->y_ ,
						str ,
						&Font16 ,
						BLACK ,
						WHITE );
}

void MENU_printString_Selected(const char* const str, MENU_Point* start_point )
{
	Paint_DrawString_EN(FONT_16_WIDTH * start_point->x_ ,
						FONT_16_HEIGHT * start_point->y_ ,
						str ,
						&Font16 ,
						RED ,
						WHITE );
}

void MENU_drawRectangle(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end)
{
	Paint_DrawRectangle(x_start, y_start, x_end , y_end, WHITE, 2, DRAW_FILL_EMPTY);
}

#define PLACE_FOR_SCROLLBAR 6

static void __update_Scrollbar()
{
	
}

static void __update_Text()
{
	
}

void MENU_printMenu(const MENU_Menu * const menu)
{
	static uint16_t frame_x_start = 0;
	static uint16_t frame_y_start = 0;
	static uint16_t frame_x_end   = 0;
	static uint16_t frame_y_end   = 0;
	
	frame_x_start = menu->begin_.x_ * FONT_16_WIDTH  + (uint8_t)(FONT_16_WIDTH  / 2) + PLACE_FOR_SCROLLBAR;
	frame_y_start = menu->begin_.y_ * FONT_16_HEIGHT + FONT_16_HEIGHT / 2;
	frame_x_end   = menu->end_.x_   * FONT_16_WIDTH  + (uint8_t)(FONT_16_WIDTH  / 2) + PLACE_FOR_SCROLLBAR;
	frame_y_end   = menu->end_.y_   * FONT_16_HEIGHT + FONT_16_HEIGHT / 2;
	
	//Paint_DrawNum(11*5,16*5,frame_y_end,&Font16,BLACK,WHITE);
	
	MENU_drawRectangle(frame_x_start, frame_y_start, frame_x_end, frame_y_end);
	MENU_
}

void MENU_updateMenu(const MENU_Menu * const menu)
{
	
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
	
	Paint_DrawString_EN(16*6,16*10,"Start",&Font16,BLUE,WHITE);
	Paint_DrawRectangle(16*6 - 5,16*10 - 5,16*6+16*4,16*10 + 16,WHITE,2,DRAW_FILL_EMPTY);
	
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
