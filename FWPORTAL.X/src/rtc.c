#include "rtc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GenericTypeDefs.h"
#include "platform.h"
//#include "gprs.h"
#include "i2c.h"


time_t Tempo;


struct tm * timeinfo;

extern int contadori2c;

char strDiaDaSemana[10];
char strMes[10];

char stringData[20];
char stringHora[20];
int minuto;

int ContagemDeMiliSegundo;

struct tm * retornaTimeInfo(void){
    return timeinfo;
}

void IniciaRTC (void){
    I2C1BRG = (unsigned int)(VELOCIDADE_I2C / (SYSCLK * 2) ) - 1;   //LUCIANO: Apliquei um cast para eliminar um warning.
    I2C1CONbits.I2CEN = 1;
    
    atualizaHoraEData();
}

char conversao1307 (char dado){
    char rascunho;
    char resultado;

    rascunho = dado;
    rascunho = rascunho >> 4;
    rascunho = rascunho * 10;
    resultado = rascunho + (dado & 0x0F);
    return resultado;
}

unsigned char converteDecimalParaDS1307(unsigned char Dado){
    unsigned char Rascunho, Resultado;
    Rascunho = Dado / 10;
    Resultado = Rascunho << 4;
    Rascunho = Dado % 10;
    Resultado = Resultado | Rascunho;
    return Resultado;
}

void aguardaI2CRBF (void){
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;
    
    contadori2c = 100;
    countTimeout = 0;
    while(!I2C1STATbits.RBF){
        if(countTimeout++ > limCountTimeout)
            break; 
    }    
}

void aguardaI2CTRSTAT (void){
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;
        
    contadori2c = 100;
    countTimeout = 0;
    while(I2C1STATbits.TRSTAT){
        if(countTimeout++ > limCountTimeout)
            break; 
    }    
}

void aguardaI2CACKEN (void){
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;
    contadori2c = 100;
    countTimeout = 0;
    while(I2C1CONbits.ACKEN && (contadori2c != 0)){
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout. 
    }//;
}

void aguardaI2CACKENSemContagem (void){
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;
    countTimeout = 0;
    while(I2C1CONbits.ACKEN && (contadori2c != 0))
    {
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout. 
    }//;
}

void atualizaHoraEData (void){
    
    int segundo;
    int hora;
    int dia;
    int diaDaSemana;
    int mes;
    int ano;
    static int SegundoAnterior;

    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;

    //START
    I2C1CONbits.SEN = 1;
    countTimeout = 0;
    while(I2C1CONbits.SEN){
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout.
    }//;

    //BYTE 1
    I2C1TRN = 0xD0;
    aguardaI2CTRSTAT();

    //BYTE 2
    I2C1TRN = 0x00;
    aguardaI2CTRSTAT();

    I2C1CONbits.RSEN = 1;
    countTimeout = 0;
    while (I2C1CONbits.RSEN){
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout.  
    }//;

    //BYTE 3
    I2C1TRN = 0xD1;
    aguardaI2CTRSTAT();

    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    segundo = I2C1RCV;

    //ACK 1
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    countTimeout = 0;
    while(I2C1CONbits.ACKEN){
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout. 
    }

    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    minuto = I2C1RCV;

    //ACK 2
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKEN();

    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    hora = I2C1RCV;

    //ACK 3
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKENSemContagem();

    // ---------------------------
    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();
    //RECEBE OS DADOS
    diaDaSemana = I2C1RCV;

    //ACK 4
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKEN();

    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    dia = I2C1RCV;

    //ACK 5
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKEN();

    // ---------------------------
    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    mes = I2C1RCV;

    //ACK 6
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKEN();

    //LIGA RECEPCAO
    I2C1CONbits.RCEN = 1;
    aguardaI2CRBF();

    //RECEBE OS DADOS
    ano = I2C1RCV;

    //ACK 7
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    aguardaI2CACKEN();

    //STOP
    I2C1CONbits.PEN = 1;
    contadori2c = 100;
    countTimeout = 0;
    while(I2C1CONbits.PEN && (contadori2c != 0)){
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout. 
    }//;
    
    
    if (segundo != SegundoAnterior){
        ContagemDeMiliSegundo = 0;
        SegundoAnterior = segundo;
    }
    


    segundo = conversao1307(segundo);
    minuto = conversao1307(minuto);
    hora = conversao1307(hora);
    dia = conversao1307(dia);
    mes = conversao1307(mes);
    ano = conversao1307(ano);

    
#ifdef RF_IDEIAS
    sprintf(stringData,"%02d-%02d-20%02d", dia, mes, ano);
#else    
    sprintf(stringData,"%02d/%02d/20%02d", dia, mes, ano);
#endif    
    
    
    time_t rawtime;

    timeinfo = localtime ( &rawtime );    

    ano = ano + 100;
    mes = mes - 1;

    timeinfo->tm_year = ano;
    timeinfo->tm_mon = mes;
    timeinfo->tm_mday = dia;
    timeinfo->tm_hour = hora;
    timeinfo->tm_min = minuto;
    timeinfo->tm_sec = segundo;
    timeinfo->tm_isdst = -1;

#ifndef RF_IDEIAS
    Tempo = mktime ( timeinfo );
#endif
    
    //strftime(Saida, sizeof(Saida), "%I:%M%p", timeinfo);    
    (void)strftime(stringHora, sizeof(stringHora), "%H:%M:%S", timeinfo);
    (void)strftime(strDiaDaSemana, sizeof(stringHora), "%a", timeinfo);
    (void)strftime(strMes, sizeof(stringHora), "%b", timeinfo);
#ifdef RF_IDEIAS
    sprintf(stringHora,"%02d.%02d.%02d.%03d", hora, minuto, segundo, ContagemDeMiliSegundo);
#else    
    sprintf(stringHora,"%02d:%02d:%02d",hora,minuto,segundo);
#endif    
    
    
        
}


void SetarHoraRTC (int segundo, int minuto, int hora, int dia, int mes, int ano){
    char resultado;
    unsigned int countTimeout = 0;
    const unsigned int limCountTimeout = 1000;

    //START
    I2C1CONbits.SEN = 1;
    contadori2c = 100;
    countTimeout = 0;
    while(I2C1CONbits.SEN && (contadori2c != 0))
    {
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout.
    }//;

    //BYTE 1
    I2C1TRN = 0xD0;
    aguardaI2CTRSTAT();

    //BYTE 2
    I2C1TRN = 0x00;
    aguardaI2CTRSTAT();

    //BYTE 3
    I2C1TRN = 0x00;
    aguardaI2CTRSTAT();
    
    resultado = converteDecimalParaDS1307(minuto);
    I2C1TRN = resultado;
    aguardaI2CTRSTAT();

    resultado = converteDecimalParaDS1307(hora);
    I2C1TRN = resultado;
    aguardaI2CTRSTAT();

    I2C1TRN = 0;
    aguardaI2CTRSTAT();

    resultado = converteDecimalParaDS1307(dia);
    I2C1TRN = resultado;
    aguardaI2CTRSTAT();

    resultado = converteDecimalParaDS1307(mes);
    I2C1TRN = resultado;
    aguardaI2CTRSTAT();
    
    resultado = converteDecimalParaDS1307(ano);
    I2C1TRN = resultado;
    aguardaI2CTRSTAT();


    //STOP
    I2C1CONbits.PEN = 1;
    contadori2c = 100;
    countTimeout = 0;
    while(I2C1CONbits.PEN && (contadori2c != 0))
    {
        if(countTimeout++ > limCountTimeout)
            break;              // sai do loop por timeout.
    }//;
}

int AjustarOHorario;
void SetaTempoComEpoch (time_t Epoch){
    AjustarOHorario = 1;
    Tempo = Epoch;
    
    
}


void operacoesParaRtcEmCodigoCorrente(void){
    struct tm * timeinfo;
    int Segundo, Minuto, Hora;
    int Dia, Mes, Ano;
    
    if (AjustarOHorario){    
        timeinfo = localtime (&Tempo);
        Segundo = timeinfo->tm_sec;
        Minuto = timeinfo->tm_min;
        Hora = timeinfo->tm_hour;
        Dia = timeinfo->tm_mday;
        Mes = timeinfo->tm_mon + 1;
        Ano = timeinfo->tm_year;
        Ano = Ano - 100;

        //int segundo, int minuto, int hora, int dia, int mes, int ano

        SetarHoraRTC(Segundo, Minuto, Hora, Dia, Mes, Ano);
        AjustarOHorario = 0;
    }
    
}

void OperacoesParaRtcEmTick (void){//O tick eh gerado por interrupcao da CPU
    static int ContadorParaUmSegundo;
    
    ContagemDeMiliSegundo = ContagemDeMiliSegundo + 1;
    
    if (ContadorParaUmSegundo < 1000){  
        ContadorParaUmSegundo = ContadorParaUmSegundo + 1;
    } else {
        ContadorParaUmSegundo = 0;
        //atualizaHoraEData();
        Tempo = Tempo + 1;
        
        /*
        if (Pulso != 0){
            Pulso = 0;
            sel_led(3,1);
        } else {
            Pulso = 1;
            sel_led(3,0);
        }
        */
    }
}

time_t retornaComEpoch (void){
    return Tempo;
}