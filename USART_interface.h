
#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H


void USART_voidInit(void);
void USART_voidSend(u8 Copy_u8Data);
u8   USART_u8Receive(void);
void USART_SendString(u8* Copy_pu8Character);
void USART_u8ReceiveAsynch(void (*PtrFunc)(void),u8* Copy_pu8ReceievedData);






#endif
