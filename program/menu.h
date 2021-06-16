/*
 * menu.h
 *
 * Created: 15/05/2021 16:37:53
 *  Author: Damian Wójcik
 */ 

#ifndef MENU_H_
#define MENU_H_

#include "../drivers/lcd/GUI_Paint.h"
#include "../drivers/core/exti.h"
#include "../drivers/core/port.h"
#include "debounce.h"
#include "com.h"

volatile uint8_t start_flag;

volatile struct MENU_Option
{
	const char __memx* str_lines_;
	uint8_t lines_nmbr;
	void (*option_service_)(void);
};

typedef struct MENU_Option MENU_Option;

extern MENU_Option menuS;

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

#define TEXT_RANGE_DIFF(range)	( range.x_end_ - range.x_start_ )

volatile struct MENU_Menu
{
	MENU_Option* options_;
	uint8_t options_count_;
	uint8_t lines_count_;
	MENU_Point begin_;
	MENU_Point end_;
};

typedef struct MENU_Menu MENU_Menu;

#define MENU_TOP_Y_CELL_POS(menu)		  ( menu.begin_.y_ + 1 )
#define MENU_BOTTOM_Y_CELL_POS(menu)	  ( menu.end_.y_   - 1 )
#define MENU_LEFT_X_LEFT_CELL_POS(menu)   ( menu.begin_.x_ + 1 )
#define MENU_LEFT_X_RIGHT_CELL_POS(menu)  ( menu.end_.x_   - 1 )
#define MENU_LINES_NMBR(menu)			  ( menu.end_.y_ - menu.start_.y_ - 1 )
#define MENU_IN_LINE_CHARS_NMBR(menu)	  ( menu.end_.x_ - menu.start_.x_ - 1 )

void MENU_printChar_NotSelected(const char chr,MENU_Point* start_point);

void MENU_printChar_Selected(const char chr,MENU_Point* start_point);

//this function will return a -1 value if it encounter a '\0'
int8_t MENU_printTextLine_NotSelected(const char __memx*		    str         , 
							          const MENU_TextRange* const   text_slice  ,
							          uint8_t		  				x_start_pos ,
							          uint8_t						y_pos);

int8_t MENU_printTextLine_Selected(const char* const			 str         , 
								   const MENU_TextRange* const   text_slice  ,
								   uint8_t		  				 x_start_pos ,
								   uint8_t						 y_pos);

void MENU_drawRectangle(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end);

void MENU_printMenu(const MENU_Menu * const menu);

void MENU_updateMenu(const MENU_Menu * const menu);

void MENU_printSorroundingLines();

void MENU_printStartMenu();

void MENU_navigate();

void MENU_printChoiceMenu();

void MENU_printDataCell(uint8_t start_line);

void MENU_printBottom();

void MENU_printHeaderBody();

void MENU_loadNextBreakpoint();

void MENU_Browser_nextBreakpoint();

void MENU_Browser_nextData();

#endif /* MENU_H_ */