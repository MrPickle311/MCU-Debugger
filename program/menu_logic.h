/*
 * menu_logic.h
 *
 * Created: 19/06/2021 07:53:06
 *  Author: Damian Wójcik
 */ 


#ifndef MENU_LOGIC_H_
#define MENU_LOGIC_H_

#include "menu_gui.h"
#include "data_view.h"


//logic functions

void MENU_loadNextBreakpoint();

void MENU_Browser_nextBreakpoint();

void MENU_Browser_nextData();

void MENU_startDebugMode();

//logic functions END


//menus activation

void MENU_activateForwardingMenu();

void MENU_activateBrowsingMenu();

void MENU_activateStartMenu();

//menus activation END


void MENU_printSorroundingLines();

void MENU_printDataCell(uint8_t start_line);

void MENU_printBottom();

void MENU_printHeaderBody();

//button service

void MENU_ButtonsService(MENU_Menu* const menu);

//button service END

//initialization

void MENU_init_ForwardingMenu();

void MENU_init_StartMenu();

void MENU_init_BrowsingMenu();

void MENU_init_CurrentDataView();

void MENU_Init();

//initialization END

//this function below starts the entire application
void MENU_startApplication();

#endif /* MENU_LOGIC_H_ */