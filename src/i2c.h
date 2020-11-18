/* 
 * File:   i2c.h
 * Author: none
 *
 * Created on 9 de Junho de 2015, 17:24
 */

#ifndef I2C_H
#define	I2C_H

#include "rtc.h"
//void SetarHoraRTC (int segundo, int minuto, int hora, int dia, int mes, int ano);
int CompararHoraDadaComADoRTC (int minutoDado, int horaDado, int diaDado, int mesDado, int anoDado);
void LigaI2CMemoria (void);
void LigaI2CRelogio (void);

int EscreveArray (int endereco, char *dados, char quantia);
int EscreverNaEEprom (int endereco, unsigned char dado);
int LerDadosDaEEprom (int endereco, unsigned char *dado);
int LerStringDaEEprom (int endereco, unsigned char *dado, int limite);
int EscreverStringNaEEprom (int endereco, unsigned char *dados, char limite);

int RetornaStringDeEventoNV (char *dados);
//int GravarEventoNV (char *dados);
int GravarEventoNV (char *dados, int tamanhoDoEvento);

unsigned long RetornaEpochDePosicaoNV (int posicao);
int SalvaEpoch (int posicao);
unsigned long RetornaEpoch(void);
void TickEpoch (void);
int PassadoXTempoDoUltimoEvento(int posicao, int tempo);
int LimpaTodaAEEprom(void);

// ***********************************************************************************
//Apenas para testes, esse metodo eh do GPRS.C
int escreverNoBufferDeFtpNV(char *tag, char *operador, int evento, int antena, int rssi);
// ***********************************************************************************

extern int contadori2c;
extern char strDiaDaSemana[10];
extern char strMes[10];
extern char strAno[10];

extern char stringData[20];
extern char stringHora[20];
extern int minuto;

extern int ContadorDePOR;
extern int ContadorDeBOR;
extern int ContadorDeEXTR;
extern int ContadorDeTRAPR;
extern int ContadorDeIOPUWR;
extern unsigned int ContadorDeEnviosBemSucedidosGPRS;
extern unsigned int ContadorDeTentativasDeEnvioGPRS;

void ZeraContadoresDeReset (void);
void RegistraSucessoGPRS (void);
void RegistraTentativaGPRS (void);
void ZeraContadoresGPRS (void);
void AtualizaContadoresDeReset (void);

void ObtemContadoresVolit (void);
void SalvaCOntadoresVolit (void);

void testeI2C (void);


#define VELOCIDADE_I2C 100000

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

