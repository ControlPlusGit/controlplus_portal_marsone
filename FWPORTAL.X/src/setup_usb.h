/* 
 * File:   setup_usb.h
 * Author: Marcos
 *
 * Created on 19 de Fevereiro de 2020, 15:36
 */

#ifndef SETUP_USB_H
#define	SETUP_USB_H

#include "ajuste_cap.h"


//#define FIRMWARE "FWPORTAL_V01R01"
//#define FIRMWARE "FWPORTAL_V02R03"
#define FIRMWARE "FWPORTAL_V02R04" // substituido todos os arquivos relacionados a RF para tentar resolver problema de parar de ler

#define TAMANHO_ENDERECO_IP 15
#define TAMANHO_BUFFER_COMANDOS_USB 100
#define TAMANHO_ID_LEITOR 4
#define TAMANHO_SSID_WIFI 16
#define TAMANHO_SENHA_WIFI 16
#define TAMANHO_DADOS_AJUSTE_CAPACITOR 3
#define TAMANHO_DADOS_ID_LEITOR 4


#define END_MODO_DE_OPERACAO                            0
#define END_ATRASO_PARA_DEGRADAR_LEITURA                1
#define END_REPETICAO_NA_LEITURA                        2
#define END_SENSIBILIDADE_DA_ANTENA                     3   
#define END_TEMPO_INAT_TAG_MONITORADA                   4
#define END_NUMERO_DE_ANTENAS                           5
#define END_AJUSTE_CAP0_ANTENA_1                        6
#define END_AJUSTE_CAP1_ANTENA_1                        7
#define END_AJUSTE_CAP2_ANTENA_1                        8
#define END_AJUSTE_CAP0_ANTENA_2                        9
#define END_AJUSTE_CAP1_ANTENA_2                        10
#define END_AJUSTE_CAP2_ANTENA_2                        11
#define END_AJUSTE_CAP0_ANTENA_3                        12
#define END_AJUSTE_CAP1_ANTENA_3                        13
#define END_AJUSTE_CAP2_ANTENA_3                        14
#define END_AJUSTE_CAP0_ANTENA_4                        15
#define END_AJUSTE_CAP1_ANTENA_4                        16
#define END_AJUSTE_CAP2_ANTENA_4                        17
#define END_AJUSTE_CAP0_ANTENA_5                        18
#define END_AJUSTE_CAP1_ANTENA_5                        19    
#define END_AJUSTE_CAP2_ANTENA_5                        20    
#define END_AJUSTE_CAP0_ANTENA_6                        21
#define END_AJUSTE_CAP1_ANTENA_6                        22
#define END_AJUSTE_CAP2_ANTENA_6                        23
#define END_AJUSTE_CAP0_ANTENA_7                        24
#define END_AJUSTE_CAP1_ANTENA_7                        25
#define END_AJUSTE_CAP2_ANTENA_7                        26
#define END_AJUSTE_CAP0_ANTENA_8                        27
#define END_AJUSTE_CAP1_ANTENA_8                        28
#define END_AJUSTE_CAP2_ANTENA_8                        29
#define END_FREQUENCIA                                  30
#define END_ID_DO_LEITOR_0                              31 // 0xxx
#define END_ID_DO_LEITOR_1                              32 // x0xx
#define END_ID_DO_LEITOR_2                              33 // xx0x
#define END_ID_DO_LEITOR_3                              34 // xxx0
#define END_IP_REMOTO_PRINCIPAL_0                       35 // 000.xxx.xxx.xxx
#define END_IP_REMOTO_PRINCIPAL_1                       36 // xxx.000.xxx.xxx
#define END_IP_REMOTO_PRINCIPAL_2                       37 // xxx.xxx.000.xxx
#define END_IP_REMOTO_PRINCIPAL_3                       38 // xxx.xxx.xxx.000
#define END_IP_REMOTO_SECUNDARIO_0                      39 // 000.xxx.xxx.xxx
#define END_IP_REMOTO_SECUNDARIO_1                      40 // xxx.000.xxx.xxx
#define END_IP_REMOTO_SECUNDARIO_2                      41 // xxx.xxx.000.xxx
#define END_IP_REMOTO_SECUNDARIO_3                      42 // xxx.xxx.xxx.000
#define END_PORTA_REMOTA_PRINCIPAL_0                    43 // MSB
#define END_PORTA_REMOTA_PRINCIPAL_1                    44 // LSB
#define END_PORTA_REMOTA_SECUNDARIA_0                   45 // MSB
#define END_PORTA_REMOTA_SECUNDARIA_1                   46 // LSB
#define END_SSID_WIFI_0                                 47 // 0xxxxxxxxxxxxxxx
#define END_SSID_WIFI_1                                 48 // x0xxxxxxxxxxxxxx
#define END_SSID_WIFI_2                                 49 // xx0xxxxxxxxxxxxx
#define END_SSID_WIFI_3                                 50 // xxx0xxxxxxxxxxxx
#define END_SSID_WIFI_4                                 51 // xxxx0xxxxxxxxxxx
#define END_SSID_WIFI_5                                 52 // xxxxx0xxxxxxxxxx
#define END_SSID_WIFI_6                                 53 // xxxxxx0xxxxxxxxx
#define END_SSID_WIFI_7                                 54 // xxxxxxx0xxxxxxxx
#define END_SSID_WIFI_8                                 55 // xxxxxxxx0xxxxxxx
#define END_SSID_WIFI_9                                 56 // xxxxxxxxx0xxxxxx
#define END_SSID_WIFI_10                                57 // xxxxxxxxxx0xxxxx
#define END_SSID_WIFI_11                                58 // xxxxxxxxxxx0xxxx
#define END_SSID_WIFI_12                                59 // xxxxxxxxxxxx0xxx
#define END_SSID_WIFI_13                                60 // xxxxxxxxxxxxx0xx
#define END_SSID_WIFI_14                                61 // xxxxxxxxxxxxxx0x
#define END_SSID_WIFI_15                                62 // xxxxxxxxxxxxxxx0
#define END_SENHA_WIFI_0                                63 // 0xxxxxxxxxxxxxxx
#define END_SENHA_WIFI_1                                64 // x0xxxxxxxxxxxxxx
#define END_SENHA_WIFI_2                                65 // xx0xxxxxxxxxxxxx
#define END_SENHA_WIFI_3                                66 // xxx0xxxxxxxxxxxx
#define END_SENHA_WIFI_4                                67 // xxxx0xxxxxxxxxxx
#define END_SENHA_WIFI_5                                68 // xxxxx0xxxxxxxxxx
#define END_SENHA_WIFI_6                                69 // xxxxxx0xxxxxxxxx
#define END_SENHA_WIFI_7                                70 // xxxxxxx0xxxxxxxx
#define END_SENHA_WIFI_8                                71 // xxxxxxxx0xxxxxxx
#define END_SENHA_WIFI_9                                72 // xxxxxxxxx0xxxxxx
#define END_SENHA_WIFI_10                               73 // xxxxxxxxxx0xxxxx
#define END_SENHA_WIFI_11                               74 // xxxxxxxxxxx0xxxx
#define END_SENHA_WIFI_12                               75 // xxxxxxxxxxxx0xxx
#define END_SENHA_WIFI_13                               76 // xxxxxxxxxxxxx0xx
#define END_SENHA_WIFI_14                               77 // xxxxxxxxxxxxxx0x
#define END_SENHA_WIFI_15                               78 // xxxxxxxxxxxxxxx0
#define END_SENSIBILIDADE_DA_ANTENA_1                   79 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_2                   80 // 00
#define END_SENSIBILIDADE_DA_ANTENA_3                   81 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_4                   82 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_5                   83 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_6                   84 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_7                   85 // 00 
#define END_SENSIBILIDADE_DA_ANTENA_8                   86 // 00
#define END_INTENSIDADE_REFLEXAO_ANTENA_1_MSB           87 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_1_LSB           88 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_2_MSB           89 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_2_LSB           90 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_3_MSB           91 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_3_LSB           92 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_4_MSB           93 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_4_LSB           94 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_5_MSB           95 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_5_LSB           96 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_6_MSB           97 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_6_LSB           98 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_7_MSB           99 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_7_LSB          100 // xx00
#define END_INTENSIDADE_REFLEXAO_ANTENA_8_MSB          101 // 00xx
#define END_INTENSIDADE_REFLEXAO_ANTENA_8_LSB          102 // xx00


enum ComandosInterfaceAjustes{
    CMD_MODO_DE_OPERACAO = 1,                
    CMD_ATRASO_PARA_DEGRAD_LEITURA,
    CMD_REPETICAO_NA_LEITURA,
    CMD_SENSIBILIDADE_DA_ANTENA,
    CMD_TEMPO_INATIV_TAG_MONIT,
    CMD_NUMERO_DE_ANTENAS,
    CMD_AJUSTE_CAP_ANTENA_1,
    CMD_AJUSTE_CAP_ANTENA_2,
    CMD_AJUSTE_CAP_ANTENA_3,
    CMD_AJUSTE_CAP_ANTENA_4,
    CMD_AJUSTE_CAP_ANTENA_5,
    CMD_AJUSTE_CAP_ANTENA_6,
    CMD_AJUSTE_CAP_ANTENA_7,
    CMD_AJUSTE_CAP_ANTENA_8,
    CMD_FREQUENCIA_OPERACAO,
    CMD_ID_DO_LEITOR,
    CMD_ATUALIZACAO_DATA_HORA,
    CMD_IP_REMOTO_PRINCIPAL,
    CMD_IP_REMOTO_SECUNDARIO,
    CMD_PORTA_REMOTA_PRINCIPAL,
    CMD_PORTA_REMOTA_SECUNDARIA,
    CMD_SSID_WIFI,
    CMD_SENHA_WIFI,
    CMD_OBTER_FIRMWARE,
    CMD_OBTER_PARAMETROS,
    CMD_CALIBRACAO_AUTOMATICA,
    CMD_SENSIBILIDADE_ANTENA_1,
    CMD_SENSIBILIDADE_ANTENA_2,
    CMD_SENSIBILIDADE_ANTENA_3,
    CMD_SENSIBILIDADE_ANTENA_4,
    CMD_SENSIBILIDADE_ANTENA_5,
    CMD_SENSIBILIDADE_ANTENA_6,
    CMD_SENSIBILIDADE_ANTENA_7,
    CMD_SENSIBILIDADE_ANTENA_8,
    CMD_HABILITA_DEBUG_WIFI,
    CMD_DESABILITA_DEBUG_WIFI,
    CMD_ENVIA_MSG_INTERFACE_WIFI,              // #CMD37
    CMD_HABILITA_DEBUG_WIFI_SILENT,            // #CMD38
    CMD_DESABILITA_DEBUG_WIFI_SILENT,          // #CMD39
    CMD_HABILITA_LOG_CONECTIVIDADE_WIFI,       // #CMD40
    CMD_DESABILITA_LOG_CONECTIVIDADE_WIFI,     // #CMD41
    CMD_HABILITA_DEBUG_ETHERNET,               // #CMD42
    CMD_DESABILITA_DEBUG_ETHERNET,             // #CMD43
    CMD_ENVIA_MSG_INTERFACE_ETHERNET,          // #CMD44
    CMD_HABILITA_DEBUG_ETHERNET_SILENT,        // #CMD45
    CMD_DESABILITA_DEBUG_ETHERNET_SILENT,      // #CMD46
};

/*
typedef struct // Cria uma STRUCT para armazenar os dados de endereço de memoria
{
    char ModoDeOperacao;
} Memoria; // Define o nome do novo tipo criado
*/

extern unsigned int debugInterfaceEthernet;
extern unsigned int debugInterfaceEthernet_Silent;

extern char bufferDebugUSB[TAMANHO_BUFFER_COMANDOS_USB];
extern char bufferRxUSB[TAMANHO_BUFFER_COMANDOS_USB];

extern unsigned char modoDeOperacao;
extern unsigned char atrasoParaDegradarLeitura;
extern unsigned char repeticaoNaLeitura;
extern unsigned char sensibilidade;
extern unsigned char tempoParaInatividadeDaTagMonitorada;
extern unsigned char numeroDeAntenasAtivas;
extern unsigned char capAntena1[3];
extern unsigned char capAntena2[3];
extern unsigned char capAntena3[3];
extern unsigned char capAntena4[3];
extern unsigned char capAntena5[3];
extern unsigned char capAntena6[3];
extern unsigned char capAntena7[3];
extern unsigned char capAntena8[3];
extern unsigned int  reflexaoAntena1;
extern unsigned int  reflexaoAntena2;
extern unsigned int  reflexaoAntena3;
extern unsigned int  reflexaoAntena4;
extern unsigned int  reflexaoAntena5;
extern unsigned int  reflexaoAntena6;
extern unsigned int  reflexaoAntena7;
extern unsigned int  reflexaoAntena8;
extern unsigned char frequenciaDeOperacao;
extern unsigned char idDoLeitor[4];
extern unsigned char rssiMinAntena1;
extern unsigned char rssiMinAntena2;
extern unsigned char rssiMinAntena3;
extern unsigned char rssiMinAntena4;
extern unsigned char rssiMinAntena5;
extern unsigned char rssiMinAntena6;
extern unsigned char rssiMinAntena7;
extern unsigned char rssiMinAntena8;

//extern unsigned char ipRemotoPrincipal[15];
//extern unsigned char ipRemotoSecundario[15];
//extern unsigned int portaRemotaPrincipal;
//extern unsigned int portaRemotaSecundaria;

extern char ipRemotoPrincipal[16];
extern char ipRemotoSecundario[16];
extern int  portaRemotaPrincipal;
extern int  portaRemotaSecundaria;

extern unsigned char ssidWifi[17];
extern unsigned char senhaWifi[17];



void commandHandlerPortaUSB(void);
void limpaBufferRxUSB(void);

void realizarCalibracaoAutomatica(void);

//Seta parametros do leitor
void setaModoDeOperacao(void);
void setaAtrasoParaDegradarLeitura(void);
void setaRepeticaoNaLeitura(void);
void setaSensibilidadeDaAntena(void);
void setaTempoParaInatividadeDaTagMonitorada(void);
void setaNumeroDeAntenasAtivas(void);
void setaAjusteDeCapacitoresAntena1(void);
void setaAjusteDeCapacitoresAntena2(void);
void setaAjusteDeCapacitoresAntena3(void);
void setaAjusteDeCapacitoresAntena4(void);
void setaAjusteDeCapacitoresAntena5(void);
void setaAjusteDeCapacitoresAntena6(void);
void setaAjusteDeCapacitoresAntena7(void);
void setaAjusteDeCapacitoresAntena8(void);
void setaFrequenciaDeOperacao(void);
void setaIdDoLeitor(void);
void setaDataHora(void);
void setaIpRemotoPrincipal(void);
void setaIpRemotoSecundario(void);
void setaPortaRemotaPrincipal(void);
void setaPortaRemotaSecundaria(void);
void setaSsidWifi(void);
void setaSenhaWifi(void);
void setarssiMinAntena1(void);
void setarssiMinAntena2(void);
void setaSensibilidadeAntena3(void);
void setaSensibilidadeAntena4(void);
void setaSensibilidadeAntena5(void);
void setaSensibilidadeAntena6(void);
void setaSensibilidadeAntena7(void);
void setaSensibilidadeAntena8(void);


//Obtem parametros do leitor
void obtemModoDeOperacao(void);
void obtemAtrasoParaDegradarLeitura(void);
void obtemRepeticaoNaLeitura(void);
void obtemSensibilidadeDaAntena(void);
void obtemTempoParaInatividadeDaTagMonitorada(void);
void obtemNumeroDeAntenasAtivas(void);
void obtemAjusteDeCapacitoresAntena1(void);
void obtemAjusteDeCapacitoresAntena2(void);
void obtemAjusteDeCapacitoresAntena3(void);
void obtemAjusteDeCapacitoresAntena4(void);
void obtemAjusteDeCapacitoresAntena5(void);
void obtemAjusteDeCapacitoresAntena6(void);
void obtemAjusteDeCapacitoresAntena7(void);
void obtemAjusteDeCapacitoresAntena8(void);
void obtemFrequenciaDeOperacao(void);
void obtemIdDoLeitor(void);
void obtemDataHora(void);
void obtemIpRemotoPrincipal(void);
void obtemIpRemotoSecundario(void);
void obtemPortaRemotaPrincipal(void);
void obtemPortaRemotaSecundaria(void);
void obtemSsidWifi(void);
void obtemSenhaWifi(void);
void obtemFimware(void);
void obtemSensibilidadeDaAntena1(void);
void obtemSensibilidadeDaAntena2(void);
void obtemSensibilidadeDaAntena3(void);
void obtemSensibilidadeDaAntena4(void);
void obtemSensibilidadeDaAntena5(void);
void obtemSensibilidadeDaAntena6(void);
void obtemSensibilidadeDaAntena7(void);
void obtemSensibilidadeDaAntena8(void);

void retornaOk(void);
void retornaNok(void);
void enviaRespostaAosComandosDeSetupUSB(char dados[TAMANHO_BUFFER_COMANDOS_USB], int tamanho);
void gravaAjusteDeCapacitoresNaMemoria(unsigned char endereco);
void gravaIpNaMemoria(unsigned char endereco);
void limpaBufferRxUSB(void);
char escreveIpNaMemoria(unsigned int endereco, unsigned char dado0, unsigned char dado1, unsigned char dado2);
void obtemAjusteDeCapacitores(unsigned int endereco);
void obtemIpRemoto(unsigned char endereco);
void obtemParametrosDaMemoria(void);
void exibirParametrosObtidos(void);

void habilitaDebugInterfaceEthernetViaUSB(void);
void desabilitaDebugInterfaceEthernetViaUSB(void);
void habilitaDebugInterfaceEthernetSilentViaUSB(void);
void desabilitaDebugInterfaceEthernetSilentViaUSB(void);
void enviaComandoParaInterfaceEthernet(void);
void enviaComandoParaInterfaceEthernet(void);

#endif	/* SETUP_USB_H */

