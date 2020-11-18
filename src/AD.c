
#include "platform.h"
#include "AD.h"

extern long int tensao_an6;
extern int contador_int_ad;

void inicializa_AD(void)
{
        AD1CON1bits.ADON = 0;   // Desabilita módulo AD

        AD1CHSbits.CH0SA0 = 0;  // Conecta canal 0, entrada +, MUX A, em AN6
        AD1CHSbits.CH0SA1 = 1;
        AD1CHSbits.CH0SA2 = 1;
        AD1CHSbits.CH0SA3 = 0;

        AD1CHSbits.CH0NA = 0;   // Conecta canal 0, entrada -, MUX A, em VR-

        AD1CON1bits.ADSIDL = 1; // Módulo continua funcionado em modo Idle
        AD1CON1bits.FORM = 0;   // Formatação inteiro não sinalizado
        //AD1CON1bits.SSRC = 2;   // Conversões quando ocorrer uma conparação no Timer 3
        AD1CON1bits.SSRC = 7;   // Conversões automaticas
        AD1CON1bits.ASAM = 1;   // Amostragem automática habilitada

        AD1CON2bits.VCFG = 0;   // AVdd e AVss como referência de tensão
        AD1CON2bits.CSCNA = 1;  // Auto scan habilitado
        AD1CON2bits.SMPI = 1;   // Interrupção a cada 2 seqüências de conversão
        AD1CON2bits.BUFM = 0;   // 1 buffer de 16 words
        AD1CON2bits.ALTS = 0;   // Sempre em MUX A

        AD1CON3bits.ADRC = 0;   // Clock derivado do oscilador principal
        AD1CON3bits.SAMC = 31;  // Tempo de amostragem 31 Tad
        AD1CON3bits.ADCS = 128; // Clock do AD = 128 x Tcy

        //AD1PCFG = 0xFFBF;       // AN6 como canal analógico e o restante digital
        ANCFG = 0x0001;
        //AD1CSSL = 0x0040;       // AN6 habilitado no modo auto scan
        AD1CSSH = 0x0008;         // AN19 habilitado

        AD1CON1bits.ADON = 1;   // Habilita o módulo AD
}

//void _ISR _ADC1Interrupt(void)
void _ISR __attribute__((no_auto_psv)) _ADC1Interrupt(void)
{
        IFS0bits.AD1IF = 0;     // Limpa flag da interrupção

        // Lê informação da tensao e soma valor com a leitura anterior.
        if (contador_int_ad)
        {
        tensao_an6 = tensao_an6 + ADC1BUF0;
        contador_int_ad--;             // decrementa contador.
        }
}
