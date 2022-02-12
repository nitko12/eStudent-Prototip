#include <avr/io.h>
#include <util/delay.h>
#include <tinySPI.h>
#include <BasicSerial3.h>

#define len(x) (uint8_t)(sizeof(x)) / (sizeof((x)[0]))

uint8_t servos[] = {1, 1 << 1, 1 << 2, 1 << 7};
uint8_t pos[] = {20, 20, 20, 17}; // [10, 40]

static inline void update_servo(uint8_t pin, uint8_t pos) // pin = (1 << n), pos: [0, 40]
{
    SPI.transfer(~pin);
    _delay_us(100);

    SPI.transfer(0xFF);

    _delay_us(50);
    for (int j = 0; j < pos; ++j)
        _delay_us(50);

    SPI.transfer(~pin);
    _delay_us(1000);

    SPI.transfer(0xFF);
}

static inline void initTimer1(void)
{
    TCCR1 |= (1 << CTC1);                             // clear timer on compare match
    TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS11); // clock prescaler 8192
    // OCR1C = 122;                                                    // compare match value
    TIMSK |= (1 << OCIE1A); // enable compare match interrupt
}

volatile uint8_t i = 0, block = 0;
ISR(TIMER1_COMPA_vect)
{
    if (block)
        return;

    if (i == len(servos))
        i = 0;
    else
        i++;

    // if (i == 0)
    //     pos[i] = (pos[i] - 10 + 10) % 30 + 10;
    update_servo(servos[i], pos[i]);
}

int main()
{
    SPI.begin();
    SPI.transfer(0xFF);

    initTimer1();
    sei();

    uint8_t t, cnt1 = 3, cnt2 = 0;
    while (1)
    {
        if (t = RxByte())
        {
            block = 1;
            if (t == '0')
            {

                pos[0] = (pos[0] - 10 + 15) % 30 + 10;
                update_servo(servos[0], pos[0]);
            }
            if (t == '1')
            {

                // pos[1] = (pos[1] - 10 + 15) % 30 + 10;
                uint8_t arr[] = {20, 10, 30, 10};
                cnt1 = (cnt1 + 1) % 4;

                pos[1] = arr[cnt1];
                update_servo(servos[1], pos[1]);
            }
            if (t == '2')
            {

                update_servo(servos[2], pos[2]);
            }
            if (t == '3')
            {
                uint8_t arr[] = {17, 23};
                cnt2 = (cnt2 + 1) % 2;

                pos[3] = arr[cnt2];
                // pos[3] = (pos[2] - 10 + 15) % 30 + 10;

                // pos[3] = (pos[3] - 10 + 15) % 30 + 10;

                update_servo(servos[3], pos[3]);
            }
            block = 0;
        }
    }

    // while (1)
    // {
    //     for (uint8_t i = 0; i < len(servos); ++i)
    //         update_servo(servos[i], pos[i]);
    // }

    return 0;
}
