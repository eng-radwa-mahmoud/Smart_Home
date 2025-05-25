/*
 * I2C_interface.h
 *
 *  Created on: Jul 24, 2023
 *      Author: DELL
 */

#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_


typedef   enum
{
	NoError,
	StartConditionError,
	RepeatedStartError,
	SlaveAddressWithWriteErr,
	SlaveAddressWithReadErr,
	MasterWriteDataByteErr,
	MasterReadDataByteErr,

}I2C_ErrSatus;

void I2C_voidInitMaster(u8 Copy_u8SalveAddress); // pass zero in the slave address argument if master will not be addressed
void I2C_voidInitSlave(u8 Copy_u8SlaveAddress);

I2C_ErrSatus I2C_SendStartCondition(void);
I2C_ErrSatus I2C_SendRepeatedStart(void);
I2C_ErrSatus I2C_SendSlaveAddressWithWrire(u8 Copy_u8SlaveAddress);
I2C_ErrSatus I2C_SendSlaveAddressWithRead(u8 Copy_u8SlaveAddress);
I2C_ErrSatus I2C_MasterWriteDataByte(u8 Copy_u8DataByte);
I2C_ErrSatus I2C_MasterReadDataByte(u8 *Copy_pu8DataByte);
void I2C_SendStopCondition(void);


#endif /* I2C_INTERFACE_H_ */
