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
#include "menu_objects.h"

volatile uint8_t start_flag;

#define UP   -1
#define DOWN  1

typedef uint8_t DIRECTION;

#define NOT_SELECTED  BLACK
#define SELECTED      RED

typedef uint16_t SELECTION;

void MENU_printChar(const char chr			    , 
				    MENU_Point* start_point     , 
				    const SELECTION is_selected);


//this function will return a -1 value if it encounter a '\0'
int8_t MENU_printTextLine(const char __memx*		    str         , 
						  const MENU_TextRange* const   text_slice  ,
						  uint8_t		  				x_start_pos ,
						  uint8_t						y_pos	    ,
						  const SELECTION				is_selected);

void MENU_drawRectangle(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end);

void MENU_clearPage(MENU_Menu * const menu);

void MENU_printMenu(const MENU_Menu * const menu);

void MENU_updateMenu(const MENU_Menu * const menu, const DIRECTION direction);

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

//initializng functions

void MENU_init_ForwardingMenu();

//initializng functions END

#endif /* MENU_H_ */