#include <avr/io.h>
#include <util/delay.h>
#include <tinySPI.h>
#include <SoftwareSerial.h>

#define RX 3 // *** D3, Pin 2
#define TX 4 // *** D4, Pin 3

SoftwareSerial Serial(RX, TX);

int clockPin = PB2; // Connected to SHCP (pin 11) on the 74HC595.
int latchPin = PB0; // Connected to STCP (pin 12) on the 74HC595.
int dataPin = PB1;  // Connected to DS (pin 14) on the 74HC595.

int pos = 8, cnt = 0;

int main()
{
    // Serial.begin(115200);

    // while (1)
    // {
    //     Serial.print("abcdefghi");

    //     _delay_ms(1000);
    // }

    SPI.begin();

    DDRB |= (1 << latchPin);
    PORTB |= (1 << latchPin);

    PORTB &= ~(1 << latchPin);
    SPI.transfer(0xff);
    PORTB |= (1 << latchPin);

    while (1)
    {
        for (char i = 0; i < 16; ++i)
        {
            PORTB &= ~(1 << latchPin);

            SPI.transfer(i > cnt / 100 ? 0xff : ~(1 << 4));
            PORTB |= (1 << latchPin);

            _delay_us(400);
        }

        ++cnt;

        if (cnt > 1000)
            cnt = 0;
    }

    return 0;
}
