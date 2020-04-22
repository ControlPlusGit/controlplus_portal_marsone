/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : mem_i2c_24LC256.H                                                 *
* Descricao : Header do mem_i2c_24LC256.c                                     *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v.1.24                                   *
* PIC : PIC24FJ256DA210                                                       *
* Versao :                                                                    *
* Data : 04/11/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

#ifndef __MEM_24LC256_H
#define __MEM_24LC256_H

/*--------------------------------------------------------------------*/
/*                   ---+++>>> Constantes <<<+++---	                  */
/*--------------------------------------------------------------------*/

#define PRIMEIRA_VEZ_FIRMWARE       0xAA

/*--------------------------------------------------------------------*/
/*                     ---+++>>> Defines <<<+++---	                  */
/*--------------------------------------------------------------------*/

#define TAMANHO_DO_NOME_NA_REDE_ZB 5

#define ADDR_CONFIG_DATA_RTC        0x00
#define ADDR_DIA_ULTIMO_AUTO_TESTE  0x01
#define ADDR_HORA_ULTIMO_AUTO_TESTE 0x02
#define ADDR_AUTO_TESTE_MEM         0x03
#define ADDR_NOME_NA_REDE_ZB         0x04
#define ENDERECO_DA_VELOCIDADE_JUIZ_DE_FORA 10

/*--------------------------------------------------------------------*/
/*                     ---+++>>> Structs <<<+++---	                  */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                    ---+++>>> Prototipagem <<<+++---	              */
/*--------------------------------------------------------------------*/

void mem_escreve_24LC256(unsigned int endereco, unsigned char dado);
void mem_escreveDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned char tamanho);
unsigned char mem_le_24LC256(unsigned int endereco);
void mem_leDados_24LC256(unsigned int endereco, unsigned char *dado, unsigned char tamanho);

/*--------------------------------------------------------------------*/
/*       ---+++>>> Fim do arquivo mem_i2c_24LC256.H <<<+++---         */
/*--------------------------------------------------------------------*/
#endif


