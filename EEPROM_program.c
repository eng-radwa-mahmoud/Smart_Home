#include"STD_TYPES.h"
#include"BIT_MATH.h"

#include"I2C_interface.h"

#include"EEPROM_interface.h"
#include"EEPROM_privare.h"
#include"EEPORM_config.h"


u8 EEPROM_WriteByte(u16 copy_u8EEPROMHWAdd, u8 copy_u8Data)
{
	I2C_SendStartCondition();
	I2C_SendSlaveAddressWithWrire(0b01010<<3|A2_PIN|copy_u8EEPROMHWAdd>>8);
	I2C_MasterWriteDataByte((u8)copy_u8EEPROMHWAdd);
	I2C_MasterWriteDataByte(copy_u8Data);
	I2C_SendStopCondition();
}

u8 EEPROM_ReadByte(u8* copy_u8ReadData, u16 copy_u8EEPROMHWAdd)
{
	I2C_SendStartCondition();
	I2C_SendSlaveAddressWithWrire(0b01010<<3|A2_PIN|copy_u8EEPROMHWAdd>>8);
	I2C_MasterWriteDataByte((u8)copy_u8EEPROMHWAdd);
    I2C_SendRepeatedStart();
    I2C_SendSlaveAddressWithRead(0b01010<<3|A2_PIN|copy_u8EEPROMHWAdd>>8);
    I2C_MasterReadDataByte(copy_u8ReadData);
    I2C_SendStopCondition();

}

