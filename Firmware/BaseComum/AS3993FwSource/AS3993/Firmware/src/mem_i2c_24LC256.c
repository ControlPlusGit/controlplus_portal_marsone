/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : mem_i2c_24LC256.C                                                 *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "timer.h"
#include "config_i2c3.h"
#include "mem_i2c_24LC256.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/
/***********************************************************************************************/
/*   ---+++>>> void mem_escreve_24LC256(unsigned int endereco, unsigned char dado) <<<+++---   */
/***********************************************************************************************/
// Funcao: void mem_escreve_24LC256(unsigned int endereco, unsigned char dado)
// Decricao: Escreve um byte na memoria 24LC256.
// @param: endereco da memoria
// @param: dado
void mem_escreve_24LC256(unsigned int endereco, unsigned char dado)
{
    union
    {
        struct
        {
            unsigned int parteBaixa:8;
            unsigned int parteAlta:8;
        };
        struct
        {
            unsigned int wTudo;       
        };    
    }unEnd;
     
    unEnd.wTudo = endereco;
    
    envia_start_i2c3();                     // Inicia a comunicacao
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(0xA0);              // controle de escrita + bit R/W = 0
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteAlta);   // envio do endereco (parte alta)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteBaixa);  // envio do endereco (parte baixa)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(dado);              // envia dado a ser escrito
    aguarda_i2c3();                         // espera fim do evento i2c
    
    envia_stop_i2c3();                      // envia stop
    delay_ms(10);                           // aguarda o fim da escrita
}

/********************************************************************************/
/*   ---+++>>> unsigned char mem_le_24LC256(unsigned int endereco,) <<<+++---   */
/********************************************************************************/
// Funcao: unsigned char mem_le_24LC256(unsigned int endereco,)
// Decricao: Escreve dados na memoria 24LC256. Dependendo do parametro 'tamanho'
// @param: endereco da memoria
// @param: endereco
// @return: dado lido
unsigned char mem_le_24LC256(unsigned int endereco)
{
    static unsigned char dado = 0;
    
    union
    {
        struct
        {
            unsigned int parteBaixa:8;
            unsigned int parteAlta:8;
        };
        struct
        {
            unsigned int wTudo;       
        };    
    }unEnd;
     
    unEnd.wTudo = endereco;
    
    envia_start_i2c3();
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(0xA0);              // controle de escrita + bit R/W = 0
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteAlta);   // envio do endereco (parte alta)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteBaixa);  // envio do endereco (parte baixa)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    envia_restart_i2c3();                   // envia restart
    aguarda_i2c3();                         // espera fim do evento i2c
   
    escreve_mestre_i2c3(0xA1);              // controle de escrita + bit R/W = 1
    aguarda_i2c3();                         // espera fim do evento i2c
    
    dado = le_mestre_i2c3();                // recebe dado a lido
    envia_nack_i2c3();                      // envia nack
    aguarda_i2c3();                         // espera fim do evento i2c
    
    envia_stop_i2c3();                      // fim da comunicacao
    aguarda_i2c3();                         // espera fim do evento i2c
    
    return(dado);
}

/**********************************************************************************************************************/
/*   ---+++>>> void mem_leDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned int tamanho) <<<+++---   */
/**********************************************************************************************************************/
// Funcao: void mem_leDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned int tamanho)
// Decricao: Escreve dados na memoria 24LC256. Dependendo do parametro 'tamanho'
// @param: endereco da memoria
// @param: *dado
// @param: tamanho (se tamanho igual a 0 sera feita uma leitura apenas)
void mem_leDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned char tamanho)
{
    static unsigned int i = 0;

    union
    {
        struct
        {
            unsigned int parteBaixa:8;
            unsigned int parteAlta:8;
        };
        struct
        {
            unsigned int wTudo;       
        };    
    }unEnd;
     
    unEnd.wTudo = endereco;
    
    envia_start_i2c3();
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(0xA0);              // controle de escrita + bit R/W = 0
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteAlta);   // envio do endereco (parte alta)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    escreve_mestre_i2c3(unEnd.parteBaixa);  // envio do endereco (parte baixa)
    aguarda_i2c3();                         // espera fim do evento i2c
    
    envia_restart_i2c3();                   // envia restart
    aguarda_i2c3();                         // espera fim do evento i2c
   
    escreve_mestre_i2c3(0xA1);              // controle de escrita + bit R/W = 1
    aguarda_i2c3();                         // espera fim do evento i2c
    
    i = 0;
    do
    {
        if(tamanho <= 1)
        {
            *(dado+i) = le_mestre_i2c3();   // recebe dado lido
            envia_nack_i2c3();              // envia nack (para finalizar leitura continua, OU ler apenas um dado)
            if(!tamanho)                    // se o tamanho for 0
                break;                      // sai do loop
        }
        else
        {
            *(dado+i) = le_mestre_i2c3();   // recebe dado lido    
            envia_ack_i2c3();               // envia ack (para leitura continua)
        }
        aguarda_i2c3();                     // espera fim do evento i2c
        delay_us(10);
        i++;
    }while(--tamanho);
    
    envia_stop_i2c3();                      // fim da comunicacao
    aguarda_i2c3();                         // espera fim do evento i2c
}

///******************************************************************************************************************************/
///*   ---+++>>> void mem_escreveDados_24LC256(unsigned int endereco, unsigned char *pDados, unsigned char tamanho) <<<+++---   */
///******************************************************************************************************************************/
//// Funcao: void mem_escreveDados_24LC256(unsigned int endereco, unsigned char *pDados, unsigned char tamanho)
//// Descricao: Escreve uma string na memoria 24LC256.
//// @param: endereco da memoria
//// @param: ponteiro para o dado a ser escrito
//// @param: quantidade de dados a serem escritos (se tamanho igual a 0 sera feita uma leitura apenas)
void mem_escreveDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned char tamanho)
{
  static unsigned char i = 0;
   
  for(i=0; ((i <= tamanho)); i++)
  {
    //ClrWdt();
    mem_escreve_24LC256(endereco+i, *(dado+i));
  }
}
 
/*************************************************************************************************************/
/*   ---+++>>> unsigned char *mem_leString_24LC256(unsigned int endereco, unsigned char tamanho) <<<+++---   */
/*************************************************************************************************************/
// Funcao: unsigned char *mem_leString_24LC256(unsigned int endereco, unsigned char tamanho)
// Descricao: Le uma string na memoria 24LC256
// @param: endereco da memoria
// @param: ponteiro de dados lida da memoria
// @param: quantidade de dados a serem escritos (se tamanho igual a 0 sera feita uma leitura apenas)
//void mem_leDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned char tamanho)
//{
//  static unsigned char i = 0;
//   
//  for(i=0; ((i <= tamanho)); i++)
//  {
//    //ClrWdt();
//    *(dado+i) = mem_le_24LC256(endereco+i);
//    delay_us(10);
//  }
//}

/*--------------------------------------------------------------------*/
/*        ---+++>>> Fim do arquivo mem_i2c_24LC256.C <<<+++---        */
/*--------------------------------------------------------------------*/		

//        //LUCIANO: TESTE da memoria serial 24LC256
//        inicializa_i2c3();
//        unsigned char dado[10] = {0};
//        //unsigned char dado = 0;
//        unsigned int endereco = 0x1000;
//        int flag = 0;
//        while(42)
//        {
//            strcpy((char*)dado,"Luciano");
//            //dado = 'L';
//            //mem_escreveDados_24LC256(endereco, &dado, 0);
//            mem_escreveDados_24LC256(endereco, dado, strlen((char*)dado));
//            strcpy((char*)dado,"0000000");
//            //dado = 0;
//            //mem_leDados_24LC256(endereco, &dado, 0);
//            mem_leDados_24LC256(endereco, dado, strlen((char*)dado));
//            if(!strcmp((char*)dado,"Luciano"))
//            //if(dado == 'L')
//            {
//                flag = !flag;
//                sel_led(1, flag);
//            }
//            delay_ms(500);
//        }
//        //LUCIANO: Fim do teste da memoria serial 24LC256