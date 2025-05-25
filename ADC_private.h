                                                 /* Radwa Mahmoud */

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

#define IDLE   0
#define BUSY   1

/* register defination */
#define ADC_u8_SFIOR_REG               *((volatile u8*)0x50)
#define ADC_u8_ADMUX_REG               *((volatile u8*)0x27)
#define ADMUX_REFS1                     7
#define ADMUX_REFS0                     6
#define ADMUX_ADLAR                     5

#define ADC_u8_ADCSRA_REG              *((volatile u8*)0x26)
#define ADCSRA_ADEN                     7
#define ADCSRA_ADSC                     6
#define ADCSRA_ADATE                    5
#define ADCSRA_ADIF                     4
#define ADCSRA_ADIE                     3
#define ADCSRA_ADPS2                    2
#define ADCSRA_ADPS1                    1
#define ADCSRA_ADPS0                    0

#define SINGLE_CHANNEL_ASYNCH           0
#define CHAIN_ASYNCH                    1





#define ADC_u8_ADCH_REG                *((volatile u8*)0x25)
#define ADC_u8_ADCL_REG                *((volatile u8*)0x24)

#define ADC_u16_ADC_REG                *((volatile u16*)0x24)


#endif /* ADC_PRIVATE_H_ */
