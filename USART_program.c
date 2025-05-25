#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "USART_interface.h"
#include "USART_private.h"
#include "USART_config.h"

static void (*USART_pvReceiveCallBack)(void) = NULL;
static u8* USART_pu8ReceivedByte = NULL;

void USART_voidInit(void)
{
	UCSRC =  0b10000110;
	CLR_BIT(UCSRB,UCSRB_RXCIE);
	CLR_BIT(UCSRB,UCSRB_TXCIE);
	CLR_BIT(UCSRB,UCSRB_UDRIE);
	SET_BIT(UCSRB,UCSRB_RXEN);
	SET_BIT(UCSRB,UCSRB_TXEN);
	UBRRL = 51;
}


void USART_voidSend(u8 Copy_u8Data)
{
	while(GET_BIT(UCSRA,UCSRA_UDRE) == 0)
		{
			asm("NOP");
		}
	UDR = Copy_u8Data;
}

void USART_SendString(u8* Copy_pu8Character)
{
	u8 counter = 0;
	while(Copy_pu8Character[counter]!='\0')
	{
		USART_voidSend(Copy_pu8Character[counter]);
		counter++;
	}
	USART_voidSend('\0');
}

u8 USART_u8Receive(void)
{
	while(GET_BIT(UCSRA,UCSRA_RXC) == 0)
	{
		asm("NOP");
	}
	u8 result = UDR;
	return result;
}
void USART_u8ReceiveAsynch(void (*PtrFunc)(void),u8* Copy_pu8ReceievedData)
{
	/* Enable receive interrupt */
	SET_BIT(UCSRB,UCSRB_RXCIE);
	/* Init global variables */
	USART_pu8ReceivedByte = Copy_pu8ReceievedData;
	/* Set call back */
	USART_pvReceiveCallBack = PtrFunc;
}

void __vector_13 (void) __attribute__((signal));
void __vector_13 (void)
{
	*USART_pu8ReceivedByte = UDR;
	if(USART_pvReceiveCallBack!=NULL)
	{
		USART_pvReceiveCallBack();
	}
}
