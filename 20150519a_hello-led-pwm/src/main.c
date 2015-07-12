
const unsigned short TMR0_INIT = 14;
enum PWM_DIRECTION_E {ERROR, INCREMENT, DECREMENT};
enum PWM_DIRECTION_E pwm_direction = INCREMENT;

//  # Interrupt routine for timer
//

void interrupt()
{
    if (T0IF_bit == 1)
    {
        TMR0 = TMR0_INIT;
        T0IF_bit = 0;

        if (pwm_direction == INCREMENT)
        {
            CCPR1L = CCPR1L + 1;

            if (CCPR1L == 0xff)
            {
                pwm_direction = DECREMENT;
            }
        }
        else if (pwm_direction == DECREMENT)
        {
            CCPR1L = CCPR1L - 1;

            if (CCPR1L == 0x00)
            {
                pwm_direction = INCREMENT;
            }
        }
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
    TRISIO = 0b11111011;
    GPIO = 0b00000000;

    //  # Setup PWM
    //
    //  PWM period should be 100Hz, or more.
    //

    TMR2 = 0x00;
    PR2 = 0xff;
    T2CON = 0b00000110;

    CCP1CON = 0b00001100;
    CCPR1L = 0x00;

    //  # Set timer for PWM duty cycle change
    //
    //  PWM period should be 4s (fade out, fade in). Because timer goes
    //  through 512 values, this gives `4000/512ms` period.
    //

    OPTION_REG = 0b11010100;
    INTCON = 0b10100000;

    TMR0 = TMR0_INIT;
}
