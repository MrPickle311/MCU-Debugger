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

volatile struct MENU_DataView
{
	MENU_Area	  area_;
}current_data_view;

typedef struct MENU_DataView MENU_DataView;

#define NAME_FIRST_LINE_OFFSET	sizeof("Name:\0")
#define DATA_LINE_OFFSET		sizeof("Value:\0")

#define DATA_VIEW_NAME_X_START_POS(data_view)	AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) + NAME_FIRST_LINE_OFFSET
#define DATA_VIEW_VALUE_X_START_POS(data_view)	AREA_LEFT_X_LEFT_CELL_POS(data_view->area_) + DATA_LINE_OFFSET

void MENU_displayCell( const MENU_DataView* const data_view ,
					   uint8_t					  y_start);

void MENU_refreshDataView( MENU_DataView* const data_view);

void MENU_displayDataView( MENU_DataView* const data_view);

void MENU_initDataView( MENU_DataView* const data_view ,
					    MENU_Point   begin			   ,
					    MENU_Point   end			);

#endif /* DATA_VIEW_H_ */