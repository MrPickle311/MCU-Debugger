/*
 * data_view.h
 *
 * Created: 19/06/2021 12:53:51
 *  Author: Damian Wójcik
 */ 


#ifndef DATA_VIEW_H_
#define DATA_VIEW_H_

#include "menu_gui.h"
#include "menu_objects.h"

volatile struct MENU_DataCell
{
	char**		name_lines_;
	uint32_t	value_;
	uint8_t		lines_count_;
};

typedef struct MENU_DataCell MENU_DataCell;

#define IN_CELLS_SCREEN_MAX_CAPACITY 6 //max count of cells screen can display

volatile struct MENU_DataView
{
	MENU_DataCell cells_ [IN_CELLS_SCREEN_MAX_CAPACITY];
	uint8_t		  cells_count_to_display_;
	uint8_t		  current_cell_to_display_;
	MENU_Area	  area_;
}current_data_view;

typedef struct MENU_DataView MENU_DataView;

#define DATA_VIEW_AT_CURRENT_CELL(data_view)	data_view->cells_[data_view->current_cell_to_display_]

#define NAME_FIRST_LINE_OFFSET	sizeof("Name:\0")
#define DATA_LINE_OFFSET		sizeof("Value:\0")

#define DATA_VIEW_NAME_X_START_POS(data_view)	AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) + NAME_FIRST_LINE_OFFSET
#define DATA_VIEW_VALUE_X_START_POS(data_view)	AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) + DATA_LINE_OFFSET

void MENU_nextCell(MENU_DataView* const data_view);

void MENU_displayCell( const MENU_DataView* const data_view ,
					   uint8_t					  y_start);

void MENU_displayDataView(MENU_DataView* const data_view);

uint8_t MENU_prepareVariabaleData(uint8_t cell_nmbr);//it prepares data to been able to be shown

void MENU_fillDataView(MENU_DataView* const data_view);


#endif /* DATA_VIEW_H_ */