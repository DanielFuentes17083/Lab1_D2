/*
 * File:   Lab1_17083.c
 * Author: Daniel Fuentes
 *
 * Created on 24 de enero de 2020, 04:51 PM
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000                                                              //configuracion del oscilo a 4M
                                               
int unsigned pass;                                                                      //bandera que indica cuando se acaba el juego y reinicia las variables
int unsigned cont1;                                                                     //clics de P1
int unsigned cont2;                                                                     //clics de P2
unsigned char const playerP[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //array para la posición de los jugadores
unsigned char const seg[] = {0b00000000, 0b00000110, 0b01011011, 0b01001111};           //array para el siete segmentos
int unsigned past1;                                                                     //guarda el valor pasado del jugador 1 para antirrebote
int unsigned past2;                                                                     //guarda el valor pasado del jugador  para antirrebote

void setting(void){                                                                     //Funcion que configura los puertos del pic
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD5 = 1;
    ANSEL = 0;
    ANSELH = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    return;
}

void main(void) {                                                                       //Funcion principal
    setting();                                                                          //se ejecuta settings  
    while (1){                                                                          //loop para que el programa no acabe
        if (PORTDbits.RD7 == 1){
            
            PORTDbits.RD0 = 1;
            PORTC = seg[3];
            __delay_ms(500);
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 1;
            PORTC = seg[2];
            __delay_ms(500);
            PORTDbits.RD1 = 0;                                                          //conteo de salida
            PORTDbits.RD2 = 1;
            PORTC = seg[1];
            __delay_ms(500);
            PORTDbits.RD0 = 1;
            PORTDbits.RD1 = 1;
            PORTDbits.RD2 = 1;
            PORTC = seg[0];
            pass = 1;
            while (pass == 1){                                                          //Proceso mientras no hay ganador
                if (PORTDbits.RD5 == 1 && past1 == 0){                                  //si presiona P1 para moverse y guarda valor
                    cont1++;
                    past1 = 1;
                }
                
                if (PORTDbits.RD6 == 1 && past2 == 0){                                  //si presiona P2 para moverse y guarda valor
                    cont2++;
                    past2 = 1;
                }
                PORTA = playerP[cont1];                                                 //Despliegue de ambos jugadores en LEDs
                PORTB = playerP[cont2];
                
                if (cont1 == 8){                                                        //si gana P1
                    PORTC = seg[1];
                    PORTDbits.RD3 = 1;
                    pass = 0;
                }
                if (cont2 == 8){                                                        //si gana P2
                    PORTC = seg[2];
                    PORTDbits.RD4 = 1;
                    pass = 0;
                }
                if (PORTDbits.RD5 == 0){                                                
                    past1 = 0;
                }
                                                                                        //antirrebote
                if (PORTDbits.RD6 == 0){
                    past2 = 0;
                }
            }
            __delay_ms(4000);                                                           //Deja los resultados desplegados por 4 segundos
        }
        cont1 = 0;
        cont2 = 0;
        PORTA = playerP[0];
        PORTB = playerP[0];
        PORTC = seg[0];
        PORTDbits.RD3 = 0;                                                              //Reinicio de variables y puertos al final del juego
        PORTDbits.RD4 = 0;                                                              //Hola jose
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 0;
    }
    return;
}