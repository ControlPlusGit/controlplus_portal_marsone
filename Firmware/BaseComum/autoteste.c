#include "global.h"
#include "lcd.h"
#include "uart_driver.h"
#include "GenericTypeDefs.h"
#include "as3993_public.h"
#include "platform.h"
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "gps.h"
//#include <gps.h>
#include "i2c.h"
#include "onewire.h"
#include "rtc.h"


/*
    Para o devido funcionamento dessas rotinas, todo os pinos envolvidos devem estar como IO, ou seja, nao pode haver 
 * nenhuma inicializacao de SPI, UART, AD, IRQ ou similares, na familia PIC24 deve-se atentar ao RPx que alocam um pino a
 * um modulo interno diretamente.
 * 
 * A I2C deve ser iniciada!!!
 
 */

/*
Conectores utilizados do hardware 7.16
 J13 = Conector do modem
 J11 = Conector do ZigBee
 JJ2 = Conector do GPS
 U19 = Conector do Wifi
 J10 = Conector de alimentacao de interfaces principal
 P2 = Conector para a placa de RF
 P3 = Conector para a placa de RF
 P1 = Conector de interface auxiliar 1
 J9 = Conector de interface auxiliar 2
 J7 = Conector de interface auxiliar 3
*/

/*
 * PLANOS PARA P3
 * LIGA_PA -E- EN, 1 e 2 de P1
 * IRQ -E- NCS, 3 e 4 de P1
 * MISO -E- MOSI, 5 e 6 de P1
 * CLKSYS -E- CLK, 7 e 8 de P1.
*/

/*
 * PLANOS PARA P2
 * GP8 -E- GP6, 1 e 2 de P2
 * GP7 -E- LIGA_PA(BARRADO), 3 e 4 de P2
 * SEL_BBA -E- SEL_A1-4, 5 e 6 de P2
 * SEL_B5-8 -E- SEL_A1-2, 7 e 8 de P2
 * SEL_A3-4 -E- SEL_B5-6, 9 e 10 de P2
 * SEL_B7-8 -E- TUNE_CAP1, 11 e 12 de P2
 * TUNE_CAP2 -E- TUNE_CAP3, 13 e 18 de P2
*/ 

/*
 * PLANOS PARA P1
 * OPTO_A1 -E- GP1, 1 e 9 de P1
 * OPTO_A2 -E- GP16, 5 e 10 de P1
 * GP3 -E- GP4, 11 e 12 de P1
 * ADIN -E- um divisor resistivo entre 3V3 e OUT4 de U4, 13 e 4 de P1 mais os resistores
 * OPTO_K1 e OPTO_K2 no GND, 2, 6 e 16 de P1, caso R28 e R30 devam ser montador, essa ligacao nao dever ser feita
*/

/*
 *  PLANOS PARA J10
 *  OPTO_A1 -E- SRL3 -E- SRL1, 1, 3 e 5 de J10
 *  OPTO_A2 -E- SRL4 -E- SRL2, 2, 4 e 7 de J10
 *  CRL1 -E- CRL2 -E- ENT_12V, 6, 8 e 9 de J10
 */

/*
 * PLANOS PARA U19
 * RX2 -E- TX2, 3 e 5 de U19
 * GP12 -E- GP13, 4 e 6 de U19
 * GP14 -E- GP15, 8 e 10 de U19
 * GP2 -E- GP5 de J13, 2 de J13 e 9 de U19
*/


/*
 * PLANOS PARA J13
 * RX3 -E- TX3, 3 e 5 de J13
 * GP9 -E- GP10, 4 e 6 de J13
 * GP5 -E- GP2 de U19, 2 de J13 e 9 de U19
*/


/*
 * 
*/


#define TEMPO_PARA_TESTE_DAS_SERIAIS 1000
#define TEMPO_PARA_TESTE_DE_PINOS 10//10

#define LOGICA_DIRETA 0
#define LOGICA_INVERSA 1

int TesteSerial1(void);

void IniciaAutoTeste(void){
    //Configura o hardware para testes
    
}

void AlertaSonoroDeErro (void){
    _TRISC4 = 0;
    _LATC4 = 1;//LIGO BUZZER
    delay_ms(500);
    _LATC4 = 0;//DESLIGO BUZZER
    delay_ms(500);

    
}

void DesligaTodosOsLeds (void){
    _TRISE0 = 0;//SAIDA LED A1
    _TRISE1 = 0;//SAIDA LED A1
    _TRISE3 = 0;//SAIDA LED A1
    _TRISE4 = 0;//SAIDA LED A1
    _TRISE5 = 0;//SAIDA LED A1
#ifdef PCB_V715
    _TRISA1 = 0;//SAIDA LED A7
    _TRISA6 = 0;//SAIDA LED A8
#else 
    _TRISD3 = 0;//SAIDA LED A7
    _TRISD2 = 0;//SAIDA LED A8
#endif
    _TRISE8 = 0;      //SAIDA LED TAG
    _TRISE9 = 0;      //SAIDA LIGA PA
    
    _TRISA10 = 0;     //SAIDA LED_ZIG
    _TRISG1 = 0;     //SAIDA RIO4
    _TRISG14 = 0;      //SAIDA LED0
    _TRISG0 = 0;      //SAIDA LED1
    _TRISG13 = 0;
    
    
    
    _LATE0 = 0;      //SAIDA LED A1
    _LATE1 = 0;      //SAIDA LED A2
    _LATE2 = 0;      //SAIDA LED A3
    _LATE3 = 0;      //SAIDA LED_A4
    _LATE4 = 0;      //SAIDA LED A5
    _LATE5 = 0;      //SAIDA LED A6
#ifdef PCB_V715
    _LATA1 = 0;      //SAIDA LED A7
    _LATA6 = 0;      //SAIDA LED A8
#else
    _LATD3 = 0;      //SAIDA LED A7
    _LATD2 = 0;      //SAIDA LED A8
#endif
    
    _LATE8 = 0;      //SAIDA LED TAG
    _LATE9 = 0;      //SAIDA LIGA PA
    
    _LATA10 = 0;     //SAIDA LED_ZIG
    _LATG1 = 0;     //SAIDA RIO4
    _LATG14 = 0;      //SAIDA LED0
    _LATG0 = 0;      //SAIDA LED1
    
    _LATG13 = 0;
    

    
    
}

void LigaTodosOsLeds (void){
    _TRISE0 = 0;//SAIDA LED A1
    _TRISE1 = 0;//SAIDA LED A1
    _TRISE3 = 0;//SAIDA LED A1
    _TRISE4 = 0;//SAIDA LED A1
    _TRISE5 = 0;//SAIDA LED A1
#ifdef PCB_V715
    _TRISA1 = 0;//SAIDA LED A7
    _TRISA6 = 0;//SAIDA LED A8
#else 
    _TRISD3 = 0;//SAIDA LED A7
    _TRISD2 = 0;//SAIDA LED A8
#endif
    _TRISE8 = 0;      //SAIDA LED TAG
    _TRISE9 = 0;      //SAIDA LIGA PA
    
    _TRISA10 = 0;     //SAIDA LED_ZIG
    _TRISG1 = 0;     //SAIDA RIO4
    //_TRISG14 = 0;      //SAIDA LED0
    _TRISG0 = 0;      //SAIDA LED1
    
    _TRISB7 = 0; // RX1
    _TRISB8 = 0; // RX1
    
    
    
    
    _LATE0 = 1;      //SAIDA LED A1
    _LATE1 = 1;      //SAIDA LED A2
    _LATE2 = 1;      //SAIDA LED A3
    _LATE3 = 1;      //SAIDA LED_A4
    _LATE4 = 1;      //SAIDA LED A5
    _LATE5 = 1;      //SAIDA LED A6
#ifdef PCB_V715
    _LATA1 = 1;      //SAIDA LED A7
    _LATA6 = 1;      //SAIDA LED A8
#else
    _LATD3 = 1;      //SAIDA LED A7
    _LATD2 = 1;      //SAIDA LED A8
#endif
    
    _LATE8 = 1;      //SAIDA LED TAG
    _LATE9 = 1;      //SAIDA LIGA PA
    
    _LATA10 = 1;     //SAIDA LED_ZIG
    _TRISG1 = 0;
    _LATG1 = 1;     //SAIDA RIO4
    _TRISG14 = 0;
    _LATG14 = 1;      //SAIDA LED0
    _RG14 = 1;      //SAIDA LED0
    _LATG0 = 1;      //SAIDA LED1
    
    _LATB7 = 1; // RX1
    _LATB8 = 1; // TX1
    _TRISG13 = 0;
    _LATG13 = 1;
    
}

void AlertaDeErro (void){
    
    while(1){
        AlertaSonoroDeErro();
    }
}

int TesteDeIO (volatile unsigned *entradaTris, volatile unsigned *entrada, int bitEntrada, volatile unsigned *saidaTris, volatile unsigned *saida, int bitSaida, int logica){
    unsigned int rascunho = 1;
    int resultado;
    
    rascunho = 1;
    rascunho = rascunho << bitEntrada;
    *entradaTris = *entradaTris | rascunho;
    
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);
    
    rascunho = 1;
    rascunho = rascunho << bitSaida;
    rascunho = ~rascunho;
    *saidaTris = *saidaTris & rascunho;
    rascunho = ~rascunho;
    *saida = *saida | rascunho;
    
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);

    rascunho = 1;
    rascunho = rascunho << bitEntrada;
    resultado = *entrada & rascunho;
    
    if (logica){
        if (resultado){
            resultado = 0;
        } else {
            resultado = 1;
        }
    }
        
    
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);
    if (resultado == 0){
        rascunho = 1;
        rascunho = rascunho << bitSaida;
        *saidaTris = *saidaTris | rascunho;
        return -1;
    }
    
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);

    rascunho = 1;
    rascunho = rascunho << bitSaida;
    rascunho = ~rascunho;
    *saida = *saida & rascunho;
    
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);
    
    rascunho = 1;
    rascunho = rascunho << bitEntrada;
    resultado = *entrada & rascunho;
    
    if (logica){
        if (resultado){
            resultado = 0;
        } else {
            resultado = 1;
        }
    }
    
    
        
    delay_ms(TEMPO_PARA_TESTE_DE_PINOS);
    asm("CLRWDT");
    if (resultado != 0){
        rascunho = 1;
        rascunho = rascunho << bitSaida;
        *saidaTris = *saidaTris | rascunho;
        return -1;
    }
    
    rascunho = 1;
    rascunho = rascunho << bitSaida;
    *saidaTris = *saidaTris | rascunho;
    
    
    return 1;
    
}


int TesteSerial1(void){
    int tamanho;
    char resposta[5];
    //enviaDadosNaSerial x e recebe por loop na mesma interface
    //uartTxByte('S');
    //uartTxByte('1');
    delay_ms(TEMPO_PARA_TESTE_DAS_SERIAIS);
    tamanho = uartRxNumBytesAvailable();
    if (tamanho != 0){
        //uartRxNBytes(resposta, &tamanho);
        if (strstr(resposta, "S1") != NULL){
            return 1;
        }
    }
    return -1;
}


int TesteMemoriaI2C (void){
    unsigned char dado;
    
    
    if (EscreverNaEEprom (0, 0x55)){
        return 0;
    }
    
    if (LerDadosDaEEprom (0, &dado)){
        return 0;
    }
    
    if (dado != 0x55){
        return 0;
    }
    
    return 1;
}

#define TEMPO_ESPERA_RTC 1000 //Em mS
int TesteRTC (void){
    //void SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano){
    SetarHoraRTC(0,10,20,30,5,25);
    
    //strstr(stringHora);
    delay_ms(TEMPO_ESPERA_RTC);
    testeI2C();
    poe_texto_XY(4,0,stringData);
    poe_texto_XY(4,12,stringHora);
    
    if (strstr(stringHora,"20:10:01") != NULL){
        return 1;
    }
    
    return 0;
}

int TesteOneWire(void){
    char rascunho[30];
    

    ObtemID(rascunho);
    
    if (strstr(rascunho, "FFFFFFFF") != NULL){
        return -1;
    } 
    
    
    return 1;
}


void LoopAutoTeste (void){
    //int valorParcial; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.

    limpa_display();
    
    //LigaTodosOsLeds();

    poe_texto_XY(0,0, "Teste 1/100");
    /*
    if (TesteDeIO(&TRISA, &PORTA, 0, &TRISG, &PORTG, 12, LOGICA_DIRETA) != 1){//GP8 e GP6 de P2
                         //0123456789012345
        poe_texto_XY(2,0, "Falha em GP8-GP6");
        AlertaDeErro();
    }
    */       
     
    poe_texto_XY(0,0, "Teste 4/100");
    if (TesteDeIO(&TRISB, &PORTB, 10, &TRISB, &PORTB, 11, LOGICA_DIRETA) != 1){//SEL_BBA -E- SEL_A1-4 de P2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha em BBA-A1-4");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 5/100");
    if (TesteDeIO(&TRISB, &PORTB, 12, &TRISB, &PORTB, 13, LOGICA_DIRETA) != 1){//SEL_B5-8 -E- SEL_A1-2 de P2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha SEL_B58 SELA12");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 6/100");
    if (TesteDeIO(&TRISB, &PORTB, 15, &TRISF, &PORTF, 13, LOGICA_DIRETA) != 1){//SEL_A3-4 -E- SEL_B5-6 de P2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha SEL_A34 SELB56");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 7/100");
    if (TesteDeIO(&TRISB, &PORTB, 15, &TRISF, &PORTF, 13, LOGICA_DIRETA) != 1){//SEL_B7-8 -E- TUNE_CAP1 de P2        
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha SEL_A34 SELB56");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 8/100");
    if (TesteDeIO(&TRISD, &PORTD, 14, &TRISF, &PORTF, 4, LOGICA_DIRETA) != 1){//TUNE_CAP2 -E- TUNE_CAP3 de P2    
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha TU_C2 e TU_C3");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 9/100");
    if (TesteDeIO(&TRISB, &PORTB, 2, &TRISE, &PORTE, 9, LOGICA_DIRETA) != 1){//EN -E- LIGA_PA
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha EN e LIGA_PA");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 10/100");
    if (TesteDeIO(&TRISB, &PORTB, 14, &TRISF, &PORTF, 8, LOGICA_DIRETA) != 1){//IRQ -E- NCS
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha IRQ e NCS");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 11/100");
    if (TesteDeIO(&TRISF, &PORTF, 5, &TRISF, &PORTF, 3, LOGICA_DIRETA) != 1){//MISO -E- MOSI
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha MISO e MOSI");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 12/100");
    if (TesteDeIO(&TRISD, &PORTD, 8, &TRISA, &PORTA, 3, LOGICA_DIRETA) != 1){//CLK -E- CLKSYS
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha CLK e CLKSYS");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 13/100");
    if (TesteDeIO(&TRISG, &PORTG, 7, &TRISC, &PORTC, 2, LOGICA_INVERSA) != 1){//SRL1 -E- OPTO2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha RL1 e OPTO2");
        AlertaDeErro();
        
    }
    
    poe_texto_XY(0,0, "Teste 14/100");
    if (TesteDeIO(&TRISG, &PORTG, 7, &TRISC, &PORTC, 1, LOGICA_INVERSA) != 1){//SRL3 -E- OPTO2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha RL3 e OPTO2");
        AlertaDeErro();
        
    }
    poe_texto_XY(0,0, "Teste 15/100");
    if (TesteDeIO(&TRISG, &PORTG, 6, &TRISG, &PORTG, 15, LOGICA_INVERSA) != 1){//RX2 -E- TX2    
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha RL2 e OPTO1");
        AlertaDeErro();
        
    }
    
    poe_texto_XY(0,0, "Teste 16/100");
    if (TesteDeIO(&TRISG, &PORTG, 6, &TRISB, &PORTB, 3, LOGICA_INVERSA) != 1){//SRL4 -E- OPTO1
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha RL4 e OPTO1");
        AlertaDeErro();
        
    }

    poe_texto_XY(0,0, "Teste 17/100");
    if (TesteOneWire() != 1){
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha no DS2401 U22");
        AlertaDeErro();
    }
    
    
    poe_texto_XY(0,0, "Teste 18/100");
    if (TesteMemoriaI2C() != 1) {
                         //01234567890123456789        
        poe_texto_XY(2,0, "Falha na Memoria I2C");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 19/100");
    if (TesteRTC () != 1){
                         //01234567890123456789        
        poe_texto_XY(2,0, "Falha no RTC");
        AlertaDeErro();
        
    }
    
    poe_texto_XY(0,0, "Teste 20/100");
    if (TesteDeIO(&TRISD, &PORTD, 1, &TRISD, &PORTD, 4, LOGICA_DIRETA) != 1){//RX3 e TX3
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX3 e TX3");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 21/100");
    if (TesteDeIO(&TRISD, &PORTD, 9, &TRISD, &PORTD, 5, LOGICA_DIRETA) != 1){//RX2 e TX2
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX2 e TX2");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 22/100");
    if (TesteDeIO(&TRISB, &PORTB, 7, &TRISB, &PORTB, 8, LOGICA_DIRETA) != 1){//RX1 e TX1
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX1 e TX1");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 23/100");
    if (TesteDeIO(&TRISG, &PORTG, 9, &TRISB, &PORTB, 9, LOGICA_DIRETA) != 1){//RX0 e TX0
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX0 e TX0");
        AlertaDeErro();
    }
                          
    _TRISA2 = 0;
    _LATA2 = 1;
    delay_ms(100);
    

    poe_texto_XY(0,0, "Teste 24/100");
    if (TesteDeIO(&TRISD, &PORTD, 1, &TRISD, &PORTD, 4, LOGICA_DIRETA) != 1){//RX3 -E- TX3
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX3 e TX3 232");
        AlertaDeErro();
        
    }
        
    poe_texto_XY(0,0, "Teste 25/100");
    if (TesteDeIO(&TRISD, &PORTD, 9, &TRISD, &PORTD, 5, LOGICA_DIRETA) != 1){//RX2 -E- TX2
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX2 e TX2 231");
        AlertaDeErro();
        
    }

    poe_texto_XY(0,0, "Teste 26/100");
    if (TesteDeIO(&TRISB, &PORTB, 7, &TRISB, &PORTB, 8, LOGICA_DIRETA) != 1){//RX1 -E- TX1
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX1 e TX1 232");
        AlertaDeErro();
        
    }
    
    poe_texto_XY(0,0, "Teste 27/100");    
    if (TesteDeIO(&TRISG, &PORTG, 9, &TRISB, &PORTB, 9, LOGICA_DIRETA) != 1){//RX0 -E- TX0
                         //01234567890123456789                
        poe_texto_XY(2,0, "Falha RX0 e TX0 232");
        AlertaDeErro();
        
    }
        
    poe_texto_XY(0,0, "Teste 28/100");
    if (TesteDeIO(&TRISC, &PORTC, 14, &TRISF, &PORTF, 0, LOGICA_DIRETA) != 1){//GP9 -E- GP10
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP9 E GP10");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 29/100");
    if (TesteDeIO(&TRISD, &PORTD, 3, &TRISD, &PORTD, 10, LOGICA_DIRETA) != 1){//GP12 -E- GP13
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP12 E GP13");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 30/100");
    if (TesteDeIO(&TRISD, &PORTD, 2, &TRISB, &PORTB, 6, LOGICA_DIRETA) != 1){//GP14 -E- GP15
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP14 E GP15");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 31/100");
    if (TesteDeIO(&TRISC, &PORTC, 13, &TRISB, &PORTB, 5, LOGICA_DIRETA) != 1){//GP1 -E- GP16
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP1 E GP16");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 32/100");
    if (TesteDeIO(&TRISC, &PORTC, 13, &TRISB, &PORTB, 5, LOGICA_DIRETA) != 1){//GP3 -E- GP4
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP3 E GP4");
        AlertaDeErro();
    }
    
    poe_texto_XY(0,0, "Teste 32/100");
    if (TesteDeIO(&TRISC, &PORTC, 13, &TRISB, &PORTB, 5, LOGICA_DIRETA) != 1){//GP3 -E- GP4
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP3 E GP4");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 33/100");
    if (TesteDeIO(&TRISA, &PORTA, 9, &TRISF, &PORTF, 2, LOGICA_DIRETA) != 1){//GP7 -E- GP2
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha GP2 E GP7");
        AlertaDeErro();
    }

    poe_texto_XY(0,0, "Teste 34/100");
    if (TesteDeIO(&TRISG, &PORTG, 8, &TRISA, &PORTA, 7, LOGICA_INVERSA) != 1){//ENT-AD -E- SAI_6
                         //01234567890123456789
        poe_texto_XY(2,0, "Falha ENT-AD E SAI-6");
        AlertaDeErro();
        
    }
    
    //AlertaDeErro();
    
    
    limpa_display();
    poe_texto_XY(2,0, "Sucesso");
    
    
    _LATA2 = 0;
   
    
    while(1){
        LigaTodosOsLeds();
        _TRISC4 = 0;
        _LATC4 = 1;//LIGO BUZZER
        delay_ms(300);
        _LATC4 = 0;//LIGO BUZZER
        delay_ms(100);
        
    }
        
}

int AutoTeste(void){
    while(1){
        LoopAutoTeste();
    }
}

