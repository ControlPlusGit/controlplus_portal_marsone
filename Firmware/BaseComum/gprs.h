/*
 * File:   gprs.h
 * Author: LUA 2
 *
 * Created on 27 de Dezembro de 2014, 14:22
 */

#ifndef GPRS_H
#define	GPRS_H

//extern char empilhadeira[];

void IniciaMaquinaGPRS (void);
void ProcessaGPRS (void);
void ProcessEnvioDeTagsGPRS (void);
//void LigaRX (void);
int escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena);
void FtpPrepararParaEnviarTags (void);  //LUCIANO: PROTOTIPEI para eliminar um warning.
void ReiniciaLogicaDeJanela (void);     //LUCIANO: PROTOTIPEI para eliminar um warning.
int TratarCGED (char*comando, char *resultado, int timeout);    //LUCIANO: PROTOTIPEI para eliminar um warning.


void OperacoesParaTickGprs (void);
void compararRespostaDoModemEmTempoReal(char * buffer);
void ProcessaTagsParaOGPS (void);
int QuantiaDeTagsPresentesNoBufferDeFTP (void);
int TesteAutomaticoDoModem (void);




#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GPRS_H */

