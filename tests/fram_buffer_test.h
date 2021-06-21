/*
 * fram_buffer_test.h
 *
 * Created: 18/05/2021 13:32:40
 *  Author: Damian Wojcik
 */ 


#ifndef FRAM_BUFFER_TEST_H_
#define FRAM_BUFFER_TEST_H_

#include "../drivers/fram/fram_circular_buffer.h"
#include "../drivers/lcd/GUI_Paint.h"
#include "../program/core.h"

void injectTXT(FRAM_CircularBuffer bf, char* txt)
{
	while(*txt)
		FRAM_CircularBuffer_safePush(bf,*txt++);
	FRAM_CircularBuffer_safePush(bf,'\0');
}

void fram_test1()
{
	Paint_Clear(BLACK);
	uint8_t var1 = 5;
	char var1name[] = {"var1\0"};
	uint8_t varp = 9;
	char varpname[] = {"varp\0"};
	uint8_t var2 = 7;
	char var2name[] = {"var2\0"};
	uint8_t var3 = 43;
	char var3name[] = {"var3\0"};
		
	FRAM_CircularBuffer bf = FRAM_CircularBuffer_init(0,11);
	FRAM_CircularBuffer_safePush(bf,var1);
	injectTXT(bf,var1name);
	
	FRAM_CircularBuffer_safePush(bf,varp);
	injectTXT(bf,varpname);
	
	FRAM_CircularBuffer_reconfigure(bf,20,39);
	FRAM_CircularBuffer_safePush(bf,var2);
	injectTXT(bf,var2name);
	
	FRAM_CircularBuffer_reconfigure(bf,40,59);
	FRAM_CircularBuffer_safePush(bf,var3);
	injectTXT(bf,var3name);
	
	char temp[5];
	
	Paint_DrawNum(10,50,FRAM_readRandomByte(0),&Font16,BLACK,WHITE);
	FRAM_readText(temp,1);
	Paint_DrawString_EN(10,50+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,100,FRAM_readRandomByte(6),&Font16,BLACK,WHITE);
	FRAM_readText(temp,7);
	Paint_DrawString_EN(10,100+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,150,FRAM_readRandomByte(20),&Font16,BLACK,WHITE);
	FRAM_readText(temp,21);
	Paint_DrawString_EN(10,150+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,200,FRAM_readRandomByte(40),&Font16,BLACK,WHITE);
	FRAM_readText(temp,41);
	Paint_DrawString_EN(10,200+16,temp,&Font16,BLACK,WHITE);
}

void fram_test2()
{
	Paint_Clear(BLACK);
	uint8_t var1 = 5;
	char var1name[] = {"var1\0"};
	uint8_t varp = 9;
	char varpname[] = {"varp\0"};
	uint8_t var2 = 7;
	char var2name[] = {"var2\0"};
	uint8_t var3 = 43;
	char var3name[] = {"var3\0"};
		
	CORE_configureDebuggerFRAM(4);
	
	FRAM_CircularBuffer_safePush(fram_buffer,var1);
	injectTXT(fram_buffer,var1name);
	
	FRAM_CircularBuffer_safePush(fram_buffer,varp);
	injectTXT(fram_buffer,varpname);
	
	CORE_goToNextSaveBreakpoint();
	
	FRAM_CircularBuffer_safePush(fram_buffer,var2);
	injectTXT(fram_buffer,var2name);
	
	CORE_goToNextSaveBreakpoint();
	
	FRAM_CircularBuffer_safePush(fram_buffer,var3);
	injectTXT(fram_buffer,var3name);
	
	char temp[5];
	
	Paint_DrawNum(10,50,FRAM_readRandomByte(0),&Font16,BLACK,WHITE);
	FRAM_readText(temp,1);
	Paint_DrawString_EN(10,50+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,100,FRAM_readRandomByte(6),&Font16,BLACK,WHITE);
	FRAM_readText(temp,7);
	Paint_DrawString_EN(10,100+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,150,FRAM_readRandomByte(8192),&Font16,BLACK,WHITE);
	FRAM_readText(temp,8193);
	Paint_DrawString_EN(10,150+16,temp,&Font16,BLACK,WHITE);
	
	Paint_DrawNum(10,200,FRAM_readRandomByte(16384),&Font16,BLACK,WHITE);
	FRAM_readText(temp,16384 + 1);
	Paint_DrawString_EN(10,200+16,temp,&Font16,BLACK,WHITE);
}

void fram_test3()
{
	Paint_Clear(BLACK);
	uint8_t var1 = 78;
	char var1name[] = {"var1\0"};
	uint8_t varp = 61;
	char varpname[] = {"varp\0"};
	uint8_t var2 = 7;
	char var2name[] = {"var2\0"};
	uint8_t var3 = 43;
	char var3name[] = {"var3\0"};
		
	FRAM_CircularBuffer bf = FRAM_CircularBuffer_init(0,3);
	FRAM_CircularBuffer_safePush(bf,var1);
	injectTXT(bf,var1name);
	
	FRAM_CircularBuffer_safePush(bf,varp);
	
	injectTXT(bf,varpname);
	
	FRAM_CircularBuffer_safePush(bf,255);
	
	char temp[5];
	
	uint8_t tmp = 0;
	
	//FRAM_CircularBuffer_pop(bf,&tmp);
	
	for_N(i,18)
	{
		FRAM_CircularBuffer_pop(bf,&tmp);
		Paint_DrawNum(10,16*i,tmp,&Font16,BLACK,WHITE);
	}
	
	for_N(i,18)
	{
		FRAM_CircularBuffer_pop(bf,&tmp);
		Paint_DrawNum(60,16*i,tmp,&Font16,BLACK,WHITE);
	}
	
	//Paint_DrawNum(10,50,tmp,&Font16,BLACK,WHITE);
	//FRAM_readText(temp,1);
	//Paint_DrawString_EN(10,50+16,temp,&Font16,BLACK,WHITE);
	//
	//Paint_DrawNum(10,100,FRAM_readRandomByte(6),&Font16,BLACK,WHITE);
	//FRAM_readText(temp,7);
	//Paint_DrawString_EN(10,100+16,temp,&Font16,BLACK,WHITE);
}

void fram_test4()
{
	Paint_Clear(BLACK);
	uint8_t var1 = 5;
	char var1name[] = {"var1\0"};
	uint8_t varp = 9;
	char varpname[] = {"varp\0"};
	uint8_t var2 = 7;
	char var2name[] = {"var2\0"};
	uint8_t var3 = 43;
	char var3name[] = {"var3\0"};
	
	char temp[20];
	uint8_t tmp = 0;
	
	CORE_configureDebuggerFRAM(4);
	
	FRAM_CircularBuffer_safePush(fram_buffer,var1);
	injectTXT(fram_buffer,var1name);
	FRAM_CircularBuffer_safePush(fram_buffer,varp);
	injectTXT(fram_buffer,varpname);
	
	
	CORE_resetLoadBreakpoint();
	for_N(i,6)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&tmp);
		Paint_DrawNum(60,16*i,tmp,&Font16,BLACK,WHITE);
	}
	
	
	CORE_goToNextSaveBreakpoint();
	sprintf(temp,"brk save :%d",current_breakpoint_to_save);
	Paint_DrawString_EN(70,100,temp,&Font16,BLACK,WHITE);
	FRAM_CircularBuffer_safePush(fram_buffer,var2);
	injectTXT(fram_buffer,var2name);
	UPDATE_SAVE_POS();
	
	CORE_goToNextLoadBreakpoint();
	sprintf(temp,"brk:%d",current_breakpoint_to_load);
	Paint_DrawString_EN(6,100,temp,&Font16,BLACK,WHITE);
	for_N(i,6)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&tmp);
		Paint_DrawNum(60,16*i + 116,tmp,&Font16,BLACK,WHITE);
	}
	
	
	CORE_goToNextSaveBreakpoint();
	FRAM_CircularBuffer_safePush(fram_buffer,var3);
	injectTXT(fram_buffer,var3name);
	
	
	CORE_resetLoadBreakpoint();
	sprintf(temp,"brk:%d",current_breakpoint_to_load);
	Paint_DrawString_EN(6,228,temp,&Font16,BLACK,WHITE);
	for_N(i,6)
	{
		FRAM_CircularBuffer_pop(fram_buffer,&tmp);
		Paint_DrawNum(60,16*i + 228,tmp,&Font16,BLACK,WHITE);
	}
	
	
	//goToNextLoadBreakpoint();
	
	
	//Paint_DrawNum(10,50,FRAM_readRandomByte(0),&Font16,BLACK,WHITE);
	//FRAM_readText(temp,1);
	//Paint_DrawString_EN(10,50+16,temp,&Font16,BLACK,WHITE);
		
	
	
}

#endif /* FRAM_BUFFER_TEST_H_ */