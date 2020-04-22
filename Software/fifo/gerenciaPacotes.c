#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#define PC 1
#ifdef PC
#include "fifo.h"
#else
#include "C:\Projetos\control-plus\software\fifo\fifo.h"
#include "p24FJ256DA210.h"
#include "timer.h"
#include "uart_driver.h"
#endif 

#include "C:\Projetos\control-plus\Firmware\BaseComum\eth.h"

#define TEMPO_PARA_REENVIO_DE_PACOTE 50 //Ms




int EstadoDeEnvioDePacotes;
char PacoteASerEnviado[TAMANHO_MAXIMO_DO_ITEM];
char PacoteRecebido[TAMANHO_MAXIMO_DO_ITEM];
int ContadorDeReenvioDePacote;

#ifndef PC
extern int ContadorDeSilencioNa485;
#endif

//int IdDoPortal = 0xAABB;
//extern int IdDoPortal;

enum EstadosDeGerenciaDePacotes {
	TEM_PACOTE,
	ESPERA_SILENCIO,
	ENVIA_PACOTE,
	ESPERA_CONFIRMACAO
};

enum EstadoDeRecepcaoDePacoteDeEsclusao {
	CABECA,
	TAMANHO,
        CORPO,
	RABO,
	SUCESSO
};

char PacoteDeConfirmacacao[TAMANHO_MAXIMO_DO_ITEM];
char PacoteDeReenvio[TAMANHO_MAXIMO_DO_ITEM];

char ExemploDePacote485[TAMANHO_MAXIMO_DO_ITEM];

#if PC
FILE *Arquivo;
#endif

//time_t Tempo, MarcaDeTempo;
//time_t MarcaDeTempo;
unsigned int MarcaDeTempo;

void escrevaPacoteParaSimularOProgramaEmPython(char *Pacote, int Tamanho);

char retornaComMSBDeInt(int Word){
	int Rascunho;
	Rascunho = Word >> 8;
	return (char)Rascunho;
}

char retornaComLSBDeInt(int Word){
	int Rascunho;
	Rascunho = Word & 0x00FF;
	return (char)Rascunho;
}

char retornaComCheckSum(char *Pacote, int Tamanho){
	char Rascunho;
	int Contador;

	Rascunho = 0;
	for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
		Rascunho = Rascunho + *Pacote;
		Pacote = Pacote + 1;
	}

	return Rascunho;
}

void formaPacoteDeConfirmacaoParaComparacao(int IdDoPortal){
	PacoteDeConfirmacacao[0] = 0xFE;//Comando
	PacoteDeConfirmacacao[1] = 0x06;//Tamanho
	PacoteDeConfirmacacao[2] = 0x55;//Corpo
	PacoteDeConfirmacacao[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	PacoteDeConfirmacacao[4] = retornaComLSBDeInt(IdDoPortal);;//Id do portal
	PacoteDeConfirmacacao[5] = retornaComCheckSum(PacoteDeConfirmacacao, (PacoteDeConfirmacacao[1] - 1));//CRC
}

void formaPaoteDeReenvioParaComparacao(int IdDoPortal){
	PacoteDeReenvio[0] = 0xFD;//Comando
	PacoteDeReenvio[1] = 0x06;//Tamanho
	PacoteDeReenvio[2] = 0xAA;//Corpo
	PacoteDeReenvio[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	//PacoteDeReenvio[3] = 0xFF;//Id do portal
	PacoteDeReenvio[4] = retornaComLSBDeInt(IdDoPortal);//Id do portal
	//PacoteDeReenvio[4] = 0xFF;//Id do portal
	PacoteDeReenvio[5] = retornaComCheckSum(PacoteDeReenvio, (PacoteDeReenvio[1] - 1));//CRC
}

long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
} 

int confereSeHaSilencioNoMeioDeComunicacao(void){
	//return 1;
#ifdef PC
	Arquivo = fopen("miso.txt", "r");
	if (Arquivo == NULL){
		fclose(Arquivo);
		return 1;
	}
	fclose(Arquivo);

	if (findSize("miso.txt") == 0){
		return 1;
	}
	
	return 0;
#else
    if (ContadorDeSilencioNa485) return 0;
	else return 1;
#endif
    
}

void enviaPacoteNoMeioDeComunicacao(void){
    int Contador;
#ifdef PC
	for (Contador = 0; Contador < PacoteASerEnviado[1];Contador = Contador + 1){
		printf("%02x", PacoteASerEnviado[Contador]);
	}
	printf("\n");
	
	Arquivo = fopen("miso.txt", "wb");
	if (Arquivo != NULL){
		for (Contador = 0; Contador < PacoteASerEnviado[1];Contador = Contador + 1){
			(void)fwrite(&PacoteASerEnviado[Contador], 1, 1, Arquivo);	
		}
	}
	fclose(Arquivo);
#else 
    _LATG13 = 1;
    delay_us(10);
    //for (Contador = 0;Contador < 8; Contador = Contador + 1){
    for (Contador = 0;Contador < PacoteASerEnviado[1]; Contador = Contador + 1){    
#ifdef PORTAL_COM_ETH
        enviaDadoParaEthPortais(PacoteASerEnviado[Contador]);
#else 
        uart1Tx(PacoteASerEnviado[Contador]);
#endif
    }
    delay_us(10);
    _LATG13 = 0;
    delay_us(10);

#endif
}

int recebidoPedidoDeReenvio(char *Pacote){
	if (memcmp(Pacote, PacoteDeReenvio, PacoteDeReenvio[1]) == 0){
		return 1;
	}
	return 0;	
}

int recebidoConfirmacaoDoPacoteEnviado(char *Pacote){
    //Para todos os outros portais
	if (memcmp(Pacote, PacoteDeConfirmacacao, PacoteDeConfirmacacao[1]) == 0){
    //Para os portais B que esta tendo problemas de comunicacao
    //if (memcmp(Pacote, PacoteDeConfirmacacao, (PacoteDeConfirmacacao[1] - 1)) == 0){
		return 1;
	}
	return 0;
}

int estouroDoTempoDeEspera(void){
#ifdef PC
    time_t Tempo;
	time(&Tempo);
	ContadorDeReenvioDePacote = Tempo - MarcaDeTempo;
	if (ContadorDeReenvioDePacote > TEMPO_PARA_REENVIO_DE_PACOTE) return 1;
	return 0;
#else 
    if (MarcaDeTempo >= TEMPO_PARA_REENVIO_DE_PACOTE) return 1;
    return 0;
#endif
}

int temPacoteASerEnviado(void){
	if (retornaComItemDaLista(PacoteASerEnviado) > -1)return 1;
	return 0;
}

void confirmaRecepcaoDoPacote(int IdDoPortal){
	char Pacote[10]; 

	/*
	PacoteRecebido[0] = 0xFE;//Comando
	PacoteRecebido[1] = 0x06;//Tamanho
	PacoteRecebido[2] = 0x55;//Corpo
	PacoteRecebido[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	PacoteRecebido[4] = retornaComLSBDeInt(IdDoPortal);;//Id do portal
	PacoteRecebido[5] = retornaComCheckSum(PacoteRecebido, (PacoteRecebido[1] - 1));//CRC
	*/

	Pacote[0] = 0xFE;//Comando
	Pacote[1] = 0x06;//Tamanho
	Pacote[2] = 0x55;//Corpo
	Pacote[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	Pacote[4] = retornaComLSBDeInt(IdDoPortal);;//Id do portal
	Pacote[5] = retornaComCheckSum(Pacote, (Pacote[1] - 1));//CRC
	escrevaPacoteParaSimularOProgramaEmPython(Pacote, Pacote[1]);
}

void pedeReenvioDoPacote(int IdDoPortal){
	char Pacote[10]; 

	/*
	PacoteRecebido[0] = 0xFD;//Comando
	PacoteRecebido[1] = 0x06;//Tamanho
	PacoteRecebido[2] = 0xAA;//Corpo
	PacoteRecebido[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	PacoteRecebido[4] = retornaComLSBDeInt(IdDoPortal);;//Id do portal
	PacoteRecebido[5] = retornaComCheckSum(PacoteRecebido, (PacoteRecebido[1] - 1));//CRC
	*/

	Pacote[0] = 0xFD;//Comando
	Pacote[1] = 0x06;//Tamanho
	Pacote[2] = 0xAA;//Corpo
	Pacote[3] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	Pacote[4] = retornaComLSBDeInt(IdDoPortal);;//Id do portal
	Pacote[5] = retornaComCheckSum(Pacote, (Pacote[1] - 1));//CRC
	escrevaPacoteParaSimularOProgramaEmPython(Pacote, Pacote[1]);
}


int trataRecepcaoDeProcoloDeExclusao (char Dado, int Estado){
	static int ContadorDeCorpo;
	static char Corpo[10];
	char CheckSum;
    
    static char Analise[20];
    static int ContagemAnalise = 0;
    
    Analise[ContagemAnalise] = (char)Dado;
    ContagemAnalise = ContagemAnalise + 1;
    if (ContagemAnalise >= 20)ContagemAnalise = 0;
    
    

	Corpo[ContadorDeCorpo] = Dado;
	ContadorDeCorpo = ContadorDeCorpo + 1;
    
    if (ContadorDeCorpo >= 10){
        Estado = CABECA;
        ContadorDeCorpo = 0;
        memset(Corpo, 0, 10);
    }

	switch (Estado){
		case CABECA:
			Corpo[ContadorDeCorpo] = Dado;
			ContadorDeCorpo = 1;
			if ((unsigned char)Dado == 0xF5) return TAMANHO;
			if ((unsigned char)Dado == 0xF6) return TAMANHO;
			if ((unsigned char)Dado == 0xFD) return TAMANHO;
			if ((unsigned char)Dado == 0xFE) return TAMANHO;
			ContadorDeCorpo = 0;
			return CABECA;
			break;

		case TAMANHO:
			if (Dado == 8) return CORPO;
			if (Dado == 6) return CORPO;
			//return CABECA;
            ContadorDeCorpo = 0;
			Corpo[ContadorDeCorpo] = Dado;
			ContadorDeCorpo = 1;
			if ((unsigned char)Dado == 0xF5) return TAMANHO;
			if ((unsigned char)Dado == 0xF6) return TAMANHO;
			if ((unsigned char)Dado == 0xFD) return TAMANHO;
			if ((unsigned char)Dado == 0xFE) return TAMANHO;
			ContadorDeCorpo = 0;
			return CABECA;
            
			break;

        case CORPO:
			if (ContadorDeCorpo == (Corpo[1] - 1)) return RABO;
			return CORPO;
			break;

		case RABO:
			CheckSum = retornaComCheckSum(Corpo, (Corpo[1] - 1));
			if (CheckSum == Corpo[ContadorDeCorpo - 1]) memcpy(PacoteRecebido, Corpo, Corpo[1]);
			ContadorDeCorpo = 0;
			return CABECA;
			break;

		default:
			ContadorDeCorpo = 0;
			return CABECA;
	}
}

void destroiPacoteRecebidoDoPython (void){
#ifdef PC
	Arquivo = fopen("mosi.txt", "w");
	fclose(Arquivo);
#else

#endif
}

void trataRecepcaoDePacoteDoAplicativoEmPython (void){
#ifdef PC
	char Dado;
	int Resultado;
	static int Estado;
	Arquivo = fopen("mosi.txt", "r");
	
	Resultado = 1;
	while (Resultado != 0){
		Resultado = fread(&Dado,1, 1, Arquivo);
		if (Resultado != 0){
			printf("%02X", Dado);
			Estado = trataRecepcaoDeProcoloDeExclusao(Dado, Estado);
		}
	}
	fclose(Arquivo);
	destroiPacoteRecebidoDoPython();
#endif
}


int EstadoDeRecepcao485;
void trataRecepcaoDeDadosPela485(char Dado){
    //static int Estado;
    //Estado = trataRecepcaoDeProcoloDeExclusao(Dado, Estado);
    
    
    EstadoDeRecepcao485 = trataRecepcaoDeProcoloDeExclusao(Dado, EstadoDeRecepcao485);
}

void resetNaRecepcaoDeDadosPela485(void){
    EstadoDeRecepcao485  = 0;
}

void iniciaMaquinaDeEnvioDePacotes (void){
	EstadoDeEnvioDePacotes = TEM_PACOTE;
#ifdef PC
	time(&Tempo);
#else
    MarcaDeTempo  = 0;
#endif
}

void operacoesParaTickParaGerenciaDePacotes(void){
    if (MarcaDeTempo < TEMPO_PARA_REENVIO_DE_PACOTE){
        MarcaDeTempo = MarcaDeTempo + 1;
    }
    
}

void destroiComandoRecebido(void){
	memset(PacoteRecebido, 0, TAMANHO_MAXIMO_DO_ITEM);
}

int trataMaquinaDeEnvioDePacotes (int Estado){
	switch(Estado){
		case TEM_PACOTE:
			if (temPacoteASerEnviado()){
				return ESPERA_SILENCIO;
			}

			return TEM_PACOTE;

		case ESPERA_SILENCIO:
			if (confereSeHaSilencioNoMeioDeComunicacao()){
				return ENVIA_PACOTE;
			}

			return ESPERA_SILENCIO;	
			break;

		case ENVIA_PACOTE:
#ifdef PC
			time(&MarcaDeTempo);
#else  
            MarcaDeTempo  = 0;
#endif
			enviaPacoteNoMeioDeComunicacao();

            // Para os portais B e G que estao com falha na comunicacao
            (void)removePrimeiroItem();
            destroiComandoRecebido();
            return TEM_PACOTE;
            // ******************************
            
            
			return ESPERA_CONFIRMACAO;
			break;

		case ESPERA_CONFIRMACAO:
            if (PacoteRecebido[0] != 0){
                if (recebidoPedidoDeReenvio(PacoteRecebido)){
                    destroiComandoRecebido();
                    resetNaRecepcaoDeDadosPela485();
                    return ESPERA_SILENCIO;
                }

                if (recebidoConfirmacaoDoPacoteEnviado(PacoteRecebido)){
                    (void)removePrimeiroItem();
                    destroiComandoRecebido();
                    resetNaRecepcaoDeDadosPela485();
                    return TEM_PACOTE;
                }
            }
            
			if (estouroDoTempoDeEspera()){
				destroiComandoRecebido();
                resetNaRecepcaoDeDadosPela485();
				return ESPERA_SILENCIO;				
			}


			//destroiComandoRecebido();
			return ESPERA_CONFIRMACAO;
			break;

		default:
			return TEM_PACOTE;
	}
}

void escrevaPacoteParaSimularOProgramaEmPython(char *Pacote, int Tamanho){
#ifdef PC
	int Contador;

	Arquivo = fopen("mosi.txt", "w");
	for (Contador = 0;Contador < Tamanho;Contador = Contador + 1){
		(void)fwrite(Pacote, 1, 1, Arquivo);
		Pacote = Pacote + 1;
	}
	fclose(Arquivo);
#endif
}

void adicionaPessoaEntrando(int IdDoPortal){
	ExemploDePacote485[0] = 0xF5;
	ExemploDePacote485[1] = 0x08;
	ExemploDePacote485[2] = 0x30;
	ExemploDePacote485[3] = 0x08;
	ExemploDePacote485[4] = 0x33;
	ExemploDePacote485[5] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	ExemploDePacote485[6] = retornaComLSBDeInt(IdDoPortal);//Id do portal		
	ExemploDePacote485[7] = retornaComCheckSum(ExemploDePacote485, (ExemploDePacote485[1] - 1));
	//escrevaPacoteParaSimularOProgramaEmPython(ExemploDePacote485, ExemploDePacote485[1]);
	adicionaElementoAFifo(ExemploDePacote485);
}

void adicionaPessoaSaindo(int IdDoPortal){
	ExemploDePacote485[0] = 0xF6;
	ExemploDePacote485[1] = 0x08;
	ExemploDePacote485[2] = 0x30;
	ExemploDePacote485[3] = 0x08;
	ExemploDePacote485[4] = 0x33;
	ExemploDePacote485[5] = retornaComMSBDeInt(IdDoPortal);//Id do portal
	ExemploDePacote485[6] = retornaComLSBDeInt(IdDoPortal);//Id do portal	
	ExemploDePacote485[7] = retornaComCheckSum(ExemploDePacote485, (ExemploDePacote485[1] - 1));
	//escrevaPacoteParaSimularOProgramaEmPython(ExemploDePacote485, ExemploDePacote485[1]);
	adicionaElementoAFifo(ExemploDePacote485);
}

void trataComunicacoes (void){
	trataRecepcaoDePacoteDoAplicativoEmPython();
	EstadoDeEnvioDePacotes = trataMaquinaDeEnvioDePacotes(EstadoDeEnvioDePacotes);
}

/*
void main (void){
	iniciaMaquinaDeEnvioDePacotes();
	formaPacoteDeConfirmacaoParaComparacao(IdDoPortal);
	formaPaoteDeReenvioParaComparacao(IdDoPortal);

	adicionaPessoaEntrando();
	adicionaPessoaSaindo();
	adicionaPessoaEntrando();

	time(&Tempo);

	printf("%d\n", Tempo);

	while (1){
		trataComunicacoes();
	}
}
*/
