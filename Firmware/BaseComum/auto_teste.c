/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : auto_teste.C                                                      *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 25/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include "string.h"
#include "gprs.h"

#include "as3993_public.h"
#include "global.h"

#include "auto_teste.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/

extern int antena_atual;
extern unsigned int readerInitStatus;
extern Tag __attribute__((far)) tags_[MAXTAG];
unsigned int auto_teste_concluido = 0;
unsigned int bufferAutoTesteZigBee[BUF_LEN_AUTO_TESTE] = {0};
unsigned int bufferAutoTesteGPS[BUF_LEN_AUTO_TESTE] = {0};

extern const enum ESTADOS_MAQUINA_COMANDOS {
    ESTADO_ENVIAR_AT_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_AT_CTZU_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_AT_CCLK_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATZ_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATH_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATE1_MAQUINA_COMANDOS,

    ESTADO_ENVIAR_CGATT_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_CPOS_MAQUINA_COMANDOS,

    ESTADO_ENVIAR_CGDCONT_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSD1_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSD2_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSD3_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSD4_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSD7_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSDA3_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UPSND8_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTP1_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTP2_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTP3_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTP6_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTPC1_MAQUINA_COMANDOS,
    ESTADO_ESPERAR_UUFTPCR_MAQUINA_COMANDOS,
    ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_UFTPC7_MAQUINA_COMANDOS,
    ESTADO_ESPERAR_CONNECT_DO_FTP_MAQUINA_COMANDOS,

    ESTADO_ESPERAR_PARA_ENVIAR_3MAIS_MAQUINAS_COMANDOS,

    ESTADO_ENVIAR_3MAIS_MAQUINA_COMANDOS,
    ESTADO_DESLOGA_DO_FTP_MAQUINA_COMANDOS,
    ESTADO_DESCONECTA_DA_REDE_MAQUINA_COMANDOS,

    ESTADO_ENVIAR_CIMI_MAQUINA_COMANDOS,
    ESTADO_ESPERAR_CIMI__RESPOSTA_MAQUINA_COMANDOS,
    ESTADO_ESPERAR_RESPOSTA_MAQUINA_COMANDOS,
    ESTADO_ESPERAR_RESPOSTA_NAO_SOLICITADA_MAQUINA_COMANDOS,
    ESTADO_ATRASO_MAQUINA_COMANDOS,
    ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS,
    ESTADO_OCIOSO_MAQUINA_COMANDOS,
    ESTADO_SUCESSO_MAQUINA_COMANDOS,
    ESTADO_ERRO_MAQUINA_COMANDOS
} MAQUINA_COMANDOS;

/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/

extern u8 inventoryGen2(void);

/******************************************************/
/*   ---+++>>> void autoTesteLeitor(void) <<<+++---   */
/******************************************************/
/* AUTO-TESTE: Implementacao feita em 17/10/2016
 * Funcao: autoTesteLeitor
 * Descricao: Realiza um auto teste basico dos principais sub-modulos do Leitor.
 */
void autoTesteLeitor(void)
{
    unsigned char as3993_Error = 0;
    unsigned char antena_Error = 0;
    unsigned char GPRS_Error = 0;
    unsigned char ZIGBEE_Error = 0;
    unsigned char GPS_Error = 0;
    unsigned char aciona_flag = 0;
    
    limpa_display();
    poe_texto_XY(0,0,"     AUTO TESTE     ");
    
    setResultadoAutoTeste(0);               // inicia como se houvesse algum problema.
    
    as3993_Error = autoTeste_AS3993();
    
    if(!as3993_Error)                       // se nao deu problemas na comunicacao com o CI AS3993
        antena_Error = autoTeste_Antenas(); // entao faz o auto-teste das antenas
    
    GPRS_Error = autoTeste_GPRS();
    ZIGBEE_Error = autoTeste_ZIGBEE();
    GPS_Error = autoTeste_GPS();
    
    // Verificamos se houve algum erro em algum subsistema do leitor
    if(as3993_Error || GPRS_Error || ZIGBEE_Error || antena_Error || GPS_Error)
    {
        limpa_display();
        // se sim, escreve no display o ocorrido.
        if(as3993_Error || antena_Error)
            poe_texto_XY(1,0,"Erro no sist. de RF!");
        
        if(GPRS_Error)
            poe_texto_XY(2,0,"Erro no sist. GPRS!");
        
        if(ZIGBEE_Error)
            poe_texto_XY(3,0,"Erro no sist. XBEE!");
        
        if(GPS_Error)
            poe_texto_XY(4,0,"Erro na com. GPS!");
        
        // e tambem bloqueia 'frente' e 're'
        bloqueia_frente();
        bloqueia_reh();
        
        while(1)            // Se ocorreu algum erro. Fica travado neste ponto.
        {
            // aciona buzzer e o led correspondente
            delay_ms(300);
            aciona_flag = !aciona_flag;
            ld_saidas (3,aciona_flag);       // comanda o acionamento do buzzer conforme a flag 'aciona_flag'
            if(as3993_Error)
                LIGA_PA(aciona_flag);
            
            if(antena_Error)
                sel_led(antena_atual,aciona_flag);
            
            if(GPRS_Error)
                LED_3G(aciona_flag);    // comanda o LED 3G do GPRS conforme a flag 'aciona_flag'
            
            if(ZIGBEE_Error)
                LED_ZIG(aciona_flag);   // comanda o LED do ZIGBEE conforme a flag 'aciona_flag'
            
            if(GPS_Error)
                LED_GPS(aciona_flag);   // comanda o LED do GPS conforme a flag 'aciona_flag'
        }
    }
    else
    {
        setResultadoAutoTeste(1);       // sinaliza que o auto teste acabou com sucesso.
    }
}

/****************************************************************/
/*   ---+++>>> unsigned char autoTeste_AS3993(void) <<<+++---   */
/****************************************************************/
/* Funcao: unsigned char autoTeste_AS3993(void)
 * Descricao: Faz um auto-teste na comunicacao SPI do AS3993.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_AS3993(void)
{
    unsigned char as3993_Error = 0;
    
#ifdef AUTO_TESTE_AS3993    // Se definido para testar o CI AS3993    
    readerInitStatus = as3993Initialize(915750);
    if(readerInitStatus)    // Se leitura for diferente de '0'
    {
        as3993_Error = 1;   // reporta problema com o CI as3993
    }
    else
    {
        as3993_Error = 0;   // caso contrario, esta tudo ok.
    }
    return(as3993_Error);
#else                       // Se nao estiver definido auto-teste para o CI as3993
    as3993_Error = 0;       // apenas sinaliza que nao ha erros.
    return(as3993_Error);
#endif
}

/*****************************************************************/
/*   ---+++>>> unsigned char autoTeste_Antenas(void) <<<+++---   */
/*****************************************************************/
/* Funcao: unsigned char autoTeste_Antenas(void)
 * Descricao: Faz um auto-teste nas antenas.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_Antenas(void)
{
    unsigned char antena_Error = 0;
    
#ifdef AUTO_TESTE_AS3993    // Se definido para testar o CI AS3993    
    unsigned int  tags_detectadas = 0;
    unsigned char i = 0;    // variavel de controle do laco for
    unsigned char j = 0;    // variavel de controle do laco for
    antena_atual = 0;       // variavel global, forca valor para zero, assim a proxima antena escolhida sera a antena 1 (pela funcao escolhe_antena())

    for(i=0; i < NUM_MAX_ANTENAS; i++)  // faz a varredura das antenas
    {        
        sel_led(0,0);                   // apaga todos os leds
        escolhe_antena();
        sel_antena(antena_atual);       // seleciona a antena
        
        for(j=0; j < 100; j++)          // fazemos cem varreduras
        {
            tags_detectadas = inventoryGen2();
            if(tags_detectadas)         // se detectar algo antes...
                break;                  // sai do laco for
        }
        
        if(tags_detectadas)
        {
            for(j=0; j < tags_detectadas; j++)   // faz a varredura das TAGs detectadas
            {
                if((tags_[j].epc[0] == 'C') && (tags_[j].epc[1]  == 'O') && (tags_[j].epc[2]  == 'N') &&
                   (tags_[j].epc[3] == 'T') && (tags_[j].epc[4]  == 'R') && (tags_[j].epc[5]  == 'O') &&
                   (tags_[j].epc[6] == 'L') && (tags_[j].epc[7]  == '-') && (tags_[j].epc[8]  == 'P') &&
                   (tags_[j].epc[9] == 'P') && (tags_[j].epc[10] == 'U') && (tags_[j].epc[11] == 'S'))
                {
                    antena_Error = 0;   // se encontrar a TAG de AUTO-TESTE, reporta que esta tudo OK.
                    break;              // sai do laco for interno apenas e vai para a proxima antena.
                }
            }
        }
        else
        {
            antena_Error = 1;           // reporta erro
            break;                      // e sai do laco for externo.
        }
    }
    return(antena_Error);
#else                       // Se nao estiver definido auto-teste para o CI as3993
    antena_Error = 0;       // 
    return(antena_Error);
#endif
}

/**************************************************************/
/*   ---+++>>> unsigned char autoTeste_GPRS(void) <<<+++---   */
/**************************************************************/
/* Funcao: unsigned char autoTeste_GPRS(void)
 * Descricao: Faz um auto-teste na comunicacao com o modulo GPRS.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_GPRS(void)
{
    unsigned char GPRS_Error = 0;
    
#ifdef AUTO_TESTE_GPRS      // Se definido para testar o GPRS
    unsigned int estado_GPRS = 0;
        
    estado_GPRS = MaquinaDeEstadosDeComandosGPRS(ESTADO_ENVIAR_AT_MAQUINA_COMANDOS);
    
    if(estado_GPRS == ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS)
    {
        estado_GPRS = MaquinaDeEstadosDeComandosGPRS(ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS);
        while(estado_GPRS == ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS)
        {
            estado_GPRS = MaquinaDeEstadosDeComandosGPRS(ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS);
        }
        if(estado_GPRS == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            GPRS_Error = 0; // Sinaliza que nao ha erros no GPRS.
        if(estado_GPRS == ESTADO_ERRO_MAQUINA_COMANDOS)
            GPRS_Error = 1; // Reporta erro na GPRS.
    }
    return(GPRS_Error);
#else                       // Se nao estiver definido auto-teste para GPRS
    GPRS_Error = 0;         // apenas sinaliza que nao ha erros.
    return(GPRS_Error);
#endif
}

/****************************************************************/
/*   ---+++>>> unsigned char autoTeste_ZIGBEE(void) <<<+++---   */
/****************************************************************/
/* Funcao: unsigned char autoTeste_ZIGBEE(void)
 * Descricao: Faz um auto-teste na comunicacao com o modulo ZIGBEE.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_ZIGBEE(void)
{
    unsigned char ZIGBEE_Error = 0;
    
#ifdef AUTO_TESTE_ZIGBEE
    unsigned int m = 0;
    for(m=0; m < 10; m++)
    {
        putsUART4((unsigned int *)"+++");   // Envia '+++' para testar o Zigbee (ele entra no modo comando e responde 'OK')

        delay_ms(1000);

        if(strstr((char *)bufferAutoTesteZigBee,"OK"))
        {
            ZIGBEE_Error = 0;   // se encontrou o 'OK', esta tudo certo.
            putsUART4((unsigned int *)"ATCN\r\n");  // envia 'ATCN' para o Zigbee sair do modo comando
            break;              // e sai do laco 'for'
        }
        else
        {
            ZIGBEE_Error = 1;   // caso contrario, sinaliza erro.
        }
    }
    return(ZIGBEE_Error);
#else
    ZIGBEE_Error = 0;           // apenas sinaliza que nao ha erros.
    return(ZIGBEE_Error);
#endif
}

/*************************************************************/
/*   ---+++>>> unsigned char autoTeste_GPS(void) <<<+++---   */
/*************************************************************/
/* Funcao: unsigned char autoTeste_GPS(void)
 * Descricao: Faz um auto-teste na comunicacao com o modulo GPS.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_GPS(void)
{
    unsigned char GPS_Error = 0;
    
#ifdef AUTO_TESTE_GPS
    //TODO: Implementar o auto-teste para o modulo GPS na rotina de recepcao.
    // adicionar o codigo abaixo na rotina de tratamento de dados recebidos pela uart1 ...
    /*
    if(DataRdyUART1())  // sera mesmo a uart1 ???
    {
        if(!getResultadoAutoTeste())   // So entra neste if se estiver em auto teste
            getsUART1((BUF_LEN_AUTO_TESTE-1), pBufferAutoTesteGPS(), 100);
    }
    */
    //Fim do TODO:
    
    unsigned int m = 0;
    for(m=0; m < 10; m++)
    {
        delay_ms(1000);

        if(strstr((char *)bufferAutoTesteGPS,"RMC"))
        {
            GPS_Error = 0;      // se encontrou o 'OK', esta tudo certo.
            break;              // e sai do laco 'for'
        }
        else
        {
            GPS_Error = 1;      // caso contrario, sinaliza erro.
        }
    }
#else
    GPS_Error = 0;              // apenas sinaliza que nao ha erros.
    return(GPS_Error);
#endif
}

/********************************************************************/
/*   ---+++>>> unsigned int getResultadoAutoTeste(void) <<<+++---   */
/********************************************************************/
/* Funcao: unsigned int getResultadoAutoTeste(void)
 * Descricao: Le o resultado do auto-teste.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned int getResultadoAutoTeste(void)
{
    return(auto_teste_concluido);
}

/******************************************************************************/
/*   ---+++>>> void setResultadoAutoTeste(unsigned int resultado) <<<+++---   */
/******************************************************************************/
/* Funcao: void setResultadoAutoTeste(unsigned int resultado)
 * Descricao: Reporta o resultado do auto-teste.
 * @param: Configura 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
void setResultadoAutoTeste(unsigned int resultado)
{
    auto_teste_concluido = resultado;
}

/**********************************************************************/
/*   ---+++>>> unsigned int *pBufferAutoTesteZigBee(void) <<<+++---   */
/**********************************************************************/
/* Funcao: unsigned int *pBufferAutoTesteZigBee(void)
 * Descricao: Retorna o endereco do buffer de auto-teste do modulo ZigBee.
 * @return: Retorna o endereco do buffer de auto-teste do modulo ZigBee.
 */
unsigned int *pBufferAutoTesteZigBee(void)
{
    return(bufferAutoTesteZigBee);
}

/*******************************************************************/
/*   ---+++>>> unsigned int *pBufferAutoTesteGPS(void) <<<+++---   */
/*******************************************************************/
/* Funcao: unsigned int *pBufferAutoTesteGPS(void)
 * Descricao: Retorna o endereco do buffer de auto-teste do modulo GPS.
 * @return: Retorna o endereco do buffer de auto-teste do modulo de GPS.
 */
unsigned int *pBufferAutoTesteGPS(void)
{
    return(bufferAutoTesteGPS);
}

/*--------------------------------------------------------------------*/
/*           ---+++>>> Fim do arquivo auto_teste.C <<<+++---          */
/*--------------------------------------------------------------------*/		
