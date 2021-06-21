/*
 * option_string.c
 *
 * Created: 17/06/2021 11:28:35
 *  Author: Damian Wójcik
 */ 

#include "flash_string.h"

//MESSAGES

flash_const_string_t updating_data_msg_string[] = {
			PGM_STR("Updating data...\0")
};

flash_const_string_t connected_with_device_data_msg_string[] = {
			PGM_STR("Connected\0") ,
			PGM_STR("with device!\0")
};

flash_const_string_t data_buffer_empty_msg_string[] = {
			PGM_STR("Empty\0")
};

//END MESSAGES

//FORWARDING MENU

flash_const_string_t forwarding_menu_option1_string[] = {
			PGM_STR("1.Load next\0") ,
			PGM_STR("breakpoint\0")
};

flash_const_string_t forwarding_menu_option2_string[] = {
			PGM_STR("2.Browse data\0")
};

//FORWARDING MENU END

//BROWSING MENU

extern flash_const_string_t browsing_menu_option1_string[] = {
			PGM_STR("1.Go next breakpoint\0")
};

extern flash_const_string_t browsing_menu_option2_string[] = {
			PGM_STR("2.Go next data\0") 
};

//BROWSING MENU END

//START MENU

flash_const_string_t start_menu_option1_string[] = {
			PGM_STR("Start\0")
};

//START MENU END