/*********************************************************************************/
/*                               Name : Karim Seif                               */
/*                               Date : 2/10/2021                                */
/*                               SWC  : KPD                                      */
/*                            version : 1.0                                      */
/*********************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#define F_CPU 8000000UL
#include <avr/delay.h>

#include "DIO_interface.h"

#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"

static u8 KPD_Au8RowPins[4]={KPD_u8_R1_PIN,KPD_u8_R2_PIN,KPD_u8_R3_PIN,KPD_u8_R4_PIN};
static u8 KPD_Au8RowPorts[4]={KPD_u8_R1_PORT,KPD_u8_R2_PORT,KPD_u8_R3_PORT,KPD_u8_R4_PORT};

static u8 KPD_Au8ColPins[4]={KPD_u8_C1_PIN,KPD_u8_C2_PIN,KPD_u8_C3_PIN,KPD_u8_C4_PIN};
static u8 KPD_Au8ColPorts[4]={KPD_u8_C1_PORT,KPD_u8_C2_PORT,KPD_u8_C3_PORT,KPD_u8_C4_PORT};

static u8 KPD_Au8Keys[4][4]= KPD_KEYS;

u8 KPD_u8GetKey(u8 * Copy_pu8ReturnedKey){
	u8 Local_u8ErrorState = STD_TYPES_OK;
	u8 Local_u8RowCounter,Local_u8ColCounter,Local_u8PinValue,Local_u8Flag = 0;
	if(Copy_pu8ReturnedKey != NULL)
	{
		/* Apply KPD Algo */
		*Copy_pu8ReturnedKey = KPD_u8_KEY_NOT_PRESSED;
		/* Activate Each row then Check The Columns */
		for(Local_u8RowCounter = 0; Local_u8RowCounter < 4 ; Local_u8RowCounter++){
			/* Activate the ROW */
			DIO_u8SetPinValue(KPD_Au8RowPorts[Local_u8RowCounter],KPD_Au8RowPins[Local_u8RowCounter],DIO_u8_LOW);
			for(Local_u8ColCounter = 0 ; Local_u8ColCounter < 4 ; Local_u8ColCounter++)
			{
				/* Check Columns Values */
				DIO_u8GetPinValue(KPD_Au8ColPorts[Local_u8ColCounter],KPD_Au8ColPins[Local_u8ColCounter],&Local_u8PinValue);
				/* Check if pin value is Low */
				if(Local_u8PinValue == DIO_u8_LOW)/* Key Pressed */
				{
					/* Debouncing */
					_delay_ms(20);
					/* Wait for Pin Goes High */
					while(Local_u8PinValue == DIO_u8_LOW){
						DIO_u8GetPinValue(KPD_Au8ColPorts[Local_u8ColCounter],KPD_Au8ColPins[Local_u8ColCounter],&Local_u8PinValue);
					}
					/* Return Key Value */
					*Copy_pu8ReturnedKey = KPD_Au8Keys[Local_u8RowCounter][Local_u8ColCounter];
					/* Terminate Cols Loop */
					Local_u8Flag = 1;
					break;
				}
			}
			/* Deactivate The Row */
			DIO_u8SetPinValue(KPD_Au8RowPorts[Local_u8RowCounter],KPD_Au8RowPins[Local_u8RowCounter],DIO_u8_HIGH);
			if(Local_u8Flag != 0)
			{
				break;
			}
		}
	}
	else
	{
		Local_u8ErrorState = STD_TYPES_NOK;
	}
	return Local_u8ErrorState;
}
