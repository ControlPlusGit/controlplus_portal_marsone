/* 
 * File:   rtc.h
 * Author: gilson
 *
 * Created on 11 de Dezembro de 2017, 15:53
 */

#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <time.h>

void IniciaRTC (void);
void atualizaHoraEData (void);
//void SetarHoraRTC2 (int minuto, int hora, int dia, int mes, int ano);
void SetarHoraRTC (int segundo, int minuto, int hora, int dia, int mes, int ano);
void SetaTempoComEpoch (time_t Epoch);
time_t retornaComEpoch (void);
void OperacoesParaRtcEmTick (void);

extern char strDiaDaSemana[10];
extern char strMes[10];
extern char stringData[20];
extern char stringHora[20];
extern int minuto;


extern time_t Tempo;





#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

