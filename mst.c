#define F_CPU 16000000UL // Установите тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Определяем пины для кнопок
#define BUTTON1_PIN PD2 // Кнопка 1
#define BUTTON2_PIN PD3 // Кнопка 2
#define BUTTON3_PIN PD4 // Кнопка 3
#define BUTTON4_PIN PD5 // Кнопка 4

// Определяем адреса для slave устройств
#define SLAVE1_ADDR '1' // Адрес для slave1
#define SLAVE2_ADDR '2' // Адрес для slave2

void uart_init(unsigned int baud) {
    unsigned int ubrr = F_CPU / 16 / baud - 1; // Вычисляем значение UBRR
    UBRR0H = (unsigned char)(ubrr >> 8); // Устанавливаем старший байт UBRR
    UBRR0L = (unsigned char)ubrr; // Устанавливаем младший байт UBRR
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Включаем приемник и передатчик
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Устанавливаем формат данных: 8 бит, 1 стоп-бит
}

void uart_send(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Ждем, пока регистр данных будет готов к отправке
    UDR0 = data; // Отправляем данные
}

char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Ждем, пока данные будут получены
    return UDR0; // Возвращаем полученные данные
}

void setup() {
    uart_init(9600); // Инициализация UART с baud rate 9600

    // Настройка пинов кнопок как входов с подтяжкой
    DDRD &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN) | (1 << BUTTON4_PIN));
    PORTD |= (1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN) | (1 << BUTTON4_PIN); // Включаем подтяжку
}

void loop() {
    if (!(PIND & (1 << BUTTON1_PIN))) { // Если нажата кнопка 1
        uart_send(SLAVE1_ADDR); // Отправляем адрес slave1
        uart_send('1'); // Отправляем значение для семисегментного дисплея 1
        _delay_ms(200); // Задержка для предотвращения дребезга контактов
    }

    if (!(PIND & (1 << BUTTON2_PIN))) { // Если нажата кнопка 2
        uart_send(SLAVE1_ADDR); // Отправляем адрес slave1
        uart_send('2'); // Отправляем значение для семисегментного дисплея 2
        _delay_ms(200);
    }

    if (!(PIND & (1 << BUTTON3_PIN))) { // Если нажата кнопка 3
        uart_send(SLAVE2_ADDR); // Отправляем адрес slave2
        uart_send('3'); // Отправляем значение для семисегментного дисплея 3
        _delay_ms(200);
    }

    if (!(PIND & (1 << BUTTON4_PIN))) { // Если нажата кнопка 4
        uart_send(SLAVE2_ADDR); // Отправляем адрес slave2
        uart_send('4'); // Отправляем значение для семисегментного дисплея 4
        _delay_ms(200);
    }
}

// Добавьте функцию main, если компилируете вне Arduino IDE.
int main(void) {
    setup(); // Вызов функции настройки

    while (1) { 
        loop(); // Постоянно вызываем функцию loop()
    }
}