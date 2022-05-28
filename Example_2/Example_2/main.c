#include "main.h"
//----------------------------------------
unsigned char sec,min,hour,day,date,month,year;
unsigned char sec_end, min_end, hour_end, day_end, month_end, year_end;
unsigned char btn_state = 0;
//Налаштування таймеру
unsigned char timer_sec = 15;
unsigned char timer_min = 0;
unsigned char timer_hour = 0;

//----------------------------------------
void port_ini(void) //Функція ініціалізації портів
{
	PORTD=0x00;	//Ініціалізуємо порт D логічним 0
	DDRD=0xFF;	//Встановляємо порту D режим виходу
	PORTC = 1<<PORTC0; //Ініціалізуємо порт С0 логічною 1
	DDRC |= 0<<PORTC0; //Встановляємо порту D режим входу
	
	
}

unsigned char RTC_ConvertFromBinDec(unsigned char c) //Функція конвертації двійково-десятичного числа у десятичний
{
	unsigned char ch = ((c>>4)*10+(0b00001111&c)); //здвигаємо вправо і множимо на 10 - отримуємо десятки, потім додаємо одиниці
	return ch;
}

//----------------------------------------
int main(void)
{
	port_ini(); //Ініціалізуємо порти
	TWI_Init(); //Ініціалізуємо шину TWI
	lcd_init();  //Ініціалізуємо  дисплей
	clear_lcd(); //Очищуємо дисплей


	//Алгоритм для першого встановлення часу
	// 	TWI_Start();
	// 	TWI_SendByte(0b11010000);
	// 	TWI_SendByte(0);//Переходим на 0x00
	// 	TWI_SendByte(RTC_ConvertFromBinDec(0)); //секунды
	// 	TWI_SendByte(RTC_ConvertFromBinDec(31)); //минуты
	// 	TWI_SendByte(RTC_ConvertFromBinDec(20)); //часы
	// 	TWI_SendByte(RTC_ConvertFromBinDec(5)); //день недели
	// 	TWI_SendByte(RTC_ConvertFromBinDec(29)); //дата
	// 	TWI_SendByte(RTC_ConvertFromBinDec(1)); //месяц
	// 	TWI_SendByte(RTC_ConvertFromBinDec(16)); //год
	//	TWI_StopCondition();
	
	while(1)
	{
		//Отримуємо час з DS1307
		TWI_SendByteByADDR(0,0b11010000);	//Переходимо за адресою першого регістру (0)
		_delay_ms(200);
		TWI_Start(); //Відправляємо умову START
		TWI_SendByte(0b11010001); //Відправляємо біт читання
		sec = TWI_ReadByte();
		min = TWI_ReadByte();
		hour = TWI_ReadByte();
		day = TWI_ReadByte();
		date = TWI_ReadByte();
		month = TWI_ReadByte();
		year = TWI_ReadLastByte();
		TWI_Stop(); //Отправим условие STOP
		//Конвертуємо числа у десятковий формат
		sec = RTC_ConvertFromBinDec(sec); 
		min = RTC_ConvertFromBinDec(min); 
		hour = RTC_ConvertFromBinDec(hour); 
		day = RTC_ConvertFromBinDec(day); 
		year = RTC_ConvertFromBinDec(year); 
		month = RTC_ConvertFromBinDec(month); 
		date = RTC_ConvertFromBinDec(date); 
		//Виводимо дату на дисплей
		send_date(sec,min,hour,day,date,month,year);
		
		//Обробка натиснення кнопки
		if (!(PINC&1<<PINC0)) //Якщо кнопка натиснута
		{
			if(!btn_state)	  //та таймер ще не був встановлений
			{
				btn_state=1;	//Змінюємо статус кнопки
				//Виводимо на екран час коли таймер має спрацювати 
				setpos(14,0);	
				send_str("End at:");
				setpos(14,1);
				sec_end = sec+timer_sec; //Отримуємо кінцевий час
				min_end = min+timer_min;
				hour_end = hour+timer_hour;
				if(sec_end>59){min_end++;sec_end-=60;} //Перетворюємо кінцевий час у правильний формат 
				if(min_end>59){hour_end++;min_end-=60;}	
				if(hour_end>23){hour_end-=23;}	
				send_time(sec_end, min_end, hour_end);
			} 
			else //Якщо кнопка натиснута під час роботи таймера, він стає на паузу
			{
				btn_state = 0;
				setpos(14,0);
				send_str("Stop    ");
			}	
		}
		if(btn_state)  //Перевіряємо чи не закінчився час
		{
			if(sec>=sec_end && min >= min_end && hour >= hour_end)
			{ 
				//Якщо час вийшов друкуємо на дисплеї повідомлення та повертаємо таймер у початкове положення
				clear_lcd();
				setpos(14,0);
				send_str("END");
				btn_state=0;
			}
		} 
	}
}