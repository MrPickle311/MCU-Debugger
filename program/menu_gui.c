/*
 * menu.c
 *
 * Created: 15/05/2021 16:38:07
 *  Author: Damian Wójcik
 */ 

#include "menu_gui.h"
#include "../drivers/core/timer.h"

void MENU_printChar(const char chr			    , 
				    MENU_Point* start_point     , 
				    const SELECTION is_selected)
{
	Paint_DrawChar( start_point->x_ * FONT_16_WIDTH  ,
				    start_point->y_ * FONT_16_HEIGHT ,
				    chr								 ,
				    &Font16							 ,
				    is_selected					     ,
				    WHITE);
}

void MENU_printNumber(uint16_t nmbr , MENU_Point* start_point)
{
	Paint_DrawNum( start_point->x_ * FONT_16_WIDTH  ,
				   start_point->y_ * FONT_16_HEIGHT ,
				   nmbr								,
				   &Font16							,
				   BLACK							,
				   WHITE);
}

int8_t MENU_printTextLine(const char __memx*		    str         , 
						  const MENU_TextRange* const   text_slice  ,
						  uint8_t		  				x_start_pos ,
						  uint8_t						y_pos	    ,
						  const SELECTION				is_selected)
{
	static const char __memx*       text_ptr     = NULL;
	static MENU_Point			    char_point   = {0,0}; 
	static uint8_t				    in_slice_pos = 0; 
	static uint8_t				    char_count   = 0; 
	
	text_ptr      = &str[text_slice->x_start_];
	char_point.x_ = x_start_pos;
	char_point.y_ = y_pos;
	in_slice_pos  = 0;
	char_count    = text_slice->x_end_ - text_slice->x_start_ + 1;
	
	while( in_slice_pos <  char_count )
	{
		if( *text_ptr == '\0' )
			return - 1;
		
		MENU_printChar( *text_ptr    , 
						&char_point  , 
						is_selected); 
	
		++char_point.x_;
		++text_ptr;
		++in_slice_pos;
	}
	
	return text_slice->x_end_;
}

#define MENU_FRAME_WIDTH 2

void MENU_drawRectangle(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end)
{
	Paint_DrawRectangle( x_start		   , 
						 y_start		   , 
						 x_end			   , 
						 y_end			   , 
						 WHITE			   , 
						 MENU_FRAME_WIDTH  , 
						 DRAW_FILL_EMPTY);
}

void MENU_printMessage(const MENU_Message __memx* const msg)
{
	MENU_wipeAll();
	for_N( i , msg->lines_nmbr_)
		MENU_printTextLine( msg->str_lines_[i] ,
						    &ENTIRE_LINE       ,
						    msg->begin_.x_     ,
						    msg->begin_.y_ + i ,
						    NOT_SELECTED);
	_delay_ms(300);
}

static inline void __print_option( const MENU_Menu * const menu , 
								   const MENU_Option* const opt , 
								   const uint8_t start_line     , 
								   const SELECTION is_selected)
{
	for_N( i , opt->lines_nmbr_ )
		MENU_printTextLine( opt->str_lines_[i]									, 
							&ENTIRE_LINE										,
							AREA_LEFT_X_LEFT_CELL_POS(menu->area_)			    ,
							AREA_TOP_Y_CELL_POS(menu->area_) + i + start_line	,
							is_selected);
}

static inline void __print_page( const MENU_Menu* const menu )
{
	uint8_t current_shift = 0;
	SELECTION is_selected = NOT_SELECTED;
	
	for_N( i , IN_PAGE_OPTIONS_COUNT(menu) )
	{
		if(menu->state_.active_option_ == i)
			is_selected = SELECTED;
		else is_selected = NOT_SELECTED;
		
		__print_option(menu								  , 
					   &AT_CURRENT_PAGE(menu).options_[i] ,
					   current_shift					  ,
					   is_selected);
		
		current_shift = AT_CURRENT_PAGE(menu).options_[i].lines_nmbr_;
	}
}

void MENU_clearArea(const MENU_Area * const area)
{
	Paint_ClearWindows( AREA_LEFT_X_LEFT_CELL_POS((*area))  * FONT_16_WIDTH   ,
					    AREA_TOP_Y_CELL_POS((*area))        * FONT_16_HEIGHT  ,
					    area->end_.x_	* FONT_16_WIDTH						  ,
					    area->end_.y_	* FONT_16_HEIGHT					  ,
						BLACK);
}

//returns a 1 if mensu has to enroll , otherwise returns 0
static inline void __update_Text(MENU_Menu * const menu , const DIRECTION direction )
{	
	MENU_clearArea(&menu->area_);
	
	if(direction == UP)
		MENU_PageBuffer_goPrevious(&menu->page_buffer_);
	else MENU_PageBuffer_goNext(&menu->page_buffer_);
	
	__print_page(menu);
	
}

#define FRAME_LEFTSIDE_OFFSET 2

void MENU_drawFrame(const MENU_Area* const area)
{
	MENU_drawRectangle( area->begin_.x_ * FONT_16_WIDTH  + (uint8_t)( FONT_16_WIDTH / 2 ) + FRAME_LEFTSIDE_OFFSET ,
					    area->begin_.y_ * FONT_16_HEIGHT + FONT_16_HEIGHT / 2									  , 
						area->end_.x_   * FONT_16_WIDTH  + (uint8_t)( FONT_16_WIDTH / 2 )						  ,
						area->end_.y_   * FONT_16_HEIGHT + FONT_16_HEIGHT / 2 );
}

void MENU_printMenu(const MENU_Menu * const menu)
{
	MENU_drawFrame(&menu->area_);
	__print_page(menu);
}

void MENU_updateCurrentPage(const MENU_Menu * const menu, const DIRECTION direction)
{
	__update_Text(menu , direction);
}

void MENU_goNextOption(MENU_Menu * const menu)
{
	if(menu->state_.active_option_ + 1 == AT_CURRENT_PAGE(menu).options_count_)
	{
		MENU_PageBuffer_goNext(menu);
		menu->state_.active_option_ = 0;
		return;
	}
	
	++menu->state_.active_option_;
}

#define FIRST_OPTION 0

void MENU_goPreviousOption(MENU_Menu * const menu)
{
	if(menu->state_.active_option_  == FIRST_OPTION)
	{
		MENU_PageBuffer_goPrevious(menu);
		menu->state_.active_option_ = AT_CURRENT_PAGE(menu).options_count_ - 1;
		return;
	}
	
	--menu->state_.active_option_;
}

void MENU_move(MENU_Menu * const menu, const DIRECTION direction)
{
	if(direction == UP)
		MENU_goPreviousOption(menu);
	else MENU_goNextOption(menu);
}

void MENU_wipeAll()
{
	Paint_Clear(BLACK);
}

//printing of all menus

#define BRKP_TEXT_X_START_POS	4
#define BRKP_TEXT_Y_START_POS	15
#define BRKP_NMBR_X_START_POS   16
#define BRKP_NMBR_Y_START_POS   15

void MENU_printForwardingMenu()
{
	MENU_wipeAll();
	MENU_printMenu(&forwarding_menu);
	
	MENU_printTextLine( PSTR("Breakpoints: " ) ,
						&ENTIRE_LINE	       ,
						BRKP_TEXT_X_START_POS  ,
						BRKP_TEXT_Y_START_POS  ,
						NOT_SELECTED);
	
	MENU_printNumber( breakpoints_total_nmbr											,
					  &(MENU_Point){ BRKP_NMBR_X_START_POS , BRKP_NMBR_Y_START_POS } );
}


void MENU_printStartMenu()
{
	MENU_wipeAll();
	MENU_printMenu(&start_menu);
}

void MENU_printBrowsingMenu()
{
	MENU_wipeAll();
	MENU_printMenu(&browsing_menu);
}

//printing of all menus END