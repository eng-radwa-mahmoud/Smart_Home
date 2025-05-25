/*
 * TIMER_private.h
 *
 *  Created on: Sep 26, 2022
 *      Author: Radwa
 */

#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

#define TCCR0    *((volatile u8*)0x53) /* Timer0 control register*/
#define TCCR0_WGM00      6             /* Waveform generation mode bit0*/
#define TCCR0_WGM01      3             /* Waveform generation mode bit1*/
#define TCCR0_CS02       2             /* Prescalar bit2*/
#define TCCR0_CS01       1             /* Prescalar bit1*/
#define TCCR0_CS00       0             /* Prescalar bit0*/

#define OCR0    *((volatile u8*)0x5C)  /* output compare register */

#define TIMSK    *((volatile u8*)0x59) /* Timer mask */
#define TIMSK_OCIE0    1                /* Output compare 0 interrupt */

#define TCCR1A    *((volatile u8*)0x4F)
#define TCCR1A_WGM10       0             /* Waveform generation mode bit0*/
#define TCCR1A_WGM11       1             /* Waveform generation mode bit1*/
#define TCCR1A_COM1A1      7
#define TCCR1A_COM1A0      6

#define TCCR1B    *((volatile u8*)0x4e)
#define TCCR1B_WGM12       4             /* Waveform generation mode bit0*/
#define TCCR1B_WGM13       3             /* Waveform generation mode bit1*/
#define TCCR1B_CS12        2             /* Prescalar bit2*/
#define TCCR1B_CS11        1             /* Prescalar bit1*/
#define TCCR1B_CS10        0             /* Prescalar bit0*/

#define TCNT1      *((volatile u16*)0x4C)

#define ICR1       *((volatile u16*)0x46)

#define OCR1A      *((volatile u16*)0x4A)



#endif /* TIMER_PRIVATE_H_ */
