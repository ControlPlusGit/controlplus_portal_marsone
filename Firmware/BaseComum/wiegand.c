#include "platform.h"
#include "as3993_public.h"
#include "as3993.h"
#include "gen2.h"
#include "global.h"
#include "timer.h"
#include "appl_commands.h"
#include "tuner.h"

#define PULSO_WIEGAND 50 //Em Us
#define TEMPO_ENTRE_BITS_WIEGAND 2 //Em Um


void escreveStartWiegand(void){
    //ld_saidas(4,1);
    //delay_us (PULSO_WIEGAND);
    //ld_saidas(4,0);
    //delay_ms (TEMPO_ENTRE_BITS_WIEGAND);
}

void escreveStopWiegand(void){
    //ld_saidas(4,1);
    //delay_us (PULSO_WIEGAND);
    //ld_saidas(4,0);
    //delay_ms (TEMPO_ENTRE_BITS_WIEGAND);
}

void escreveNivelAltoWiegandPorPolling (void){
    //ld_saidas(4,1);
    //delay_us (PULSO_WIEGAND);
    //ld_saidas(4,0);
    //delay_ms (TEMPO_ENTRE_BITS_WIEGAND);
}

void escreveNivelBaixoWiegandPorPolling (void){
    //ld_saidas(7,1);
    //delay_us (PULSO_WIEGAND);
    //ld_saidas(7,0);
    //delay_ms (TEMPO_ENTRE_BITS_WIEGAND);
    
}

int calculaParidadeImpar(unsigned char *data){ //Even
    unsigned int Rascunho;
    int Contador;
    Rascunho = *data;
    Rascunho = Rascunho << 8;
    data = data + 1;
    Rascunho = Rascunho | *data;
    Rascunho = Rascunho >> 4;
    
    Contador = 0;
    for (;Rascunho != 0;Rascunho = Rascunho >> 1){
        if (Rascunho & 0x0001){
            Contador = Contador + 1;
        };
    };
    
    if (Contador & 0x0001){
        return 1;
    } else {
        return 0;
    }
}

int calculaParaidaPar(unsigned char *data){ //Odd
    unsigned int Rascunho;
    int Contador;
    data = data + 1;
    Rascunho = *data;
    Rascunho = Rascunho << 8;
    data = data + 1;
    Rascunho = Rascunho | *data;
    Rascunho = Rascunho & 0x0FFF;
    
    Contador = 0;
    for (;Rascunho != 0;Rascunho = Rascunho >> 1){
        if (Rascunho & 0x0001){
            Contador = Contador + 1;
        };
    };
    
    if (Contador & 0x0001){
        return 0;
    } else {
        return 1;
    }
}

void enviaByteWiegand(unsigned char dado){
    unsigned char Contador;
    
    for (Contador = 0x80;Contador != 0;Contador = Contador >> 1){
        if (Contador & dado){
            escreveNivelAltoWiegandPorPolling();
        } else {
            escreveNivelBaixoWiegandPorPolling();
        }
    }
    
}

void enviaWiegand(unsigned char *data, int Tamanho){
    int Contador;
    
    if (calculaParidadeImpar(data)){
        escreveNivelAltoWiegandPorPolling();
    } else {
        escreveNivelBaixoWiegandPorPolling();
    }
    

    for(Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        enviaByteWiegand(*data);
        data = data + 1;
    }
    
    data = data - 1;
    data = data - 1;
    data = data - 1;
    
    if (calculaParaidaPar(data)){
        escreveNivelAltoWiegandPorPolling();
    } else {
        escreveNivelBaixoWiegandPorPolling();
    }
    
    delay_ms(50);
}
