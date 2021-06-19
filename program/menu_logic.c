/*
 * menu_logic.c
 *
 * Created: 19/06/2021 07:53:20
 *  Author: Damian W�jcik
 */ 

#include "menu_logic.h"

inline static void empty(){MENU_printNumber(6,&(MENU_Point){5 ,5 } );}

void MENU_activateForwardingMenu()
{
	MENU_printForwardingMenu();
	MENU_ButtonsService(&forwarding_menu);
}

void MENU_activateBrowsingMenu()
{
	MENU_printBrowsingMenu();
	MENU_ButtonsService(&browsing_menu);
}

void MENU_startMenu_OK_Service()
{
	DISABLE_BUTTONS();
	
	//COM_sendDebugSignal();//receiving data
	//COM_commandProcessor();// i have guarannce that data receiving will end until next line
	
	ENABLE_BUTTONS();
	MENU_printNumber(70,&(MENU_Point){5 ,5 } );
	MENU_activateForwardingMenu();
}

//initializng functions END

void MENU_browsingData()
{
	resetLoadBreakpoint();
	MENU_printBrowserMenu();
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
			MENU_printForwardingMenu();
			break;
		}
	}
}

void MENU_loadNextBreakpoint()
{
	DISABLE_BUTTONS();
	
	MENU_printMessage(&updating_data_msg);
	
	//resetSaveBreakpoint();
	
	//COM_sendDebugSignal();
	//COM_commandProcessor();//data are injected to fram now
	//
	//goToNextSaveBreakpoint();//so i can switch this buffer
	
	MENU_printForwardingMenu();
	
	ENABLE_BUTTONS();
}

void MENU_ButtonsService(MENU_Menu* const menu)
{
	ENABLE_BUTTONS();
	MENU_resetMenuView(menu);

	while(1)
	{
		//if(button_down(OK_BUTTON_MASK))
		//	MENU_startMenu_OK_Service();
			//AT_ACTIVE_OPTION(menu).OK_action_service_();
		
		if(button_down(RIGHT_BUTTON_MASK))
		{
			MENU_goNextOption(menu);
			MENU_printMenu(menu);
		}
		else  if (button_down(LEFT_BUTTON_MASK))
		{
			MENU_goPreviousOption(menu);
			MENU_printMenu(menu);
		}
		else  if (button_down(OK_BUTTON_MASK))
		{
			//menu->page_buffer_.pages_[0].options_[0].OK_action_service_();
			//MENU_printNumber(get_pressed_button_code(),&(MENU_Point){5 ,5 } );
			AT_ACTIVE_OPTION(menu).OK_action_service_();
		}
		else  if (button_down(BACK_BUTTON_MASK))
		{
			menu->BACK_action_service_();
		}
		else  if (button_down(SWITCH_BUTTON_MASK))
		{
			AT_ACTIVE_OPTION(menu).SWITCH_action_service_();
		}
		/*
		switch(get_pressed_button_code())
		{
			
			case RIGHT_BUTTON_MASK:
				MENU_goNextOption(menu);
				MENU_printMenu(menu);
				break;
			case LEFT_BUTTON_MASK:
				MENU_goPreviousOption(menu);
				MENU_printMenu(menu);
				break;
			case OK_BUTTON_MASK:
				AT_ACTIVE_OPTION(menu).OK_action_service_();
				break;
			case BACK_BUTTON_MASK:
				menu->BACK_action_service_();
				break;
			case SWITCH_BUTTON_MASK:
				AT_ACTIVE_OPTION(menu).SWITCH_action_service_();
				break;
			default: 
			//MENU_printNumber(get_pressed_button_code(),&(MENU_Point){5 ,5 } );
			//_delay_ms(5); 
			break;
		}*/
		/*
		if(button_down(RIGHT_BUTTON_MASK))
			MENU_goNextOption(menu);
		else if(button_down(LEFT_BUTTON_MASK))
			MENU_goPreviousOption(menu);
		else if(button_down(OK_BUTTON_MASK))
			AT_ACTIVE_OPTION(menu).OK_action_service_();
		else if(button_down(BACK_BUTTON_MASK))
			AT_ACTIVE_OPTION(menu).BACK_action_service_();
		else if(button_down(SWITCH_BUTTON_MASK))
			AT_ACTIVE_OPTION(menu).SWITCH_action_service_();
		*/
	}
}

void MENU_start()
{
	MENU_printStartMenu();
	MENU_ButtonsService(&start_menu);
}

//#define 

void MENU_updateBreakpointNumber()
{
	MENU_printNumber(current_breakpoint_to_load, &(MENU_Point){10 , 0} );
}

#define BROWSER_HEADER_X_START_POS 3
#define BROWSER_HEADER_Y_START_POS 0

void MENU_Browser_printHeaderBody()
{
	MENU_printTextLine( PSTR("Breakpoint:")			 , 
					    &ENTIRE_LINE				 , 
					    BROWSER_HEADER_X_START_POS	 ,
					    BROWSER_HEADER_Y_START_POS	 ,
					    NOT_SELECTED );
	
	MENU_updateBreakpointNumber();
	//Paint_DrawLine(0,16,LCD_WIDTH,16,BLUE,2,LINE_STYLE_SOLID);
}

void MENU_Browser_printBottomBody()
{
	Paint_DrawLine(0,LCD_HEIGHT - 32, LCD_WIDTH, LCD_HEIGHT - 32, WHITE,2,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
	Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
}

//TO LECI DO DATA VIEW
void MENU_Browser_printDataCell(uint8_t start_line)
{
	++start_line;
	Paint_DrawString_EN(0,16*start_line*3      -27 + (start_line - 1)*3, "Name:",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(0,16*start_line*3 + 32 -27 + (start_line - 1)*3, "Value:",&Font16,BLACK,WHITE);
}

void MENU_printData(uint8_t line)
{
	static char     string_buffer[40];
	static uint8_t  sb_idx					= 0;
	static uint32_t temp_value				= 0;
	static uint8_t  var_size				= 0;
	static uint8_t  helper_var				= 0;
	static char	    number_str_buffer[10];
	
	//reset
	sb_idx				 = 0;
	temp_value			 = 0;
	string_buffer[0]	 = 0;
	var_size			 = 0;
	helper_var			 = 0;
	number_str_buffer[0] = 0;
	
	FRAM_CircularBuffer_pop(fram_buffer,&var_size);//get var size
	
	//value
	for_N(i,var_size)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&helper_var);
		temp_value |= helper_var << ( 8 * i ) ;
	}
	
	FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	//name
	while(string_buffer[sb_idx++])
		FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	//view
	//MENU_printTextLine(PSTR("                  \0"),&ENTIRE_LINE , 4 , )
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

void MENU_printBrowserMenu()
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

//initialization

#define FORWARDING_MENU_PAGES_NMBR    1
#define FORWARDING_MENU_OPTIONS_COUNT 2

#define FORWARDING_MENU_X_BEGIN		  3
#define FORWARDING_MENU_Y_BEGIN		  7
#define FORWARDING_MENU_X_END 		  18
#define FORWARDING_MENU_Y_END 		  11

void MENU_init_ForwardingMenu()
{
	MENU_initMenu( &forwarding_menu													 ,
				   forwarding_menu_pages											 ,
				   FORWARDING_MENU_PAGES_NMBR										 ,
				   FORWARDING_MENU_OPTIONS_COUNT									 ,
				   (MENU_Point){ FORWARDING_MENU_X_BEGIN , FORWARDING_MENU_Y_BEGIN } ,
				   (MENU_Point){ FORWARDING_MENU_X_END   , FORWARDING_MENU_Y_END   } ,
				   (MENU_State){ FIRST_OPTION_AT_START }  );
	
	forwarding_menu.BACK_action_service_ = empty;
	
	forwarding_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = &MENU_loadNextBreakpoint;//
	forwarding_menu.page_buffer_.pages_[0].options_[1].OK_action_service_ = &MENU_activateBrowsingMenu;// MENU_loadNextBreakpoint;
	
}

#define START_MENU_PAGES_NMBR     1
#define START_MENU_OPTIONS_COUNT  1

#define START_MENU_X_BEGIN		  8
#define START_MENU_Y_BEGIN		  7
#define START_MENU_X_END 		  14
#define START_MENU_Y_END 		  9

void MENU_init_StartMenu()
{
	MENU_initMenu( &start_menu											   ,
				   start_menu_pages										   ,
				   START_MENU_PAGES_NMBR								   ,
				   START_MENU_OPTIONS_COUNT								   ,
				   (MENU_Point){ START_MENU_X_BEGIN , START_MENU_Y_BEGIN } ,
				   (MENU_Point){ START_MENU_X_END   , START_MENU_Y_END   } ,
				   (MENU_State){ FIRST_OPTION_AT_START }  );
	
	start_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = &MENU_startMenu_OK_Service;
	start_menu.BACK_action_service_ = empty;
}

#define BROWSING_MENU_PAGES_NMBR     1
#define BROWSING_MENU_OPTIONS_COUNT  2

#define BROWSING_MENU_X_BEGIN		  0
#define BROWSING_MENU_Y_BEGIN		  17
#define BROWSING_MENU_X_END 		  20
#define BROWSING_MENU_Y_END 		  19

void MENU_init_BrowsingMenu()
{
	MENU_initMenu( &browsing_menu														,
				   browsing_menu_pages													,
				   BROWSING_MENU_PAGES_NMBR											,
				   BROWSING_MENU_OPTIONS_COUNT										,
				   (MENU_Point){ BROWSING_MENU_X_BEGIN , BROWSING_MENU_Y_BEGIN }	,
				   (MENU_Point){ BROWSING_MENU_X_END   , BROWSING_MENU_Y_END   }	,
				   (MENU_State){ FIRST_OPTION_AT_START }  );
	
	browsing_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = empty;
	browsing_menu.page_buffer_.pages_[0].options_[1].OK_action_service_ = empty;

	browsing_menu.BACK_action_service_ = &MENU_activateForwardingMenu;
}

//ZR�B BUFOR DLA GRUP MENU!!!!

void MENU_Init()
{
	MENU_init_StartMenu();
	MENU_init_ForwardingMenu();
	MENU_init_BrowsingMenu();
}

//initialization END