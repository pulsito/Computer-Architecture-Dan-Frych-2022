#ifndef LCD_H_
#define LCD_H_

#include "main.h"

//----------------------------------------
#define e1    PORTD|=0b00001000		//ставимо лінію Е в 1
#define e0    PORTD&=0b11110111		//ставимо лінію E в 0
#define rs1    PORTD|=0b00000100	//ставимо лінію RS в 1 (дані)
#define rs0    PORTD&=0b11111011	//ставимо лініюRS в 0 (команди)
//----------------------------------------

void lcd_init(void);				//функція ініціалізації дісплею
void setpos(unsigned char x, unsigned y);	//функція постановки курсора 
void send_char(unsigned char c);	//функція передачі символу
void send_str (char str[]);			//функція передачі рядка 
void send_time(char sec, char min,char hour); //функція передачі часу
void send_date(char sec, char min,char hour,char day,char date,char month,char year); //функція передачі дати
void clear_lcd(void);				//функція очищення дісплею


#endif /* LCD_H_ */