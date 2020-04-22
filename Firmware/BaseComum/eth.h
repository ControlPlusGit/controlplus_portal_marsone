#ifndef ETH_H
#define ETH_H

#define TAMANHO_RESERVA_ETH_PORTAIS 100
#define TIMEOUT_ETH_PORTAIS 100
#define TAMANHO_BUFFER_RX_ETHERNET 100

#define IP_REMOTO_PRINCIPAL 1
#define IP_REMOTO_SECUNDARIO 2

#define NUMERO_DE_PORTAIS_ALPHA 2
#define TEMPO_DE_VIDA_DO_PORTAL_ALPHA 45000//45000 //30000
#define TEMPO_PARA_ENVIAR_O_MEU_PARA_SER_O_ALPHA_DOS_OUTROS_PORTAIS 200

#define QUANTIA_MAXIMA_DE_PORTAIS_EM_UMA_AREA 20

#define TAMANHO_DA_STRING_DE_IP 15

//STATUS DE CONEXAO TCP
#define CONNECTED 1
#define NOT_CONNECTED 0
#define TENTANDO_CONECTAR 2

// COMANDO ATIVO MODULO ETHERNET
#define NENHUM 0
#define CMD_KEEP_ALIVE 1
#define CMD_RESET_ETHERNET 2
#define CMD_SAI_MODO_AT 3
#define CMD_SOLICITA_DATA_HORA 4
#define CMD_STATUS_DE_CONEXAO 5

#define MODO_REDE_TCP 0
#define MODO_COMANDOS_AT 1

#define INTERVALO_DE_TROCA_REMOTE_IP 25


//#define TIMEOUT_PARA_SER_ALFA 20000

void recebeDadosDeEthPortal(char Dado);
//void enviaDadosParaEthPortais(char *Dados, int Tamanho);
void enviaDadosParaEthPortais(char dados[TAMANHO_BUFFER_RX_ETHERNET], int Tamanho);
int estouroDoTimeOutDoBufferDoEthDosPortais(char *BufferDeSaida);
int enviaStringEthPortais(char *Dados);
void trataTimeOutEthPortais(void);
void enviaDadoParaEthPortais(char Dado);
void lidaComATrocaDePortalAlpha(void);
void decrementaContadorDeTempoDeVidaDoPortalAlpha(void);
void operacoesEmTickParaEth(void);
void acoesDePortalComEthParaCodigoCorrente(void);
void iniciaOperacoesComPortalComEth (void);

int enviaParaTodosOsPortaisAPresencaDesseLeitor(int ModuloOcupado);
void resetNaReservaDeDadosDaEth(void);
int disponivelParaSerOAlpha(void);
void enviaParaTodosOsPortiasAPresencaDesseLeitorPorPolling(void);
void coletaMacDoMosuloUSRTCP232T24(void);

void solicitaDataHoraPelaEthernet(void);
void enviaKeepAliveParaEthernet(char status);
void defineDestinoDasMensagensEthernet(char ipRemoto[TAMANHO_DA_STRING_DE_IP], int portaRemota);
char solicitaStatusDeConexaoTCP(void);
void CommandHandleModuloEthernet(void);
void contaIntervaloDeKeepAlive(void);
void setaStatusDoLedDeEthernet(void);
void alertaSemConexaoComEthernet(void);
void checaNecessidadeDeTrocaDeIPRemoto(void);
void defineIPRemotoInicial(void);
void limpaBufferRxEthernet(void);
void aguardoPrimeiraConexaoTCP(void);

void entraEmModoAt(void);
void saiDoModoAt(void);
void resetModuloETH(void);

void geraEventosDePassagemParaTestarEmpi(void);


extern int TimeOutEthPortal;
extern int ContadorReservaEthPortais;
extern char ReservaEthPortais[TAMANHO_RESERVA_ETH_PORTAIS];
extern char bufferRxEthernet[TAMANHO_BUFFER_RX_ETHERNET];
extern unsigned char comandoAtivo;
extern char statusDeOperacaoEth;
extern char timerParaTrocaDeRemoteIP;
extern char alarmeFaltaDeRedeEthernet;
extern unsigned char statusDeConexaoTCP;


//extern char IdDOLeitor[20];




#endif
