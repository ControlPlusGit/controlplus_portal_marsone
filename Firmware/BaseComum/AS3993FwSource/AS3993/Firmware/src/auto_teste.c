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
#include "mem_i2c_24LC256.h"
#include "tags.h"
#include "rtc.h"

#include "as3993_public.h"
#include "global.h"

#include "auto_teste.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/

extern int antena_atual;
extern unsigned int readerInitStatus;
extern Tag __attribute__((far)) tags_[MAXTAG];

extern char stringData[20];
extern char stringHora[20];

unsigned int auto_teste_concluido = 0;
unsigned int auto_teste_antenas_concluido = 0;
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
    unsigned char MEM_Error = 0;
    unsigned char GPS_Error = 0;
    //unsigned char aciona_flag = 0;
    
    limpa_display();
    poe_texto_XY(0,0,"     AUTO TESTE     ");
    
    setResultadoAutoTeste(0);               // inicia como se houvesse algum problema.
    
    as3993_Error = autoTeste_AS3993();      // testa a comunicacao SPI com CI AS3993
    
    if(!as3993_Error)                       // se nao deu problemas na comunicacao com o CI AS3993
        antena_Error = autoTeste_Antenas(); // entao faz o auto-teste das antenas
    
    setResultadoTesteAntenas(!antena_Error); // salva o resultado (a logica aqui eh invertida) do teste das antenas
    
    GPRS_Error = autoTeste_GPRS();          // testa comunicacao com o MODEM
    ZIGBEE_Error = autoTeste_ZIGBEE();      // testa a comunicacao com o ZIGBEE
    MEM_Error = autoTeste_mem24LC256();     // testa a comunicacao com a memoria serial i2c
    GPS_Error = autoTeste_GPS();            // testa a comunicacao com o GPS
    
    // Verificamos se houve algum erro em algum subsistema do leitor
    verifAutoTesteLeitor(as3993_Error, antena_Error, GPRS_Error, ZIGBEE_Error, MEM_Error, GPS_Error);
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
    //readerInitStatus = as3993Initialize(915750);
    readerInitStatus = as3993Initialize(902750);
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
    
#ifdef AUTO_TESTE_ANTENAS    // Se definido para testar o CI AS3993    
    unsigned int  tags_detectadas = 0;
    unsigned char i = 0;    // variavel de controle do laco for
    unsigned char j = 0;    // variavel de controle do laco for
    antena_atual = 0;       // variavel global, forca valor para zero, assim a proxima antena escolhida sera a antena 1 (pela funcao escolhe_antena())

    for(i=0; i < NUM_MAX_ANTENAS; i++)  // faz a varredura das antenas por 8 ciclos completos
    {        
        sel_led(0,0);                   // apaga todos os leds
        //escolhe_antena();
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
                if(verifSeEhTagAutoTeste(j))
                {
                    antena_Error = 0;   // se encontrar a TAG de AUTO-TESTE, reporta que esta tudo OK.
                    break;              // sai do laco for interno apenas e vai para a proxima antena.
                }

                if((antena_atual == 6) && ((verifSeEhCondutor(j)) || (verifSeEhOperarioComum(j))))
                {
                    antena_Error = 0;   // se estiver selecionada a antena 6 e a TAG comecar com 5 (TAG de condutor) ou 3 (TAG operador comum), reporta que esta tudo OK.
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
    antena_atual = 0;
    sel_led(0,0);                       // apaga todos os leds
    //escolhe_antena();
    sel_antena(antena_atual);           // seleciona a antena
    limpa_buffer_tags(MAXTAG);          // limpa buffer de TAGs
    return(antena_Error);
#else                       // Se nao estiver definido AUTO_TESTE_ANTENAS para a varredura das antenas
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

        if(strstr((char *)pBufferAutoTesteZigBee(),"OK"))
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

        if(strstr((char *)pBufferAutoTesteGPS(),"RMC"))
        {
            GPS_Error = 0;      // se encontrou o 'OK', esta tudo certo.
            break;              // e sai do laco 'for'
        }
        else
        {
            GPS_Error = 1;      // caso contrario, sinaliza erro.
        }
    }
    return(GPS_Error);
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
 * @return: Retorna 0 se algo errado ocorreu, ou 1 se tudo estiver OK.
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
 * @param: Configura 0 se algo errado ocorreu, ou 1 se tudo estiver OK.
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

/********************************************************************/
/*   ---+++>>> unsigned int verifSeEhTagAutoTeste(void) <<<+++---   */
/********************************************************************/
/* Funcao: unsigned int verifSeEhTagAutoTeste(void)
 * Descricao: Verifica se a TAG detectada eh a TAG de auto-teste.
 * @return: Retorna 1 se sim, caso contrario retorna 0.
 */
unsigned int verifSeEhTagAutoTeste(unsigned char indice)
{
    //AUTO-TESTE: Verifica se eh a TAG de AUTO-TESTE (implementacao feita em 17/10/2016)
    if((tags_[indice].epc[0] == 'C') && (tags_[indice].epc[1]  == 'O') && (tags_[indice].epc[2]  == 'N') && // se forem as TAGs de AUTO-TESTE...
       (tags_[indice].epc[3] == 'T') && (tags_[indice].epc[4]  == 'R') && (tags_[indice].epc[5]  == 'O') &&
       (tags_[indice].epc[6] == 'L'))
    {
        return(1);
    }
    return(0);
}

/***********************************************************************/
/*   ---+++>>> unsigned int getResultadoTesteAntenas(void) <<<+++---   */
/***********************************************************************/
/* Funcao: unsigned int getResultadoTesteAntenas(void)
 * Descricao: Le o resultado do teste das antenas.
 * @return: Retorna 0 se algo errado ocorreu, ou 1 se tudo estiver OK.
 */
unsigned int getResultadoTesteAntenas(void)
{
    return(auto_teste_antenas_concluido);
}

/*********************************************************************************/
/*   ---+++>>> void setResultadoTesteAntenas(unsigned int resultado) <<<+++---   */
/*********************************************************************************/
/* Funcao: void setResultadoTesteAntenas(unsigned int resultado)
 * Descricao: Reporta o resultado do teste das antenas.
 * @param: Configura 0 se algo errado ocorreu, ou 1 se tudo estiver OK.
 */
void setResultadoTesteAntenas(unsigned int resultado)
{
    unsigned int horaAtual = 0;
    unsigned int diaAtual = 0;
    unsigned char horaUltTeste = 0;
    unsigned char diaUltTeste = 0;
    unsigned char horaResult = 0;
    unsigned char diaResult = 0;
    
    horaAtual = verifHoraAtual();           // verificamos a hora atual
    diaAtual = verifDiaAtual();             // verificamos o dia atual
    
    if(resultado)                           // se nao deu problemas no autoteste das antenas
    {
        mem_escreve_24LC256(ADDR_DIA_ULTIMO_AUTO_TESTE, (unsigned char)diaAtual);   //salvamos na mem. externa
        mem_escreve_24LC256(ADDR_HORA_ULTIMO_AUTO_TESTE, (unsigned char)horaAtual); //salvamos na mem. externa
        auto_teste_antenas_concluido = resultado;
    }
    else
    {
        horaUltTeste = mem_le_24LC256(ADDR_HORA_ULTIMO_AUTO_TESTE);     // lemos o ultimo dia em que foi feito o teste
        diaUltTeste = mem_le_24LC256(ADDR_DIA_ULTIMO_AUTO_TESTE);       // lemos a ultima hora em que foi feito o teste
        diaResult = (unsigned char)diaAtual - diaUltTeste;              // vemos quantos dias se passaram desdee o ultimo teste
        if(((unsigned char)horaAtual >= horaUltTeste) && (!diaResult))  // se hora do Ult. teste for menor que hora atual e estivermos no mesmo dia ainda
            horaResult = (unsigned char)horaAtual - horaUltTeste;
        else    // senao significa que virou o dia
            horaResult = ((diaResult*24) + (unsigned char)horaAtual) - horaUltTeste;
        
        if(horaResult > 8)                      // verifica se passou de 8 horas desde o ultimo teste
            auto_teste_antenas_concluido = 0;   // salva o resultado reportando problemas
        else
            auto_teste_antenas_concluido = 1;   // salva o resultado reportando que esta tudo OK
    }
}

/************************************************************/
/*   ---+++>>> unsigned int verifDiaAtual(void) <<<+++---   */
/************************************************************/
/* Funcao: unsigned int verifDiaAtual(void)
 * Descricao: Retorna o dia atual.
 * @return: diaAtual - Retorna o dia atual
 */
unsigned int verifDiaAtual(void)
{
    unsigned char strDia[3];
    unsigned int diaAtual = 0;
    
    //testeI2C();                         // pega a data do autoteste
    atualizaHoraEData();
    strncpy((char*)strDia,stringData,2);// copiamos o dia
    strDia[2] = 0x00;                   // garantimos a finalizacao da string com NULL
    diaAtual = atoi((char*)strDia);     // convertemos para o tipo inteiro        
    return(diaAtual);
}

/*************************************************************/
/*   ---+++>>> unsigned int verifHoraAtual(void) <<<+++---   */
/*************************************************************/
/* Funcao: unsigned int verifHoraAtual(void)
 * Descricao: Retorna o dia atual.
 * @return: diaAtual - Retorna o dia atual
 */
unsigned int verifHoraAtual(void)
{
    unsigned char strHora[3];
    unsigned int horaAtual = 0;
    
    //testeI2C();                          // pega a data do autoteste
    atualizaHoraEData();
    strncpy((char*)strHora,stringHora,2);// copiamos a hora
    strHora[2] = 0x00;                   // garantimos a finalizacao da string com NULL
    horaAtual = atoi((char*)strHora);    // convertemos para o tipo inteiro        
    return(horaAtual);
}

/***********************************************************/
/*   ---+++>>> void verifAutoTesteLeitor(void) <<<+++---   */
/***********************************************************/
/* Funcao: void verifAutoTesteLeitor(void)
 * Descricao: Verifica se teve algum erro no autoteste.
 * @param: as3993_Error - indicador de erro na comunicacao com o CI AS3993
 * @param: antena_Error - indicador de erro com as antenas
 * @param: GPRS_Error - indicador de erro na comunicacao com o GPRS
 * @param: ZIGBEE_Error - indicador de erro na comunicacao com o ZIGBEE
 * @param: MEM_Error - indicador de erro na comunicacao com a mem. serial i2c
 * @param: GPS_Error - indicador de erro na comunicacao com o GPS
 */
void verifAutoTesteLeitor(unsigned char as3993_Error, unsigned char antena_Error,
                          unsigned char GPRS_Error,   unsigned char ZIGBEE_Error,
                          unsigned char MEM_Error,    unsigned char GPS_Error)
{
    unsigned char aciona_flag = 0;
    
    // Verificamos se houve algum erro em algum subsistema do leitor
    // Obs. Apenas as antenas NAO DEVEM ser verificadas aqui! Elas ja foram tratadas. 
    if(as3993_Error || GPRS_Error || ZIGBEE_Error || MEM_Error || GPS_Error)
    {
        setResultadoAutoTeste(0);       // sinaliza que o auto teste apresentou algum erro.
        limpa_display();
        // se sim, escreve no display o ocorrido.
        if(as3993_Error || antena_Error)
            poe_texto_XY(1,0,"Erro no sist. de RF!");
        
        if(GPRS_Error)
            poe_texto_XY(2,0,"Erro no sist. GPRS!");
        
        if(ZIGBEE_Error)
            poe_texto_XY(3,0,"Erro no sist. XBEE!");
        
        if(MEM_Error)
            poe_texto_XY(4,0,"Erro na mem. I2C!");
        
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
            ld_saidas(3,aciona_flag);   // comanda o acionamento do buzzer conforme a flag 'aciona_flag'
            
            if(as3993_Error)
                LIGA_PA(aciona_flag);
            
            if(antena_Error)
                sel_led(antena_atual,aciona_flag);
            
            if(GPRS_Error)
                LED_3G(aciona_flag);    // comanda o LED 3G do GPRS conforme a flag 'aciona_flag'
            
            if(ZIGBEE_Error)
                LED_ZIG(aciona_flag);   // comanda o LED do ZIGBEE conforme a flag 'aciona_flag'
            
            if(MEM_Error)
                sel_led(4,aciona_flag);// comanda o LED 4 para informar falha na memoria serial I2C
            
            if(GPS_Error)
                LED_GPS(aciona_flag);   // comanda o LED do GPS conforme a flag 'aciona_flag'
        }
    }
    else
    {
        setResultadoAutoTeste(1);       // sinaliza que o auto teste acabou com sucesso.
    }
}

/********************************************************************/
/*   ---+++>>> unsigned char autoTeste_mem24LC256(void) <<<+++---   */
/********************************************************************/
/* Funcao: unsigned char autoTeste_mem24LC256(void)
 * Descricao: Faz um auto-teste na memoria serial externa 24LC256.
 * @return: Retorna 1 se algo errado ocorreu, ou 0 se tudo estiver OK.
 */
unsigned char autoTeste_mem24LC256(void)
{
    unsigned char MEM_Error = 0;
    
#ifdef AUTO_TESTE_MEM_I2C
        mem_escreve_24LC256(ADDR_AUTO_TESTE_MEM, 0x55); //escrevemos um dado na mem. serial

        if(0x55 == mem_le_24LC256(ADDR_AUTO_TESTE_MEM)) // e agora lemos e verificamos se o dado foi recuperado
        {
            MEM_Error = 0;      // se o dado foi recuperado, sinaliza que a mem. serial i2c esta ok
        }
        else
        {
            MEM_Error = 1;      // caso contrario, sinaliza erro
        }
        return(MEM_Error);
#else
    MEM_Error = 0;              // apenas sinaliza que nao ha erros.
    return(MEM_Error);
#endif
}

/*--------------------------------------------------------------------*/
/*           ---+++>>> Fim do arquivo auto_teste.C <<<+++---          */
/*--------------------------------------------------------------------*/		
