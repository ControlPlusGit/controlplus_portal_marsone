/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : auto_teste.H                                                      *
* Descricao : Header do auto_teste.c                                          *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao :                                                                    *
* Data : 25/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

#ifndef __AUTO_TESTE_H
#define __AUTO_TESTE_H

/*--------------------------------------------------------------------*/
/*                    ---+++>>> Constantes <<<+++---	              */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                     ---+++>>> Defines <<<+++---	                  */
/*--------------------------------------------------------------------*/

//Defines para o AUTO-TESTE que ocorre toda vez que o Leitor eh inicializado.
#define AUTO_TESTE                  // Comentar para desativar o autoteste
#ifdef AUTO_TESTE
    #define AUTO_TESTE_AS3993       // comentar para desativar apenas o teste da comunicacao com o CI AS3993
    #ifdef AUTO_TESTE_AS3993        //
        #define AUTO_TESTE_ANTENAS  // comentar para desativar o teste de varredura das antenas
        #define NUM_MAX_ANTENAS 8   //
    #endif
    #define AUTO_TESTE_GPRS         // comentar para desativar apenas o teste do GPRS
    #define AUTO_TESTE_ZIGBEE       // comentar para desativar apenas o teste do ZIGBEE
    #define AUTO_TESTE_MEM_I2C      // comentar para desativar apenas o teste da memoria serial i2c
    //#define AUTO_TESTE_GPS        //TODO: Fazer a implementacao de auto-teste para o GPS.
#endif

#define BUF_LEN_AUTO_TESTE  10
// Fim dos defines para o AUTO-TESTE

/*--------------------------------------------------------------------*/
/*                     ---+++>>> Structs <<<+++---	                  */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                    ---+++>>> Prototipagem <<<+++---	              */
/*--------------------------------------------------------------------*/

void autoTesteLeitor(void);
unsigned char autoTeste_AS3993(void);
unsigned char autoTeste_Antenas(void);
unsigned char autoTeste_GPRS(void);
unsigned char autoTeste_ZIGBEE(void);
unsigned char autoTeste_GPS(void);
unsigned int getResultadoAutoTeste(void);
void setResultadoAutoTeste(unsigned int resultado);
unsigned int *pBufferAutoTesteZigBee(void);
unsigned int *pBufferAutoTesteGPS(void);
unsigned int verifSeEhTagAutoTeste(unsigned char indice);
unsigned int getResultadoTesteAntenas(void);
void setResultadoTesteAntenas(unsigned int resultado);
unsigned int verifDiaAtual(void);
unsigned int verifHoraAtual(void);
void verifAutoTesteLeitor(unsigned char as3993_Error, unsigned char antena_Error,
                          unsigned char GPRS_Error,   unsigned char ZIGBEE_Error,
                          unsigned char MEM_Error,    unsigned char GPS_Error);
unsigned char autoTeste_mem24LC256(void);

/*--------------------------------------------------------------------*/
/*          ---+++>>> Fim do arquivo auto_teste.H <<<+++---           */
/*--------------------------------------------------------------------*/
#endif
