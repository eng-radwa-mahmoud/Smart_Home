                                                  /* Radwa Mahmoud */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

typedef struct
{
	u8*  Channel;
	u16* Result;
	u8   Chain_Size;
	void (*NotificationFunc)(void);

}Chain_t;

void ADC_voidInit(void);
u8 ADC_u8GetDigitalValueSync(u8 Copy_u8ChannelNum,u16 *Copy_pu16ReturnedDigitalValue);

u8 ADC_u8StartConversionAsync(u8 Copy_u8ChannelNum,u16 *Copy_pu16Reading,void (*Copy_pvNotificationFunction)(void));

u8 ADC_u8StartChainAsync(Chain_t * Copy_Chain);

#endif /* ADC_INTERFACE_H_ */
