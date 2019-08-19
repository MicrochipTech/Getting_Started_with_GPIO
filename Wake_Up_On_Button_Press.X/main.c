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

#define F_CPU 3333333
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <util/atomic.h>

#define PB2_LOW !(PORTB.IN & PIN2_bm)
#define PB2_INTERRUPT PORTB.INTFLAGS & PIN2_bm
#define PB2_CLEAR_INTERRUPT_FLAG PORTB.INTFLAGS &= PIN2_bm

volatile uint8_t pb2Ioc;

ISR(PORTB_PORT_vect)
{
    if(PB2_INTERRUPT)
    {
        pb2Ioc = 1;
        PB2_CLEAR_INTERRUPT_FLAG;
    }
}

void LED_init(void)
{
    PORTB.DIR |= PIN5_bm;
    PORTB.OUT |= PIN5_bm;
}

void BUTTON_init(void)
{
    PORTB.DIR &= ~ PIN2_bm;
    PORTB.PIN2CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
}

int main(void)
{  
    LED_init();
    BUTTON_init();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sei();

    while (1)
    {
        sleep_mode();
        
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            _delay_ms(10);
        }
        
        if(pb2Ioc)
        {
            if(PB2_LOW)
            {
                PORTB.OUT &= ~PIN5_bm;
            }
            else
            {
                PORTB.OUT |= PIN5_bm;
            }
            
            pb2Ioc = 0;
        }
    }

   return 0;
}
