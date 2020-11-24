//#include <p24Fxxxx.h>
#include "p24FJ256DA210.h"
#include "Compiler.h"
#include "as3993_config.h"
#include "global.h"
#include "platform.h"
#include "timer.h"

void tick(void);
static volatile u16 slowTimerMsValue;

void slowTimerStart( )
{
    T3CONbits.TON = 0;
    slowTimerMsValue = 1;   //start with 1ms to get immediate stop at 0ms delays
    TMR3 = 0;
    _T3IF = 0;
    _T3IE = 1;
    T3CONbits.TON = 1;
}

u16 slowTimerValue( )
{
    return slowTimerMsValue;
}

void slowTimerStop( )
{
    _T3IE = 0;
    T3CONbits.TON = 0;
}

void timerInit()
{
    // Timer3 is slow-running timer used for timing of longer periods
    // prescaler 1:64, period 10ms
    T3CON = 0x00;
    T3CONbits.TON = 0;
    T3CONbits.TCKPS = 2;        // prescaler 1:64
    _T3IP = 2; // Timer3 interrupt priority 2 (low)
    PR3 = (SYSCLK / 64) / 100;
    // do not enable T3 interrupt here, they will be enabled in slowTimerStart()
}

void INTERRUPT timer3Isr(void)	// interrupt handler for Timer3
{					// overflow
    _T3IF = 0;
    slowTimerMsValue += 10;             // increase ms counter by 10 as period is 10ms
}


void ligaTimer2 (void)
{
    T2CONbits.TON = 0;

    //T2CON = 0x0000;
    PR2 = 16000;
    TMR2 = 0;
    _T2IF = 0;
    _T2IE = 1;
    T2CONbits.TON = 1;

}


void INTERRUPT timer2Isr (void)    // interrupt handler for Timer2
{
    _T2IF = 0;
    tick();
    //_LATC8 = !_LATC8;
}

