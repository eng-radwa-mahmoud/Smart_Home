/*
 * TIMER_interface.h
 *
 *  Created on: Sep 26, 2022
 *      Author: DELL
 */

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

void TIMER0_voidInit(void);
void TIMER0_voidSetCompMatchValue(u8 Copy_u8Value);
u8   TIMER0_u8SetCallBack(void(*copy_pvCallBackFunc)(void));
void TIMER0_voidDisconnectOC0Pin(void);
void TIMER0_voidSetFastPwmNonInverting(void);

void TIMER1_voidInit(void);
void TIMER1_voidSetICRTopValue(u16 Copy_u16Value);
void TIMER1_voidSetChannelACompMatchValue(u16 Copy_u16Value);
void TIMER1_voidSetTimerValue(u16 Copy_u16Value);
u16  TIMER1_voidReadTimerValue(void);




#endif /* TIMER_INTERFACE_H_ */
