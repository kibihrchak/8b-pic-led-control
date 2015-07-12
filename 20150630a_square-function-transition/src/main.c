#define LINEAR_OUTPUT

#ifndef LINEAR_OUTPUT

#include "pwm_lookup_table.h"

#endif

#include <stdint.h>

const uint8_t TRM0_INIT = 13;

enum PWM_DIRECTION_E {
    PD_INCREMENT,
    PD_DECREMENT,
};

enum PWM_DIRECTION_E pwm_direction = PD_INCREMENT;

uint8_t update_duty_cycle = 0;
uint16_t next_duty_cycle = 0;
const uint8_t DUTY_CYCLE_LSB_MASK = 0b00110000;

uint8_t pwm_lookup_table_index = 0;

uint8_t ccpr1l_new, ccp1con_new;

enum DIFFERENCE_STATES_E {
    DS_INCREMENT,
    DS_HOLD,
    DS_DECREMENT,
};

enum DIFFERENCE_STATES_E difference_state = DS_INCREMENT;

struct bytes_struct
{
    uint8_t high;
    uint8_t low;
};

union word_union
{
    struct bytes_struct bytes;
    uint16_t word;
};

union word_union value;
uint16_t first_difference = 0;
const uint16_t SECOND_DIFFERENCE = 1;

uint16_t iteration_index = 0;
const uint16_t HALF_ITERATION_STEPS = 255;

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
    value.word = 0x007f;

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
                case PD_INCREMENT:
                    value.word += first_difference;
                    break;
                case PD_DECREMENT:
                    value.word -= first_difference;
                    break;
                default:
                    break;
            }

            iteration_index++;

            switch (difference_state)
            {
                case DS_INCREMENT:
                    first_difference += SECOND_DIFFERENCE;

                    if (iteration_index == HALF_ITERATION_STEPS)
                    {
                        difference_state = DS_HOLD;
                    }
                    break;
                case DS_HOLD:
                    difference_state = DS_DECREMENT;
                    break;
                case DS_DECREMENT:
                    first_difference -= SECOND_DIFFERENCE;

                    if (first_difference == 0)
                    {
                        switch (pwm_direction)
                        {
                            case PD_INCREMENT:
                                value.word =
                                    (value.word & 0xff00) | 0x00ff;
                                pwm_direction = PD_DECREMENT;
                                break;
                            case PD_DECREMENT:
                                value.word =
                                    (value.word & 0xff00) | 0x0000;
                                pwm_direction = PD_INCREMENT;
                                break;
                            default:
                                break;
                        }

                        difference_state = DS_INCREMENT;
                        iteration_index = 0;
                    }
                    break;
            }



            if (value.bytes.low != pwm_lookup_table_index)
            {
                pwm_lookup_table_index = value.bytes.low;

#ifndef LINEAR_OUTPUT
                next_duty_cycle =
                    PWM_LOOKUP_TABLE[pwm_lookup_table_index];

                ccpr1l_new = (uint8_t)(next_duty_cycle >> 2);
                ccp1con_new = (CCP1CON & ~DUTY_CYCLE_LSB_MASK) |
                    (((uint8_t)(next_duty_cycle)) << 4) &
                    DUTY_CYCLE_LSB_MASK;
#else
                ccpr1l_new = pwm_lookup_table_index;
                ccp1con_new = (CCP1CON & ~DUTY_CYCLE_LSB_MASK) |
                    ((0x00) << 4) &
                    DUTY_CYCLE_LSB_MASK;
#endif
            }

            INTCON.T0IE = 0;
            update_duty_cycle = 0;
            INTCON.T0IE = 1;
        }
    }
}
