


#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

/* OPTIONS: 1-LCD_u8_8_BIT_MODE
            2-LCD_u8_4_BIT_MODE   */
#define LCD_u8_MODE                   LCD_u8_4_BIT_MODE

#define LCD_u8_RS_PIN                 DIO_u8_PIN3
#define LCD_u8_RS_PORT                DIO_u8_PORTA

#define LCD_u8_RW_PIN                 DIO_u8_PIN2
#define LCD_u8_RW_PORT                DIO_u8_PORTA

#define LCD_u8_EN_PIN                 DIO_u8_PIN1
#define LCD_u8_EN_PORT                DIO_u8_PORTA

#define LCD_u8_DATA_PORT              DIO_u8_PORTC

/* MACRO FOR CONFIGRATION OF 4_MODE PINS */
#define LCD_u8_D7_PIN                 DIO_u8_PIN7
#define LCD_u8_D7_PORT               DIO_u8_PORTC

#define LCD_u8_D6_PIN                 DIO_u8_PIN6
#define LCD_u8_D6_PORT               DIO_u8_PORTC

#define LCD_u8_D5_PIN                 DIO_u8_PIN5
#define LCD_u8_D5_PORT               DIO_u8_PORTC

#define LCD_u8_D4_PIN                 DIO_u8_PIN4
#define LCD_u8_D4_PORT               DIO_u8_PORTC


//#define LCD_u8_SizeOfString           14

#endif
