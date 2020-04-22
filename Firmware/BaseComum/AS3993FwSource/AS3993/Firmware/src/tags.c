/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : tags.C                                                            *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 28/11/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

#include "as3993_public.h"
#include "global.h"

#include "tags.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/


extern unsigned int readerInitStatus;
extern Tag __attribute__((far)) tags_[MAXTAG];

extern int pode_operar;
extern int tem_ze_na_area;
extern int total_parcial;
extern int  conta_tags_validas;
extern unsigned char pont_tag_atual;
extern int situacao;
extern unsigned char meu_operador[3];
extern unsigned char tag_valida;
extern __eds__ unsigned int __attribute__((far)) dbm_tag[MAXTAG][1];
extern __eds__ unsigned int __attribute__((far)) conta_leituras_dbm[MAXTAG];
//extern __eds__ unsigned int __attribute__((far)) serie_tag[MAXTAG][3];
extern unsigned char qt_operadores;
extern unsigned char tem_operador[30];
extern unsigned char operadores[30][3];
extern unsigned char qt_entrou[11];
extern __eds__ unsigned char __attribute__((far)) portais[10][300];

/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/

extern u8 inventoryGen2(void);

/********************************************************/
/*   ---+++>>> void limpa_buffer_tags(void) <<<+++---   */
/********************************************************/
/* Funcao: void limpa_buffer_tags(void)
 * Descricao
 * : Funcao para limpar o buffer das TAGs.
 * @param: numDeteccoes - eh a quantidade de TAGs detectadas.
 */
void limpa_buffer_tags(unsigned int numDeteccoes)
{
    int x;
    for(x = 0; x < numDeteccoes; x++)
    {
        tags_[x].epc[0] = 0;
        tags_[x].epc[1] = 0;
        tags_[x].epc[2] = 0;
        tags_[x].epc[3] = 0;
        tags_[x].epc[4] = 0;
        tags_[x].epc[5] = 0;
        tags_[x].epc[6] = 0;
        tags_[x].epc[7] = 0;
        tags_[x].epc[8] = 0;
        tags_[x].epc[9] = 0;
        tags_[x].epc[10] = 0;
        tags_[x].epc[11] = 0;

        //serie_tag[x][0] = 0;
        //serie_tag[x][1] = 0;
        //serie_tag[x][2] = 0;
    }
}

/**************************************************/
/*   ---+++>>> void troca_bytes(void) <<<+++---   */
/**************************************************/
/* Funcao: void troca_bytes(void)
 * Modificada em 19/12/2016 - o nome da funcao foi mantido, mas ja nao tem mais significado logico.
 * Descricao: Funcao para verificar se a TAG de maior RSSI pertence aa zona de exclusao.
 * @param: numDeteccoes - eh a quantidade de TAGs detectadas.
 */
void troca_bytes(unsigned int numDeteccoes)
{
    static unsigned int conta_ausencia_ze = 0;          // contador de ausencia da area de exclusao
    unsigned int indiceMaiorRSSI = 0;
    char buffer[4] = {0};
    char epcBuffer[25] = {0};
    unsigned int i = 0;
    const unsigned char EPC_LENGTH = 12;
    
    if(numDeteccoes)                                    // se houve deteccoes...
    {
        indiceMaiorRSSI = verifMaxRSSI(numDeteccoes);   // buscamos a TAG com maior RSSI

        memset(epcBuffer,0,strlen(epcBuffer));          // limpamos o buffer
        for(i=0; i < EPC_LENGTH; i++)
        {
            itoa(buffer,(unsigned long)tags_[indiceMaiorRSSI].epc[i],16); // converte o conteudo do EPC para ascii na base 16
            strcat(epcBuffer,buffer);                   // e armazena no buffer
        }
        
        poe_texto_XY(3,11,"QTD.:    ");
        poe_dado_XY(3,17,numDeteccoes);
        poe_texto_XY(4,0,"EPC: ");                      // limpamos o campo antes de preencher o valor
        poe_texto_XY(4,17,"   ");
        poe_texto_XY(4,5,epcBuffer);                    // exibe a TAG de maior RSSI no display LCD
        
        if(verifSeEhZonaExclusao(indiceMaiorRSSI))      // Verificamos se ela eh da zona de exclusao
        {
            if(pode_operar)
            {
                conta_ausencia_ze = 0;
                tem_ze_na_area = 1;
                total_parcial = 0;                        
            }   
        }
    }
    else
        tem_ze_na_area = 0;

    if(tem_ze_na_area)
    {
        total_parcial = 0;                          // zera a variavel global de numero de detecoes
        sel_led(7,1);                               // notifica que zona de exclusao foi detectada na area atraves do LED 7.
    }
    else
    {
        sel_led(7,0);
        tem_ze_na_area  = 0;
    }
}

//void troca_bytes(void)
//{
//    int x, y;
//    static unsigned int conta_ausencia_ze = 0;
//    
//    y = 0;
//    
//    for(x=0; x < MAXTAG; x++)
//    {
//        if (tags_[x].epc[0] == 0x50 || tags_[x].epc[0] == 0x30)      //somente o marcador de zona de tem o byte 11 em 1
//        {                          // ou nossas tags, empilhadeira tem no. de serie gigante
//            tags_[x].epc[11]=tags_[x].epc[2];       //vamos trocar os bytes
//            tags_[x].epc[10]=tags_[x].epc[1];       //para ficar MSB.MID.LSB
//            tags_[x].epc[9]=tags_[x].epc[0];        //empilhadeira = 0.1.2...
//        }
//#ifdef EMPILHADEIRA     //empilhadeira sÃƒÆ’Ã‚Â³ aceita tag 1 ou tag com comeÃƒÆ’Ã‚Â§o 0x50 ou 0x30
//        else if ((tags_[x].epc[0] == 0x10) && (tags_[x].epc[1] == 0) && (tags_[x].epc[2] == 0))//ve se eh ZE
//        {
//            tags_[x].epc[11]= 1;       //vamos trocar os bytes
//            tags_[x].epc[10]= 0;       //para ficar MSB.MID.LSB
//            tags_[x].epc[9]= 0;        //empilhadeira = 0.1.2...
//            if (pode_operar)
//            {
//                y++;
//                conta_ausencia_ze = 0;
//                tem_ze_na_area = 1;
//                total_parcial = 0;
//            }
//        }
//        else if ((tags_[x].epc[11] == 1) && (tags_[x].epc[10] == 0) && (tags_[x].epc[9] == 0))//ve se eh ZE
//        {
//            if (pode_operar)
//            {
//                y++;
//                conta_ausencia_ze = 0;
//                tem_ze_na_area = 1;
//                total_parcial = 0;
//            }
//        }
//        else
//        {
//            tags_[x].epc[11] = 0;
//            tags_[x].epc[10] = 0;
//            tags_[x].epc[9] = 0;
//        }
//#endif
//    }
//    if(conta_ausencia_ze < 65000)
//        conta_ausencia_ze++;
//    
//    if((conta_ausencia_ze < 4) && (tem_ze_na_area))
//    {
//        total_parcial = 0;
//        //LUCIANO: MODIFICACAO - Desativei o acionamento do LED 7
//        sel_led(7,1);
//        //LUCIANO: FIM DA MODIFICACAO
//    }
//    else
//    {
//        //LUCIANO: MODIFICACAO - Comentei o comando abaixo
//        sel_led(7,0);
//        //LUCIANO: FIM DA MODIFICCAO
//        tem_ze_na_area  = 0;
//    }
//}

/*******************************************************/
/*   ---+++>>> void pega_dados_tags (void) <<<+++---   */
/*******************************************************/
/* Funcao: void pega_dados_tags (void)
 * Descricao: Funcao para fazer tratamento da TAGs.
 * @param: numDeteccoes - eh a quantidade de TAGs detectadas.
 */
void pega_dados_tags(unsigned int numDeteccoes)
{

}

/**********************************************************/
/*   ---+++>>> void trata_tags_excessao(void) <<<+++---   */
/**********************************************************/
/* Funcao: void trata_tags_excessao(void)
 * Descricao: Funcao para fazer tratamento da TAGs.
 */
void trata_tags_excessao(void)
{

}

/***********************************************************************************/
/*   ---+++>>> unsigned int verifMaxRSSI(unsigned int tags_detectadas) <<<+++---   */
/***********************************************************************************/
/* Funcao: unsigned int verifMaxRSSI(unsigned int tags_detectadas)
 * Descricao: Funcao que encontra a TAG com o maior valor de RSSI^2
 * @param: tags_detectadas - eh a quantidade de TAGs que foram detectadas
 * @return: indiceMaxRSSI - eh o indice cuja a TAG teve o maior RSSI
 */
unsigned int verifMaxRSSI(unsigned int tags_detectadas)
{
    signed int rssi;
    unsigned int maxRSSI = 0;
    unsigned int i = 0, indiceMaxRSSI = 0;
        
    poe_texto_XY(3,6,"    ");                   // limpamos o campo de exibicao do RSSI
    
    for(i=0; i < tags_detectadas; i++)
    {
        rssi = calculaRSSI(i);                  // calculo do modulo do RSSI sem extrair a raiz quadrada
        
        //TESTE: se for TAG de Zona de Exclusao, somar o fator de correcao do rssi
        if(verifSeEhZonaExclusao(i))
        {   
            //rssi += FATOR_CORRECAO_TAG;         // soma o fator
            if(rssi <= 2)                               // se rssi menor que 2 (extremamente fraco)
                rssi += (FATOR_CORRECAO_TAG - ((FATOR_CORRECAO_TAG >> 2)+(FATOR_CORRECAO_TAG >> 3)));   // entao soma o fator-((fator/4)+(fator/8)) (faixa de 25 ~ 27)
            else if((rssi > 2) && (rssi <= 10))         // se rssi entre 2 e 10 (muito fraco)
                rssi += (FATOR_CORRECAO_TAG >> 1);      // entao soma 1/2 do fator (faixa de 22 ~ 30)
            else if((rssi > 10) && (rssi <= 30))        // se rssi entre 10 e 30 (fraco)
                rssi += (FATOR_CORRECAO_TAG >> 2);      // entao soma 1/4 do fator (faixa de 20 ~ 40)
            else if((rssi > 30) && (rssi <= 40))        // se rssi entre 30 e 40 (medio)
                rssi += (FATOR_CORRECAO_TAG >> 3);      // entao soma 1/8 do fator (faixa de 35 ~ 45)
            else if((rssi > 40) && (rssi <= 45))        // se rssi entre 40 e 45 (forte)
                rssi += (FATOR_CORRECAO_TAG >> 4);      // entao soma 1/16 do fator (faixa de 42 ~ 47)
            else if((rssi > 45) && (rssi <= 47))        // se rssi entre 45 e 47 (muito forte)
                rssi += (FATOR_CORRECAO_TAG >> 5);      // entao soma 1/32 do fator (faixa de 46 ~ 48)
            else if(rssi > 47)                          // se rssi maior que 47
                ;                                       // nao soma nada
        }
        // Fim do teste.
        
        if(maxRSSI <= rssi)
        {
            maxRSSI = rssi;                     // salvamos o maior RSSI
            indiceMaxRSSI = i;                  // e tambem o indice da respectiva TAG
        }
    }
    
    poe_texto_XY(3,0,"RSSI: ");                 // mostramos o valor do RSSI no display LCD
    poe_dado_XY(3,6,maxRSSI);                   // mostramos o modulo RSSI de maior valor
    
    return(indiceMaxRSSI);
}

/********************************************************************************************/
/*   ---+++>>> unsigned int verifSeEhZonaExclusao(unsigned int indiceMaiorRSSI) <<<+++---   */
/********************************************************************************************/
/* Funcao: unsigned int verifSeEhZonaExclusao(unsigned int indiceMaiorRSSI)
 * Descricao: Funcao que verifica se a TAG eh da zona de exclusao.
 * @param: indice - eh o indice da TAG a ser analisada
 * @return: 1 - se a TAG pertence a zona de exclusao, ou 0 se a TAG nao pertence a zzona de exclusao
 */

/*
unsigned int verifSeEhZonaExclusao(unsigned int indice)
{
    if((tags_[indice].epc[11] == 1) && (tags_[indice].epc[10] == 0) && (tags_[indice].epc[9] == 0) &&
       (tags_[indice].epc[8] == 0)  && (tags_[indice].epc[7] == 0)  && (tags_[indice].epc[6] == 0) &&
       (tags_[indice].epc[5] == 0)  && (tags_[indice].epc[4] == 0)  && (tags_[indice].epc[3] == 0) &&
       (tags_[indice].epc[2] == 0)  && (tags_[indice].epc[1] == 0)  && (tags_[indice].epc[0] == 0))   // eh uma TAG da zona de exclusao
    {
        return(1);                      // se sim, retorna 1
    }
    return(0);                          // caso contrario, retorna 0
}
*/

/************************************************************************************/
/*   ---+++>>> unsigned int verifSeEhOperarioComum(unsigned int indice) <<<+++---   */
/************************************************************************************/
/* Funcao: unsigned int verifSeEhOperarioComum(unsigned int indice)
 * Descricao: Funcao que verifica se a TAG eh de operario comum.
 * @param: indice - eh o indice da TAG a ser analisada
 * @return: 1 - se a TAG pertence a zona de exclusao, ou 0 se a TAG nao pertence a zzona de exclusao
 */
unsigned int verifSeEhOperarioComum(unsigned int indice)
{
    if(tags_[indice].epc[0] == 0x30)    // eh uma TAG de operario comum
    {
        return(1);                      // se sim, retorna 1
    }
    return(0);                          // caso contrario, retorna 0
}

/*******************************************************************************/
/*   ---+++>>> unsigned int verifSeEhCondutor(unsigned int indice) <<<+++---   */
/*******************************************************************************/
/* Funcao: unsigned int verifSeEhCondutor(unsigned int indice)
 * Descricao: Funcao que verifica se a TAG eh de condutor.
 * @param: indice - eh o indice da TAG a ser analisada
 * @return: 1 - se a TAG pertence a zona de exclusao, ou 0 se a TAG nao pertence a zzona de exclusao
 */
unsigned int verifSeEhCondutor(unsigned int indice)
{
    if(tags_[indice].epc[0] == 0x50)    // eh uma TAG de condutor
    {
        return(1);                      // se sim, retorna 1
    }
    return(0);                          // caso contrario, retorna 0
}

/*******************************************************************************/
/*   ---+++>>> unsigned int verifSeEhCondutor(unsigned int indice) <<<+++---   */
/*******************************************************************************/
/* Funcao: signed int calculaRSSI(unsigned int indice)
 * Descricao: Funcao que calculao RSSI da TAG detectada.
 * @param: indice - eh o indice da TAG a ser analisada
 * @return: valor do RSSI
 */

/*
signed int calculaRSSI(unsigned int indice)
{
    signed int rssi;
    signed int rssiI, rssiQ;

    rssiI = tags_[indice].rssi & 0x0F;           // pegamos a parte real do RSSI
    rssiQ = (tags_[indice].rssi >> 4) & 0x0F;    // pegamos a parte imaginaria do RSSI
    //rssi = (signed int)sqrt((rssiI*rssiI) + (rssiQ*rssiQ));   // calculo do modulo do RSSI
    rssi = (signed int)((rssiI*rssiI) + (rssiQ*rssiQ));         // calculo do modulo do RSSI sem extrair a raiz quadrada
    
    return(rssi);
}
*/

/************************************************/
/*   ---+++>>> void testeRSSI(void) <<<+++---   */
/************************************************/
/* Funcao: void testeRSSI(void)
 * Descricao: Funcao de teste de RSSI (apenas para testes)
 */
void testeRSSI(void)
{
    unsigned int i = 0, j = 0;
    unsigned int  tags_detectadas = 0;
    unsigned int indiceMaiorRSSI = 0;
    signed int rssi;
    signed int rssiI, rssiQ;
    const unsigned char EPC_LENGTH = 12;
    char buffer[4] = {0};
    char epcBuffer[25] = {0};
    
    limpa_display();
    
    while(42)
    {
        poe_texto_XY(1,0,"   Teste de RSSI    ");
        poe_texto_XY(2,0,"I: ");
        poe_texto_XY(2,7,"Q: ");
        poe_texto_XY(3,0,"RSSI: ");
        poe_texto_XY(3,10,"TAGs: ");
        poe_texto_XY(4,0,"EPC: ");
        tags_detectadas = inventoryGen2();
        if(tags_detectadas)
        {
            indiceMaiorRSSI = verifMaxRSSI(tags_detectadas);    // verifica qual TAG possui o maior RSSI
            if(verifSeEhZonaExclusao(indiceMaiorRSSI))          // verifica se a TAG eh da zona de exclusao
            {
                sel_led(7,1);                       // caso seja, acende o LED 7
            }
            else
            {
                sel_led(7,0);                       // caso contrario, apaga o LED 7
            }

            poe_texto_XY(2,3,"    ");               // limpamos os campos todas as vezes que alguma TAG eh detectada
            poe_texto_XY(2,10,"    ");
            poe_texto_XY(3,6,"    ");
            poe_texto_XY(3,16,"   ");
            poe_texto_XY(4,5,"               ");
            delay_ms(1000);                         // intervalo de 1 segundo para notarmos que ocorreu um novo ciclo de leitura
            for(j=0; j < tags_detectadas; j++)      // faz a varredura das TAGs detectadas
            {                
                memset(epcBuffer,0,strlen(epcBuffer));  // limpamos o buffer
                rssiI = tags_[j].rssi & 0x0F;           // pegamos a parte real do RSSI
                rssiQ = (tags_[j].rssi >> 4) & 0x0F;    // pegamos a parte imaginaria do RSSI
                rssi = calculaRSSI(j);                  // calculo do modulo do RSSI sem extrair a raiz quadrada
                for(i=0; i < EPC_LENGTH; i++)
                {
                    itoa(buffer,(unsigned long)tags_[j].epc[i],16); // converte o conteudo do EPC para ascii na base 16
                    strcat(epcBuffer,buffer);                       // e armazena no buffer
                }
                poe_texto_XY(2,3,"    ");           // limpamos o campo antes de preencher o valor
                poe_dado_XY(2,3,rssiI);             // mostramos o valor de I
                poe_texto_XY(2,10,"    ");          // limpamos o campo antes de preencher o valor
                poe_dado_XY(2,10,rssiQ);            // mostramos o valor de Q
                poe_texto_XY(3,6,"    ");           // limpamos o campo antes de preencher o valor
                poe_dado_XY(3,6,rssi);              // mostramos o modulo RSSI
                poe_texto_XY(3,16,"   ");           // limpamos o campo antes de preencher o valor
                poe_dado_XY(3,16,(j+1));            // mostramos o indice + 1 da TAG
                poe_texto_XY(3,17,"/");             // separador
                poe_dado_XY(3,18,tags_detectadas);  // mostramos a quantidade de TAGs detectadas
                poe_texto_XY(4,5,"               ");// limpamos o campo antes de preencher o valor
                poe_texto_XY(4,5,epcBuffer);        // exibe a TAG no display LCD
                delay_ms(4000);                     // delay de 4 segundos para podermos visualizar todos os valores
            }
        }
    }        
}

/*--------------------------------------------------------------------*/
/*              ---+++>>> Fim do arquivo tags.C <<<+++---             */
/*--------------------------------------------------------------------*/		
