/* 
 * File:   gprs.h
 * Author: LUA 2
 *
 * Created on 27 de Dezembro de 2014, 14:22
 */

#ifndef GPRS_H
#define	GPRS_H

extern char empilhadeira[20];

void EnviaByteParaSerial (char dado);
void ProcessaGPRS (void);
int escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena);
//void SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano);
void IniciaMaquinaGPRS(void);   //LUCIANO: Prototipei para eliminar um warning.
void testeI2C (void);           //LUCIANO: Prototipei para eliminar um warning.
void OperacoesParaTickGprs(void);   //LUCIANO: Prototipei para eliminar um warning.
int MaquinaDeEstadosAT(int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado);
int MaquinaDeEstadosDeComandosGPRS(int estado);


int conectaOModem (void);
int loggarNoFTP (void);
int fecharOArquivo (void);
//int MaquinaDeEstadosAT (int estado, char *comando, char *esperado, long  *timeout, char *resposta, int *resultado);
//int MaquinaDeEstadosDeComandosGPRS (int estado);
void resetNasVariaisDeModemEmTempoReal (void);
char conversao1307 (char dado);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GPRS_H */

