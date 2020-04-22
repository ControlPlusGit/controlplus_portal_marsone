#include "as3993_config.h"
#include "platform.h"
/*
#include "C:\projetos\Ambve\common\firmware\microchip\include\stream_dispatcher.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\usb_hid_stream_driver.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\logger.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\uart_driver.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\errno.h"
 * */
#include "as3993_public.h"
#include "as3993.h"
#include "gen2.h"
#include "global.h"
#include "timer.h"
#include "appl_commands.h"
#include "tuner.h"
//#include "main.h"
#include "i2c.h"
#include "perifericos.h"
#include "rtc.h"

//#include "varal.h"

#define ENDERECO_EEPROM_LSB_CONTADOR_POR 1
#define ENDERECO_EEPROM_MSB_CONTADOR_POR 0

#define ENDERECO_EEPROM_LSB_CONTADOR_BOR 3
#define ENDERECO_EEPROM_MSB_CONTADOR_BOR 2

#define ENDERECO_EEPROM_LSB_CONTADOR_EXTR 5
#define ENDERECO_EEPROM_MSB_CONTADOR_EXTR 4

#define ENDERECO_EEPROM_LSB_CONTADOR_TRAPR 7
#define ENDERECO_EEPROM_MSB_CONTADOR_TRAPR 6

#define ENDERECO_EEPROM_LSB_CONTADOR_IOPUWR 9
#define ENDERECO_EEPROM_MSB_CONTADOR_IOPUWR 8

#define ENDERECO_EEPROM_LSB_CONTADOR_SUCESSO_GPRS 11
#define ENDERECO_EEPROM_MSB_CONTADOR_SUCESSO_GPRS 10

#define ENDERECO_EEPROM_LSB_CONTADOR_TENTATIVA_GPRS 13
#define ENDERECO_EEPROM_MSB_CONTADOR_TENTATIVA_GPRS 12


int ContadorDePOR;
int ContadorDeBOR;
int ContadorDeEXTR;
int ContadorDeTRAPR;
int ContadorDeIOPUWR;
unsigned int ContadorDeEnviosBemSucedidosGPRS;
unsigned int ContadorDeTentativasDeEnvioGPRS;

int contadori2c;

int gpsMinuto;
int gpsHora;
int gpsDia;
int gpsMes;
int gpsAno;

//char strDiaDaSemana[10];
//char strMes[10];

//char stringData[20];
//char stringHora[20];
//int minuto;




void LigaI2CMemoria (void){
    I2C3BRG = (unsigned int)(VELOCIDADE_I2C / (SYSCLK * 2) ) - 1;   //LUCIANO: Apliquei um cast para eliminar um warning.
    I2C3CONbits.I2CEN = 1;
}


void LigaI2CRelogio (void){
    I2C1BRG = (unsigned int)(VELOCIDADE_I2C / (SYSCLK * 2) ) - 1;   //LUCIANO: Apliquei um cast para eliminar um warning.
    I2C1CONbits.I2CEN = 1;
}

int EscreveArray (int endereco, char *dados, char quantia){
    int contador;
    int enderecoTemporario;

    enderecoTemporario = endereco;
    for (contador = 0;contador < quantia;contador = contador + 1){
        EscreverNaEEprom(enderecoTemporario, *dados);
        enderecoTemporario = enderecoTemporario + 1;
        dados = dados + 1;
    }
    return 0;
}

int EscreverNaEEprom (int endereco, unsigned char dado){
    unsigned char temporario;
    //START
    I2C3CONbits.SEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.SEN && (contadori2c != 0) );

    //COMANDO
    I2C3TRN = 0xA0;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );

    //MSB ENDERECO
    temporario = endereco >> 8;
    I2C3TRN = temporario;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );

    //LSB ENDERECO
    temporario = endereco & 0x00FF;
    I2C3TRN = temporario;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );

    //BYTE A SER ESCRITO
    I2C3TRN = dado;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );

    //STOP
    I2C3CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.PEN && (contadori2c != 0) );

    delay_ms(5);
    return 0;// OK

    return -1; //Erro
}

int LerDadosDaEEprom (int endereco, unsigned char *dado){
    char ack = 0;   //LUCIANO: Inicializei a variavel ack. Eliminei um warning.
    unsigned char temporario;
    //START
    I2C3CONbits.SEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.SEN && (contadori2c != 0) );

    //COMANDO
    I2C3TRN = 0xA0;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );
    ack = ack | I2C3STATbits.ACKSTAT;

    //MSB ENDERECO
    temporario = endereco >> 8;
    I2C3TRN = temporario;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );
    ack = ack | I2C3STATbits.ACKSTAT;

    //LSB ENDERECO
    temporario = (endereco & 0x00FF);
    I2C3TRN = temporario;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );
    ack = ack | I2C3STATbits.ACKSTAT;

    //RESTART
    I2C3CONbits.RSEN = 1; 
    contadori2c = 100;
    while (I2C3CONbits.RSEN && (contadori2c != 0) );

    //COMANDO
    I2C3TRN = 0xA1;
    contadori2c = 100;
    while(I2C3STATbits.TRSTAT && (contadori2c != 0) );
    ack = ack | I2C3STATbits.ACKSTAT;

    //LIGA RECEPCAO
    I2C3CONbits.RCEN = 1;
    //while(I2C1CONbits.RCEN);
    contadori2c = 100;
    while(!I2C3STATbits.RBF && (contadori2c != 0) );
  
    //RECEBE OS DADOS
    ack = ack | I2C3STATbits.ACKSTAT;

    *dado = I2C3RCV;

    //STOP
    I2C3CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.PEN && (contadori2c != 0) );


    delay_ms(5);

    if (ack == 0)
        return 0;// OK
    else
        return -1; //Erro
}


/*
void SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano){
    char rascunho;
    char resultado;

    //START
    I2C1CONbits.SEN = 1;
    contadori2c = 100;
    while(I2C1CONbits.SEN && (contadori2c != 0) );

    //BYTE
    I2C1TRN = 0xD0;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    //BYTE
    I2C1TRN = 0x00;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    //BYTE
    I2C1TRN = 0x00;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );


    rascunho = minuto / 10;
    resultado = rascunho << 4;
    rascunho = minuto % 10;
    resultado = resultado | rascunho;

    I2C1TRN = resultado;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );


    rascunho = hora / 10;
    resultado = rascunho << 4;
    rascunho = hora % 10;
    resultado = resultado | rascunho;

    I2C1TRN = resultado;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    I2C1TRN = 0;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    rascunho = dia / 10;
    resultado = rascunho << 4;
    rascunho = dia % 10;
    resultado = resultado | rascunho;

    I2C1TRN = resultado;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    rascunho = mes / 10;
    resultado = rascunho << 4;
    rascunho = mes % 10;
    resultado = resultado | rascunho;

    I2C1TRN = resultado;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

    rascunho = ano / 10;
    resultado = rascunho << 4;
    rascunho = ano % 10;
    resultado = resultado | rascunho;

    I2C1TRN = resultado;
    contadori2c = 100;
    while(I2C1STATbits.TRSTAT && (contadori2c != 0) );


    //STOP
    I2C1CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C1CONbits.PEN && (contadori2c != 0) );



}
*/

int CompararHoraDadaComADoRTC (int minutoDado, int horaDado, int diaDado, int mesDado, int anoDado){
    int resultado;

    resultado = 0;
    if (minutoDado != gpsMinuto)
        resultado = resultado | 1;

    if (horaDado != gpsHora)
        resultado = resultado | 1;

    if (horaDado != gpsDia)
        resultado = resultado | 1;

    if (mesDado != gpsMes)
        resultado = resultado | 1;

    if (anoDado != gpsAno)
        resultado = resultado | 1;

    return resultado;
}

/*
char conversao1307 (char dado){
    char rascunho;
    char resultado;

    rascunho = dado;
    rascunho = rascunho >> 4;
    rascunho = rascunho * 10;
    resultado = rascunho + (dado & 0x0F);

    return resultado;

}
*/


void testeI2C (void){
    int segundo;
    int hora;
    int dia;
    int diaDaSemana;
    int mes;
    int ano;

    //int rascunho;
    //int resultado;

    //for (;;){
        //START
        I2C1CONbits.SEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.SEN && (contadori2c != 0) );

        //BYTE
        I2C1TRN = 0xD0;
        contadori2c = 100;
        while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

        //I2C1STATbits.ACKSTAT

        //BYTE
        I2C1TRN = 0x00;
        contadori2c = 100;
        while(I2C1STATbits.TRSTAT && (contadori2c != 0) );

        I2C1CONbits.RSEN = 1;
        contadori2c = 100;
        while (I2C1CONbits.RSEN && (contadori2c != 0) );


        //BYTE
        I2C1TRN = 0xD1;
        contadori2c = 100;
        while(I2C1STATbits.TRSTAT && (contadori2c != 0) );





        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        segundo = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );


        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        minuto = I2C1RCV;

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );

        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        hora = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );

// ---------------------------
        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        //dia = I2C1RCV;
        diaDaSemana = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );

                //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        dia = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );

// ---------------------------
        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        mes = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 0;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );


        //LIGA RECEPCAO
        I2C1CONbits.RCEN = 1;
        //while(I2C1CONbits.RCEN);
        contadori2c = 100;
        while(!I2C1STATbits.RBF && (contadori2c != 0) );

        //RECEBE OS DADOS
        ano = I2C1RCV;
        //if ()

        //ACK
        I2C1CONbits.ACKDT = 1;
        I2C1CONbits.ACKEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.ACKEN && (contadori2c != 0) );



        //STOP
        I2C1CONbits.PEN = 1;
        contadori2c = 100;
        while(I2C1CONbits.PEN && (contadori2c != 0) );

    //}

        /*
    int segundo;
    int minuto;
    int hora;
    int dia;
    int mes;
    int ano;
*/


        segundo = conversao1307(segundo);
        minuto = conversao1307(minuto);
        hora = conversao1307(hora);
        dia = conversao1307(dia);
        mes = conversao1307(mes);
        ano = conversao1307(ano);

        gpsMinuto = minuto;
        gpsHora = hora;
        gpsDia = dia;
        gpsMes = mes;
        gpsAno = ano;


        sprintf(stringData,"%02d/%02d/20%02d",dia,mes,ano);
#ifdef VOLIT
        sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
#else
        sprintf(stringHora,"%02d:%02d:%02d",hora,minuto,segundo);
#endif

        if (diaDaSemana == 1)
            sprintf(strDiaDaSemana,"Sun");
        if (diaDaSemana == 2)
            sprintf(strDiaDaSemana,"Mon");
        if (diaDaSemana == 3)
            sprintf(strDiaDaSemana,"Tue");
        if (diaDaSemana == 4)
            sprintf(strDiaDaSemana,"Wed");
        if (diaDaSemana == 5)
            sprintf(strDiaDaSemana,"Thu");
        if (diaDaSemana == 6)
            sprintf(strDiaDaSemana,"Fri");
        if (diaDaSemana == 7)
            sprintf(strDiaDaSemana,"Sat");

        if (mes == 1)
            sprintf(strMes,"Jan");
        if (mes == 2)
            sprintf(strMes,"Feb");
        if (mes == 3)
            sprintf(strMes,"Mar");
        if (mes == 4)
            sprintf(strMes,"Abr");
        if (mes == 5)
            sprintf(strMes,"Mac");
        if (mes == 6)
            sprintf(strMes,"Jun");
        if (mes == 7)
            sprintf(strMes,"Jul");
        if (mes == 8)
            sprintf(strMes,"Agu");
        if (mes == 9)
            sprintf(strMes,"Sen");
        if (mes == 10)
            sprintf(strMes,"Oct");
        if (mes == 11)
            sprintf(strMes,"Nov");
        if (mes == 12)
            sprintf(strMes,"Dec");


}

void ObtemContadoresVolit (void){
    unsigned char rascunho;

    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_POR, &rascunho) != 0);
    ContadorDePOR = rascunho;
    ContadorDePOR = ContadorDePOR << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_POR, &rascunho) != 0);
    ContadorDePOR = ContadorDePOR + rascunho;

    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_BOR, &rascunho) != 0);
    ContadorDeBOR = rascunho;
    ContadorDeBOR = ContadorDeBOR << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_BOR, &rascunho) != 0);
    ContadorDeBOR = ContadorDeBOR + rascunho;
       
    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_EXTR, &rascunho) != 0);
    ContadorDeEXTR = rascunho;
    ContadorDeEXTR = ContadorDeEXTR << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_EXTR, &rascunho) != 0);
    ContadorDeEXTR = ContadorDeEXTR + rascunho;

    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_TRAPR, &rascunho) != 0);
    ContadorDeTRAPR = rascunho;
    ContadorDeTRAPR = ContadorDeTRAPR << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_TRAPR, &rascunho) != 0);
    ContadorDeTRAPR = ContadorDeTRAPR + rascunho;
    
    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_IOPUWR, &rascunho) != 0);
    ContadorDeIOPUWR = rascunho;
    ContadorDeIOPUWR = ContadorDeIOPUWR << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_IOPUWR, &rascunho) != 0);
    ContadorDeIOPUWR = ContadorDeIOPUWR + rascunho;
    
    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_SUCESSO_GPRS, &rascunho) != 0);
    ContadorDeEnviosBemSucedidosGPRS = rascunho;
    ContadorDeEnviosBemSucedidosGPRS = ContadorDeEnviosBemSucedidosGPRS << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_SUCESSO_GPRS, &rascunho) != 0);
    ContadorDeEnviosBemSucedidosGPRS = ContadorDeEnviosBemSucedidosGPRS + rascunho;
    
    while (LerDadosDaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_TENTATIVA_GPRS, &rascunho) != 0);
    ContadorDeTentativasDeEnvioGPRS = rascunho;
    ContadorDeTentativasDeEnvioGPRS = ContadorDeTentativasDeEnvioGPRS << 8;
    while (LerDadosDaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_TENTATIVA_GPRS, &rascunho) != 0);
    ContadorDeTentativasDeEnvioGPRS = ContadorDeTentativasDeEnvioGPRS + rascunho;
}

void SalvaCOntadoresVolit (void){
    unsigned char rascunho;
    unsigned int RascunhoInt;

    RascunhoInt = ContadorDePOR;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_POR, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_POR, rascunho);

    RascunhoInt = ContadorDeBOR;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_BOR, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_BOR, rascunho);

    RascunhoInt = ContadorDeEXTR;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_EXTR, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_EXTR, rascunho);

    RascunhoInt = ContadorDeTRAPR;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_TRAPR, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_TRAPR, rascunho);

    RascunhoInt = ContadorDeIOPUWR;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_IOPUWR, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_IOPUWR, rascunho);

    RascunhoInt = ContadorDeEnviosBemSucedidosGPRS;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_SUCESSO_GPRS, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_SUCESSO_GPRS, rascunho);

    RascunhoInt = ContadorDeTentativasDeEnvioGPRS;
    rascunho = (char)RascunhoInt;
    EscreverNaEEprom (ENDERECO_EEPROM_LSB_CONTADOR_TENTATIVA_GPRS, rascunho);
    rascunho = (char)(RascunhoInt >> 8);
    EscreverNaEEprom (ENDERECO_EEPROM_MSB_CONTADOR_TENTATIVA_GPRS, rascunho);
}

void AtualizaContadoresDeReset (void){
    ObtemContadoresVolit();

    if (_POR){
        ContadorDePOR = ContadorDePOR + 1;
    }

    if (_BOR){
        ContadorDeBOR = ContadorDeBOR + 1;
    }

    if (_EXTR){
        ContadorDeEXTR = ContadorDeEXTR + 1;
    }
;
    if (_TRAPR){
        ContadorDeTRAPR = ContadorDeTRAPR + 1;
    }

    if (_IOPUWR){
        ContadorDeIOPUWR = ContadorDeIOPUWR + 1;
    }

    SalvaCOntadoresVolit();
};

void ZeraContadoresDeReset (void){
    ContadorDePOR = 0;
    ContadorDeBOR = 0;
    ContadorDeEXTR = 0;
    ContadorDeTRAPR = 0;
    ContadorDeIOPUWR = 0;
    SalvaCOntadoresVolit();
}

void RegistraSucessoGPRS (void){
    ContadorDeEnviosBemSucedidosGPRS = ContadorDeEnviosBemSucedidosGPRS + 1;
    SalvaCOntadoresVolit();
}

void RegistraTentativaGPRS (void){
    ContadorDeTentativasDeEnvioGPRS = ContadorDeTentativasDeEnvioGPRS + 1;
    SalvaCOntadoresVolit();
}

void ZeraContadoresGPRS (void){
    ContadorDeEnviosBemSucedidosGPRS = 0;
    ContadorDeTentativasDeEnvioGPRS = 0;
    SalvaCOntadoresVolit();
}



