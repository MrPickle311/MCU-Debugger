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
	char* str_;
	uint8_t str_length_;
	void (*option_service_)(void);
};

typedef struct MENU_Option MENU_Option;

volatile struct MENU_Point
{
	uint8_t x_;
	uint8_t y_;
};

typedef struct MENU_Point MENU_Point;

volatile struct MENU_Menu
{
	MENU_Option* options_;
	uint8_t options_count_;
	MENU_Point begin_;
	MENU_Point end_;
};

typedef struct MENU_Menu MENU_Menu;

void MENU_printString_NotSelected(const char* const str, MENU_Point* start_point);

void MENU_printString_Selected(const char* const str, MENU_Point* start_point);

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