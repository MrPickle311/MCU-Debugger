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

MENU_TextRange WHOLE_LINE;

#define TEXT_RANGE_DIFF(range)	( range.x_end_ - range.x_start_ )

struct MENU_State
{
	MENU_OptionRange displayed_range_;
};

typedef struct MENU_State MENU_State;

volatile struct MENU_Menu
{
	MENU_Option* options_;
	uint8_t      options_count_;
	uint8_t      total_lines_count_;
	MENU_Point   begin_;
	MENU_Point   end_;
	MENU_State   state_;
};

typedef struct MENU_Menu MENU_Menu;

void MENU_initMenu(MENU_Menu*   menu				,
				   MENU_Option* options				,
				   uint8_t      options_count		,
				   uint8_t      total_lines_count	,
				   MENU_Point   begin				,
				   MENU_Point   end					,
				   MENU_State   state);


#define MENU_TOP_Y_CELL_POS(menu)		  ( menu->begin_.y_ + 1 )
#define MENU_BOTTOM_Y_CELL_POS(menu)	  ( menu->end_.y_   - 1 )
#define MENU_LEFT_X_LEFT_CELL_POS(menu)   ( menu->begin_.x_ + 1 )
#define MENU_LEFT_X_RIGHT_CELL_POS(menu)  ( menu->end_.x_   - 1 )
#define MENU_LINES_NMBR(menu)			  ( menu->end_.y_ - menu->begin_.y_ - 1 )
#define MENU_IN_LINE_CHARS_NMBR(menu)	  ( menu->end_.x_ - menu->begin_.x_ - 1 )

//forwarding_menu

extern MENU_Menu forwarding_menu_;

extern MENU_Option forwarding_menu_option_1;

extern MENU_Option forwarding_menu_option_2;

//forwarding_menu END

#endif /* MENU_OBJECTS_H_ */