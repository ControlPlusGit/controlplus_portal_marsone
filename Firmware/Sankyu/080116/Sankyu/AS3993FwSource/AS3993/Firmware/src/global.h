
#define PODE_SER_ALPHA 1

#define ARCELOR 1//CLIENTE

#elif ARCELOR //CLIENTE

#define JUIZ_DE_FORA  1//LUGAR

#ifdef JUIZ_DE_FORA

#define PORTAL 1//PRODUTO

#ifdef PORTAL
//Configuracoes extras
#define PORTAL_COM_ETH 1
//#define WIFI 1
//#define INTERFACE_DE_AJUSTES 1

#define QUATRO_ANTENAS 1//CONFIGURACAO

#ifdef DUAS_ANTENAS
// **** DEFINE A SER USADO **** //
#define ARCELOR_JUIZ_DE_FORA_PORTAL_DUAS_ANTENAS 1
#elif QUATRO_ANTENAS
// **** DEFINE A SER USADO **** //
#define ARCELOR_JUIZ_DE_FORA_PORTAL_QUATRO_ANTENAS 1
#endif

#endif

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/** Defines uC used clock frequency */
#define SYSCLK_16MHZ                16000000ULL
#define SYSCLK_12MHZ                12000000ULL
#define SYSCLK_8MHZ                 8000000ULL
#define SYSCLK_4MHZ                 4000000ULL

#define SYSCLK    SYSCLK_16MHZ

#define FCY    (SYSCLK)

#include "as3993_config.h"
#include "ams_types.h"

/** Definition high */
#define HIGH                      1

/** Definition all bits low */
#define LOW                       0

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

void bin2Chars(int value, unsigned char *destbuf);
void bin2Hex(char value, unsigned char *destbuf);
void u32ToEbv(u32 value, u8 *ebv, u8 *len);
void insertBitStream(u8 *dest, u8 const *source, u8 len, u8 bitpos);
u32 readU32FromLittleEndianBuffer(u8 const *buffer);
void recebeOperador (u8 dado);
//void resetContadores (void);
void bloqueia_frente(void);
void bloqueia_reh(void);
void ld_saidas(int saida, int status);
void sel_led(int led, int status);
//void escolhe_antena(void);

extern volatile int operador_atual_outro;


/** Definition for the maximal EPC length */
#define EPCLENGTH              12 //32  // number of bytes for EPC, standard allows up to 62 bytes
/** Definition for the PC length */
#define PCLENGTH                2
/** Definition for the CRC length */
#define CRCLENGTH               2
/** Definition of the maximum frequencies for the hopping */
#define MAXFREQ                 53
/** Definition of the maximum tune entries in tuning table */
#define MAXTUNE                 52
/** Definition of the maximum number of tags, which can be read in 1 round */
//#define MAXTAG 255
#define MAXTAG 12//20//120

#endif
