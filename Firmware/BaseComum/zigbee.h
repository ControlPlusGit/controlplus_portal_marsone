/* 
 * File:   zigbee.h
 * Author: gilson
 *
 * Created on 30 de Agosto de 2017, 13:57
 */

#ifndef ZIGBEE_H
#define	ZIGBEE_H


#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

    
void enviaTextoDeLogDaOperacao (char *Texto);
void enviaMensagemAoMonitorPorZigBee(char *Mensagem, unsigned char Comando);
void enviaOcorrenciaDeEventoPorZigBee (unsigned char *EpcPedestre, unsigned char *EpcOperador);
void enviaMensagemBinariaAoMonitorPorZigBee(char *Mensagem, int Tamanho, unsigned char Comando);
void enviaMensagemPor485 (char *Mensagem);
void iniciaEstruturaDoProtocolo (void);
void enviaTextoDeLogDaOperacao (char *Texto);
void lidaComExpiracaoDoBufferDeRecepcaoDoZigBee (void);
void trataRecepcaoDeDadosDeZigBee(unsigned char Dado);
void operacoesEmTickParaZigBee (void);
void operacoesEmCodigoCorrenteParaZigBee (void);
void enviaBytePorZigBee(unsigned char Dado);
void enviaWordPorZigBee(unsigned int Dado);

int tempoDeSilencioNoZigBee(void);
void zeraAContagemDeSilencioNoZigBee(void);

//int trataRecepcaoNoProcoloPC (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned char *CheckSum);
//int trataRecepcaoNoProcoloPC (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum); 
//int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum);
int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, unsigned int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum);

void recebeDadosDaUartDaTabelaDeExclusao(unsigned char Dado);
int haDadosASeremProcessadosNoBufferDeRecepcaoDaUartDaTabelaDeExclusao(void);
void decrementaContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao(void);
void ResetaTempoParaExpirarOBufferDoZigBee (void);


enum ESTADOS_DE_RX {
	ESPERANDO_CABECA,
	ESPERANDO_TAMANHO_MSB,
    ESPERANDO_TAMANHO_LSB,
	PEGANDO_O_CORPO,
    ESPERANDO_CHECKSUM_MSB,
	ESTADO_DE_RECEPCAO_FINAL
};


struct EstruturaProtocolo {
	int Estado;
	unsigned char Comando;
	unsigned int Tamanho;
	unsigned int CheckSum;
	int ContagemDoCorpo;
};


#define COMANDO_DE_LOG_ZIG_BEE 0xFF
#define COMANDO_EVENTO_ZIG_BEE 0xFE
#define COMANDO_DE_ID_ZIG_BEE 0xFD

#define TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE 3000//30000
#define TEMPO_PARA_EXPIRAR_OS_DADOS_DO_ZIGBEE 3

#define TAMANHO_DO_PACOTE_DE_TESTE_DO_ZIGBEE 50

#define TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE 120




#ifdef	__cplusplus
}
#endif

#endif	/* ZIGBEE_H */

