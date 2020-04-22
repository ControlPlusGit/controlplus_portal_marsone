/* 
 * File:   gps.h
 * Author: gilson
 *
 * Created on 6 de Junho de 2017, 14:57
 */

#ifndef GPS_H
#define	GPS_H

#ifdef	__cplusplus
extern "C" {
#endif

int pegarVelocidadeDoGPS (char dado);
void iniciaVelocidadeGps(void);
void operacacoesEmTickParaGps (void);
void atualizaLedGps(void);

extern char SubStringVelocidade[6];
extern float VelocidadeFinal;

#define ESTADO_LED_DESLIGADO_GPS            0
#define ESTADO_LED_PISCANDO__ACESSO_GPS     1
#define ESTADO_LED_PISCANDO__APAGADO_GPS    2
#define ESTADO_LED_LIGADO_GPS               3
extern int EstadoLedGps;



#ifdef	__cplusplus
}
#endif

#endif	/* GPS_H */

