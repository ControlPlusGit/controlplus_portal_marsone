/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : perifericos.C                                                     *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 31/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

#include "platform.h"
#include "timer.h"
#include "perifericos.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/

unsigned char status_zig = 0;
unsigned char status_dir = 0;
unsigned char status_3g = 0;
extern int pode_operar;

/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/

/*****************************************************/
/*   ---+++>>> void desliga_led_vm(void) <<<+++---   */
/*****************************************************/
// Funcao: void desliga_led_vm(void)
// Decricao: 
// @param: 
void desliga_led_vm(void)
{
    //poe_texto_XY(2,18," ");
    desliga_rele4();
}

/**************************************************/
/*   ---+++>>> void liga_led_vm(void) <<<+++---   */
/**************************************************/
// Funcao: void liga_led_vm(void)
// Decricao: 
// @param: 
void liga_led_vm(void)
{
    //poe_texto_XY(2,18,"V");
    liga_rele4();
}

/*****************************************************/
/*   ---+++>>> void desliga_sirene(void) <<<+++---   */
/*****************************************************/
// Funcao: void desliga_sirene(void)
// Decricao: 
// @param: 
void desliga_sirene(void)
{
    //poe_texto_XY(2,19," ");
    desliga_rele2();
}

/**************************************************/
/*   ---+++>>> void liga_sirene(void) <<<+++---   */
/**************************************************/
// Funcao: void liga_sirene(void)
// Decricao: 
// @param: 
void liga_sirene(void)
{
    //poe_texto_XY(2,19,"S");
    liga_rele2();
}

/*************************************************/
/*   ---+++>>> void liga_rele1(void) <<<+++---   */
/*************************************************/
// Funcao: void liga_rele1(void)
// Decricao: 
// @param: 
void liga_rele1(void)
{
    ld_saidas (5,1);
}

/*************************************************/
/*   ---+++>>> void liga_rele2(void) <<<+++---   */
/*************************************************/
// Funcao: void liga_rele2(void)
// Decricao: 
// @param: 
void liga_rele2(void)
{
    ld_saidas (6,1);
}

/****************************************************/
/*   ---+++>>> void desliga_rele1(void) <<<+++---   */
/****************************************************/
// Funcao: void desliga_rele1(void)
// Decricao: 
// @param: 
void desliga_rele1(void)
{
    ld_saidas (5,0);
}

/****************************************************/
/*   ---+++>>> void desliga_rele2(void) <<<+++---   */
/****************************************************/
// Funcao: void desliga_rele2(void)
// Decricao: 
// @param: 
void desliga_rele2(void)
{
    ld_saidas (6,0);
}

/*************************************************/
/*   ---+++>>> void liga_rele3(void) <<<+++---   */
/*************************************************/
// Funcao: void liga_rele3(void)
// Decricao: 
// @param: 
void liga_rele3(void)
{
    ld_saidas (1,1);
}

/*************************************************/
/*   ---+++>>> void liga_rele4(void) <<<+++---   */
/*************************************************/
// Funcao: void liga_rele4(void)
// Decricao: 
// @param: 
void liga_rele4(void)
{
    ld_saidas (2,1);
}

/****************************************************/
/*   ---+++>>> void desliga_rele3(void) <<<+++---   */
/****************************************************/
// Funcao: void desliga_rele3(void)
// Decricao: 
// @param: 
void desliga_rele3(void)
{
    ld_saidas (1,0);
}

/****************************************************/
/*   ---+++>>> void desliga_rele4(void) <<<+++---   */
/****************************************************/
// Funcao: void desliga_rele4(void)
// Decricao: 
// @param: 
void desliga_rele4(void)
{
    ld_saidas (2,0);
}

/***************************************************/
/*   ---+++>>> void liga_saida_4(void) <<<+++---   */
/***************************************************/
// Funcao: void liga_saida_4(void)
// Decricao: 
// @param: 
void liga_saida_4(void)
{
    ld_saidas (4,1);
}

/******************************************************/
/*   ---+++>>> void desliga_saida_4(void) <<<+++---   */
/******************************************************/
// Funcao: void desliga_saida_4(void)
// Decricao: 
// @param: 
void desliga_saida_4(void)
{
    ld_saidas (4,0);
}

/**************************************************/
/*   ---+++>>> void liga_buzzer(void) <<<+++---   */
/**************************************************/
// Funcao: void liga_buzzer(void)
// Decricao: 
// @param: 
void liga_buzzer(void)
{
    ld_saidas (3,1);
}

/*****************************************************/
/*   ---+++>>> void desliga_buzzer(void) <<<+++---   */
/*****************************************************/
// Funcao: void desliga_buzzer(void)
// Decricao: 
// @param: 
void desliga_buzzer(void)
{
    ld_saidas (3,0);
}

/***************************************************/
/*   ---+++>>> void liga_led_zig(void) <<<+++---   */
/***************************************************/
// Funcao: void liga_led_zig(void)
// Decricao: 
// @param: 
void liga_led_zig(void)
{
    status_zig = 1 ;
    LED_ZIG(1);
}

/******************************************************/
/*   ---+++>>> void desliga_led_zig(void) <<<+++---   */
/******************************************************/
// Funcao: void desliga_led_zig(void)
// Decricao: 
// @param: 
void desliga_led_zig(void)
{
    status_zig = 0 ;
    LED_ZIG(0);
}

/****************************************************/
/*   ---+++>>> void liga_led_wifi(void) <<<+++---   */
/****************************************************/
// Funcao: void liga_led_wifi(void)
// Decricao: 
// @param: 
void liga_led_wifi(void)
{
    LED_WF(1);
}

/*******************************************************/
/*   ---+++>>> void desliga_led_wifi(void) <<<+++---   */
/*******************************************************/
// Funcao: void desliga_led_wifi(void)
// Decricao: 
// @param: 
void desliga_led_wifi(void)
{
    LED_WF(0);
}

/***************************************************/
/*   ---+++>>> void pisca_led_3g(void) <<<+++---   */
/***************************************************/
// Funcao: void pisca_led_3g(void)
// Decricao: 
// @param: 
void pisca_led_3g(void)
{
    if (status_3g == 1)desliga_led_3g();
    else liga_led_3g();
}

/**************************************************/
/*   ---+++>>> void liga_led_3g(void) <<<+++---   */
/**************************************************/
// Funcao: void liga_led_3g(void)
// Decricao: 
// @param: 
void liga_led_3g(void)
{
    status_3g = 1;
    LED_3G(1);
}

/*****************************************************/
/*   ---+++>>> void desliga_led_3g(void) <<<+++---   */
/*****************************************************/
// Funcao: void desliga_led_3g(void)
// Decricao: 
// @param: 
void desliga_led_3g(void)
{
    status_3g = 0;
    LED_3G(0);
}

/***************************************************/
/*   ---+++>>> void liga_led_gps(void) <<<+++---   */
/***************************************************/
// Funcao: void liga_led_gps(void)
// Decricao: 
// @param: 
void liga_led_gps(void)
{
    LED_GPS(1);
}

/******************************************************/
/*   ---+++>>> void desliga_led_gps(void) <<<+++---   */
/******************************************************/
// Funcao: void desliga_led_gps(void)
// Decricao: 
// @param: 
void desliga_led_gps(void)
{
    LED_GPS(0);
}

/***********************************************/
/*   ---+++>>> void liga_dir(void) <<<+++---   */
/***********************************************/
// Funcao: void liga_dir(void)
// Decricao: 
// @param: 
void liga_dir(void)
{
    status_dir = 1 ;
    SAIDA_DIR(1);
}

/**************************************************/
/*   ---+++>>> void desliga_dir(void) <<<+++---   */
/**************************************************/
// Funcao: void desliga_dir(void)
// Decricao: 
// @param: 
void desliga_dir(void)
{
    status_dir = 0 ;
    SAIDA_DIR(0);
}

/*************************************************************/
/*   ---+++>>> void sel_led(int led, int status) <<<+++---   */
/*************************************************************/
// Funcao: void sel_led(int led, int status)
// Decricao: 
// @param: 
void sel_led(int led, int status)
{
    switch (led)
    {
            case 0:     //led = 0 = poe estatus em todos
                LED_A1(status);
                LED_A2(status);
                LED_A3(status);
                LED_A4(status);
                LED_A5(status);
                LED_A6(status);
                LED_A7(status);
                LED_A8(status);
                break;
            case 8:
                LED_A4(status);
                break;
            case 7:
                LED_A3(status);
                break;
            case 6:
                LED_A2(status);
                break;
            case 5:
                LED_A1(status);
                break;
            case 4:
                LED_A8(status);
                break;
            case 3:
                LED_A7(status);
                break;
            case 2:
                LED_A6(status);
                break;
            case 1:
                LED_A5(status);
                break;
    }
}

/***************************************************/
/*   ---+++>>> void pow_on_modem(void) <<<+++---   */
/***************************************************/
// Funcao: void pow_on_modem(void)
// Decricao: 
// @param: 
void pow_on_modem(void)
{
     _LATA2 = 0;
}

/****************************************************/
/*   ---+++>>> void pow_off_modem(void) <<<+++---   */
/****************************************************/
// Funcao: void pow_off_modem(void)
// Decricao: 
// @param: 
void pow_off_modem(void)
{
     _LATA2 = 1;
}

/*****************************************************/
/*   ---+++>>> void liga_saida_pa (void) <<<+++---   */
/*****************************************************/
// Funcao: void liga_saida_pa (void)
// Decricao: 
// @param: 
void liga_saida_pa (void)
{

    //ld_pa (0);
    ld_pa (1);

}

/********************************************************/
/*   ---+++>>> void desliga_saida_pa (void) <<<+++---   */
/********************************************************/
// Funcao: void desliga_saida_pa (void)
// Decricao: 
// @param: 
void desliga_saida_pa (void)
{
#ifdef RF_NOVA
    ld_pa (1);
#else
    ld_pa (0);
#endif
}

/***************************************************/
/*   ---+++>>> void ld_pa(int status ) <<<+++---   */
/***************************************************/
// Funcao: void ld_pa(int status)
// Decricao: 
// @param: 
void ld_pa(int status )
{
    LIGA_PA(status);
}

/***************************************************/
/*   ---+++>>> void liga_led_tag(void) <<<+++---   */
/***************************************************/
// Funcao: void liga_led_tag(void)
// Decricao: 
// @param: 
void liga_led_tag(void)
{
     LED_TAG(1);
}

/******************************************************/
/*   ---+++>>> void desliga_led_tag(void) <<<+++---   */
/******************************************************/
// Funcao: void desliga_led_tag(void)
// Decricao: 
// @param: 
void desliga_led_tag(void)
{
     LED_TAG(0);
}

/*****************************************************************/
/*   ---+++>>> void ld_saidas(int saida, int status) <<<+++---   */
/*****************************************************************/
// Funcao: void ld_saidas(int saida, int status)
// Decricao: 
// @param: 
void ld_saidas(int saida, int status)
{
    switch (saida)
    {
        case 0:
            SAI_1(status);
            SAI_2(status);
            SAI_3(status);
            SAI_4(status);
            SAI_5(status);
            SAI_6(status);
            SAI_7(status);
            break;
        case 1:
            SAI_1(status);
            break;
        case 2:
            SAI_2(status);
            break;
        case 3:
            SAI_3(status);
            break;
        case 4:
            SAI_4(status);
            break;
        case 5:
            SAI_5(status);
            break;
        case 6:
            SAI_6(status);
            break;
        case 7:
            SAI_7(status);
            break;
    }
}

/******************************************************/
/*   ---+++>>> void sel_antena (int antx) <<<+++---   */
/******************************************************/
// Funcao: void sel_antena(int antx)
// Decricao: 
// @param: 
void sel_antena(int antx)
{
    //sel_led (antx,1);
    switch (antx)
    {
            case 1:                         //LUCIANO: Seleciona a antena 1
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(1);                //sel. 1-2
                SEL_A1_2(1);                //sel. A1
            break;
            case 2:                         //LUCIANO: Seleciona a antena 2
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(1);                //sel. 1-2
                SEL_A1_2(0);                //sel. A2
            break;
            case 3:                         //LUCIANO: Seleciona a antena 3
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(0);                //sel. 3-4
                SEL_A3_4(1);                //sel. A3
            break;
            case 4:                         //LUCIANO: Seleciona a antena 4
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(0);                //sel. 3-4
                SEL_A3_4(0);                //sel. A4
            break;
            case 5:                         //LUCIANO: Seleciona a antena 5
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(1);                //sel. 5-6
                SEL_B5_6(1);                //sel. A5
            break;
            case 6:                         //LUCIANO: Seleciona a antena 6
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(1);                //sel. 5-6
                SEL_B5_6(0);                //sel. A6
            break;
            case 7:                         //LUCIANO: Seleciona a antena 7
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(0);                //sel. 7-8
                SEL_B7_8(1);                //sel. A7
            break;
            case 8:                         //LUCIANO: Seleciona a antena 8
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(0);                //sel. 7-8
                SEL_B7_8(0);                //sel. A8
            break;
            default:                        //LUCIANO:
                break;
    }
}

/***************************************************/
/*   ---+++>>> void sel_cap_cin(int x) <<<+++---   */
/***************************************************/
// Funcao: void sel_cap_cin(int x)
// Decricao: 
// @param: 
void sel_cap_cin(int x)
{
    TUNE_CAP2(0);
    TUNE_CAP3(0);
    TUNE_CAP1(x);
}

/****************************************************/
/*   ---+++>>> void sel_cap_cout(int x) <<<+++---   */
/****************************************************/
// Funcao: void sel_cap_cout(int x)
// Decricao: 
// @param: 
void sel_cap_cout(int x)
{
    TUNE_CAP1(0);
    TUNE_CAP3(0);
    TUNE_CAP2(x);
}

/****************************************************/
/*   ---+++>>> void sel_cap_clen(int x) <<<+++---   */
/****************************************************/
// Funcao: void sel_cap_clen(int x)
// Decricao: 
// @param: 
void sel_cap_clen(int x)
{
    TUNE_CAP1(0);
    TUNE_CAP2(0); 
    TUNE_CAP3(x);
}

/***************************************************/
/*   ---+++>>> void bloqueia_reh(void) <<<+++---   */
/***************************************************/
// Funcao: void bloqueia_reh(void)
// Decricao: 
// @param: 
/*
void bloqueia_reh(void)
{
    desliga_rele1();
    liga_led_vm ();
    if(pode_operar)
    {
        liga_sirene();
        delay_ms(200);
        desliga_sirene();
        delay_ms(3000);
    }
}
*/
/*************************************************/
/*   ---+++>>> void libera_reh(void) <<<+++---   */
/*************************************************/
// Funcao: void libera_reh(void)
// Decricao: 
// @param: 
void libera_reh(void)
{
    //poe_texto_XY(2,17," ");
//#ifdef SANKYU
//    desliga_rele1();
//#else
    liga_rele1();
    
//#endif
}

/******************************************************/
/*   ---+++>>> void bloqueia_frente(void) <<<+++---   */
/******************************************************/
// Funcao: void bloqueia_frente(void)
// Decricao: 
// @param: 
/*
void bloqueia_frente(void)
{
    //poe_texto_XY(2,16,"F");
    desliga_rele3();
    liga_led_vm ();
    if(pode_operar)
    {
        liga_sirene();
        delay_ms(200);
        desliga_sirene();
        delay_ms(3000);
    }
}
*/
/****************************************************/
/*   ---+++>>> void libera_frente(void) <<<+++---   */
/****************************************************/
// Funcao: void libera_frente(void)
// Decricao: 
// @param: 
void libera_frente(void)
{
    //poe_texto_XY(2,16," ");
    liga_rele3();
}

/*********************************************/
/*   ---+++>>> void LigaRX(void) <<<+++---   */
/*********************************************/
// Funcao: void LigaRX(void)
// Decricao: 
// @param: 
void LigaRX(void)
{
    RPINR18 = 0x1F07;//RX1
    RPINR19 = 0x0004;//RX2
    RPINR17 = 0x1800;//RX3
    RPINR27 = 0x001B;//RX0
    IPC2 = 0x4000;
    //U1MODE = 0x8000;

    IEC0bits.U1RXIE = 1;
    IEC1bits.U2RXIE = 1;
    IEC5bits.U3RXIE = 1;
    IEC5bits.U4RXIE = 1;
}

/*--------------------------------------------------------------------*/
/*          ---+++>>> Fim do arquivo perifericos.C <<<+++---          */
/*--------------------------------------------------------------------*/		
