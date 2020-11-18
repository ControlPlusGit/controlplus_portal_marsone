
#include "as3993_public.h"
#include "timer.h"
#include "i2c.h"
#include "onewire.h"
#include "as3993_public.h"
#include "uart_driver.h"
#include "as3993.h"
#include "platform.h"
#include "tuner.h"
#include <time.h>
#include "rtc.h"
#include "exclusao.h"
#include "portal.h"
#include "perifericos.h"
#include "eth.h"
#include "appl_commands.h"
#include "cancelas.h"
#include "setup_usb.h"


extern u16 readerInitStatus;
extern Freq Frequencies;
extern void uart4Tx (u8 dado);

extern TunerConfiguration mainTuner;

TipoRegistroDeTagEmPortal RegistroDeTagEmPortal[MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL];

#define PARAMETRO_SETA_FREQUENCIA_ANATEL 0x55


char IdDoLeitor[QUANTIA_DE_BYTES_DE_ID];

char StringDeSaidaParaSerial[TAMANHO_MAXIMO_DE_STRING_DE_SAIDA];
char DadosParaASerial;
char SerialOcupada;
int ContadorDeTagMonitoradas;
int ContadorDeTagMovimentos;

int TagEmTratamento;
//int TagEmTratamentoEmTick;

int ModoDeOperacao;
int TempoParaInatividadeDeTagMonitorada;
char estadoSerialFrango;
unsigned char ComandoDaSerial;
unsigned char PrimeiroParametroDaSerial;
unsigned char SensibilidadeDaAntena;

int NumeroDeAntenasLidas;
int AtrasoParaDegradarLeitura;
int RepeticaoNaLeitura;
int ModoWieGand;
int Frequencia;
int FrequenciaTraseira;
int PortaDePortalDeExclusao;
int InibeOperacaoEMesagensComOPC;
char IdDOLeitor[20] = "4035";

int PausaNasMensagensDeAvaliacao = 0;

int Beepar;
char Capacitores[6];

struct tm *dataHora;

int ContadorParaEnviarTabelaDeTagsVisiveis;
int ContadorParaEnviarTabelaDeTagsInisiveis;
#define TEMPO_PARA_ENVIAR_TABELA 50//2


#define TEMPO_DO_ACIONAMENTO_DO_SINALEIRO 20//10
volatile int EstadoDoSinaleiro;
int ContadorDeTempoDoSinaleiro;

#define TEMPO_DE_SILENCIO_NA_485 20
int ContadorDeSilencioNa485;
int PodeEnviarTabelaDeInvisiveis;



void setaSensibilidade (unsigned char valor);
void lidaComComandoDoPcLegado(unsigned char dado);
void obtemParametrosSalvosNaEeprom(void);
void enviaFluxoPorPortaSelecionada(char *Fluxo, int Tamanho, int Porta);
void acionaBeepDeComandoFrango(int quantia);

//struct EstruturaProtocolo EstruturaDeRecepcaoDoPC;
unsigned char CorpoDeComandoRecebidoDoPC[10];

int ContagemParaExpirarOsComandosDoPC;

#define TAMANHO_DA_LISTA 10
#define TAMANHO_DO_ITEM 10

unsigned char ListaDeSaida[TAMANHO_DA_LISTA][TAMANHO_DO_ITEM];
int ContadorDeReenvioDeDadosDaListaDeSaida;

void EnviaBytePara485 (unsigned char dado){
    
#ifdef RF_IDEIAS
        uart3Tx(dado);
        dado = dado + 1;
#else 
    /*
    //Para zigbee beta
    //while (_LATG13 != 0);
    _LATG13 = 1;
    
    U1TXREG = dado;
    while( (!U1STAbits.TRMT ) );
    _LATG13 = 0;
    */

    //Para zigbee normal
    //while (_LATG13 != 0);
    _LATG13 = 1;
    
    //U4TXREG = dado;
    //while( (!U4STAbits.TRMT ) );
    U1TXREG = dado;
    while( (!U1STAbits.TRMT ) );
    _LATG13 = 0;
#endif    
    
}

typedef struct {
    int InterfaceUsada;
    int Comando;
    int TamanhoDoPacote;
	char Reserva[50];
    int PodeTratar;
} ComandoDoPC;

ComandoDoPC ComandoObtidoPorUmPC;


void limpaComandoObtidoDoPC(void){
    ComandoObtidoPorUmPC.Comando = 0;
    ComandoObtidoPorUmPC.InterfaceUsada = 0;
    ComandoObtidoPorUmPC.PodeTratar = -1;
}

void trataComandoRecebidoDoPCEmCodigoCorrente(void){
    /*char Rascunho[50];
    
    confereSeTemUmModuloPresentef();
    
    if (ComandoObtidoPorUmPC.PodeTratar == 0){
        if (ComandoObtidoPorUmPC.Comando == 0xB0){
            coletaOMACDoModulo(Rascunho);
            if (Rascunho[0] != 0){
                enviaFluxoPorPortaSelecionada(Rascunho, strlen(Rascunho), ComandoObtidoPorUmPC.InterfaceUsada);
            }
            acionaBeepDeComandoFrango(1);
        }
        
        if (ComandoObtidoPorUmPC.Comando == 0xB1){
            setaOMACDOModulo(ComandoObtidoPorUmPC.Reserva);
            acionaBeepDeComandoFrango(1);
        }
        
        if (ComandoObtidoPorUmPC.Comando == 0xB2){
            formataOModuloWifi();
        }

        if (ComandoObtidoPorUmPC.Comando == 0xB3){
            memcpy(IdDoLeitor, ComandoObtidoPorUmPC.Reserva, QUANTIA_DE_BYTES_DE_ID);
            EscreveArray(POSICAO_ID_DO_LEITOR, (char *)IdDoLeitor, QUANTIA_DE_BYTES_DE_ID);
            //enviaFluxoPorPortaSelecionada(Rascunho, strlen(Rascunho), ComandoObtidoPorUmPC.InterfaceUsada);
            acionaBeepDeComandoFrango(1);

        }
        
        if (ComandoObtidoPorUmPC.Comando == 0xB4){
            LerDadosDaEEprom(POSICAO_ID_DO_LEITOR, (unsigned char *)&Rascunho[0]);
            IdDoLeitor[0] = Rascunho[0];
            LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 1, (unsigned char *)&Rascunho[0]);
            IdDoLeitor[1] = Rascunho[0];
            LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 2, (unsigned char *)&Rascunho[0]);
            IdDoLeitor[2] = Rascunho[0];
            LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 3, (unsigned char *)&Rascunho[0]);
            IdDoLeitor[3] = Rascunho[0];
            sprintf(Rascunho, "\n%c%c%c%c", IdDoLeitor[0],IdDoLeitor[1],IdDoLeitor[2],IdDoLeitor[3]);
            enviaFluxoPorPortaSelecionada(Rascunho, QUANTIA_DE_BYTES_DE_ID, ComandoObtidoPorUmPC.InterfaceUsada);
        }


        
        if (ComandoObtidoPorUmPC.Comando == 0x56){
            //formataAMemoria();
            acionaBeepDeComandoFrango(1);
        }
        
        limpaComandoObtidoDoPC();
    }
     */ 
}



void EnviaArrayDeBytesPara485 (char *dado, int tamanho){
    int contador;
    //while (_LATG13 != 0);
    _LATG13 = 1;
    
    for (contador = 0;contador < tamanho;contador = contador + 1){
#ifdef RF_IDEIAS
        uart3Tx((u8)*dado);
        dado = dado + 1;
#else
        U1TXREG = *dado;
        //U4TXREG = *dado;
        dado = dado + 1;
        while( (!U1STAbits.TRMT ) );
        //while( (!U4STAbits.TRMT ) );
#endif
    }
    _LATG13 = 0;
}


void EnviaStringPara485 (char * dado){
    while (*dado != NULL){
        EnviaBytePara485(*dado);
        dado = dado + 1;
    }
}


void iniciaListaDeSaida(void){
    int Contador;
    
    for (Contador = 0;Contador < TAMANHO_DA_LISTA;Contador = Contador + 1){
        memset(ListaDeSaida[Contador], 0, TAMANHO_DO_ITEM);
    }
    ContadorDeReenvioDeDadosDaListaDeSaida = 0;
}

int procuraPosicaoLivreNaListaDeSaida(void){
    int Contador;
    for(Contador = 0;Contador < TAMANHO_DA_LISTA;Contador = Contador + 1){
        if (ListaDeSaida[Contador][0] == 0){
            return Contador;
        }
    }
    return -1;
}

int procuraPosicaoOcupadaNaListaDeSaida(void){
    int Contador;
    for(Contador = 0;Contador < TAMANHO_DA_LISTA;Contador = Contador + 1){
        if (ListaDeSaida[Contador][0] != 0){
            return Contador;
        }
    }
    return -1;
}


int adicionaItemNaListaDeSaida(unsigned char *Item){
    int Posicao;
    return 0;
    Posicao = procuraPosicaoLivreNaListaDeSaida();
    if (Posicao < 0)return -1;
    memcpy(ListaDeSaida[Posicao], Item, TAMANHO_DO_ITEM);
    ContadorDeReenvioDeDadosDaListaDeSaida = TEMPO_PARA_REENVIO_DA_LISTA_DE_SAIDA;
    return 0;
}

void realocaListaDeSaida(void){
    int Contador;
    for(Contador = 0;Contador < (TAMANHO_DA_LISTA - 1);Contador = Contador + 1){
        memcpy(ListaDeSaida[Contador], ListaDeSaida[Contador + 1], TAMANHO_DO_ITEM);
        memset(ListaDeSaida[Contador + 1], 0, TAMANHO_DO_ITEM);
    }
}

int recuperaERemoveItemDaListaDeSaida(unsigned char *Item){
    int Posicao;
    Posicao = procuraPosicaoOcupadaNaListaDeSaida();
    if (Posicao >= 0){
        memcpy(Item, ListaDeSaida[Posicao], TAMANHO_DO_ITEM);
        realocaListaDeSaida();
        return Posicao;
    }
    return -1;
}


int recuperaItemDaListaDeSaida(unsigned char *Item){
    int Posicao;
    Posicao = procuraPosicaoOcupadaNaListaDeSaida();
    if (Posicao >= 0){
        memcpy(Item, ListaDeSaida[Posicao], TAMANHO_DO_ITEM);
        return Posicao;
    }
    return -1;
}

void RemovePrimeiroItemDaListaDeSaida(void){
    //int Posicao;
    //Posicao = procuraPosicaoOcupadaNaListaDeSaida();
    realocaListaDeSaida();
}

void iniciaEstruturaDoProtocoloPC (void){
/*    EstruturaDeRecepcaoDoPC.Comando = 0;
    EstruturaDeRecepcaoDoPC.Estado = 0;
    EstruturaDeRecepcaoDoPC.Tamanho = 0;
    EstruturaDeRecepcaoDoPC.ContagemDoCorpo = 0;
    //ContadorParaExpirarOsDadosObtidosPorZigBee = 0;*/
}


int trataRecepcaoDoProcoloNoPortal (unsigned char Dado, int *Estado, unsigned char *Comando, unsigned int *Tamanho, unsigned char *Corpo,
                                    int *ContadorDoCorpo, unsigned int  *CheckSum){
    /*
    unsigned char Rascunho;
    //zeraAContagemDeSilencioNoZigBee();
    ContagemParaExpirarOsComandosDoPC = 3;
	*CheckSum = *CheckSum + Dado;
	*Tamanho = *Tamanho - 1;
	switch (*Estado){
		case ESPERANDO_CABECA:
            //Rascunho = Dado & 0xF0;
            Rascunho = Dado;
            if (//(Rascunho == 0xF0) ||
                (Rascunho == 0xF5) ||
                (Rascunho == 0xF6) 
#ifdef INTERFACE_DE_AJUSTES
                ||
                (Rascunho == 0xF6) ||
                (Rascunho == 0xFE) ||
                (Rascunho == 0xFD) ||
                (Rascunho == 0x55) ||
                (Rascunho == 0xAA) ||
                (Rascunho == 0xAB) ||
                (Rascunho == 0x80) ||
                (Rascunho == 0x7F) ||
                (Rascunho == 0x89) ||
                (Rascunho == 0x8A) ||
                (Rascunho == 0x8B) ||
                (Rascunho == 0x8C) ||
                (Rascunho == 0x8D) ||
                (Rascunho == 0x8E) ||
                (Rascunho == 0x8F) ||
                (Rascunho == 0x90) ||
                (Rascunho == 0x91) ||
                (Rascunho == 0x92)
#endif
                    ){
                *Estado = ESPERANDO_TAMANHO_MSB;
                *CheckSum = Dado;
                //printf("Obtem cabeca = %02X\n", Dado);
                *Comando = Dado;
                *ContadorDoCorpo = 0;
            }
			break;
		case ESPERANDO_TAMANHO_MSB:
			*Estado = PEGANDO_O_CORPO;
			//printf("Obtem tamanho = %d\n", Dado);
			*Tamanho = Dado - 2;//2
            if (*Tamanho > 16){
                *Estado = ESPERANDO_CABECA;
                *Tamanho = 0;
            }
			break;
		case PEGANDO_O_CORPO:
			if (*Tamanho != 0){
				*Corpo = Dado;
				//printf("Obtem corpo = %c\n", Dado);
				*ContadorDoCorpo = *ContadorDoCorpo + 1;
			} else {
				*CheckSum = *CheckSum - Dado;
				//printf("CheckSum CG = %d e DD %d\n", *CheckSum, Dado);

				*Estado = ESPERANDO_CABECA;
                *CheckSum = *CheckSum & 0x00FF;
				if (Dado == *CheckSum){
                    *CheckSum = 0;
					return 1;
                    *CheckSum = 0;
				} else {
					return -1;
				}
			}
			break;
		default:
			*Estado = ESPERANDO_CABECA;
			break;
	}*/
	return 0;
}


void testeSinaleiro (void){
    while(1){
        liga_rele2(); // amarelo
        desliga_rele3();
        desliga_rele4();
        delay_ms(2000);
        liga_rele3(); //verde
        desliga_rele2();
        desliga_rele4();
        delay_ms(2000);
        liga_rele4(); //vermelho
        desliga_rele2();
        desliga_rele3();
        delay_ms(2000);
    }
}

void setaSinaleiro (int estado){
    if (EstadoDoSinaleiro != SINALEIRO_VERDE){
        ContadorDeTempoDoSinaleiro = TEMPO_DO_ACIONAMENTO_DO_SINALEIRO;
        if (EstadoDoSinaleiro != estado){
            EstadoDoSinaleiro = estado;
            /*
            desliga_rele2();
            desliga_rele3();
            desliga_rele4();
            */
        }
    }
    
    /*
    if (EstadoDoSinaleiro == SINALEIRO_VERMELHO){
        ContadorDeTempoDoSinaleiro = TEMPO_DO_ACIONAMENTO_DO_SINALEIRO;
        EstadoDoSinaleiro = estado;
    }
    */
}

int TempoDeAcionamentoDoBuzzerDeCancelamento;

void acionaBeepDeCancelamento(void){
    TempoDeAcionamentoDoBuzzerDeCancelamento = 25;
}

void atulizaBeepDeCancelamento(void){
    if (TempoDeAcionamentoDoBuzzerDeCancelamento > 15){
        TempoDeAcionamentoDoBuzzerDeCancelamento = TempoDeAcionamentoDoBuzzerDeCancelamento - 1;
        liga_rele1();
    } else {
        if (TempoDeAcionamentoDoBuzzerDeCancelamento > 10){
            TempoDeAcionamentoDoBuzzerDeCancelamento = TempoDeAcionamentoDoBuzzerDeCancelamento - 1;
            desliga_rele1();
        } else {
            if (TempoDeAcionamentoDoBuzzerDeCancelamento > 0){
                TempoDeAcionamentoDoBuzzerDeCancelamento = TempoDeAcionamentoDoBuzzerDeCancelamento - 1;
                liga_rele1();
            } else {
                TempoDeAcionamentoDoBuzzerDeCancelamento = 0;
                desliga_rele1();
            }
                
        }

    }
    
    if (TempoDeAcionamentoDoBuzzerDeCancelamento < 0){
        TempoDeAcionamentoDoBuzzerDeCancelamento = 0;
    }
            

    
    /*
    if (TempoDeAcionamentoDoBuzzerDeCancelamento > 0){
        TempoDeAcionamentoDoBuzzerDeCancelamento = TempoDeAcionamentoDoBuzzerDeCancelamento - 1;
        liga_rele1();
    } else {
        TempoDeAcionamentoDoBuzzerDeCancelamento = 0;
        desliga_rele1();
    }
    */
}

/*
void atualizarSinaleiro (void){
    if (ContadorDeTempoDoSinaleiro > (TEMPO_DO_ACIONAMENTO_DO_SINALEIRO - 1)){
        ContadorDeTempoDoSinaleiro = ContadorDeTempoDoSinaleiro - 1;
    } else {

        if (ContadorDeTempoDoSinaleiro > 0){
            ContadorDeTempoDoSinaleiro = ContadorDeTempoDoSinaleiro - 1;
        }


        if ((ContadorDeTempoDoSinaleiro == 0) && (EstadoDoSinaleiro != SINALEIRO_VERMELHO)){
            ContadorDeTempoDoSinaleiro = -1;
            EstadoDoSinaleiro = SINALEIRO_VERMELHO;
            desliga_rele2();
            //liga_rele2(); // amarelo
            desliga_rele3();
            //desliga_rele4();
            liga_rele4(); //vermelho
        } else {
        
            switch(EstadoDoSinaleiro){
                case SINALEIRO_VERMELHO:
                    liga_rele4(); //vermelho
                    desliga_rele2();
                    desliga_rele3();
                    break;
                case SINALEIRO_AMARELO:
                    liga_rele2(); // amarelo
                    desliga_rele3();
                    desliga_rele4();
                    break;
                case SINALEIRO_VERDE:
                    liga_rele3(); //verde
                    desliga_rele2();
                    desliga_rele4();
                    break;
                
                //case SINALEIRO_CANCELADO:
                    //desliga_rele3();
                    //liga_rele2(); // amarelo
                    //liga_rele4(); //vermelho
                    //break;
                //
                default:
                    EstadoDoSinaleiro = SINALEIRO_VERMELHO;
                    ContadorDeTempoDoSinaleiro = 2;
                    break;
            }
        }
    }
}
*/
void atualizarSinaleiro (void){
    if (ContadorDeTempoDoSinaleiro > (TEMPO_DO_ACIONAMENTO_DO_SINALEIRO - 1)){
        ContadorDeTempoDoSinaleiro = ContadorDeTempoDoSinaleiro - 1;
    } else {

        if (ContadorDeTempoDoSinaleiro > 0){
            ContadorDeTempoDoSinaleiro = ContadorDeTempoDoSinaleiro - 1;
        }


        if ((ContadorDeTempoDoSinaleiro == 0) && (EstadoDoSinaleiro != SINALEIRO_VERMELHO)){
            ContadorDeTempoDoSinaleiro = -1;
            EstadoDoSinaleiro = SINALEIRO_VERMELHO;
            desliga_rele2();
            //liga_rele2(); // amarelo
            desliga_rele3();
            //desliga_rele4();
            liga_rele4(); //vermelho
        } 
    }
        
    switch(EstadoDoSinaleiro){
        case SINALEIRO_VERMELHO:
            liga_rele4(); //vermelho
            desliga_rele2();
            desliga_rele3();
            break;
        case SINALEIRO_AMARELO:
            liga_rele2(); // amarelo
            desliga_rele3();
            desliga_rele4();
            break;
        case SINALEIRO_VERDE:
            liga_rele3(); //verde
            desliga_rele2();
            desliga_rele4();
            break;

        //case SINALEIRO_CANCELADO:
            //desliga_rele3();
            //liga_rele2(); // amarelo
            //liga_rele4(); //vermelho
            //break;
        //
        default:
            EstadoDoSinaleiro = SINALEIRO_VERMELHO;
            ContadorDeTempoDoSinaleiro = 2;
            break;
    }
        

   
}

void acionaBeepDeComandoFrango(int quantia){
    Beepar = quantia;
}

void ligarBuzzer(void){
    _LATC4 = 1;
}

void desligarBuzzer(void){
    _LATC4 = 0;
}


void realizaBeepDeComandoFrango (void){
    while (Beepar !=0){
        Beepar = Beepar - 1;
        //liga_buzzer();
        ligarBuzzer();
        delay_ms(100);
        //desliga_buzzer();
        desligarBuzzer();
        delay_ms(100);
    }
}

void aguradaSilencioNa485(void){
    while(ContadorDeSilencioNa485);
}

void enviaMovimentoParaWiegand(unsigned char *Dado, int Movimento, unsigned char Antena){
    /*unsigned char Dados[3];
    unsigned char Rascunho;

    if (ModoWieGand == WIEGAND_PARA_PORTAL){
        Dados[0] = Antena;
        Dados[0] = Dados[0] - 1;
        Dados[0] = Dados[0] << 5;
        Movimento = Movimento << 4;
        Dados[0] = Dados[0] | Movimento;
        Rascunho = *Dado & 0x0F;
        Dados[0] = Dados[0] | Rascunho;
        Dado = Dado + 1;
        Dados[1] = *Dado;
        Dado = Dado + 1;
        Dados[2] = *Dado;
        enviaWiegand(Dados, 3);
    }*/
}


void enviaTAgParaAvaliacaoParaWiegand(unsigned char *Dado, unsigned char Antena){
    /*unsigned char Dados[3];
    unsigned char Rascunho;

    if (ModoWieGand == WIEGAND_PARA_AVALIACAO){
        Dados[0] = Antena;
        Dados[0] = Dados[0] - 1;
        Dados[0] = Dados[0] << 5;
        Rascunho = *Dado & 0x1F;
        Dados[0] = Dados[0] | Rascunho;
        Dado = Dado + 1;
        Dados[1] = *Dado;
        Dado = Dado + 1;
        Dados[2] = *Dado;
        enviaWiegand(Dados, 3);
    }*/
}

void resetaParametrosFrango (void){
    
}


__attribute__((far)) char SsidWifi[TAMANHO_MAXIMO_DO_TEXT_SSID];
__attribute__((far)) char SenhaWifi[TAMANHO_MAXIMO_DO_TEXT_SSID];

void salvaParametrosFrango (void){
//int EscreverNaEEprom (int endereco, unsigned char dado){
    EscreverNaEEprom(0, (unsigned char)ModoDeOperacao);
    EscreverNaEEprom(1, (unsigned char)AtrasoParaDegradarLeitura);
    EscreverNaEEprom(2, (unsigned char)RepeticaoNaLeitura);
    EscreverNaEEprom(3, (unsigned char)SensibilidadeDaAntena);
    EscreverNaEEprom(4, (unsigned char)TempoParaInatividadeDeTagMonitorada);
    EscreverNaEEprom(5, (unsigned char)NumeroDeAntenasLidas);
    EscreverNaEEprom(6, (unsigned char)ModoWieGand);
    EscreverNaEEprom(7, (unsigned char)Frequencia);
    EscreverNaEEprom(8, (unsigned char)PortaDePortalDeExclusao);
    EscreverNaEEprom(9, (unsigned char)Capacitores[0]);
    EscreverNaEEprom(10, (unsigned char)Capacitores[1]);
    EscreverNaEEprom(11, (unsigned char)Capacitores[2]);
    EscreverNaEEprom(12, (unsigned char)InibeOperacaoEMesagensComOPC);
    EscreverNaEEprom(13, (unsigned char)InibeOperacaoEMesagensComOPC);
    EscreverNaEEprom(14, (unsigned char)Capacitores[3]);
    EscreverNaEEprom(15, (unsigned char)Capacitores[4]);
    EscreverNaEEprom(16, (unsigned char)Capacitores[5]);
    EscreverNaEEprom(17, (unsigned char)FrequenciaTraseira);
    
    //EscreverNaEEprom(POSICAO_SENHA_WIFI, (unsigned char)InibeOperacaoEMesagensComOPC);
    
    
}


void enviaNovosjustesDeWifi (void){
    /*int Tamanho = 0;
    
    Tamanho = strlen(SsidWifi);
    if (Tamanho > 0){
        //enviaFluxoDadosPorInterrupcaoDeTX(SsidWifi, strlen(SsidWifi));
        enviaFluxoDeDadosParaUartWifi(SsidWifi, strlen(SsidWifi));
        memset(SsidWifi, 0, TAMANHO_MAXIMO_DO_TEXT_SSID);
        delay_ms(1000);
    }*/
}

void salvaSSIDDoWifi (char *Texto){
    /*char Comando[50];
    //enviaPacoteParaOEsp8266('AT+CWJAP_DEF="controlup1","controlup"')
    sprintf(Comando, "AT+CWJAP_DEF=%s\r\n", Texto);
    //enviaFluxoDadosPorInterrupcaoDeTX(Comando, strlen(Comando));
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));
    //EscreveArray(13, Texto, (strlen(Texto) + 1));
    //sprintf(SsidWifi, "%s", Texto);
    //mudaOSSIDESenha();*/
}

void resetNoWifi (void){
    /*char Comando[50];
    sprintf(Comando, "\r\nAT+RST\r\n");
    enviaFluxoDeDadosPorPolling(Comando, strlen(Comando));*/
}

void salvaSenhaDoWifi (char *Texto){
    /*EscreveArray(POSICAO_SENHA_WIFI, Texto, strlen(Texto));
    sprintf(SenhaWifi, "%s",  Texto);
    mudaOSSIDESenha();*/
}

void leOSSIDESenhaDoWifi(void){
    int Contador;
    unsigned char Dado; 
    
    for (Contador = 0;Contador < TAMANHO_MAXIMO_DO_TEXT_SSID;Contador = Contador + 1){
        LerDadosDaEEprom((13 + Contador), &Dado); 
        SsidWifi[Contador] = Dado;
    }
    
    for (Contador = 0;Contador < TAMANHO_MAXIMO_DO_TEXT_SSID;Contador = Contador + 1){
        LerDadosDaEEprom((POSICAO_SENHA_WIFI + Contador), &Dado); 
        SenhaWifi[Contador] = Dado;
    }
    
    __asm("NOP");
}


void SetaFrequencias800(void){
    unsigned long rascunho;
    if (Frequencia < 100){
        rascunho = Frequencia;
        rascunho = rascunho + 800;
        rascunho = rascunho * 1000;
        Frequencies.freq[0] = rascunho;
        //Frequencies.numFreqs=1;
        Frequencies.numFreqs = 0;
    }

    if (Frequencia == PARAMETRO_SETA_FREQUENCIA_ANATEL){
        //lista_freq_anatel();
    }

    //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    //tunerInit(&mainTuner);
}


void SetaFrequencias(void){
    unsigned long rascunho;
    
    if (Frequencia < 33){
        rascunho = Frequencia;
        rascunho = rascunho + 900;
        rascunho = rascunho * 1000;
        Frequencies.freq[0] = rascunho;
        Frequencies.freq[1] = rascunho;
        Frequencies.numFreqs=1;
    }

    if (Frequencia == PARAMETRO_SETA_FREQUENCIA_ANATEL){
        //lista_freq_anatel();
        Frequencies.freq[0] = 902750;
        Frequencies.numFreqs = 1;
    }

#ifdef INTERFACE_DE_AJUSTES
    TerminaInvetorio();
    //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    comecaInvetorio();
    //tunerInit(&mainTuner);
#endif
    
}


void SetaFrequenciaTraseira(void){
    unsigned long rascunho;
    
    if (Frequencia < 33){
        rascunho = FrequenciaTraseira;
        rascunho = rascunho + 900;
        rascunho = rascunho * 1000;
        Frequencies.freq[0] = rascunho;
        Frequencies.freq[1] = rascunho;
        Frequencies.numFreqs=1;
    }

    if (Frequencia == PARAMETRO_SETA_FREQUENCIA_ANATEL){
        //lista_freq_anatel();
        Frequencies.freq[0] = 902750;
        Frequencies.numFreqs = 1;
    }

#ifdef INTERFACE_DE_AJUSTES
    TerminaInvetorio();
    //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    comecaInvetorio();
    //tunerInit(&mainTuner);
#endif
}

void obtemParametrosSalvosNaEeprom(void){
//int LerDadosDaEEprom (int endereco, unsigned char *dado){
    unsigned char rascunho;

    LerDadosDaEEprom(0, &rascunho);
    ModoDeOperacao = rascunho;
    leOSSIDESenhaDoWifi();
    
    /*
    if ((ModoDeOperacao != OPERACAO_COM_LEITURA_UNICA) &&
            (ModoDeOperacao != OPERACAO_COM_MULTIPLAS_LEITURAS)){
        ModoDeOperacao = OPERACAO_COM_MULTIPLAS_LEITURAS;
    }
    */
    
    if ((ModoDeOperacao < OPERACAO_COM_LEITURA_UNICA) &&
            (ModoDeOperacao > OPERACAO_LEITURAS_INITERRUPTAS)){
        ModoDeOperacao = OPERACAO_COM_LEITURA_UNICA;
    }
    

    LerDadosDaEEprom(1, &rascunho);
    AtrasoParaDegradarLeitura = rascunho;
    if (AtrasoParaDegradarLeitura > (LIMITE_PARA_ATRASO_PARA_DEGRADAR_LEITURA - 1)){
        AtrasoParaDegradarLeitura = 0;
    }

    LerDadosDaEEprom(2, &rascunho);
    RepeticaoNaLeitura = rascunho;
    if (RepeticaoNaLeitura > (LIMITE_DE_REPETICAO_EM_ANTENAS - 1)){
        RepeticaoNaLeitura = 1;
    }

    LerDadosDaEEprom(3, &rascunho);
    SensibilidadeDaAntena = rascunho;
    if (SensibilidadeDaAntena > (LIMITE_DA_SENSIBILIDADE - 1)){
        SensibilidadeDaAntena = LIMITE_DA_SENSIBILIDADE;
    }

    LerDadosDaEEprom(4, &rascunho);
    TempoParaInatividadeDeTagMonitorada = rascunho;
    if (TempoParaInatividadeDeTagMonitorada > TEMPO_LIMITE_PARA_INATIVIDADE_DE_TAG_MONITORADA){
        TempoParaInatividadeDeTagMonitorada = TEMPO_LIMITE_PARA_INATIVIDADE_DE_TAG_MONITORADA;
    }
    
    

    LerDadosDaEEprom(5, &rascunho);
    NumeroDeAntenasLidas = rascunho;
    if (NumeroDeAntenasLidas > (LIMITE_DE_ANTENAS_FRANGO - 1)){
        NumeroDeAntenasLidas = LIMITE_DE_ANTENAS_FRANGO;
    }

    LerDadosDaEEprom(6, &rascunho);
    if (rascunho < FINAL_WIEGAND){
        ModoWieGand = rascunho;
    }

    LerDadosDaEEprom(7, &rascunho);
    Frequencia = rascunho;
    SetaFrequencias();

    LerDadosDaEEprom(8, &rascunho);
    PortaDePortalDeExclusao = rascunho;

    LerDadosDaEEprom(9, &rascunho);
    Capacitores[0] = rascunho;

    LerDadosDaEEprom(10, &rascunho);
    Capacitores[1] = rascunho;

    LerDadosDaEEprom(11, &rascunho);
    Capacitores[2] = rascunho;
    
    LerDadosDaEEprom(12, &rascunho);
    InibeOperacaoEMesagensComOPC = rascunho;
    
    
    LerDadosDaEEprom(14, &rascunho);
    Capacitores[3] = rascunho;

    LerDadosDaEEprom(15, &rascunho);
    Capacitores[4] = rascunho;

    LerDadosDaEEprom(16, &rascunho);
    Capacitores[5] = rascunho;
    
    LerDadosDaEEprom(17, &rascunho);
    FrequenciaTraseira = rascunho;
    
}

void obtemParamentrosSalvosFrango (void){
//int LerDadosDaEEprom (int endereco, unsigned char *dado){
    unsigned char rascunho;

    LerDadosDaEEprom(0, &rascunho);
    ModoDeOperacao = rascunho;
      
    if (ModoDeOperacao < OPERACAO_COM_LEITURA_UNICA){
        ModoDeOperacao = OPERACAO_COM_LEITURA_UNICA;
    }

    if (ModoDeOperacao > OPERACAO_LEITURAS_INITERRUPTAS){
        ModoDeOperacao = OPERACAO_LEITURAS_INITERRUPTAS;
    }
    
    LerDadosDaEEprom(1, &rascunho);
    AtrasoParaDegradarLeitura = rascunho;
    if (AtrasoParaDegradarLeitura > (LIMITE_PARA_ATRASO_PARA_DEGRADAR_LEITURA - 1)){
        AtrasoParaDegradarLeitura = 0;
    }

    LerDadosDaEEprom(2, &rascunho);
    RepeticaoNaLeitura = rascunho;
    if (RepeticaoNaLeitura > (LIMITE_DE_REPETICAO_EM_ANTENAS - 1)){
        RepeticaoNaLeitura = 1;
    }

    LerDadosDaEEprom(3, &rascunho);
    SensibilidadeDaAntena = rascunho;
    if (SensibilidadeDaAntena > (LIMITE_DA_SENSIBILIDADE - 1)){
        SensibilidadeDaAntena = LIMITE_DA_SENSIBILIDADE;
    }

    LerDadosDaEEprom(4, &rascunho);
    TempoParaInatividadeDeTagMonitorada = rascunho;
    if (TempoParaInatividadeDeTagMonitorada > TEMPO_LIMITE_PARA_INATIVIDADE_DE_TAG_MONITORADA){
        TempoParaInatividadeDeTagMonitorada = TEMPO_LIMITE_PARA_INATIVIDADE_DE_TAG_MONITORADA;
    }

    LerDadosDaEEprom(5, &rascunho);
    NumeroDeAntenasLidas = rascunho;
    if (NumeroDeAntenasLidas > (LIMITE_DE_ANTENAS_FRANGO - 1)){
        NumeroDeAntenasLidas = LIMITE_DE_ANTENAS_FRANGO;
    }

    LerDadosDaEEprom(6, &rascunho);
    if (rascunho < FINAL_WIEGAND){
        ModoWieGand = rascunho;
    }

    LerDadosDaEEprom(7, &rascunho);
    Frequencia = rascunho;
    SetaFrequencias();

    LerDadosDaEEprom(8, &rascunho);
    PortaDePortalDeExclusao = rascunho;

    LerDadosDaEEprom(9, &rascunho);
    Capacitores[0] = rascunho;

    LerDadosDaEEprom(10, &rascunho);
    Capacitores[1] = rascunho;

    LerDadosDaEEprom(11, &rascunho);
    Capacitores[2] = rascunho;
    
    LerDadosDaEEprom(12, &rascunho);
    InibeOperacaoEMesagensComOPC = rascunho;

    LerDadosDaEEprom(13, &rascunho);
    IdDoLeitor[0] = rascunho;

    LerDadosDaEEprom(14, &rascunho);
    IdDoLeitor[1] = rascunho;

    LerDadosDaEEprom(15, &rascunho);
    IdDoLeitor[2] = rascunho;

    LerDadosDaEEprom(16, &rascunho);
    IdDoLeitor[3] = rascunho;

    LerDadosDaEEprom(17, &rascunho);
    FrequenciaTraseira = rascunho;
    SetaFrequenciaTraseira();
    
    TempoParaInatividadeDeTagMonitorada = 3000;
    
    ModoDeOperacao = OPERACAO_LEITURAS_INITERRUPTAS;
    
    AtrasoParaDegradarLeitura = 0;
    setaSensibilidade(125);
    NumeroDeAntenasLidas = 4;
    
    Frequencia = 19;
    SetaFrequencias();
    TempoParaInatividadeDeTagMonitorada = 7;//5;    
    PortaDePortalDeExclusao = 1;
    
    Capacitores[0] = 15; 
    Capacitores[1] = 7;
    Capacitores[2] = 9;
    
    RepeticaoNaLeitura = 10;
    
}

void salvaCapacitores(int Cin, int Cen, int Cout){
    EscreverNaEEprom(9, (unsigned char)Cin);
    EscreverNaEEprom(10, (unsigned char)Cen);
    EscreverNaEEprom(11, (unsigned char)Cout);
}

void enviarID(void){
    //sprintf(IdDOLeitor, "123455678");
    delay_ms(100);
    EnviaArrayDeBytesPara485(IdDOLeitor, strlen(IdDOLeitor));
    delay_ms(1000);
}

void EnviaByteParaPortaSelecionada(char Dado, int Porta){
    switch(Porta){
        case 1:
            //WIFI,
            U1TXREG = Dado;
            while( (!U1STAbits.TRMT ) );
            break;
        case 2:

        case 3:
            uart3Tx(Dado);
            break;
        case 4:
            U4TXREG = Dado;
            while( (!U4STAbits.TRMT ) );
            break;
    }

}

void enviaFluxoPorPortaSelecionada(char *Fluxo, int Tamanho, int Porta){
    int Contador;
    for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        switch(Porta){
            case 1:
                //WIFI,
                U1TXREG = *Fluxo;
                while( (!U1STAbits.TRMT ) );
                break;
            case 2:

            case 3:
                uart3Tx((u8)*Fluxo);
                break;
            case 4:
                U4TXREG = *Fluxo;
                while( (!U4STAbits.TRMT ) );
                break;
        }
        Fluxo = Fluxo + 1;
    }
    _LATG13 = 1;
}

void enviaParaSerialOsParametrosAJustados(int Interface){
    char rascunho[TAMANHO_MAXIMO_DE_STRING_DE_SAIDA];
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
//#ifndef EMPILHADEIRA
    unsigned int rascunhoParaFrequencia;
//#endif
    //EnviaBytePara485(0xAA);
    EnviaByteParaPortaSelecionada(0xAA, Interface);
    
//#ifdef PORTAL

    if (ModoDeOperacao == OPERACAO_COM_LEITURA_UNICA){
        (void)sprintf(rascunho, "Modo de operacao = Unico\n");
        //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
        enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
        memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    } else {
        if (ModoDeOperacao == OPERACAO_COM_MULTIPLAS_LEITURAS) {
            (void)sprintf(rascunho, "Modo de operacao = Multiplo\n");
            //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
            enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
            memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
        } else {
            if (ModoDeOperacao == OPERACAO_LEITURAS_INITERRUPTAS){
                (void)sprintf(rascunho, "Modo de operacao = Ininterrupto\n");
                //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
                enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
                memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
            } else {
                (void)sprintf(rascunho, "Modo de operacao invalido\n");
                //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
                enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
                memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
            }
        }
    }
    

    (void)sprintf(rascunho, "Atraso para degradacao = %d\n", AtrasoParaDegradarLeitura);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);

    (void)sprintf(rascunho, "Repeticao de leitura = %d\n", RepeticaoNaLeitura);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);

    (void)sprintf(rascunho, "Sensibilidade = %d\n", SensibilidadeDaAntena);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);

    (void)sprintf(rascunho, "Tempo para fechar um movimento ou abortar tag = %d\n", TempoParaInatividadeDeTagMonitorada);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);

    (void)sprintf(rascunho, "Numero de antenas lidas = %d\n", NumeroDeAntenasLidas);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);


    switch (ModoWieGand){
        case SEM_WIEGAND:
            (void)sprintf(rascunho, "Sem WIEGAND\n");
            //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
            enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
            memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
            break;
        case WIEGAND_PARA_AVALIACAO:
            (void)sprintf(rascunho, "WIEGAND para avaliar leituras\n");
            //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
            enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
            memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
            break;
        case WIEGAND_PARA_PORTAL:
            (void)sprintf(rascunho, "WIEGAND para portal\n");
            //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
            enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
            memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
            break;
    }


    if (Frequencia < 28){
        rascunhoParaFrequencia = Frequencia;
        rascunhoParaFrequencia = rascunhoParaFrequencia + 900;
        (void)sprintf(rascunho, "Frequencia = %dMhz\n", rascunhoParaFrequencia);
        //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
        enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
        memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    }
    if (Frequencia == PARAMETRO_SETA_FREQUENCIA_ANATEL){
        (void)sprintf(rascunho, "Frequencia = Anatel\n");
        //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
        enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
        memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    }
    
    
    if (Frequencia < 28){
        rascunhoParaFrequencia = FrequenciaTraseira;
        rascunhoParaFrequencia = rascunhoParaFrequencia + 900;
        (void)sprintf(rascunho, "Frequencia traseira = %dMhz\n", rascunhoParaFrequencia);
        //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
        enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
        memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    }
    if (Frequencia == PARAMETRO_SETA_FREQUENCIA_ANATEL){
        (void)sprintf(rascunho, "Frequencia traseira = Anatel\n");
        //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
        enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
        memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    }
    
    
    if (PortaDePortalDeExclusao != 0){
        (void)sprintf(rascunho, "Portal de exclusao ligado\n");
    } else {
        (void)sprintf(rascunho, "Portal de exclusao desligado\n");
    }
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);

    (void)sprintf(rascunho, "Capacitores: Cin = %d, Cen = %d, Cout = %d\n", Capacitores[0], Capacitores[1], Capacitores[2]);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    (void)sprintf(rascunho, "Capacitores Traseira: Cin = %d, Cen = %d, Cout = %d\n", Capacitores[3], Capacitores[4], Capacitores[5]);
    //EnviaArrayDeBytesPara485(rascunho, strlen(rascunho));
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    memset(rascunho, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    

//#else 
    //sprintf(rascunho, "Empilhadeira\nAjustes fixados em firmware");
    //enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);

    
//#endif 
    
    LerDadosDaEEprom(POSICAO_ID_DO_LEITOR, (unsigned char *)&rascunho[0]);
    IdDoLeitor[0] = rascunho[0];
    LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 1, (unsigned char *)&rascunho[0]);
    IdDoLeitor[1] = rascunho[0];
    LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 2, (unsigned char *)&rascunho[0]);
    IdDoLeitor[2] = rascunho[0];
    LerDadosDaEEprom(POSICAO_ID_DO_LEITOR + 3, (unsigned char *)&rascunho[0]);
    IdDoLeitor[3] = rascunho[0];
    sprintf(rascunho, "Id do leitor = %c%c%c%c\n", IdDoLeitor[0],IdDoLeitor[1],IdDoLeitor[2],IdDoLeitor[3]);
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);

    atualizaHoraEData();
    sprintf(rascunho, "Hora do leitor = %s\n", stringHora);
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    sprintf(rascunho, "Data do leitor = %s\n", stringData);
    enviaFluxoPorPortaSelecionada(rascunho, strlen(rascunho), Interface);
    
    //EnviaBytePara485(0x7F);
    EnviaByteParaPortaSelecionada(0x7F, Interface);
    
}

void iniciaPortalFrango(void){
    int Contador;

    //testeSinaleiro();
    setaSinaleiro(SINALEIRO_VERMELHO);
    memset(StringDeSaidaParaSerial, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    memset(StringDeSaidaParaSerial, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    DadosParaASerial = 0;
    ContadorDeTagMonitoradas = 0;
    ContadorDeTagMovimentos = 0;
    TagEmTratamento = 0;
    estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
    
    memset(SsidWifi, 0, TAMANHO_MAXIMO_DO_TEXT_SSID);
    memset(SenhaWifi, 0, TAMANHO_MAXIMO_DO_TEXT_SSID);

    
    ObtemID(IdDOLeitor);

    /*
    ModoDeOperacao = OPERACAO_COM_MULTIPLAS_LEITURAS;
    AtrasoParaDegradarLeitura = 0;
    RepeticaoNaLeitura = 1;

    if (NumeroDeAntenasLidas != OPERACAO_COM_QUATRO_ANTENAS){2'
        NumeroDeAntenasLidas = OPERACAO_COM_
     * DUAS_ANTENAS;
    }

    TempoParaInatividadeDeTagMonitorada = TEMPO_PARA_INATIVIDADE_DE_TAG_MONITORADA;
    */

    obtemParamentrosSalvosFrango();
    

    for(Contador = 0;Contador < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;Contador = Contador + 1){
        memset(RegistroDeTagEmPortal[Contador].Epc, 0, TAMANHO_DE_EPC_EM_PORTAL_FRANGO);
        RegistroDeTagEmPortal[Contador].AntenaDaPrimeiraLeitura = 0;
        RegistroDeTagEmPortal[Contador].AntenaDaUltimaLeitura = 0;
        RegistroDeTagEmPortal[Contador].MarcaDePosicaoOcupada = 0;
        RegistroDeTagEmPortal[Contador].TempoParaInatividade = -1;
        RegistroDeTagEmPortal[Contador].ContagemDeEntrada = 0;
        RegistroDeTagEmPortal[Contador].ContagemDeSaida = 0;
        RegistroDeTagEmPortal[Contador].LeuUmaAntena = 0;
        RegistroDeTagEmPortal[Contador].LeuOutroAntena = 0;
        RegistroDeTagEmPortal[Contador].estado = PORTAL_ELIMINADA;
    }

    PausaNasMensagensDeAvaliacao = 0;
    InibeOperacaoEMesagensComOPC = 1;//Forcado nesse nivel para trabalhar na solucao de Juiz de Fora
    //InibeOperacaoEMesagensComOPC = 0;//Operacao normal
    


    if (InibeOperacaoEMesagensComOPC == FALSE){
        enviaParaSerialOsParametrosAJustados(1);
    }
    
    sintonizaCapacitores(&mainTuner, Capacitores[0], Capacitores[1], Capacitores[2]);
    
    
    PodeEnviarTabelaDeInvisiveis = 0;

    //enviaEpcParaDeixarDeSerInvisivelNoPortalDeExclusao((unsigned char *)"123");//Apenas para teste


    //sel_antena (1);
    //callAutoTuner();
    //as3993SingleWrite(AS3993_REG_REGULATORCONTROL, 0xBF); //0x1B
    //as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, 0xEF);

    //liga_buzzer();
    ligarBuzzer();
    _LATG13 = 0;
    delay_ms(1000);
    //desliga_buzzer();
    desligarBuzzer();
}


void reiniciaPortalFrango(void){
    int Contador;
    memset(StringDeSaidaParaSerial, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    memset(StringDeSaidaParaSerial, 0, TAMANHO_MAXIMO_DE_STRING_DE_SAIDA);
    DadosParaASerial = 0;
    ContadorDeTagMonitoradas = 0;
    ContadorDeTagMovimentos = 0;
    TagEmTratamento = 0;
    estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;

    for(Contador = 0;Contador < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;Contador = Contador + 1){
        memset(RegistroDeTagEmPortal[Contador].Epc, 0, TAMANHO_DE_EPC_EM_PORTAL_FRANGO);
        RegistroDeTagEmPortal[Contador].AntenaDaPrimeiraLeitura = 0;
        RegistroDeTagEmPortal[Contador].AntenaDaUltimaLeitura = 0;
        RegistroDeTagEmPortal[Contador].LeuUmaAntena = 0;
        RegistroDeTagEmPortal[Contador].LeuOutroAntena = 0;
        RegistroDeTagEmPortal[Contador].MarcaDePosicaoOcupada = 0;
        RegistroDeTagEmPortal[Contador].TempoParaInatividade = -1;
        RegistroDeTagEmPortal[Contador].ContagemDeEntrada = 0;
        RegistroDeTagEmPortal[Contador].ContagemDeSaida = 0;
        RegistroDeTagEmPortal[Contador].estado = PORTAL_ELIMINADA;
    }
    //liga_buzzer();

    ContadorParaEnviarTabelaDeTagsVisiveis = 0;
    ContadorParaEnviarTabelaDeTagsInisiveis = TEMPO_PARA_ENVIAR_TABELA;

    ligarBuzzer();
    delay_ms(1000);
    //desliga_buzzer();
    desligarBuzzer();
}


unsigned char gerarCheckSum (char *Dados, int Tamanho){
    unsigned char Resultado;
    int Contador;
    Resultado = 0;

    for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        Resultado = Resultado + *Dados;
        Dados = Dados + 1;
    }

    return Resultado;
}

void enviaTagLidaParaAvaliacao(u8 *data, int Tamanho, int Antena, char Estado){
    int Contador;
    char Rascunho[30];
    memset(Rascunho, 0, 30);
    Rascunho[POSICAO_DO_COMANDO_PIC_PC_FRANGO] = TAG_PARA_AVALIACAO_FRANGO;

    Rascunho[POSICAO_DO_TAMANHO_DA_CARGA_PIC_PC_FRANGO] = Tamanho + 2;
    Rascunho[POSICAO_DA_ANTENA_DA_LEITURA_PIC_PC_FRANGO] = (char)Antena;


    for (Contador = POSICAO_DO_EPC_PARA_AVALIACAO_PIC_PC_FRANGO;Contador < 
            (Tamanho + POSICAO_DO_EPC_PARA_AVALIACAO_PIC_PC_FRANGO);Contador = Contador + 1){
        Rascunho[Contador] = *data;
        data = data + 1;
    }

    Rascunho[Contador] = Estado;
    Contador = Contador + 1;
    //Rascunho[POSICAO_DO_CHECKSUM_PARA_AVALIACAO_PIC_PC_FRANGO] =
            //gerarCheckSum(Rascunho, POSICAO_DO_CHECKSUM_PARA_AVALIACAO_PIC_PC_FRANGO);
    Rascunho[Contador] =
            gerarCheckSum(Rascunho, Contador);
    Contador = Contador + 1;
    
    if (PausaNasMensagensDeAvaliacao == 0){
        if (InibeOperacaoEMesagensComOPC == 0){
            EnviaArrayDeBytesPara485(Rascunho, Contador);
        }
    }
}


int procuraPosicaoLivreNosRegistrosDeTag (void){
   int contador;

   if(ContadorDeTagMonitoradas >= MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL){
       return -1;
   }

    for (contador = 0;contador < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;contador = contador + 1){
        if (RegistroDeTagEmPortal[contador].MarcaDePosicaoOcupada == 0){
            return contador;
        }
    }
    return -1;
}

int tagJaPresenteNosRegistros (unsigned char * Tag, int Tamanho){
    int contador;

    if (Tamanho > TAMANHO_DE_EPC_EM_PORTAL_FRANGO){
        return  -1;
    }

    for (contador = 0;contador < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;contador = contador + 1){
        if (memcmp (&RegistroDeTagEmPortal[contador].Epc[0], Tag, Tamanho) == 0)
            return contador;
    };

    return -1;
}

int AtualizaEstadoDaTag(int Posicao, int Antena){
    
    if (RegistroDeTagEmPortal[Posicao].LeuUmaAntena != Antena){
        //RegistroDeTagEmPortal[Posicao].LeuOutroAntena = RegistroDeTagEmPortal[Posicao].LeuUmaAntena;
        RegistroDeTagEmPortal[Posicao].LeuOutroAntena = Antena;
    }
    
    if (Antena == ANTENA_DE_SAIDA){
        RegistroDeTagEmPortal[Posicao].ContagemDeSaida = 1;
    }

    if (Antena == ANTENA_DE_ENTRADA){
        RegistroDeTagEmPortal[Posicao].ContagemDeEntrada = 1;
    }

    if (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada != 0 &&
            RegistroDeTagEmPortal[Posicao].ContagemDeSaida == 0){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_ENTRADA;
        return 0;
    }

    if (RegistroDeTagEmPortal[Posicao].ContagemDeSaida != 0 &&
            RegistroDeTagEmPortal[Posicao].ContagemDeEntrada == 0){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_SAIDA;
        return 0;
    }

    if ((Antena == ANTENA_DE_SAIDA) && (RegistroDeTagEmPortal[Posicao].ContagemDeSaida != 0) &&
            (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada != 0)){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_SAINDO;
        RegistroDeTagEmPortal[Posicao].AntenaDaUltimaLeitura = Antena;
        return 0;
    }

    if ((Antena == ANTENA_DE_ENTRADA) && (RegistroDeTagEmPortal[Posicao].ContagemDeSaida != 0) &&
            (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada != 0)){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_ENTRANDO;
        RegistroDeTagEmPortal[Posicao].AntenaDaUltimaLeitura = Antena;
        return 0;
    }


    return 0;
}


void registraTagDePortal (unsigned char *Tag, int Posicao, int Antena, int AntenaReal){
    int Contador;
    for (Contador = 0;Contador < TAMANHO_DE_EPC_EM_PORTAL_FRANGO;Contador = Contador + 1){
        RegistroDeTagEmPortal[Posicao].Epc[Contador] = *Tag;
        Tag = Tag + 1;
    }
    RegistroDeTagEmPortal[Posicao].TamanhoDoEpc = TAMANHO_DE_EPC_EM_PORTAL_FRANGO;
    RegistroDeTagEmPortal[Posicao].AntenaDaPrimeiraLeitura = Antena;
    RegistroDeTagEmPortal[Posicao].AntenaDaUltimaLeitura = SEM_REGISTRO_DE_ANTENA;
    //RegistroDeTagEmPortal[Posicao].TempoParaInatividade = TEMPO_PARA_INATIVIDADE_DE_TAG_MONITORADA;
    //RegistroDeTagEmPortal[Posicao].TempoParaInatividade = TempoParaInatividadeDeTagMonitorada;
    RegistroDeTagEmPortal[Posicao].TempoParaInatividade = tempoParaInatividadeDaTagMonitorada;
    RegistroDeTagEmPortal[Posicao].MarcaDePosicaoOcupada = 1;
    ContadorDeTagMonitoradas = ContadorDeTagMonitoradas + 1;
    
    RegistroDeTagEmPortal[Posicao].LeuUmaAntena = Antena;
    RegistroDeTagEmPortal[Posicao].LeuOutroAntena = SEM_REGISTRO_DE_ANTENA;

    /*
    if (Antena == 1){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_ENTRADA;
        if (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada < LIMITE_DE_INCREMENTO_DE_PORTAL){
            RegistroDeTagEmPortal[Posicao].ContagemDeEntrada =
                    RegistroDeTagEmPortal[Posicao].ContagemDeEntrada + 1;
        }

        if (RegistroDeTagEmPortal[Posicao].ContagemDeSaida > 0){
            RegistroDeTagEmPortal[Posicao].ContagemDeSaida =
                    RegistroDeTagEmPortal[Posicao].ContagemDeSaida - DECREMENTO_DE_PORTAL;
            if (RegistroDeTagEmPortal[Posicao].ContagemDeSaida < 0){
                RegistroDeTagEmPortal[Posicao].ContagemDeSaida = 0;
            }

        }

    }
    */

    /*
    if (Antena == 2){
        RegistroDeTagEmPortal[Posicao].estado = PORTAL_SAIDA;
        if (RegistroDeTagEmPortal[Posicao].ContagemDeSaida < LIMITE_DE_INCREMENTO_DE_PORTAL){
            RegistroDeTagEmPortal[Posicao].ContagemDeSaida =
                    RegistroDeTagEmPortal[Posicao].ContagemDeSaida + 1;
        }

        if (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada > 0){
            RegistroDeTagEmPortal[Posicao].ContagemDeEntrada =
                    RegistroDeTagEmPortal[Posicao].ContagemDeEntrada - DECREMENTO_DE_PORTAL;
            if (RegistroDeTagEmPortal[Posicao].ContagemDeEntrada < 0){
                RegistroDeTagEmPortal[Posicao].ContagemDeEntrada = 0;
            }
        }

    }
    */
    AtualizaEstadoDaTag(Posicao, Antena);

    enviaTagLidaParaAvaliacao(RegistroDeTagEmPortal[Posicao].Epc,
            TAMANHO_DE_EPC_EM_PORTAL_FRANGO, AntenaReal,
            RegistroDeTagEmPortal[Posicao].estado);
}

void registraAUltimaAntenaQueATagFoiLida(int Posicao, int Antena, int AntenaReal){
    RegistroDeTagEmPortal[Posicao].AntenaDaUltimaLeitura = Antena;
    if (RegistroDeTagEmPortal[Posicao].LeuUmaAntena != Antena){
        RegistroDeTagEmPortal[Posicao].LeuOutroAntena = Antena;
    }
    
    //RegistroDeTagEmPortal[Posicao].TempoParaInatividade = TEMPO_PARA_INATIVIDADE_DE_TAG_MONITORADA;
    //RegistroDeTagEmPortal[Posicao].TempoParaInatividade = TempoParaInatividadeDeTagMonitorada;
    RegistroDeTagEmPortal[Posicao].TempoParaInatividade = tempoParaInatividadeDaTagMonitorada;

    AtualizaEstadoDaTag(Posicao, Antena);

    enviaTagLidaParaAvaliacao(RegistroDeTagEmPortal[Posicao].Epc,
            TAMANHO_DE_EPC_EM_PORTAL_FRANGO, AntenaReal,
            RegistroDeTagEmPortal[Posicao].estado);

}

int TrataTagDePortalFrango (unsigned char *Tag, int Antena, int AntenaReal){
    int Posicao;


    Posicao = tagJaPresenteNosRegistros(Tag, TAMANHO_DE_EPC_EM_PORTAL_FRANGO);
    if (Posicao < 0){
        Posicao = procuraPosicaoLivreNosRegistrosDeTag();
        if (Posicao < 0){
            return -1;
        }
        registraTagDePortal(Tag, Posicao, Antena, AntenaReal);
    } else {
        registraAUltimaAntenaQueATagFoiLida(Posicao, Antena, AntenaReal);
    }

    enviaTAgParaAvaliacaoParaWiegand(Tag, AntenaReal);

    return 0;
}



void eliminaRegistroDeTag(int Posicao){
    //memset(Tag.Epc, 0, TAMANHO_DE_EPC_EM_PORTAL_FRANGO);
    memset(RegistroDeTagEmPortal[Posicao].Epc, 0, TAMANHO_DE_EPC_EM_PORTAL_FRANGO);

    //Tag.AntenaDaPrimeiraLeitura = SEM_REGISTRO_DE_ANTENA;
    RegistroDeTagEmPortal[Posicao].AntenaDaPrimeiraLeitura = SEM_REGISTRO_DE_ANTENA;

    //Tag.AntenaDaUltimaLeitura = SEM_REGISTRO_DE_ANTENA;
    RegistroDeTagEmPortal[Posicao].AntenaDaUltimaLeitura = SEM_REGISTRO_DE_ANTENA;
    //Tag.TempoParaInatividade = -1;
    RegistroDeTagEmPortal[Posicao].TempoParaInatividade = -1;
    //Tag.MarcaDePosicaoOcupada = 0;
    RegistroDeTagEmPortal[Posicao].MarcaDePosicaoOcupada = 0;
    if (ContadorDeTagMonitoradas > 0){
        ContadorDeTagMonitoradas = ContadorDeTagMonitoradas - 1;
    }
    
    RegistroDeTagEmPortal[Posicao].LeuUmaAntena = SEM_REGISTRO_DE_ANTENA;
    RegistroDeTagEmPortal[Posicao].LeuOutroAntena = SEM_REGISTRO_DE_ANTENA;
}

void geraStringDeArrayDeBytes (char * String, unsigned char *Array, int Tamanho){
    int Contador;
    for(Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        String += sprintf(String, "%02X", Array[Contador]);
    }
}


extern char NomeDoPortal[2];

void enviaStringDoRegistroDeMovimento (TipoRegistroDeTagEmPortal Tag){
    int Contador;
    char rascunho[30];
    char mensagem[100];
    rascunho[POSICAO_DO_COMANDO_PIC_PC_FRANGO] = TAG_DE_MOVIMENTO_FRANGO;
    rascunho[POSICAO_DO_TAMANHO_DA_CARGA_PIC_PC_FRANGO] = Tag.TamanhoDoEpc + 2;

    for (Contador = 0;Contador < TAMANHO_DE_EPC_EM_PORTAL_FRANGO;Contador = Contador + 1){
        rascunho[POSICAO_DO_EPC_EM_MOVIMENTO_PIC_PC_FRANGO + Contador] = Tag.Epc[Contador];
    }
    
    rascunho[POSICAO_DA_ANTENA_DA_ULTIMA_LEITURA_PIC_PC_FRANGO] = Tag.AntenaDaUltimaLeitura;
    
    rascunho[POSICAO_DO_ESTADO_PARA_MOVIMENTO_PIC_PC_FRANGO] = Tag.estado;
    rascunho[POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO] =
            gerarCheckSum(rascunho, POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO);
    if (PausaNasMensagensDeAvaliacao == 0){
        if (InibeOperacaoEMesagensComOPC == 0){
            EnviaArrayDeBytesPara485(rascunho, POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO);//5
        }
    }
    enviaMovimentoParaWiegand(Tag.Epc, Tag.estado, Tag.AntenaDaUltimaLeitura);
    dataHora = retornaTimeInfo();
    if (Tag.AntenaDaUltimaLeitura == ANTENA_DE_SAIDA){
        //enviaEpcParaSerInvisivelNoPortalDeExclusao(Tag.Epc, retornaComEpoch());
        //enviaEpcParaSerInvisivelNoPortalDeExclusaoNa485(Tag.Epc, retornaComEpoch());
        //enviaEventoDeQueUmPedestreSaiuNaAreaSeguraNa485(Tag.Epc, retornaComEpoch());
        sprintf(mensagem, "<FF;%c%c%c%c;S;%02x%02x%02x00;%02d%02d20%02d;%02d%02d%02d>", idDoLeitor[0], idDoLeitor[1], idDoLeitor[2], idDoLeitor[3], Tag.Epc[0], Tag.Epc[1], Tag.Epc[2], dataHora->tm_mday, dataHora->tm_mon + 1, dataHora->tm_year - 100, dataHora->tm_hour, dataHora->tm_min, dataHora->tm_sec);
        enviaDadosParaEthPortais(mensagem, strlen(mensagem));
        //adicionaItemNaTabelaDeInvisiveis(Tag.Epc);
        //(void)adicionaElementoDaTabelaDeExclusao (Tag.Epc, SAIU_NA_AREA_SEGURA);
        (void)adicionaElementoDaTabelaDeExclusao (Tag.Epc, NomeDoPortal[0], NomeDoPortal[1], SAIU_NA_AREA_SEGURA);
    } else {
        //enviaEpcParaDeixarDeSerInvisivelNoPortalDeExclusao(Tag.Epc, retornaComEpoch());
        //enviaEpcParaDeixarDeSerInvisivelNoPortalDeExclusaoPara485(Tag.Epc, retornaComEpoch());
        //enviaEventoDeQueUmPedestreEntrouNaAreaSeguraNa485(Tag.Epc, retornaComEpoch());
        sprintf(mensagem, "<FF;%c%c%c%c;E;%02x%02x%02x00;%02d%02d20%02d;%02d%02d%02d>", idDoLeitor[0], idDoLeitor[1], idDoLeitor[2], idDoLeitor[3], Tag.Epc[0], Tag.Epc[1], Tag.Epc[2], dataHora->tm_mday, dataHora->tm_mon + 1, dataHora->tm_year - 100, dataHora->tm_hour, dataHora->tm_min, dataHora->tm_sec);
        enviaDadosParaEthPortais(mensagem, strlen(mensagem));
        //adicionaItemNaTabelaDeVisiveis(Tag.Epc);
        (void)adicionaElementoDaTabelaDeExclusao (Tag.Epc, NomeDoPortal[0], NomeDoPortal[1], ENTROU_NA_AREA_SEGURA);
    }
    
    
    rascunho[POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO] = 0xFF;
    setaSinaleiro(SINALEIRO_VERDE);
    destravaCancelaDoPortal();

}

//void geraStringDoRegistroDeMovimento (TipoRegistroDeTagEmPortal Tag, char *StringFinal){
void geraStringDoRegistroDeMovimento (TipoRegistroDeTagEmPortal Tag){
    int Contador;
    char rascunho[30];
    memset(rascunho, 0, 30);
    int GerouEvento;
    GerouEvento = 0;
    
    if ((Tag.AntenaDaPrimeiraLeitura ==  ANTENA_DE_ENTRADA) &&
            (Tag.AntenaDaUltimaLeitura == ANTENA_DE_SAIDA)){
        Tag.estado = PORTAL_SAIU;
        enviaStringDoRegistroDeMovimento(Tag);
        GerouEvento = 1;
    } else {
        if ((Tag.AntenaDaPrimeiraLeitura ==  ANTENA_DE_ENTRADA) &&
                (Tag.estado == PORTAL_SAINDO)){
            Tag.estado = PORTAL_SAIU;
            enviaStringDoRegistroDeMovimento(Tag);
            GerouEvento = 1;
        }
    }

    if ((Tag.AntenaDaPrimeiraLeitura ==  ANTENA_DE_SAIDA) &&
            (Tag.AntenaDaUltimaLeitura == ANTENA_DE_ENTRADA)){
        Tag.estado = PORTAL_ENTROU;
        enviaStringDoRegistroDeMovimento(Tag);
        GerouEvento = 1;
    } else {
        if ((Tag.AntenaDaPrimeiraLeitura ==  ANTENA_DE_SAIDA) &&
                (Tag.estado == PORTAL_ENTRANDO)){
            Tag.estado = PORTAL_ENTROU;
            enviaStringDoRegistroDeMovimento(Tag);
            GerouEvento = 1;
        }
    }

    /*
    rascunho[POSICAO_DO_ESTADO_PARA_MOVIMENTO_PIC_PC_FRANGO] = Tag.estado;

    switch(Tag.AntenaDaUltimaLeitura){
        ContadorDeTagMovimentos = ContadorDeTagMovimentos + 1;
        case ANTENA_DE_ENTRADA:
            if (Tag.AntenaDaPrimeiraLeitura == ANTENA_DE_SAIDA){
                rascunho[POSICAO_DA_ANTENA_DA_ULTIMA_LEITURA_PIC_PC_FRANGO] = 1;
            }
            break;
        case ANTENA_DE_SAIDA:
            if (Tag.AntenaDaPrimeiraLeitura == ANTENA_DE_ENTRADA){
                rascunho[POSICAO_DA_ANTENA_DA_ULTIMA_LEITURA_PIC_PC_FRANGO] = 2;
            }
            break;
        default:
            //(void)sprintf(StringFinal, "Tag,%s,Foi e lida e ocorreu algum erro\n",(char *)SubStringDeEPC);
            break;
    }
    */
    
    //if (rascunho[POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO] == 0){
    if (GerouEvento == 0){
        rascunho[POSICAO_DO_COMANDO_PIC_PC_FRANGO] = TAG_DE_MOVIMENTO_CANCELADO;
        rascunho[POSICAO_DA_ANTENA_DA_LEITURA_PIC_PC_FRANGO] = 0xFF;
        rascunho[POSICAO_DO_TAMANHO_DA_CARGA_PIC_PC_FRANGO] = Tag.TamanhoDoEpc + 2;
        for (Contador = 0;Contador < TAMANHO_DE_EPC_EM_PORTAL_FRANGO;Contador = Contador + 1){
            rascunho[POSICAO_DO_EPC_EM_MOVIMENTO_PIC_PC_FRANGO + Contador] = Tag.Epc[Contador];
        }
        rascunho[POSICAO_DO_ESTADO_PARA_MOVIMENTO_PIC_PC_FRANGO] = 0;
        rascunho[POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO] =
                gerarCheckSum(rascunho, POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO);
        if (PausaNasMensagensDeAvaliacao == 0){
            if (InibeOperacaoEMesagensComOPC == 0){
#ifdef PORTAL_COM_ETH
                enviaDadosParaEthPortais(rascunho, POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO);
#else 
                EnviaArrayDeBytesPara485(rascunho, POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO);
#endif 
            }
        }
       
        enviaMovimentoParaWiegand(Tag.Epc, Tag.estado, Tag.AntenaDaUltimaLeitura);
        
        setaSinaleiro(SINALEIRO_VERMELHO);
        //setaSinaleiro(SINALEIRO_CANCELADO);
        
        
        if (Tag.LeuOutroAntena != SEM_REGISTRO_DE_ANTENA || GerouEvento == 0){
            acionaBeepDeCancelamento();
            travaCancelaDoPortal();
        }
        
        //acionaBeepDeCancelamento();
        
    }
}

int DadosASeremEnviadosPelaSerialDoPortalFrango(void){
    return DadosParaASerial;
}

void DadosDaSerialDoPortalTratadosFrango(void){
    DadosParaASerial = 0;
}

int DadosEstaoSendoEnviadosAoPC(void){
    return SerialOcupada;
}

void ComecouAEnviarDadosAoPC(void){
    SerialOcupada = 1;
}

void ParouDeEnviarDadosAoPC(void){
    SerialOcupada = 0;
}


void LidaComEstadosDaTagEmTick(void){
    if (RegistroDeTagEmPortal[TagEmTratamento].TempoParaInatividade == 0){
        if (RegistroDeTagEmPortal[TagEmTratamento].estado == PORTAL_SAINDO){
            RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_SAIU;
        }
    }

    if (RegistroDeTagEmPortal[TagEmTratamento].TempoParaInatividade == 0){
        if (RegistroDeTagEmPortal[TagEmTratamento].estado == PORTAL_ENTRANDO){
            RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_ENTROU;
        }
    }

    /*
    if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeEntrada == 0){
        if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeSaida != 0){
            RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_SAIDA;
        }
    }

    if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeEntrada <
            RegistroDeTagEmPortal[TagEmTratamento].ContagemDeSaida){
        RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_SAINDO;
    }

    if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeSaida == 0){
        if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeEntrada != 0){
            RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_ENTRADA;
        }
    }

    if (RegistroDeTagEmPortal[TagEmTratamento].ContagemDeEntrada >
            RegistroDeTagEmPortal[TagEmTratamento].ContagemDeSaida){
        RegistroDeTagEmPortal[TagEmTratamento].estado = PORTAL_ENTRANDO;
    }
    */
}

void acoesEmCodigoCorrentePortalFrango(void){
    
    for (TagEmTratamento = 0;TagEmTratamento < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;TagEmTratamento = TagEmTratamento + 1){
    //if (TagEmTratamento < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL){
        //TagEmTratamento = TagEmTratamento + 1;
        LidaComEstadosDaTagEmTick();

        if (RegistroDeTagEmPortal[TagEmTratamento].TempoParaInatividade == 0){
            //*********************************************************8
            //Nesse ponto eh aonde o desempenho eh degradado
            //geraStringDoRegistroDeMovimento(RegistroDeTagEmPortal[TagEmTratamento], StringDeSaidaParaSerial);
            geraStringDoRegistroDeMovimento(RegistroDeTagEmPortal[TagEmTratamento]);
            //*********************************************************8

            DadosParaASerial = 1;
            if (RegistroDeTagEmPortal[TagEmTratamento].Epc[0] != 0){
                eliminaRegistroDeTag(TagEmTratamento);
            }            
        }
    }     
}

void acoesEmTickParaPortalFrango(void){
   /* int TempoDeSilencioAtualNoZigBee;
    if (ContadorDeSilencioNa485){
        ContadorDeSilencioNa485 =  ContadorDeSilencioNa485 - 1;
    }
    
    TempoDeSilencioAtualNoZigBee = tempoDeSilencioNoZigBee();
    if (TempoDeSilencioAtualNoZigBee >= (TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE - 1)){
        PodeEnviarTabelaDeInvisiveis = 1;
    }
*/
}

void acoesEmTickACadaSegundoParaPortalFrango(void){
    /*
    if (PausaNasMensagensDeAvaliacao != 0){
        if (InibeOperacaoEMesagensComOPC == 0){
            PausaNasMensagensDeAvaliacao = PausaNasMensagensDeAvaliacao - 1;
        }
    }
    */
    int Posicao;
    for(Posicao = 0;Posicao < MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL;Posicao = Posicao + 1){
        if (RegistroDeTagEmPortal[Posicao].TempoParaInatividade > 0){
            RegistroDeTagEmPortal[Posicao].TempoParaInatividade = RegistroDeTagEmPortal[Posicao].TempoParaInatividade - 1;
        }
    }

    if (PortaDePortalDeExclusao != 0){
        if(alarmeFaltaDeRedeEthernet == 0){
            atualizarSinaleiro();
            atulizaBeepDeCancelamento();
        }
        
    }
    /*
    if (ContagemParaExpirarOsComandosDoPC > 0){
        ContagemParaExpirarOsComandosDoPC = ContagemParaExpirarOsComandosDoPC - 1;
        if (ContagemParaExpirarOsComandosDoPC == 1){
            iniciaEstruturaDoProtocoloPC();
        }
    }
    */

}

void setaSensibilidade (unsigned char valor){
    //signed char sensibilidadeAtual, resultado;
    signed char rascunho = 0;
    SensibilidadeDaAntena = valor;
    rascunho = rascunho - valor;
    //sensibilidadeAtual = as3993GetSensitivity();
    //if (sensibilidadeAtual != rascunho){
      //resultado = as3993SetSensitivity(rascunho);
    //}
    as3993SetSensitivity(rascunho);
}

void setaNumeroDeAntenas (int valor){
    if ((valor > 0) && (valor < 8)){
        NumeroDeAntenasLidas = valor;
    }
}

void setaAtrasoNaLeitura (int valor){
    AtrasoParaDegradarLeitura = valor;
}

void setaRepeticaoDaLeitura (int valor){
    RepeticaoNaLeitura = valor;
}

extern int EuSouOAlphaDosPortais;



void repetirOComando(void){
    /*
    if (EuSouOAlphaDosPortais == 1){
        //Pede para quem enviou o movimento o fazer novamente
        _LATG13 = 1;
        delay_us(10);
        uart1Tx(0xFE);
        //delay_us (120);
        //delay_ms (4);
        uart1Tx(0x04);
        //delay_us (120);
        //delay_ms (4);
        uart1Tx(0x55);
        //delay_us (120);
        //delay_ms (4);
        uart1Tx(0x58);
        //delay_us (120);
        //delay_ms (4);

        _LATG13 = 0;            
    }
    */
}

void comandoRecebidoNa485(void){
    /*
    if (EuSouOAlphaDosPortais == 1){
        //Pede para quem enviou o movimento o fazer novamente
        _LATG13 = 1;
        uart1Tx(0xFD);
        //delay_ms (4);
        //delay_us (500);
        uart1Tx(0x04);
        //delay_ms (4);
        //delay_us (500);
        uart1Tx(0xAA);
        //delay_ms (4);
        //delay_us (500);
        uart1Tx(0xAC);
        //delay_ms (4);
        //delay_us (500);

        _LATG13 = 0;            
    }
    */
}

void reenvioDeItemDaListaDeSaida(void){
    /*
    unsigned char Saida[10];
    int Contador;
    
    if (recuperaItemDaListaDeSaida(Saida) >= 0){
        
        aguradaSilencioNa485();

        _LATG13 = 1;
        delay_us(10);
        for (Contador = 0;Contador < 8; Contador = Contador + 1){
            uart1Tx(Saida[Contador]);
            //delay_us (120);
            //delay_ms (4);
            //delay_us (500);
        }
        _LATG13 = 0; 
        
    }
    */
}


void reenvioDeItemDaListaDeSaidaPorEstouDeTempo(void){
    /*
    unsigned char Saida[10];
    int Contador;
    
    if (ContadorDeReenvioDeDadosDaListaDeSaida == 0){
        ContadorDeReenvioDeDadosDaListaDeSaida = TEMPO_PARA_REENVIO_DA_LISTA_DE_SAIDA;
        if (recuperaItemDaListaDeSaida(Saida) >= 0){
            _LATG13 = 1;
            for (Contador = 0;Contador < 8; Contador = Contador + 1){
                uart1Tx(Saida[Contador]);
                //delay_us (120);
                //delay_ms (4);
                //delay_us (500);
            }
            _LATG13 = 0; 
        }
    }
    */
}


#define TAMANHO_MAXIMO_DE_TEXT_DE_PARAMENTRO 60
void lidaComComandoDOPCPrototipo(unsigned char Dado, int Interface){
    int rascunho;
    //int contador;
    static int QuantiaDeDadosAReceber;
    static int ContagemDeRecepcao;
    static char ParametroEmTexto[TAMANHO_MAXIMO_DE_TEXT_DE_PARAMENTRO];
    static unsigned long Epoch;
    switch(estadoSerialFrango){
        case ESPERANDO_COMANDO_SERIAL_FRANGO:
            ComandoObtidoPorUmPC.Comando = Dado;
            ComandoObtidoPorUmPC.InterfaceUsada = Interface;
            ComandoObtidoPorUmPC.TamanhoDoPacote = 1;
            memset(ComandoObtidoPorUmPC.Reserva, 0, 50);
            ComandoObtidoPorUmPC.PodeTratar = -1;
            
            if ((Dado == 5) || (Dado == 15) || (Dado == 30)/* || (dado == 60)*/){
                TempoParaInatividadeDeTagMonitorada = Dado;
                TempoParaInatividadeDeTagMonitorada = TempoParaInatividadeDeTagMonitorada * 1000;
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
            }

            if (Dado == 0x55){
                reiniciaPortalFrango();
            }

            if (Dado == 0x56){
                ComandoObtidoPorUmPC.PodeTratar = 0;
            }
            
            
            if (Dado == 0xAA){
                enviarID();
            }

            if (Dado == 0xAB){
                PausaNasMensagensDeAvaliacao = 2;
            }


            if (Dado == 0xAC){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }
            
            if ((Dado == 0xAD) || (Dado == 0xAE)){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }
            
            if (Dado == 0xAF){
                //ComandoDaSerial = Dado;
                //estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
                //enviaFluxoPorPortaSelecionada(MeuIpWifi, strlen(MeuIpWifi), Interface);
                acionaBeepDeComandoFrango(1);
            }
            
            if (Dado == 0xB0){
                ComandoObtidoPorUmPC.PodeTratar = 0;
            }
            
            if (Dado == 0xB1){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }

            if (Dado == 0xB2){
                ComandoObtidoPorUmPC.PodeTratar = 0;
            }
            
            if (Dado == 0xB3){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }
           
            if (Dado == 0xB4){
                ComandoObtidoPorUmPC.PodeTratar = 0;
            }
            
            
            rascunho = Dado & 0xF0;
            
            if (rascunho == 0x80){
                
                rascunho = Dado & 0x0F;
                if (rascunho < 9){
                    NumeroDeAntenasLidas = rascunho;
                    salvaParametrosFrango();
                    //for(contador = 0;contador < rascunho;contador = contador + 1){
                    acionaBeepDeComandoFrango(1);
                    //}
                }
                
            }
            
            if (Dado == 0x89){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }

            if (Dado == 0x8A){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }

            if (Dado == 0x8B){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }

            if (Dado == 0x8C){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }
            
            if (Dado == 0x8D){
                enviaParaSerialOsParametrosAJustados(Interface);
                acionaBeepDeComandoFrango(1);                
            }

            if (Dado == 0x8E){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;
            }
            
            if (Dado == 0x8F){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;                
            }
            
            
            if (Dado == 0x91){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;                
            }

            if (Dado == 0x92){
                ComandoDaSerial = Dado;
                estadoSerialFrango = ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO;                
            }


            
            break;
        case ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO:
            PrimeiroParametroDaSerial = Dado;
            if  (ComandoDaSerial == 0x89){
                setaSensibilidade(Dado);
                salvaParametrosFrango();
                
                acionaBeepDeComandoFrango(1);                
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }
            
            
            if  (ComandoDaSerial == 0x91){
                Capacitores[0] = Dado;
                estadoSerialFrango = ESPERANDO_SEGUNDO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
            
            if (ComandoDaSerial == 0x8A){//Leitura unica ou multiplica
                if (Dado == 0){
                    ModoDeOperacao = OPERACAO_COM_LEITURA_UNICA;
                }
                        
                if (Dado == 1){
                    ModoDeOperacao = OPERACAO_COM_MULTIPLAS_LEITURAS;
                }

                if (Dado == 2){
                    ModoDeOperacao = OPERACAO_LEITURAS_INITERRUPTAS;
                }
                
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);                
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }


            if (ComandoDaSerial == 0x8B){//Atraso
                setaAtrasoNaLeitura(Dado);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }

            if (ComandoDaSerial == 0x8C){//Varredura na mesma antena
                setaRepeticaoDaLeitura(Dado);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);                
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }
            
            if (ComandoDaSerial == 0x8E){//Wiegand
                if (Dado < FINAL_WIEGAND){
                    ModoWieGand = Dado;
                    salvaParametrosFrango();
                    acionaBeepDeComandoFrango(1);
                }                
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }

            if (ComandoDaSerial == 0x92){//Frequencia traseira
                if ((Dado < 28) || (Dado == PARAMETRO_SETA_FREQUENCIA_ANATEL)){
                    FrequenciaTraseira = Dado;
                    salvaParametrosFrango();
                    SetaFrequenciaTraseira();
                    acionaBeepDeComandoFrango(1);                                    
                }
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }

            if (ComandoDaSerial == 0x8F){//Frequencia
                if ((Dado < 28) || (Dado == PARAMETRO_SETA_FREQUENCIA_ANATEL)){
                    Frequencia = Dado;
                    salvaParametrosFrango();
                    SetaFrequencias();
                    acionaBeepDeComandoFrango(1);                                    
                }
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            }
            
            
            
            if (ComandoDaSerial == 0xAC){//Hora e data
                Epoch = Dado;
                estadoSerialFrango = ESPERANDO_SEGUNDO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
            
            if ((ComandoDaSerial == 0xAD) || (ComandoDaSerial == 0xAE) || (ComandoDaSerial == 0xB1) ||
                    (ComandoDaSerial == 0xB3)){
                QuantiaDeDadosAReceber = Dado;
                if (QuantiaDeDadosAReceber < TAMANHO_MAXIMO_DE_TEXT_DE_PARAMENTRO){
                    ContagemDeRecepcao = 0;
                    estadoSerialFrango = ESPERANDO_SEGUNDO_PARAMENTRO_SERIAL_FRANGO;
                    memset(ParametroEmTexto, 0, TAMANHO_MAXIMO_DE_TEXT_DE_PARAMENTRO);
                } else {
                    estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                }
                break;
            }
           
            break;
        case ESPERANDO_SEGUNDO_PARAMENTRO_SERIAL_FRANGO:
            if  (ComandoDaSerial == 0x91){
                Capacitores[1] = Dado;
                estadoSerialFrango = ESPERANDO_TERCEIRO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
            
            if ((ComandoDaSerial == 0xB1) || (ComandoDaSerial == 0xB3)){
                if ((QuantiaDeDadosAReceber > ContagemDeRecepcao)){
                    ParametroEmTexto[ContagemDeRecepcao] = Dado;
                    ContagemDeRecepcao = ContagemDeRecepcao + 1;
                    break;
                } else {
                    ComandoObtidoPorUmPC.TamanhoDoPacote = QuantiaDeDadosAReceber;
                    memcpy(ComandoObtidoPorUmPC.Reserva, ParametroEmTexto, QuantiaDeDadosAReceber);
                    ComandoObtidoPorUmPC.PodeTratar = 0;
                }
            }
            
            
            if ((ComandoDaSerial == 0xAD) || (ComandoDaSerial == 0xAE)){     
                if ((QuantiaDeDadosAReceber > ContagemDeRecepcao)){
                    ParametroEmTexto[ContagemDeRecepcao] = Dado;
                    ContagemDeRecepcao = ContagemDeRecepcao + 1;
                    break;
                } else {
                    if (ComandoDaSerial == 0xAD){
                        salvaSSIDDoWifi (ParametroEmTexto);
                        delay_ms(1000);
                        resetNoWifi();
                        //acionaBeepDeComandoFrango(1);
                        Reset();
                    }
                    if (ComandoDaSerial == 0xAE){
                        salvaSenhaDoWifi (ParametroEmTexto);
                        acionaBeepDeComandoFrango(1);
                        //Reset();
                    }
                    QuantiaDeDadosAReceber = 0;
                    memset(ParametroEmTexto, 0, TAMANHO_MAXIMO_DE_TEXT_DE_PARAMENTRO);
                    estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                    break;
                }
                
            }
                
            
            if (ComandoDaSerial == 0xAC){//Hora e data
                Epoch = Epoch << 8;
                Epoch = Epoch | Dado;
                estadoSerialFrango = ESPERANDO_TERCEIRO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }

           estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
             break;
       case  ESPERANDO_TERCEIRO_PARAMENTRO_SERIAL_FRANGO:
            if  (ComandoDaSerial == 0x91){
                Capacitores[2] = Dado;
                estadoSerialFrango = ESPERANDO_QUARTO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
           
            if (ComandoDaSerial == 0xAC){//Hora e data
                Epoch = Epoch << 8;
                Epoch = Epoch | Dado;
                estadoSerialFrango = ESPERANDO_QUARTO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
           
            estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            break;
            
        case ESPERANDO_QUARTO_PARAMENTRO_SERIAL_FRANGO:
            if  (ComandoDaSerial == 0x91){
                Capacitores[3] = Dado;
                estadoSerialFrango = ESPERANDO_QUINTO_PARAMENTRO_SERIAL_FRANGO;
                break;
                break;
            }
            
            if (ComandoDaSerial == 0xAC){//Hora e data
                Epoch = Epoch << 8;
                Epoch = Epoch | Dado;
                SetaTempoComEpoch(Epoch);
            }
            
            estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            break;
            
        case ESPERANDO_QUINTO_PARAMENTRO_SERIAL_FRANGO:
            if  (ComandoDaSerial == 0x91){
                Capacitores[4] = Dado;
                estadoSerialFrango = ESPERANDO_SEXTO_PARAMENTRO_SERIAL_FRANGO;
                break;
            }
            
            break;
            
        case ESPERANDO_SEXTO_PARAMENTRO_SERIAL_FRANGO:
            if  (ComandoDaSerial == 0x91){
                Capacitores[5] = Dado;
                estadoSerialFrango = ESPERANDO_SEXTO_PARAMENTRO_SERIAL_FRANGO;
                salvaParametrosFrango();
                Reset();
            }
            
            
            
            break;
            
        default:
            estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
            break;
    }

}


#ifdef INTERFACE_DE_AJUSTES
void lidaComComandoDOPC(unsigned char Dado, int Interface){
        lidaComComandoDOPCPrototipo(Dado, Interface);
}
#else
void lidaComComandoDOPC(unsigned char Dado, int Interface){
   /* int rascunho;
    int Resultado;
    //int Contador;
    //unsigned char Saida[TAMANHO_DO_ITEM];
    
    //Resultado = trataRecepcaoNoProcolo (dado, &EstruturaDeRecepcaoDoPC.Estado, &EstruturaDeRecepcaoDoPC.Comando, &EstruturaDeRecepcaoDoPC.Tamanho, &CorpoDeComandoRecebidoDoPC[EstruturaDeRecepcaoDoPC.ContagemDoCorpo], &EstruturaDeRecepcaoDoPC.ContagemDoCorpo, &EstruturaDeRecepcaoDoPC.CheckSum);
    Resultado = trataRecepcaoDoProcoloNoPortal (Dado, &EstruturaDeRecepcaoDoPC.Estado, &EstruturaDeRecepcaoDoPC.Comando, &EstruturaDeRecepcaoDoPC.Tamanho, &CorpoDeComandoRecebidoDoPC[EstruturaDeRecepcaoDoPC.ContagemDoCorpo], &EstruturaDeRecepcaoDoPC.ContagemDoCorpo, &EstruturaDeRecepcaoDoPC.CheckSum);
    //int contador;
    
    //if (Resultado == 1){
    
    if (Resultado == -1){
        if (EuSouOAlphaDosPortais == 1){
            //Pede para quem enviou o movimento o fazer novamente
            
            _LATG13 = 1;
            delay_us(10);
            uart1Tx(0xFE);
            //delay_us (120);
            //delay_ms (4);
            uart1Tx(0x05);
            //delay_us (120);
            //delay_ms (4);
            uart1Tx(0x55);
            //delay_us (120);
            //delay_ms (4);
            uart1Tx(0x58);
            //delay_us (120);
            //delay_ms (4);
            
            _LATG13 = 0;            
             * 
        } else {
            
            if (recuperaItemDaListaDeSaida(Saida) >= 0){
                _LATG13 = 1;
                delay_us(10);
                for (Contador = 0;Contador < 8; Contador = Contador + 1){
                    uart1Tx(Saida[Contador]);
                    //delay_us (120);
                    //delay_ms (4);
                    //delay_us (500);
                }
                _LATG13 = 0; 
            }
            
        }
    }
    //if (Resultado != 0){//Puro desespero
    if (Resultado == 1){//Puro desespero
        switch(EstruturaDeRecepcaoDoPC.Comando){
            case 0xF0:
                TempoParaInatividadeDeTagMonitorada = CorpoDeComandoRecebidoDoPC[0];
                salvaParametrosFrango();
                break;
            case 0x55:
                reiniciaPortalFrango();
                break;
            case 0xAA:
                enviarID();
                break;
                
            case 0xAB:
                PausaNasMensagensDeAvaliacao = 2;
                break;
            case 0x7F:
                PortaDePortalDeExclusao = PrimeiroParametroDaSerial;
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
                
            case 0x80:
                rascunho = CorpoDeComandoRecebidoDoPC[0];
                if (rascunho < 9){
                    NumeroDeAntenasLidas = rascunho;
                    salvaParametrosFrango();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x89:
                setaSensibilidade(Dado);
                salvaParametrosFrango();

                acionaBeepDeComandoFrango(1);
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                break;
            case 0x8A:
                if (Dado == 0){
                    ModoDeOperacao = OPERACAO_COM_LEITURA_UNICA;
                }

                if (Dado == 1){
                    ModoDeOperacao = OPERACAO_COM_MULTIPLAS_LEITURAS;
                }
                
                if (Dado == 2){
                    ModoDeOperacao = OPERACAO_LEITURAS_INITERRUPTAS;
                }

                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8B:
                setaAtrasoNaLeitura(CorpoDeComandoRecebidoDoPC[0]);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8C:
                setaRepeticaoDaLeitura(CorpoDeComandoRecebidoDoPC[0]);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8D:
                enviaParaSerialOsParametrosAJustados(Interface);
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8E:
                if (CorpoDeComandoRecebidoDoPC[0] < FINAL_WIEGAND){
                    ModoWieGand = CorpoDeComandoRecebidoDoPC[0];
                    salvaParametrosFrango();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x8F:
                if ((CorpoDeComandoRecebidoDoPC[0] < 28) || (CorpoDeComandoRecebidoDoPC[0] == PARAMETRO_SETA_FREQUENCIA_ANATEL)){
                    Frequencia = CorpoDeComandoRecebidoDoPC[0];
                    salvaParametrosFrango();
                    SetaFrequencias();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x90:
                CorpoDeComandoRecebidoDoPC[0] = CorpoDeComandoRecebidoDoPC[0] & 0x03;
                CorpoDeComandoRecebidoDoPC[0] = CorpoDeComandoRecebidoDoPC[0] << 2;
                as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, CorpoDeComandoRecebidoDoPC[0]);
                acionaBeepDeComandoFrango(1);
                break;
            case 0x91:
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                sintonizaCapacitores(&mainTuner, CorpoDeComandoRecebidoDoPC[0], CorpoDeComandoRecebidoDoPC[1], CorpoDeComandoRecebidoDoPC[2]);
                Capacitores[0] = CorpoDeComandoRecebidoDoPC[0];
                Capacitores[1] = CorpoDeComandoRecebidoDoPC[1];
                Capacitores[2] = CorpoDeComandoRecebidoDoPC[2];
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x92:
                salvaParametrosFrango();
                InibeOperacaoEMesagensComOPC = CorpoDeComandoRecebidoDoPC[0];
                acionaBeepDeComandoFrango(1);
                break;
                
            case 0xF5:
                ContadorDeSilencioNa485 = TEMPO_DE_SILENCIO_NA_485;
                //adicionaItemNaTabelaDeInvisiveis(CorpoDeComandoRecebidoDoPC);
                //(void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC, ENTROU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC,CorpoDeComandoRecebidoDoPC[3],CorpoDeComandoRecebidoDoPC[4], ENTROU_NA_AREA_SEGURA);
                comandoRecebidoNa485();
                break;
                
            case 0xF6:
                ContadorDeSilencioNa485 = TEMPO_DE_SILENCIO_NA_485;
                //adicionaItemNaTabelaDeVisiveis(CorpoDeComandoRecebidoDoPC);
                //(void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC, SAIU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC,CorpoDeComandoRecebidoDoPC[3],CorpoDeComandoRecebidoDoPC[4], SAIU_NA_AREA_SEGURA);
                comandoRecebidoNa485();
                break;
            case 0xFD:
                RemovePrimeiroItemDaListaDeSaida();
                break;
            case 0xFE:
                reenvioDeItemDaListaDeSaida();
                
                if (recuperaItemDaListaDeSaida(Saida) >= 0){
                    _LATG13 = 1;
                    for (Contador = 0;Contador < 8; Contador = Contador + 1){
                        uart1Tx(Saida[Contador]);
                        //delay_us (120);
                        delay_ms (4);
                        delay_us (500);
                    }
                    _LATG13 = 0; 
                }
                
                break;             
        }
        iniciaEstruturaDoProtocoloPC();
    }   */
}


#endif

void ajustesDoLeitor (unsigned char Dado, int Interface){
   /* int rascunho;
    int Resultado;
    
    Resultado = trataRecepcaoDoProcoloNoPortal (Dado, &EstruturaDeRecepcaoDoPC.Estado, &EstruturaDeRecepcaoDoPC.Comando, &EstruturaDeRecepcaoDoPC.Tamanho, &CorpoDeComandoRecebidoDoPC[EstruturaDeRecepcaoDoPC.ContagemDoCorpo], &EstruturaDeRecepcaoDoPC.ContagemDoCorpo, &EstruturaDeRecepcaoDoPC.CheckSum);
   
    if (Resultado == 1){//Puro desespero
        switch(EstruturaDeRecepcaoDoPC.Comando){
            case 0xF0:
                TempoParaInatividadeDeTagMonitorada = CorpoDeComandoRecebidoDoPC[0];
                salvaParametrosFrango();
                break;
            case 0x55:
                reiniciaPortalFrango();
                break;
            case 0xAA:
                enviarID();
                break;
                
            case 0xAB:
                PausaNasMensagensDeAvaliacao = 2;
                break;
            case 0x7F:
                PortaDePortalDeExclusao = PrimeiroParametroDaSerial;
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x80:
                rascunho = CorpoDeComandoRecebidoDoPC[0];
                if (rascunho < 9){
                    NumeroDeAntenasLidas = rascunho;
                    salvaParametrosFrango();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x89:
                setaSensibilidade(Dado);
                salvaParametrosFrango();

                acionaBeepDeComandoFrango(1);
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                break;
            case 0x8A:
                if (Dado == 0){
                    ModoDeOperacao = OPERACAO_COM_LEITURA_UNICA;
                }

                if (Dado == 1){
                    ModoDeOperacao = OPERACAO_COM_MULTIPLAS_LEITURAS;
                }
                
                if (Dado == 2){
                    ModoDeOperacao = OPERACAO_LEITURAS_INITERRUPTAS;
                }

                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8B:
                setaAtrasoNaLeitura(CorpoDeComandoRecebidoDoPC[0]);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8C:
                setaRepeticaoDaLeitura(CorpoDeComandoRecebidoDoPC[0]);
                salvaParametrosFrango();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8D:
                enviaParaSerialOsParametrosAJustados(Interface);
                acionaBeepDeComandoFrango(1);
                break;
            case 0x8E:
                if (CorpoDeComandoRecebidoDoPC[0] < FINAL_WIEGAND){
                    ModoWieGand = CorpoDeComandoRecebidoDoPC[0];
                    salvaParametrosFrango();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x8F:
                if ((CorpoDeComandoRecebidoDoPC[0] < 28) || (CorpoDeComandoRecebidoDoPC[0] == PARAMETRO_SETA_FREQUENCIA_ANATEL)){
                    Frequencia = CorpoDeComandoRecebidoDoPC[0];
                    salvaParametrosFrango();
                    SetaFrequencias();
                    acionaBeepDeComandoFrango(1);
                }
                break;
            case 0x90:
                CorpoDeComandoRecebidoDoPC[0] = CorpoDeComandoRecebidoDoPC[0] & 0x03;
                CorpoDeComandoRecebidoDoPC[0] = CorpoDeComandoRecebidoDoPC[0] << 2;
                as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, CorpoDeComandoRecebidoDoPC[0]);
                acionaBeepDeComandoFrango(1);
                break;
            case 0x91:
                estadoSerialFrango = ESPERANDO_COMANDO_SERIAL_FRANGO;
                //sintonizaCapacitores(&mainTuner, CorpoDeComandoRecebidoDoPC[0], CorpoDeComandoRecebidoDoPC[1], CorpoDeComandoRecebidoDoPC[2]);
                Capacitores[0] = CorpoDeComandoRecebidoDoPC[0];
                Capacitores[1] = CorpoDeComandoRecebidoDoPC[1];
                Capacitores[2] = CorpoDeComandoRecebidoDoPC[2];
                
                Capacitores[3] = CorpoDeComandoRecebidoDoPC[3];
                Capacitores[4] = CorpoDeComandoRecebidoDoPC[4];
                Capacitores[5] = CorpoDeComandoRecebidoDoPC[5];
                
                salvaParametrosFrango();
                
                Reset();
                acionaBeepDeComandoFrango(1);
                break;
            case 0x92:
                salvaParametrosFrango();
                InibeOperacaoEMesagensComOPC = CorpoDeComandoRecebidoDoPC[0];
                acionaBeepDeComandoFrango(1);
                break;
                
            case 0xF5:
                ContadorDeSilencioNa485 = TEMPO_DE_SILENCIO_NA_485;
                //adicionaItemNaTabelaDeInvisiveis(CorpoDeComandoRecebidoDoPC);
                //(void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC, ENTROU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC,CorpoDeComandoRecebidoDoPC[3],CorpoDeComandoRecebidoDoPC[4], ENTROU_NA_AREA_SEGURA);
                comandoRecebidoNa485();
                break;
                
            case 0xF6:
                ContadorDeSilencioNa485 = TEMPO_DE_SILENCIO_NA_485;
                //adicionaItemNaTabelaDeVisiveis(CorpoDeComandoRecebidoDoPC);
                //(void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC, SAIU_NA_AREA_SEGURA);
                (void)adicionaElementoDaTabelaDeExclusao (CorpoDeComandoRecebidoDoPC,CorpoDeComandoRecebidoDoPC[3],CorpoDeComandoRecebidoDoPC[4], SAIU_NA_AREA_SEGURA);
                comandoRecebidoNa485();
                break;
            case 0xFD:
                RemovePrimeiroItemDaListaDeSaida();
                break;
            case 0xFE:
                reenvioDeItemDaListaDeSaida();
                
                if (recuperaItemDaListaDeSaida(Saida) >= 0){
                    _LATG13 = 1;
                    for (Contador = 0;Contador < 8; Contador = Contador + 1){
                        uart1Tx(Saida[Contador]);
                        //delay_us (120);
                        delay_ms (4);
                        delay_us (500);
                    }
                    _LATG13 = 0; 
                }
                
                break;             
        }
        iniciaEstruturaDoProtocoloPC();
    }   */
}


void ajustaFrequenciaEmFuncaoDaAntena (int Antena){
   /* //static int JaSintonizado = FRENTE;
    static int FrequenciaAjustada = 0;
    extern Freq Frequencies;
    extern int Frequencia;
    extern int FrequenciaTraseira;
    //extern u16 readerInitStatus;
    unsigned long rascunho;
    
    Antena = Antena & 0x00001;
    
    if (Antena == FRENTE){
        if (FrequenciaAjustada != Antena){
            rascunho = Frequencia;
            rascunho = rascunho + 900;
            rascunho = rascunho * 1000;
            Frequencies.freq[0] = rascunho;
            Frequencies.freq[1] = rascunho;
            
            //Frequencies.freq[0] = Frequencia;
            Frequencies.numFreqs = 1;
            TerminaInvetorio();
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
            comecaInvetorio();

            FrequenciaAjustada = Antena;
        }
        
    } else {
        if (FrequenciaAjustada != Antena){
            rascunho = FrequenciaTraseira;
            rascunho = rascunho + 900;
            rascunho = rascunho * 1000;
            Frequencies.freq[0] = rascunho;
            Frequencies.freq[1] = rascunho;
            
            Frequencies.numFreqs = 1;
            TerminaInvetorio();
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
            comecaInvetorio();
            
            FrequenciaAjustada = Antena;
        }
    }*/
}

void ajustaSintoniaEmFuncaoDaAntenaEmPortal(int antena){
    
    switch(antena){
        case 1:
            sintonizaCapacitores(&mainTuner, capAntena1[0], capAntena1[1], capAntena1[2]);
            break;
        case 2:
            sintonizaCapacitores(&mainTuner, capAntena2[0], capAntena2[1], capAntena2[2]);
            break;
        case 3:
            sintonizaCapacitores(&mainTuner, capAntena3[0], capAntena3[1], capAntena3[2]);
            break;
        case 4:
            sintonizaCapacitores(&mainTuner, capAntena4[0], capAntena4[1], capAntena4[2]);
            break;
        case 5:
            sintonizaCapacitores(&mainTuner, capAntena5[0], capAntena5[1], capAntena5[2]);
            break;
        case 6:
            sintonizaCapacitores(&mainTuner, capAntena6[0], capAntena6[1], capAntena6[2]);
            break;
        case 7:
            sintonizaCapacitores(&mainTuner, capAntena7[0], capAntena7[1], capAntena7[2]);
            break;
        case 8:
            sintonizaCapacitores(&mainTuner, capAntena8[0], capAntena8[1], capAntena8[2]);
            break;
        default:
            sintonizaCapacitores(&mainTuner, 0, 0, 0);
            break;
    }
}

void testaEnvio(void){
    (void)sprintf((char *)RegistroDeTagEmPortal[0].Epc, "123");

    RegistroDeTagEmPortal[0].TamanhoDoEpc = TAMANHO_DE_EPC_EM_PORTAL_FRANGO;
    RegistroDeTagEmPortal[0].AntenaDaPrimeiraLeitura = ANTENA_DE_ENTRADA;
    //RegistroDeTagEmPortal[0].AntenaDaPrimeiraLeitura = ANTENA_DE_SAIDA;
    RegistroDeTagEmPortal[0].AntenaDaUltimaLeitura = ANTENA_DE_SAIDA;
    //RegistroDeTagEmPortal[0].AntenaDaUltimaLeitura = ANTENA_DE_ENTRADA;
    RegistroDeTagEmPortal[0].TempoParaInatividade = 0;
    RegistroDeTagEmPortal[0].MarcaDePosicaoOcupada = 0;
    RegistroDeTagEmPortal[0].ContagemDeEntrada = 0;
    RegistroDeTagEmPortal[0].ContagemDeSaida = 0;
    RegistroDeTagEmPortal[0].estado = 0;
    //geraStringDoRegistroDeMovimento(RegistroDeTagEmPortal[0], Rascunho);
    geraStringDoRegistroDeMovimento(RegistroDeTagEmPortal[0]);

    //void enviaTagLidaParaAvaliacao(u8 *data, int Tamanho, int Antena, char Estado){
    //enviaTagLidaParaAvaliacao(RegistroDeTagEmPortal[0].Epc,3,1,0);
}


