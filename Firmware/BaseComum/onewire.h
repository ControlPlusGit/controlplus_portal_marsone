/* 
 * File:   onewire.h
 * Author: none
 *
 * Created on 17 de Julho de 2015, 17:52
 */

#ifndef ONEWIRE_H
#define	ONEWIRE_H

#ifdef	__cplusplus
extern "C" {
#endif


#define PINO_ONE_WIRE _LATB4
#define DIRECAO_ONE_WIRE _TRISB4

#define PINO_COMO_ENTRADA 1
#define PINO_COMO_SAIDA 0
#define TAMANHO_ID 8

#define OITO 8
#define MASCARA_OITAVO_BIT 0x80

#define COMANDO_LER_ROM_DS2401 0x33

#define DELAY_RESET_DS2401 500
#define DELAY_BIT_DS2401 60
#define DELAY_PAUSA_DS2401 10



void ObtemID (char *string);
void IniciaComunicaoOneWire (void);

#ifdef	__cplusplus
}
#endif

#endif	/* ONEWIRE_H */

