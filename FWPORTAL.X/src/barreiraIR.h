/* 
 * File:   barreiraIR.h
 * Author: Marcos
 *
 * Created on 22 de Setembro de 2021, 18:28
 */

#ifndef BARREIRAIR_H
#define	BARREIRAIR_H

#include <xc.h>
#include "PIC24F_periph_features.h"
#include "p24FJ256DA210.h"


#define SENSOR_IR PORTGbits.RG6 
#define PRESENTE 1
#define AUSENTE 0
#define TEMPO_DE_MONITORAMENTO_DE_PASSAGEM 30 //2 segundos




void InitSensorIR(void);
void LerSensorIR(void);
void SetaContadorDeTempoDeVidaDaAntenaLida(void);
void MonitoraPassagemValidaPeloPortal(void);
void ChecaSeHouveFalhaNoSensorIR(void);



#endif	/* BARREIRAIR_H */

