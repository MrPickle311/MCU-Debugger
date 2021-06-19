/*
 * menu_logic.h
 *
 * Created: 19/06/2021 07:53:06
 *  Author: Damian Wójcik
 */ 


#ifndef MENU_LOGIC_H_
#define MENU_LOGIC_H_

#include "menu_gui.h"

volatile uint8_t start_flag;

void MENU_printSorroundingLines();

void MENU_navigate();

void MENU_printDataCell(uint8_t start_line);

void MENU_printBottom();

void MENU_printHeaderBody();

void MENU_loadNextBreakpoint();

void MENU_Browser_nextBreakpoint();

void MENU_Browser_nextData();


#endif /* MENU_LOGIC_H_ */