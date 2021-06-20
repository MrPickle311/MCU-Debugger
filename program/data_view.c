/*
 * data_view.c
 *
 * Created: 19/06/2021 12:54:08
 *  Author: Damian Wójcik
 */ 

#include "data_view.h"

void MENU_nextCell(MENU_DataView* const data_view)
{
	data_view->current_cell_to_display_ = ( data_view->current_cell_to_display_  + 1 )  %  
											data_view->cells_count_to_display_;
}

static inline void __display_prefix(const char __memx* prefix_txt , 
									const MENU_Point   start)
{
	MENU_printTextLine( prefix_txt	 ,
						&ENTIRE_LINE ,
						start.x_	 ,
						start.y_ 	 ,
						NOT_SELECTED);
}


static inline void __display_name_prefix(const MENU_Point start)
{
	__display_prefix( PSTR("Name:\0") , start);
}

static inline void __display_value_prefix(const MENU_Point start)
{
	__display_prefix( PSTR("Value:\0") , start);
}

static inline void __display_var_name(const MENU_DataView* const data_view , 
									  uint8_t					 y_start)
{
	uint8_t x_pos = DATA_VIEW_NAME_X_START_POS(data_view);
	
	for_N( i , DATA_VIEW_AT_CURRENT_CELL(data_view).lines_count_ )
	{
		if(i != 0 )	
			x_pos = AREA_LEFT_X_LEFT_CELL_POS(data_view->area_);
			
		MENU_printTextLine( DATA_VIEW_AT_CURRENT_CELL(data_view).name_lines_[i]	,
						    &ENTIRE_LINE										,
						    x_pos												,
						    y_start + i											,
						    NOT_SELECTED);
	}
}

#define NAME_MAX_LENGTH		40

static void __display_var_value( const MENU_DataView* const data_view ,
								 uint8_t					y_start)
{
	static char	number_str_buffer[NAME_MAX_LENGTH];
	
	sprintf( number_str_buffer								, 
			 "0x%08x"										,  
			 DATA_VIEW_AT_CURRENT_CELL(data_view).value_ );
	
	MENU_printTextLine( number_str_buffer						,
						&ENTIRE_LINE							,
						DATA_VIEW_VALUE_X_START_POS(data_view)	,
						y_start									,
						NOT_SELECTED);
}

void MENU_displayCell( const MENU_DataView* const data_view ,
					   uint8_t					  y_shift)
{
	__display_var_name( data_view ,
						AREA_TOP_Y_CELL_POS(data_view->area_)	+ 
						y_shift );
	
	__display_var_value( data_view									, 
						 AREA_TOP_Y_CELL_POS(data_view->area_)	+
						 y_shift								+ 
						 DATA_VIEW_AT_CURRENT_CELL(data_view).lines_count_);
}

void MENU_displayDataView(MENU_DataView* const data_view)
{
	uint8_t accum = 0;
	
	MENU_clearArea(&data_view->area_);
	
	for_N(i , data_view->cells_count_to_display_)
	{
		MENU_displayCell(data_view , 
						 accum);
		accum = DATA_VIEW_AT_CURRENT_CELL(data_view).lines_count_;
		MENU_nextCell(data_view);
	}
}
