/*
 * EEPROM_interface.h
 *
 *  Created on: Nov 19, 2021
 *      Author: Waheed
 */

#ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

u8 EEPROM_WriteByte(u16 copy_u8EEPROMHWAdd, u8 copy_u8Data);
u8 EEPROM_ReadByte(u8* copy_u8ReadData, u16 copy_u8EEPROMHWAdd);

#define FIRST_USER_START_ADD        10
#define SECOND_USER_START_ADD       14
#define THIRD_USER_START_ADD        18

#define NUMBER_OF_USERS             3


#endif /* EEPROM_INTERFACE_H_ */
