#include "GenericTypeDefs.h"
#include "platform.h"
#include "timer.h"
#include "onewire.h"

void IniciaComunicaoOneWire (void);
void EscreveByteOneWire (int);
//int LeByteOneWire (void);
unsigned char LeByteOneWire (int);

int id_byte_um;
//O BYTE 1 eh o ID da familia do DS2401
//O BYTE 7 eh o CRC da comunicacao
//O ID eh composto do BYTE 1 ao 6

void ObtemID (char *string){
    unsigned char ID[TAMANHO_ID];
    int contador;

    IniciaComunicaoOneWire();
    EscreveByteOneWire(COMANDO_LER_ROM_DS2401);

    for (contador = 0;contador < OITO;contador = contador + 1){
        ID[contador] = LeByteOneWire(contador);
    }
    id_byte_um = ID[1] ;
    sprintf(string,"%02x%02x%02x%02x%02x%02x",ID[6],ID[5],ID[4],ID[3],ID[2],ID[1]);

    delay_us(DELAY_PAUSA_DS2401); //apenas para BREAKPOINT
}

void IniciaComunicaoOneWire (void){
    PINO_ONE_WIRE = 0;

    DIRECAO_ONE_WIRE = PINO_COMO_SAIDA;
    delay_us(DELAY_RESET_DS2401);

    DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;
    delay_us(DELAY_RESET_DS2401);
}

void EscreveByteOneWire (int dado){
    int contador;
    char mascara;

    mascara = 1;
    PINO_ONE_WIRE = 0;

    for (contador = 0;contador < OITO;contador = contador + 1){
        DIRECAO_ONE_WIRE = PINO_COMO_SAIDA;
        delay_us(DELAY_PAUSA_DS2401);

        if (dado & 0x01) {
            DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;
        } else {
            DIRECAO_ONE_WIRE = PINO_COMO_SAIDA;
        }
        delay_us(DELAY_BIT_DS2401);
        dado = dado >> 1;

        DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;
        delay_us(DELAY_PAUSA_DS2401);
    }

    DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;
    delay_us(DELAY_PAUSA_DS2401);
}

unsigned char LeByteOneWire (int dado){
    int contador;
    unsigned char rascunho;

    DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;

    rascunho = 0;

    for (contador = 0;contador < OITO;contador = contador + 1){
        PINO_ONE_WIRE = 0;
        DIRECAO_ONE_WIRE = PINO_COMO_SAIDA;
        delay_us(2);
        DIRECAO_ONE_WIRE = PINO_COMO_ENTRADA;

        delay_us(DELAY_PAUSA_DS2401);

        rascunho = rascunho >> 1;
        
        if (_RB4){
            rascunho = rascunho + MASCARA_OITAVO_BIT;
        }
        delay_us(DELAY_BIT_DS2401);
    }

    delay_us(DELAY_BIT_DS2401); // apenas para BRAKPOINT
    return rascunho;
    
}