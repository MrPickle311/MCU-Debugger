/*
 * menu_objects.c
 *
 * Created: 17/06/2021 12:38:32
 *  Author: Damian Wójcik
 */ 

#include "menu_objects.h"
#include "../drivers/lcd/GUI_Paint.h"

void MENU_resetMenuView(MENU_Menu* const menu)
{
	menu->state_.active_option_    = 0;
	menu->page_buffer_.buffer_pos_ = 0;
}

void MENU_initMenu( MENU_Menu*   menu			,
				    MENU_Page*   pages			,
				    uint8_t		 pages_count	,
				    uint8_t      options_count	,
				    MENU_Point   begin			,
				    MENU_Point   end			,
				    MENU_State   state)
{
	menu->options_count_        = options_count;
	menu->area_.begin_			= begin;
	menu->area_.end_			= end;
	menu->state_			    = state;
	menu->state_.active_option_ = 0;
	
	MENU_PageBuffer_init(&menu->page_buffer_, pages , pages_count);
}

void MENU_assignActions( MENU_Menu* const menu			,
						 option_service_t ok_action		, 
					     option_service_t back_action	, 
					     option_service_t switch_action)
{
	//EMPTY
}

volatile const MENU_TextRange ENTIRE_LINE = { 0, 22 };

void MENU_Page_init(MENU_Page* page, MENU_Option* options, uint8_t options_count)
{
	page->options_		 = options;
	page->options_count_ = options_count;
}

void MENU_PageBuffer_init(MENU_PageBuffer* buffer ,
						  MENU_Page*	   pages  ,
						  uint8_t		   size)
{
	buffer->pages_		 = pages;
	buffer->buffer_size_ = size;
	buffer->buffer_pos_  = 0;
}

MENU_Page* MENU_PageBuffer_goNext(MENU_PageBuffer* buffer)
{
	static pos_temp = 0;
	
	pos_temp = buffer->buffer_pos_;
	buffer->buffer_pos_ = ( buffer->buffer_pos_ + 1 ) % buffer->buffer_size_ ;
	
	return &buffer->pages_[pos_temp];
}

MENU_Page* MENU_PageBuffer_goPrevious(MENU_PageBuffer* buffer)
{
	static pos_temp = 0;
	
	pos_temp = buffer->buffer_pos_;
	if(buffer->buffer_pos_ == 0)
		buffer->buffer_pos_ = buffer->buffer_size_ - 1;
	else buffer->buffer_pos_ -= 1; 
		
	return &buffer->pages_[pos_temp];
}

//messages

#define UPDATING_DATA_MSG_LINES_COUNT 1
#define UPDATING_DATA_MSG_X_BEGIN     3
#define UPDATING_DATA_MSG_Y_BEGIN     9

const __flash MENU_Message updating_data_msg = {
	updating_data_msg_string											, 
	UPDATING_DATA_MSG_LINES_COUNT										, 
	(MENU_Point){ UPDATING_DATA_MSG_X_BEGIN , UPDATING_DATA_MSG_Y_BEGIN }
};

#define CONNECTED_WITH_DEVICE_MSG_LINES_COUNT 2
#define CONNECTED_WITH_DEVICE_MSG_X_BEGIN     3
#define CONNECTED_WITH_DEVICE_MSG_Y_BEGIN     9

const __flash MENU_Message connected_with_device_msg = {
	connected_with_device_data_msg_string												,
	CONNECTED_WITH_DEVICE_MSG_LINES_COUNT												,
	(MENU_Point){ CONNECTED_WITH_DEVICE_MSG_X_BEGIN , CONNECTED_WITH_DEVICE_MSG_Y_BEGIN }
};

//messages END

//forwarding_menu

MENU_Menu forwarding_menu;

MENU_Option forwarding_menu_options_page1[] = {
	{
		forwarding_menu_option1_string ,
		2 ,
		NULL						,
		NULL						
	} ,
	{
		forwarding_menu_option2_string ,
		1 ,
		NULL						,
		NULL						
	}
};

MENU_Page  forwarding_menu_pages[] = {
	{
		forwarding_menu_options_page1,
		2
	}
};

//forwarding_menu END

//browsing menu

MENU_Menu   browsing_menu;

#define BROWSING_MENU_PAGE_1_OPTIONS_COUNT 2

MENU_Option browsing_menu_options_page1[] = {
	{
		browsing_menu_option1_string ,
		1							 ,
		NULL						 ,
		NULL
	} ,
	{
		browsing_menu_option2_string ,
		1							 ,
		NULL						 ,
		NULL
	}
};

MENU_Page   browsing_menu_pages[] = {
	{
		browsing_menu_options_page1		,
		BROWSING_MENU_PAGE_1_OPTIONS_COUNT
	}
};

//browsing menu END

//start_menu

MENU_Menu   start_menu;

#define START_MENU_PAGE_1_OPTIONS_COUNT 1

MENU_Option start_menu_options_page1[] = {
	{
		start_menu_option1_string	,
		1							,
		NULL						,
		NULL						
	}
};

MENU_Page   start_menu_pages[] = {
	{
		start_menu_options_page1		,
		START_MENU_PAGE_1_OPTIONS_COUNT
	}
};

//start_menu END

