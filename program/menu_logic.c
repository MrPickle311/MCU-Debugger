/*
 * menu_logic.c
 *
 * Created: 19/06/2021 07:53:20
 *  Author: Damian Wójcik
 */ 

#include "menu_logic.h"

//logic functions 

#define BRKP_NMBR_STRING_X_POS 10
#define BRKP_NMBR_STRING_Y_POS 10

static inline void __update_brakpoint_nmbr()
{
	MENU_printNumber( current_breakpoint_to_load										,	 
					  &(MENU_Point){ BRKP_NMBR_STRING_X_POS , BRKP_NMBR_STRING_Y_POS } );
}

#define BROWSER_HEADER_X_START_POS 3
#define BROWSER_HEADER_Y_START_POS 0

static inline void __browser_print_header_body()
{
	MENU_printTextLine( PSTR("Breakpoint:")			 , 
					    &ENTIRE_LINE				 , 
					    BROWSER_HEADER_X_START_POS	 ,
					    BROWSER_HEADER_Y_START_POS	 ,
					    NOT_SELECTED );
	
	__update_brakpoint_nmbr();
}

static inline void __update_loaded_breakpoints_nmbr()
{
	if(var_buffer.breakpoints_loaded_ < breakpoints_total_nmbr)
		++var_buffer.breakpoints_loaded_;
}

void MENU_loadNextBreakpoint()
{
	DISABLE_BUTTONS();
	
	MENU_printMessage(&updating_data_msg);
	
	CORE_resetSaveBreakpoint();
	
	COM_sendDebugSignal();
	COM_commandProcessor();//data are injected to fram now
	
	CORE_goToNextSaveBreakpoint();//so i can switch this buffer
	
	__update_loaded_breakpoints_nmbr();
	
	ENABLE_BUTTONS();
	
	MENU_activateForwardingMenu();
}

void MENU_Browser_nextBreakpoint()
{
	DISABLE_BUTTONS();
	
	CORE_goToNextLoadBreakpoint();
	
	//MENU_updateBreakpointNumber();
	MENU_refreshDataView(&current_data_view);
	MENU_ButtonsService(&browsing_menu);
	ENABLE_BUTTONS();
}

void MENU_Browser_nextData()
{
	DISABLE_BUTTONS();
	MENU_refreshDataView(&current_data_view);
	MENU_ButtonsService(&browsing_menu);
	ENABLE_BUTTONS();
}

void MENU_startDebugMode()
{
	DISABLE_BUTTONS();
	
	COM_sendDebugSignal();//receiving data
	COM_commandProcessor();// i have guarannce that data receiving will end until next line
	
	MENU_printMessage(&connected_with_device_msg);
	
	ENABLE_BUTTONS();

	MENU_activateForwardingMenu();
}

//logic functions END

//menus activation

void MENU_activateForwardingMenu()
{
	MENU_printForwardingMenu();
	MENU_ButtonsService(&forwarding_menu);
}

void MENU_activateBrowsingMenu()
{
	if(var_buffer.breakpoints_loaded_ == 0)
	{
		MENU_printMessage(&data_buffer_empty_msg);
		MENU_activateForwardingMenu();
	}	
	
	CORE_resetLoadBreakpoint();
	MENU_printBrowsingMenu();
	MENU_displayDataView(&current_data_view);
	MENU_ButtonsService(&browsing_menu);
}

void MENU_activateStartMenu()
{
	MENU_printStartMenu();
	MENU_ButtonsService(&start_menu);
}

//menus activation END

//current menu services && button service

static struct CurrentMenu
{
	MENU_Menu* current_menu_body;
}current_menu;

static inline void __reset_current_menu_view()
{
	MENU_resetMenuView(current_menu.current_menu_body);
}

static inline void __back_to_current_menu()
{
	MENU_printMenu(current_menu.current_menu_body);
	MENU_ButtonsService(current_menu.current_menu_body);
}

static inline void GO_NEXT_action_service()
{
	MENU_goNextOption(current_menu.current_menu_body);
	__back_to_current_menu();
}

static inline void GO_PREV_action_service()
{
	MENU_goPreviousOption(current_menu.current_menu_body);
	__back_to_current_menu();
}

static inline void __assign_actions(option_service_t* actions)
{
	actions[0] = GO_NEXT_action_service;
	actions[1] = GO_PREV_action_service;
	actions[2] = AT_ACTIVE_OPTION(current_menu.current_menu_body).OK_action_service_;
	actions[3] = current_menu.current_menu_body->BACK_action_service_ ;
}

void MENU_ButtonsService(MENU_Menu* const menu)
{
	static option_service_t actions [MENU_BUTTONS_COUNT];//current actions stored in an array
	static BUTTON_ID button_id	= NO_BUTTON_PRESSED;//the id of button which has been pressed
	
	//initiazilization
	current_menu.current_menu_body	= menu;//this function argument
	button_id						= NO_BUTTON_PRESSED;
	
	__assign_actions(actions);//assign current menu actions to the actions array
	
	ENABLE_BUTTONS();

	while(button_id == NO_BUTTON_PRESSED)//waiting for a selection
		button_id = get_pressed_button_id();
	
	DISABLE_BUTTONS();
	
	actions[button_id]();//invoke an certain action
}

//current menu services && button service END

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
	
	forwarding_menu.BACK_action_service_ = MENU_activateForwardingMenu;
	
	forwarding_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = MENU_loadNextBreakpoint;
	forwarding_menu.page_buffer_.pages_[0].options_[1].OK_action_service_ = MENU_activateBrowsingMenu;
	
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
	
	start_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = MENU_startDebugMode;
	start_menu.BACK_action_service_ = MENU_activateStartMenu;
}

#define BROWSING_MENU_PAGES_NMBR     1
#define BROWSING_MENU_OPTIONS_COUNT  2

#define BROWSING_MENU_X_BEGIN		  0
#define BROWSING_MENU_Y_BEGIN		  16
#define BROWSING_MENU_X_END 		  21
#define BROWSING_MENU_Y_END 		  19

void MENU_init_BrowsingMenu()
{
	MENU_initMenu( &browsing_menu													,
				   browsing_menu_pages												,
				   BROWSING_MENU_PAGES_NMBR											,
				   BROWSING_MENU_OPTIONS_COUNT										,
				   (MENU_Point){ BROWSING_MENU_X_BEGIN , BROWSING_MENU_Y_BEGIN }	,
				   (MENU_Point){ BROWSING_MENU_X_END   , BROWSING_MENU_Y_END   }	,
				   (MENU_State){ FIRST_OPTION_AT_START }  );
	
	browsing_menu.page_buffer_.pages_[0].options_[0].OK_action_service_ = MENU_Browser_nextBreakpoint;
	browsing_menu.page_buffer_.pages_[0].options_[1].OK_action_service_ = MENU_Browser_nextData;

	browsing_menu.BACK_action_service_ = MENU_activateForwardingMenu;
}

#define CURRENT_DATA_VIEW_X_BEGIN		  0
#define CURRENT_DATA_VIEW_Y_BEGIN		  0
#define CURRENT_DATA_VIEW_X_END 		  21
#define CURRENT_DATA_VIEW_Y_END 		  15

void MENU_init_CurrentDataView()
{
	MENU_initDataView( &current_data_view													  ,
					   (MENU_Point){ CURRENT_DATA_VIEW_X_BEGIN , CURRENT_DATA_VIEW_Y_BEGIN }  ,
					   (MENU_Point){ CURRENT_DATA_VIEW_X_END   , CURRENT_DATA_VIEW_Y_END   });
}

void MENU_Init()
{
	MENU_init_StartMenu();
	MENU_init_ForwardingMenu();
	MENU_init_BrowsingMenu();
	MENU_init_CurrentDataView();
	COM_initVariableBuffer();
	debounce_init();
}

//initialization END

void MENU_startApplication()
{
	MENU_Init();
	MENU_activateStartMenu();
}