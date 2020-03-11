/*
 * File:   ServoDCspeed.c
 * Author: David Vela
 *
 * Created on 22 de febrero de 2020, 06:38 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <pic16f887.h>
char ADCvalant = 0;
char ADCval = 0;
char close = 0;
char open = 0;

void __interrupt() ISR(void){
    if (PIR1bits.TMR2IF == 1){
        PIR1bits.TMR2IF = 0;
        TRISCbits.TRISC1 = 0;
    }
    else
        asm("nop");
}
void setup(void);
char var1 = 0;

void main(void) {
    setup();
    while(1){
        if (PORTBbits.RB7 == 1){
            CCPR2L = 0b00001000;
            CCP2CONbits.DC2B0 = 0;
            CCP2CONbits.DC2B1 = 0;
            PORTBbits.RB2 = 1;
            PORTBbits.RB3 = 0;
        }
        else if (PORTBbits.RB6 == 1){
            CCPR2L = 0b00001111;
            CCP2CONbits.DC2B0 = 1;
            CCP2CONbits.DC2B1 = 1;
            PORTBbits.RB2 = 0;
            PORTBbits.RB3 = 1;
            open = 0;
        }
        else{
            PORTBbits.RB2 = 0; 
            PORTBbits.RB3 = 0;
        }
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    OSCCONbits.IRCF = 3;
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    INTCON = 0b11000000;
    TRISCbits.TRISC1 = 0;
    PORTCbits.RC1 = 0;
    T2CONbits.T2CKPS = 2;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    PR2 = 156;
    CCP2CONbits.CCP2M = 12;
    CCPR2L = 0b00001011;
    CCP2CONbits.DC2B0 = 1;
    CCP2CONbits.DC2B1 = 1;
    T2CONbits.TMR2ON = 1;
}