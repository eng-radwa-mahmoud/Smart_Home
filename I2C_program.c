/*
 * I2C_program.c
 *
 *  Created on: Jul 24, 2023
 *      Author: DELL
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"

#include "I2C_config.h"
#include "I2C_interface.h"
#include "I2C_private.h"


void I2C_voidInitMaster(u8 Copy_u8SalveAddress)
{
	/* Set clock freq to 400kbps */

	/* Set TWBR to 2 */
	TWBR = 2;
	/* Clear TWPS */
	CLR_BIT(TWSR,TWSR_TWPS1);
	CLR_BIT(TWSR,TWSR_TWPS0);
	/* Set address */
	if (Copy_u8SalveAddress != 0)
	{
		TWAR = Copy_u8SalveAddress<<1;
	}
	//SET_BIT(TWCR,TWCR_TWEA);
	/* Enable TWI */
	SET_BIT(TWCR,TWCR_TWEN);

}

void I2C_voidInitSlave(u8 Copy_u8SlaveAddress)
{
	/* Init slave address */
	TWAR = Copy_u8SlaveAddress<<1;
	/* Enable TWI */
	SET_BIT(TWCR,TWCR_TWEN);
}

I2C_ErrSatus I2C_SendStartCondition(void)
{
	I2C_ErrSatus Local_ErrStatus = NoError;
	/* Send start condition */
	SET_BIT(TWCR,TWCR_TWSTA);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0);

	if ((TWSR & 0xFB) != START_ACK)
	{
		Local_ErrStatus = StartConditionError;
	}

	return Local_ErrStatus;

}

I2C_ErrSatus I2C_SendRepeatedStart(void)
{
	I2C_ErrSatus Local_ErrStatus = NoError;
	/* Send start condition */
	SET_BIT(TWCR,TWCR_TWSTA);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0);
	if ((TWSR & 0xFB) != REPEATED_START_ACK)
	{
		Local_ErrStatus = RepeatedStartError;
	}

	return Local_ErrStatus;

}

I2C_ErrSatus I2C_SendSlaveAddressWithWrire(u8 Copy_u8SlaveAddress)
{
	I2C_ErrSatus Local_ErrStatus = NoError;
	/* Sens slave address */
	TWDR = Copy_u8SlaveAddress<<1;
	CLR_BIT(TWDR,0);
	/* Clear start cond bit */
	CLR_BIT(TWCR,TWCR_TWSTA);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0)
		{asm("NOP");}
	if ((TWSR & 0xFB) != SLAVE_ADD_AND_WR_ACK)
	{
		Local_ErrStatus = SlaveAddressWithWriteErr;
	}

	return Local_ErrStatus;
}

I2C_ErrSatus I2C_SendSlaveAddressWithRead(u8 Copy_u8SlaveAddress)
{
	I2C_ErrSatus Local_ErrStatus = NoError;

	/* Sens slave address */
	TWDR = Copy_u8SlaveAddress<<1;
	SET_BIT(TWDR,0);
	/* Clear start cond bit */
	CLR_BIT(TWCR,TWCR_TWSTA);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0);
	if ((TWSR & 0xFB) != SLAVE_ADD_AND_RD_ACK)
	{
		Local_ErrStatus = SlaveAddressWithReadErr;
	}

	return Local_ErrStatus;
}

I2C_ErrSatus I2C_MasterWriteDataByte(u8 Copy_u8DataByte)
{
	I2C_ErrSatus Local_ErrStatus = NoError;
	/* Send Data */
	TWDR = Copy_u8DataByte;
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0)
	{
		asm("NOP");
	}
	//DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_HIGH);
	if ((TWSR & 0xFB) != MSTR_WR_BYTE_ACK)
	{
		Local_ErrStatus = MasterWriteDataByteErr;
	}

	return Local_ErrStatus;
}

I2C_ErrSatus I2C_MasterReadDataByte(u8 *Copy_pu8DataByte)
{
	I2C_ErrSatus Local_ErrStatus = NoError;
	/* Enable master generating Acknowledge */
	//SET_BIT(TWCR,TWCR_TWEA);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
	/* Wait until INT flag is raised again */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0);
	//DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_HIGH);
	if ((TWSR & 0xFB) != MSTR_RD_BYTE_WITH_NACK)
	{
		Local_ErrStatus = MasterReadDataByteErr;
		//DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_HIGH);
	}
	else
	{
		*Copy_pu8DataByte = TWDR;
		//DIO_u8SetPinValue(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_HIGH);
	}

	return Local_ErrStatus;
}
void I2C_SendStopCondition(void)
{
	//CLR_BIT(TWCR,TWCR_TWEA);
	/* Send stop condition */
	SET_BIT(TWCR,TWCR_TWSTO);
	/* Clear the interrupt flag */
	SET_BIT(TWCR,TWCR_TWINT);
}
