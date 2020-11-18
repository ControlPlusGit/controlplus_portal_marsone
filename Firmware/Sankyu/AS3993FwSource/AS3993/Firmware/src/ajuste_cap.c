

#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "as3993_config.h"
#include "platform.h"
#include "perifericos.h"
#include "stream_dispatcher.h"
#include "usb_hid_stream_driver.h"
#include "logger.h"
#include "uart_driver.h"
#include "uart.h"
#include "errno.h"
#include "as3993_public.h"
#include "as3993.h"
#include "gen2.h"
#include "global.h"
#include "timer.h"
#include "appl_commands.h"
#include "tuner.h"
//#include "gprs.h"
//#include "varal.h"
//#include "sankyu.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\onewire.h"
//#include "lista.h"
//#include "auto_teste.h"
#include "config_i2c3.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\mem_i2c_24LC256.h"
#include "tags.h"
#include "empilhadeira.h"
#include "i2c.h"
//#include "zigbee.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\zigbee.h"
//#include "gps.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\rtc.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\exclusao.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\autoteste.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\wifi.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\eth.h"

#include "C:\Projetos\control-plus\Firmware\BaseComum\portal.h"

#include "C:\projetos\control-plus\Firmware\Sankyu\AS3993FwSource\AS3993\Firmware\src\global.h"
//#include "C:\Projetos\control-plus\Firmware\BaseComum\cancelas.h"
#include "as3993.h"
#include "setup_usb.h"

extern TunerConfiguration mainTuner;


void iniciaVarreduraDeCapacitores(void){
    AtrasoParaDegradarLeitura = 0;
    setaSensibilidade(125);
    NumeroDeAntenasLidas = 4;//Normalmente com 2 antenas
    Frequencia = 23;
    RepeticaoNaLeitura = 4;
    SetaFrequencias();
    //Frequencies.numFreqs = 0;
    //TempoParaInatividadeDeTagMonitorada = 7;//7;
    //PortaDePortalDeExclusao = 1;
    
    //Frequencies.freq[0] = 916000; //teste do ajuste automatico
    //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    
}



void buscaTagPorVarreduraDeCapacitores(void){
    int cap0, cap1, cap2;
    int QuantiaDeTagsObtidas;
    char mensagem[100];
//    int config;
    int Antena;
    int timeout;
    int contador;
    
    Antena = 3;
    comecaInvetorio();
    
    sel_led(Antena, 1);
    sel_antena(Antena);
    for(cap2 = 0; cap2 < 33; cap2++){
        for(cap1 = 0; cap1 < 33; cap1++){
            for(cap0 = 0; cap0 < 33; cap0++){
                //sprintf(mensagem, "%02d %02d %02d", cap0, cap1, cap2);
                //enviaDadosParaUSBserial(mensagem, strlen(mensagem));
                tunerSetTuning(&mainTuner, cap0, cap1, cap2);
                timeout = 0;
                contador = 0;
                REPETE:
                QuantiaDeTagsObtidas = inventorioSimplificado();
                if (QuantiaDeTagsObtidas > 0){
                    //sprintf(mensagem, "%02d %02d %02d\r\n", cap0, cap1, cap2);
                    //enviaDadosParaUSBserial(mensagem, strlen(mensagem));
                    timeout = 0;
                    contador = contador + 1;
                    LED_TAG(1);
                    liga_buzzer();
                    delay_ms(20); 
                    LED_TAG(0);
                    desliga_buzzer();
                    delay_ms(20);
                    goto REPETE;
                }else{
                    timeout = timeout + 1;
                    if(timeout < 5){
                        goto REPETE;
                    }
                    if(contador > 1){
                        sprintf(mensagem, "Ajuste: %02d %02d %02d - Resposta: %d\r\n", cap0, cap1, cap2, contador);
                        enviaDadosParaUSBserial(mensagem, strlen(mensagem));
                    }
                    
                }
            }
        }
        
    } 
}

void enviaDadosParaUSBserial(char mensagem[100], int tamanho){
    int contador;   
    if(!debugInterfaceEthernet){
        for (contador = 0; contador < tamanho; contador = contador + 1){
            //uart4Tx(Mensagem[SubContador]);
            uart3Tx(mensagem[contador]); //Porta USB Serial
            //uart2Tx(Mensagem[SubContador]); // Porta Ethernet
            //uart1Tx(Mensagem[SubContador]);
        }  
    }
}

void setaTunerTeste(const TunerConfiguration *config){
    //tunerInit(&mainTuner);
}

/*
int leUmaTagParaAjusteCapacitores(int TempoDeLeitura, char *EpcCapturado) {
    int Resultado;
    int Contador;
    for (Contador = 0; Contador < TempoDeLeitura; Contador = Contador + 1) {
        Resultado = inventorioSimplificado();
        if (Resultado != 0) {
            if (*EpcCapturado == 0) {
                memcpy(EpcCapturado, tags_[0].epc, TAMANHO_EPC_PARA_EMPILHADEIRA);
                return 1;
            } else {
                Resultado = memcmp(EpcCapturado, tags_[0].epc, TAMANHO_EPC_PARA_EMPILHADEIRA);
                if (Resultado == 0) return 1;
            }
        }
        delay_ms(1);
    }
    return -1;
}
*/


void initAutoSintoniaPorReflexao(void){
    
    char mensagem[100];

    sprintf(mensagem, "Entrando em modo de Auto Sintonia\r\n");
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
        
    int Metodo;
    int Antena;
    while(1){
        for (Antena = 1; Antena <= 4; Antena = Antena + 1){
            for (Metodo = 2; Metodo <= 2; Metodo = Metodo + 1){
                realizaAutoSintoniaDosCapacitores(Metodo);
                delay_ms(500);
            }
        }
        liga_buzzer();
        delay_ms(100);
        desliga_buzzer();
        delay_ms(100);
    }
    
}

void realizaAutoSintoniaDosCapacitores(int Metodo){
    //extern  TunerParameters tunerParams;
    static TunerParameters tunerParams = {2, 12, 12};
    extern TunerConfiguration mainTuner;
    TunerConfiguration *tuner = NULL;
    TunerParameters *params = NULL;
//    int Resultados[3];
    
    int endereco = 0;
    char enderecoRefl_MSB = 0;
    char enderecoRefl_LSB = 0;
    unsigned char aux1,aux2;
    
    char Mensagem[100];
    int Contador;
    
    tuner = &mainTuner;
    params = &tunerParams;
    
    int _AntenaASerLida;
    _AntenaASerLida = 1;
    sel_led(0,0);
    for(_AntenaASerLida=1;_AntenaASerLida<=4;_AntenaASerLida++){
        sel_led(_AntenaASerLida, 1);
        sel_antena(_AntenaASerLida);
        delay_ms(300);
        switch(Metodo){
            case (0):
                as3993AntennaPower(1);
                tunerOneHillClimb(tuner, params, 100);
                as3993AntennaPower(0);
                break;
            case (1): /* advanced: hill climb from more points */
                as3993AntennaPower(1);
                tunerMultiHillClimb(tuner, params);
                as3993AntennaPower(0);
                break;
            case (2):
                as3993AntennaPower(1);
                tunerTraversal(tuner, params);
                as3993AntennaPower(0);
        }

        sprintf(Mensagem, "Antena %d -> Cin: %d, Clen: %d, Cout: %d, Reflexao: %d\n", _AntenaASerLida,params->cin, params->clen, params->cout,params->reflectedPower);
        
        switch(_AntenaASerLida){
            case 1:
                endereco = END_AJUSTE_CAP0_ANTENA_1;
                enderecoRefl_MSB = END_INTENSIDADE_REFLEXAO_ANTENA_1_MSB;
                enderecoRefl_LSB = END_INTENSIDADE_REFLEXAO_ANTENA_1_LSB;
                break;
            case 2:
                endereco = END_AJUSTE_CAP0_ANTENA_2;
                enderecoRefl_MSB = END_INTENSIDADE_REFLEXAO_ANTENA_2_MSB;
                enderecoRefl_LSB = END_INTENSIDADE_REFLEXAO_ANTENA_2_LSB;
                break;
            case 3:
                endereco = END_AJUSTE_CAP0_ANTENA_3;
                enderecoRefl_MSB = END_INTENSIDADE_REFLEXAO_ANTENA_3_MSB;
                enderecoRefl_LSB = END_INTENSIDADE_REFLEXAO_ANTENA_3_LSB;
                break;
            case 4: 
                endereco = END_AJUSTE_CAP0_ANTENA_4;
                enderecoRefl_MSB = END_INTENSIDADE_REFLEXAO_ANTENA_4_MSB;
                enderecoRefl_LSB = END_INTENSIDADE_REFLEXAO_ANTENA_4_LSB;                       
                break;
        }
        
        EscreverNaEEprom(endereco, params->cin);
        EscreverNaEEprom(endereco+1, params->clen);
        EscreverNaEEprom(endereco+2, params->cout);
        
        aux1 = (unsigned char)(params->reflectedPower>>8);
        aux2 = (unsigned char)(params->reflectedPower & 0X00FF);
        EscreverNaEEprom(enderecoRefl_MSB,aux1);
        EscreverNaEEprom(enderecoRefl_LSB,aux2);
        
        Contador = 0;
        while(Mensagem[Contador]){
            uart3Tx(Mensagem[Contador]);
            Contador = Contador + 1;
        }
            sel_led(_AntenaASerLida, 0);
    }
}

