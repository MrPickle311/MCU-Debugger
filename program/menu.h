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

//initializng functions

void MENU_init_ForwardingMenu();

//initializng functions END

#endif /* MENU_H_ */