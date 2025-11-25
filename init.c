#include "init.h"

volatile uint32_t msTicks = 0;

void SysTick_Handler() {
    msTicks++;
}

void init_SysTick() {
    SysTick->LOAD = SystemCoreClock / 1000 - 1;
    SysTick->CTRL = 7;  
}

void init_LEDs() {
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
    PORTD->PCR[5] = PORT_PCR_MUX(1);  
    PTE->PDDR |= (1 << 29);           
    PTD->PDDR |= (1 << 5);            
    PORTE->PCR[29] = PORT_PCR_MUX(1); 
    PTD->PSOR = (1 << 5);             
    PTE->PSOR = (1 << 29);            
}

void init_SWs() {
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    PORTC->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;   
    PORTC->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;  
    PTC->PDDR &= ~((1 << 3) | (1 << 12));  
}

void system_logic() {
    static uint8_t inSeat = 0, belt = 0;
    static uint32_t t0 = 0, blink = 0;
    uint32_t now = msTicks;

    uint8_t sw1 = !(PTC->PDIR & (1 << 3));    
    uint8_t sw3 = !(PTC->PDIR & (1 << 12));

    if (sw1) {  
        if (!inSeat) { 
            inSeat = 1; 
            t0 = now; 
        }

        belt = sw3;

        if (belt) {  
            PTD->PCOR = (1 << 5);  
            PTE->PSOR = (1 << 29); 
        } else {  
            PTD->PSOR = (1 << 5);  

            if (now - t0 > 3000) {
                if (now - blink > 500) {
                    PTE->PTOR = (1 << 29);  
                    blink = now;
                }
            } else {
                PTE->PSOR = (1 << 29);  
            }
        }
    } else {  
        inSeat = 0;
        belt = 0;
        PTD->PSOR = (1 << 5);  
        PTE->PSOR = (1 << 29); 
    }
}
