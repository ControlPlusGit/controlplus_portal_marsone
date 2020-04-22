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
#include "gprs.h"   //LUCIANO: Inclui gprs.h para eliminar warnings.

#define POSICAO_TAGS_LEFT                       1
#define POSICAO_TAGS_IN_REPLY                   2
#define POSICAO_RSSI                            6
#define POSICAO_FREQUENCIA_HIGH                 9
#define POSICAO_FREQUENCIA_MID                  8
#define POSICAO_FREQUENCIA_LOW                  7
#define POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC     10
#define POSICAO_INICIAL_TAG                     11

#define MARGEM_PARA_LIDAR_COM_O_BUG_DE_ENVIO    3

//extern char StringRMCPura[TAMANHO_MAXIMO_STRING_RMC];

u8 status_conexao = 0;
int estadoEnvio;
int estadoComando;
int estadoAcao;
long timeoutEnvio;
int timeoutEnvioEmTentativa;
int timeoutComando;
int timeoutAcao;
int tentativasEnvio;
int tentativasComando;
int tentativasAcao;

int estadoEnvioWifi;
int estadoComandoWifi;
int estadoAcaoWifi;
int timeoutEnvioWifi;
int timeoutEnvioEmTentativaWifi;
int timeoutComandoWifi;
int timeoutAcaoWifi;
int tentativasEnvioWifi;
int tentativasComandoWifi;
int tentativasAcaoWifi;

int contadorDetagsEnviadasAoFtp;

int contadorParaUmSegundoLedGprs;

int contadorRtc;

char estadoLedsGprs; //variavel para deixar o led piscar ou acender dependendo do estado da maquina gprs
#define APAGADO         0
#define PISCAR_ACESSO   1
#define PISCAR_APAGADO  2
#define ACESSO          3


extern int conta_registros;

signed long ContadorDeTempoPassadoParaMaquinaDeComandosAT;

extern void pow_off_modem(void);
extern void  pow_on_modem(void);
extern void liga_led_3g(void);
extern void desliga_led_3g(void);
extern void pisca_led_3g(void);
extern void LimpaBufferRX3 (void);

#define TAMANHO_BUFFER_REPOSTA 120
char resposta[TAMANHO_BUFFER_REPOSTA];
extern int conta_envio;

#define TICKS_PARA_OCORRER_UM_INCREMENTE_DA_JANELA_DE_TEMPO 60000

#define TEMPO_PARA_CADA_ATIVACAO_DA_MAQUINA_DO_MODEM 300000 //milisegundos
signed long contadorParaDispararAMaquinaDeEstadosDoModem;

//extern Tag __attribute__((far)) tags_[MAXTAG];
extern Tag tags_[MAXTAG];


#define TAMANHO_MAXIMO_STRING_FTP 10//130
#ifndef VARAL
//#define TAMANHO_BUFFER_FTP 130
//#define TAMANHO_BUFFER_FTP 100
#define TAMANHO_BUFFER_FTP 10//Era 80 em 03/03/16
#else
#define TAMANHO_BUFFER_FTP 10
#endif
//char __attribute__((far)) bufferFTP[TAMANHO_BUFFER_FTP][TAMANHO_MAXIMO_STRING_FTP];
char bufferFTP[TAMANHO_BUFFER_FTP][TAMANHO_MAXIMO_STRING_FTP];
//char __attribute__((far)) bufferFTP[TAMANHO_MAXIMO_STRING_FTP][TAMANHO_BUFFER_FTP];
//char __attribute__((far)) bufferFTP[TAMANHO_BUFFER_FTP][TAMANHO_MAXIMO_STRING_FTP];

//__eds__ char bufferFTP[TAMANHO_MAXIMO_STRING_FTP][TAMANHO_BUFFER_FTP] __attribute__((eds,address(0x8000ul),noload));
//__eds__ char bufferFTP[TAMANHO_MAXIMO_STRING_FTP][TAMANHO_BUFFER_FTP] __attribute__((eds));
//__eds__ __attribute__((far)) char bufferFTP[TAMANHO_MAXIMO_STRING_FTP][TAMANHO_BUFFER_FTP];
//char __attribute__((far)) EnviadoFTP[TAMANHO_BUFFER_FTP];
char  EnviadoFTP[TAMANHO_BUFFER_FTP];

char  registroDeJanelaDeTempo[TAMANHO_BUFFER_FTP];
char contadorDeJanelaDeTempo;

unsigned int contadorParaIncrementeDoContadorDeJanelaDeTempo;

//char subBufferParaEnvioDeTagsNaSerial[TAMANHO_MAXIMO_STRING_FTP]; // o uso porque nao posso passar o espaco EDS como parametro ou usar com rotinas ANSI-C





#define TAMANHO_MAXIMO_STRING_AT 60
char  comandoAT[TAMANHO_MAXIMO_STRING_AT];
char  respostaAT[TAMANHO_MAXIMO_STRING_AT];

//char __attribute__((far)) respostaWifi[TAMANHO_MAXIMO_STRING_AT];
int IndiceDaMensagemWifi;
int BytesNaMensagemWifi;

extern char empilhadeira[20];

int todasAsTagsEnviadas;

int posicao_buffer_ftp;

volatile char CompararEmTempoReal;
volatile char ComparacaoValidaEmTempoReal;


#define VALOR_DE_TEMPO_PARA_PULSO_DE_LIGAR_O_MODEM 100
#define VALOR_DE_TEMPO_PARA_LIGAR_O_MODEM  10000
signed int ContadorParaPulsoDeLigarOModem = 0;
signed int ContadorParaOModemInicar = 0;
#define VALOR_DE_TEMPO_PARA_O_TRES_MAIS 2100
#define VALOR_DE_TEMPO_PARA_ENVIAR_O_COMANDO_DE_DESLIGAR_O_MODEM 1100
#define VALOR_DE_TEMPO_PARA_DEIXAR_AS_MAQUINAS_DE_ESTADOS_VOLTAREM_A_OPERAR 60000
signed int ContadorParaOsTresMais;
signed int ContadorParaMandarOComandoDeDesligarOModem;
signed long ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar;

u16 estados_maquina_envio,estados_maquina_comandos,estados_maquina_acao_gps = 0;

#define TAMANHO_MAXIMO_DO_TEXTO_DE_INFORAMACAO_DAS_TORRES 2000
char ListaDasTorresProximas[TAMANHO_MAXIMO_DO_TEXTO_DE_INFORAMACAO_DAS_TORRES];

const enum ESTADOS_MAQUINA_ENVIO {
    ESTADO_ENVIAR_MAQUINA_AT,
    ESTADO_RECEBER_RESPOSTA_MAQUINA_AT,
    ESTADO_RESPOSTA_RECEBIDA_MAQUINA_AT,
    ESTADO_ESPERAR_COMANDO_NAO_REQUISITADO_MAQUINA_AT,
    ESTADO_OCIOSO_MAQUINA_AT
} MAQUINA_ENVIO;


const enum ESTADOS_MAQUINA_COMANDOS {
    ESTADO_LIGAR_MODEM_MAQUINA_COMANDOS,
    ESTADO_DESLIGAR_MODEM_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_AT_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_AT_CTZU_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_AT_CCLK_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATZ_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATH_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_ATE1_MAQUINA_COMANDOS,

    ESTADO_ENVIAR_CGATT_MAQUINA_COMANDOS,
    ESTADO_ENVIAR_CGED_MAQUINA_COMANDOS,
    ESTADO_COLETA_DE_TORRES_DO_CGED_MAQUINA_COMANDOS,
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
    ESTADO_ESPERAR_UUFTPCR7__MAQUINA_COMANDOS,

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


const enum ESTADOS_MAQUINA_ACAO_GPRS {
    ESTADO_LIGAR_MODEM_ACAO_GPRS,
    ESTADO_ESPERANDO_LIGAR_MODEM_ACAO_GPRS,
    ESTADO_DESLIGAR_MODEM_ACAO_GPRS,
    ESTADO_ESPERANDO_DESLIGAR_MODEM_ACAO_GPRS,
    ESTADO_AT_ACAO_GPRS,
    ESTADO_ESPERANDO_AT_ACAO_GPRS,
    ESTADO_AT_CTZU_ACAO_GPRS,
    ESTADO_ESPERANDO_AT_CTZU_ACAO_GPRS,
    ESTADO_AT_CCLK_ACAO_GPRS,
    ESTADO_ESPERANDO_AT_CCLK_ACAO_GPRS,
    ESTADO_ATZ_ACAO_GPRS,
    ESTADO_ESPERANDO_ATZ_ACAO_GPRS,
    ESTADO_ATH_ACAO_GPRS,
    ESTADO_ESPERANDO_ATH_ACAO_GPRS,
    ESTADO_ATE1_ACAO_GPRS,
    ESTADO_ESPERANDO_ATE1_ACAO_GPRS,
    ESTADO_AT_CIMI_ACAO_GPRS,
    ESTADO_ESPERANDO_AT_CIMI_ACAO_GPRS,
    ESTADO_AT_CSQ_ACAO_GPRS,
    ESTADO_ESPERANDO_AT_CSQ_ACAO_GPRS,
    ESTADO_CGATT_ACAO_GPRS,
    ESTADO_ESPERANDO_CGATT_ACAO_GPRS,
    ESTADO_CGED_ACAO_GPRS,
    ESTADO_ESPERANDO_CGED_ACAO_GPRS,
    ESTADO_CPOS_ACAO_GPRS,
    ESTADO_ESPERANDO_CPOS_ACAO_GPRS,
    ESTADO_CGDCONT_ACAO_GPRS,
    ESTADO_ESPERANDO_CGDCONT_ACAO_GPRS,
    ESTADO_UPSD1_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSD1_ACAO_GPRS,
    ESTADO_UPSD2_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSD2_ACAO_GPRS,
    ESTADO_UPSD3_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSD3_ACAO_GPRS,
    ESTADO_UPSD4_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSD4_ACAO_GPRS,
    ESTADO_UPSD7_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSD7_ACAO_GPRS,
    ESTADO_UPSDA3_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSDA3_ACAO_GPRS,
    ESTADO_UPSND8_ACAO_GPRS,
    ESTADO_ESPERANDO_UPSND8_ACAO_GPRS,
    ESTADO_UFTP1_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTP1_ACAO_GPRS,
    ESTADO_UFTP2_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTP2_ACAO_GPRS,
    ESTADO_UFTP3_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTP3_ACAO_GPRS,
    ESTADO_UFTP6_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTP6_ACAO_GPRS,
    ESTADO_UFTPC1_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTPC1_ACAO_GPRS,
    ESTADO_UUFTPC_ACAO_GPRS,
    ESTADO_ESPERANDO_UUFTPC_ACAO_GPRS,
    ESTADO_UFTPC7_ACAO_GPRS,
    ESTADO_ESPERANDO_UFTPC7_ACAO_GPRS,
    ESTADO_FTP_PRONTO_ACOES_GPRS,

    ESTADO_PREPARAR_PARA_ENVIAR_3MAIS_ACOES_GPRS,
    ESTADO_ESPEAR_PARA_ENVIAR_3MAIS_ACOES_GPRS,
    PREPARAR_PARA_ENVIAR_UUFTPCR7_ACOES_GPRS,
    ESTADO_ESPERAR_UUFTPCR7_ACOES_GPRS,
            
    ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS,
    ESTADO_ESPERANDO_FTP_FECHA_ARQUIVO_ACOES_GPRS,
    ESTADO_FTP_DESCONEXAO_ACOES_GPRS,
    ESTADO_ESPERANDO_FTP_DESCONEXAO_ACAO_GPRS,
    ESTADO_DESCONEXAO_DA_REDE_ACOES_GPRS,
    ESTADO_ESPERANDO_DESCONEXAO_DA_REDE_ACAO_GPRS,

    ESTADO_CONFERIR_MODEM_MAQUINA_ACAO_GPRS,
    ESTADO_CONFERIR_MODEM_ESPERANDO_RESPOSTA_MAQUINA_ACAO_GPRS,
    ESTADO_CONFERIR_CHIP_MAQUINA_ACAO_GPRS,
    ESTADO_DESBLOQUEAR_CHIP_MAQUINA_ACAO_GPRS,
    ESTADO_CONFERIR_CSQ_MAQUINA_ACAO_GPRS,
    ESTADO_CONFERIR_SE_ATACHOU_NA_REDE_MAQUINA_ACAO_GPRS,
    ESTADO_CONECTAR_TCP_MAQUINA_ACAO_GPRS,
    ESTADO_CONECTAR_FTP_MAQUINA_ACAO_GPRS,
    ESTADO_SUBIR_DADOS_FTP_MAQUINA_ACAO_GPRS,
    ESTADO_FECHAR_FTP_MAQUINA_ACAO_GPRS,
    ESTADO_ESPERA_PARA_NOVA_SUBIDA_NO_FTP_MAQUINA_ACAO_GPRS,
    ESTADO_OCIOSA_MAQUINA_ACAO_GPRS,
    ESTADO_ERRO_MAQUINA_ACAO_GPRS
} MAQUINA_ACAO_GPRS;


int conectaOModem (void);
int loggarNoFTP (void);
int fecharOArquivo (void);
int MaquinaDeEstadosAT (int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado);
int MaquinaDeEstadosDeComandosGPRS (int estado);
void resetNasVariaisDeModemEmTempoReal (void);
void cabecaVolit (void);
void EnviaTags (void);
void AnulaOEnvioDaTag (void);
void LimpaTagEnviada(void);


//Rotinas
void EnviaByteParaSerial (char dado){
    U3TXREG = dado;
    
    while( (!U3STAbits.TRMT ) );
}


void EnviaStringParaSerial (char * dado){
    while (*dado != 0){
        EnviaByteParaSerial(*dado);
        dado = dado + 1;
    }
}

#ifdef DEBUG_DE_COMANDOS_AT
void EnviaByteDebugParaSerial (char dado){
    U4TXREG = dado;
    while( (!U4STAbits.TRMT ) );
}


void EnviaStringDebugParaSerial (char * dado){
    while (*dado != 0){
        EnviaByteDebugParaSerial(*dado);
        dado = dado + 1;
    }
}
#endif


/*
int QuantiaDeTagsPresentesNoBufferDeFTP (void){
    int contador;
    int soma;

    soma = 0;
    for (contador = 0;contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        if (bufferFTP[contador][0] != 0){
            soma = soma + 1;
        }
    }
    return soma;
}
 * */

int posicaoOpcuapada(void){
    int contador;

    for(contador = 0;contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        if (bufferFTP[contador][0] != 0)
            return contador;
    }
    return -1;
}

int posicaoOpcuapadaENaoEnviada(void){
    int contador;

    for(contador = 0;contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        if ( (bufferFTP[contador][0] != 0) && (EnviadoFTP[contador] == 0) )
            return contador;
    }
    return -1;
}


// strstr esta falhando, logo vou fazer a minha
int ProcuraSubStringEmString (char *string, char *substring){
    int contadorDeOcorrencias;

    while (*string != 0){        
        if (*string == *substring){
            contadorDeOcorrencias = contadorDeOcorrencias + 1;
            substring = substring  + 1;

            if (*substring == 0)
                return 1;

        } else {
            if (contadorDeOcorrencias != 0)
                return -1;
        }

        string = string + 1;
        
    }
    return -1;
}

int MaquinaDeAcoesGPRS (int estado){
    switch (estado){
        //LIGAR MODODEM
        case (ESTADO_LIGAR_MODEM_ACAO_GPRS):
            RegistraTentativaGPRS();
            estadoComando = ESTADO_LIGAR_MODEM_MAQUINA_COMANDOS;
            return ESTADO_ESPERANDO_LIGAR_MODEM_ACAO_GPRS;
            break;

        case (ESTADO_ESPERANDO_LIGAR_MODEM_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if ((estadoComando == ESTADO_OCIOSO_MAQUINA_COMANDOS) || (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS) || (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) ){
                return ESTADO_AT_ACAO_GPRS;
            }
            break;

        // AT
        case (ESTADO_AT_ACAO_GPRS):
            
            //ESTADO_ENVIAR_AT_MAQUINA_COMANDOS
            estadoComando = ESTADO_ENVIAR_AT_MAQUINA_COMANDOS;
            estados_maquina_comandos = 1;
            estados_maquina_acao_gps = 2;
            return ESTADO_ESPERANDO_AT_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_AT_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                //return ESTADO_AT_CTZU_ACAO_GPRS;
                estados_maquina_acao_gps = 6;
                return ESTADO_ATZ_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 53;
                status_conexao = 129; // GILSON, MODEM NAO RESPONDE OU NAO ESTA PRESENTE
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_AT_CTZU_ACAO_GPRS):
            //ESTADO_ENVIAR_AT_MAQUINA_COMANDOS
            status_conexao = 1; // GILSON, MODEM PRESENTE
            estadoLedsGprs  =PISCAR_ACESSO;
            estadoComando = ESTADO_ENVIAR_AT_CTZU_MAQUINA_COMANDOS;
            estados_maquina_comandos = 2;
            estados_maquina_acao_gps = 3;
            return ESTADO_ESPERANDO_AT_CTZU_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_AT_CTZU_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 80;
                return ESTADO_ATZ_ACAO_GPRS;
            }
                //return ESTADO_ATZ_ACAO_GPRS;
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 54;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

            //ATZ
        case (ESTADO_ATZ_ACAO_GPRS):
            status_conexao = 1; // GILSON, MODEM PRESENTE, DUPLICADO DE PROPOSITO PORQUE ATZ E ATCZU PODEM SER USADOS
            estadoComando = ESTADO_ENVIAR_ATZ_MAQUINA_COMANDOS;
            estados_maquina_comandos = 4;
            estados_maquina_acao_gps = 7;
            estadoLedsGprs  =PISCAR_ACESSO;
            return ESTADO_ESPERANDO_ATZ_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_ATZ_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 8;
                return ESTADO_ATH_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 55;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

            //ATH
        case (ESTADO_ATH_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_ATH_MAQUINA_COMANDOS;
            estados_maquina_comandos = 5;
            estados_maquina_acao_gps = 9;
            estadoLedsGprs  =PISCAR_ACESSO;
            return ESTADO_ESPERANDO_ATH_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_ATH_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 10;
                return ESTADO_ATE1_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 56;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

            //ATE1
        case (ESTADO_ATE1_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_ATE1_MAQUINA_COMANDOS;
            estados_maquina_comandos = 6;
            estados_maquina_acao_gps = 11;
            return ESTADO_ESPERANDO_ATE1_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_ATE1_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 12;
                return ESTADO_CGATT_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 57;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_CGATT_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_CGATT_MAQUINA_COMANDOS;
            estados_maquina_comandos = 7;
            estados_maquina_acao_gps = 13;
            return ESTADO_ESPERANDO_CGATT_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_CGATT_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                //return ESTADO_CPOS_ACAO_GPRS;
                estados_maquina_acao_gps = 15;
                //return ESTADO_CGDCONT_ACAO_GPRS;
                return ESTADO_CGED_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 58;
                status_conexao = 130; // GILSON, NAO FOI POSSIVEL CONECTAR NA REDE
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;
            
        case (ESTADO_CGED_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_CGED_MAQUINA_COMANDOS;
            estados_maquina_comandos = 7;
            estados_maquina_acao_gps = 13;
            return ESTADO_ESPERANDO_CGED_ACAO_GPRS;

            break;
            
        case (ESTADO_ESPERANDO_CGED_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                //return ESTADO_CPOS_ACAO_GPRS;
                estados_maquina_acao_gps = 15;
                return ESTADO_CGDCONT_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 58;
                status_conexao = 130; // GILSON, NAO FOI POSSIVEL CONECTAR NA REDE
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_CPOS_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_CPOS_MAQUINA_COMANDOS;
            estados_maquina_comandos = 8;
            estados_maquina_acao_gps = 14;
            return ESTADO_ESPERANDO_CPOS_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_CPOS_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 61;
                return ESTADO_CGDCONT_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 59;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;


        case (ESTADO_CGDCONT_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_CGDCONT_MAQUINA_COMANDOS;
            estados_maquina_comandos = 9;
            estados_maquina_acao_gps = 16;
            return ESTADO_ESPERANDO_CGDCONT_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_CGDCONT_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 17;
                return ESTADO_UPSD1_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 79;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UPSD1_ACAO_GPRS):
            status_conexao = 2; // GILSON, CONECTOU NA REDE
            estadoComando = ESTADO_ENVIAR_UPSD1_MAQUINA_COMANDOS;
            estados_maquina_comandos = 10;
            estados_maquina_acao_gps = 17;
            return ESTADO_ESPERANDO_UPSD1_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSD1_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 4;
                return ESTADO_AT_CCLK_ACAO_GPRS;
            }
                //return ESTADO_UPSD2_ACAO_GPRS;
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 61;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            return estado;
            break;

        //AT+CCLK?
        case (ESTADO_AT_CCLK_ACAO_GPRS):
            //ESTADO_ENVIAR_AT_MAQUINA_COMANDOS
            estadoComando = ESTADO_ENVIAR_AT_CCLK_MAQUINA_COMANDOS;
            estados_maquina_comandos = 3;
            estados_maquina_acao_gps = 5;
            return ESTADO_ESPERANDO_AT_CCLK_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_AT_CCLK_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 18;
                return ESTADO_UPSD2_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 62;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;



        case (ESTADO_UPSD2_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSD2_MAQUINA_COMANDOS;
            estados_maquina_comandos = 11;
            estados_maquina_acao_gps = 19;
            return ESTADO_ESPERANDO_UPSD2_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSD2_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 20;
                return ESTADO_UPSD3_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 63;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UPSD3_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSD3_MAQUINA_COMANDOS;
            estados_maquina_comandos = 12;
            estados_maquina_acao_gps = 21;
            return ESTADO_ESPERANDO_UPSD3_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSD3_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 22;
                return ESTADO_UPSD4_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 64;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            return estado;
            break;

        case (ESTADO_UPSD4_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSD4_MAQUINA_COMANDOS;
            estados_maquina_comandos = 13;
            estados_maquina_acao_gps = 23;
            return ESTADO_ESPERANDO_UPSD4_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSD4_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 24;
                return ESTADO_UPSD7_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 65;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            return estado;
            break;

        case (ESTADO_UPSD7_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSD7_MAQUINA_COMANDOS;
            estados_maquina_comandos = 14;
            estados_maquina_acao_gps = 25;
            return ESTADO_ESPERANDO_UPSD7_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSD7_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 26;
                return ESTADO_UPSDA3_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 66;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UPSDA3_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSDA3_MAQUINA_COMANDOS;
            estados_maquina_comandos = 15;
            estados_maquina_acao_gps = 27;
            return ESTADO_ESPERANDO_UPSDA3_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSDA3_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 28;
                return ESTADO_UPSND8_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 67;
                status_conexao = 131; // GILSON, NAO FOI POSSIVEL CONECTAR NA INTERNET
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UPSND8_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UPSND8_MAQUINA_COMANDOS;
            estados_maquina_comandos = 16;
            estados_maquina_acao_gps = 29;
            return ESTADO_ESPERANDO_UPSND8_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UPSND8_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 30;
                return ESTADO_UFTP1_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 68;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UFTP1_ACAO_GPRS):
            status_conexao = 3; // GILSON, CONECTOU NA INTERNET
            estadoComando = ESTADO_ENVIAR_UFTP1_MAQUINA_COMANDOS;
            estados_maquina_comandos = 17;
            estados_maquina_acao_gps = 31;
            return ESTADO_ESPERANDO_UFTP1_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTP1_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 32;
                return ESTADO_UFTP2_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 69;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UFTP2_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UFTP2_MAQUINA_COMANDOS;
            estados_maquina_comandos = 18;
            estados_maquina_acao_gps = 32;
            return ESTADO_ESPERANDO_UFTP2_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTP2_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 33;
                return ESTADO_UFTP3_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 70;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UFTP3_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UFTP3_MAQUINA_COMANDOS;
            estados_maquina_comandos = 19;
            estados_maquina_acao_gps = 34;
            return ESTADO_ESPERANDO_UFTP3_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTP3_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 35;
                return ESTADO_UFTP6_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 71;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UFTP6_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UFTP6_MAQUINA_COMANDOS;
            estados_maquina_comandos = 20;
            estados_maquina_acao_gps = 36;
            return ESTADO_ESPERANDO_UFTP6_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTP6_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 37;
                return ESTADO_UFTPC1_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 72;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UFTPC1_ACAO_GPRS):
            status_conexao = 4; // GILSON, CONECTOU NO FTP
            estadoComando = ESTADO_ENVIAR_UFTPC1_MAQUINA_COMANDOS;
            estados_maquina_comandos = 21;
            estados_maquina_acao_gps = 38;
            return ESTADO_ESPERANDO_UFTPC1_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTPC1_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 39;
                //return ESTADO_UUFTPC_ACAO_GPRS;
                return ESTADO_UFTPC7_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 73;
                status_conexao = 132; // GILSON, NAO FOI POSSIVEL CONECTAR NO FTP, COMANDO NAO ACEITO
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case (ESTADO_UUFTPC_ACAO_GPRS):
            estadoComando = ESTADO_ESPERAR_UUFTPCR_MAQUINA_COMANDOS;
            estados_maquina_comandos = 22;
            estados_maquina_acao_gps = 40;
            return ESTADO_ESPERANDO_UUFTPC_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UUFTPC_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 41;
                return ESTADO_UFTPC7_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 74;
                status_conexao = 133; // GILSON, NAO FOI POSSIVEL CONECTAR NO FTP, REJEITADO PELO FTP
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;


        case (ESTADO_UFTPC7_ACAO_GPRS):
            estadoComando = ESTADO_ENVIAR_UFTPC7_MAQUINA_COMANDOS;
            estados_maquina_comandos = 23;
            estados_maquina_acao_gps = 42;
            _TRISA9 = 0;
            _LATA9 = 1;

            return ESTADO_ESPERANDO_UFTPC7_ACAO_GPRS;
            break;
        case (ESTADO_ESPERANDO_UFTPC7_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 43;
                _LATA9 = 0;
                FtpPrepararParaEnviarTags();
                return ESTADO_FTP_PRONTO_ACOES_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS)
            { // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
                //delay_ms(2100);
                //EnviaByteParaSerial("+++");

                delay_ms(1000);//apenasp para testes
                asm("CLRWDT");
                EnviaStringParaSerial("AT\r\n");
                delay_ms(1000);//apenasp para testes
                asm("CLRWDT");
                
                
            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
            EnviaStringParaSerial("AT+UFTPER\r\n");
            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
                
                status_conexao = 134; // GILSON, NAO FOI POSSIVEL ABRIR UM ARQUIVO NO FTP
                estados_maquina_acao_gps = 75;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            return estado;
            break;




        case ESTADO_FTP_PRONTO_ACOES_GPRS:
            status_conexao = 5; // GILSON, SUBINDO O ARQUIVO
            //liga_led_3g();
            estadoLedsGprs  =ACESSO;
            return estado;//Soh sai desse estado apos acao externa
            break;



        case ESTADO_PREPARAR_PARA_ENVIAR_3MAIS_ACOES_GPRS:
            //delay_ms(2100);
            //return ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS;

            estadoComando = ESTADO_ESPERAR_PARA_ENVIAR_3MAIS_MAQUINAS_COMANDOS;
            return ESTADO_ESPEAR_PARA_ENVIAR_3MAIS_ACOES_GPRS;
            break;

        case ESTADO_ESPEAR_PARA_ENVIAR_3MAIS_ACOES_GPRS:
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS){
                return ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS;
                //return PREPARAR_PARA_ENVIAR_UUFTPCR7_ACOES_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS){
                return ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS;
            }
            return ESTADO_ESPEAR_PARA_ENVIAR_3MAIS_ACOES_GPRS;
            break;

        case (ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS):
            estadoComando = ESTADO_ENVIAR_3MAIS_MAQUINA_COMANDOS;
            estados_maquina_comandos = 24;
            estados_maquina_acao_gps = 44;
            return ESTADO_ESPERANDO_FTP_FECHA_ARQUIVO_ACOES_GPRS;
            break;
        case (ESTADO_ESPERANDO_FTP_FECHA_ARQUIVO_ACOES_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 45;
                //RegistraSucessoGPRS();
                //LimpaTagEnviada();

                //return ESTADO_FTP_DESCONEXAO_ACOES_GPRS;
                return PREPARAR_PARA_ENVIAR_UUFTPCR7_ACOES_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 76;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return estado;
            break;

        case PREPARAR_PARA_ENVIAR_UUFTPCR7_ACOES_GPRS:
            estadoComando = ESTADO_ESPERAR_UUFTPCR7__MAQUINA_COMANDOS;
            return ESTADO_ESPERAR_UUFTPCR7_ACOES_GPRS;
            break;

        case ESTADO_ESPERAR_UUFTPCR7_ACOES_GPRS:
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);

            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS){
                RegistraSucessoGPRS();
                LimpaTagEnviada();
                ReiniciaLogicaDeJanela();
                
                return ESTADO_FTP_DESCONEXAO_ACOES_GPRS;

            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS){
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            break;



        case ESTADO_FTP_DESCONEXAO_ACOES_GPRS:
            estadoComando = ESTADO_DESLOGA_DO_FTP_MAQUINA_COMANDOS;
            estados_maquina_comandos = 25;
            estados_maquina_acao_gps = 46;
            return ESTADO_ESPERANDO_FTP_DESCONEXAO_ACAO_GPRS;
            break;

        case ESTADO_ESPERANDO_FTP_DESCONEXAO_ACAO_GPRS:
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 47;
                return ESTADO_DESCONEXAO_DA_REDE_ACOES_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 77;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }

            return estado;
            break;

        case ESTADO_DESCONEXAO_DA_REDE_ACOES_GPRS:
            estadoComando = ESTADO_DESCONECTA_DA_REDE_MAQUINA_COMANDOS;
            estados_maquina_comandos = 26;
            estados_maquina_acao_gps = 48;
            return ESTADO_ESPERANDO_DESCONEXAO_DA_REDE_ACAO_GPRS;
            //return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            break;

        case ESTADO_ESPERANDO_DESCONEXAO_DA_REDE_ACAO_GPRS:
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 51;
                //U3MODE = U3MODE & 0x7FFF;

                //return ESTADO_OCIOSA_MAQUINA_ACAO_GPRS;
                return ESTADO_DESLIGAR_MODEM_ACAO_GPRS;
            }
            if (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS) // REPLICAR ESSA LINHA SE A LOGICA ESCOLHIDA FOR DE RETORNAR EM CASO DE QUALQUER ERRO.
            {
                estados_maquina_acao_gps = 78;
                return ESTADO_ERRO_MAQUINA_ACAO_GPRS;
            }
            return ESTADO_ESPERANDO_DESCONEXAO_DA_REDE_ACAO_GPRS;
            break;

        case (ESTADO_DESLIGAR_MODEM_ACAO_GPRS):
            estadoComando = ESTADO_DESLIGAR_MODEM_MAQUINA_COMANDOS;
            return ESTADO_ESPERANDO_DESLIGAR_MODEM_ACAO_GPRS;
            break;

        case (ESTADO_ESPERANDO_DESLIGAR_MODEM_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);

            if ((estadoComando == ESTADO_OCIOSO_MAQUINA_COMANDOS) || (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS) || (estadoComando == ESTADO_ERRO_MAQUINA_COMANDOS)){
                return ESTADO_OCIOSA_MAQUINA_ACAO_GPRS;
            }
            break;


        case ESTADO_OCIOSA_MAQUINA_ACAO_GPRS:
            //Comentada a linha abaixo que do contrario a variavel statis_conexao nao sera visivel.
            //status_conexao = 0; // GILSON, ENCERROU O PROCESSO
            _TRISC12 = 0;
            _LATC12 = 1;
            return estado;
            break;



            //AT+CIMI

            //AT+CSQ

            //AT+CGATT

            //



        case (ESTADO_CONFERIR_CHIP_MAQUINA_ACAO_GPRS):
            estadoComando = MaquinaDeEstadosDeComandosGPRS(estadoComando);
            if (estadoComando == ESTADO_SUCESSO_MAQUINA_COMANDOS)
            {
                estados_maquina_acao_gps = 50;
                return ESTADO_DESBLOQUEAR_CHIP_MAQUINA_ACAO_GPRS;
            }
            break;
        case (ESTADO_DESBLOQUEAR_CHIP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_CONFERIR_CSQ_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_CONFERIR_SE_ATACHOU_NA_REDE_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_CONECTAR_TCP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_CONECTAR_FTP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_SUBIR_DADOS_FTP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_FECHAR_FTP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_ESPERA_PARA_NOVA_SUBIDA_NO_FTP_MAQUINA_ACAO_GPRS):
            break;
        case (ESTADO_ERRO_MAQUINA_ACAO_GPRS):
            status_conexao = 135; // GILSON, ERRO NAO LISTADO, POREM PAROU O PROCESSAMENTO
            //Na duvida desliga o MODEM
            //DesligaMaquinaGPRS();//Adicionado em 12/08/15                      
            //return ESTADO_DESLIGAR_MODEM_ACAO_GPRS;
            
            delay_ms(2000);
            EnviaStringParaSerial("+++");
            delay_ms(2000);
            EnviaStringParaSerial("AT+CPWROFF\r\n");
            delay_ms(2000);
            
#ifdef MODEM_V5
            _TRISF0 = 0;
            _LATF0 = 1;
#else 
            _TRISG12 = 0;
            //_LATG12 = 1;
            _LATG12 = 0;
#endif

            return ESTADO_OCIOSA_MAQUINA_ACAO_GPRS;

            break;


        default:
            estados_maquina_acao_gps = 49;
            return ESTADO_CONFERIR_CHIP_MAQUINA_ACAO_GPRS;
            break;
    }
    return estado;
}

/*
int ;
int estadoComando;
int estadoAcao;
int ;
int timeoutComando;
int timeoutAcao;
int ;
int tentativasComando;
int tentativasAcao;
*/

int TesteAutomaticoDoModem (void){
    int tamanho;
    //char respostap[20];   //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.
    EnviaStringParaSerial("AT\r\n");
    delay_ms(1000);
    tamanho= uartRx3NumBytesAvailable();
    uartRx3NBytes((u8*)resposta, (u16*)&tamanho);   //LUCIANO: Apliquei casts para eliminar warnings.
    
    if (strstr(resposta, "OK") != NULL)
        return 0;
    return -1;
}

int MaquinaDeEstadosDeComandosGPRS (int estado){

    char rascunho[40];
    char rascunho_data[30];
    char rascunho_dia[10];
    int resultado;
    switch(estado){

        case (ESTADO_LIGAR_MODEM_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

#ifdef MODEM_V5
            _TRISF0 = 0;
            _LATF0 = 0;
#else 
            _TRISG12 = 0;
            //_LATG12 = 0;
            _LATG12 = 1;
#endif
            timeoutEnvio = 2000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 2;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 1;
            //sprintf(comandoAT, "");
            memset(comandoAT,0,TAMANHO_MAXIMO_STRING_AT);
            //sprintf(respostaAT, "");
            memset(respostaAT,0,TAMANHO_MAXIMO_STRING_AT);
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;


        case (ESTADO_DESLIGAR_MODEM_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

#ifdef MODEM_V5
            _TRISF0 = 0;
            _LATF0 = 1;
#else 
            _TRISG12 = 0;
            //_LATG12 = 1;
            _LATG12 = 0;
#endif
            timeoutEnvio = 1000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 2;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 1;
            sprintf(comandoAT, "r\n");
            //sprintf(respostaAT, "");
            memset(respostaAT,0,TAMANHO_MAXIMO_STRING_AT);
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;

        case (ESTADO_ENVIAR_AT_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);


            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 1;
            sprintf(comandoAT, "AT\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 60;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;

        case (ESTADO_ENVIAR_AT_CTZU_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 2;
            //sprintf(comandoAT, "AT+CTZU=1\r\n");
            sprintf(comandoAT, "AT+CPIN=8486\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 28;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;


        case (ESTADO_ENVIAR_AT_CCLK_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 3;
            sprintf(comandoAT, "AT+CCLK?\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 29;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;

        case (ESTADO_ENVIAR_ATZ_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 4;
            sprintf(comandoAT, "ATZ\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 30;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_ATH_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 5;
            sprintf(comandoAT, "ATH\r\n");            
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 31;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_ATE1_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 6;
            sprintf(comandoAT, "ATE1\r\n");
            
            
#ifdef CLARO
            EnviaStringParaSerial("ATE1\r\n");
            delay_ms(1000);//apenasp para testes
            EnviaStringParaSerial("AT+CIMI\r\n");
            delay_ms(1000);//apenasp para testes
            
            //timeoutEnvio = 400;
            timeoutEnvio = 2000;
            //timeoutEnvioEmTentativa = 400;
            timeoutEnvioEmTentativa = 2000;
            

            //sprintf(comandoAT, "AT+CPIN=|3636|\r\n");
#endif
            
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 32;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_CGATT_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 10000;
            timeoutEnvioEmTentativa = 10000;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            
            //adicionado para testes
            //CompararEmTempoReal = 1;
            
            estados_maquina_envio = 7;
            sprintf(comandoAT, "AT+CGATT?\r\n");
            sprintf(respostaAT, "+CGATT: 1");
            estados_maquina_comandos = 33;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case ESTADO_ENVIAR_CGED_MAQUINA_COMANDOS:
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 180000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 8;
            sprintf(comandoAT, "AT+CGED=0\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 34;
            return ESTADO_COLETA_DE_TORRES_DO_CGED_MAQUINA_COMANDOS;

            break;
            
        case (ESTADO_COLETA_DE_TORRES_DO_CGED_MAQUINA_COMANDOS):
            //ListaDasTorresProximas[TAMANHO_BUFFER_DA_LISTAGEM_DAS_TORRES_PROXIMAS];            
            //char*comando, char *resultado, int timeout
            if (TratarCGED(comandoAT,ListaDasTorresProximas,500))return ESTADO_SUCESSO_MAQUINA_COMANDOS;
            return ESTADO_ERRO_MAQUINA_COMANDOS;                          
            break;

        case ESTADO_ENVIAR_CPOS_MAQUINA_COMANDOS:
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 180000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 8;
            sprintf(comandoAT, "AT+CPOS=0\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 34;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;


        case (ESTADO_ENVIAR_CGDCONT_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 9;
#ifdef VIVO
            sprintf(comandoAT, "AT+CGDCONT=1,|IP|,|zap.vivo.com.br|\r\n");
#endif
            
#ifdef TIM
            sprintf(comandoAT, "AT+CGDCONT=1,|IP|,|tim.br|\r\n");
#endif
            
#ifdef CLARO
            //sprintf(comandoAT, "AT+CPIN=|3636|\r\n");
            sprintf(comandoAT, "AT+CGDCONT=1,|IP|,|claro.com.br|\r\n");
#endif
            
#ifdef OI
            sprintf(comandoAT, "AT+CGDCONT=1,|IP|,|gprs.oi.com.br|\r\n");
#endif

            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 35;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;


        case (ESTADO_ENVIAR_UPSD1_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 10;
#ifdef VIVO
            sprintf(comandoAT, "AT+UPSD=0,1,|vivo|\r\n");
#endif
            
#ifdef TIM
            sprintf(comandoAT, "AT+UPSD=0,1,|tim|\r\n");
#endif
            
#ifdef CLARO
            sprintf(comandoAT, "AT+UPSD=0,1,|claro.com.br|\r\n");
#endif
            
#ifdef OI
            sprintf(comandoAT, "AT+UPSD=0,1,|oi|\r\n");
#endif
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 36;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UPSD2_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 11;
#ifdef VIVO
            sprintf(comandoAT, "AT+UPSD=0,2,|vivo|\r\n");
#endif
            
#ifdef TIM            
            sprintf(comandoAT, "AT+UPSD=0,2,|tim|\r\n");
#endif
            
#ifdef CLARO
            sprintf(comandoAT, "AT+UPSD=0,2,|claro|\r\n");
#endif
            
#ifdef OI
            sprintf(comandoAT, "AT+UPSD=0,2,|oi|\r\n");
#endif
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 37;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;


        case (ESTADO_ENVIAR_UPSD3_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 12;
#ifdef VIVO
            sprintf(comandoAT, "AT+UPSD=0,3,|vivo|\r\n");
#endif
            
#ifdef TIM
            sprintf(comandoAT, "AT+UPSD=0,3,|tim|\r\n");
#endif
            
#ifdef CLARO
            sprintf(comandoAT, "AT+UPSD=0,3,|claro|\r\n");
#endif
            
#ifdef OI
            sprintf(comandoAT, "AT+UPSD=0,3,|oi|\r\n");
#endif
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 38;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UPSD4_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 13;
            sprintf(comandoAT, "AT+UPSD=0,4,|8.8.8.8|\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 39;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UPSD7_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 100;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 14;
            sprintf(comandoAT, "AT+UPSD=0,7,|0.0.0.0|\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 40;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UPSDA3_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            //timeoutEnvio = 3000;
            //timeoutEnvioEmTentativa = 3000;
            //timeoutEnvio = 5000;
            //timeoutEnvioEmTentativa = 10000;
            timeoutEnvio = 180000;
            timeoutEnvioEmTentativa = timeoutEnvio;

            //tentativasEnvio = 10;
            tentativasEnvio = 0;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 15;
            sprintf(comandoAT, "AT+UPSDA=0,3\r\n");
            sprintf(respostaAT, "OK");

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            estados_maquina_comandos = 41;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UPSND8_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 3000;
            timeoutEnvioEmTentativa = 3000;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 16;
            sprintf(comandoAT, "AT+UPSND=0,8\r\n");
            sprintf(respostaAT, "+UPSND: 0,8,1");
            estados_maquina_comandos = 42;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;


        case (ESTADO_ENVIAR_UFTP1_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 300;
            timeoutEnvioEmTentativa = 300;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 17;

#ifdef VOLIT
            sprintf(comandoAT, "AT+UFTP=1,|ftp.volitsistemas.com.br|\r\n");
            //sprintf(comandoAT, "AT+UFTP=0,|189.38.81.79|\r\n");
#else
            //sprintf(comandoAT, "AT+UFTP=1,|ftp.control-up.com|\r\n");
            sprintf(comandoAT, "AT+UFTP=1,|66.96.147.106|\r\n");
#endif
            
#ifdef K2
            sprintf(comandoAT, "AT+UFTP=0,|200.98.169.134|\r\n");
#endif
            
#ifdef MONITORAMENTO
            sprintf(comandoAT, "AT+UFTP=1,|ftp.control-up.com|\r\n");
#endif

            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 43;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UFTP2_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 300;
            timeoutEnvioEmTentativa = 300;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 18;
#ifdef FTP_TESTE_GPS
            sprintf(comandoAT, "AT+UFTP=2,|Testegps|\r\n");
#else
            sprintf(comandoAT, "AT+UFTP=2,|fernandoca|\r\n");
#endif

#ifdef VOLIT
            sprintf(comandoAT, "AT+UFTP=2,|volitsistemas01|\r\n");
#endif
            
#ifdef K2
            sprintf(comandoAT, "AT+UFTP=2,|k2tec_rfid|\r\n");
#endif
            
#ifdef MONITORAMENTO
    sprintf(comandoAT, "AT+UFTP=2,|testemonitoramento|\r\n");            
#endif

            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 44;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UFTP3_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 300;
            timeoutEnvioEmTentativa = 300;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 19;
#ifdef FTP_TESTE_GPS
            sprintf(comandoAT, "AT+UFTP=3,|Teste_gps1|\r\n");
#else
            sprintf(comandoAT, "AT+UFTP=3,|Lucas-28|\r\n");
#endif

#ifdef VOLIT
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            sprintf(comandoAT, "AT+UFTP=3,|controlup|\r\n");
#endif
            
#ifdef K2
            sprintf(comandoAT, "AT+UFTP=3,|K28t$d@0|\r\n");
#endif
            
#ifdef MONITORAMENTO
            sprintf(comandoAT, "AT+UFTP=3,|Teste.01|\r\n");
#endif
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 45;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UFTP6_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 300;
            timeoutEnvioEmTentativa = 300;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 20;
            //sprintf(comandoAT, "AT+UFTP=6,0\r\n");
            sprintf(comandoAT, "AT+UFTP=6,1\r\n");
            sprintf(respostaAT, "OK");
            estados_maquina_comandos = 46;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;


        case ESTADO_ESPERAR_UUFTPCR7__MAQUINA_COMANDOS:
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            //timeoutEnvio = 10000;
            timeoutEnvio = 30000;
            timeoutEnvioEmTentativa = 3000;
            tentativasEnvio = 1;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 21;
            //sprintf(comandoAT, "");
            memset(comandoAT,0,TAMANHO_MAXIMO_STRING_AT);
            //sprintf(respostaAT, "OK");
            sprintf(respostaAT, "+UUFTPCR: 7,1");

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            estados_maquina_comandos = 47;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_UFTPC1_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            //timeoutEnvio = 10000;
            timeoutEnvio = 30000;
            timeoutEnvioEmTentativa = 3000;
            tentativasEnvio = 1;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 21;
            sprintf(comandoAT, "AT+UFTPC=1\r\n");
            //sprintf(respostaAT, "OK");
            sprintf(respostaAT, "+UUFTPCR: 1,1");

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            estados_maquina_comandos = 47;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ESPERAR_UUFTPCR_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            //timeoutEnvio = 3000;
            //timeoutEnvio = 32000;
            timeoutEnvio = 60000;
            timeoutEnvioEmTentativa = 3000;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;//ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
            estados_maquina_envio = 22;
            //sprintf(comandoAT, "\r\n");
            //sprintf(comandoAT, "");
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            sprintf(respostaAT, "+UUFTPCR: 1,1");
            return ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS;
            break;


        case (ESTADO_ENVIAR_UFTPC7_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(resposta,0,120);

            //timeoutEnvio = 10000;
            //timeoutEnvioEmTentativa = 10000;
            //tentativasEnvio = 5;
            //timeoutEnvio = 50000;]
            timeoutEnvio = 100000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 1;


            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 23;
            /*
            rascunho_data[0] = stringData[0];
            rascunho_data[1] = stringData[1];
            rascunho_data[2] = '-';
            rascunho_data[3] = stringData[3];
            rascunho_data[4] = stringData[4];
            rascunho_data[5] = '-';
            rascunho_data[6] = '2';
            rascunho_data[7] = '0';
            rascunho_data[8] = stringData[8];
            rascunho_data[9] = stringData[9];
            rascunho_data[10] = 0;
             * */

            rascunho_data[0] = stringData[6];
            rascunho_data[1] = stringData[7];
            rascunho_data[2] = stringData[8];
            rascunho_data[3] = stringData[9];
            rascunho_data[4] = 0;

            rascunho_dia[0] = stringData[0];
            rascunho_dia[1] = stringData[1];
            rascunho_dia[2] = 0;

            stringHora[2] = '_';
            stringHora[5] = '_';
            //stringHora[6] = '0';
            //stringHora[7] = '0';
            

            //sprintf(rascunho, "1S1 %s %s %s.pkt", empilhadeira, stringHora, rascunho_data);//, stringData
            //sprintf(rascunho, "1S1 %s Fri Jan 30 %s 2015.pkt", empilhadeira, stringHora, rascunho_data);//, stringData

            //sprintf(rascunho, "1S1 %s %s %s 30 %s 2015.pkt", empilhadeira, strDiaDaSemana, strMes, stringHora, rascunho_data);//, stringData
            //sprintf(rascunho, "1S1 %s %s %s %s %s %s.pkt", empilhadeira, strDiaDaSemana, strMes, rascunho_dia, stringHora, rascunho_data);//, stringData
            
            
            //sprintf(rascunho, "1S1 %s %s %s %s %s.mnt", empilhadeira, strMes, rascunho_dia, stringHora, strAno);//, stringData
            
#ifdef MONITORAMENTO            
            //sprintf(rascunho, "1S1 %s %s %s %s 2015.mnt", empilhadeira, rascunho_dia, strMes, stringHora);//, stringData
            sprintf(rascunho, "1S1 %s %s %s %s 20%c%c.mnt", empilhadeira, rascunho_dia, strMes, stringHora, stringData[8], stringData[9]);//, stringData
#else
            //sprintf(rascunho, "1S1 %s %s %s %s %s.pkt", empilhadeira, strMes, rascunho_dia, stringHora, strAno);//, stringData
#endif
            stringHora[2] = ':';
            stringHora[5] = ':';

            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
            EnviaStringParaSerial("AT+UFTP?\r\n");
            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
            EnviaStringParaSerial("AT+UFTPER\r\n");
            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
            delay_ms(1000);//apenasp para testes
            asm("CLRWDT");
            
            //sprintf(comandoAT, "AT+UFTPC=7,|testePIC.txt|\r\n");
            sprintf(comandoAT, "AT+UFTPC=7,|%s|\r\n",rascunho);
            //sprintf(respostaAT, "CONNECT");
            sprintf(respostaAT, "CONNECT");

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            estados_maquina_comandos = 48;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;



        case (ESTADO_ESPERAR_CONNECT_DO_FTP_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 3000;
            timeoutEnvioEmTentativa = 3000;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
            estados_maquina_envio = 24;
            //sprintf(comandoAT, "");
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            sprintf(respostaAT, "CONNECT");
            return ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS;
            break;

        case ESTADO_ESPERAR_PARA_ENVIAR_3MAIS_MAQUINAS_COMANDOS:
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            //timeoutEnvio = 2100; //A maquina esta levando mais tempo do ue 1ms para cada passada
            timeoutEnvio = 1000;
            timeoutEnvioEmTentativa = 100;
            tentativasEnvio = 1;
            resultado = 1;
            estadoEnvio = ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
            estados_maquina_envio = 24;
            //sprintf(comandoAT, "");
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case (ESTADO_ENVIAR_3MAIS_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 5000;
            //timeoutEnvioEmTentativa = 10000;
            timeoutEnvioEmTentativa = 5000;
            tentativasEnvio = 2;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 25;
            //delay_ms(2100);
            sprintf(comandoAT, "+++");
            sprintf(respostaAT, "DISCONNECT");
            //sprintf(respostaAT, "OK");

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            
            //sprintf(respostaAT, "OK");
            //sprintf(respostaAT, "");
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);
            //return ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS;
            estados_maquina_comandos = 49;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;



        case(ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS):

            //int MaquinaDeEstadosAT (int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado){
            //estadoEnvio = MaquinaDeEstadosAT(estadoEnvio,comandoAT, respostaAT, &timeoutEnvio, &resposta, &resultado);
            estadoEnvio = MaquinaDeEstadosAT(estadoEnvio,comandoAT, respostaAT, &timeoutEnvio, resposta, &resultado);   //LUCIANO: Passando corretasmente o endereco da string resposta. Eliminei um warning.
            if (estadoEnvio == ESTADO_OCIOSO_MAQUINA_AT){
                if (resultado > 0)
                {
                    estados_maquina_comandos = 54;
                    return ESTADO_SUCESSO_MAQUINA_COMANDOS;
                }
                if ( (timeoutEnvio == 0) || (resultado < 0) ){
                    timeoutEnvio = timeoutEnvioEmTentativa;
                    if (tentativasEnvio > 1)
                    {
                        tentativasEnvio = tentativasEnvio - 1;
                        timeoutEnvio = timeoutEnvioEmTentativa;
                        estadoEnvio = ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
                        estados_maquina_envio = 26;
                        return ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS;
                    }
                    else
                    {
                        estados_maquina_comandos = 57;
                        return ESTADO_ERRO_MAQUINA_COMANDOS;
                    }
                }
            }

            return ESTADO_ESPERANDO_MENSAGEM_NAO_SOLICITADA_MAQUINA_COMANDOS;
            break;




        case (ESTADO_ENVIAR_CIMI_MAQUINA_COMANDOS):
            timeoutEnvio = 100;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 27;
            estadoEnvio = MaquinaDeEstadosAT(estadoEnvio,"AT+CIMI", "", &timeoutEnvio, resposta, &resultado);   //LUCIANO: Corrigindo os parametros. Eliminei warnings.
            estados_maquina_comandos = 50;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            break;

        case (ESTADO_ESPERAR_CIMI__RESPOSTA_MAQUINA_COMANDOS):
            break;
        case (ESTADO_ESPERAR_RESPOSTA_NAO_SOLICITADA_MAQUINA_COMANDOS):
            break;
        case (ESTADO_ATRASO_MAQUINA_COMANDOS):
            break;

        case (ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS):
            estadoEnvio = MaquinaDeEstadosAT(estadoEnvio,comandoAT, respostaAT, &timeoutEnvio, resposta, &resultado);   //LUCIANO: Corrigindo a passagem dos parametros. Eliminei warnings.
            /*
            //if (strstr(memchr(resposta,respostaAT[0],strlen(resposta)),respostaAT)){
            if (ProcuraSubStringEmString(resposta,respostaAT) == 1){
                estados_maquina_comandos = 55;
                return ESTADO_SUCESSO_MAQUINA_COMANDOS;
            }
            */


            if (estadoEnvio == ESTADO_OCIOSO_MAQUINA_AT)
            {


                if (resultado > 0)
                {
                    estados_maquina_comandos = 55;
                    return ESTADO_SUCESSO_MAQUINA_COMANDOS;
                }
                if ( (timeoutEnvio == 0) || (resultado < 0) ){
                    timeoutEnvio = timeoutEnvioEmTentativa;
                    if (tentativasEnvio > 1)
                    {
                        tentativasEnvio = tentativasEnvio - 1;
                        //timeoutEnvio = timeoutEnvioEmTentativa;
                        estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
                        estados_maquina_envio = 28;
                        estados_maquina_comandos = 51;
                        return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
                    }
                    else
                    {
                        estados_maquina_comandos = 58;
                        return ESTADO_ERRO_MAQUINA_COMANDOS;
                    }
                }
            }
            estados_maquina_comandos = 52;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;



        case (ESTADO_DESLOGA_DO_FTP_MAQUINA_COMANDOS):
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 300;
            timeoutEnvioEmTentativa = 300;
            tentativasEnvio = 10;
            resultado = 1;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 29;
            sprintf(comandoAT, "AT+UFTPC=0\r\n");
            sprintf(respostaAT, "OK");
            //sprintf(respostaAT, "");
            //memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            LimpaBufferRX3();
            CompararEmTempoReal = 1;

            estados_maquina_comandos = 53;
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;

            break;

        case ESTADO_DESCONECTA_DA_REDE_MAQUINA_COMANDOS:
            memset(comandoAT, 0, TAMANHO_MAXIMO_STRING_AT);
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);

            timeoutEnvio = 1000;
            timeoutEnvioEmTentativa = 1000;
            tentativasEnvio = 10;//10
            resultado = 0;
            estadoEnvio = ESTADO_ENVIAR_MAQUINA_AT;
            estados_maquina_envio = 30;
            //sprintf(comandoAT, "AT+UPSDA=0,4\r\n");
            sprintf(comandoAT, "AT+CPWROFF\r\n");
            //sprintf(respostaAT, "OK");
            //sprintf(respostaAT, "");
            memset(respostaAT, 0, TAMANHO_MAXIMO_STRING_AT);
            //desliga_led_3g();
            return ESTADO_ESPERA_RESPOSTA_MAQUINA_COMANDOS;
            //return ESTADO_OCIOSO_MAQUINA_COMANDOS;

            break;

        case (ESTADO_OCIOSO_MAQUINA_COMANDOS):
            break;
        case (ESTADO_SUCESSO_MAQUINA_COMANDOS):
            estados_maquina_comandos = 56;
            return ESTADO_SUCESSO_MAQUINA_COMANDOS;
            break;
        case (ESTADO_ERRO_MAQUINA_COMANDOS):
            estados_maquina_comandos = 59;
            return ESTADO_ERRO_MAQUINA_COMANDOS;
            break;
        default:
            estados_maquina_comandos = 27;
            return ESTADO_ERRO_MAQUINA_COMANDOS;
            break;
    }
    return estado;
}

void IniciaContadorDeJanela (void){
    contadorDeJanelaDeTempo = contadorDeJanelaDeTempo + 1;

}

void IniciaMaquinaGPRS(void)
{
    //U3MODE = U3MODE | 0x8000;
    estados_maquina_envio = ESTADO_OCIOSO_MAQUINA_AT;
    estados_maquina_comandos = ESTADO_LIGAR_MODEM_MAQUINA_COMANDOS;
    estados_maquina_acao_gps = ESTADO_LIGAR_MODEM_ACAO_GPRS;
    estadoAcao = ESTADO_LIGAR_MODEM_ACAO_GPRS;
    //estados_maquina_acao_gps = 1;
    ContadorParaPulsoDeLigarOModem = 0;
    ContadorParaOModemInicar = 0;
    resetNasVariaisDeModemEmTempoReal();
    contadorParaUmSegundoLedGprs = 1000; 

    contadorParaDispararAMaquinaDeEstadosDoModem = TEMPO_PARA_CADA_ATIVACAO_DA_MAQUINA_DO_MODEM;

    
    
#ifdef MODEM_V5
    _TRISF0 = 0;
    _LATF0 = 1;    
#else 
    //_TRISG12 = 0;
    //_LATG12 = 1;
    //_LATG12 = 0;
#endif
    

    
    todasAsTagsEnviadas = 0;
}

void DesligaMaquinaGPRS(void){

    //estadoAcao = ESTADO_FTP_FECHA_ARQUIVO_ACOES_GPRS;
    estadoAcao = ESTADO_PREPARAR_PARA_ENVIAR_3MAIS_ACOES_GPRS;
    estadoLedsGprs = APAGADO;
    //estadoAcao = ESTADO_ERRO_MAQUINA_ACAO_GPRS; // FORCA DESLIGAR
}

int Ftp_ocioso (void){
    if (estadoAcao == ESTADO_OCIOSA_MAQUINA_ACAO_GPRS)return 1;
    else return 0;
}


int Ftp_pronto (void){
    if (estadoAcao == ESTADO_FTP_PRONTO_ACOES_GPRS)return 1;
    else return 0;
}

int FtpForamEnviadasTodasAsTags (void){
    if (todasAsTagsEnviadas == 1)
        return 1;
    return 0;
}

void FtpPrepararParaEnviarTags (void){
    todasAsTagsEnviadas = 0;
    contadorDetagsEnviadasAoFtp = 0;
}

void FtpTerminouDeEnviarTodasAsTags (void){
    todasAsTagsEnviadas = 1;
}

int Ftp_falhou (void){
    if (estadoAcao == ESTADO_ERRO_MAQUINA_ACAO_GPRS)return 1;
    else return 0;
}

/*
 *
//Funciona mas tem limitacoes de tempo e conflita com o inventory
int MaquinaDeEstadosAT (int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado){
    //int contador;
    int tamanho;
    switch(estado){
        case ESTADO_ENVIAR_MAQUINA_AT:
            while( (*comando != 0) && (*comando < 128) )
            {
                if (*comando == '|')
                    EnviaByteParaSerial('"');
                else EnviaByteParaSerial(*comando);
                comando = comando + 1;
            }

            estados_maquina_envio = 31;
            return ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
            break;

        case ESTADO_RECEBER_RESPOSTA_MAQUINA_AT:
            if (*timeout){
                *timeout = *timeout - 1;
                delay_ms(1);
            }
            else {

                tamanho = uartRx3NumBytesAvailable();
                if (tamanho)
                {
                    uartRx3NBytes(resposta, &tamanho);
                    estados_maquina_envio = 32;
                    return ESTADO_RESPOSTA_RECEBIDA_MAQUINA_AT;

                }

                *resultado = -1;
                estados_maquina_envio = 33;
                return ESTADO_OCIOSO_MAQUINA_AT;


            }
            return estado;
            break;

        case ESTADO_RESPOSTA_RECEBIDA_MAQUINA_AT:
            if (strstr(resposta, esperado) != NULL)
            {
                *resultado = 1;

            } 
            else
            {
                *resultado = -1;
            }
            estados_maquina_envio = 34;
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;


        case ESTADO_ESPERAR_COMANDO_NAO_REQUISITADO_MAQUINA_AT:
            estados_maquina_envio = 35;
            break;

        case ESTADO_OCIOSO_MAQUINA_AT:
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;

        default:
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;
    }
    return estado;
}
*/

void LimpaReposta (void){
    int contador;
    
    for (contador = 0;contador < TAMANHO_BUFFER_REPOSTA;contador = contador + 1){
        resposta[contador] = 0;
    }
    
}

int TratarCGED (char*comando, char *resultado, int timeout){
    int contemOK, contemERROR;
    int tamanho;
    int QuantiaDeDadosObtidos;
    int contador;
    LimpaBufferRX3();
    LimpaReposta();
    EnviaByteParaSerial(*comando);
    
    contemOK = 0;
    contemERROR = 0;
    
    contador = timeout;
    QuantiaDeDadosObtidos = 0;
    
    EnviaStringParaSerial(comando);
    
    while (contemOK == 0 && contemERROR == 0 && contador != 0){
        tamanho = uartRx3NumBytesAvailable();
        QuantiaDeDadosObtidos = QuantiaDeDadosObtidos + tamanho;
        
        if (QuantiaDeDadosObtidos >= TAMANHO_MAXIMO_DO_TEXTO_DE_INFORAMACAO_DAS_TORRES){
            return 1;
        }
                
        if (tamanho){
            uartRx3NBytes((u8*)resultado, (u16*)&tamanho);  //LUCIANO: Apliquei cast. Eliminei warnings.
            resultado = resultado + tamanho;
        }
        if (strstr(resultado,"OK") != NULL)
            contemOK = 1;
        if (strstr(resultado,"ERROR") != NULL)
            contemERROR = 1;
        delay_ms(1);
        contador = contador - 1;
    }
    
    if (contemOK)return 1;
    //if (contemERROR)return -1;
    return -1;
    
}

int MaquinaDeEstadosAT (int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado){
    //int contador; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.
    u16 tamanho;
    switch(estado){
        case ESTADO_ENVIAR_MAQUINA_AT:
            LimpaBufferRX3();
            LimpaReposta();
            //EnableIntU3RX;
            
            while( (*comando != 0) && (*comando < 128) )
            {
                if (*comando == '|')
                    EnviaByteParaSerial('"');
                else EnviaByteParaSerial(*comando);
                comando = comando + 1;
            }

            ContadorDeTempoPassadoParaMaquinaDeComandosAT = *timeout; //determino o tempo limite do comando

            estados_maquina_envio = 31;
            return ESTADO_RECEBER_RESPOSTA_MAQUINA_AT;
            break;

        case ESTADO_RECEBER_RESPOSTA_MAQUINA_AT:

            if ( (ContadorDeTempoPassadoParaMaquinaDeComandosAT == 0) || (ComparacaoValidaEmTempoReal == 1) ){
                *timeout = 0;

                if (ComparacaoValidaEmTempoReal == 1){
                    resetNasVariaisDeModemEmTempoReal();
                    *resultado = 1;
                    _LATC14 = 0;
                    return ESTADO_OCIOSO_MAQUINA_AT;

                }
                
                //delay_ms(1);

                _TRISA0 = 0;
                _LATA0 = 1;

                _TRISC14 = 0;

                _LATC14 = 1;

            
                tamanho = uartRx3NumBytesAvailable();
                if (tamanho > 3)
                {
                    uartRx3NBytes((u8*)resposta, (u16*)&tamanho);   //LUCIANO: Apliquei casts. Eliminei warnings.
                    estados_maquina_envio = 32;
                    _LATA0 = 0;
                    return ESTADO_RESPOSTA_RECEBIDA_MAQUINA_AT;
                }
                _LATA0 = 0;

                *resultado = -1;
                estados_maquina_envio = 33;
                _LATC14 = 0;
                return ESTADO_OCIOSO_MAQUINA_AT;


            }
            return estado;
            break;

        case ESTADO_RESPOSTA_RECEBIDA_MAQUINA_AT:

            if (*esperado == 0){ // Nao espera nada, entao retornar com sucesso
                *resultado = 1;
                _LATC14 = 0;
                return ESTADO_OCIOSO_MAQUINA_AT;
            }

            //if (ProcuraSubStringEmString(resposta, esperado) == 1)
#ifdef DEBUG_DE_COMANDOS_AT
            EnviaStringDebugParaSerial("Dados a serem comparados:");
            EnviaStringDebugParaSerial(resposta);
            EnviaStringDebugParaSerial("  E  ");
            EnviaStringDebugParaSerial(esperado);
            EnviaStringDebugParaSerial("  POSSIVEL  ");
            EnviaStringDebugParaSerial(BufferGPRS);
            
#endif

            if (strstr(resposta, esperado) != NULL)
            {
                *resultado = 1;

            }
            else
            {
                *resultado = -1;
                _LATC14 = 0;
            }

            /*
            if (strstr (respostaAT, rx3Buffer) != NULL){
                *resultado = 1;
            }
             * */
            
            estados_maquina_envio = 34;
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;


        case ESTADO_ESPERAR_COMANDO_NAO_REQUISITADO_MAQUINA_AT:
            estados_maquina_envio = 35;
            break;

        case ESTADO_OCIOSO_MAQUINA_AT:
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;

        default:
            return ESTADO_OCIOSO_MAQUINA_AT;
            break;
    }
    return estado;
}





int procuraPosicaoComJanelaDeTempoDesatualizadaNoBufferDeFTP (void){
    int contador;

    for (contador = 0; contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        if (registroDeJanelaDeTempo[contador] != contadorDeJanelaDeTempo)
            return contador;
    }

    return -1;
}

int procuraPossicaoLivreNoBufferDeFtp (void){
    int contador;
    
    for(contador = 0;contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        if (bufferFTP[contador][0] == 0)
            return contador;
    }

    return -1;
}

void limpaPossicaoNoBufferDeFtp (int posicao){
    //int contador; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.

    memset(bufferFTP[posicao], 0, TAMANHO_MAXIMO_STRING_FTP);

    /*
    for (contador = 0;contador < TAMANHO_MAXIMO_STRING_FTP;contador = contador +1){ //Usada por EDS nao ser aceito em ANSI-C
        bufferFTP[posicao][contador] = 0;
    }
     * */
    
    if (conta_registros)conta_registros--;
}


int escreverNoBufferDeFtp(char *tag, char *operador, int evento, int antena, int rssi){
    int posicao;
    //int contador; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.
    //char rascunho[TAMANHO_BUFFER_FTP];

    posicao = procuraPossicaoLivreNoBufferDeFtp();

    //SObreescrever possicoes velhas
    /*
    if (posicao == -1){
        posicao = procuraPosicaoComJanelaDeTempoDesatualizadaNoBufferDeFTP();
    }
     * */


    if (posicao > -1){
        registroDeJanelaDeTempo[posicao] = contadorDeJanelaDeTempo;
        //sprintf(data, "1/1/15");
        //sprintf(hora, "12:00");
        //sprintf(bufferFTP[posicao],"1S1,%s,%s,%s,OK,0,%s,%s,0,0", tag, stringData, stringHora, operador, empilhadeira, int evento);
        //$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D
        //sprintf(bufferFTP[posicao],"1S1,%s, %d, %d, %s,%s,OK,%d,%s,%s,0,0", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,OK,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        stringData[2] = ' ';
        stringData[5] = ' ';
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);

#ifdef VOLIT

        /*
        //Esse trecho eh para quando uso o espaco EDS, e o ANSI-C nao suporta ele.
        sprintf(rascunho,"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        for (contador = 0;(contador < TAMANHO_BUFFER_FTP) && (bufferFTP[posicao][contador] != 0);contador = contador + 1){
            bufferFTP[posicao][contador] = rascunho[contador];
        }
         * */

        //VOLIT
        sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        
        //TESTE MONITORAMENTO
        

        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
#elif SANKYU
        sprintf(bufferFTP[posicao],"1S2,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
#elif K2TEC
        sprintf(bufferFTP[posicao],"1S4,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
#elif ALPHAHEAD
        sprintf(bufferFTP[posicao],"1S5,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
#elif PORTAL_FRANGO
        //sprintf(bufferFTP[posicao],"1S5,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
        sprintf(bufferFTP[posicao],"1S5,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", "001", stringData, stringHora, evento, tag, rssi, antena, operador);



#elif MONITORAMENTO
        char RMCPura[100];
        RepassaRMC(RMCPura);
        sprintf(bufferFTP[posicao],"1S1,%s,%s,%s,%s,%s\r\n", tag, stringData, stringHora, "Evento A",RMCPura);        
        
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,Resets,%u,%u,%u,%u,%u,GPRS,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS);
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);

#else
        /* // 13/10/15
        if (StringResultadoValidadeGPS[0] == 'A'){
            sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        } else {
            sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS);
        }
        */
#endif

        /*
        sprintf(bufferFTP[posicao + 1],"1S1,%s,%d,%d,%s,%s,2,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        sprintf(bufferFTP[posicao + 2],"1S1,%s,%d,%d,%s,%s,3,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        sprintf(bufferFTP[posicao + 3],"1S1,%s,%d,%d,%s,%s,4,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        sprintf(bufferFTP[posicao + 4],"1S1,%s,%d,%d,%s,%s,5,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        sprintf(bufferFTP[posicao + 5],"1S1,%s,%d,%d,%s,%s,6,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
         * */
        //sprintf(bufferFTP[posicao + 6],"1S1,%s,%d,%d,%s,%s,7,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        //sprintf(bufferFTP[posicao + 7],"1S1,%s,%d,%d,%s,%s,8,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        //sprintf(bufferFTP[posicao + 8],"1S1,%s,%d,%d,%s,%s,9,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        //sprintf(bufferFTP[posicao + 9],"1S1,%s,%d,%d,%s,%s,A,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        //sprintf(bufferFTP[posicao + 1],"1S1,%s,%d,%d,%s,%s,OK,%d,%s,%s,$GPRMC,173114.000,A,2330.7715,S,04635.0776,W,0.00,,250814,,*1D", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        //sprintf(hora, "11:00");
    }

    return 0;
}

void ProcessEnvioDeTagsGPRS (void){
    int resultado = 1;
    char BufferParaEventosNV[129];    
    
    if  (Ftp_pronto()){
        if  (contadorDetagsEnviadasAoFtp < (TAMANHO_BUFFER_FTP - MARGEM_PARA_LIDAR_COM_O_BUG_DE_ENVIO)){
                if (bufferFTP[contadorDetagsEnviadasAoFtp][0] == '1'){
                EnviaStringParaSerial(&bufferFTP[contadorDetagsEnviadasAoFtp][0]);
            }	
            contadorDetagsEnviadasAoFtp = contadorDetagsEnviadasAoFtp + 1;
        }
        
        //Procurar a I2C.c que contenha essa rotina 11/10/16
        //resultado = RetornaStringDeEventoNV(&BufferParaEventosNV[0]);
        
        if(resultado == 1){
            EnviaStringParaSerial(&BufferParaEventosNV[0]);
        }
        
        if ((contadorDetagsEnviadasAoFtp >= (TAMANHO_BUFFER_FTP - MARGEM_PARA_LIDAR_COM_O_BUG_DE_ENVIO)) && (resultado == 0)){
            FtpTerminouDeEnviarTodasAsTags();           
        }
                
        /*
        if (resultado < 0){ //Finaliza, mas ocorreu algum erro;
              FtpTerminouDeEnviarTodasAsTags(); 
        }
        */        

        /*
        if (resultado == 0){
              FtpTerminouDeEnviarTodasAsTags(); 
              while (escreverNoBufferDeFtpNV("1234567890-", "Gilson", 0, 1, 80) != 0);
        }
        */


        
    }
    
    
}



void ProcessaGPRS (void){

        //pisca_led_3g();
        estadoAcao = MaquinaDeAcoesGPRS(estadoAcao);
        if (contadorRtc == 0){
            //pisca_led_3g();
            testeI2C();
            contadorRtc = 1000;//A cada um segundo atualizo as variaveis do RTC



            /*
            if (estadoLedsGprs == PISCAR_ACESSO){
                liga_led_3g();
                estadoLedsGprs  = PISCAR_APAGADO;
            } else if (estadoLedsGprs  == PISCAR_APAGADO){
                desliga_led_3g();
                estadoLedsGprs = PISCAR_ACESSO;
            }

            if (estadoLedsGprs == APAGADO){
                liga_led_3g();
            }

            if (estadoLedsGprs == ACESSO){
                desliga_led_3g();
            }
             * */

        }

        if (Ftp_ocioso() == 1){
            //if ( ( (minuto % 5) == 0) && (ContadorParaPulsoDeLigarOModem == 0) && (ContadorParaOModemInicar == 0) ){
            if ( ( contadorParaDispararAMaquinaDeEstadosDoModem == 0) && (ContadorParaPulsoDeLigarOModem == 0) && (ContadorParaOModemInicar == 0) ){

                //pow_off_modem();
                _LATA2 = 1;
                ContadorParaPulsoDeLigarOModem = VALOR_DE_TEMPO_PARA_PULSO_DE_LIGAR_O_MODEM;
                ContadorParaOModemInicar = VALOR_DE_TEMPO_PARA_LIGAR_O_MODEM;
            }

            if (ContadorParaPulsoDeLigarOModem == 0 && (ContadorParaOModemInicar > 0) ){
                //pow_on_modem();
                _LATA2 = 0;
                ContadorParaOModemInicar = VALOR_DE_TEMPO_PARA_LIGAR_O_MODEM;
                ContadorParaPulsoDeLigarOModem = -1;
            }

            if ( (ContadorParaOModemInicar == 0) && (ContadorParaPulsoDeLigarOModem == -1) ){
                ContadorParaOModemInicar = -1;
                IniciaMaquinaGPRS();
            }
        }

        //int minuto, int hora, int dia, int mes, int ano){

        //delay_ms(100);

        
        //if (Ftp_pronto()){//140815
        if (FtpForamEnviadasTodasAsTags() && Ftp_pronto()){
        
#ifdef VOLIT
            //cabecaVolit();
#endif
            //EnviaStringParaSerial("Teste um dois tres quatro cinco");
            /*
            posicao_buffer_ftp = posicaoOpcuapada();

            while(posicao_buffer_ftp != -1){
                EnviaStringParaSerial(&bufferFTP[posicao_buffer_ftp][0]);
                limpaPossicaoNoBufferDeFtp(posicao_buffer_ftp);
                posicao_buffer_ftp = posicaoOpcuapada();
            }
             * */

            //EnviaTags ();//Comentado em 140815 para p algoritmo de envio de tags no main.c

            EnviaStringParaSerial("\n\rEND\n\r");

            status_conexao = 6; // GILSON, ARQUIVO ENVIADO COM SUCESSO

            pisca_led_3g();
            conta_envio++;
            //testeI2C();

            //sprintf(empilhadeira, "EFOOO11");
            //escreverNoBufferDeFtp("ED000","EA0001", 7, 8, 0);
            //escreverNoBufferDeFtp("ED000","EA0001", 8, 8, 0);


            
            //delay_ms(2100);
            DesligaMaquinaGPRS();
            //delay_ms(10000);
        }

        /*
        if (Ftp_falhou()){3
            //delay_ms(10000);
            //IniciaMaquinaGPRS();
            //DesligaMaquinaGPRS();

            //Funciona, mas bloqueia o codigo
            //delay_ms(2100);
            //EnviaStringParaSerial("+++");
            //delay_ms(1100);
            //EnviaStringParaSerial("AT+CPWROFF\r\n");
            //estadoAcao = ESTADO_OCIOSA_MAQUINA_ACAO_GPRS;


            if ( (ContadorParaOsTresMais == 0) && (ContadorParaMandarOComandoDeDesligarOModem  == 0) ){
                ContadorParaOsTresMais = VALOR_DE_TEMPO_PARA_O_TRES_MAIS;
                ContadorParaMandarOComandoDeDesligarOModem = VALOR_DE_TEMPO_PARA_ENVIAR_O_COMANDO_DE_DESLIGAR_O_MODEM + VALOR_DE_TEMPO_PARA_O_TRES_MAIS;
            }


            if (ContadorParaOsTresMais == 0){
                //delay_ms(2100);
                EnviaStringParaSerial("+++");
                ContadorParaOsTresMais = -1;
                ContadorParaMandarOComandoDeDesligarOModem = VALOR_DE_TEMPO_PARA_ENVIAR_O_COMANDO_DE_DESLIGAR_O_MODEM;
            }

            if (ContadorParaMandarOComandoDeDesligarOModem == 0)
            {
                estadoLedsGprs = APAGADO;
                EnviaStringParaSerial("AT+CPWROFF\r\n");
                ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar = VALOR_DE_TEMPO_PARA_DEIXAR_AS_MAQUINAS_DE_ESTADOS_VOLTAREM_A_OPERAR;
                //ContadorParaOsTresMais = 0;
                ContadorParaMandarOComandoDeDesligarOModem = -1;
            }
                       

            if (ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar == 0){
                estados_maquina_acao_gps = 52;
                ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar = -1;
                estadoAcao = ESTADO_OCIOSA_MAQUINA_ACAO_GPRS;


            }

        }
         * */

}

/*
void LigaRX (void){
    RPINR18 = 0x1F07;//TX1
    RPINR19 = 0x0004;//TX2
    RPINR17 = 0x1800;//TX3
    RPINR27 = 0x001B;//TX0
    IPC2 = 0x4000;
    //U1MODE = 0x8000;

    IEC0bits.U1RXIE = 1;
    IEC1bits.U2RXIE = 1;
    IEC5bits.U3RXIE = 1;
    IEC5bits.U4RXIE = 1;
};
*/


void EnviaTags (void){
    //int base; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.
    //int contador; //LUCIANO: Variavel sem uso. Comentei para eliminar um warning.
    //posicao_buffer_ftp = posicaoOpcuapada();
    posicao_buffer_ftp = 0;
    //posicao_buffer_ftp = posicaoOpcuapadaENaoEnviada();

    //while(posicao_buffer_ftp != -1){
    while (posicao_buffer_ftp < TAMANHO_BUFFER_FTP){

        if (bufferFTP[posicao_buffer_ftp][0] == '1'){

            /*
            // espaco EDS nao suportado por ANSI-C
            for (contador = 0; contador < TAMANHO_MAXIMO_STRING_FTP; contador = contador + 1){
                subBufferParaEnvioDeTagsNaSerial[contador] = bufferFTP[posicao_buffer_ftp][contador];
            }
            EnviaStringParaSerial(subBufferParaEnvioDeTagsNaSerial);
             * */

            EnviaStringParaSerial(&bufferFTP[posicao_buffer_ftp][0]);
            
            delay_ms(100); // APENAS PARA TESTES, ACHO QUE O MODEM NAO SUPORTA MAIS QUE 4096 BYTES DE FTP
            asm("CLRWDT");
        }
        //EnviaStringParaSerial(&bufferFTP[posicao_buffer_ftp][0]);
        EnviadoFTP[posicao_buffer_ftp] = EnviadoFTP[posicao_buffer_ftp] + 1;

        posicao_buffer_ftp = posicao_buffer_ftp + 1;
        
        //limpaPossicaoNoBufferDeFtp(posicao_buffer_ftp);
        //posicao_buffer_ftp = posicaoOpcuapadaENaoEnviada();
        //base = posicao_buffer_ftp;
        //posicao_buffer_ftp = posicaoOpuapada();
        //if (posicao_buffer_ftp <= base)posicao_buffer_ftp = -1;
    }

};

void AnulaOEnvioDaTag (void){
    posicao_buffer_ftp = posicaoOpcuapada();
    
    while(posicao_buffer_ftp != -1){
        //EnviaStringParaSerial(&bufferFTP[posicao_buffer_ftp][0]);
        //limpaPossicaoNoBufferDeFtp(posicao_buffer_ftp);
        //Limpa as tags ja enviadas
        if (EnviadoFTP[posicao_buffer_ftp] > 0)EnviadoFTP[posicao_buffer_ftp] = 0;
        posicao_buffer_ftp = posicaoOpcuapada();
    }

}


void LimpaTagEnviada(void){
    posicao_buffer_ftp = posicaoOpcuapada();

    while(posicao_buffer_ftp != -1){
        //EnviaStringParaSerial(&bufferFTP[posicao_buffer_ftp][0]);
        limpaPossicaoNoBufferDeFtp(posicao_buffer_ftp);
        
        //Limpa as tags ja enviadas
        //if (EnviadoFTP[posicao_buffer_ftp] > 0)limpaPossicaoNoBufferDeFtp(posicao_buffer_ftp);
        posicao_buffer_ftp = posicaoOpcuapada();
        asm("CLRWDT");
    }
    
}

void ReiniciaLogicaDeJanela (void){
    int contador;
    
    for (contador = 0;contador < TAMANHO_BUFFER_FTP;contador = contador + 1){
        registroDeJanelaDeTempo[contador] = 0;
    }
        
    contadorDeJanelaDeTempo = 0;

    contadorParaIncrementeDoContadorDeJanelaDeTempo = 0;
    
}

void MaquinaDeLedGprs (void){
    switch (estadoLedsGprs){
        case APAGADO:
            desliga_led_3g();
            break;

        case PISCAR_ACESSO:
            liga_led_3g();
            estadoLedsGprs = PISCAR_APAGADO;
            break;

        case PISCAR_APAGADO:
            desliga_led_3g();
            estadoLedsGprs = PISCAR_ACESSO;
            break;

        case ACESSO:
            liga_led_3g();
            break;

        default:
            estadoLedsGprs = APAGADO;
            break;
    }

}


void OperacoesParaTickGprs (void){
    //_TRISA9 = 0;


    contadorParaIncrementeDoContadorDeJanelaDeTempo = contadorParaIncrementeDoContadorDeJanelaDeTempo + 1;
    if (contadorParaIncrementeDoContadorDeJanelaDeTempo > TICKS_PARA_OCORRER_UM_INCREMENTE_DA_JANELA_DE_TEMPO){
        contadorParaIncrementeDoContadorDeJanelaDeTempo = 0;
        contadorDeJanelaDeTempo = contadorDeJanelaDeTempo + 1;
    }



    if (contadorRtc)contadorRtc = contadorRtc - 1;

    contadorParaUmSegundoLedGprs = contadorParaUmSegundoLedGprs - 1;
    if (contadorParaUmSegundoLedGprs == 0){
        contadorParaUmSegundoLedGprs = 1000;
        MaquinaDeLedGprs();
    }

    //_LATA9 = !_LATA9;

    if (contadorParaDispararAMaquinaDeEstadosDoModem > 0){
        contadorParaDispararAMaquinaDeEstadosDoModem = contadorParaDispararAMaquinaDeEstadosDoModem - 1;
    }


    if (ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar > 0){
        ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar = ContadorParaDeixarAMaquinaDeEstadosVoltarAOperar - 1;
    }

    if (ContadorDeTempoPassadoParaMaquinaDeComandosAT > 0){
        ContadorDeTempoPassadoParaMaquinaDeComandosAT = ContadorDeTempoPassadoParaMaquinaDeComandosAT - 1;
    }

    if (ContadorParaOsTresMais)
        ContadorParaOsTresMais = ContadorParaOsTresMais - 1;
    if (ContadorParaMandarOComandoDeDesligarOModem > 0)
        ContadorParaMandarOComandoDeDesligarOModem = ContadorParaMandarOComandoDeDesligarOModem - 1;
    if (ContadorParaPulsoDeLigarOModem > 0)
        ContadorParaPulsoDeLigarOModem = ContadorParaPulsoDeLigarOModem - 1;
    if (ContadorParaOModemInicar > 0)
        ContadorParaOModemInicar = ContadorParaOModemInicar - 1;

    //(void)sprintf(arrayXbee, "teste um dois t 2");

    /*
    contadorTimeOutRX = contadorTimeOutRX + 1;
    if (contadorTimeOutRX == 100){//fecha pacote, vai para o tratamento
        contadorRX = 0;
        contadorTimeOutRX = 0;
        if (contadorRX <= TAMANHO_ARRAY_XBEE){ //se maior que o tamanho do array, eh invalido
            //tratamento aqui


        }
    }
     * */

    if (contadori2c)contadori2c = contadori2c - 1;



}

void resetNasVariaisDeModemEmTempoReal (void){
    CompararEmTempoReal = 0;
    ComparacaoValidaEmTempoReal = 0;
}

void compararRespostaDoModemEmTempoReal(char * buffer){
    if (CompararEmTempoReal){
#ifdef DEBUG_DE_COMANDOS_AT
        /*
        EnviaStringDebugParaSerial("Dados a serem comparados:");
        EnviaStringDebugParaSerial(buffer);
        EnviaStringDebugParaSerial(respostaAT);
         * */
#endif
        
        if (strstr(buffer, respostaAT) != NULL){
        //if (strstr(respostaAT, buffer) != NULL){
            ComparacaoValidaEmTempoReal = 1;
        }
    }
}

int tagJaPresenteNoBufferGps (unsigned char * tag, int posicao, int tamanho){
    //int contador; //LUCIANO: Variavel sem uso. Comentei para eliminr um warning.


    /*
    for (contador = 4;contador < tamanho;contador = contador + 1){
        if (bufferFTP[posicao][contador] != tag)
            return 0;
    }
     * */


    //if (memcmp (&bufferFTP[posicao][0], tag, tamanho) == 0)
    // Comentado pelo o espaco de memoria EDS nao ser aceito em rotinas ANSI-C
    if (memcmp (&bufferFTP[posicao][4], tag, tamanho) == 0)
        return 0;
    
//    if (memcmp (&BufferDeTags[posicao][0], tag, tamanho) == 0)
//        return 0;




    //for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        //if (memcmp (&BufferDeTags[contador][posicao], &tag[posicao], tamanho) == 0)
        //if (memcmp (&BufferDeTags[contador][posicao], &tag[posicao], tamanho) == 0)
          //  return 0;
    //}

    return -1;
}


int RetornaComAUltimaJanelaValidaDeUmaTag (char *rascunho, int tamanho){
    int contador;
    int resultado;

    for (contador = 0;contador < (TAMANHO_BUFFER_FTP - 1 );contador = contador + 1) {
        if ( tagJaPresenteNoBufferGps ((unsigned char*)rascunho, contador, tamanho ) == 0){ //LUCIANO: Apliquei um cast. Eliminei um warning.
            resultado = registroDeJanelaDeTempo[contador];
        }

    }
    
    return resultado;
}

int RetornaComAUltimaPosicaoDeUmaTag (char *rascunho, int tamanho){
    int contador;
    int resultado;

    for (contador = 0;contador < (TAMANHO_BUFFER_FTP - 1 );contador = contador + 1) {
        if ( tagJaPresenteNoBufferGps ((unsigned char*)rascunho, contador, tamanho ) == 0){ //LUCIANO: Apliquei um cast para eliminar um warning.
            resultado = contador;
        }

    }
    
    return resultado;
}


//Nessa rotina passo as tags da estrutura da AMS para string a serem enviadas no varal.
void ProcessaTagsParaOGPS (void){
    //char __attribute__((far)) rascunho[200];
    char rascunho[200];
    int contador;
    int contador_de_tags;
    int tamanho;

    char resultado;

    memset(rascunho, 0, 200);

    //Comentando partindo daqui para testes
    for (contador_de_tags = 0; contador_de_tags < MAXTAG;contador_de_tags = contador_de_tags + 1){
        resultado = 0;
        for (contador = 0;(contador < tags_[contador_de_tags].epclen) && (resultado == 0);contador = contador + 1){
            resultado = resultado | tags_[contador_de_tags].epc[contador];
        }


        if (resultado != 0){ // Tem tag na posicao?

            tamanho = tags_[contador_de_tags].epclen ;//+ POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1;
            for (contador = 0;contador < tamanho;contador = contador + 1){
                //rascunho[contador + POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1] = tags_[contador_de_tags].epc[contador];
                sprintf(&rascunho[contador * 2], "%02X", tags_[contador_de_tags].epc[contador]);
            }

            resultado = 0;
            for (contador = 0;contador < (TAMANHO_BUFFER_FTP - 1 );contador = contador + 1) {
                //int tagJaPresenteNoBuffer (unsigned char * tag, int posicao, int tamanho){
                //if ( tagJaPresenteNoBuffer (rascunho, contador, (tags_[contador_de_tags].epclen * 2 ) ) == 0){
                if ( tagJaPresenteNoBufferGps ((unsigned char*)rascunho, contador, tamanho ) == 0){ //LUCIANO: Apliquei um castpara eliminar um warning.
                    resultado = resultado | 1;
                }

            }

#ifdef DEBUG_DE_PERFORMACE_DE_LEITURA_E_TRATAMENTO_DE_TAGS
            envioDeStringParaDebugPelaA485 (".");
#endif
            if (resultado == 0){
                //escreverNoBufferDeFtp(char *tag, char *operador, int evento, int antena, int rssi)
                escreverNoBufferDeFtp(rascunho, "9000", 1, 4, 80);
#ifdef DEBUG_DE_PERFORMACE_DE_LEITURA_E_TRATAMENTO_DE_TAGS
                envioDeStringParaDebugPelaA485 ("L,");
#endif


            } else {
                //contador
                //if (procuraPosicaoComJanelaDeTempoDesatualizadaNoBufferDeFTP()){
                //if (registroDeJanelaDeTempo[contador] != contadorDeJanelaDeTempo){
                if (RetornaComAUltimaJanelaValidaDeUmaTag(rascunho, tamanho)  != contadorDeJanelaDeTempo){
                    escreverNoBufferDeFtp(rascunho, "9000", 1, 4, 80);
                    registroDeJanelaDeTempo[contador] = contadorDeJanelaDeTempo;
#ifdef DEBUG_DE_PERFORMACE_DE_LEITURA_E_TRATAMENTO_DE_TAGS
                    envioDeStringParaDebugPelaA485 ("J,");
#endif
                }
            }
        }
    }
    //Comentando interrompido daqui para testes

}

void cabecaVolit (void){
    char rascunho[200];

    //RegistraSucessoGPRS();
    sprintf(rascunho, "Resets,%u,%u,%u,%u,%u\r\n", ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR);
    EnviaStringParaSerial(rascunho);
    sprintf(rascunho, "GPRS,%u,%u\r\n", ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
    EnviaStringParaSerial(rascunho);
    sprintf(rascunho, "GPS,%s,%s\r\n", stringResultadoLatitudeGPS,stringResultadoLongitudeGPS);
    EnviaStringParaSerial(rascunho);
}

