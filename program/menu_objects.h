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
	option_service_t     OK_action_service_;
	option_service_t     SWITCH_action_service_;
};

typedef struct MENU_Option MENU_Option;

#define FIRST_OPTION_AT_START  0

volatile struct MENU_Point
{
	uint8_t x_;
	uint8_t y_;
};

typedef struct MENU_Point MENU_Point;

volatile struct MENU_Area
{
	MENU_Point	begin_;
	MENU_Point	end_;
};

typedef struct MENU_Area MENU_Area;

volatile struct MENU_TextRange
{
	uint8_t x_start_;
	uint8_t x_end_;
};

typedef struct MENU_TextRange MENU_TextRange;

volatile const MENU_TextRange ENTIRE_LINE;

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
#define AT_ACTIVE_OPTION(menu)			AT_CURRENT_PAGE(menu).options_[menu->state_.active_option_]

void MENU_PageBuffer_init( MENU_PageBuffer* buffer ,
						   MENU_Page*	    pages  ,
						   uint8_t		    size);
										   
MENU_Page*	MENU_PageBuffer_goNext(MENU_PageBuffer* buffer);

MENU_Page*	MENU_PageBuffer_goPrevious(MENU_PageBuffer* buffer);

volatile struct MENU_Menu
{
	MENU_PageBuffer	  page_buffer_;
	uint8_t			  options_count_;
	MENU_Area		  area_;
	MENU_State		  state_;
	option_service_t  BACK_action_service_;
};

typedef struct MENU_Menu MENU_Menu;

void MENU_resetMenuView(MENU_Menu* const menu);

volatile struct MENU_Message
{
	flash_string_array_t str_lines_;
	uint8_t				 lines_nmbr_;
	MENU_Point			 begin_;
};

typedef struct MENU_Message MENU_Message;

void MENU_initMenu( MENU_Menu*   menu			,
				    MENU_Page*   pages			,
				    uint8_t		 pages_count	,
				    uint8_t      options_count	,
				    MENU_Point   begin			,
				    MENU_Point   end			,
				    MENU_State   state);

#define AREA_TOP_Y_CELL_POS(area)		  ( area.begin_.y_ + 1 )
#define AREA_BOTTOM_Y_CELL_POS(area)	  ( area.end_.y_   - 1 )
#define AREA_LEFT_X_LEFT_CELL_POS(area)   ( area.begin_.x_ + 1 )
#define AREA_LEFT_X_RIGHT_CELL_POS(area)  ( area.end_.x_   - 1 )
#define AREA_LINES_NMBR_AVALAIBLE(area)	  ( area.end_.y_   - area.begin_.y_ - 1 )
#define AREA_IN_LINE_CHARS_NMBR(area)	  ( area.end_.x_   - area.begin_.x_ - 1 )

//messages

extern const __flash MENU_Message updating_data_msg;

extern const __flash MENU_Message connected_with_device_msg;

//messages END

//forwarding_menu

extern MENU_Menu   forwarding_menu;

extern MENU_Option forwarding_menu_options_page1[];

extern MENU_Page   forwarding_menu_pages[];

//forwarding_menu END

//browsing_menu

extern MENU_Menu   browsing_menu;

extern MENU_Option browsing_menu_options_page1[];

extern MENU_Page   browsing_menu_pages[];

//browsing_menu END

//start_menu

extern MENU_Menu   start_menu;

extern MENU_Option start_menu_options_page1[];

extern MENU_Page   start_menu_pages[];

//start_menu END



#endif /* MENU_OBJECTS_H_ */