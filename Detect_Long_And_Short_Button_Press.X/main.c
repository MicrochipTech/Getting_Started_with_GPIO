/*
    \file   main.c

    \brief  Main source file.

    (c) 2019 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip software and any
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party
    license terms applicable to your use of third party software (including open source software) that
    may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
    FOR A PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
    SOFTWARE.
 */

#define F_CPU		3333333UL

#include <avr/io.h>
#include <util/delay.h>

#define STEP_DELAY          10
#define THRESHOLD           100					/* 100 steps x 10 ms/step -> 1000 ms */
#define LONG_DELAY          500
#define SHORT_DELAY         100
#define NUMBER_OF_BLINKS    3

inline void LED_blink(uint32_t delay);

int main(void)
{
    uint8_t counter = 0;

    PORTB.DIRCLR = PIN2_bm; /* set PB2 as input */
    PORTB.DIRSET = PIN5_bm; /* set PB5 as output */
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm; /* enable internal pull-up */

    while (1)
    {
        if (~PORTB.IN & PIN2_bm) /* check if PB2 is pulled to GND */
        {
            while (~PORTB.IN & PIN2_bm) /* wait until PB2 is pulled to VDD */
            {
                _delay_ms(STEP_DELAY);
                counter++;
                if (counter >= THRESHOLD)
                {
                    LED_blink(LONG_DELAY);
                    while (~PORTB.IN & PIN2_bm) /* wait until PB2 is pulled to VDD */
                    {
                        ;
                    }
                    break;
                }
            }
            if (counter < THRESHOLD)
            {
                LED_blink(SHORT_DELAY);
            }
            counter = 0;
        }
    }
}

inline void LED_blink(uint32_t time_ms)
{
    for (uint8_t i = 0; i < NUMBER_OF_BLINKS; i++)
    {
        PORTB.OUT |= PIN5_bm;
        _delay_ms(time_ms);
        PORTB.OUT &= ~PIN5_bm;
        _delay_ms(time_ms);
    }
}
