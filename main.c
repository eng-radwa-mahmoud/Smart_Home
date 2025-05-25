/*
 * main.c
 *
 *  Created on: Aug 2, 2023
 *      Author: DELL
 */
#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>

#include "DIO_interface.h"
#include "USART_private.h"
#include "ADC_interface.h"
#include "I2C_interface.h"
#include "KPD_interface.h"
#include "LCD_interface.h"
#include "TIMER_interface.h"
#include "USART_interface.h"
#include "EEPROM_interface.h"

#define  SREG      *((volatile u8*)0x5F)

static void WelcomeScreen(void);
static void System_voidInit(void);
static void USART_voidVoiceReceived(void);
static void EEPROM_voidReadUserIds(u8 StartAddress);
static void ControlHome(void);
static void ControlLight(void);
static void CheckPassword(void);
static void GetPassword(void);

static u8 USART_u8ReadByte;
static u8 ReceivedVoice[25];
static u8 ControlLightFlag = 0;
static u8 UserIDs[NUMBER_OF_USERS][5];
static u8 EnteredID[15] ;


void main(void)
{
	u8 StartAddress = FIRST_USER_START_ADD;
	/* Enable GIE */
	SET_BIT(SREG,7);
	System_voidInit();
	EEPROM_voidReadUserIds(StartAddress);
	WelcomeScreen();
	GetPassword();
	CheckPassword();

	while(1)
	{

		if (ControlLightFlag == 1)
		{
			ControlLight();
		}
	}
}

void System_voidInit(void)
{
	DIO_u8INIT();
	LCD_voidInit();
	ADC_voidInit();
	I2C_voidInitMaster(0);
	TIMER0_voidInit();
	TIMER1_voidInit();
	TIMER1_voidSetICRTopValue(20000);
	USART_voidInit();
}

void WelcomeScreen(void)
{
	LCD_u8GoToXY(0,0);
	LCD_voidPrintString("Welcome");
	LCD_u8GoToXY(1,0);
	LCD_voidPrintString("Enter ID:");
}

void USART_voidVoiceReceived(void)
{
	static u8 counter = 0;
	if (USART_u8ReadByte == '\n')
	{
		ReceivedVoice[counter] = '\0';
		counter = 0;
		ControlHome();
	}
	else
	{
		ReceivedVoice[counter] = USART_u8ReadByte;
		counter++;
	}
}

void ControlHome(void)
{
	if (!(strcmp(ReceivedVoice,"turn on light")))
	{
		TIMER0_voidDisconnectOC0Pin();
		DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_HIGH);
		ControlLightFlag = 0;
	}
	else if (!(strcmp(ReceivedVoice,"turn off light")))
	{
		TIMER0_voidDisconnectOC0Pin();
		DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_LOW);
		ControlLightFlag = 0;
	}
	else if (!(strcmp(ReceivedVoice,"control light")))
	{
		ControlLightFlag = 1;
		TIMER0_voidSetFastPwmNonInverting();
	}
	else if(!(strcmp(ReceivedVoice,"open the door")))
	{
		TIMER1_voidSetChannelACompMatchValue(750);
	}
	else if(!(strcmp(ReceivedVoice,"close the door")))
	{
		TIMER1_voidSetChannelACompMatchValue(2500);
	}
}

void ControlLight(void)
{
	u16 Local_u16DigitalValue = 0;
	u16 Comp_u16MatchValue = 0;
	ADC_u8GetDigitalValueSync(7,&Local_u16DigitalValue);
	Comp_u16MatchValue = ((255*(1024-Local_u16DigitalValue))/1024)-255;
	TIMER0_voidSetCompMatchValue(Comp_u16MatchValue);
}

void GetPassword(void)
{
	u8 i = 0;
	u8 PressedKey = 'd';
	memset(EnteredID, '\0', sizeof(EnteredID)/sizeof(EnteredID[0]));
	while(1)
	{
		KPD_u8GetKey(&PressedKey);
		if(PressedKey != KPD_u8_KEY_NOT_PRESSED)
		{
			if (PressedKey != '+')
			{
				EnteredID[i] = PressedKey;
				LCD_voidWriteChar(PressedKey+'0');
				i++;

			}
			else
			{
				CheckPassword();
				break;
			}
		}
	}
}

void CheckPassword(void)
{
	static u8 TryNumber = 0;
	u8 SuccessRegister = 0;
	for (u8 i=0;i<3;i++)
	{
		if(strcmp(EnteredID,UserIDs[i]))
		{

		}
		else
		{
			LCD_voidWriteCmnd(0b00000001);
			LCD_u8GoToXY(0,0);
			LCD_voidPrintString("Sucess register");
			_delay_ms(2000);
			LCD_voidWriteCmnd(0b00000001);
			LCD_u8GoToXY(0,0);
			LCD_voidPrintString("Wait for");
			LCD_u8GoToXY(1,0);
			LCD_voidPrintString("voice commands...");
			USART_u8ReceiveAsynch(USART_voidVoiceReceived,&USART_u8ReadByte);
			SuccessRegister = 1;
			break;
		}
	}
	if (SuccessRegister == 0 && TryNumber<3)
	{
		TryNumber++;
		LCD_voidWriteCmnd(0b00000001);
		LCD_u8GoToXY(0,0);
		LCD_voidPrintString("Wrong Password");
		LCD_u8GoToXY(1,0);
		LCD_voidPrintString("Try Again...");
		_delay_ms(1500);
		LCD_voidWriteCmnd(0b00000001);
		LCD_u8GoToXY(0,0);
		LCD_voidPrintString("Enter ID:");
		GetPassword();

	}
	else if (SuccessRegister == 0 && TryNumber>=3)
	{
		TryNumber = 0;
		LCD_voidWriteCmnd(0b00000001);
		LCD_u8GoToXY(0,0);
		LCD_voidPrintString("Try again");
		LCD_u8GoToXY(1,0);
		LCD_voidPrintString("after 5 seconds...");
		_delay_ms(5000);
		LCD_voidWriteCmnd(0b00000001);
		LCD_u8GoToXY(0,0);
		LCD_voidPrintString("Enter ID:");
		GetPassword();


	}
}

void EEPROM_voidReadUserIds(u8 StartAddress)
{

	for(u8 x = 0;x<3;x++)
	{
		for(u8 y=0;y<4;y++)
		{
			EEPROM_ReadByte(&UserIDs[x][y],StartAddress++);
		}
	}
}
