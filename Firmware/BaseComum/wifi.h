#ifndef WIFI_C
#define WIFI_C

#include "eth.h"

//#define TIMEOUT_WIFI_PORTAIS 200
#define TIMEOUT_WIFI_PORTAIS 20//100
#define TAMANHO_BUFFER_WIFI_PORTAIS 1500

#define TAMANHO_MAXIMO_STRING_IP            14
//Definicoes
#define PORTA_EMPILHADEIRAS                 9000
#define PORTA_PORTAIS                       8000
#define PORTA_SARS                          6000


#define QUANTIA_MAXIMA_DE_IPS_CADASTRADOS   20

#define TAMANHO_DA_RESERVA                  TAMANHO_BUFFER_WIFI_PORTAIS //1000

extern int volatile TimeOutWifiPortal;
extern char BufferWifiPortais[TAMANHO_BUFFER_WIFI_PORTAIS];
extern int ContadorBufferWifiPortais;

extern char MeuIpWifi[TAMANHO_DA_STRING_DE_IP];

void iniciaWifiDosPortais(void);
void recebeDadosDeWifiPortal(char Dado);
int estouroDoTimeOutDoBufferDoWifiDosPortais(char *BufferDeSaida);

//Envios do portal alfa
void enviaMovimentoParaOSars(char *Ip, char *Dado, int Tamanho);
void enviaMovimentoParaUmaEmpilhadeira(char *Ip, char *Dado, int Tamanho);
void enviaFluxoDeDadosPorWifi(char *Ip, int Porta, char *Dados, int Tamanho);

//Envios da empilhadeira
void enviaEventosParaOSars (void);

int enviaATabelaDeExclusaoParaAsEmpilhadeirasPresentes(int ModuloOcupado);
char trabalhaNaInterrupcaoDeWifi(void);

int pacoteSendoEnviadoPelaUartDeWifi(void);


int adicionaByteAReservaDeDadosDoWifi(char Dado);
int adicionaWordAReservaDeDadosDoWifi(int Entrada);
int disparaEnvioDaReservaDeDadosDoWifi(void);
int recebeOFluxoDeTrabalhoDoWifi(char *BufferDeSaida, int LimiteDoBuffer);
void enviaFluxoDadosPorInterrupcaoDeTX(char *Fluxo, int Tamanho);

void disparaOEnvioDaReservaDeDadosDoWifi(int Tamanho);
void resetTimeOutWifi(void);

void exemploDeUso (void);

void mudaOSSIDESenha (void);

void enviaFluxoDeDadosParaUartWifi(char *Dados, int Tamanho);

void disparaOEnvioDeFLuxoDeDadosPorWifi(char *Ip, int Porta, int Tamanho);
void enviaOFluxoDeDadosEmSoqueteAberto(char *Dados, int Tamanho);

void inibeAOperecaoDoWifi(void);

int foiRecebidaEssaMensagemPeloWifi(char *Mensagem);
void operacoesDeTickParaWifi(void);

void trataRecepcaoDeDadosDeEmpilhadeiraPorWifi(void);
void iniciaWifiDasEmpilhadeiras(void);
void LimpaEResetaARecepcaoDeDadosDoWifi (void);
void trataRecepcaoDeDadosParaAEmpilhadeiraPorWifi(void);
void enviaPerguntaDePresencaParaAsEmpilhadeirasNaArea(void);
void trataRecepcaoDeDadosDeEmpilhadeiraPorWifi(void);
void confereSeTemUmModuloPresente(void);
void enviaFluxoDeDadosPorPolling(char *Fluxo, int Tamanho);
void trabalharNaBuscaDoMeuIpWifi(void);
int confirmacaoDeRecepcaoDoModulo (void);

void coletaOMACDoModulo (char *Saida);
void setaOMACDOModulo (char *Mac);

int estouroNoTempoDeEnvio(int Tempo);
int confirmacaoDeRecepcaoDoModulo (void);
void formataOModuloWifi(void);
void resetaELimpaAReservaDeEnvioPorWifi(void);


void iniciaOTempoParaConferirAConexaoDeWifi (void);
void decrementaOTempoParaConferirAConexaoWifi (void);
void confereAConexaoDoWifi (void);
int confereSeOLinkNaoEhValido(void);


//

#endif 
