/*
 * menu_objects.c
 *
 * Created: 17/06/2021 12:38:32
 *  Author: Damian Wójcik
 */ 

#include "menu_objects.h"
#include "../drivers/lcd/GUI_Paint.h"

void MENU_initMenu(MENU_Menu*   menu				,
				   MENU_Option* options				,
				   uint8_t      options_count		,
				   uint8_t      total_lines_count	,
				   MENU_Point   begin				,
				   MENU_Point   end					,
				   MENU_State   state)
{
	menu->options_		     = options;
	menu->options_count_     = options_count;
	menu->total_lines_count_ = total_lines_count;
	menu->begin_			 = begin;
	menu->end_				 = end;
	menu->state_			 = state;
}



MENU_TextRange WHOLE_LINE = {0, 22};

//forwarding_menu

MENU_Menu forwarding_menu_;

MENU_Option forwarding_menu_option_1 = {
	forwarding_menu_option1_string ,
	2 ,
	NULL 
};

MENU_Option forwarding_menu_option_2 = {
	forwarding_menu_option2_string ,
	1 ,
	NULL
};

//forwarding_menu END