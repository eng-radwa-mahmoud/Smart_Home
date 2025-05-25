                                                /* Radwa Mahmoud */
#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include"ADC_interface.h"
#include"ADC_config.h"
#include"ADC_private.h"

static u16 *ADC_pu16Reading = NULL;
static void (*ADC_pvCallBackNotificationFunction)(void) = NULL;
u8 ADC_u8BusyState = IDLE;

static u8*  ADC_pu8ChainChannelArr = NULL;   // Global variable to carry chain array
static u8   ADC_u8ChainSize;
static u16* ADC_pu16ChainResultArr = NULL;
static u8   ADC_u8ChainConversionIndex = 0;

static u8 ADC_u8ISRSource;

void ADC_voidInit(void)
{
	/* select Vref = Avcc */
	CLR_BIT(ADC_u8_ADMUX_REG,ADMUX_REFS1);
	SET_BIT(ADC_u8_ADMUX_REG,ADMUX_REFS0);
	/* Left adjustment */
	CLR_BIT(ADC_u8_ADMUX_REG,ADMUX_ADLAR);
	/* Single conversion */
	CLR_BIT(ADC_u8_ADCSRA_REG,5);
	/* set prescalar to CLK/64 */
	SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADPS2);
	SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADPS1);
	CLR_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADPS0);
	/* ADC Enable */
	SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADEN);

}


u8 ADC_u8GetDigitalValueSync(u8 Copy_u8ChannelNum,u16 *Copy_pu16ReturnedDigitalValue)
{
	u8 local_u8ErrorState=STD_TYPES_OK;

u32 local_u32TimerOutCounter=0;

if(ADC_u8BusyState==IDLE)
{
   if((Copy_u8ChannelNum<=31)&&(Copy_pu16ReturnedDigitalValue!= NULL))
  {   ADC_u8BusyState = BUSY;
	/* Clear channel region */
	ADC_u8_ADMUX_REG &= 0b11100000;
	/* Select channel */
	ADC_u8_ADMUX_REG |= Copy_u8ChannelNum;
	/* start conversion */
    SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADSC);
    /* wait for the flag ( POLLING ) */
    while(!GET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIF)&&(local_u32TimerOutCounter!=ADC_u32TimeOut))
    {
    	local_u32TimerOutCounter++;
    }
    if (local_u32TimerOutCounter==ADC_u32TimeOut)
    {
    	/* loop is broken due to reaching time out */
    	local_u8ErrorState = STD_TYPES_NOK;
    }
    else
    {

    	/* Loop is broken because flag is raised */
    	/*Clear flag  */
    	SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIF);
    	/* read the digital value */
    	*Copy_pu16ReturnedDigitalValue = ADC_u16_ADC_REG;
    	ADC_u8BusyState = IDLE;
    }
  }

    else
    {
    	local_u8ErrorState=STD_TYPES_NOK;
    }
}

else

{
	local_u8ErrorState=STD_TYPES_NOK;
}

return local_u8ErrorState;
}

u8 ADC_u8StartConversionAsync(u8 Copy_u8ChannelNum,u16 *Copy_pu16Reading,void (*Copy_pvNotificationFunction)(void))
{ u8 Local_u8ErrorState =STD_TYPES_OK;
if(ADC_u8BusyState==IDLE)
{

	if((Copy_pu16Reading==NULL)||(Copy_pvNotificationFunction==NULL))
	{
		Local_u8ErrorState=STD_TYPES_NOK;
	}
	else
	{
    	ADC_u8BusyState = BUSY;
    	/* Make ISR source single channel asynch */
    	ADC_u8ISRSource = SINGLE_CHANNEL_ASYNCH;
		/* Initialize the reading variable globally */
		ADC_pu16Reading = Copy_pu16Reading;
		/* Initialize the callback notification function globally */
	    ADC_pvCallBackNotificationFunction = Copy_pvNotificationFunction;
		/* Clear channel region */
		ADC_u8_ADMUX_REG &= 0b11100000;
		/* Select channel */
		ADC_u8_ADMUX_REG |= Copy_u8ChannelNum;
		/* start conversion */
	    SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADSC);
	    /* Enable ADC interrupt */
	    SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIE);
	}

}
else
{
   Local_u8ErrorState = STD_TYPES_NOK;
}

	return Local_u8ErrorState;
}

u8 ADC_u8StartChainAsync(Chain_t* Copy_Chain)
{
	u8 Local_u8ErrorState = STD_TYPES_OK;
	/* Check null pointer */
	if(Copy_Chain == NULL)
	{
		Local_u8ErrorState = STD_TYPES_NOK;
	}
	else
		{
			if(ADC_u8BusyState == IDLE)
			{
				/* Make ADC busy*/
				ADC_u8BusyState = BUSY;
		    	/* Make ISR source channel asynch */
		    	ADC_u8ISRSource = CHAIN_ASYNCH;
				/* Initialize chain channel array */
				ADC_pu8ChainChannelArr = Copy_Chain->Channel;
				/* Init Result array */
				ADC_pu16ChainResultArr = Copy_Chain->Result;
				/* Init chain size */
				ADC_u8ChainSize = Copy_Chain->Chain_Size;
				/* Init Notification func */
				ADC_pvCallBackNotificationFunction = Copy_Chain->NotificationFunc;
				/* Init Chain index to zero */
				ADC_u8ChainConversionIndex = 0;

				/* Set required channel */
				ADC_u8_ADMUX_REG &= 0b11100000;
				ADC_u8_ADMUX_REG |= ADC_pu8ChainChannelArr[ADC_u8ChainConversionIndex];
				/* Start conversion */
				SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADSC);
				/* Enable interrupt */
				SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIE);
			}
			else
			{
				Local_u8ErrorState = STD_TYPES_NOK;
			}
		}
	return Local_u8ErrorState;
}

void __vector_16 (void) __attribute((signal));
void __vector_16 (void)
{
	if (ADC_u8ISRSource == SINGLE_CHANNEL_ASYNCH)
	{
	/* read ADC result */
	*ADC_pu16Reading = ADC_u16_ADC_REG;
	/* Make ADC state be IDLE */
	ADC_u8BusyState = IDLE;
	/* Invoke callback notification function */
	ADC_pvCallBackNotificationFunction();
	/* Disable ADC conversion complete interrupt */
	CLR_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIE);
	}
	else if (ADC_u8ISRSource == CHAIN_ASYNCH)   // ISR Source is chain source
	{
		/* read current conversion */
		ADC_pu16ChainResultArr[ADC_u8ChainConversionIndex] = ADC_u16_ADC_REG;
		/* Increment Index */
		ADC_u8ChainConversionIndex++;
		/* Check chain is finished or not */
		if (ADC_u8ChainConversionIndex == ADC_u8ChainSize)
		{
			/* Chain is finished */
			/* Make ADC state be IDLE */
			ADC_u8BusyState = IDLE;
			/* Invoke callback notification func */
			ADC_pvCallBackNotificationFunction();
			/* Disable Interrupt */
			CLR_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADIE);

		}
		else
		{
			/* Chain is not finished */
			/* Set new required channel */
			/* Set required channel */
			ADC_u8_ADMUX_REG &= 0b11100000;
			ADC_u8_ADMUX_REG |= ADC_pu8ChainChannelArr[ADC_u8ChainConversionIndex];
			/* Start conversion */
			SET_BIT(ADC_u8_ADCSRA_REG,ADCSRA_ADSC);
		}
	}
}



