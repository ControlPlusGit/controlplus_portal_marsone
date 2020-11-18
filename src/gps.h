/* 
 * File:   gps.h
 * Author: none
 *
 * Created on 27 de Fevereiro de 2015, 14:39
 */

#ifndef GPS_H
#define	GPS_H

#ifdef	__cplusplus
extern "C" {
#endif

void ProcessaGPS(void); //Apenas para testes
void ProcessaNaInterrupcaoDeUart(char dado);

void iniciaGPS (void);

#define TAMANHO_MAXIMO_STRING_VALIDADE_GPS 5
#define TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS 10
#define TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS 50


extern char StringResultadoValidadeGPS[TAMANHO_MAXIMO_STRING_VALIDADE_GPS];
extern char stringResultadoLatitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];
extern char stringResultadoLongitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];


int pegarVelocidadeDoGPS (char dado);
void iniciaVelocidadeGps(void);
void operacacoesEmTickParaGps (void);
void atualizaLedGps(void);

extern char SubStringVelocidade[6];
//extern float VelocidadeFinal;
extern int VelocidadeFinal;

void recebeDadosDoGps(unsigned char Dado);

#define ESTADO_LED_DESLIGADO_GPS            0
#define ESTADO_LED_PISCANDO__ACESSO_GPS     1
#define ESTADO_LED_PISCANDO__APAGADO_GPS    2
#define ESTADO_LED_LIGADO_GPS               3
extern int EstadoLedGps;


#define LIMITE_DE_VELOCIDADE_GPS 7

#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

