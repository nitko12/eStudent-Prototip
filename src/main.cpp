#include <avr/io.h>
#include <util/delay.h>
#include <tinySPI.h>
#include <BasicSerial3.h>
#include <DigiUSB.h>

#define len(x) (uint8_t)(sizeof(x)) / (sizeof((x)[0]))

const char map[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                 // 0-15
    32, 10, 32, 12, 32, 11, 30, 10, 32, 8, 30, 15, 25, 15, 26, 17,  // 16-31
    20, 20, 22, 20, 20, 20, 20, 20, 20, 17, 20, 20, 20, 20, 23, 22, // 32-47
    14, 27, 15, 27, 16, 27, 13, 25, 15, 23, 15, 27, 14, 27, 17, 32, // 48-63
};

const int UP = 30,
          MIDDLE = 20,
          DOWN = 15;

const int LEFT_PLUCK = 25,
          MIDDLE_PLUCK = 20,
          RIGHT_PLUCK = 15;

// uint8_t servos[] = {1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7};
// uint8_t pos[] = {17, 17, 17, 17, 17, 17, 17, 17}; // [10, 40]
uint8_t pos[] = {20, 20, 20, 20, 20, 20, 20, 20}; // [10, 40]

static inline void update_servo(uint8_t pin, uint8_t pos) // pin = (1 << n), pos: [0, 40]
{
    SPI.transfer(pin >= 8 ? ~(1 << (pin % 8)) : 0xFF);
    SPI.transfer(pin < 8 ? ~(1 << pin) : 0xFF);

    _delay_us(100);

    SPI.transfer(0xFF);
    SPI.transfer(0xFF);

    _delay_us(50);
    for (int j = 0; j < pos; ++j)
        _delay_us(50);

    SPI.transfer(pin >= 8 ? ~(1 << (pin % 8)) : 0xFF);
    SPI.transfer(pin < 8 ? ~(1 << pin) : 0xFF);

    _delay_us(100);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
}

// static inline void initTimer1(void)
// {
//     TCCR1 |= (1 << CTC1);                             // clear timer on compare match
//     TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS11); // clock prescaler 8192
//     // OCR1C = 122;                                                    // compare match value
//     TIMSK |= (1 << OCIE1A); // enable compare match interrupt
// }

// volatile uint8_t i = 0, block = 0;
// ISR(TIMER1_COMPA_vect)
// {
//     if (block)
//         return;

//     if (i == len(servos))
//         i = 0;
//     else
//         i++;

//     // pos[0] = (pos[0] - 10 + 15) % 30 + 10;
//     update_servo(servos[i], pos[i]);
// }

int main()

{
    DigiUSB.begin();

    SPI.begin();
    SPI.transfer(0xFF);

    // initTimer1();
    // sei();

    int t;
    while (1)
    {
        if (DigiUSB.available())
        {
            t = DigiUSB.read();

            update_servo(t % 16, map[t]);
        }

        DigiUSB.refresh();
    }

    return 0;
}
