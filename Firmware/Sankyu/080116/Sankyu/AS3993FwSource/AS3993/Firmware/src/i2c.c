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

//#include "varal.h"
extern char StringResultadoValidadeGPS[];
extern char empilhadeira[];

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

extern char num_serie[];
extern char stringResultadoLatitudeGPS[];
extern char stringResultadoLongitudeGPS[];
int contadori2c;

int gpsMinuto;
int gpsHora;
int gpsDia;
int gpsMes;
int gpsAno;

char strDiaDaSemana[10];
char strMes[10];
char strAno[10];

//char stringData[20];
char stringHora[20];
int minuto;

volatile unsigned long epochAtual;
unsigned long epoch;

char EpochIniciado;

int registrosNaEeprom; //
int QuantasPossicaoEstaoOcupadas (void);

// ****************************************************************************
// Estrutura de dados na EEPROM externa
// Os 256 iniciais sao de uso livre, partindo deles, vou usa-los como buffer nao volatil
// ****************************************************************************

#define FIM_DA_MEMORIA_LIVRE 256
#define REGIAO_EPOCH_NV 1024

#define INICIO_DO_BUFFER_NAO_VOLATIU (FIM_DA_MEMORIA_LIVRE + REGIAO_EPOCH_NV)


//Estruturas da entrandas no buffer
// 1 byte que indica a ocupacao, 0 = livre, 0 != ocupado para acelecar as indexacoes.
// Cada entrada se consiste de ate 127 bytes.
// Como estou usando uma 24LC256 tenho 32K dividido em blocos de 128 em 256 registros.

#define TAMANHO_DA_MEMORIA 32727
#define TAMANHO_DA_MEMORIA_SEM_A_AREA_LIVRE (TAMANHO_DA_MEMORIA - (INICIO_DO_BUFFER_NAO_VOLATIU))
#define TAMANHO_DO_REGISTRO 128
#define LIMITE_DA_MEMORIA (TAMANHO_DA_MEMORIA_SEM_A_AREA_LIVRE / (TAMANHO_DO_REGISTRO))

int LimpaTodaAEEprom(void){
    int posicao;
    char nullBuffer[64];
    
    memset(nullBuffer,0,64);
    
    /*
    for(posicao = 0;posicao < 32767;posicao = posicao + 1){
        if (EscreverNaEEprom(posicao, 0) != 0)
            return -1;
    }
    */
        

    for(posicao = 0;posicao < 32500;posicao = posicao + 64){
        if (EscreverStringNaEEprom (posicao, nullBuffer, 64) != 0)
            return -1;
    }
           
    return 0;
    
}

int ProcuraPosicaoLivre (void){
    char dado;
    int endereco;
    int resultado;
    //LerDadosDaEEprom (int endereco, unsigned char *dado)
    
    for(endereco = (INICIO_DO_BUFFER_NAO_VOLATIU  + (registrosNaEeprom * TAMANHO_DO_REGISTRO) );endereco < TAMANHO_DA_MEMORIA;endereco = endereco + TAMANHO_DO_REGISTRO){
    //for(endereco = INICIO_DO_BUFFER_NAO_VOLATIU;endereco < TAMANHO_DA_MEMORIA;endereco = endereco + TAMANHO_DO_REGISTRO){
    //for(endereco = 0;endereco < TAMANHO_DA_MEMORIA;endereco = endereco + TAMANHO_DO_REGISTRO){
        resultado = LerDadosDaEEprom(endereco,&dado);
        if (resultado == 0){
            if (dado == 0)
                return endereco;
        } else {
            return -2;//falha na memoria
        }
    }
    return -1;//sem posicao livre
}

int GravarEventoNV (char *dados, int tamanhoDoEvento){
    int posicao;
    
    posicao = ProcuraPosicaoLivre();
    
    //PassadoXTempoDoUltimoEvento(posicao, 60);
    
    if (posicao >= 0){
        if (EscreverStringNaEEprom(posicao, dados, tamanhoDoEvento) != 0){
            return -1;
        }
        registrosNaEeprom = registrosNaEeprom + 1;
        //int EscreverNaEEprom (int endereco, unsigned char dado);
        /*
        while(*dados){
            if (EscreverNaEEprom((posicao), *dados) != 0){
                return -1;//Falha na escrita
            }
            dados = dados + 1;
            posicao = posicao + 1;
        }
        */
    } else {
        return -2;//Nao ha espaco livre
    }
    return 0;//Sucesso
}

int QuantasPossicaoEstaoOcupadas (void){
    char dado;
    int endereco;
    int registros;
    
    registros = 0;
    
    //LerDadosDaEEprom (int endereco, unsigned char *dado)
    for(endereco = INICIO_DO_BUFFER_NAO_VOLATIU;endereco < (TAMANHO_DA_MEMORIA - (TAMANHO_DO_REGISTRO * 2));endereco = endereco + TAMANHO_DO_REGISTRO){
        if (LerDadosDaEEprom(endereco,&dado) == 0){
            if (dado != 0)
                registros = registros + 1;
        } else {
            return -1;//falha na memoria
        }
    }

    return registros;
}


int ProcuraPossicaoOcupada (void){
    char dado;
    int endereco;
    
    if (registrosNaEeprom == 0){
        return -1;//sem posicao ocupada
    }
    //LerDadosDaEEprom (int endereco, unsigned char *dado)
    for(endereco = INICIO_DO_BUFFER_NAO_VOLATIU;endereco < (TAMANHO_DA_MEMORIA - (TAMANHO_DO_REGISTRO * 2));endereco = endereco + TAMANHO_DO_REGISTRO){
        if (LerDadosDaEEprom(endereco,&dado) == 0){
            if (dado != 0)
                return endereco;
        } else {
            return -2;//falha na memoria
        }
    }
    registrosNaEeprom = 0;//
    return -1;//sem posicao ocupada
}

int ApagaEventoDaMemoriaNV (int endereco){
    //EscreverNaEEprom(posicao, *dados)
    int contador;
    char nullBuffer[64];
    
    memset(nullBuffer,0,64);
    if (EscreverStringNaEEprom (endereco, nullBuffer, 64) == 0){
        registrosNaEeprom = registrosNaEeprom - 1;
        return 0;
        
    }
        
    
    return -1;
    

    /*
    for(contador = 0;contador < TAMANHO_DO_REGISTRO;contador = contador + 1){
        EscreverNaEEprom((endereco + contador), 0);
    }
    */
}

/*
int RetornaPossicaoDeEnderecoEpochDoEnderecoDoEventoNV(int endereco){
    int resultado;
    int rascunho;
    
    rascunho = endereco - INICIO_DO_BUFFER_NAO_VOLATIU;
    rascunho = rascunho / TAMANHO_DO_REGISTRO;
    resultado = rascunho * 4;
    resultado = resultado + REGIAO_EPOCH_NV;
    
    return resultado;
}
*/

int RetornaPossicaoDeEnderecoEpochDoEnderecoDoEventoNV(int endereco){
    int resultado;
    int rascunho;
    
    rascunho = endereco - INICIO_DO_BUFFER_NAO_VOLATIU;
    rascunho = rascunho / TAMANHO_DO_REGISTRO;
    resultado = rascunho * 4;
    resultado = resultado + FIM_DA_MEMORIA_LIVRE;
    
    return resultado;
}



int LimpaEpoch (int posicao){
    int endereco;
    endereco = RetornaPossicaoDeEnderecoEpochDoEnderecoDoEventoNV(posicao);

    LerDadosDaEEprom(endereco, 0);
    LerDadosDaEEprom((endereco + 1),0);
    LerDadosDaEEprom((endereco + 2),0);
    LerDadosDaEEprom((endereco + 3),0);
    
    return 0;
}

int SalvaEpoch (int posicao){
    int endereco;
    unsigned char dados;
    int rascunho;    
    
    //endereco = (posicao - INICIO_DO_BUFFER_NAO_VOLATIU);
    //EscreverNaEEprom((endereco + contador), 0);
//INICIO_DO_BUFFER_NAO_VOLATIU
//REGIAO_EPOCH_NV
    
    posicao = posicao + INICIO_DO_BUFFER_NAO_VOLATIU;
    
    endereco = RetornaPossicaoDeEnderecoEpochDoEnderecoDoEventoNV(posicao);

    //LerDadosDaEEprom(endereco,dado);
    
    dados = (unsigned char)epochAtual;
    EscreverNaEEprom(endereco, dados);
    dados = (unsigned char)(epochAtual >> 8);
    EscreverNaEEprom((endereco + 1), dados);
    dados = (unsigned char)(epochAtual >> 16);
    EscreverNaEEprom((endereco + 2), dados);
    dados = (unsigned char)(epochAtual >> 24);
    EscreverNaEEprom((endereco + 3), dados);
}

//A posicao eh em funcao do endereco em que o evento foi gravado
unsigned long RetornaEpochDePosicaoNV (int posicao){
    int endereco;
    unsigned long rascunho;
    unsigned char dados;
    unsigned long resultado;
    
    posicao = posicao + INICIO_DO_BUFFER_NAO_VOLATIU;
    
    endereco = RetornaPossicaoDeEnderecoEpochDoEnderecoDoEventoNV(posicao);

    //LerDadosDaEEprom(endereco,dado);
    
    LerDadosDaEEprom(endereco, &dados);
    resultado = dados;
    LerDadosDaEEprom((endereco + 1), &dados);
    rascunho = dados;
    rascunho = rascunho << 8;
    resultado = resultado + rascunho;
    LerDadosDaEEprom((endereco + 2), &dados);
    rascunho = dados;
    rascunho = rascunho << 16;
    resultado = resultado + rascunho;
    LerDadosDaEEprom((endereco + 3), &dados);
    rascunho = dados;
    rascunho = rascunho << 24;
    resultado = resultado + rascunho;
    
    return resultado;
    
}

int PassadoXTempoDoUltimoEvento(int posicao, int tempo){
    unsigned long epoch, rascunho;
    epoch = RetornaEpochDePosicaoNV(posicao);
    rascunho = epochAtual - epoch;
    if (rascunho > tempo)
        return 1;
    else return 0;
}


int escreverNoBufferDeFtpNV(char *tag, char *operador, int evento, int antena, int rssi){
    char Buffer[129];
    Buffer[128] = 0;
    int resultado;
    int tamanhoDaString;
    stringData[2] = ' ';
    stringData[5] = ' ';
 #ifdef AMBEV
        sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
#elif SANKYU
        sprintf(bufferFTP[posicao],"1S2,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
#elif K2TEC
        sprintf(Buffer,"1S4,%s,%s,%s,%d,%s,%d,%d,%s,%s,%s,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador,stringResultadoLatitudeGPS,stringResultadoLongitudeGPS);
        //sprintf(bufferFTP[posicao],"1S4,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
#elif ALPHAHEAD
        sprintf(bufferFTP[posicao],"1S5,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
#elif PORTAL_FRANGO
        sprintf(bufferFTP[posicao],"1S5,%s,%s,%s,%d,%s,%d,%d,%s,0,0,0,0,0,0,0,0,0,0,0,0,*1D\n", num_serie, stringData, stringHora, evento, tag, rssi, antena, operador);
#elif VOLIT

        /*
        //Esse trecho eh para quando uso o espaco EDS, e o ANSI-C nao suporta ele.
        sprintf(rascunho,"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        for (contador = 0;(contador < TAMANHO_BUFFER_FTP) && (bufferFTP[posicao][contador] != 0);contador = contador + 1){
            bufferFTP[posicao][contador] = rascunho[contador];
        }
         * */

        sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,%u,%u,%u,%u,%u,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s,Resets,%u,%u,%u,%u,%u,GPRS,%u,%u\r\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS,ContadorDePOR,ContadorDeBOR,ContadorDeEXTR,ContadorDeTRAPR,ContadorDeIOPUWR,ContadorDeEnviosBemSucedidosGPRS,ContadorDeTentativasDeEnvioGPRS);
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS);
        //sprintf(bufferFTP[posicao],"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);

#else
        if (StringResultadoValidadeGPS[0] == 'A'){
            sprintf(Buffer,"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,$GPRMC,173114.000,A,2332.7021,S,04639.7523,W,0.00,,250814,,*1D\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador);
        } else {
            sprintf(Buffer,"1S1,%s,%d,%d,%s,%s,1,%d,%s,%s,%s,%s\n", tag, rssi, antena, stringData, stringHora, evento, empilhadeira, operador,stringResultadoLatitudeGPS, stringResultadoLongitudeGPS);
        }

#endif
//#ifdef K2TEC
//    sprintf(Buffer,"1S4,%s,%s,%s,%d,%s,%d,%d,%s,%s,%s,*1D\n", "0010", stringData, stringHora, evento, tag, rssi, antena, operador,"","");    
    tamanhoDaString = strlen(Buffer);
    
    if (tamanhoDaString > 63){
        resultado = GravarEventoNV(Buffer, 64);
        resultado = resultado | GravarEventoNV(&Buffer[64], (tamanhoDaString - 64));
    } else {
        resultado = GravarEventoNV(Buffer, tamanhoDaString);
    }
    
    
    
//#endif
    if (resultado == 0)
       return 0;//Sucesso
    return resultado;//Ocorreu alguma falha no gravacao do evento na memoria NV.

}


int RetornaStringDeEventoNV (char *dados){
    int posicao;
    int contador;
    char x;
    posicao = ProcuraPossicaoOcupada();
    
    if ((posicao >= INICIO_DO_BUFFER_NAO_VOLATIU) && (posicao < TAMANHO_DA_MEMORIA)){
        //int LerDadosDaEEprom (int endereco, unsigned char *dado){
        
        
        /*
        if (LerDadosDaEEprom((posicao), &x) == 0)
            *dados = x; 
        else 
            *dados = 0; 
        
        contador = 1;
        */
        
        LerStringDaEEprom((posicao), dados, TAMANHO_DO_REGISTRO);
        /*
        while((contador < TAMANHO_DO_REGISTRO) && (*dados != 0)){
            if (LerDadosDaEEprom((posicao + contador), &x) == 0){
                dados = dados + 1;
                *dados = x;
                contador = contador + 1;
            } else {
                return -1;//Falha na EEPROM
            }
        }
        */
        
        asm("NOP");
        ApagaEventoDaMemoriaNV(posicao);
        return 1;
    }
    return 0;//Todas as possicoes foram enviadas ou a memoria esta livre
}


void LigaI2CMemoria (void){
    I2C3BRG = (VELOCIDADE_I2C / (SYSCLK * 2) ) - 1;
    I2C3CONbits.I2CEN = 1;
    
    registrosNaEeprom = QuantasPossicaoEstaoOcupadas();
}


void LigaI2CRelogio (void){
    I2C1BRG = (VELOCIDADE_I2C / (SYSCLK * 2) ) - 1;
    I2C1CONbits.I2CEN = 1;
    EpochIniciado = 0;
}

int EscreveArray (int endereco, char *dados, char quantia){
    int contador;
    int enderecoTemporario;

    enderecoTemporario = endereco;
    for (contador = 0;contador < quantia;contador = contador + 1){
        EscreverNaEEprom(enderecoTemporario, *dados);
        dados = dados + 1;
    }
    return 0;
}


unsigned long RetornaEpoch(void){
    return epochAtual;
}

int EscreverStringNaEEprom (int endereco, unsigned char *dados, char limite){
    unsigned char temporario;
    int contador;
    
    
    if (limite > 64)return -1;//escrita alem do limite
        
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
    for(contador = 0;contador < limite;contador = contador + 1){
        I2C3TRN = *dados;
        dados = dados + 1;
        contadori2c = 100;
        while(I2C3STATbits.TRSTAT && (contadori2c != 0) );
    }

    //STOP
    I2C3CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.PEN && (contadori2c != 0) );

    delay_ms(5);
    /*
    delay_ms(1);//5
    delay_us(400);
    */
    return 0;// OK

    return -1; //Erro
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
    //delay_ms(1);//5
    //delay_us(400);
    return 0;// OK

    return -1; //Erro
}


unsigned char ReadComAckNaI2c (unsigned char ack){ // 0 = ack, 1 = NACK
    unsigned char dado;
    //LIGA RECEPCAO
    I2C3CONbits.RCEN = 1;
    //while(I2C1CONbits.RCEN);
    contadori2c = 100;
    while(!I2C3STATbits.RBF && (contadori2c != 0));
    
    dado = I2C3RCV;
    
    I2C3CONbits.ACKDT = ack;
    I2C3CONbits.ACKEN = 1;
    
    while(I2C3CONbits.ACKEN == 1);
    
    delay_us(10);
    
    return dado;
    
}


int LerStringDaEEprom (int endereco, unsigned char *dado, int limite){
    char ack;
    int contador;
    unsigned char temporario;
    
    ack = 0;
    
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

    /*
    //LIGA RECEPCAO
    I2C3CONbits.RCEN = 1;
    //while(I2C1CONbits.RCEN);
    contadori2c = 100;
    while(!I2C3STATbits.RBF && (contadori2c != 0) );
  
    //RECEBE OS DADOS
    //ack = ack | I2C3STATbits.ACKSTAT;
    *dado = I2C3RCV;
    */
    
    for (contador = 0;contador < (limite - 1);contador = contador + 1){
        *dado = ReadComAckNaI2c(0);
        
        if (*dado == 0){
            contador = limite;
        }
        
        dado = dado + 1;

    }
    
    (void)ReadComAckNaI2c(1);//leitura vazia para fechar o comando na EEPROM
    
    //STOP
    I2C3CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.PEN && (contadori2c != 0) );
    
    I2C3CONbits.RCEN = 0;


    delay_ms(5);
    //delay_ms(1);//5
    //delay_us(400);

    if (ack == 0)
        return 0;// OK
    else
        return -1; //Erro
}


int LerDadosDaEEprom (int endereco, unsigned char *dado){
    char ack;
    unsigned char temporario;
    
    ack = 0;
    
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
    
    //ACK
    I2C3CONbits.ACKDT = 1;
    I2C3CONbits.ACKEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.ACKEN && (contadori2c != 0) );
    

    //STOP
    I2C3CONbits.PEN = 1;
    contadori2c = 100;
    while(I2C3CONbits.PEN && (contadori2c != 0) );


    delay_ms(5);
    //delay_ms(1);//5
    //delay_us(400);

    if (ack == 0)
        return 0;// OK
    else
        return -1; //Erro
}





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

char conversao1307 (char dado){
    char rascunho;
    char resultado;

    rascunho = dado;
    rascunho = rascunho >> 4;
    rascunho = rascunho * 10;
    resultado = rascunho + (dado & 0x0F);

    return resultado;

}

unsigned long geraEpoch(int segundo, int minuto, int hora, int dia, int mes, int ano){
    unsigned long epoch;
    epoch = segundo;
    epoch = epoch + (minuto * 60);
    epoch = epoch + (hora * 3600);
    epoch = epoch + (dia * 86400);
    epoch = epoch + (mes * 2629743);
    ano = ano - 1970;
    epoch = epoch + (ano * 31556926);
    return epoch;
}         

void TickEpoch (void){
    epochAtual = epochAtual + 1;
}

/*
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
        
        // ***************************
        // Para registros de eventos NV 29/02/16
        // ***************************
        if (EpochIniciado == 0){
            epochAtual = geraEpoch(segundo, minuto, hora, dia, mes, (2000 + ano));
            EpochIniciado = 1;
        }
        // ***************************
        
        
        
        
        sprintf(strAno, "%d", ano);
#ifdef VOLIT
        sprintf(strAno, "20%d", ano);
#endif


        sprintf(stringData,"%02d/%02d/20%02d",dia,mes,ano);
        
#ifdef VOLIT
        //sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
        sprintf(stringHora,"%02d:%02d:%02d",hora,minuto,segundo);
#else
        sprintf(stringHora,"%02d:%02d:%02d",hora,minuto,segundo);
        sprintf(stringHora,"%02d_%02d_%02d",hora,minuto,segundo);
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
            //sprintf(strMes,"Jan");
            sprintf(strMes,"01");
        if (mes == 2)
            //sprintf(strMes,"Feb");
            sprintf(strMes,"02");
        if (mes == 3)
            //sprintf(strMes,"Mar");
            sprintf(strMes,"03");
        if (mes == 4)
            //sprintf(strMes,"Abr");
            sprintf(strMes,"04");
        if (mes == 5)
            //sprintf(strMes,"Mac");
            sprintf(strMes,"05");
        if (mes == 6)
            //sprintf(strMes,"Jun");
            sprintf(strMes,"06");
        if (mes == 7)
            //sprintf(strMes,"Jul");
            sprintf(strMes,"07");
        if (mes == 8)
            //sprintf(strMes,"Agu");
            sprintf(strMes,"08");
        if (mes == 9)
            //sprintf(strMes,"Sen");
            sprintf(strMes,"09");
        if (mes == 10)
            //sprintf(strMes,"Oct");
            sprintf(strMes,"10");
        if (mes == 11)
            //sprintf(strMes,"Nov");
            sprintf(strMes,"11");
        if (mes == 12)
            //sprintf(strMes,"Dec");
            sprintf(strMes,"12");


}
*/

//Rotina do Gabriel
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

        sprintf(stringData,"%02d/%02d/%02d",dia,mes,ano);
    #ifdef VOLIT
        sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
    #elif K2TEC
        sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
    #elif ALPHAHEAD
        sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
    #elif PORTAL_FRANGO
        sprintf(stringHora,"%02d%02d%02d",hora,minuto,segundo);
    #else
        sprintf(stringHora,"%02d:%02d:%02d",hora,minuto,segundo);
    #endif
    #ifdef ALPHAHEAD
    if (diaDaSemana == 1)
        sprintf(strDiaDaSemana,"Dom");
    else if (diaDaSemana == 2)
        sprintf(strDiaDaSemana,"Seg");
    else if (diaDaSemana == 3)
        sprintf(strDiaDaSemana,"Ter");
    else if (diaDaSemana == 4)
        sprintf(strDiaDaSemana,"Qua");
    else if (diaDaSemana == 5)
        sprintf(strDiaDaSemana,"Qui");
    else if (diaDaSemana == 6)
        sprintf(strDiaDaSemana,"Sex");
    else if (diaDaSemana == 7)
        sprintf(strDiaDaSemana,"Sab");
    else
        sprintf(strDiaDaSemana,"Sab");

    if (mes == 1)
        sprintf(strMes,"Jan");
    else if (mes == 2)
        sprintf(strMes,"Fev");
    else if (mes == 3)
        sprintf(strMes,"Mar");
    else if (mes == 4)
        sprintf(strMes,"Abr");
    else if (mes == 5)
        sprintf(strMes,"Mai");
    else if (mes == 6)
        sprintf(strMes,"Jun");
    else if (mes == 7)
        sprintf(strMes,"Jul");
    else if (mes == 8)
        sprintf(strMes,"Ago");
    else if (mes == 9)
        sprintf(strMes,"Set");
    else if (mes == 10)
        sprintf(strMes,"Out");
    else if (mes == 11)
        sprintf(strMes,"Nov");
    else if (mes == 12)
        sprintf(strMes,"Dez");
    else
        sprintf(strMes,"Mai");
    #endif
#ifndef ALPHAHEAD
    if (diaDaSemana == 1)
        sprintf(strDiaDaSemana,"Sun");
    else if (diaDaSemana == 2)
        sprintf(strDiaDaSemana,"Mon");
    else if (diaDaSemana == 3)
        sprintf(strDiaDaSemana,"Tue");
    else if (diaDaSemana == 4)
        sprintf(strDiaDaSemana,"Wed");
    else if (diaDaSemana == 5)
        sprintf(strDiaDaSemana,"Thu");
    else if (diaDaSemana == 6)
        sprintf(strDiaDaSemana,"Fri");
    else if (diaDaSemana == 7)
        sprintf(strDiaDaSemana,"Sat");
    else
        sprintf(strDiaDaSemana,"Sun");

    if (mes == 1)
        sprintf(strMes,"Jan");
    else if (mes == 2)
        sprintf(strMes,"Feb");
    else if (mes == 3)
        sprintf(strMes,"Mar");
    else if (mes == 4)
        sprintf(strMes,"Abr");
    else if (mes == 5)
        sprintf(strMes,"Mac");
    else if (mes == 6)
        sprintf(strMes,"Jun");
    else if (mes == 7)
        sprintf(strMes,"Jul");
    else if (mes == 8)
        sprintf(strMes,"Agu");
    else if (mes == 9)
        sprintf(strMes,"Sen");
    else if (mes == 10)
        sprintf(strMes,"Oct");
    else if (mes == 11)
        sprintf(strMes,"Nov");
    else if (mes == 12)
        sprintf(strMes,"Dec");
    else
        sprintf(strMes,"Mac");
#endif
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