#include "twi.h"

void TWI_Init (void)
{
	TWBR=2;	//Швидкість передачі (при 8 МГц мк = 400 кГц)
}


void TWI_Start(void) //Функція початку процедури передачі даних
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Виставляємо біти переривання, старту та активації шини
	while (!(TWCR & (1<<TWINT)));	//Очікуємо встановлення TWIN
}

void TWI_Stop(void) //Функція завершення процедури передачі даних
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); //Виставляємо біти переривання, стопу та активації шини
}

void TWI_SendByte(unsigned char c) //Функція передачі біту
{
	TWDR = c;		//Записуємо байт в регістр даних
	TWCR = (1<<TWINT)|(1<<TWEN);	//Вмикаємо передачу байту
	while (!(TWCR & (1<<TWINT)));	//Очікуємо встановлення TWIN
}

void TWI_SendByteByADDR(unsigned char c,unsigned char addr) //передача байту в шину на пристрій у певний регістр за адресою 
{
	TWI_Start(); // Відправляємо команду START
	TWI_SendByte(addr); // Відправляємо до шини адресу пристрою + біт читання-запису
	TWI_SendByte(c);// Отправим байт данных
	TWI_Stop();	// Відправляємо команду STOP
}

unsigned char TWI_ReadByte(void) // Функція читання байту
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // Виставляємо біти переривання, активації шини та підтвердження
	while (!(TWCR & (1<<TWINT)));	//Очікуємо встановлення TWIN
	return TWDR;		//Читаємо дані з регістру
}

unsigned char TWI_ReadLastByte(void) // Функція читання сотанноього байту
{
	TWCR = (1<<TWINT)|(1<<TWEN);	 // Виставляємо біти переривання та активації шини 
	while (!(TWCR & (1<<TWINT)));	//Очікуємо встановлення TWIN
	return TWDR;	//Читаємо дані з регістру
}