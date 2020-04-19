#include "servos.h"

void init_servos() {
    
   TMR1 = 0;
   PR1 = 5000; 
   T1CON = 0x8000;
   IFS0 &= 0xFFF7;
   IEC0 |= 0x0008;
   IPC0bits.T1IP = 7;
   
}

int get_servo_1(int estado, int contador) { // Servo giro
    
    int AP;
    
    if(estado == 1) { // Rango: 20 - 11
        AP = 14; 
    } else if(estado == 2) {
        AP = 11; 
    } else if(estado == 3) {
        if(contador <= 8000) { 
            AP = 11; 
        } else {
            AP = 20; 
        }
    } else if(estado == 4) {
        if(contador <= 8000) { 
            AP = 20; 
        } else {
            AP = 14; 
        }
    }
    
    return AP;
    
}

int get_servo_2(int estado, int contador) { // Servo primera articulacion
    
    int AP;
    
    if(estado == 1) { // Rango: 20 - 18
        AP = 20; 
    } else if(estado == 2) {
        if(contador <= 8000) { 
            AP = 20; 
        } else {
            AP = 18; 
        }
    } else if(estado == 3) {
        if(contador <= 16000) { 
            AP = 20; 
        } else {
            AP = 18; 
        }
    } else if(estado == 4) {
        AP = 20; 
    }
        
    return AP;
    
}

int get_servo_3(int estado, int contador) { // Servo segunda articulacion
    
    int AP;
    
    if(estado == 1) { // Rango: 20 - 16
        AP = 20; 
    } else if(estado == 2) {
        if(contador <= 8000) { 
            AP = 20; 
        } else {
            AP = 16; 
        }
    } else if(estado == 3) {
        if(contador <= 16000) { 
            AP = 20; 
        } else {
            AP = 16; 
        }
    } else if(estado == 4) {
        AP = 20; 
    }
    
    return AP;
    
}

int get_servo_4(int estado, int contador) { // Servo pinza
    
    int AP;
    
    if(estado == 1) { // Rango: 8 - 4
        AP = 8; 
    } else if(estado == 2) {
        if(contador <= 16000) { 
            AP = 8; 
        } else {
            AP = 4; 
        }
    } else if(estado == 3) {
        if(contador <= 24000) { 
            AP = 4; 
        } else {
            AP = 8; 
        }
    } else if(estado == 4) {
        AP = 8; 
    }

    return AP;
    
}
