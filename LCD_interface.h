


#ifndef LCD_INTERFACE_H
#define LCD_INTERFACE_H

void LCD_voidInit(void);
void LCD_voidWriteChar(u8 Copy_u8Char);
void LCD_voidWriteCmnd(u8 Copy_u8Cmnd);
/*void LCD_voidWriteString (u8* Copy_pu8String, u8 Copy_u8NumberOfCharacters);*/
void LCD_voidPrintString (u8* Copy_pu8String);
void LCD_voidPrintNumber (u8 Copy_pu8Number);
void LCD_u8GoToXY (u8 copyu8_xpos,u8 copyu8_ypos);
#endif
