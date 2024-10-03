#define F_CPU 16000000UL // Установите тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Определяем пины для семисегментного дисплея
#define SEG_A PB0
#define SEG_B PB1
#define SEG_C PB2
#define SEG_D PB3
#define SEG_E PC0
#define SEG_F PB5
#define SEG_G PB4

// Функция для установки состояния сегментов
void setSegments(uint8_t segments) {
    PORTB = ~segments; // Инвертируем состояние портов для отображения цифры на дисплее с общим катодом
}

// Функция для отображения цифры на семисегментном дисплее
void displayNumber(int num) {
    switch(num) {
        case 0: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F)); break; // 0
        case 1: setSegments((1 << SEG_B) | (1 << SEG_C)); break; // 1
        case 2: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_G)); break; // 2
        case 3: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_G)); break; // 3
        case 4: setSegments((1 << SEG_B) | (1 << SEG_C) | (1 << SEG_F) | (1 << SEG_G)); break; // 4
        case 5: setSegments((1 << SEG_A) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_F) | (1 << SEG_G)); break; // 5
        case 6: setSegments((1 << SEG_A) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F) | (1 << SEG_G)); break; // 6
        case 7: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C)); break; // 7
        case 8: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_E) | (1 << SEG_F) | (1 << SEG_G)); break; // 8
        case 9: setSegments((1 << SEG_A) | (1 << SEG_B) | (1 << SEG_C) | (1 << SEG_D) | (1 << SEG_F)); break; // 9
        default: setSegments(0xFF); // Включаем все сегменты для недопустимых значений, если нужно показать ошибку.
    }
}

void uart_init(unsigned int baud) {
    unsigned int ubrr = F_CPU / 16 / baud - 1; 
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Включаем приемник и передатчик
    UCSR0C = (3<<UCSZ00); // Устанавливаем формат данных: 8 бит, 2 стоп-бита
}

void uart_send(char data) {
    while (!(UCSR0A & (1<<UDRE0))); 
    UDR0 = data; 
}

char uart_receive(void) {
    while (!(UCSR0A & (1<<RXC0))); 
    return UDR0; 
}

void setup() {
    uart_init(9600); 
    DDRB = (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G); // Настройка портов для вывода на дисплей
}

void loop() {
    if(uart_receive() == '2') { 
        char value = uart_receive(); 
        if(value == '3') {
            displayNumber(3);
        } else if(value == '4') {
            displayNumber(4);
        }
    }
}

int main(void) {
    setup(); // Вызов функции настройки

    while (1) { 
        loop(); // Постоянно вызываем функцию loop()
    }
}