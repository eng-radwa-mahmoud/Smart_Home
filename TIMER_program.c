/*
 * TIMER_program.c
 *
 *  Created on: Sep 26, 2022
 *      Author: DELL
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "TIMER_interface.h"
#include "TIMER_private.h"
#include "TIMER_config.h"

static void (*TIMER0_pvCallBackFunc)(void)=NULL;

void TIMER0_voidInit(void)
{
	/* Choose Fast PWM mode mode */
	SET_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);

//	/*Output compare match interrupt enable */
//   SET_BIT(TIMSK,TIMSK_OCIE0);

////	/* Set compare match value to 64 */
//	OCR0 = 64;

	/* Clear on compare ... Set on Top */
//	SET_BIT(TCCR0,5);
//	CLR_BIT(TCCR0,4);

	/* Prescalar: Division by 8 */
 	CLR_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);


}

void TIMER0_voidDisconnectOC0Pin(void)
{
	CLR_BIT(TCCR0,5);
	CLR_BIT(TCCR0,4);
}

void TIMER0_voidSetFastPwmNonInverting()
{
	SET_BIT(TCCR0,5);
	CLR_BIT(TCCR0,4);
}

void TIMER1_voidInit(void)
{

	/* Choose Fast PWM & Top=ICR1 mode */
	CLR_BIT(TCCR1A,TCCR1A_WGM10);
	SET_BIT(TCCR1A,TCCR1A_WGM11);
	SET_BIT(TCCR1B,TCCR1B_WGM12);
	SET_BIT(TCCR1B,TCCR1B_WGM13 );

	/* Set fast PWM non inverting mode */
	SET_BIT(TCCR1A,TCCR1A_COM1A1);
	CLR_BIT(TCCR1A,TCCR1A_COM1A0);


	/* Set prescalar divide by 8 */
 	CLR_BIT(TCCR1B,TCCR1B_CS10);
	SET_BIT(TCCR1B,TCCR1B_CS11);
	CLR_BIT(TCCR1B,TCCR1B_CS12);

}

void TIMER1_voidSetTimerValue(u16 Copy_u16Value)
{
	TCNT1 = Copy_u16Value;
}

u16 TIMER1_voidReadTimerValue(void)
{
	return TCNT1;
}

void TIMER0_voidSetCompMatchValue(u8 Copy_u8Value)
{
	OCR0 = Copy_u8Value;
}

void TIMER1_voidSetChannelACompMatchValue(u16 Copy_u16Value)
{
	OCR1A = Copy_u16Value;
}

void TIMER1_voidSetICRTopValue(u16 Copy_u16Value)
{
	ICR1  =Copy_u16Value;
}


u8 TIMER0_u8SetCallBack(void(*copy_pvCallBackFunc)(void))
{
	u8 Local_u8ErrorStatus =STD_TYPES_OK;
	if(copy_pvCallBackFunc!=NULL)
	{
		TIMER0_pvCallBackFunc = copy_pvCallBackFunc;
	}
	else
	{
		Local_u8ErrorStatus = STD_TYPES_NOK;
	}
	return Local_u8ErrorStatus;
}

void __vector_10 (void) __attribute__((signal));
void __vector_10 (void)
{
	if(TIMER0_pvCallBackFunc!=NULL)
	{
		TIMER0_pvCallBackFunc();
	}
}
