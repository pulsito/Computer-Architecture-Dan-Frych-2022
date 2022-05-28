#ifndef TWI_H_
#define TWI_H_

#include "main.h"

void TWI_Init (void); //ініціалізація 
void TWI_Start(void); //Відправка команди START
void TWI_Stop(void); //Відправка команди STOP
void TWI_SendByte(unsigned char c); //Передача байту в шину
void TWI_SendByteByADDR(unsigned char c,unsigned char addr); //Передача байту в шину на пристрій у певний регістр за адресою 
unsigned char TWI_ReadByte(void); //читаємо байт
unsigned char TWI_ReadLastByte(void); //читаємо останній байт


#endif /* TWI_H_ */