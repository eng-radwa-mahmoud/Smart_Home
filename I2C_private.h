/*
 * I2C_private.h
 *
 *  Created on: Jul 24, 2023
 *      Author: DELL
 */

#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_

#define TWCR          *((volatile u8*)0x56)
#define TWCR_TWINT        7
#define TWCR_TWEA         6
#define TWCR_TWSTA        5
#define TWCR_TWSTO        4
#define TWCR_TWWC         3
#define TWCR_TWEN         2
#define TWCR_TWIE         0

#define TWDR          *((volatile u8*)0x23)

#define TWAR          *((volatile u8*)0x22)

#define TWSR          *((volatile u8*)0x21)
#define TWSR_TWPS1        1
#define TWSR_TWPS0        0


#define TWBR          *((volatile u8*)0x20)

#define START_ACK                    0x08
#define REPEATED_START_ACK           0x10
#define SLAVE_ADD_AND_WR_ACK         0x18
#define SLAVE_ADD_AND_RD_ACK         0x40
#define MSTR_WR_BYTE_ACK             0x28
#define MSTR_RD_BYTE_WITH_ACK        0x50
#define MSTR_RD_BYTE_WITH_NACK       0x58
#define SLAVE_ADD_REC_RD_REQ         0xA8
#define SLAVE_ADD_REC_WR_REQ       	 0x60
#define SLAVE_DATA_REC               0x80
#define SLAVE_BYTE_TRANSMITTED       0xB8



#endif /* I2C_PRIVATE_H_ */
