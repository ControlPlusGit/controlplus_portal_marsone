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
void TrataRecepcaoDeDadosDeZigBee(unsigned char Dado);
void operacoesEmTickParaZigBee (void);
void operacoesEmCodigoCorrenteParaZigBee (void);
void logicaDeRemocaoDeTabelaDeExclusao (void);
void ResetaTempoParaExpirarOBufferDoZigBee (void);

struct EstruturaProtocolo {
	int Estado;
	unsigned char Comando;
	int Tamanho;
	unsigned int CheckSum;
	int ContagemDoCorpo;
};


enum ESTADOS_DE_RX {
	ESPERANDO_CABECA,
	ESPERANDO_TAMANHO_MSB,
    ESPERANDO_TAMANHO_LSB,
	PEGANDO_O_CORPO,
    ESPERANDO_CHECKSUM_MSB,
	ESTADO_DE_RECEPCAO_FINAL
};


//
/*
#define QUANTIA_DE_ELEMENTOS 70//50//250
#define TAMANHO_DOS_ELEMENTOS 5
#define QUANTIA_DE_DADOS_DA_TABELA (QUANTIA_DE_ELEMENTOS * TAMANHO_DOS_ELEMENTOS)
#define TAMANHO_DOS_DADOS_NA_TABELA (QUANTIA_DE_DADOS_DA_TABELA + 6)


struct EstruturaTabelaDeExclusao {
	//time_t MomentoQueFoiCriada;//4
	char Elementos[QUANTIA_DE_ELEMENTOS][TAMANHO_DOS_ELEMENTOS];
    char Excluido[QUANTIA_DE_ELEMENTOS];
    unsigned int ContagemParaExpirar[QUANTIA_DE_ELEMENTOS];
    //char ContagemParaExpirar[QUANTIA_DE_ELEMENTOS];
	//int CheckSumDaRTabela;//2
    //char *PonteiroParaOsDadosRecebidos;//2
    //int CheckSumGerado;//2
    
    //int QuantiaDeDadosPresentes;//2
};
*/


#define COMANDO_DE_LOG_ZIG_BEE 0xFF
#define COMANDO_EVENTO_ZIG_BEE 0xFE
#define COMANDO_DE_ID_ZIG_BEE 0xFD

#define TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE 30000
#define TEMPO_PARA_EXPIRAR_OS_DADOS_DO_ZIGBEE 75//20
#define TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO 30000//15000//5000

#define TAMANHO_DO_PACOTE_DE_TESTE_DO_ZIGBEE 50

#define TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE 120






#ifdef	__cplusplus
}
#endif

#endif	/* ZIGBEE_H */

