#ifndef EMPILHADEIRA_H
#define EMPILHADEIRA_H

#define NAO 0
#define SIM 1

#define DIRECAO_NULA 0
#define FRENTE 1
#define TRAS 2

#define TABELA_DE_EXCLUSAO

//#define SEM_OPERADOR

#define LIMITE_DE_ANTENAS 8
//#define LIMITE_DE_PEDESTRES 50
//#define LIMITE_DE_PEDESTRES MAXTAG //70
#define LIMITE_DE_PEDESTRES 100//70
#define ANTENA_PARA_LER_O_OPERADOR 4//6
#define TAMANHO_EPC_PARA_EMPILHADEIRA 3
#define QUANTIA_MAXIMA_DE_OPERADORES_NO_SISTEMA LIMITE_DE_PEDESTRES//100//20

#define TEMPO_DE_PAUSA_EM_POSSIVEL_ACIDENTE 7000

#define TEMPO_DE_VALIDADE_DE_OPERADOR 100//30//30000
#define TEMPO_PARA_PROCURAR_O_OPERADOR 1000

#define TICKS_PARA_UM_SEGUNDO 1000

#define TEMPO_PARA_ENVIAR_O_OPERADOR_PARA_OUTRAS_MAQUINAS 5000//400//2000

#define TAMANHO_BUFFER_DE_COMANDO_PARA_EMPILHADEIRA 100

//int sePedestreForOperadorEliminarDaListaDePedestresLidos(int PedestresLidos);
int sePedestreForOperadorEliminarDaListaDePedestresLidos(void);
void logicaDeEmpilhadeira (void);
int cadastrarOperador(unsigned char *Operador);
void iniciaLogicaDeEmpilhadeira (void);

void operacoesEmTickParaEmpilhadeira(void);

int adicionarPedestre(unsigned char *Pedestre, int Antena, int Rssi, int Indice);
int cadastrarPedestreAListaDoPortalDeExclusao (unsigned char *Pedestre);
int removePedestreDaListaDoPortalDeExclusaoPorEpc(unsigned char *Pedestre);
int OperadorEstaNaTabelaDeExclusao (char *Operador);
void trataRecepcaoDeDadosParaAvalicaoDeEmpilhadeira(char *Mensagem);
void reiniciaTempoParaEnviarOMeuOperador (void);
void logicaDeEmpilhadeiraV2 (void);


#define TAMANHO_DO_NOME_DA_EMPILHADEIRA 5
extern char NomeDaEmpilhadeira[TAMANHO_DO_NOME_DA_EMPILHADEIRA];

typedef struct EstruturaDePedestre {
  char Epc[TAMANHO_EPC_PARA_EMPILHADEIRA];
  int Dbm;
  int Antena;
} Pedestre;


typedef struct EstruturaDeOperadores {
  char Epc[TAMANHO_EPC_PARA_EMPILHADEIRA];
  int TempoDeValidade;
} Operador;

extern Operador MeuOperador;
extern char CintoVirtual;
extern char DirecaoVirtual;

extern char ComandoParaEmpilhadeira[TAMANHO_BUFFER_DE_COMANDO_PARA_EMPILHADEIRA];





//int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned char *CheckSum);
//int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum);
int trataEnvioDeProtocolo (unsigned char *Dado, int Tamanho, unsigned char Comando, unsigned char *Saida);


#endif
