/*
 * option_string.h
 *
 * Created: 17/06/2021 11:22:16
 *  Author: Damian Wójcik
 */ 

#ifndef FLASH_STRING_H_
#define FLASH_STRING_H_

#include <avr/pgmspace.h>

#define PGM_STR(X) ( (const __flash char[]){ X } )//initializing conversion

typedef const __flash char* const __flash flash_const_string_t;

typedef flash_const_string_t* flash_string_array_t;

//MESSAGES

extern flash_const_string_t updating_data_msg_string[];

//END MESSAGES

//FORWARDING MENU

extern flash_const_string_t forwarding_menu_option1_string[];

extern flash_const_string_t forwarding_menu_option2_string[];

//FORWARDING MENU END

//START MENU

extern flash_const_string_t start_menu_option1_string[];

//START MENU END

#endif /* OPTION_STRING_H_ */