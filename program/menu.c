/*
 * menu.c
 *
 * Created: 15/05/2021 16:38:07
 *  Author: Damian Wójcik
 */ 

#include "menu.h"
#include "../drivers/core/timer.h"

volatile uint8_t start_flag = 0;


void MENU_updateBreakpointNumber()
{
	Paint_DrawNum(16*10,0,current_breakpoint_to_load,&Font16,BLUE,WHITE);
}

void MENU_Browser_printHeaderBody()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*3,0,"Breakpoint:",&Font16,BLUE,WHITE);
	MENU_updateBreakpointNumber();
	Paint_DrawLine(0,16,LCD_WIDTH,16,BLUE,2,LINE_STYLE_SOLID);
}

void MENU_Browser_printBottomBody()
{
	Paint_DrawLine(0,LCD_HEIGHT - 32, LCD_WIDTH, LCD_HEIGHT - 32, WHITE,2,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
	Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
}

void MENU_Browser_printDataCell(uint8_t start_line)
{
	++start_line;
	//Paint_DrawLine(     0,16*start_line*3      -27 + (start_line - 1)*2,LCD_WIDTH,16*start_line*3      -27 + (start_line - 1)*2,BLUE,1,LINE_STYLE_SOLID);
	Paint_DrawString_EN(0,16*start_line*3      -27 + (start_line - 1)*3, "Name:",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(0,16*start_line*3 + 32 -27 + (start_line - 1)*3, "Value:",&Font16,BLACK,WHITE);
}

volatile char string_buffer[40];
volatile uint8_t sb_idx = 0;
volatile uint32_t temp_value = 0;
volatile uint8_t var_size = 0;
volatile uint8_t helper_var = 0;
volatile char number_str_buffer[10];

void MENU_printData(uint8_t line)
{
	//reset
	sb_idx = 0;
	temp_value =  0;
	string_buffer[0] = 0;
	var_size = 0;
	helper_var = 0;
	number_str_buffer[0] = 0;
	
	FRAM_CircularBuffer_pop(fram_buffer,&var_size);//get var size
	
	for_N(i,var_size)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&helper_var);
		temp_value |= helper_var << ( 8 * i ) ;
	}
	
	FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	while(string_buffer[sb_idx++])
		FRAM_CircularBuffer_pop(fram_buffer,&string_buffer[sb_idx]);
	
	Paint_DrawString_EN(16*4,16*line*3      -27 + (line - 1)*3, "                  ",&Font16,BLACK,WHITE);
	Paint_DrawString_EN(16*4,16*line*3 + 32 -27 + (line - 1)*3,"                  ",&Font16,BLACK,WHITE);
	
	Paint_DrawString_EN(16*4,16*line*3      -27 + (line - 1)*3, string_buffer,&Font16,BLACK,WHITE);
	
	sprintf(number_str_buffer,"0x%08x", temp_value);
	
	Paint_DrawString_EN(16*4,16*line*3 + 32 -27 + (line - 1)*3 , number_str_buffer,&Font16,BLACK,WHITE);
	//Paint_DrawNum(16*4,16*line*3 + 32 -27 + (line - 1)*3 , temp_value,&Font16,BLACK,WHITE);
}

void MENU_printDataPage()
{
	for_N(i,5)
		MENU_printData(i + 1);
}

void MENU_printBrowser_Menu()
{
	Paint_Clear(BLACK);
	MENU_Browser_printHeaderBody();
	for_N(i,5)
		MENU_Browser_printDataCell(i);
	MENU_Browser_printBottomBody();
}

void MENU_Browser_nextBreakpoint()
{
	DISABLE_BUTTONS();
	goToNextLoadBreakpoint();
	MENU_updateBreakpointNumber();
	MENU_printDataPage();
	ENABLE_BUTTONS();
}

void MENU_Browser_nextData()
{
	DISABLE_BUTTONS();
	MENU_printDataPage();
	ENABLE_BUTTONS();
}

//initializng functions

void MENU_init_ForwardingMenu()
{
	MENU_initMenu( &forwarding_menu_,
				   forwarding_menu_pages,
				   2,
				   3,
				   (MENU_Point){ 3 , 7  } ,
				   (MENU_Point){ 18, 11 } ,
				   (MENU_State){ 0 }  );
}

//initializng functions END

void MENU_browsingData()
{
	resetLoadBreakpoint();
	MENU_printBrowser_Menu();
	MENU_printDataPage();
	volatile uint8_t curr_pos = 0;
	while(1)
	{
		if(button_down(RIGHT_BUTTON_MASK))
		{
			if(curr_pos == 1)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos == 0)
			{
				Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,RED,WHITE);
				++curr_pos;
			}
		}
		else if(button_down(OK_BUTTON_MASK))
		{
			Paint_DrawString_EN(0,LCD_HEIGHT - 16,"Go next data",&Font16,BLACK,WHITE);
			Paint_DrawString_EN(0,LCD_HEIGHT - 32,"Go next breakpoint",&Font16,RED,WHITE);
			switch(curr_pos)
			{
				case 0:
					MENU_Browser_nextBreakpoint();
					break;
				case 1:
					MENU_Browser_nextData();
					break;
			}
			curr_pos = 0;
		}
		else if(button_down(BACK_BUTTON_MASK))
		{
			MENU_printChoiceMenu();
			break;
		}
	}
}

void MENU_printUpdatingDataMessage()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*1,16*11,"Updating data...",&Font16,BLACK,WHITE);
}

void MENU_loadNextBreakpoint()
{
	DISABLE_BUTTONS();
	MENU_printUpdatingDataMessage();
	resetSaveBreakpoint();
	COM_sendDebugSignal();
	COM_commandProcessor();//data are injected to fram now 
	
	goToNextSaveBreakpoint();//so i can switch this buffer
	
	MENU_printChoiceMenu();
	ENABLE_BUTTONS();
}

void MENU_serviceChoiceMenu()
{
	uint8_t curr_pos = 0;
	while(1)
	{
		if(button_down(RIGHT_BUTTON_MASK))
		{
			if(curr_pos == 1)
			{
				Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
				--curr_pos;
			}
		}
		else if(button_down(LEFT_BUTTON_MASK))
		{
			if(curr_pos == 0)
			{
				Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,BLACK,WHITE);
				Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,RED,WHITE);
				++curr_pos;
			}
		}
		else if(button_down(OK_BUTTON_MASK))
		{
			switch(curr_pos)
			{
				case 0:
					MENU_browsingData();
					break;
				case 1:
					MENU_loadNextBreakpoint();
					break;
			}
			curr_pos = 0;
		}
	}
}

void MENU_printChoiceMenu()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*1,16*10,"Browse data",&Font16,RED,WHITE);
	Paint_DrawString_EN(16*1,16*11,"Load next breakpoint",&Font16,BLACK,WHITE);
	Paint_DrawRectangle(16*1 - 5, 16*10 - 5, 16*6+16*9, 16*10 + 16*2,WHITE,2,DRAW_FILL_EMPTY);
	
	Paint_DrawString_EN(0,16*15,"Breakpoints: ",&Font16,BLACK,WHITE);
	Paint_DrawNum(16*10,16*15,breakpoints_total_nmbr,&Font16,BLACK,WHITE);
}

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

void MENU_drawRectangle(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end)
{
	Paint_DrawRectangle(x_start, y_start, x_end , y_end, WHITE, 2, DRAW_FILL_EMPTY);
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
	for_N( i ,opt->lines_nmbr_ )
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

void MENU_updateMenu(const MENU_Menu * const menu, const DIRECTION direction)
{
	__update_Text(menu , direction);
	//__print_Scrollbar( menu, direction );
	//__update_State( menu, direction, options_printed );
}

void MENU_printStartMenu()
{
	Paint_Clear(BLACK);
	Paint_DrawString_EN(16*6,16*10,"Start",&Font16,RED,WHITE);
	Paint_DrawRectangle(16*6 - 5,16*10 - 5,16*6+16*4,16*10 + 16,WHITE,2,DRAW_FILL_EMPTY);
}

void MENU_serviceStartMenu()
{
	ENABLE_BUTTONS();
	while(!button_down(OK_BUTTON_MASK));
	
	Paint_DrawString_EN(16*6,16*10,"Start",&Font16,BLUE,WHITE);
	Paint_DrawRectangle(16*6 - 5,16*10 - 5,16*6+16*4,16*10 + 16,WHITE,2,DRAW_FILL_EMPTY);
	
	DISABLE_BUTTONS();
	COM_sendDebugSignal();//receiving data
	COM_commandProcessor();// i have guarannce that data receiving will end until next line
	ENABLE_BUTTONS();
}

void MENU_navigate()
{
	if(start_flag == 0)
	{
		MENU_printStartMenu();
		MENU_serviceStartMenu();
		start_flag = 1;
	}
	else 
	{
		MENU_printChoiceMenu();
		MENU_serviceChoiceMenu();
	}
}
