#include <avr/io.h>
#include <util/delay.h>
#include <tinySPI.h>
#include <SoftwareSerial.h>

#define RX PB3 // *** D3, Pin 2
#define TX PB4 // *** D4, Pin 3

SoftwareSerial Serial(RX, TX);

int clockPin = PB2; // Connected to SHCP (pin 11) on the 74HC595.
int latchPin = PB0; // Connected to STCP (pin 12) on the 74HC595.
int dataPin = PB1;  // Connected to DS (pin 14) on the 74HC595.

int pos = 8, cnt = 0;

int main()
{
    // Serial.begin(9600);
    // Serial.listen();

    // while (1)
    // {
    //     while (Serial.available())
    //         Serial.write(Serial.read());
    // }

    SPI.begin();

    SPI.transfer(0);

    int cnt = 0;
    while (1)
    {
        SPI.transfer((char)0);
        _delay_us(2000);

        SPI.transfer(1 << 7);
        if (cnt & (1 << 2))
            _delay_us(2000);
        else
            _delay_us(1000);

        SPI.transfer((char)0);
        _delay_us(18000);

        SPI.transfer(1 << 2);
        if (cnt & (1 << 3))
            _delay_us(1000);
        else
            _delay_us(2000);

        SPI.transfer((char)0);
        _delay_us(18000);

        SPI.transfer(1);
        if (cnt & (1 << 4))
            _delay_us(1000);
        else
            _delay_us(2000);

        SPI.transfer((char)0);
        _delay_us(18000);

        SPI.transfer((char)0xFF);

        SPI.transfer(1 << 1);
        if (cnt & (1 << 5))
            _delay_us(1000);
        else
            _delay_us(2000);

        SPI.transfer((char)0);
        _delay_us(18000);

        SPI.transfer((char)0xFF);

        _delay_us(18000);

        ++cnt;
    }

    return 0;
}
