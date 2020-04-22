
#include "setup_usb.h"
#include "stddef.h"



char bufferRxUSB[TAMANHO_BUFFER_COMANDOS_USB];

unsigned char modoDeOperacao;
unsigned char atrasoParaDegradarLeitura;
unsigned char repeticaoNaLeitura;
unsigned char sensibilidade;
unsigned char tempoParaInatividadeDaTagMonitorada;
unsigned char numeroDeAntenasAtivas;
unsigned char capAntena1[3];
unsigned char capAntena2[3];
unsigned char capAntena3[3];
unsigned char capAntena4[3];
unsigned char capAntena5[3];
unsigned char capAntena6[3];
unsigned char capAntena7[3];
unsigned char capAntena8[3];
unsigned char frequenciaDeOperacao;
unsigned char idDoLeitor[4];

extern char ipRemotoPrincipal[16];
extern char ipRemotoSecundario[16];
extern int  portaRemotaPrincipal;
extern int  portaRemotaSecundaria;

//unsigned char ipRemotoPrincipal[15];
//unsigned char ipRemotoSecundario[15];
//unsigned int portaRemotaPrincipal;
//unsigned int portaRemotaSecundaria;

unsigned char ssidWifi[17];
unsigned char senhaWifi[17];

unsigned char ipTemp[4];



void commandHandlerPortaUSB(void){
    char strCMD[10];
    int comando = 0;
    
    // VERIFICA SE É UM COMANDO VALIDO
    if(strncmp(bufferRxUSB, "#CMD", 4) == 0){
        memset(strCMD, NULL, 10);
        strCMD[0] = bufferRxUSB[4];
        strCMD[1] = bufferRxUSB[5];
        //strCMD[2] = '\0'; // fim da string
        comando = atoi(strCMD); // Converte String em Número int
                
        // VERIFICA SE É COMANDO PARA TERAR PARAMETROS
        if(bufferRxUSB[6] == ';'){
            switch(comando){
                case CMD_MODO_DE_OPERACAO:
                    setaModoDeOperacao();
                    break;
                case CMD_ATRASO_PARA_DEGRAD_LEITURA:
                    setaAtrasoParaDegradarLeitura();
                    break;

                case CMD_REPETICAO_NA_LEITURA:
                    setaRepeticaoNaLeitura();
                    break;

                case CMD_SENSIBILIDADE_DA_ANTENA:
                    setaSensibilidadeDaAntena();
                    break;

                case CMD_TEMPO_INATIV_TAG_MONIT:
                    setaTempoParaInatividadeDaTagMonitorada();
                    break;

                case CMD_NUMERO_DE_ANTENAS:
                    setaNumeroDeAntenasAtivas();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_1:
                    setaAjusteDeCapacitoresAntena1();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_2:
                    setaAjusteDeCapacitoresAntena2();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_3:
                    setaAjusteDeCapacitoresAntena3();
                    break;
    
                case CMD_AJUSTE_CAP_ANTENA_4:
                    setaAjusteDeCapacitoresAntena4();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_5:
                    setaAjusteDeCapacitoresAntena5();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_6:
                    setaAjusteDeCapacitoresAntena6();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_7:
                    setaAjusteDeCapacitoresAntena7();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_8:
                    setaAjusteDeCapacitoresAntena8();
                    break;

                case CMD_FREQUENCIA_OPERACAO:
                    setaFrequenciaDeOperacao();
                    break;

                case CMD_ID_DO_LEITOR:
                    setaIdDoLeitor();
                    break;

                case CMD_ATUALIZACAO_DATA_HORA:
                    setaDataHora();
                    break;

                case CMD_IP_REMOTO_PRINCIPAL:
                    setaIpRemotoPrincipal();
                    break;

                case CMD_IP_REMOTO_SECUNDARIO:
                    setaIpRemotoSecundario();
                    break;
  
                case CMD_PORTA_REMOTA_PRINCIPAL:
                    setaPortaRemotaPrincipal();
                    break;

                case CMD_PORTA_REMOTA_SECUNDARIA:
                    setaPortaRemotaSecundaria();
                    break;

                case CMD_SSID_WIFI:
                    setaSsidWifi();
                    break;

                case CMD_SENHA_WIFI:
                    setaSenhaWifi();
                    break;
            }   
        }
        
        // VERIFICA SE É COMANDO PARA OBTER PARAMETROS SALVOS (QUERY)
        if(bufferRxUSB[6] == '?'){
            switch(comando){
                case CMD_MODO_DE_OPERACAO:
                    obtemModoDeOperacao();
                    break;
                case CMD_ATRASO_PARA_DEGRAD_LEITURA:
                    obtemAtrasoParaDegradarLeitura();
                    break;

                case CMD_REPETICAO_NA_LEITURA:
                    obtemRepeticaoNaLeitura();
                    break;

                case CMD_SENSIBILIDADE_DA_ANTENA:
                    obtemSensibilidadeDaAntena();
                    break;

                case CMD_TEMPO_INATIV_TAG_MONIT:
                    obtemTempoParaInatividadeDaTagMonitorada();
                    break;

                case CMD_NUMERO_DE_ANTENAS:
                    obtemNumeroDeAntenasAtivas();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_1:
                    obtemAjusteDeCapacitoresAntena1();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_2:
                    obtemAjusteDeCapacitoresAntena2();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_3:
                    obtemAjusteDeCapacitoresAntena3();
                    break;
    
                case CMD_AJUSTE_CAP_ANTENA_4:
                    obtemAjusteDeCapacitoresAntena4();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_5:
                    obtemAjusteDeCapacitoresAntena5();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_6:
                    obtemAjusteDeCapacitoresAntena6();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_7:
                    obtemAjusteDeCapacitoresAntena7();
                    break;

                case CMD_AJUSTE_CAP_ANTENA_8:
                    obtemAjusteDeCapacitoresAntena8();
                    break;

                case CMD_FREQUENCIA_OPERACAO:
                    obtemFrequenciaDeOperacao();
                    break;

                case CMD_ID_DO_LEITOR:
                    obtemIdDoLeitor();
                    break;

                case CMD_ATUALIZACAO_DATA_HORA:
                    obtemDataHora();
                    break;

                case CMD_IP_REMOTO_PRINCIPAL:
                    obtemIpRemotoPrincipal();
                    break;

                case CMD_IP_REMOTO_SECUNDARIO:
                    obtemIpRemotoSecundario();
                    break;
  
                case CMD_PORTA_REMOTA_PRINCIPAL:
                    obtemPortaRemotaPrincipal();
                    break;

                case CMD_PORTA_REMOTA_SECUNDARIA:
                    obtemPortaRemotaSecundaria();
                    break;

                case CMD_SSID_WIFI:
                    obtemSsidWifi();
                    break;

                case CMD_SENHA_WIFI:
                    obtemSenhaWifi();
                    break;
                    
                case CMD_OBTER_FIRMWARE:
                    obtemFimware();
                    break;
                    
                case CMD_OBTER_PARAMETROS:
                    obtemParametrosDaMemoria();
                    exibirParametrosObtidos();
                    break;
            }   
        }
        limpaBufferRxUSB();
    } 
}


void setaModoDeOperacao(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor > 0 && valor <= 3){ //RANGE
            EscreverNaEEprom(END_MODO_DE_OPERACAO, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaAtrasoParaDegradarLeitura(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor >= 0 && valor <= 100){ //RANGE
            EscreverNaEEprom(END_ATRASO_PARA_DEGRADAR_LEITURA, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaRepeticaoNaLeitura(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor >= 0 && valor <= 100){ //RANGE
            EscreverNaEEprom(END_REPETICAO_NA_LEITURA, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaSensibilidadeDaAntena(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor >= 0 && valor <= 127){ //RANGE
            EscreverNaEEprom(END_SENSIBILIDADE_DA_ANTENA, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaTempoParaInatividadeDaTagMonitorada(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor >= 0 && valor <= 100){ //RANGE
            EscreverNaEEprom(END_TEMPO_INAT_TAG_MONITORADA, valor);
            retornaOk();
            return;        
        }
    }   
    retornaNok();
}


void setaNumeroDeAntenasAtivas(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor > 0 && valor <= 8){ //RANGE
            EscreverNaEEprom(END_NUMERO_DE_ANTENAS, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaAjusteDeCapacitoresAntena1(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_1);
        retornaOk();
        return;
    }
    retornaNok();
}


void setaAjusteDeCapacitoresAntena2(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_2);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaAjusteDeCapacitoresAntena3(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_3);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaAjusteDeCapacitoresAntena4(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_4);
        retornaOk();
        return;
    }
    retornaNok();
}


void setaAjusteDeCapacitoresAntena5(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_5);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaAjusteDeCapacitoresAntena6(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_6);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaAjusteDeCapacitoresAntena7(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_7);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaAjusteDeCapacitoresAntena8(void){
    if(bufferRxUSB[10] == ';' && bufferRxUSB[14] == ';' && bufferRxUSB[18] == '\r'){
        gravaAjusteDeCapacitoresNaMemoria(END_AJUSTE_CAP0_ANTENA_8);
        retornaOk();
        return;
    }
    retornaNok(); 
}


void setaFrequenciaDeOperacao(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[10] == '\r' && bufferRxUSB[11] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        valor = atoi(num);
        if(valor >= 0 && valor <= 22){ //RANGE
            EscreverNaEEprom(END_FREQUENCIA, valor);
            retornaOk();
            return;        
        }
    }
    retornaNok();
}


void setaIdDoLeitor(void){
    
    if(bufferRxUSB[11] == '\r' && bufferRxUSB[12] == '\n'){
        EscreverNaEEprom(END_ID_DO_LEITOR_0, bufferRxUSB[7]);
        EscreverNaEEprom(END_ID_DO_LEITOR_1, bufferRxUSB[8]);
        EscreverNaEEprom(END_ID_DO_LEITOR_2, bufferRxUSB[9]);
        EscreverNaEEprom(END_ID_DO_LEITOR_3, bufferRxUSB[10]);
        retornaOk();
        return;
    }
    retornaOk();
}



void setaDataHora(void){
    retornaNok();
}


void setaIpRemotoPrincipal(void){
    if(bufferRxUSB[10] == '.' && bufferRxUSB[14] == '.' && bufferRxUSB[18] == '.' && bufferRxUSB[22] == '\r' && bufferRxUSB[23] == '\n'){
        if(escreveIpNaMemoria(END_IP_REMOTO_PRINCIPAL_0, bufferRxUSB[7], bufferRxUSB[8], bufferRxUSB[9]) == 1){
            if(escreveIpNaMemoria(END_IP_REMOTO_PRINCIPAL_1, bufferRxUSB[11], bufferRxUSB[12], bufferRxUSB[13]) == 1){
                if(escreveIpNaMemoria(END_IP_REMOTO_PRINCIPAL_2, bufferRxUSB[15], bufferRxUSB[16], bufferRxUSB[17]) == 1){
                    if(escreveIpNaMemoria(END_IP_REMOTO_PRINCIPAL_3, bufferRxUSB[19], bufferRxUSB[20], bufferRxUSB[21]) == 1){
                        retornaOk();
                        return;
                    }
                }
            }
        }
    }
    retornaNok();
    return;
}
   

void setaIpRemotoSecundario(void){
    if(bufferRxUSB[10] == '.' && bufferRxUSB[14] == '.' && bufferRxUSB[18] == '.' && bufferRxUSB[22] == '\r' && bufferRxUSB[23] == '\n'){
        if(escreveIpNaMemoria(END_IP_REMOTO_SECUNDARIO_0, bufferRxUSB[7], bufferRxUSB[8], bufferRxUSB[9]) == 1){
            if(escreveIpNaMemoria(END_IP_REMOTO_SECUNDARIO_1, bufferRxUSB[11], bufferRxUSB[12], bufferRxUSB[13]) == 1){
                if(escreveIpNaMemoria(END_IP_REMOTO_SECUNDARIO_2, bufferRxUSB[15], bufferRxUSB[16], bufferRxUSB[17]) == 1){
                    if(escreveIpNaMemoria(END_IP_REMOTO_SECUNDARIO_3, bufferRxUSB[19], bufferRxUSB[20], bufferRxUSB[21]) == 1){
                        retornaOk();
                        return;
                    }
                }
            }
        }
    }
    retornaNok();
    return;
}


void setaPortaRemotaPrincipal(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[12] == '\r' && bufferRxUSB[13] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        num[3] = bufferRxUSB[10];
        num[4] = bufferRxUSB[11];
        valor = atoi(num);
        if(valor > 0 && valor < 65536){ //RANGE
            EscreverNaEEprom(END_PORTA_REMOTA_PRINCIPAL_0, valor >> 8);
            EscreverNaEEprom(END_PORTA_REMOTA_PRINCIPAL_1, valor);
            retornaOk();
            return;
        }
    }
    retornaNok();
}


void setaPortaRemotaSecundaria(void){
    unsigned char num[10];
    unsigned int valor;
    
    if(bufferRxUSB[12] == '\r' && bufferRxUSB[13] == '\n'){
        memset(num, NULL, 10);
        num[0] = bufferRxUSB[7];
        num[1] = bufferRxUSB[8];
        num[2] = bufferRxUSB[9];
        num[3] = bufferRxUSB[10];
        num[4] = bufferRxUSB[11];
        valor = atoi(num);
        if(valor > 0 && valor < 65536){ //RANGE
            EscreverNaEEprom(END_PORTA_REMOTA_SECUNDARIA_0, valor >> 8);
            EscreverNaEEprom(END_PORTA_REMOTA_SECUNDARIA_1, valor);
            retornaOk();
            return;
        }
    }
    retornaNok();
}


void setaSsidWifi(void){
    unsigned char endereco;
    unsigned char ponteiro;
    
    ponteiro = 7;
    endereco = END_SSID_WIFI_0;
    
    if(bufferRxUSB[17] == '\r' && bufferRxUSB[18] == '\n'){
        for(ponteiro = 7; ponteiro < 17; ponteiro = ponteiro + 1){
            EscreverNaEEprom(endereco, bufferRxUSB[ponteiro]);
            endereco = endereco + 1;
        }
        retornaOk();
        return;
    }
    retornaNok();
}


void setaSenhaWifi(void){
    unsigned char endereco;
    unsigned char ponteiro;
    
    ponteiro = 7;
    endereco = END_SENHA_WIFI_0;
    
    if(bufferRxUSB[17] == '\r' && bufferRxUSB[18] == '\n'){
        for(ponteiro = 7; ponteiro < 17; ponteiro = ponteiro + 1){
            EscreverNaEEprom(endereco, bufferRxUSB[ponteiro]);
            endereco = endereco + 1;
        }
        retornaOk();
        return;
    }
    retornaNok();
}


// ROTINAS PARA OBTER DADOS

void obtemModoDeOperacao(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_MODO_DE_OPERACAO, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));   
}


void obtemAtrasoParaDegradarLeitura(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_ATRASO_PARA_DEGRADAR_LEITURA, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemRepeticaoNaLeitura(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_REPETICAO_NA_LEITURA, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemSensibilidadeDaAntena(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_SENSIBILIDADE_DA_ANTENA, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemTempoParaInatividadeDaTagMonitorada(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_TEMPO_INAT_TAG_MONITORADA, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemNumeroDeAntenasAtivas(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_NUMERO_DE_ANTENAS, &valor);
    sprintf(mensagem, "#OK,%d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemAjusteDeCapacitoresAntena1(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_1);
}


void obtemAjusteDeCapacitoresAntena2(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_2);
}


void obtemAjusteDeCapacitoresAntena3(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_3);
}


void obtemAjusteDeCapacitoresAntena4(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_4);
}


void obtemAjusteDeCapacitoresAntena5(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_5);
}


void obtemAjusteDeCapacitoresAntena6(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_6);
}


void obtemAjusteDeCapacitoresAntena7(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_7);
}


void obtemAjusteDeCapacitoresAntena8(void){
    obtemAjusteDeCapacitores(END_AJUSTE_CAP0_ANTENA_8);
}


void obtemFrequenciaDeOperacao(void){
    char mensagem[50];
    unsigned char valor;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_FREQUENCIA, &valor);
    sprintf(mensagem, "#OK,%03d\r\n", valor);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void obtemIdDoLeitor(void){
    char mensagem[50];
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_ID_DO_LEITOR_0, &mensagem[0]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_1, &mensagem[1]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_2, &mensagem[2]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_3, &mensagem[3]);
    mensagem[4] = '\r';
    mensagem[5] = '\n';
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));    
}


void obtemDataHora(void){
    retornaNok();
}


void obtemIpRemotoPrincipal(void){
    obtemIpRemoto(END_IP_REMOTO_PRINCIPAL_0);    
}


void obtemIpRemotoSecundario(void){
    obtemIpRemoto(END_IP_REMOTO_SECUNDARIO_0);
}


void obtemPortaRemotaPrincipal(void){
    char mensagem[50];
    unsigned char valor;
    unsigned int numPorta;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(END_PORTA_REMOTA_PRINCIPAL_0, &valor);
    numPorta = valor;
    LerDadosDaEEprom(END_PORTA_REMOTA_PRINCIPAL_1, &valor);
    numPorta = (numPorta << 8) | valor;
    sprintf(mensagem, "#OK,%05d\r\n", numPorta);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void obtemPortaRemotaSecundaria(void){
    char mensagem[50];
    unsigned int valor;
    unsigned int numPorta;
    
    memset(mensagem, NULL, 50);
    valor = 0;
    numPorta = 0;
    LerDadosDaEEprom(END_PORTA_REMOTA_SECUNDARIA_0, &valor);
    numPorta = valor;
    LerDadosDaEEprom(END_PORTA_REMOTA_SECUNDARIA_1, &valor);
    numPorta = (numPorta << 8) | valor;
    sprintf(mensagem, "#OK,%05d\r\n", numPorta);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void obtemSsidWifi(void){
    char mensagem[50];
    char str[16];
    unsigned int ponteiro;
    unsigned int endereco;
    
    memset(mensagem, NULL, 50);
    memset(str, NULL, 16);
    endereco = END_SSID_WIFI_0;
    for(ponteiro = 0; ponteiro < 10; ponteiro = ponteiro + 1){
        LerDadosDaEEprom(endereco, &str[ponteiro]);
        endereco = endereco + 1;
    }
    sprintf(mensagem, "#OK,%s\r\n", str);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void obtemSenhaWifi(void){
    char mensagem[50];
    char str[16];
    unsigned int ponteiro;
    unsigned int endereco;
    
    memset(mensagem, NULL, 50);
    memset(str, NULL, 16);
    endereco = END_SENHA_WIFI_0;
    for(ponteiro = 0; ponteiro < 10; ponteiro = ponteiro + 1){
        LerDadosDaEEprom(endereco, &str[ponteiro]);
        endereco = endereco + 1;
    }
    sprintf(mensagem, "#OK,%s\r\n", str);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void obtemFimware(void){
    char mensagem[50];
    sprintf(mensagem, "Firmware CTRP-PTL-ETH_1.0\r\n");
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}




void retornaOk(void){
    char mensagem[10];
    sprintf(mensagem, "OK\r\n");
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void retornaNok(void){
    char mensagem[10];
    sprintf(mensagem, "NOK\r\n");
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}


void enviaRespostaAosComandosDeSetupUSB(char dados[TAMANHO_BUFFER_COMANDOS_USB], int tamanho){
    int posicao;
    for (posicao = 0; posicao < tamanho; posicao = posicao + 1){
        uart3Tx(dados[posicao]); //Porta USB
        //uart2Tx(*Dados);//Porta ETH
        //uart2Tx(dados[posicao]);//Porta ETH
        //uart1Tx(*Dados);//ZigBee Beta
        //uart4Tx(Dado);//ZigBee Alfa
        //Dados = Dados + 1;
    }
}



void gravaAjusteDeCapacitoresNaMemoria(unsigned char endereco){
    unsigned char num[10];
    unsigned int valor;
    unsigned char enderecoMemoria;
    
    //enderecoMemoria = endereco;
        
    memset(num, NULL, 10);
    num[0] = bufferRxUSB[7];
    num[1] = bufferRxUSB[8];
    num[2] = bufferRxUSB[9];
    valor = atoi(num);
    if(valor >= 0 && valor <= 33){ //RANGE
        EscreverNaEEprom(endereco, valor);
    }else{
        retornaNok();
        return;
    }
    
    memset(num, NULL, 10);
    num[0] = bufferRxUSB[11];
    num[1] = bufferRxUSB[12];
    num[2] = bufferRxUSB[13];
    valor = atoi(num);
    if(valor >= 0 && valor <= 33){ //RANGE
        EscreverNaEEprom(endereco + 1, valor);
    }else{
        retornaNok();
        return;
    }
    
    memset(num, NULL, 10);
    num[0] = bufferRxUSB[15];
    num[1] = bufferRxUSB[16];
    num[2] = bufferRxUSB[17];
    valor = atoi(num);
    if(valor >= 0 && valor <= 33){ //RANGE
        EscreverNaEEprom(endereco + 2, valor);
    }else{
        retornaNok();
        return;
    }
}



void limpaBufferRxUSB(void){
    memset(bufferRxUSB, 0, TAMANHO_BUFFER_COMANDOS_USB);
}


char escreveIpNaMemoria(unsigned int endereco, unsigned char dado0, unsigned char dado1, unsigned char dado2){
    unsigned char num[10];
    unsigned int valor;
    
    memset(num, NULL, 10);
    num[0] = dado0;
    num[1] = dado1;
    num[2] = dado2;
    valor = atoi(num);
    if(valor >= 0 && valor <= 255){ //RANGE
        EscreverNaEEprom(endereco, valor);
        return 1;
    }
    return 0;
}


void obtemAjusteDeCapacitores(unsigned int endereco){
    char mensagem[50];
    unsigned char valor0;
    unsigned char valor1;
    unsigned char valor2;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(endereco, &valor0);
    LerDadosDaEEprom(endereco + 1, &valor1);
    LerDadosDaEEprom(endereco + 2, &valor2);
    sprintf(mensagem, "#OK,%03d,%03d,%03d\r\n", valor0, valor1, valor2);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem)); 
}


void obtemIpRemoto(unsigned char endereco){
    char mensagem[50];
    unsigned char valor0;
    unsigned char valor1;
    unsigned char valor2;
    unsigned char valor3;
    
    memset(mensagem, NULL, 50);
    LerDadosDaEEprom(endereco, &valor0);
    LerDadosDaEEprom(endereco + 1, &valor1);
    LerDadosDaEEprom(endereco + 2, &valor2);
    LerDadosDaEEprom(endereco + 3, &valor3);
    
    sprintf(mensagem, "#OK,%03d.%03d.%03d.%03d\r\n", valor0, valor1, valor2, valor3);
    enviaRespostaAosComandosDeSetupUSB(mensagem, strlen(mensagem));
}

void obtemParametrosDaMemoria(void){
    int endereco;
    char vetor;
    
    LerDadosDaEEprom(END_MODO_DE_OPERACAO, &modoDeOperacao);
    LerDadosDaEEprom(END_ATRASO_PARA_DEGRADAR_LEITURA, &atrasoParaDegradarLeitura);
    LerDadosDaEEprom(END_REPETICAO_NA_LEITURA, &repeticaoNaLeitura);
    LerDadosDaEEprom(END_SENSIBILIDADE_DA_ANTENA, &sensibilidade);
    LerDadosDaEEprom(END_TEMPO_INAT_TAG_MONITORADA, &tempoParaInatividadeDaTagMonitorada);
    LerDadosDaEEprom(END_NUMERO_DE_ANTENAS, &numeroDeAntenasAtivas);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_1, &capAntena1[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_1, &capAntena1[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_1, &capAntena1[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_2, &capAntena2[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_2, &capAntena2[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_2, &capAntena2[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_3, &capAntena3[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_3, &capAntena3[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_3, &capAntena3[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_4, &capAntena4[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_4, &capAntena4[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_4, &capAntena4[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_5, &capAntena5[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_5, &capAntena5[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_5, &capAntena5[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_6, &capAntena6[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_6, &capAntena6[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_6, &capAntena6[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_7, &capAntena7[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_7, &capAntena7[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_7, &capAntena7[2]);
    
    LerDadosDaEEprom(END_AJUSTE_CAP0_ANTENA_8, &capAntena8[0]);
    LerDadosDaEEprom(END_AJUSTE_CAP1_ANTENA_8, &capAntena8[1]);
    LerDadosDaEEprom(END_AJUSTE_CAP2_ANTENA_8, &capAntena8[2]);
    
    LerDadosDaEEprom(END_FREQUENCIA, &frequenciaDeOperacao);
    
    LerDadosDaEEprom(END_ID_DO_LEITOR_0, &idDoLeitor[0]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_1, &idDoLeitor[1]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_2, &idDoLeitor[2]);
    LerDadosDaEEprom(END_ID_DO_LEITOR_3, &idDoLeitor[3]);
    
    
    LerDadosDaEEprom(END_IP_REMOTO_PRINCIPAL_0, &ipTemp[0]);
    LerDadosDaEEprom(END_IP_REMOTO_PRINCIPAL_1, &ipTemp[1]);
    LerDadosDaEEprom(END_IP_REMOTO_PRINCIPAL_2, &ipTemp[2]);
    LerDadosDaEEprom(END_IP_REMOTO_PRINCIPAL_3, &ipTemp[3]);
    sprintf(ipRemotoPrincipal, "%d.%d.%d.%d", ipTemp[0], ipTemp[1], ipTemp[2], ipTemp[3]);
    
    LerDadosDaEEprom(END_IP_REMOTO_SECUNDARIO_0, &ipTemp[0]);
    LerDadosDaEEprom(END_IP_REMOTO_SECUNDARIO_1, &ipTemp[1]);
    LerDadosDaEEprom(END_IP_REMOTO_SECUNDARIO_2, &ipTemp[2]);
    LerDadosDaEEprom(END_IP_REMOTO_SECUNDARIO_3, &ipTemp[3]);
    sprintf(ipRemotoSecundario, "%d.%d.%d.%d", ipTemp[0], ipTemp[1], ipTemp[2], ipTemp[3]);
    
    LerDadosDaEEprom(END_PORTA_REMOTA_PRINCIPAL_0, &portaRemotaPrincipal);
    portaRemotaPrincipal = portaRemotaPrincipal << 8;
    LerDadosDaEEprom(END_PORTA_REMOTA_PRINCIPAL_1, &portaRemotaPrincipal);
    
    LerDadosDaEEprom(END_PORTA_REMOTA_SECUNDARIA_0, &portaRemotaSecundaria);
    portaRemotaSecundaria = portaRemotaSecundaria << 8;
    LerDadosDaEEprom(END_PORTA_REMOTA_SECUNDARIA_1, &portaRemotaSecundaria);
    
    
    memset(ssidWifi, 17, NULL);
    endereco = END_SSID_WIFI_0;
    for(vetor = 0; vetor < 16; vetor++){
        LerDadosDaEEprom(endereco, &ssidWifi[vetor]);
        endereco = endereco + 1;
    } 
    
    memset(senhaWifi, 17, NULL);
    endereco = END_SENHA_WIFI_0;
    for(vetor = 0; vetor < 16; vetor++){
        LerDadosDaEEprom(endereco, &senhaWifi[vetor]);
        endereco = endereco + 1;
    }    
}


void exibirParametrosObtidos(void){
    char mensagem[100];
    
    sprintf(mensagem, "MODO DE OPERACAO:    %03d\r\n", modoDeOperacao);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "ATRASO DEG LEITURA:  %03d\r\n", atrasoParaDegradarLeitura);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "REPETICAO LEITURA:   %03d\r\n", repeticaoNaLeitura);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SENSIBILIDADE:       %03d\r\n", sensibilidade);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "TEMPO INAT. TAG:     %03d\r\n", tempoParaInatividadeDaTagMonitorada);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "NUM ANTENAS ATIVAS:  %03d\r\n", numeroDeAntenasAtivas);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 1:  %03d, %03d, %03d\r\n", capAntena1[0], capAntena1[1], capAntena1[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 2:  %03d, %03d, %03d\r\n", capAntena2[0], capAntena2[1], capAntena2[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 3:  %03d, %03d, %03d\r\n", capAntena3[0], capAntena3[1], capAntena3[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 4:  %03d, %03d, %03d\r\n", capAntena4[0], capAntena4[1], capAntena4[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 5:  %03d, %03d, %03d\r\n", capAntena5[0], capAntena5[1], capAntena5[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 6:  %03d, %03d, %03d\r\n", capAntena6[0], capAntena6[1], capAntena6[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 7:  %03d, %03d, %03d\r\n", capAntena7[0], capAntena7[1], capAntena7[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "SINTONIA CAP ANT 8:  %03d, %03d, %03d\r\n", capAntena8[0], capAntena8[1], capAntena8[2]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "FREQUENCIA OPERACAO: %02d\r\n", frequenciaDeOperacao);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "ID DO LEITOR:        %c%c%c%c\r\n", idDoLeitor[0], idDoLeitor[1], idDoLeitor[2], idDoLeitor[3]);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    
    sprintf(mensagem, "IP REMOTO PRINC.:    %s\r\n", ipRemotoPrincipal);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "IP REMOTO SECUND.:   %s\r\n", ipRemotoSecundario);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "PORTA REMOTA PRINC.: %05d\r\n", portaRemotaPrincipal);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    sprintf(mensagem, "PORTA REMOTA SEC.:   %05d\r\n", portaRemotaSecundaria);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    
    memset(mensagem, 100, NULL);
    sprintf(mensagem, "SSID WIFI:           %s\r\n", ssidWifi);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    memset(mensagem, 100, NULL);
    sprintf(mensagem, "SENHA WIFI:          %s\r\n", senhaWifi);
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
}



