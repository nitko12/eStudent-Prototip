#include <avr/io.h>
#include <util/delay.h>
#include <tinySPI.h>
#include <BasicSerial3.h>

#define len(x) (uint8_t)(sizeof(x)) / (sizeof((x)[0]))

uint8_t servos[] = {1, 1 << 1, 1 << 2, 1 << 7};
uint8_t pos[] = {10, 20, 30, 40}; // [10, 40]

static inline void update_servo(uint8_t pin, uint8_t pos) // pin = (1 << n), pos: [0, 40]
{
    SPI.transfer(0);
    _delay_us(100);

    SPI.transfer(pin);

    _delay_us(50);
    for (int j = 0; j < pos; ++j)
        _delay_us(50);

    SPI.transfer(0);
    _delay_us(18000);

    SPI.transfer(0xFF);
}

int main()
{
    SPI.begin();
    SPI.transfer(0xFF);

    DDRB |= (1 << PB4);

    while (1)
    {
        uint8_t c;
        while (c = RxByte())
        {

            pos[0] -= 10;

            if (c == '0')
                pos[0] = (pos[0] - 5 + 30) % 30;
            if (c == '1')
                pos[0] = (pos[0] + 5) % 30;

            pos[0] += 10;

            for (uint8_t i = 0; i < len(servos); ++i)
                update_servo(servos[i], pos[i]);
        }
    }

    // while (1)
    // {
    //     for (uint8_t i = 0; i < len(servos); ++i)
    //         update_servo(servos[i], pos[i]);
    // }

    return 0;
}
