#include "ad.h"

void init_ad(int pines){
    
    TRISB |= ((pines & 0x3F) >> 2);
    TRISA |= pines & 3;
    AD1PCFGL = ~pines;
    AD1CON3 = 0x105; // SAMC = 1, ADCS = 5 -> 1 ciclo de muestreo
    AD1CON2 = 0;
    AD1CON1 = 0x80E0; // ADON, SSRC = 111
    IPC3bits.AD1IP = 6;
    
}

int get_ad(int pin){
    
    AD1CHS0 = pin; // selecciona pin de conversión
    IFS0bits.AD1IF = 0;
    AD1CON1bits.SAMP = 1; // comienza
    while (!IFS0bits.AD1IF); // conversión terminada?
    
    return ADC1BUF0;
    
}