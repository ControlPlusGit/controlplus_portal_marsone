/* 
 * File:   portal.h
 * Author: CQ-23
 *
 * Created on 21 de Fevereiro de 2017, 08:17
 */

#ifndef PORTAL_H
#define	PORTAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#define DELAY_PARA_DEGRADACAO_DA_LEITURA 0 //0 para capacidade maxima
    
#define TAMANHO_DE_EPC_EM_PORTAL_FRANGO 3//3
    
#define SEM_REGISTRO_DE_ANTENA 0
#define ANTENA_DE_ENTRADA  1
#define ANTENA_DE_SAIDA  2
    
#define MAXIMO_DE_TAGS_MONITORASDAS_EM_PORTAL 50
//#define TEMPO_PARA_INATIVIDADE_DE_TAG_MONITORADA 30000 //Em ticks da maquina
#define TEMPO_PARA_INATIVIDADE_DE_TAG_MONITORADA 30 //Em segundos
#define TEMPO_LIMITE_PARA_INATIVIDADE_DE_TAG_MONITORADA 60 //Em segundos
#define TAMANHO_MAXIMO_DE_STRING_DE_SAIDA 55
    
#define LIMITE_DE_INCREMENTO_DE_PORTAL  8
#define DECREMENTO_DE_PORTAL  2
    

#define OPERACAO_COM_UMA_ANTENAS    1
#define OPERACAO_COM_DUAS_ANTENAS   2
#define OPERACAO_COM_TRES_ANTENAS   3
#define OPERACAO_COM_QUATRO_ANTENAS 4
#define OPERACAO_COM_CINCO_ANTENAS  5
#define OPERACAO_COM_SEIS_ANTENAS   6
#define OPERACAO_COM_SETE_ANTENAS   7
#define OPERACAO_COM_OITO_ANTENAS   8

    
    
#define OPERACAO_COM_LEITURA_UNICA          1
#define OPERACAO_COM_MULTIPLAS_LEITURAS     2
#define OPERACAO_LEITURAS_INITERRUPTAS      3
    
    
#define LIMITE_PARA_ATRASO_PARA_DEGRADAR_LEITURA 100
#define LIMITE_DE_ANTENAS_FRANGO 8
#define LIMITE_DE_REPETICAO_EM_ANTENAS 8
#define LIMITE_DA_SENSIBILIDADE 125

    
enum ESTADOS_DE_TAG_EM_PORTAL {
    PORTAL_ELIMINADA,
    PORTAL_SAIDA,
    PORTAL_ENTRADA,
    PORTAL_ENTRANDO,
    PORTAL_SAINDO,
    PORTAL_ENTROU,
    PORTAL_SAIU
};
    

enum ESTADOS_DE_RECEPCAO_SERIAL_PORTAL_FRANGO {
    ESPERANDO_COMANDO_SERIAL_FRANGO,
    ESPERANDO_PRIMEIRO_PARAMENTRO_SERIAL_FRANGO,
    ESPERANDO_SEGUNDO_PARAMENTRO_SERIAL_FRANGO,
    ESPERANDO_TERCEIRO_PARAMENTRO_SERIAL_FRANGO,
    ESPERANDO_QUARTO_PARAMENTRO_SERIAL_FRANGO,
    ESPERANDO_QUINTO_PARAMENTRO_SERIAL_FRANGO,
    ESPERANDO_SEXTO_PARAMENTRO_SERIAL_FRANGO,
    FINAL_SERIAL_FRANGO
};

enum MODOS_WIEGAND {
    SEM_WIEGAND,
    WIEGAND_PARA_AVALIACAO,
    WIEGAND_PARA_PORTAL,
    FINAL_WIEGAND
};



#define TAG_PARA_AVALIACAO_FRANGO                               0x01
#define TAG_DE_MOVIMENTO_FRANGO                                 0x02
#define TAG_DE_MOVIMENTO_CANCELADO                              0x03

#define POSICAO_DO_COMANDO_PIC_PC_FRANGO                        0
#define POSICAO_DO_TAMANHO_DA_CARGA_PIC_PC_FRANGO               1
#define POSICAO_DA_ANTENA_DA_LEITURA_PIC_PC_FRANGO              2
#define POSICAO_DO_EPC_PARA_AVALIACAO_PIC_PC_FRANGO             3
#define POSICAO_DO_ESTADO_PARA_AVALIACAO_PIC_PC_FRANGO          6
#define POSICAO_DO_CHECKSUM_PARA_AVALIACAO_PIC_PC_FRANGO        7
#define POSICAO_DO_TAMANHO_TOTAL_PARA_AVALIACAO_PIC_PC_FRANGO   8

#define POSICAO_DA_ANTENA_DA_ULTIMA_LEITURA_PIC_PC_FRANGO       2
#define POSICAO_DO_EPC_EM_MOVIMENTO_PIC_PC_FRANGO               3
#define POSICAO_DO_ESTADO_PARA_MOVIMENTO_PIC_PC_FRANGO          (POSICAO_DO_EPC_EM_MOVIMENTO_PIC_PC_FRANGO + TAMANHO_DE_EPC_EM_PORTAL_FRANGO)//6
#define POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO        (POSICAO_DO_ESTADO_PARA_MOVIMENTO_PIC_PC_FRANGO + 1)//7
#define POSICAO_DO_TAMANHO_TOTAL_PARA_MOVIMENTO_PIC_PC_FRANGO   (POSICAO_DO_CHECKSUM_PARA_MOVIMENTO_PIC_PC_FRANGO + 1)//8



    
struct RegistroDeTagEmPortal_
{
    unsigned char Epc[TAMANHO_DE_EPC_EM_PORTAL_FRANGO];
    int TamanhoDoEpc;
    int AntenaDaPrimeiraLeitura;
    int AntenaDaUltimaLeitura;
    int LeuOutroAntena;
    int LeuUmaAntena;
    int TempoParaInatividade;
    char MarcaDePosicaoOcupada; //Apenas para otimizar os lacos de busca de posicao vazia
    char ContagemDeEntrada;
    char ContagemDeSaida;
    char estado;
    time_t MarcaDeTempo;
};

typedef struct RegistroDeTagEmPortal_ TipoRegistroDeTagEmPortal;


void acoesEmTickParaPortalFrango(void);
void acoesEmTickACadaSegundoParaPortalFrango(void);
//int TrataTagDePortalFrango (unsigned char *Tag, int Antena);
int TrataTagDePortalFrango (unsigned char *Tag, int Antena, int AntenaReal);
void iniciaPortalFrango(void);
int DadosASeremEnviadosPelaSerialDoPortalFrango(void);
void DadosDaSerialDoPortalTratadosFrango(void);
void acoesEmCodigoCorrentePortalFrango(void);
//void lidaComComandoDOPC(unsigned char Dado);
//void lidaComComandoDOPC(unsigned char Dado, int Interface);
//void lidaComComandoDOPCPrototipo(unsigned char Dado);
void lidaComComandoDOPCPrototipo(unsigned char Dado, int Interface);
void realizaBeepDeComandoFrango (void);
void SetaFrequencias(void);
void leOSSIDESenhaDoWifi(void);
void trataComandoRecebidoDoPCEmCodigoCorrente(void);

extern char StringDeSaidaParaSerial[TAMANHO_MAXIMO_DE_STRING_DE_SAIDA];
extern int ContadorDeTagMonitoradas;
extern int ContadorDeTagMovimentos;
extern int ModoDeOperacao;
extern int NumeroDeAntenasLidas;
extern int AtrasoParaDegradarLeitura;
extern int RepeticaoNaLeitura;
extern int Frequencia;

extern unsigned char SensibilidadeDaAntena;
void setaSensibilidade (unsigned char valor);

void aguradaSilencioNa485(void);

//void geraStringDoRegistroDeMovimento (TipoRegistroDeTagEmPortal Tag, char *StringFinal);
void geraStringDoRegistroDeMovimento (TipoRegistroDeTagEmPortal Tag);

#define SINALEIRO_VERMELHO  0
#define SINALEIRO_AMARELO   1
#define SINALEIRO_VERDE     2
#define SINALEIRO_CANCELADO 3

/*
#define SINALEIRO_VERMELHO              0
#define SINALEIRO_INDO_PARA_AMARELO     1
#define SINALEIRO_AMARELO               2
#define SINALEIRO_INDO_PARA_VERDE       3
#define SINALEIRO_VERDE                 4
#define SINALEIRO_INDO_PARA_VERMELHOR   5
#define SINALEIRO_CANCELADO             6
*/

void setaSinaleiro (int estado);
int adicionaItemNaListaDeSaida(unsigned char *Item);
int recuperaItemDaListaDeSaida(unsigned char *Item);
void iniciaListaDeSaida(void);
void EnviaStringPara485 (char * dado);
void obtemParametrosSalvosNaEeprom(void);
void enviaNovosjustesDeWifi (void);
void ajustesDoLeitor (unsigned char Dado, int Interface);
void ajustaFrequenciaEmFuncaoDaAntena (int Antena);
void ajustaSintoniaEmFuncaoDaAntenaEmPortal(int antena);

#define TEMPO_PARA_REENVIO_DA_LISTA_DE_SAIDA 100

#define TAMANHO_MAXIMO_DO_TEXT_SSID 40
#define POSICAO_SENHA_WIFI (14 + TAMANHO_MAXIMO_DO_TEXT_SSID)

#define POSICAO_ID_DO_LEITOR (POSICAO_SENHA_WIFI + TAMANHO_MAXIMO_DO_TEXT_SSID)

#define QUANTIA_DE_BYTES_DE_ID 4
extern char IdDoLeitor[QUANTIA_DE_BYTES_DE_ID];


extern __attribute__((far)) char SsidWifi[TAMANHO_MAXIMO_DO_TEXT_SSID];
extern __attribute__((far)) char SenhaWifi[TAMANHO_MAXIMO_DO_TEXT_SSID];

void testaEnvio(void);
#ifdef	__cplusplus
}
#endif

#endif	/* PORTAL_H */

