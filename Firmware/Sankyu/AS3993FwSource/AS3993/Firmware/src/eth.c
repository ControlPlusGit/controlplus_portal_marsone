#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eth.h"
#include "uart_driver.h"
#include <timer.h>
#include <time.h>
#include "wifi.h"
#include <ctype.h>
#include <uart.h>
#include "global.h"
#include "perifericos.h"
#include "empilhadeira.h"
#include "platform.h"
#include "ajuste_cap.h"
#include "setup_usb.h"
#include "cancelas.h"

const char DESTINOS_PARA_OS_EVENTOS[NUMERO_DE_PORTAIS_ALPHA][TAMANHO_DA_STRING_DE_IP] = {
    //"192.168.1.101",
    //"192.168.1.102"
    //"10.20.140.9",
    //"10.20.140.11"
    "10.244.23.9",
    "10.244.23.11",
            
};

char DestinoDosEventosSelecionados[TAMANHO_DA_STRING_DE_IP];

int PossoSerOPortalAlpha;

const char TABELA_COM_OS_IPS_DOS_PORTAIS[QUANTIA_MAXIMA_DE_PORTAIS_EM_UMA_AREA][TAMANHO_MAXIMO_STRING_IP] = {
    
    "10.244.23.5",
    "10.244.23.6",
    "10.244.23.7",
    "10.244.23.8",
    "10.244.23.9",
    "10.244.23.10",
    "10.244.23.11",
    "10.244.23.12",
    "10.244.23.13",
    "10.244.23.14",
    "10.244.23.15",
    "10.244.23.16",
    "10.159.158.10"
    
};

int TimeOutEthPortal;
char ReservaEthPortais[TAMANHO_RESERVA_ETH_PORTAIS];
int ContadorReservaEthPortais;
long ContadorDeTempoDeVidaDoPortal;
int ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais;

int timeoutRespostaKeepAlive;
char bufferRxEthernet[TAMANHO_BUFFER_RX_ETHERNET];

unsigned char statusDeConexaoTCP = NOT_CONNECTED;
unsigned char comandoAtivo = NENHUM;
char statusDeOperacaoEth = MODO_REDE_TCP;

//char ipRemotoPrincipal[15] = {"192.168.1.110"};
//char ipRemotoSecundario[15] = {"192.168.1.102"};
char ipRemotoPrincipal[16] = {"104.154.47.107\0"};
char ipRemotoSecundario[16] = {"104.154.47.107\0"};
int portaRemotaPrincipal = 8000;
int portaRemotaSecundaria = 8001;

char destinoDosEventos;
char timerParaTrocaDeRemoteIP = 0;
char alarmeFaltaDeRedeEthernet;

char __attribute__((near)) MeuIpEth[TAMANHO_DA_STRING_DE_IP];

extern struct tm *dataHora;

void decrementaContadorDeParaEnvioDoMeuIpParaSerOAlphaDosOutrosPortais(void);
void coletaOIpDoModuloUSRTCP232T24(void);
void impedeDeSerAlpha(void);

void iniciaOperacoesComPortalComEth (void){
    ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
        TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS;
    ContadorDeTempoDeVidaDoPortal = TEMPO_DE_VIDA_DO_PORTAL_ALPHA;
    
    //(void)sprintf(DestinoDosEventosSelecionados, "192.168.1.101");
    (void)sprintf(DestinoDosEventosSelecionados, "10.244.23.9");
    
    coletaOIpDoModuloUSRTCP232T24();
    impedeDeSerAlpha();
    //coletaMacDoMosuloUSRTCP232T24();
    
}

int digits_only(const char *s){
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}

int RetornaComNumeroDeString(char *Texto){
	char *Ponteiro;
	char Rascunho[4];
	int Numero;
	
	Ponteiro = strchr(Texto, '.');
	Numero = Ponteiro - Texto;
	if (Numero > 3)return -1;
	if (Numero < 0)return -1;
	memset(Rascunho, 0, 4);
	memcpy(Rascunho, Texto, Numero);
	if (digits_only(Rascunho)==0)return -1;
	Numero = atoi(Texto);
	if (Numero > 255)return -1;
	return Numero;
}

int ipValido(char *Texto){
	char *Ponteiro;
	int Numero;
	
	Numero = RetornaComNumeroDeString(Texto);
	if (Numero < 0)return -1;
	printf("Numero obtido = %d\n", Numero);
	Ponteiro = strchr(Texto, '.');
	if (Ponteiro == NULL)return -1;
	Ponteiro = Ponteiro + 1;
	
	Numero = RetornaComNumeroDeString(Ponteiro);
	if (Numero < 0)return -1;
	printf("Numero obtido = %d\n", Numero);
	Ponteiro = strchr(Ponteiro, '.');
	if (Ponteiro == NULL)return -1;
	Ponteiro = Ponteiro + 1;
	
	Numero = RetornaComNumeroDeString(Ponteiro);
	if (Numero < 0)return -1;
	printf("Numero obtido = %d\n", Numero);
	Ponteiro = strchr(Ponteiro, '.');
	Ponteiro = Ponteiro + 1;
	
	if (digits_only(Ponteiro)==0)return -1;
	Numero = atoi(Ponteiro);
	printf("Numero obtido = %d\n", Numero);

	return 0;
}

int disponivelParaSerOAlpha(void){
    //if (PossoSerOPortalAlpha < 0) PossoSerOPortalAlpha = TIMEOUT_PARA_SER_ALFA;
    //return PossoSerOPortalAlpha;
    if (ContadorDeTempoDeVidaDoPortal > 0)return 1;
    return 0;
}

void impedeDeSerAlpha(void){
    //PossoSerOPortalAlpha = TIMEOUT_PARA_SER_ALFA;
    ContadorDeTempoDeVidaDoPortal = TEMPO_DE_VIDA_DO_PORTAL_ALPHA;
}

void resetNaReservaDeDadosDaEth(void){
    //if (ContadorReservaEthPortais > 0){
        memset(ReservaEthPortais, 0, TAMANHO_RESERVA_ETH_PORTAIS);
        ContadorReservaEthPortais = 0;
    //}
}

int estouroDoTimeOutDoBufferDoEthDosPortais(char *BufferDeSaida){
    int Rascunho;
	if (TimeOutEthPortal >= TIMEOUT_ETH_PORTAIS){
		if (ContadorReservaEthPortais > 0){
			(void)memcpy(BufferDeSaida, ReservaEthPortais, ContadorReservaEthPortais);
            Rascunho = ContadorReservaEthPortais;
            resetNaReservaDeDadosDaEth();            
			return Rascunho;
		}
	}
	return -1;
}


void enviaDadosParaEthPortais(char dados[TAMANHO_BUFFER_RX_ETHERNET], int Tamanho){
    int posicao;
    if(!debugInterfaceEthernet){
        for (posicao = 0; posicao < Tamanho; posicao = posicao + 1){
            uart2Tx(dados[posicao]);//Porta ETH
        }
    }
    if(debugInterfaceEthernet_Silent){
        for (posicao = 0; posicao < Tamanho; posicao = posicao + 1){
            uart3Tx(dados[posicao]);//Porta USB
        }
    }
}

void enviaDadoParaEthPortais(char Dado){
    if(!debugInterfaceEthernet){
        uart2Tx(Dado);//Porta ETH
    }
}

void enviarComandoParaOModulo(char *Comando){
    char Rascunho[50];
    int Tamanho;
    memset(Rascunho, 0, 50);
    Tamanho = sprintf(Rascunho, "%s", Comando);
    enviaDadosParaEthPortais(Rascunho, Tamanho);
}

void trataTimeOutEthPortais(void){
	TimeOutEthPortal = TimeOutEthPortal + 1;
}

int enviaStringEthPortais(char *Dados){
    int Tamanho;
    char Reserva[TAMANHO_RESERVA_ETH_PORTAIS];
    Tamanho = strlen(Dados);
    if (Tamanho > TAMANHO_RESERVA_ETH_PORTAIS) return -1;
    Tamanho = sprintf(Reserva, "%s", Dados);
    enviaDadosParaEthPortais(Reserva, Tamanho);
    return 0;
}

int Rastro[500];
int ContaRastro;

void recebeDadosDeEthPortal(char Dado){
	TimeOutEthPortal = 0;
	ReservaEthPortais[ContadorReservaEthPortais] = Dado;
	
    
    Rastro[ContaRastro] = ContadorReservaEthPortais;
    if (ContaRastro != ContadorReservaEthPortais){
       __asm("NOP");
    }
    
    ContaRastro = ContaRastro + 1;
    if (ContaRastro >= 500)ContaRastro = 0;
    
    ContadorReservaEthPortais = ContadorReservaEthPortais + 1;
}

void CarregaBufferRxEthernet(char dado, int posicao){
    bufferRxEthernet[posicao] = dado;
}


void exemploDeUsoDaEth (void){
    while(1){
        char Comando[TAMANHO_RESERVA_ETH_PORTAIS];
        int QuantiaDeDadosColetados;
        
        QuantiaDeDadosColetados = estouroDoTimeOutDoBufferDoEthDosPortais(Comando);
        if (QuantiaDeDadosColetados > 0){
            (void)memset(Comando, 0, TAMANHO_RESERVA_ETH_PORTAIS);
        }
    }
}


void entraEmModoAt(void){
    char comando[20];
    //comandoAtivo == ENTRA_MODO_AT;
    sprintf(comando, "+++");
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(100);
    sprintf(comando, "a");
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(100);
    //comandoAtivo == NENHUM;
}


void saiDoModoAt(void){
    char comando[10];
    //comandoAtivo = CMD_SAI_MODO_AT;
    sprintf(comando, "AT+ENTM\r");
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(200);
    //comandoAtivo = NENHUM;
}


void resetModuloETH(void){
    char comando[10];
    //comandoAtivo == CMD_RESET_ETHERNET;
    sprintf(comando, "AT+Z\r");
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(300);
    //comandoAtivo = NENHUM;
}


void coletaOIpDoModuloUSRTCP232T24(void){
    char *Posicao;
    char *PosicaoFinal;
    char Rascunho[100];
    
    memset(Rascunho, 0, 100);
    memset(MeuIpEth, 0, TAMANHO_DA_STRING_DE_IP);
TENTA_NOVAMENTE:
    entraEmModoAt();
    enviarComandoParaOModulo("AT+WANN\r\n");
	while (TimeOutEthPortal <= TIMEOUT_ETH_PORTAIS);
    Posicao = strstr(ReservaEthPortais, "+OK=STATIC,");
    if (Posicao != NULL){
        Posicao = Posicao + strlen("+OK=STATIC,");
        PosicaoFinal = strstr(Posicao, ",");
        *PosicaoFinal = 0;
        sprintf(MeuIpEth, "%s", Posicao);
        //sprintf(MeuIpEth, "%s", Posicao);
        //PosicaoFinal = PosicaoFinal + 1;
        //sprintf(MeuIpEth, "%s", PosicaoFinal);
        if (MeuIpEth == 0)goto TENTA_NOVAMENTE;
        __asm("NOP");
    } else {
        memset(ReservaEthPortais, 0, TAMANHO_RESERVA_ETH_PORTAIS);
        ContadorReservaEthPortais = 0;
        delay_ms(100);
        saiDoModoAt();
        delay_ms(100);
        goto TENTA_NOVAMENTE;
    }
        
    delay_ms(100);
    saiDoModoAt();
    delay_ms(100);
}

void coletaMacDoMosuloUSRTCP232T24(void){
    char *Posicao;
    char Rascunho[100];
    
    memset(ReservaEthPortais, 0, TAMANHO_RESERVA_ETH_PORTAIS);
    ContadorReservaEthPortais = 0;
    
    
    memset(Rascunho, 0, 100);
    memset(MeuIpEth, 0, TAMANHO_DA_STRING_DE_IP);
    

    entraEmModoAt();
    //enviarComandoParaOModulo("AT+MAC?\r\n");
    enviarComandoParaOModulo("AT+USERMAC=9CA5259613CE\r\n");
	while (TimeOutEthPortal <= TIMEOUT_ETH_PORTAIS);
    Posicao = strstr(ReservaEthPortais, "+OK=STATIC,");
    memset(ReservaEthPortais, 0, TAMANHO_RESERVA_ETH_PORTAIS);
    ContadorReservaEthPortais = 0;
        
    delay_ms(100);
    saiDoModoAt();
    delay_ms(100);
    
}


void mudarODestinatarioDasMensagens (char *Ip){
    char Comando[50];
    entraEmModoAt();
    // \n\r
    
    //if (strcmp(Ip, DestinoDosEventosSelecionados) != 0){
    if (strcmp(Ip, MeuIpEth) != 0){
        (void)sprintf(Comando, "AT+SOCK=UDPC,%s,%d\r\n", Ip, 8000);
        (void)sprintf(DestinoDosEventosSelecionados, "%s", Ip);
        enviarComandoParaOModulo(Comando);
        delay_ms(100);
        (void)sprintf(Comando, "AT+CFGTF\r\n");
        enviarComandoParaOModulo(Comando);
        delay_ms(100);
        saiDoModoAt();
        delay_ms(100);
    }
    //}
    saiDoModoAt();    
}

void realizaResetNoModulo (void){
    enviarComandoParaOModulo("AT+Z");
}

void operacoesEmTickParaEth(void){
    decrementaContadorDeTempoDeVidaDoPortalAlpha();
    decrementaContadorDeParaEnvioDoMeuIpParaSerOAlphaDosOutrosPortais();
    //trataSePossoTentarSerAlpha();
}

void decrementaContadorDeTempoDeVidaDoPortalAlpha(void){
    if (ContadorDeTempoDeVidaDoPortal > 0)
        ContadorDeTempoDeVidaDoPortal = ContadorDeTempoDeVidaDoPortal - 1;
}

void decrementaContadorDeParaEnvioDoMeuIpParaSerOAlphaDosOutrosPortais(void){
    if (ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais > 0){
        ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
            ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais - 1;
    }
}

void lidaComATrocaDePortalAlpha(void){
    //const char IP_DO_SARS[20] = "192.168.1.200";
    char IpColetado[20];
    
    extern char __attribute__((near)) MeuIpEth[TAMANHO_DA_STRING_DE_IP];
    /*
     Nao ha porque enviar o ID para mim mesmo.
     Nao porque abdicar a funcao de alfa, se nao ouvi uma ordem para isso
     
    */ 
    //static int Portal = 0;
    int Contador;
    char *Ponteiro;
    char *DelimitadorDoFimDoIp;
    
//#ifdef PODE_SER_ALPHA
    /*
    if (ContadorDeTempoDeVidaDoPortal == 0){
        if (strstr(DestinoDosEventosSelecionados, MeuIpEth) == 0){
            ContadorDeTempoDeVidaDoPortal = TEMPO_DE_VIDA_DO_PORTAL_ALPHA;
            mudarODestinatarioDasMensagens(IP_DO_SARS);
        }
    }
    */
//#else 
    //if (ipValido(ReservaEthPortais) == 0)impedeDeSerAlpha();
    for(Contador = 0;Contador < NUMERO_DE_PORTAIS_ALPHA;Contador = Contador + 1){
        Ponteiro = strstr(ReservaEthPortais, DESTINOS_PARA_OS_EVENTOS[Contador]);
        DelimitadorDoFimDoIp = Ponteiro + strlen(DESTINOS_PARA_OS_EVENTOS[Contador]);
        *DelimitadorDoFimDoIp = 0;
        sprintf(IpColetado, "%s", Ponteiro);
        if (Ponteiro != NULL){
            //ContadorDeTempoDeVidaDoPortal = TEMPO_DE_VIDA_DO_PORTAL_ALPHA;
            //if (strcmp(ReservaEthPortais, MeuIpEth) != 0){
            if (strcmp(IpColetado, MeuIpEth) != 0){
                impedeDeSerAlpha();
                memset(Ponteiro, 0, strlen(DESTINOS_PARA_OS_EVENTOS[Contador]));
                //if (strcmp(DestinoDosEventosSelecionados, ReservaEthPortais) != 0){
                if (strcmp(DestinoDosEventosSelecionados, IpColetado) != 0){
                    //(void)sprintf(DestinoDosEventosSelecionados, "%s", ReservaEthPortais);
                    (void)sprintf(DestinoDosEventosSelecionados, "%s", IpColetado);
                    mudarODestinatarioDasMensagens(DestinoDosEventosSelecionados);
                    ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
                        TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS;
                }
            }
        }
    }

    /*
    if (disponivelParaSerOAlpha() < 1){
        if (strcmp(DestinoDosEventosSelecionados, IP_DO_SARS) != 0){
            (void)sprintf(DestinoDosEventosSelecionados, "%s", IP_DO_SARS);
            mudarODestinatarioDasMensagens(DestinoDosEventosSelecionados);
        }
    }
    */
#ifndef PODE_SER_ALPHA
    resetNaReservaDeDadosDaEth();
#endif
}

void enviaParaTodosOsPortiasAPresencaDesseLeitorPorPolling(void){
    /*static int Indice = 0;
    static int Tamanho;
    
    Tamanho = strlen(MeuIpEth);
    if (ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais == 0){
        ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
            TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS;
        liga_led_zig();
outroIP:
        Indice = Indice + 1;
        if (Indice >= QUANTIA_MAXIMA_DE_PORTAIS_EM_UMA_AREA)Indice = 0;
        //if (strcmp(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], MeuIpEth) != NULL){
        if (strcmp(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], MeuIpEth) != NULL){
            if (strlen(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice]) > 0){
                
                disparaOEnvioDeFLuxoDeDadosPorWifi(
                    (char *)TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], 8000, Tamanho);
                delay_ms(5);
                enviaFluxoDeDadosPorPolling(MeuIpEth, Tamanho);
                
                //enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], 8000, MeuIpEth, Tamanho);
                enviaFluxoDeDadosPorWifi((char *)"10.244.23.11", 8000, MeuIpEth, Tamanho);
                
                if (confereSeOLinkNaoEhValido() < 0){
                    iniciaWifiDosPortais();
                }
                 
            } else {
                goto outroIP;
            }
        } else {
            goto outroIP;
        }
    }*/
}
    


int enviaParaTodosOsPortaisAPresencaDesseLeitor(int ModuloOcupado){
    /*static int Indice;
    static int Estado;
    static int Tamanho;
    int PasseiAOcuparOModulo;
    //char Resposta[TAMANHO_BUFFER_WIFI_PORTAIS];
    
    
    if (strstr(DestinoDosEventosSelecionados, MeuIpEth) == NULL){
        Estado = 0;
        return -2;
        //return -1;
    }
    
    
    switch(Estado){
        case 0://Envia comandos
            if (ModuloOcupado == -1){
                Tamanho = strlen(MeuIpEth);
                if (ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais == 0){
                    ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
                        TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS;
outroIP:
                    if (strcmp(DestinoDosEventosSelecionados, MeuIpEth) != NULL){
                        if (Indice < QUANTIA_MAXIMA_DE_PORTAIS_EM_UMA_AREA)Indice = Indice + 1;
                        else Indice = 0;
                        if (strlen(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice]) > 0){
                            if (strstr(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], MeuIpEth) == NULL){
                                disparaOEnvioDeFLuxoDeDadosPorWifi(
                                    (char *)TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], 8000, Tamanho);
                                Estado = 1;
                                PasseiAOcuparOModulo = 1;
                            }
                        } else {
                            goto outroIP;
                        }
                    }
                }
            }
            break;
        case 1://Envia Dados
            if (pacoteSendoEnviadoPelaUartDeWifi() == 1)break;
            else Estado = 2;
            break;
        case 2://Espera o envio
            enviaOFluxoDeDadosEmSoqueteAberto(MeuIpEth, Tamanho);
            Estado = 3;
            break;
        case 3://Espera a confirmacao de envio
            if (pacoteSendoEnviadoPelaUartDeWifi() == 1)break;
            else Estado = 0;
            PasseiAOcuparOModulo = -1;
            return -1;
            break;
        default:
            Estado = 0;
            PasseiAOcuparOModulo = -1;
            break;
    }
    
    PasseiAOcuparOModulo = -1;
    if (Estado > 0){
        PasseiAOcuparOModulo = 1; 
    }
    
    return PasseiAOcuparOModulo;
    
    
    
    Tamanho = strlen(MeuIpEth);
    if (ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais == 0){
        ContadorParaEnviarOMeuIpParaSerOAlphaDosOutrosPortais = 
                TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS;
TENTA_NOVAMENTE:        
        if (Indice < QUANTIA_MAXIMA_DE_PORTAIS_EM_UMA_AREA)Indice = Indice + 1;
        else Indice = 0;
        if (strlen(TABELA_COM_OS_IPS_DOS_PORTAIS[Indice]) > 0){
            enviaFluxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DOS_PORTAIS[Indice], 
                8000, MeuIpEth, Tamanho);
        }
        
        memset(Resposta, 0, TAMANHO_BUFFER_WIFI_PORTAIS);
        while(estouroDoTimeOutDoBufferDoWifiDosPortais(Resposta) < 0);
        if (strstr(Resposta, "\r\nSEND OK\r\n") == NULL){
            delay_ms(100);
            goto TENTA_NOVAMENTE;
        }
    }
    */
    return 0;
}

void acoesDePortalComEthParaCodigoCorrente(void){
//#ifdef PODE_SER_ALPHA    
    //lidaComATrocaDePortalAlpha();
//#endif
//#ifdef PODE_SER_ALPHA
    //enviaParaTodosOsPortaisAPresencaDesseLeitor();
//#endif
}

/*
void solicitaDataHoraPelaEthernet(void){
    char mensagem[100];
    if(comandoAtivo == NENHUM){
        sprintf(mensagem, "<FE;%c%c%c%c>", IdDOLeitor[0], IdDOLeitor[1], IdDOLeitor[2], IdDOLeitor[3]);
        enviaDadosParaEthPortais(mensagem, strlen(mensagem));
        //comandoAtivo == CMD_SOLICITA_DATA_HORA;    
    }
}
*/

void enviaKeepAliveParaEthernet(char status){
    char mensagem[100];
//    static int timer = 0;
    
    /*
    if(timer < 50){
        timer = timer + 1;
        return;
    }
    */
    //if(statusDeConexaoTCP == TENTANDO_CONECTAR || statusDeConexaoTCP == CONNECTED){
        //timer = 0;
        //sprintf(mensagem, "<FD;%c%c%c%c;%02d>\r\n", IdDOLeitor[0], IdDOLeitor[1], IdDOLeitor[2], IdDOLeitor[3], status);
        sprintf(mensagem, "<FD;%c%c%c%c;A0>", idDoLeitor[0], idDoLeitor[1], idDoLeitor[2], idDoLeitor[3]);
        enviaDadosParaEthPortais(mensagem, strlen(mensagem));
    //}       
}


void defineDestinoDasMensagensEthernet(char ipRemoto[TAMANHO_DA_STRING_DE_IP], int portaRemota){
    char comando[50];

    entraEmModoAt();
    //entraEmModoAt();
    sprintf(comando, "AT+SOCK=TCPC,%s,%d\r", ipRemoto, portaRemota);
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(100);
    resetModuloETH();
    //delay_ms(100);
    //saiDoModoAt();
}


char solicitaStatusDeConexaoTCP(void){
    char comando[50];
    char respostaEsperada[TAMANHO_BUFFER_RX_ETHERNET];
    
    entraEmModoAt();
    delay_ms(200);
    limpaBufferRxEthernet();
    sprintf(comando, "AT+SOCKLK\r");
    enviaDadosParaEthPortais(comando, strlen(comando));
    delay_ms(100);
    strcpy(respostaEsperada, "\r\n+OK=connect\r\n");
    if(strncmp(bufferRxEthernet, respostaEsperada, 15) == 0){
        delay_ms(100);
        saiDoModoAt();
        delay_ms(100);
        return 1;
    }else{
        delay_ms(200);
        saiDoModoAt();
        delay_ms(100);
        return 0;
    } 
}


// esta rotina manipula os dados que chegam da ethernet

void CommandHandleModuloEthernet(void){
    //char comandoRecebido[100];
    //char string[100];
    
    if(bufferRxEthernet[0] != '<'){
        return;
    }
    
    if(strncmp(bufferRxEthernet, "<FD;OK>", 7) == 0){
        limpaBufferRxEthernet();
        liga_buzzer();
        delay_ms(5);
        desliga_buzzer();
        timeoutRespostaKeepAlive = 0;
        return;
    }
    
    if(strncmp(bufferRxEthernet, "<FE;OK>", 7) == 0){
        limpaBufferRxEthernet();
        liga_buzzer();
        delay_ms(5);
        desliga_buzzer();
        return;
    }
}



void contaIntervaloDeKeepAlive(void){
    //if(statusDeConexaoTCP != NOT_CONNECTED) return;
    if(timeoutRespostaKeepAlive < 5){
        timeoutRespostaKeepAlive = timeoutRespostaKeepAlive + 1;
        statusDeConexaoTCP = CONNECTED;
        alarmeFaltaDeRedeEthernet = 0;
        return;
    }else{
        statusDeConexaoTCP = NOT_CONNECTED;
        alarmeFaltaDeRedeEthernet = 1;
    }
}

void contaIntevaloEntreTrocaDeRemoteIP(void){
    if(timerParaTrocaDeRemoteIP < INTERVALO_DE_TROCA_REMOTE_IP){
        timerParaTrocaDeRemoteIP = timerParaTrocaDeRemoteIP + 1;
    }
}


void setaStatusDoLedDeEthernet(void){
    if(statusDeConexaoTCP == CONNECTED){
        LED_REDE = 1;
    }else{
        LED_REDE = !LED_REDE;
    }
}

void alertaSemConexaoComEthernet(void){
    static char alarmeAtivado = 0;
    
    if(alarmeFaltaDeRedeEthernet == 1){
        _LATB3 = !_LATB3; // SINALEIRO VERMELHO
        _LATC2 = !_LATC2; // SIRENE
        _LATG15 = 0;
        _LATC1 = 0;
        //travaCancelaDoPortal();
        alarmeAtivado = 1;
    }else{
        if(alarmeAtivado == 1){
            alarmeAtivado = 0;
            _LATB3 = 0; // SINALEIRO VERMELHO
            _LATC2 = 0; // SIRENE
            //destravaCancelaDoPortal();
        }
    }        
}


void checaNecessidadeDeTrocaDeIPRemoto(void){
    char mensagem[100];
    
    if(statusDeConexaoTCP == NOT_CONNECTED && timerParaTrocaDeRemoteIP >= INTERVALO_DE_TROCA_REMOTE_IP){
        timerParaTrocaDeRemoteIP = 0;
        //statusDeConexaoTCP = TENTANDO_CONECTAR;
        if(destinoDosEventos == IP_REMOTO_PRINCIPAL){
            defineDestinoDasMensagensEthernet(ipRemotoSecundario, portaRemotaSecundaria);
            destinoDosEventos = IP_REMOTO_SECUNDARIO;
            sprintf(mensagem, "Tentando conexao com: %s:%d\r\n", ipRemotoSecundario, portaRemotaSecundaria);
            enviaDadosParaUSBserial(mensagem, strlen(mensagem));
        }else{
            defineDestinoDasMensagensEthernet(ipRemotoPrincipal, portaRemotaPrincipal);
            destinoDosEventos = IP_REMOTO_PRINCIPAL;
            sprintf(mensagem, "Tentando conexao com: %s:%d\r\n", ipRemotoPrincipal, portaRemotaPrincipal);
            enviaDadosParaUSBserial(mensagem, strlen(mensagem));
        }
    }
}


void defineIPRemotoInicial(void){
    defineDestinoDasMensagensEthernet(ipRemotoPrincipal, portaRemotaPrincipal);
    destinoDosEventos = IP_REMOTO_PRINCIPAL;    
}

void limpaBufferRxEthernet(void){
    memset(bufferRxEthernet, 0, TAMANHO_BUFFER_RX_ETHERNET - 1);
}

  
void aguardoPrimeiraConexaoTCP( void ) {
    
    char mensagem[ 100 ];
    
    statusDeConexaoTCP = NOT_CONNECTED;
    
    if( debugInterfaceEthernet_Silent ) {
    
        sprintf( mensagem, "Definindo IP Remoto principal...\r\n" );
        
        enviaDadosParaUSBserial( mensagem, strlen( mensagem ) );
        
    }
    
    defineIPRemotoInicial();
    
    delay_ms( 2000 );
    
    if( debugInterfaceEthernet_Silent ) {
        
        sprintf( mensagem, "Tentando conexao com: %s:%d\r\n", ipRemotoPrincipal, portaRemotaPrincipal );
    
        enviaDadosParaUSBserial( mensagem, strlen( mensagem ) );
    }
    
    while( solicitaStatusDeConexaoTCP() == 0 ) {
        //sprintf( mensagem, "." );
        //enviaDadosParaUSBserial( mensagem, strlen( mensagem ) );
        delay_ms( 100 );
    }
    
    statusDeConexaoTCP = CONNECTED;
    
    if( debugInterfaceEthernet_Silent ) {
        
        sprintf( mensagem, "\r\nCONECTADO!\r\n" );

        enviaDadosParaUSBserial( mensagem, strlen( mensagem ) );
        
    }
}



void geraEventosDePassagemParaTestarEmpi( void ) {
    
    char mensagem[ 100 ];
    
    static char EPC0 = 30;
    
    static char EPC1 = 0;
    
    static char EPC2 = 0;    
    
    EPC0 = 30;
    
    if( statusDeConexaoTCP == CONNECTED ){
        
        if( EPC1 >= 3 ) return;
        
        for( EPC1 = 0; EPC1 < 3; EPC1 = EPC1 + 1 ){
            
            EPC2 = 0;
            
            for( EPC2 = 0; EPC2 < 50; EPC2 = EPC2 + 1 ){
                
                sprintf( mensagem, "<FF;%c%c%c%c;E;%02d%02d%02d00;%02d%02d20%02d;%02d%02d%02d>", 
                                    idDoLeitor[0], 
                                    idDoLeitor[1], 
                                    idDoLeitor[2], 
                                    idDoLeitor[3], 
                                    EPC0, EPC1, EPC2, 
                                    dataHora->tm_mday, 
                                    dataHora->tm_mon + 1, 
                                    dataHora->tm_year - 100, 
                                    dataHora->tm_hour, 
                                    dataHora->tm_min, 
                                    dataHora->tm_sec );
                
                enviaDadosParaEthPortais( mensagem, strlen( mensagem ) );
                
                delay_ms( 200 );
                
            }
        }
    }    
}