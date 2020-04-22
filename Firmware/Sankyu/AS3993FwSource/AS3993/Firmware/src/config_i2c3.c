/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : config_i2c3.C                                                     *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 04/11/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

#include "p24FJ256DA210.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "config_i2c3.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/
/*****************************************************/
/*   ---+++>>> void envia_ack_i2c3(void) <<<+++---   */
/*****************************************************/
// Funcao: void envia_ack_i2c3(void)
// Decricao: Rotina que gera ack = 0.
void envia_ack_i2c3(void)
{
    I2C3CONbits.ACKDT = 0;  // Acerto do estado do ack a ser enviado ACK = 0.
    I2C3CONbits.ACKEN = 1;  // Envia o ACK
}

/******************************************************/
/*   ---+++>>> void envia_nack_i2c3(void) <<<+++---   */
/******************************************************/
// Funcao: void envia_nack_i2c3(void)
// Decricao: Rotina que gera ack = 1.
void envia_nack_i2c3(void)
{
    I2C3CONbits.ACKDT = 1;  // Acerto do estado do ack a ser enviado ACK = 1.
    I2C3CONbits.ACKEN = 1;  // Envia o ACK
}

/***************************************************/
/*   ---+++>>> void aguarda_i2c3(void) <<<+++---   */
/***************************************************/
// Funcao: void aguarda_i2c3(void)
// Decricao: Aguarda o termino de alguma acao do barramento i2c.
void aguarda_i2c3(void)
{
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;

    Nop();                  // Aguarda delay de 4 CM's
    Nop();                  // para estabilizacao do bit
    Nop();                  // antes da leitura do mesmo.
    Nop();
    
    // Aguarda todos os estados do barramento i2c serem concluidos.
    while(I2C3CONbits.SEN || I2C3CONbits.PEN || I2C3CONbits.RCEN || 
          I2C3CONbits.ACKEN || I2C3STATbits.TRSTAT || I2C3CONbits.RSEN)
    {
        if(countTimeout++ > limCountTimeout)
    		break;              // sai do loop por timeout.
    }//;
}

/**************************************************************/
/*   ---+++>>> unsigned char le_mestre_i2c3(void) <<<+++---   */
/**************************************************************/
// Funcao: unsigned char le_mestre_i2c3(void)
// Decricao: Le informacao do barramento i2c.
// @return: Retorna o dado do buffer de recepcao.
unsigned char le_mestre_i2c3(void)
{
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;

    I2C3CONbits.RCEN = 1;       // Ativa a recepcao de dados
    aguarda_i2c3();             // Aguarda fim do evento i2c
    while(!I2C3STATbits.RBF)    // Aguarda a recepcao do dado
    {
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout.
    }//;   
    I2C3STATbits.I2COV = 0;     // Limpa bit overrun
    return(I2C3RCV);            // Retorna o dado do buffer de recepcao.
}

/******************************************************/
/*   ---+++>>> void inicializa_i2c3(void) <<<+++---   */
/******************************************************/
// Funcao: void inicializa_i2c3(void)
// Decricao: Faz a inicializacao do modulo i2c3.
void inicializa_i2c3(void)
{
    I2C3BRG = (unsigned int)(VELOCIDADE_I2C3 / (SYSCLK * 2) ) - 1;  // Acerto do baudrate (100KHz)
    //I2C3BRG = 58;               // baudrate = FCY/FSCL - FCY/10M - 1 = 100 KHz
                                // FCY = FOSC / 2
    I2C3CONbits.A10M = 0;       // Enderecamento de 10 bits desabilitado.
    I2C3CONbits.SCLREL = 1;     // Clock sempre habilitado.
    I2C3CONbits.I2CSIDL = 1;    // Modulo continua funcionando em modo idle.
    I2C3CONbits.DISSLW = 1;     // Desabilita controle de slew rate.
    I2C3CONbits.SMEN = 0;       // Schmitt Trigger conforme especificacao.
    I2C3CONbits.I2CEN = 1;      // Habilita o modulo i2c3.
}

/*--------------------------------------------------------------------*/
/*          ---+++>>> Fim do arquivo config_i2c3.C <<<+++---          */
/*--------------------------------------------------------------------*/		
