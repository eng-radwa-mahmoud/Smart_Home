/*LIB Layer*/
#include"STD_TYPES.h"
#include "BIT_MATH.h"
#define F_CPU  8000000UL
#include <avr/delay.h>

/*MCAL*/
#include "DIO_interface.h"

/*HAL*/
#include "LCD_interface.h"
#include "LCD_private.h"
#include "LCD_config.h"


void LCD_voidInit(void)
{
	_delay_ms(35);
#if (LCD_u8_MODE == LCD_u8_8_BIT_MODE)
	/* Function Set Command */
	LCD_voidWriteCmnd(0b00111000);
#elif (LCD_u8_MODE == LCD_u8_4_BIT_MODE)
	/* Rs = 0 (Command Byte) */
	DIO_u8SetPinValue(LCD_u8_RS_PORT,LCD_u8_RS_PIN,DIO_u8_LOW);
	/* Rw = 0 (write Operation) */
	DIO_u8SetPinValue(LCD_u8_RW_PORT,LCD_u8_RW_PIN,DIO_u8_LOW);
	/* Send 4 Bits Seq. */
	PRIVATE_voidSetHalfPort(0b00100000);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
	/* Send Normal Command */
	LCD_voidWriteCmnd(0b00101000);
#else
	#error"Wrong Choice"
#endif

	_delay_us(40);
	LCD_voidWriteCmnd(0b00001111);
	_delay_us(40);
	LCD_voidWriteCmnd(0x01);
	_delay_ms(2);
	LCD_voidWriteCmnd(0b00000110);
}


void LCD_voidWriteChar(u8 Copy_u8Char)
{
	/* Rs = 1 (Data Byte) */
	DIO_u8SetPinValue(LCD_u8_RS_PORT,LCD_u8_RS_PIN,DIO_u8_HIGH);
	/* Rw = 0 (write Operation) */
	DIO_u8SetPinValue(LCD_u8_RW_PORT,LCD_u8_RW_PIN,DIO_u8_LOW);
#if (LCD_u8_MODE == LCD_u8_8_BIT_MODE)
	/* Send Data Byte to LCD */
	DIO_u8SetPortValue(LCD_u8_DATA_PORT,Copy_u8Char);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
#elif (LCD_u8_MODE == LCD_u8_4_BIT_MODE)
	/* Send Most Nibbles to LCD */
	PRIVATE_voidSetHalfPort(Copy_u8Char);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
	/* Send Least 4 bits to LCD */
	PRIVATE_voidSetHalfPort(Copy_u8Char<<4);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
#else
	#error "Wrong Choice"
#endif
}
void LCD_voidWriteCmnd(u8 Copy_u8Cmnd)
{
	/* Rs = 0 (Command Byte) */
	DIO_u8SetPinValue(LCD_u8_RS_PORT,LCD_u8_RS_PIN,DIO_u8_LOW);
	/* Rw = 0 (write Operation) */
	DIO_u8SetPinValue(LCD_u8_RW_PORT,LCD_u8_RW_PIN,DIO_u8_LOW);
#if (LCD_u8_MODE == LCD_u8_8_BIT_MODE)

	/* Send Data Byte to LCD */
	DIO_u8SetPortValue(LCD_u8_DATA_PORT,Copy_u8Cmnd);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
#elif (LCD_u8_MODE == LCD_u8_4_BIT_MODE)
	/* Send Most Nibbles to LCD */
	PRIVATE_voidSetHalfPort(Copy_u8Cmnd);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
	/* Send Least 4 bits to LCD */
	PRIVATE_voidSetHalfPort(Copy_u8Cmnd<<4);
	/* Enable Pulse */
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_HIGH);
	_delay_us(1);
	DIO_u8SetPinValue(LCD_u8_EN_PORT,LCD_u8_EN_PIN,DIO_u8_LOW);
	#else
	#error "Wrong Choice"
#endif
}

void LCD_voidPrintString (u8* Copy_pu8String)
{
	u8 Local_u8String=0;
	while((Copy_pu8String[Local_u8String])>0)
	{
		// Write Character on LCD.
		LCD_voidWriteChar(Copy_pu8String[Local_u8String]);
		// Delay to let the LCD show the character.
		_delay_ms(2);
		Local_u8String++ ;
	}
}
void LCD_voidPrintNumber (u8 Copy_pu8Number)
{
	 int i, rem, len = 0, n;
	 char str[5];

	    n = Copy_pu8Number;
	    while (n != 0)
	    {
	        len++;
	        n /= 10;
	    }
	    for (i = 0; i < len; i++)
	    {
	        rem = Copy_pu8Number% 10;
	        Copy_pu8Number = Copy_pu8Number / 10;
	        str[len - (i + 1)] = rem + '0';
	        LCD_voidWriteChar(rem+'0');
	    }

}
void LCD_u8GoToXY (u8 copyu8_xpos,u8 copyu8_ypos)
{
	u8 localu8_Address;
	if (copyu8_xpos==0)
		localu8_Address=copyu8_ypos;
	else if (copyu8_xpos==1)
		localu8_Address=copyu8_ypos+0x40;
	LCD_voidWriteCmnd(localu8_Address+128);
}
void PRIVATE_voidSetHalfPort(u8 Copy_u8Value)
{
	if (GET_BIT(Copy_u8Value,7))
	{
		DIO_u8SetPinValue(LCD_u8_D7_PORT,LCD_u8_D7_PIN,DIO_u8_HIGH);
	}
	else
	{
		DIO_u8SetPinValue(LCD_u8_D7_PORT,LCD_u8_D7_PIN,DIO_u8_LOW);
	}
	if (GET_BIT(Copy_u8Value,6))
	{
		DIO_u8SetPinValue(LCD_u8_D6_PORT,LCD_u8_D6_PIN,DIO_u8_HIGH);
	}
	else
	{
		DIO_u8SetPinValue(LCD_u8_D6_PORT,LCD_u8_D6_PIN,DIO_u8_LOW);
	}
	if (GET_BIT(Copy_u8Value,5))
	{
		DIO_u8SetPinValue(LCD_u8_D5_PORT,LCD_u8_D5_PIN,DIO_u8_HIGH);
	}
	else
	{
		DIO_u8SetPinValue(LCD_u8_D5_PORT,LCD_u8_D5_PIN,DIO_u8_LOW);
	}
	if (GET_BIT(Copy_u8Value,4))
	{
		DIO_u8SetPinValue(LCD_u8_D4_PORT,LCD_u8_D4_PIN,DIO_u8_HIGH);
	}
	else
	{
		DIO_u8SetPinValue(LCD_u8_D4_PORT,LCD_u8_D4_PIN,DIO_u8_LOW);
	}
}
