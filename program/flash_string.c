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

//END MESSAGES

flash_const_string_t forwarding_menu_option1_string[] = {
			PGM_STR("1.Go next\0") ,
			PGM_STR("breakpoint\0")
};
			
flash_const_string_t forwarding_menu_option2_string[] = {
			PGM_STR("2.Go next data\0") 
};

flash_const_string_t start_menu_option1_string[] = {
			PGM_STR("Start\0")
};