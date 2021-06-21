/*
 * data_view.c
 *
 * Created: 19/06/2021 12:54:08
 *  Author: Damian Wójcik
 */ 

#include "data_view.h"

static inline void __display_prefix( const char __memx* prefix_txt , 
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

static inline uint8_t __display_var_name( const MENU_DataView* const data_view , 
										  uint8_t					 y_start)
{
	uint8_t x_pos				= DATA_VIEW_NAME_X_START_POS(data_view);
	uint8_t current_line_nmbr	= 0;
	bool	printing_is_done	= false;
	
	__display_name_prefix( (MENU_Point){ AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) , 
										 y_start  } );
	
	while ( !printing_is_done )
	{
		while( x_pos != data_view->area_.end_.x_ )
		{
			MENU_printChar( VAR_BUF_AT_CURRENT_CHAR(var_buffer)				     ,
						    &(MENU_Point){ x_pos , y_start + current_line_nmbr } ,
						    NOT_SELECTED);
			
			++x_pos;
			++var_buffer.current_text_pos_;
			
			if(VAR_BUF_AT_CURRENT_CHAR(var_buffer) == '\0')
			{
				printing_is_done = true;
				break;
			}
		}
		
		++current_line_nmbr;
		x_pos	= AREA_LEFT_X_LEFT_CELL_POS(data_view->area_);
	}
	
	return current_line_nmbr + 1;
}

static void __display_var_value( const MENU_DataView* const data_view ,
								 uint8_t					y_start)
{
	static char	number_str_buffer[NAME_MAX_LENGTH];
	
	__display_value_prefix( (MENU_Point){ AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) , 
							y_start  } );
	
	sprintf( number_str_buffer	 , 
			 "0x%08x"			 ,  
			 var_buffer.value_ );
	
	MENU_printTextLine( number_str_buffer						,
						&ENTIRE_LINE							,
						DATA_VIEW_VALUE_X_START_POS(data_view)	,
						y_start									,
						NOT_SELECTED);
}

#define TWO_LINES 2

void MENU_refreshDataView(MENU_DataView* const data_view)
{
	uint8_t lines_filled = 0;
	
	MENU_clearArea(&data_view->area_);
	
	while( lines_filled + TWO_LINES <= AREA_LINES_NMBR_AVALAIBLE(current_data_view.area_) )
	{
		COM_getVariableData();
		lines_filled += __display_var_name(data_view, lines_filled + 
													  AREA_TOP_Y_CELL_POS(data_view->area_));
		__display_var_value(data_view, lines_filled );
		++lines_filled;//a line for a value
	}
	
}

void MENU_displayDataView(MENU_DataView* const data_view)
{
	MENU_drawFrame(&data_view->area_);
	MENU_refreshDataView(data_view);
}

void MENU_initDataView( MENU_DataView* const data_view ,
					    MENU_Point   begin			   ,
					    MENU_Point   end			)
{
	data_view->area_.begin_ = begin;
	data_view->area_.end_   = end;
 }
