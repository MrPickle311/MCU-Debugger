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

void MENU_printMessage(const MENU_Message* const msg)
{
	MENU_wipeAll();
	for_N( i , msg->lines_nmbr_)
		MENU_printTextLine(msg->str_lines_[i] ,
						   &ENTIRE_LINE       ,
						   msg->begin_.x_     ,
						   msg->begin_.y_ + i ,
						   NOT_SELECTED);
}

/*

#define PLACE_FOR_SCROLLBAR 8
#define SCROLLBAR_OFFSET	6

static inline void __print_scrollbar_line(uint8_t start_y    , 
										  uint8_t end_y      ,
										  uint8_t x          ,
										  uint8_t width)
{
	Paint_DrawLine( x		, 
				    start_y	, 
				    x		,
					end_y	, 
				    RED		, 
				    width	, 
				    LINE_STYLE_SOLID);
}
*/

/*
#define THIN_LINE_WIDTH  2
#define THICK_LINE_WIDTH 4



static void __print_Scrollbar(const MENU_Menu * const menu)
{
	//a thin line
	__print_scrollbar_line( MENU_TOP_Y_CELL_POS(menu) * FONT_16_HEIGHT       ,
							menu->end_.y_ * FONT_16_HEIGHT				     , 
						    menu->end_.x_ * FONT_16_WIDTH + SCROLLBAR_OFFSET , 
						    THIN_LINE_WIDTH);
	//a thick line
	//POKOMBINUJ TAK: START = DLUGOSC CALEGO SUWAKA * ( OBECNA POZYCJA / ILOSC STRON )
	// END = START + DLUGOSC CALEGO SUWAKA * ( 1 / ILOSC STRON )
	uint8_t start = (uint8_t)( MENU_LINES_NMBR(menu) * (float)( menu->page_buffer_.buffer_pos_ / menu->page_buffer_.buffer_size_ ) ) * FONT_16_HEIGHT;
	__print_scrollbar_line(  start,
							( menu->end_.y_  ) * FONT_16_HEIGHT				     , 
						     menu->end_.x_ * FONT_16_WIDTH + SCROLLBAR_OFFSET    , 
						     THICK_LINE_WIDTH);
}

*/

/*

static inline uint8_t __determine_start_option(const MENU_Menu * const menu, const DIRECTION direction)
{
	if(direction == DOWN)
		return menu->state_.displayed_range_.last_ + 1;
	else return menu->state_.displayed_range_.first_ - 1;
}

static inline uint8_t __get_options_nmbr(const MENU_Menu * const menu, const DIRECTION direction)
{
	uint8_t available_lines   =  MENU_LINES_NMBR(menu);
	uint8_t accumulated_lines = 0;
	uint8_t options_to_print  = 0;
	uint8_t current_option    = __determine_start_option(menu, direction);
	
	while(1)
	{
		if(current_option == 0 || current_option == menu->options_count_ - 1)
			break;
		
		accumulated_lines += menu->options_[current_option + direction * options_to_print ].lines_nmbr_;
		
		if(accumulated_lines > available_lines)
			break;
		++options_to_print;
	}
	
	return options_to_print;
}

*/

static inline void __print_option(const MENU_Menu * const menu , 
								  const MENU_Option* const opt , 
								  const uint8_t start_line     , 
								  const SELECTION is_selected)
{
	for_N( i , opt->lines_nmbr_ )
		MENU_printTextLine( opt->str_lines_[i]						   , 
							&ENTIRE_LINE							   ,
							MENU_LEFT_X_LEFT_CELL_POS(menu)			   ,
							MENU_TOP_Y_CELL_POS(menu) + i + start_line ,
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

/*
static inline void __update_State( MENU_Menu * const menu            , 
								   const DIRECTION   direction       , 
								   const uint8_t     options_printed   )
{
	menu->state_.displayed_range_.first_ += direction * options_printed; 
	menu->state_.displayed_range_.last_ += direction * options_printed; 
}
*/

void MENU_clearPage(MENU_Menu * const menu)
{
	Paint_ClearWindows( MENU_LEFT_X_LEFT_CELL_POS(menu)  * FONT_16_WIDTH   ,
					    MENU_TOP_Y_CELL_POS(menu)        * FONT_16_HEIGHT  ,
					    menu->end_.x_ * FONT_16_WIDTH					   ,
					    menu->end_.y_	 * FONT_16_HEIGHT				   ,
						BLACK);
}

//returns a 1 if mensu has to enroll , otherwise returns 0
static inline void __update_Text(MENU_Menu * const menu , const DIRECTION direction )
{	
	MENU_clearPage(menu);
	
	if(direction == UP)
		MENU_PageBuffer_goPrevious(&menu->page_buffer_);
	else MENU_PageBuffer_goNext(&menu->page_buffer_);
	
	__print_page(menu);
	
}

#define FRAME_LEFTSIDE_OFFSET 2

static void __draw_Frame(const MENU_Menu * const menu)
{
	MENU_drawRectangle( menu->begin_.x_ * FONT_16_WIDTH  + (uint8_t)( FONT_16_WIDTH / 2 ) + FRAME_LEFTSIDE_OFFSET ,
					    menu->begin_.y_ * FONT_16_HEIGHT + FONT_16_HEIGHT / 2									  , 
						menu->end_.x_   * FONT_16_WIDTH  + (uint8_t)( FONT_16_WIDTH / 2 )						  ,
						menu->end_.y_   * FONT_16_HEIGHT + FONT_16_HEIGHT / 2 );
}

void MENU_printMenu(const MENU_Menu * const menu)
{
	__draw_Frame(menu);
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

void MENU_goPreviousOption(MENU_Menu * const menu)
{
	if(menu->state_.active_option_  == 0)
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
	
	MENU_printNumber(breakpoints_total_nmbr,
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