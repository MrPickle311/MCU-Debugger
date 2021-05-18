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