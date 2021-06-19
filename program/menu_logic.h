/*
 * menu_logic.h
 *
 * Created: 19/06/2021 07:53:06
 *  Author: Damian Wójcik
 */ 


#ifndef MENU_LOGIC_H_
#define MENU_LOGIC_H_

#include "menu_gui.h"

void MENU_printSorroundingLines();

void MENU_start();

void MENU_printDataCell(uint8_t start_line);

void MENU_printBottom();

void MENU_printHeaderBody();

void MENU_loadNextBreakpoint();

void MENU_Browser_nextBreakpoint();

void MENU_Browser_nextData();

//initialization

void MENU_init_ForwardingMenu();

void MENU_init_StartMenu();

void MENU_init_BrowsingMenu();

void MENU_Init();

//initialization END

#endif /* MENU_LOGIC_H_ */