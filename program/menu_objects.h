/*
 * menu_objects.h
 *
 * Created: 17/06/2021 12:38:18
 *  Author: Damian
 */ 

#ifndef MENU_OBJECTS_H_
#define MENU_OBJECTS_H_

#include <avr/io.h>
#include "flash_string.h"
#include <stdbool.h>

typedef void (*option_service_t)();

volatile struct MENU_Option
{
	flash_string_array_t str_lines_;
	uint8_t				 lines_nmbr_;
	option_service_t     option_service_;
};

typedef struct MENU_Option MENU_Option;

#define FIRST_OPTION_AT_START  0

struct MENU_OptionRange
{
	uint8_t first_;
	uint8_t last_;
};

typedef struct MENU_OptionRange MENU_OptionRange;

volatile struct MENU_Point
{
	uint8_t x_;
	uint8_t y_;
};

typedef struct MENU_Point MENU_Point;

volatile struct MENU_TextRange
{
	uint8_t x_start_;
	uint8_t x_end_;
};

typedef struct MENU_TextRange MENU_TextRange;

MENU_TextRange ENTIRE_LINE;

struct MENU_State
{
	uint8_t			 active_option_;
};

typedef struct MENU_State MENU_State;

volatile struct MENU_Page
{
	MENU_Option* options_;
	uint8_t		 options_count_;
};

typedef struct MENU_Page MENU_Page;

void MENU_Page_init(MENU_Page* page, MENU_Option* options, uint8_t options_count);

volatile struct MENU_PageBuffer
{
	MENU_Page* pages_;
	uint8_t    buffer_size_;
	uint8_t    buffer_pos_;
};

typedef struct MENU_PageBuffer MENU_PageBuffer;

#define AT_CURRENT_PAGE(menu)			menu->page_buffer_.pages_[menu->page_buffer_.buffer_pos_]
#define IN_PAGE_OPTIONS_COUNT(menu)		menu->page_buffer_.pages_[menu->page_buffer_.buffer_pos_].options_count_

void MENU_PageBuffer_init(MENU_PageBuffer* buffer ,
						  MENU_Page*	   pages  ,
						  uint8_t		   size);

MENU_Page*	MENU_PageBuffer_goNext(MENU_PageBuffer* buffer);

MENU_Page*	MENU_PageBuffer_goPrevious(MENU_PageBuffer* buffer);

volatile struct MENU_Menu
{
	MENU_PageBuffer page_buffer_;
	uint8_t         options_count_;
	MENU_Point      begin_;
	MENU_Point      end_;
	MENU_State      state_;
};

typedef struct MENU_Menu MENU_Menu;

volatile struct MENU_Message
{
	flash_string_array_t str_lines_;
	uint8_t				 lines_nmbr_;
	MENU_Point			 begin_;
};

typedef struct MENU_Message MENU_Message;

void MENU_initMenu(MENU_Menu*   menu				,
				   MENU_Page*   pages				,
				   uint8_t		pages_count			,
				   uint8_t      options_count		,
				   MENU_Point   begin				,
				   MENU_Point   end					,
				   MENU_State   state);


#define MENU_TOP_Y_CELL_POS(menu)		  ( menu->begin_.y_ + 1 )
#define MENU_BOTTOM_Y_CELL_POS(menu)	  ( menu->end_.y_   - 1 )
#define MENU_LEFT_X_LEFT_CELL_POS(menu)   ( menu->begin_.x_ + 1 )
#define MENU_LEFT_X_RIGHT_CELL_POS(menu)  ( menu->end_.x_   - 1 )
#define MENU_LINES_NMBR(menu)			  ( menu->end_.y_ - menu->begin_.y_ - 1 )
#define MENU_IN_LINE_CHARS_NMBR(menu)	  ( menu->end_.x_ - menu->begin_.x_ - 1 )

//messages

extern MENU_Message updating_data_msg;

//messages END

//forwarding_menu

extern MENU_Menu   forwarding_menu_;

extern MENU_Option forwarding_menu_options_page1[];

extern MENU_Page   forwarding_menu_pages[];

//forwarding_menu END

//start_menu

extern MENU_Menu   start_menu_;

extern MENU_Option start_menu_options_page1[];

extern MENU_Page   start_menu_pages[];

//start_menu END

//initializng functions

void MENU_init_ForwardingMenu();

void MENU_init_StartMenu();

//initializng functions END

#endif /* MENU_OBJECTS_H_ */