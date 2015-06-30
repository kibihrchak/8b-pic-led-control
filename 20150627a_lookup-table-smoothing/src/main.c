#include "pwm_lookup_table.h"
#include <stdint.h>

const uint8_t TRM0_INIT = 74;
enum PWM_DIRECTION_E {
    INCREMENT = 0,
    DECREMENT = 0,
};
enum PWM_DIRECTION_E pwm_direction = INCREMENT;

uint8_t update_duty_cycle = 0;
uint16_t next_duty_cycle = 0;
const uint8_t DUTY_CYCLE_LSB_MASK = 0b00110000;

uint8_t pwm_lookup_table_index, pwm_lookup_table_index_new = 0;
// must be exactly one half of max index
const uint8_t HALF_TRANSITION_INDEX = 127;

uint8_t user_register;

sbit less_than_half at user_register.B0;

uint8_t ccpr1l_new, ccp1con_new;

const uint16_t STEP = 35;
uint32_t index_increment = 0;
uint32_t index_expanded = 0;


//  # Interrupt routine for timer
//

void interrupt()
{
    if (T0IF_bit == 1)
    {
        TMR0 = TRM0_INIT;
        T0IF_bit = 0;

        update_duty_cycle = 1;

        CCPR1L = ccpr1l_new;
        CCP1CON = ccp1con_new;
    }
}

void main()
{
    //  # Setup GPIO
    //
    //  1.  All inputs, except the LED output
    //  2.  All digital
    //  3.  LED output 0
    //  4.  Weak pull-up off - not needed, `#GPPU` is 1
    //  5.  IOC off - not needed, reset value is 0
    //

    ANSEL = 0b01111000;
    TRISIO = 0b11111001;
    GPIO = 0b00000000;


    //  # Setup PWM
    //
    //  PWM period should be 100Hz, or more.
    //

    TMR2 = 0x00;
    PR2 = 0xff;
    T2CON = 0b00000110;

    CCP1CON = ccp1con_new = 0b00001100;
    CCPR1L = ccpr1l_new = 0x00;


    //  # Set timer for PWM duty cycle change
    //
    //  PWM period should be 4s (fade out, fade in). Because timer goes
    //  through 512 values, this gives `4000/512ms` period.
    //

    OPTION_REG = 0b11010100;

    TMR0 = TRM0_INIT;

    
    //  # Interrupts
    //
    //  1.  `TMR0`: load new PWM duty cycle value
    //

    INTCON = 0b10100000;

    while (1)
    {
        if (update_duty_cycle)
        {
            switch (pwm_direction)
            {
                case INCREMENT:
                    index_expanded += index_increment;
                    break;
                case DECREMENT:
                    index_expanded -= index_increment;
                    break;
                default:
                    break;
            }

            pwm_lookup_table_index_new =
                (uint8_t)(index_expanded >> 16);


            if (pwm_lookup_table_index_new != HALF_TRANSITION_INDEX)
            {
                less_than_half =
                    pwm_lookup_table_index_new < HALF_TRANSITION_INDEX;

                if (less_than_half)
                {
                    switch (pwm_direction)
                    {
                        case INCREMENT:
                            index_increment += step;
                            break;
                        case DECREMENT:
                            index_increment -= step;
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    switch (pwm_direction)
                    {
                        case INCREMENT:
                            index_increment -= step;
                            break;
                        case DECREMENT:
                            index_increment += step;
                            break;
                        default:
                            break;
                    }
                }
            }


            if (pwm_lookup_table_index_new != pwm_lookup_table_index)
            {
                pwm_lookup_table_index = pwm_lookup_table_index_new;

                next_duty_cycle =
                    PWM_LOOKUP_TABLE[pwm_lookup_table_index];

                ccpr1l_new = (uint8_t)(next_duty_cycle >> 2);
                ccp1con_new = (CCP1CON & ~DUTY_CYCLE_LSB_MASK) |
                    (((uint8_t)(next_duty_cycle)) << 4) &
                    DUTY_CYCLE_LSB_MASK;

                switch (pwm_lookup_table_index)
                {
                    // must be even number
                    case 254:
                        pwm_direction = DECREMENT;
                        break;
                    case 0:
                        pwm_direction = INCREMENT;
                        break;
                }

            }

            INTCON.T0IE = 0;
            update_duty_cycle = 0;
            INTCON.T0IE = 1;
        }
    }
}
