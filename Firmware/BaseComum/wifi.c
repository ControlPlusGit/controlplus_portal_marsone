#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C:\Projetos\control-plus\Firmware\BaseComum\wifi.h"
#include "C:\projetos\control-plus\Firmware\Sankyu\080116\Sankyu\AS3993FwSource\AS3993\Firmware\src\timer.h"
#include "C:\projetos\control-plus\Firmware\Sankyu\080116\Sankyu\AS3993FwSource\AS3993\Firmware\src\uart_driver.h"
#include "exclusao.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\portal.h"

#include "C:\Projetos\control-plus\Firmware\BaseComum\zigbee.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\rtc.h"
#include "perifericos.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\i2c.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\mem_i2c_24LC256.h"
#include "C:\projetos\control-plus\Firmware\Sankyu\080116\Sankyu\AS3993FwSource\AS3993\Firmware\src\empilhadeira.h"
#include "C:\projetos\control-plus\Firmware\Sankyu\080116\Sankyu\AS3993FwSource\AS3993\Firmware\src\global.h"
#include "platform.h"




//Constantes
const char TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[QUANTIA_MAXIMA_DE_IPS_CADASTRADOS][TAMANHO_MAXIMO_STRING_IP] = {
    /*
    "192.168.1.101",
    "192.168.1.102",
    "192.168.1.103",
    "192.168.1.104",
    "192.168.1.105",
    "192.168.1.106",
    "192.168.1.107",
    "192.168.1.108",
    "192.168.1.109",
    "192.168.1.110",
    "192.168.1.111",
     * */
    //"192.168.1.106",
    //"192.168.1.150",
    //"192.168.1.151",
    //"192.168.1.152",
    //"192.168.1.153",
    // bn  bn "192.168.1.201",
    //"192.168.1.200" //Sars
            
    "10.20.140.10",
    "10.20.140.11",
    "10.20.140.12",
    "10.20.140.13",
    "10.20.140.14",
    "10.20.140.15",
    
    "10.20.140.16",
    "10.20.140.17",
    "10.20.140.18",
    "10.20.140.19",
    "10.20.140.20",
    
    "10.20.140.21",
    "10.20.140.22",
    "10.20.140.23",
    "10.20.140.24",
    "10.20.140.25",
    "10.20.140.26",
    "10.20.140.27",
     
    //"10.20.140.200"
    "10.159.158.10"
};

const char COMANDO_PARA_INICIAR_WIFI[50] = {
    //"AT+CIPSTART=\"UDP\",\"192.168.1.101\",9000,9000\r\n"
    //"AT+CIPSTART=\"UDP\",\"192.168.1.101\",9000,9000\r\n"
    //"AT+CIPSTART=\"UDP\",\"10.20.140.12\",9000,9000\r\n"
    "AT+CIPSTART=\"UDP\",\"10.159.158.1\",8000,8000\r\n"
};


const char ERROR_DE_LINK[20] = "link is not valid";

typedef struct {
	const int LIMITE_DA_RESERVA;
    char IpRemente[TAMANHO_MAXIMO_STRING_IP];
    int PortaRemetente;
    int TamanhoDoPacote;
	char Reserva[TAMANHO_DA_RESERVA];
} pacoteTcp;


char MeuIpWifi[TAMANHO_DA_STRING_DE_IP];


//Variaveis

//pacoteTcp PacoteRecebidoWifi = {TAMANHO_DA_RESERVA}; 
pacoteTcp PacoteRecebidoWifi; 

int volatile TimeOutWifiPortal;
char BufferWifiPortais[TAMANHO_BUFFER_WIFI_PORTAIS];
int ContadorBufferWifiPortais;
char __attribute__((far)) ReservaDeEnvioDaUartDeWifi[TAMANHO_DA_RESERVA];
//char ReservaDeEnvioDaUartDeWifi[TAMANHO_DA_RESERVA];
int ContadorDaReservaDeEnvioDaUsarDeWifi;
int QuantiaDeDadosParaEnviarPorWifiPorInterrupcao;

//Prototipos
void enviaFluxoDeDadosParaUartWifi(char *Dados, int Tamanho);
char retornaComUmByteDaResersaDeDadosDaUartDeTx (void);
void setaOModoDeInterrupcaoParaAUartDeWifi(void);
void enviaFluxoDadosPorInterrupcaoDeTX(char *Fluxo, int Tamanho);
void enviaFluxoDeDadosPorWifi(char *Ip, int Porta, char *Dados, int Tamanho);
void enviaFluxoDeDadosPorPolling(char *Fluxo, int Tamanho);
void TrataTabela (void);
int pegaOIpDoWifi(void);
int confirmacaoDoComandoCIPSEND (void);
int ErroDeRecepcaoDoModulo (void);
int FalhaDeRecepcaoDoModulo (void);


//Rotinas


void formataOModuloWifi(void){
    //enviaPacoteParaOEsp8266("AT+RESTORE\r\n");
    //enviaFluxoDeDadosPorPolling("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
    delay_ms(10000);
    //enviaPacoteParaOEsp8266("AT+CWMODE_DEF=1\r\n");
    enviaFluxoDeDadosPorPolling("AT+CWMODE_DEF=1\r\n", strlen("AT+CWMODE_DEF=1\r\n"));
    delay_ms(1000);
    enviaFluxoDeDadosPorPolling("AT+CIPSTA_DEF=\"10.20.140.20\",\"10.20.140.1\",\"255.255.254.0\"\r\n", strlen("AT+CIPSTA_DEF=\"10.20.140.20\",\"10.20.140.1\",\"255.255.254.0\"\r\n"));
    delay_ms(1000);
    //enviaPacoteParaOEsp8266("AT+CWDHCP_DEF=1,1\r\n");
    //enviaPacoteParaOEsp8266("AT+CIPMODE=0\r\n");
    enviaFluxoDeDadosPorPolling("AT+CIPMODE=0\r\n", strlen("AT+CIPMODE=0\r\n"));
    delay_ms(1000);
    //enviaPacoteParaOEsp8266("AT+CIPMUX=0\r\n");
    enviaFluxoDeDadosPorPolling("AT+CIPMUX=0\r\n", strlen("AT+CIPMUX=0\r\n"));
    delay_ms(1000);
    enviaFluxoDeDadosPorPolling("AT+CWDHCP_DEF=1,1\r\n", strlen("AT+CWDHCP_DEF=1,1\r\n"));
    delay_ms(1000);
    
    
    //#Considerar o adicao desse comando
    //#enviaPacoteParaOEsp8266('AT+CIPDINFO=1\r\n')
    //#Considerar a adicao desses comandos
    //#AT+CIPSTA_DEF="192.168.6.100","192.168.6.1","255.255.255.0"
    //enviaPacoteParaOEsp8266("AT+CWJAP_DEF=\"controlup1\",\"controlup\"");
    //enviaFluxoDeDadosPorPolling("AT+CWJAP_DEF=\"controlup1\",\"controlup\"\r\n", strlen("AT+CWJAP_DEF=\"controlup1\",\"controlup\"\r\n"));
    delay_ms(2000);
    
}

void iniciaSoqueteUDPPortais(void){
    int Tamanho;
    
    //leOSSIDESenhaDoWifi();
    
    //enviaFluxoDeDadosParaUartWifi((char *)COMANDO_PARA_INICIAR_WIFI, strlen(COMANDO_PARA_INICIAR_WIFI));
    Tamanho = strlen(COMANDO_PARA_INICIAR_WIFI);
    //enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_INICIAR_WIFI, Tamanho);
    enviaFluxoDeDadosPorPolling((char *)COMANDO_PARA_INICIAR_WIFI, Tamanho);
    //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
    delay_ms(1000);
}

void iniciaWifiDosPortais(void){
    memset(MeuIpWifi, 0, TAMANHO_DA_STRING_DE_IP);
    const char COMANDO_PARA_IPD_FORNECER_O_IP[16] = "AT+CIPDINFO=1\r\n";
    const char COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA[20] = "AT+RFPOWER=82\r\n";
    
    TimeOutWifiPortal = 0;
    (void)memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
    ContadorBufferWifiPortais = 0;
    iniciaSoqueteUDPPortais();
    //enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_IPD_FORNECER_O_IP, strlen(COMANDO_PARA_IPD_FORNECER_O_IP));
    enviaFluxoDeDadosPorPolling((char *)COMANDO_PARA_IPD_FORNECER_O_IP, strlen(COMANDO_PARA_IPD_FORNECER_O_IP));
    //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);

    enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA, strlen(COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA));
    while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
    
    pegaOIpDoWifi();
    setaOModoDeInterrupcaoParaAUartDeWifi();
    iniciaOTempoParaConferirAConexaoDeWifi();
}

int ModuloWifiPresente = -1;

void confereSeTemUmModuloPresente(void){
    char *Ponteiro;
    if (ModuloWifiPresente == -1){
        enviaFluxoDeDadosParaUartWifi("AT\r\n", strlen("AT\r\n"));
        delay_ms(20);
        while (TimeOutWifiPortal < TIMEOUT_WIFI_PORTAIS);
        ModuloWifiPresente = -1;
        Ponteiro = strstr(BufferWifiPortais, "OK");
        if (Ponteiro != NULL){
            ModuloWifiPresente = 0;
        }
    }
}

void iniciaWifiDasEmpilhadeiras(void){
    memset(MeuIpWifi, 0, TAMANHO_DA_STRING_DE_IP);
    const char COMANDO_PARA_IPD_FORNECER_O_IP[16] = "AT+CIPDINFO=1\r\n";
    const char COMANDO_PARA_PERMITIR_VARIOS_SOQUETES[16] = "AT+CIPMUX=0\r\n";
    const char COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA[20] = "AT+RFPOWER=82\r\n";
    TimeOutWifiPortal = 0;
    (void)memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
    ContadorBufferWifiPortais = 0;
    iniciaSoqueteUDPPortais();
    enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_IPD_FORNECER_O_IP, strlen(COMANDO_PARA_IPD_FORNECER_O_IP));
    while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
    delay_ms(10);
    
    enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_PERMITIR_VARIOS_SOQUETES, strlen(COMANDO_PARA_PERMITIR_VARIOS_SOQUETES));
    while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
    delay_ms(10);
    
    enviaFluxoDadosPorInterrupcaoDeTX((char *)COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA, strlen(COMANDO_PARA_QUE_O_MODULO_OPERE_COM_POTENCIA_MAXIMA));
    while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
    delay_ms(10);
    
    pegaOIpDoWifi();
    
    setaOModoDeInterrupcaoParaAUartDeWifi();
    iniciaOTempoParaConferirAConexaoDeWifi();
}

int pegaOIpDoWifi(void){
    char Comando[50];
    char *Ponteiro;
    char *PonteiroFimDoIp;
    int Indice;
    char *tok;
    char *delims = "\n";
    
    /*
    delay_ms(10);
    sprintf(Comando, "\r\n\r\n");
    enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));   
    delay_ms(10);
    */
    
    (void)memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
    ContadorBufferWifiPortais = 0;
    
    sprintf(Comando, "\r\nAT+CIFSR\r\n");
    enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
    delay_ms(20);
    while (TimeOutWifiPortal < TIMEOUT_WIFI_PORTAIS);
    //memset(MeuIpWifi, 0, TAMANHO_DA_STRING_DE_IP);
    sprintf(MeuIpWifi, "0.0.0.0");

    tok = strtok(BufferWifiPortais, delims);
    while (tok != NULL) {
        Ponteiro = strstr(tok, ":STAIP,\"");
        Indice = strlen(":STAIP,\"");
        Ponteiro = Ponteiro + Indice;
        PonteiroFimDoIp = strchr(Ponteiro, '"');
        if (PonteiroFimDoIp != NULL){
            *PonteiroFimDoIp = 0;
            if (Ponteiro != NULL){
                sprintf(MeuIpWifi, "%s", Ponteiro);
                return 0;
            }
        }
        tok = strtok(NULL, delims);
    }
    
    return -1;
    
    /*
    sprintf(Comando, "AT+CIFSR\r\n");
    enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
    delay_ms(20);
    while (TimeOutWifiPortal < TIMEOUT_WIFI_PORTAIS);
    //memset(MeuIpWifi, 0, TAMANHO_DA_STRING_DE_IP);
    sprintf(MeuIpWifi, "0.0.0.0");
    Ponteiro = strstr(BufferWifiPortais, ":STAIP,\"");
    Indice = strlen(":STAIP,\"");
    Ponteiro = Ponteiro + Indice;
    PonteiroFimDoIp = strchr(Ponteiro, '"');
    if (PonteiroFimDoIp == NULL)return -1;
    *PonteiroFimDoIp = 0;
    if (Ponteiro != NULL){
        sprintf(MeuIpWifi, "%s", Ponteiro);
        return 0;
    }
    return -1;
    */
}

void trabalharNaBuscaDoMeuIpWifi(void){
    if (MeuIpWifi[0] == '0'){
        pegaOIpDoWifi();
    }
}

void trataLedDeWifi (void){
    static int Contador;
    
    //Apagado = sem modulo
    //Piscando meio segundo = procurando rede
    //Acesso = Com rede
    //Piscada breve = envio de dados
    
                     
    if (ModuloWifiPresente == -1){
        desliga_led_zig();
    }
    
    if (ModuloWifiPresente == 0){
        if (MeuIpWifi[0] == '0'){
            if (Contador > 0){
                liga_led_zig();
            }
            if (Contador > 500){
                desliga_led_zig();
            }
            Contador = Contador + 1;
            if (Contador >= 1000)Contador = 0;
        } else {
            liga_led_zig();
        }
    }
}

void enviaFluxoDeDadosParaEmpilhadeira(char *Ip, char *Dado, int Tamanho){
    /*
    char Comando[50];

    sprintf(Comando, "AT+CIPSEND=%d,\"%s\",%d\r\n", Tamanho, Ip, PORTA_EMPILHADEIRAS);
    enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
    delay_ms(20);
    enviaFluxoDeDadosParaUartWifi(Dado, Tamanho);
    delay_ms(100);
    resetTimeOutWifi();
    */
    enviaFluxoDeDadosPorWifi(Ip,PORTA_EMPILHADEIRAS, Dado, Tamanho);
}

void enviaFluxoDeDadosParaPortal(char *Ip, char *Dado, int Tamanho){
    /*
    char Comando[50];
    
    sprintf(Comando, "AT+CIPSEND=%d,\"%s\",%d\r\n", Tamanho, Ip, PORTA_PORTAIS);
    enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
    delay_ms(20);
    enviaFluxoDeDadosParaUartWifi(Dado, Tamanho);
    delay_ms(100);
    resetTimeOutWifi();
    */
    enviaFluxoDeDadosPorWifi(Ip,PORTA_PORTAIS, Dado, Tamanho);
}

void enviaFluxoDeDadosParaSars(char *Dado, int Tamanho){
    char Comando[50];
    char Ip[TAMANHO_MAXIMO_STRING_IP];
    int TamanhoDoIp;
    
    
    TamanhoDoIp = sprintf(Ip, "%s", TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[QUANTIA_MAXIMA_DE_IPS_CADASTRADOS - 1]);
    if (TamanhoDoIp > 7){
        /*
        sprintf(Comando, "AT+CIPSEND=%d,\"%s\",%d\r\n", Tamanho, Ip, PORTA_SARS);
        enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
        delay_ms(20);
        enviaFluxoDeDadosParaUartWifi(Dado, Tamanho);
        delay_ms(100);
        resetTimeOutWifi();
         * */
        enviaFluxoDeDadosPorWifi(Ip,PORTA_SARS, Comando, strlen(Comando));
    }
}

void enviaFluxoDeDadosPorWifi(char *Ip, int Porta, char *Dados, int Tamanho){
    char Comando[50];
    int TamanhoDoIp;
    int Segura;
    //int Resultado;
    
    TamanhoDoIp = strlen(Ip);
    if (TamanhoDoIp > 7){
        memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
TENTA_NOVAMENTE:        
        sprintf(Comando, "AT+CIPSEND=%d,\"%s\",%d\r\n", Tamanho, Ip, Porta);
        enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
        (void)estouroNoTempoDeEnvio(1000);
        //while ((confirmacaoDoComandoCIPSEND() == -1) &&  (estouroNoTempoDeEnvio(-1) == -1));
        Segura = 0;
        while (Segura == 0){
            if (confirmacaoDoComandoCIPSEND() == 0)Segura = Segura + 1;
            if (estouroNoTempoDeEnvio(-1) == 0) Segura = Segura + 1;
            if (FalhaDeRecepcaoDoModulo() == 0)goto TENTA_NOVAMENTE;
            if (ErroDeRecepcaoDoModulo() == 0)goto TENTA_NOVAMENTE;
        };
        
        //enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
        //enviaFluxoDadosPorInterrupcaoDeTX(Comando, strlen(Comando));
        
        //enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
        
        //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
        //delay_ms(5);
        //delay_ms(20);
        //enviaFluxoDeDadosParaUartWifi(Dados, Tamanho);
        //enviaFluxoDadosPorInterrupcaoDeTX(Dados, Tamanho);
        //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
        enviaFluxoDeDadosPorPolling(Dados, Tamanho);
        
        //Esperar o "SEND OK" ou definir um time out
        //delay_ms(5);
        //resetTimeOutWifi();
        
        (void)estouroNoTempoDeEnvio(1000);
        while ((confirmacaoDeRecepcaoDoModulo() == -1) 
                &&  (estouroNoTempoDeEnvio(-1) == -1) 
                && (FalhaDeRecepcaoDoModulo() == -1) 
                && (ErroDeRecepcaoDoModulo() == -1)
              );
    }
}
 

char *procuraStringNaMemoria(char *BufferWifiPortais, char *String, int Tamanho){
    char *Posicao;
    char *PosicaoInicial;
    int Contador;
    Contador = 0;
    int TamanhoDaEntrada;
    PosicaoInicial = BufferWifiPortais;
    
    while (Contador < Tamanho){
        Posicao = strstr(BufferWifiPortais, String);
        if (Posicao == NULL){
            TamanhoDaEntrada = strlen(BufferWifiPortais);
            Contador = Contador + TamanhoDaEntrada;
            BufferWifiPortais = BufferWifiPortais + TamanhoDaEntrada;
            if (TamanhoDaEntrada == 0){
                Contador = Contador + 1;
                BufferWifiPortais = BufferWifiPortais + 1;
            }
        } else {
            return Posicao;
        }
        
    }
    return NULL;
}

int confirmacaoDeRecepcaoDoModulo (void){
    char *Ponteiro;
    //Ponteiro  = strstr(BufferWifiPortais, "SEND OK");
    Ponteiro  = procuraStringNaMemoria(BufferWifiPortais, "SEND OK", TAMANHO_BUFFER_WIFI_PORTAIS);
    if (Ponteiro != NULL){
        //memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        memset(Ponteiro, 0, strlen("SEND OK"));
        return 0;
    }
    return -1;
}

int FalhaDeRecepcaoDoModulo (void){
    char *Ponteiro;
    //Ponteiro  = strstr(BufferWifiPortais, "SEND FAIL");
    Ponteiro  = procuraStringNaMemoria(BufferWifiPortais, "SEND FAIL", TAMANHO_BUFFER_WIFI_PORTAIS);
    if (Ponteiro != NULL){
        //memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        memset(Ponteiro, 0, strlen("SEND FAIL"));
        return 0;
    }
    return -1;
}

int ErroDeRecepcaoDoModulo (void){
    char *Ponteiro;
    //Ponteiro  = strstr(BufferWifiPortais, "ERROR");
    Ponteiro  = procuraStringNaMemoria(BufferWifiPortais, "ERROR", TAMANHO_BUFFER_WIFI_PORTAIS);
    if (Ponteiro != NULL){
        //memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        memset(Ponteiro, 0, strlen("ERROR"));
        return 0;
    }
    return -1;
}


int confirmacaoDoComandoCIPSEND (void){
    char *Ponteiro;
    //Ponteiro  = strstr(BufferWifiPortais, "\r\nOK\r\n>");
    Ponteiro  = procuraStringNaMemoria(BufferWifiPortais, "\r\nOK\r\n>", TAMANHO_BUFFER_WIFI_PORTAIS);
    if (Ponteiro != NULL){
        //memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        memset(Ponteiro, 0, strlen("\r\nOK\r\n>"));
        return 0;
    }
    return -1;
}



int estouroNoTempoDeEnvio(int Tempo){
    static int ContadorDeTempoDeEnvio;
    if (Tempo > 0)ContadorDeTempoDeEnvio = Tempo;
    if (ContadorDeTempoDeEnvio > 0)ContadorDeTempoDeEnvio = ContadorDeTempoDeEnvio - 1;
    if (ContadorDeTempoDeEnvio <= 0)return 0;
    //else return -1;
    return -1;
}

void disparaOEnvioDeFLuxoDeDadosPorWifi(char *Ip, int Porta, int Tamanho){
    char Comando[50];
    //int Enviado;
    
    memset(Comando, 0, 50);
    while(sprintf(Comando, "AT+CIPSEND=%d,\"%s\",%d\r\n", Tamanho, Ip, Porta) < 0);
    //enviaFluxoDeDadosParaUartWifi(Comando, strlen(Comando));
    //enviaFluxoDadosPorInterrupcaoDeTX(Comando, strlen(Comando));
    //enviaFluxoDadosPorInterrupcaoDeTX(Comando, strlen(Comando));
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
}

void enviaOFluxoDeDadosEmSoqueteAberto(char *Dados, int Tamanho){
    enviaFluxoDadosPorInterrupcaoDeTX(Dados, Tamanho);
}

void mudaOSSIDESenha (void){
    //Esses parametros vem do portal.c 
    char Comando[100];
    //enviaPacoteParaOEsp8266('AT+CWJAP_DEF="controlup1","controlup"')
    sprintf(Comando, "AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", SsidWifi, SenhaWifi);
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
    delay_ms(1000);
}

void inibeAOperecaoDoWifi(void){
    enviaFluxoDeDadosPorPolling("AT+CWJAP_DEF="",""\r\n", strlen("AT+CWJAP_DEF="",""\r\n"));
    delay_ms(200);
    enviaFluxoDeDadosPorPolling("AT+RST\r\n", strlen("AT+RST\r\n"));
    delay_ms(200);
    
    mudaOSSIDESenha();
}


void enviaFluxoDeDadosParaUartWifi(char *Dados, int Tamanho){
    int Contador;
    char Dado;
    
    for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        //uart3Tx(*Dados);//Porta USB
        //delay_ms(10);
        //uart2Tx(*Dados);//Porta ETH
        //delay_ms(10);
        //uart1Tx(*Dados);//ZigBee Beta
        //delay_ms(10);
        Dado = *Dados;
        uart4Tx(Dado);//ZigBee Alfa
        Dados = Dados + 1;
    }
    //resetTimeOutWifi();
}

void resetTimeOutWifi(void){
    TimeOutWifiPortal = 0;
}

int confereSeOLinkNaoEhValido(void){
    if (strstr(BufferWifiPortais, ERROR_DE_LINK) == 0){
        return -1;
    }
    return 0;
}
void recebeDadosDeWifiPortal(char Dado){
    desliga_led_zig();
	resetTimeOutWifi();
	BufferWifiPortais[ContadorBufferWifiPortais] = Dado;
	ContadorBufferWifiPortais = ContadorBufferWifiPortais + 1;
    
    if (ContadorBufferWifiPortais >= TAMANHO_BUFFER_WIFI_PORTAIS){
        ContadorBufferWifiPortais = 0;
    }
}

void coletaOMACDoModulo (char *Saida){
    char Comando[100];
    char *Ponteiro;
    char *PonteiroFinal;
    memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
    (void)sprintf(Comando, "AT+CIPSTAMAC?\r\n");
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
    while (TimeOutWifiPortal < TIMEOUT_WIFI_PORTAIS);
    //delay_ms(20);
    //delay_ms(5);
	//Ponteiro = strstr(BufferWifiPortais, "+CIPSTAMAC:\"");
    Ponteiro = procuraStringNaMemoria(BufferWifiPortais, "+CIPSTAMAC:\"", TAMANHO_BUFFER_WIFI_PORTAIS);
    if (Ponteiro != NULL){
        Ponteiro = Ponteiro + strlen("+CIPSTAMAC:\"");
        PonteiroFinal = strchr(Ponteiro, '"');
        if (PonteiroFinal != NULL){
            *PonteiroFinal = 0;
            sprintf(Saida, "%s", Ponteiro);
        } else {
            sprintf(Saida, "y.y.y.y");
        }
    } else {
        sprintf(Saida, "x.x.x.x");
    }
    
}

void setaOMACDOModulo (char *Mac){
    char Comando[50];
    (void)sprintf(Comando, "AT+CIPSTAMAC_DEF=%s\r\n", Mac);
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
}

void limpaBufferWifiDosPortais(void){
    memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
    ContadorBufferWifiPortais = 0;
}

int estouroDoTimeOutDoBufferDoWifiDosPortais(char *BufferDeSaida){
    int Rascunho;
	if (TimeOutWifiPortal >= TIMEOUT_WIFI_PORTAIS){
        Rascunho = ContadorBufferWifiPortais;
		if (ContadorBufferWifiPortais > 0){
			(void)memmove(BufferDeSaida, BufferWifiPortais, ContadorBufferWifiPortais);
            LimpaEResetaARecepcaoDeDadosDoWifi();
		}
        return Rascunho;
	} else {
        return -1;
    }
	return -1;
}


int recebeOFluxoDeTrabalhoDoWifi(char *BufferDeSaida, int LimiteDoBuffer){
    int QuantiaDeDadosColetados;
    char *Posicao, *PosicaoInicial;
    int Contador;
    char Dado;
    
    QuantiaDeDadosColetados = estouroDoTimeOutDoBufferDoWifiDosPortais(BufferDeSaida);
    if (QuantiaDeDadosColetados > 0){
        Posicao = strstr(BufferDeSaida, "+IPD,");
        PosicaoInicial = Posicao;
        if (Posicao != NULL){
            for(Contador = (&BufferDeSaida[0] - Posicao);Contador < QuantiaDeDadosColetados;Contador = Contador + 1){
                Dado = *Posicao;
                Posicao = Posicao + 1;
                if (Dado == ':'){
                    Contador = Posicao - PosicaoInicial;
                    Contador = QuantiaDeDadosColetados - Contador;
                    //memset(Resultado, 0, 100);
                    memcpy(BufferDeSaida, Posicao, Contador);
                    memset((&BufferDeSaida[0] + Contador), 0, (LimiteDoBuffer - Contador));
                    __asm("NOP");
                    return 0;
                    break;
                }
            }
        }
        //(void)memset(BufferDeSaida, 0, 100);
        return -1;
    } 
    if (QuantiaDeDadosColetados == 0)return -1;
    return -2;
}

void trocaVirgulaPorEspaco(char *Texto){
	while(*Texto != 0){
		if (*Texto == ',')*Texto = ' ';
		Texto = Texto + 1;
        if (*Texto == ':')break;
	}
}

int recebeOFluxoDeTrabalhoDoWifiComIp(pacoteTcp *Pacote){
    int Resultado;
    char *Posicao;
    char *PosicaoDoIpd;
    int Parcial;
    Resultado = 0;
    
    //if (ContadorBufferWifiPortais > 0){
    //if (TimeOutWifiPortal >= 20){
        
        //LimpaEResetaARecepcaoDeDadosDoWifi();
        //return -2;
        
        
        //PosicaoDoIpd = strstr(BufferWifiPortais, "+IPD");
        PosicaoDoIpd = procuraStringNaMemoria(BufferWifiPortais, "+IPD", TAMANHO_BUFFER_WIFI_PORTAIS);
        if (PosicaoDoIpd != NULL){
            Posicao = strchr(PosicaoDoIpd, ':');
            if (Posicao != NULL){
                Posicao = Posicao + 1;
                trocaVirgulaPorEspaco(PosicaoDoIpd);
                //Resultado = sscanf(BufferWifiPortais, "\r\n+IPD %d %s %d:%s", &Pacote->TamanhoDoPacote, Pacote->IpRemente, &Pacote->PortaRemetente, Pacote->Reserva);
                Resultado = sscanf(PosicaoDoIpd, "\r\n+IPD %d %s %d:", &Pacote->TamanhoDoPacote, Pacote->IpRemente, &Pacote->PortaRemetente);
                Parcial = Posicao - BufferWifiPortais;
                Parcial = ContadorBufferWifiPortais - Parcial;
                Parcial = Pacote->TamanhoDoPacote;
                //memcpy(Pacote->Reserva, Posicao, ((Posicao - BufferWifiPortais) + ContadorBufferWifiPortais));
                memcpy(Pacote->Reserva, Posicao, Parcial);
                if (ContadorBufferWifiPortais >= TAMANHO_DA_RESERVA){
                    LimpaEResetaARecepcaoDeDadosDoWifi();
                    return -1;
                }
                //memcpy(Pacote->Reserva, Posicao, Parcial);
                if (Resultado == 3){
                    LimpaEResetaARecepcaoDeDadosDoWifi();
                    return 0;
                } else {
                    __asm("NOP");
                    LimpaEResetaARecepcaoDeDadosDoWifi();
                    return -2;
                }
            }
        }
        //LimpaEResetaARecepcaoDeDadosDoWifi();
    //} 
    if (ContadorBufferWifiPortais == 0)return -1;
    return -2;
}

int foiRecebidaEssaMensagemPeloWifi(char *Mensagem){
    int Resultado;
#ifdef PORTAL
    
    Resultado = recebeOFluxoDeTrabalhoDoWifiComIp(&PacoteRecebidoWifi);
    
    /*
    if (Resultado == 0){
        return 0;
    }
    */
    
    
            
    if (strstr(PacoteRecebidoWifi.Reserva, Mensagem) != NULL)return 0;
    
    if (Resultado == -1){
        memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        ContadorBufferWifiPortais = 0;
        return -1;
    }

    return Resultado;
    
    if (ContadorBufferWifiPortais > 0){
        if (strstr(BufferWifiPortais, Mensagem) != NULL)return 0;
    } 
    return -2;
#else
    
    Resultado =  recebeOFluxoDeTrabalhoDoWifiComIp(&PacoteRecebidoWifi);
    if (Resultado == 0){
        Resultado = memcmp(PacoteRecebidoWifi.Reserva, Mensagem, strlen(Mensagem));
        return Resultado;
    }
#endif
    
    
	if (TimeOutWifiPortal >= TIMEOUT_WIFI_PORTAIS){
        memset(BufferWifiPortais, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        ContadorBufferWifiPortais = 0;
        return -1;
    }
    return -1;
}

void trataTimeOutWifiPortais(void){
	TimeOutWifiPortal = TimeOutWifiPortal + 1;
    if (TimeOutWifiPortal >= TIMEOUT_WIFI_PORTAIS){
        TimeOutWifiPortal = TIMEOUT_WIFI_PORTAIS;
    }
}

void exemploDeUso (void){
    //iniciaWifiDosPortais();
    
    enviaFluxoDeDadosPorWifi("192.168.1.101", 9000, "1234", strlen("1234"));
    
    while(1){
        //char Comando[100];
        
        //enviaATabelaDeExclusaoParaAsEmpilhadeirasPresentes();
        //operacaoDeCodigoCorrenteParaExclusao();
        
        
        /*
        if (recebeOFluxoDeTrabalhoDoWifi(Comando, 100) > -1){
            __asm("NOP");
            
        } else {
            memset(Comando, 0, 100);
            __asm("NOP");
        }
        */
    }
}


void setaOModoDeInterrupcaoParaAUartDeWifi(void){
    // 0,0 ou 0,1 ou 1,0 nunca 1,1
    U4STAbits.UTXISEL0 = 0;
    U4STAbits.UTXISEL1 = 1;
}

void ligaInterrupcaoDeTX(void){
    IEC5bits.U4TXIE = 1;
}

void desligaInterrupcaoDeTX(void){
    IEC5bits.U4TXIE = 0;
}

int pacoteSendoEnviadoPelaUartDeWifi(void){
    return IEC5bits.U4TXIE;
}

void limpaFlagUartWifi(void){
    IFS5bits.U4TXIF = 0;
}

char trabalhaNaInterrupcaoDeWifi(void){
    limpaFlagUartWifi();
    return retornaComUmByteDaResersaDeDadosDaUartDeTx();
}

char retornaComUmByteDaResersaDeDadosDaUartDeTx (void){
    char Saida;
    
    Saida = ReservaDeEnvioDaUartDeWifi[ContadorDaReservaDeEnvioDaUsarDeWifi];
    ContadorDaReservaDeEnvioDaUsarDeWifi = ContadorDaReservaDeEnvioDaUsarDeWifi + 1;
    if (ContadorDaReservaDeEnvioDaUsarDeWifi >= QuantiaDeDadosParaEnviarPorWifiPorInterrupcao){
        desligaInterrupcaoDeTX();
        memset(ReservaDeEnvioDaUartDeWifi, 0, TAMANHO_DA_RESERVA);
        ContadorDaReservaDeEnvioDaUsarDeWifi = 0;
        QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = 0;
    }
    return Saida;
}

void enviaFluxoDadosPorInterrupcaoDeTX(char *Fluxo, int Tamanho){
    int Contador;
    
    for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        ReservaDeEnvioDaUartDeWifi[Contador] = *Fluxo;
        if (*Fluxo < 0){
            __asm("NOP");
        }
        Fluxo = Fluxo + 1;
    }
    
    disparaOEnvioDaReservaDeDadosDoWifi(Tamanho);
}

void enviaFluxoDeDadosPorPolling(char *Fluxo, int Tamanho){
    int Contador;
    //while (pacoteSendoEnviadoPelaUartDeWifi());
    //desligaInterrupcaoDeTX();
    delay_ms(50);
    for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        //uart1Tx(*Fluxo);
        uart4Tx(*Fluxo);
        Fluxo = Fluxo + 1;
    }
}

void disparaOEnvioDaReservaDeDadosDoWifi(int Tamanho){
    QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = Tamanho;
    ligaInterrupcaoDeTX();
}

void resetaELimpaAReservaDeEnvioPorWifi(void){
    QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = 0;
    memset(ReservaDeEnvioDaUartDeWifi, 0, TAMANHO_DA_RESERVA);
}

int adicionaByteAReservaDeDadosDoWifi(char Dado){
    static int Contador = 0;
    int Ocupado;
    
    //Ocupado = pacoteSendoEnviadoPelaUartDeWifi();
    Ocupado = 0;
    //if (QuantiaDeDadosParaEnviarPorWifiPorInterrupcao == 0){
        if (QuantiaDeDadosParaEnviarPorWifiPorInterrupcao < TAMANHO_DA_RESERVA){
            ReservaDeEnvioDaUartDeWifi[QuantiaDeDadosParaEnviarPorWifiPorInterrupcao] = Dado;
            QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = QuantiaDeDadosParaEnviarPorWifiPorInterrupcao + 1;
            Contador = Contador + 1;
            return 0;
        }
    //} else {
        Contador = 0;
    //}
    return -1;
}

int adicionaStringAReservaDeDadosDoWifi(char *Fluxo){
    int Resultado;
    while(*Fluxo != 0){
        Resultado = adicionaByteAReservaDeDadosDoWifi(*Fluxo);
        Fluxo = Fluxo + 1;
        if (Resultado < 0)return -1;
    }
    return 0;
}

int adicionaWordAReservaDeDadosDoWifi(int Entrada){
    int Saida;
    int Rascunho;
    char Dado;
    
    Rascunho = Entrada >> 8;
    Dado = (char)Rascunho;
    Saida = adicionaByteAReservaDeDadosDoWifi(Dado);
    if (Saida < 0)return -1;
    Dado = (char)Entrada;
    Saida = adicionaByteAReservaDeDadosDoWifi(Dado);
    if (Saida < 0)return -1;
    return 0;
}

int retornaComOIndiceParaOProximoEquipamentoASeComunicar(void){
    static int Contador = -1;
    Contador = Contador + 1;
    if (Contador >= QUANTIA_MAXIMA_DE_IPS_CADASTRADOS)Contador = 0;
    return Contador;
}

void enviaComandoParaConferirAPresencaDoEquipamentoNaArea(int Indice){
    char Comando[12];
    sprintf(Comando, "Presente?");
    if (strlen(TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice]) > 0){
        enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice], 9000, Comando, strlen(Comando));
        resetTimeOutWifi();
    }
}

char Reposta[TAMANHO_BUFFER_WIFI_PORTAIS];

int enviaTabelaDeExclusaoParaEquipamentoNaArea(int Indice){
    //  Nao posso parar para enviar toda a tabela de uma vez, logo tenho que enviar
    //por cagas de dados.
    //sd enviaFluxoDeDadosPorWifi(char *Ip, int Porta, char *Dados, int Tamanho){
    __asm("NOP");
    
    enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice],
            9000, ReservaDeEnvioDaUartDeWifi, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
    
    //enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice],
    //        8000, ReservaDeEnvioDaUartDeWifi, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);


    return 1;//Pacote enviado
    //return 0;//Falta enviar dados
}

int delayParaEnvioNoWifi;


void enviaPerguntaDePresencaParaAsEmpilhadeirasNaArea(void){
    static int Indice = 0;
    /*
    disparaOEnvioDeFLuxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice], 9000, strlen("Presente"));
    //enviaFluxoDadosPorInterrupcaoDeTX(strlen("Presente"), strlen("Presente"));
    delay_ms(5);
    enviaFluxoDeDadosPorPolling("Presente", strlen("Presente"));
    */
    enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice], 9000, "Presente", strlen("Presente"));
outroIp:
    Indice = Indice + 1;
    if (Indice >= QUANTIA_MAXIMA_DE_IPS_CADASTRADOS){
        Indice = 0;
    }
    if (TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice][0] == 0)goto outroIp;
}

int enviaATabelaDeExclusaoParaAsEmpilhadeirasPresentes(int ModuloOcupado){
    //static int Contador = 0;
    static int Estado = 0;
    static int Indice = 0;
    //const char TabelaExTeste[2000];
    int PasseiAOcuparOModulo;
    //int Resultado;
    
    if (ModuloOcupado == -2){
        return ModuloOcupado;
    }
    
    const char Comando[10] = "Presente";

    switch(Estado){
        case 0:
            if (ModuloOcupado == -1){
                int TamanhoDoIp;
outroIP:
                TamanhoDoIp = strlen(TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice]);
                if (TamanhoDoIp > 7){
                    disparaOEnvioDeFLuxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice], 9000, strlen(Comando));
                    //enviaComandoParaConferirAPresencaDoEquipamentoNaArea(Contador);
                    Estado = 1;
                    PasseiAOcuparOModulo = 1;
                } else {
                    Indice = Indice + 1;
                    if (Indice > (QUANTIA_MAXIMA_DE_IPS_CADASTRADOS - 1))Indice = 0;
                    goto outroIP;
                    //Estado = 6;
                }
            }
            break;
            
        case 1:
            if (pacoteSendoEnviadoPelaUartDeWifi() == 1)break;
            else Estado = 2;
            break;
            
        case 2:
            enviaFluxoDadosPorInterrupcaoDeTX((char *)Comando, strlen(Comando));
            Estado = 3;
            break;
            
        case 3:
            if (pacoteSendoEnviadoPelaUartDeWifi() == 1)break;
            else {
                Estado = 4;
            }
        case 4:
            Estado = 7;
            break;
            
        case 5:
            atualizaTabelaDeExclusaoParaEnvioPorWifi();
            disparaOEnvioDeFLuxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice], 9000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
            Estado = 6;
            break;
            
        case 6:
            atualizaTabelaDeExclusaoParaEnvioPorWifi();
            enviaOFluxoDeDadosEmSoqueteAberto(ReservaDeEnvioDaUartDeWifi, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
            Estado = 7;
            break;
            
        case 7:
            Indice = Indice + 1;
            if (Indice > (QUANTIA_MAXIMA_DE_IPS_CADASTRADOS - 1)){
                Indice = 0;
            }
            Estado = 0;
            break;
            
        default:
            Estado = 0;
            break;
    }
    
    /*
    switch(Estado){
        case 0:
            //Envia comando
            if (ModuloOcupado > 0){
                enviaComandoParaConferirAPresencaDoEquipamentoNaArea(Contador);
                Estado = 1;
            }
            break;
        case 1:
            //Falha por timeout ou falhar ou processe com o envio da tabela
            Resultado = recebeOFluxoDeTrabalhoDoWifi(Reposta, TAMANHO_BUFFER_WIFI_PORTAIS);
            if (Resultado == 0){
                if (strcmp(Reposta, "Presente") == 0){
                    __asm("NOP");
                    Estado = 2;
                } else {
                    //Falha, reposta invalida
                    Estado = 3;
                }
            }
            if (Resultado == -1){
                //Estouro de tempo
                Estado = 3;
            }
            
            break;
            
        case 2:
            atualizaTabelaDeExclusaoParaEnvioPorWifi();
            Resultado = enviaTabelaDeExclusaoParaEquipamentoNaArea(Contador);
            if (Resultado > 0){
                Estado = 3;
            }
            break;
                    
        case 3:
            Contador = retornaComOIndiceParaOProximoEquipamentoASeComunicar();
            //Contador = 0;
            Estado = 0;
            PasseiAOcuparOModulo = -1;
            break;
            
        default:
            Estado = 0;
            break;
    }
    */
    
    PasseiAOcuparOModulo = -1;
    if (Estado > 0){
        PasseiAOcuparOModulo = 1; 
    }
    
    return PasseiAOcuparOModulo;
    
}

void operacoesDeTickParaWifi(void){
    trataTimeOutWifiPortais();
    if (delayParaEnvioNoWifi > 0){
        delayParaEnvioNoWifi = delayParaEnvioNoWifi - 1;
    }
#ifdef EMPILHADEIRA
    trataLedDeWifi();
#endif
#ifdef PODE_SER_ALPHA
    trataLedDeWifi();
#endif    
    
    (void)estouroNoTempoDeEnvio(-1);
    decrementaOTempoParaConferirAConexaoWifi();
}

void LimpaEResetaARecepcaoDeDadosDoWifi (void){
    if (TimeOutWifiPortal == TIMEOUT_WIFI_PORTAIS){
        limpaBufferWifiDosPortais();
        resetTimeOutWifi();
    }
}

void LimpaOPacoteRecebidoPorWifi(void){
    memset(PacoteRecebidoWifi.IpRemente, 0, TAMANHO_MAXIMO_STRING_IP);
    PacoteRecebidoWifi.PortaRemetente = 0;
    PacoteRecebidoWifi.TamanhoDoPacote = 0;
    memset(PacoteRecebidoWifi.Reserva, 0, TAMANHO_DA_RESERVA);
}

int ProcuraCharEmArray(char *Memoria, char Letra, int PosicaoAtual, int Limite){
	int Contador;
	Contador = PosicaoAtual;
	Memoria = Memoria + PosicaoAtual;
	for (;Contador < Limite;Contador = Contador + 1){
		if (*Memoria == Letra){
			//printf("\n%d\n", Contador);
            return Contador;
		}
		Memoria = Memoria + 1;
	}
	return -1;
}

void trataRecepcaoDeDadosDeEmpilhadeiraPorWifi(void){
    //char *Ponteiro;
    int Resultado;
	int Indice;
	
    Resultado = recebeOFluxoDeTrabalhoDoWifiComIp(&PacoteRecebidoWifi);
    if (Resultado == 0){
    	Indice = 0;	
    	Indice = ProcuraCharEmArray(PacoteRecebidoWifi.Reserva, 'P', Indice, TAMANHO_DA_RESERVA);
    	while (Indice >= 0){
    		if (strstr(&PacoteRecebidoWifi.Reserva[Indice], "Presente")!= NULL) {
                resetaELimpaAReservaDeEnvioPorWifi();
                atualizaTabelaDeExclusaoParaEnvioPorWifi();
                disparaOEnvioDeFLuxoDeDadosPorWifi(PacoteRecebidoWifi.IpRemente, 9000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                delay_ms(5);
                //ContadorDaReservaDeEnvioDaUsarDeWifi = 0;
                //disparaOEnvioDaReservaDeDadosDoWifi(QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
                enviaFluxoDeDadosPorPolling(ReservaDeEnvioDaUartDeWifi,QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                //LimpaOPacoteRecebidoPorWifi();
			}
			Indice = Indice + 1;
    		Indice = ProcuraCharEmArray(PacoteRecebidoWifi.Reserva, 'P', Indice, TAMANHO_DA_RESERVA);
		} 
        LimpaOPacoteRecebidoPorWifi();
    } 
	

    
    
    /*
    Resultado = recebeOFluxoDeTrabalhoDoWifiComIp(&PacoteRecebidoWifi);
    if (Resultado == 0){
        //Ponteiro = strstr(PacoteRecebidoWifi.Reserva, "Presente");
        //Ponteiro = strstr(PacoteRecebidoWifi.Reserva, "Presente");
        Ponteiro = memchr(PacoteRecebidoWifi.Reserva, 'P', TAMANHO_DA_RESERVA);
        while(Ponteiro != NULL){
            Ponteiro = strstr(Ponteiro, "Presente");
            if (Ponteiro != NULL){
            //if (foiRecebidaEssaMensagemPeloWifi("Presente") == 0){
                resetaELimpaAReservaDeEnvioPorWifi();
                atualizaTabelaDeExclusaoParaEnvioPorWifi();
                disparaOEnvioDeFLuxoDeDadosPorWifi(PacoteRecebidoWifi.IpRemente, 9000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                delay_ms(5);
                //ContadorDaReservaDeEnvioDaUsarDeWifi = 0;
                //disparaOEnvioDaReservaDeDadosDoWifi(QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                //while(pacoteSendoEnviadoPelaUartDeWifi() == 1);
                enviaFluxoDeDadosPorPolling(ReservaDeEnvioDaUartDeWifi,QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                LimpaOPacoteRecebidoPorWifi();
            } else {
                //LimpaOPacoteRecebidoPorWifi();
            }
            Ponteiro = memchr(Ponteiro, 'P', (Ponteiro - PacoteRecebidoWifi.Reserva + 2));
        }
        LimpaOPacoteRecebidoPorWifi();
    } 
    */
}

int retornaComUmaStringDeEventoParaSerEnviadaParaOSars(char *Saida){
    static unsigned int Endereco = 100;
    int Limite;
    char *Dado;
    
    Dado = Saida;
    Limite = eventosArmazenadosEmE2PROM();
    Limite = Limite * 100;
    mem_leDados_24LC256(Endereco,(unsigned char *) Saida, 100);
    Endereco = Endereco + 100;
    if (Endereco >= Limite)Endereco = 100;
    if ((unsigned char)*Dado != 0xFF)return 0;
    return -1;
}

void trataRecepcaoDeDadosParaAEmpilhadeiraPorWifi(void){
    int Contador;
    extern int ResultadoProtocolo;
    extern struct EstruturaProtocolo Teste;
    
    if (recebeOFluxoDeTrabalhoDoWifiComIp(&PacoteRecebidoWifi) == 0){
        if (strcmp(PacoteRecebidoWifi.Reserva, "Presente") == 0){
            QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = strlen("Presente");
            disparaOEnvioDeFLuxoDeDadosPorWifi(PacoteRecebidoWifi.IpRemente, 9000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
            delay_ms(5);
            ContadorDaReservaDeEnvioDaUsarDeWifi = 0;
            resetaELimpaAReservaDeEnvioPorWifi();
            enviaFluxoDeDadosPorPolling("Presente",strlen("Presente"));
        }
        
        if (strcmp(PacoteRecebidoWifi.Reserva, "EnviarEvento") == 0){
            char Mensagem[100];
            if (retornaComUmaStringDeEventoParaSerEnviadaParaOSars(Mensagem) == 0){
                QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = strlen(Mensagem);
                disparaOEnvioDeFLuxoDeDadosPorWifi(PacoteRecebidoWifi.IpRemente, 6006, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
                delay_ms(5);
                ContadorDaReservaDeEnvioDaUsarDeWifi = 0;
                resetaELimpaAReservaDeEnvioPorWifi();
                QuantiaDeDadosParaEnviarPorWifiPorInterrupcao = strlen(Mensagem);
                enviaFluxoDeDadosPorPolling(Mensagem,QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
            }
        }
        
        if (PacoteRecebidoWifi.Reserva[0] == (char)0xF9){
            for(Contador = 0;Contador < PacoteRecebidoWifi.TamanhoDoPacote;Contador = Contador + 1){
                ResultadoProtocolo = trataRecepcaoNoProcolo (PacoteRecebidoWifi.Reserva[Contador], &Teste.Estado, &Teste.Comando, &Teste.Tamanho, NULL/*&Corpo[Teste.ContagemDoCorpo](*/, &Teste.ContagemDoCorpo, &Teste.CheckSum);
                if (ResultadoProtocolo == 1){
                    TrataTabela();
                }
            }
            __asm("NOP");
        }
        LimpaOPacoteRecebidoPorWifi();
        LimpaEResetaARecepcaoDeDadosDoWifi();
    } 
}


void TrataTabela (void){
    int Resultado;
    int Contador;
    int Indice;
    
    extern struct EstruturaProtocolo Teste;
    extern unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];
    
    union {
        unsigned char bytes[4];
        unsigned long Dado;
    } Conversao;
    
    unsigned char Rascunho[7];//5//3
    
    if (1){

        /*
        if(Teste.Comando == 0xF9){
            if (Teste.Tamanho > TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){
                goto finaliza;
            }
            Indice = 0;
            for(Contador = 3;Contador < Teste.ContagemDoCorpo;Contador = Contador + 7){
                Rascunho[0] = CorpoDoComando[5 + Contador];
                Rascunho[1] = CorpoDoComando[4 + Contador];
                Rascunho[2] = CorpoDoComando[3 + Contador];
                Rascunho[3] = CorpoDoComando[2 + Contador];
                Rascunho[4] = CorpoDoComando[1 + Contador];
                
                Rascunho[5] = CorpoDoComando[0 + Contador];
                Rascunho[6] = CorpoDoComando[Contador - 1];
                
                liga_led_rede();
                if ((unsigned char)Rascunho[5] == 0){
                    (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], ENTROU_NA_AREA_SEGURA);
                } else {
                    (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], SAIU_NA_AREA_SEGURA);
                }
                    
                desliga_led_rede();
                Indice = Indice + 1;
                if (Indice >= QUANTIA_DE_ELEMENTOS){
                    Indice = 0;
                }
            }
            
            if(Teste.Comando == 0xFA){//Data e hora por epoch
                for (Contador = 0;Contador < 4;Contador = Contador + 1){
                    Conversao.bytes[Contador] = CorpoDoComando[3 - Contador];
                }
                SetaTempoComEpoch(Conversao.Dado);
            }
            goto finaliza;
        }
        */
        
        switch(Teste.Comando){
            case 0xF9:
                if (Teste.Tamanho > TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){
                    goto finaliza;
                }
                Indice = 0;
                for(Contador = 3;Contador < Teste.ContagemDoCorpo;Contador = Contador + 7){
                    Rascunho[0] = CorpoDoComando[5 + Contador];
                    Rascunho[1] = CorpoDoComando[4 + Contador];
                    Rascunho[2] = CorpoDoComando[3 + Contador];
                    Rascunho[3] = CorpoDoComando[2 + Contador];
                    Rascunho[4] = CorpoDoComando[1 + Contador];

                    Rascunho[5] = CorpoDoComando[0 + Contador];
                    Rascunho[6] = CorpoDoComando[Contador - 1];

                    LED_REDE = 1;
                    if ((unsigned char)Rascunho[5] == 0){
                        (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], ENTROU_NA_AREA_SEGURA);
                    } else {
                        (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], SAIU_NA_AREA_SEGURA);
                    }

                    LED_REDE = 0;
                    Indice = Indice + 1;
                    if (Indice >= QUANTIA_DE_ELEMENTOS){
                        Indice = 0;
                    }
                }

                if(Teste.Comando == 0xFA){//Data e hora por epoch
                    for (Contador = 0;Contador < 4;Contador = Contador + 1){
                        Conversao.bytes[Contador] = CorpoDoComando[3 - Contador];
                    }
                    SetaTempoComEpoch(Conversao.Dado);
                }
                goto finaliza;
                break;
            
            case 0xF0:
                (void)cadastrarOperador((unsigned char *)&PacoteRecebidoWifi.Reserva[5]);
                break;
                
            case 0xF1://Monitor envia um operador para a maquina
                for(Contador = 0;Contador < TAMANHO_EPC_PARA_EMPILHADEIRA;Contador = Contador + 1){
                    MeuOperador.Epc[Contador] = PacoteRecebidoWifi.Reserva[Contador];
                }
                MeuOperador.TempoDeValidade = TEMPO_DE_VALIDADE_DE_OPERADOR;
                //enviaMensagemAoMonitorPorZigBee("Operador obtido", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Operador obtido\n");
                break;
                
            case 0xF2:
                CintoVirtual = PacoteRecebidoWifi.Reserva[0];
                if (CintoVirtual != 0){
                    //enviaMensagemAoMonitorPorZigBee("Cinto virtual colocado", COMANDO_DE_LOG_ZIG_BEE);
                    enviaTextoDeLogDaOperacao("Cinto virtual colocado");
                } else {
                    //enviaMensagemAoMonitorPorZigBee("Cinto virtual retirado", COMANDO_DE_LOG_ZIG_BEE);
                    enviaTextoDeLogDaOperacao("Cinto virtual retirado\n");
                }
                break;
                
            case 0xF3:
                //enviaMensagemAoMonitorPorZigBee("Pedestre virtual adicionado", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Pedestre virtual adicionado\n");
                (void)adicionarPedestre((unsigned char *)PacoteRecebidoWifi.Reserva, PacoteRecebidoWifi.Reserva[3], PacoteRecebidoWifi.Reserva[4], 0);//Cuidado com o parametro Indice, vou que acertar isso no futuro
                break;

            case 0xF4:
                //enviaMensagemAoMonitorPorZigBee("Pedestre virtual adicionado", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Pedestre virtual adicionado\n");
                DirecaoVirtual = PacoteRecebidoWifi.Reserva[0];
                break;
                
            case 0xF5:
                //adicionaItemNaTabelaDeInvisiveis(Corpo);
                //(void)adicionaElementoDaTabelaDeExclusao (PacoteRecebidoWifi.Reserva, ENTROU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)PacoteRecebidoWifi.Reserva, PacoteRecebidoWifi.Reserva[3], PacoteRecebidoWifi.Reserva[4], ENTROU_NA_AREA_SEGURA);
                //(void)cadastrarPedestreAListaDoPortalDeExclusao(Corpo);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre passou a ser invisiveil", COMANDO_DE_LOG_ZIG_BEE);
                //enviaTextoDeLogDaOperacao("Um pedestre passou a ser invisiveil\n");
                break;

            case 0xF6:
                //(void)removePedestreDaListaDoPortalDeExclusaoPorEpc(Corpo);
                //(void)adicionaElementoDaTabelaDeExclusao (PacoteRecebidoWifi.Reserva, SAIU_NA_AREA_SEGURA);
                //(void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], SAIU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)PacoteRecebidoWifi.Reserva, PacoteRecebidoWifi.Reserva[3], PacoteRecebidoWifi.Reserva[4], SAIU_NA_AREA_SEGURA);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre deixou de ser invisivel", COMANDO_DE_LOG_ZIG_BEE);
                //enviaTextoDeLogDaOperacao("Um pedestre deixou de ser invisivel\n");
                break;
              
            case 0xF7:
                //as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, Corpo[0]);
                break;

            case 0xF8:
                mem_escreveDados_24LC256(ADDR_NOME_NA_REDE_ZB, (unsigned char *)PacoteRecebidoWifi.Reserva, TAMANHO_DO_NOME_NA_REDE_ZB);
                //mem_leDados_24LC256(ADDR_NOME_NA_REDE_ZB, &Nome[0], TAMANHO_DO_NOME_NA_REDE_ZB);
                break;
                
                
            case 0xFE:
                break;
            
            case 0xFF:
                //FF-06-FF-FF-FF-03
                //
                if ((PacoteRecebidoWifi.Reserva[0] == 'A') && (PacoteRecebidoWifi.Reserva[1] == 'A') && (PacoteRecebidoWifi.Reserva[2] == 'A')){
                    //enviaTextoDeLogDaOperacao("BroadCast recebido\n");
                    //enviaTextoDeLogDaOperacao(NomeDaEmpilhadeira);
                    //enviaTextoDeLogDaOperacao(num_serie);
                    //extern char num_serie[20];
                    //enviaIdPorZigBee(num_serie);
                } else { 
                    enviaTextoDeLogDaOperacao("Falha ao obter broadcast\n");
                }
                break;
            default:
                //enviaMensagemAoMonitorPorZigBee("Comando nao reconhecido\n", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Comando nao reconhecido\n");
                break;
        }

        goto finaliza;
    }

    if (Resultado == -1){
finaliza:        
        memset(CorpoDoComando, 0, TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE);//10
        iniciaEstruturaDoProtocolo();
    }
}

#define TEMPO_PARA_CONFERIR_O_ESTADO_DA_CONEXAO_DO_WIFI 20000
int ContadorParaConferirSeOWifiEstaConectado = 0; //Em mili segundos 

void iniciaOTempoParaConferirAConexaoDeWifi (void){
    ContadorParaConferirSeOWifiEstaConectado  = TEMPO_PARA_CONFERIR_O_ESTADO_DA_CONEXAO_DO_WIFI;
}

void decrementaOTempoParaConferirAConexaoWifi (void){
    if (ContadorParaConferirSeOWifiEstaConectado > 0)
        ContadorParaConferirSeOWifiEstaConectado = ContadorParaConferirSeOWifiEstaConectado - 1;
}

void confereAConexaoDoWifi (void){
    if (ContadorParaConferirSeOWifiEstaConectado == 0){
        if (pegaOIpDoWifi() == -1){
            iniciaSoqueteUDPPortais();
        }
        iniciaOTempoParaConferirAConexaoDeWifi();
    }
}