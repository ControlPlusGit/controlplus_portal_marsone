/*
 * File:   barreiraIR.c
 * Author: Marcos
 *
 * Created on 22 de Setembro de 2021, 17:37
 */


#include <xc.h>
#include "p24FJ256DA210.h"
#include "barreiraIR.h"
#include "eth.h"
#include "platform.h"


unsigned char pedestreNoSensorIR;
unsigned char pedestreNaAntenaRFID;
unsigned int tempoDeMonitoramentoDoPortal;
unsigned char statusDeOperacaoDoLeitorRFID;
unsigned char flagSensorIRAtivadoPorPassagem;


void InitSensorIR(void){
    _TRISG6 = 1; //Define Opto1 como entrada
    pedestreNoSensorIR = AUSENTE;
    pedestreNaAntenaRFID = AUSENTE; 
    tempoDeMonitoramentoDoPortal = 0;
    flagSensorIRAtivadoPorPassagem = 0;
}


void LerSensorIR(void){
    if(SENSOR_IR == 1 && pedestreNoSensorIR == AUSENTE){
        pedestreNoSensorIR = PRESENTE;
        flagSensorIRAtivadoPorPassagem = 1;
        if(tempoDeMonitoramentoDoPortal == 0){
            tempoDeMonitoramentoDoPortal = TEMPO_DE_MONITORAMENTO_DE_PASSAGEM;
            LED_BT(1);
        }
    }
}


void setaPedestreNaAntenaRFID(){
    if(pedestreNaAntenaRFID == AUSENTE){
        pedestreNaAntenaRFID = PRESENTE;
        if(tempoDeMonitoramentoDoPortal == 0){
            tempoDeMonitoramentoDoPortal = TEMPO_DE_MONITORAMENTO_DE_PASSAGEM;
            LED_BT(1);
        }
    }
}

void MonitoraPassagemValidaPeloPortal(void){
    
    if(pedestreNoSensorIR == PRESENTE && pedestreNaAntenaRFID == PRESENTE){
        statusDeOperacaoDoLeitorRFID = STATUS_NORMAL;
    }
    
    if(pedestreNoSensorIR == PRESENTE && pedestreNaAntenaRFID == AUSENTE){
        statusDeOperacaoDoLeitorRFID = STATUS_FALHA_NA_ANTENA;
    }
    
    if(tempoDeMonitoramentoDoPortal == 0){
        pedestreNaAntenaRFID = AUSENTE;
        pedestreNoSensorIR = AUSENTE;
        LED_BT(0);
    }else{
        tempoDeMonitoramentoDoPortal = tempoDeMonitoramentoDoPortal - 1;
    }
}


void ChecaSeHouveFalhaNoSensorIR(void){
    if(flagSensorIRAtivadoPorPassagem == 1){
        flagSensorIRAtivadoPorPassagem = 0;
        statusDeOperacaoDoLeitorRFID = STATUS_NORMAL;
    }else{
        statusDeOperacaoDoLeitorRFID = STATUS_FALHA_SENSOR_IR;
    }
}





