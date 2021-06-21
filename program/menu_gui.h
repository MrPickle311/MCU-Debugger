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



#define UP   -1
#define DOWN  1

typedef uint8_t DIRECTION;

#define NOT_SELECTED		   BLACK
#define SELECTED			   RED

typedef uint16_t SELECTION;

void MENU_printChar( const char chr			     , 
				     MENU_Point* start_point     , 
				     const SELECTION is_selected);

void MENU_printNumber( uint16_t nmbr , MENU_Point* start_point);

//this function will return a -1 value if it encounter a '\0'
int8_t MENU_printTextLine( const char __memx*		    str         , 
						   const MENU_TextRange* const  text_slice  ,
						   uint8_t		  				x_start_pos ,
						   uint8_t						y_pos	    ,
						   const SELECTION				is_selected);

void MENU_drawRectangle( UWORD x_start , 
						 UWORD y_start , 
						 UWORD x_end   , 
						 UWORD y_end);

void MENU_wipeAll();

void MENU_printMessage(const MENU_Message __memx* const msg);

void MENU_clearArea(const MENU_Area* const area);

void MENU_drawFrame(const MENU_Area* const area);

void MENU_printMenu(const MENU_Menu* const menu);

void MENU_updateCurrentPage( const MENU_Menu * const menu , 
							 const DIRECTION direction);

void MENU_goNextOption(MENU_Menu * const menu);

void MENU_goPreviousOption(MENU_Menu * const menu);

void MENU_move( MENU_Menu * const menu	  , 
			    const DIRECTION direction);


//printing certain menus

void MENU_printForwardingMenu();

void MENU_printStartMenu();

void MENU_printBrowsingMenu();

//printing of all menus END

#endif /* MENU_H_ */