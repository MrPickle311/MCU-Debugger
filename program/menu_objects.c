/*
 * menu_objects.c
 *
 * Created: 17/06/2021 12:38:32
 *  Author: Damian Wójcik
 */ 

#include "menu_objects.h"
#include "../drivers/lcd/GUI_Paint.h"

void MENU_initMenu(MENU_Menu*   menu				,
				   MENU_Page*   pages				,
				   uint8_t		pages_count			,
				   uint8_t      options_count		,
				   MENU_Point   begin				,
				   MENU_Point   end					,
				   MENU_State   state)
{
	menu->options_count_        = options_count;
	menu->begin_			    = begin;
	menu->end_				    = end;
	menu->state_			    = state;
	menu->state_.active_option_ = 0;
	
	MENU_PageBuffer_init(&menu->page_buffer_, pages , pages_count);
}

MENU_TextRange ENTIRE_LINE = {0, 22};

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

//forwarding_menu

MENU_Menu forwarding_menu_;

MENU_Option forwarding_menu_options_page1[] = {
	{
		forwarding_menu_option1_string ,
		2 ,
		NULL
	} ,
	{
		forwarding_menu_option2_string ,
		1 ,
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