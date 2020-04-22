///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FINITE STATE MACHINE - MÁQUINA DE ESTADOS FINITOS PARA EVENTOS DE EMPILHADEIRA
// EMPRESA: CONTROL + PLUS
// AUTOR: Vitor M. dos S. Alho
// DATA: 07/02/2020
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.24/support/peripheral_24F/uart.h"
//#include "FSM_Ethernet.h"
#include "timer.h"
#include "FSM_DataHora.h"
#include "uart_driver.h"
#include <stdint.h>
#include "empilhadeira.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "rtc.h"
#include "ajuste_cap.h"
#include <time.h>
#include "../../../../../../BaseComum/portal.h"

#include "setup_usb.h"

enum estadosDaMaquina{
        AGUARDANDO_TAREFA=0,
        VERIFICA_SE_JA_ATUALIZOU_O_RELOGIO,
        ENVIAR_SOLICITACAO_DATA_HORA,
        AGUARDANDO_ACK,        
        FIM_CICLO
}estados_DataHora;

struct tm objetoDataHora;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: maquinaDeEstadosLiberada_DataHora
// UTILIZADA EM: inicializaMaquinaDeEstados_DataHora
// FUNÇÃO: saber quando a maquina de estado está liberada para executar
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char maquinaDeEstadosLiberada_DataHora  = NAO;   
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: delay
// UTILIZADA EM: executaMaquinaDeEstados_DataHora
// FUNÇÃO: causar um delay no envio de mensagens no estadoAtual = ENVIAR_SOLICITACAO_DATA_HORA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int delayExecucao_DataHora = 0;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: leitorAcabouDeLigar_DataHora
// UTILIZADA EM: inicializaMaquinaDeEstados_DataHora
// FUNÇÃO: armazena se o leitor acabou de ser ligado. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    unsigned char leitorAcabouDeLigar_DataHora = SIM;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: estadoAtual
// UTILIZADA EM: executaMaquinaDeEstados_DataHora
// FUNÇÃO: armazena o estado atual da maquina de estado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char estadoAtual_DataHora = AGUARDANDO_TAREFA;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: estadoAnterior
// UTILIZADA EM: executaMaquinaDeEstados_DataHora
// FUNÇÃO: armazena o estado anterior da maquina de estado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char estadoAnterior_DataHora = -1;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: mensagemSARS
// UTILIZADA EM: executaMaquinaDeEstados_DataHora e em recebeDadoNaMaquinaDeEstados_DataHora
// FUNÇÃO: armazena as mensagens recebidas por interrupção da porta serial 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char mensagemSARS_DataHora[200];
   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: contadorMensagemSARS
// UTILIZADA EM: recebeDadoNaMaquinaDeEstados_DataHora e zeraPonteiroDoBufferNaMaquinaDeEstados_DataHora
// FUNÇÃO: auxilia no preenchimento do buffer mensagemSARS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char contadorMensagemSARS_DataHora = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEL: stringSolicitacaoDataHora
// UTILIZADA EM: executaMaquinaDeEstados_DataHora
// FUNÇÃO: armazena a string do evento que é enviado pela serial
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char stringSolicitacaoDataHora[300];
    
    static unsigned char idLeitor_DataHora[20];
    
    
    char horarioFoiAtualizado = NAO;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////      FUNÇÕES      /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

    void zeraPonteiroDoBufferNaMaquinaDeEstados_DataHora(void){
            contadorMensagemSARS_DataHora=0;
        }

    void recebeDadoNaMaquinaDeEstados_DataHora(char dado){
            mensagemSARS_DataHora[contadorMensagemSARS_DataHora] = dado;
            contadorMensagemSARS_DataHora++;            
        }

    void habilitaMaquinaDeEstados_DataHora(void){
            //enviarMensagem = ENVIAR_SOLICITACAO_DATA_HORA;
            maquinaDeEstadosLiberada_DataHora = SIM;
        }

    void bloqueiaMaquinaDeEstados_DataHora(void){
            maquinaDeEstadosLiberada_DataHora = NAO;
        }

    void limpaBufferNaMaquinaDeEstados_DataHora(void)  {
            int i=0;
            for(i=0;i<20;i++){
                mensagemSARS_DataHora[i]=0;
            }
        }

    void inicializaMaquinaDeEstados_DataHora(void){    
        if(leitorAcabouDeLigar_DataHora){
            leitorAcabouDeLigar_DataHora = NAO;
            memcpy(idLeitor_DataHora, IdDoLeitor, 4);
        }
    }
    
    void incrementaContadorExecucao_FSM_DataHora(void){
        delayExecucao_DataHora++;
    }
    
    void zeraContadorExecucao_FSM_DataHora(void){
        delayExecucao_DataHora = 0;
    }
    
    char funcao_atualizaDataHoraNoRTC_FSM_DataHora_jaExecutou = NAO;
    
    void atualizaDataHoraNoRTC_FSM_DataHora(void){ // isso não pôde ser executado na máquina pois demorava muito.
        if(horarioFoiAtualizado){
            if(!funcao_atualizaDataHoraNoRTC_FSM_DataHora_jaExecutou){  
                funcao_atualizaDataHoraNoRTC_FSM_DataHora_jaExecutou = SIM;
                /*
                SetarHoraRTC(objetoDataHora.tm_sec,
                     objetoDataHora.tm_min,
                     objetoDataHora.tm_hour,
                     objetoDataHora.tm_mday,
                     objetoDataHora.tm_mon,
                     objetoDataHora.tm_year);   */             
            }           
        }        
    }  
        
    void executaMaquinaDeEstados_DataHora(void){     
        int temp=0;
        char mensagemParaDebug[200];
     
        incrementaContadorExecucao_FSM_DataHora();
        
        switch(estadoAtual_DataHora)
        {
            case AGUARDANDO_TAREFA:   
                if(delayExecucao_DataHora > TEMPO_ENTRE_ESTADOS_FSM_DATA_HORA){  
                    zeraContadorExecucao_FSM_DataHora();
                    
                    sprintf(mensagemParaDebug,"FSM_DataHora aguardando\n\r");
                    //escreverMensagemUSB(mensagemParaDebug);
                                
                    if(maquinaDeEstadosLiberada_DataHora){     
                       
                        //inicializaMaquinaDeEstados_DataHora();

                        sprintf(mensagemParaDebug,"FSM_DataHora liberada\n\r");
                        //escreverMensagemUSB(mensagemParaDebug);

                        estadoAtual_DataHora = VERIFICA_SE_JA_ATUALIZOU_O_RELOGIO;
                        estadoAnterior_DataHora = AGUARDANDO_TAREFA;   
                    }                           
                }
            break;
            case VERIFICA_SE_JA_ATUALIZOU_O_RELOGIO: 

                if(delayExecucao_DataHora > TEMPO_ENTRE_ESTADOS_FSM_DATA_HORA){
                    zeraContadorExecucao_FSM_DataHora();
                    
                    sprintf(mensagemParaDebug,"FSM_DataHora verifica relogio\n\r");
                    //escreverMensagemUSB(mensagemParaDebug);                
                    
                    if(maquinaDeEstadosLiberada_DataHora){  
                        
                        if(!horarioFoiAtualizado){
                            estadoAtual_DataHora = ENVIAR_SOLICITACAO_DATA_HORA;
                            estadoAnterior_DataHora = VERIFICA_SE_JA_ATUALIZOU_O_RELOGIO; 
                        }                              
                    }   
                }
            break;         
            case ENVIAR_SOLICITACAO_DATA_HORA:  
                
                if(delayExecucao_DataHora > TEMPO_ENTRE_ESTADOS_FSM_DATA_HORA){
                    zeraContadorExecucao_FSM_DataHora();
                    sprintf(mensagemParaDebug,"FSM_DataHora enviando solicitacao\n\r");
                    //escreverMensagemUSB(mensagemParaDebug);                
                    
                    if(maquinaDeEstadosLiberada_DataHora){  
                       
                        //if(delayExecucao_DataHora>500){
                        
                        sprintf(stringSolicitacaoDataHora,"<FE;%c%c%c%c>",idLeitor_DataHora[0],idLeitor_DataHora[1],idLeitor_DataHora[2],idLeitor_DataHora[3]);
                        //sprintf(stringSolicitacaoDataHora,"<FE;1302>\n\r");
                        sprintf(stringSolicitacaoDataHora,"<FE;%c%c%c%c>", idDoLeitor[0], idDoLeitor[1], idDoLeitor[2], idDoLeitor[3]);
                        enviaDadosParaEthPortais(stringSolicitacaoDataHora, strlen(stringSolicitacaoDataHora));
                        //escreverMensagemEthernet(stringSolicitacaoDataHora);
                        //escreverMensagemUSB(stringSolicitacaoDataHora); // para debug da comunicação
                        estadoAtual_DataHora=AGUARDANDO_ACK;
                        estadoAnterior_DataHora=ENVIAR_SOLICITACAO_DATA_HORA; 
                        //}
                    } 
                }
                
            break;
            case AGUARDANDO_ACK:                    
                if(delayExecucao_DataHora < TEMPO_AGUARDANDO_ACK){
                    if(maquinaDeEstadosLiberada_DataHora){
                       if(   mensagemSARS_DataHora[0]  == '<' && // <OK;20092020;101520>
                             mensagemSARS_DataHora[1]  == 'F' &&
                             mensagemSARS_DataHora[2]  == 'E' &&
                             mensagemSARS_DataHora[3]  == ';' &&
                             mensagemSARS_DataHora[4]  == 'O' &&
                             mensagemSARS_DataHora[5]  == 'K' &&
                             mensagemSARS_DataHora[6]  == ';' &&
                             mensagemSARS_DataHora[15] == ';' &&
                             mensagemSARS_DataHora[22] == '>'){         

                             sprintf(mensagemParaDebug,"FSM_DataHora confirmacao recebida\n\r");
                             //escreverMensagemUSB(mensagemParaDebug);
                             objetoDataHora.tm_mday = (mensagemSARS_DataHora[7]  - '0') * 10 + (mensagemSARS_DataHora[8]  - '0');  
                             objetoDataHora.tm_mon  = ((mensagemSARS_DataHora[9]  - '0') * 10 + (mensagemSARS_DataHora[10]  - '0'));
                             objetoDataHora.tm_year = ((mensagemSARS_DataHora[13] - '0') * 10 + (mensagemSARS_DataHora[14] - '0'));
                             objetoDataHora.tm_hour = ((mensagemSARS_DataHora[16] - '0') * 10 + (mensagemSARS_DataHora[17] - '0'));
                             objetoDataHora.tm_min  = ((mensagemSARS_DataHora[18] - '0') * 10 + (mensagemSARS_DataHora[19] - '0'));
                             objetoDataHora.tm_sec  = ((mensagemSARS_DataHora[20] - '0') * 10 + (mensagemSARS_DataHora[21] - '0'));
                                                                                     
                             limpaBufferNaMaquinaDeEstados_DataHora(); 
                             estadoAtual_DataHora = FIM_CICLO;
                             estadoAnterior_DataHora = AGUARDANDO_ACK; 
                         }
                    }  
                }
                else{
                    zeraContadorExecucao_FSM_DataHora();
                    estadoAtual_DataHora=ENVIAR_SOLICITACAO_DATA_HORA;
                    estadoAnterior_DataHora=AGUARDANDO_ACK;                                          
                }
            break;
            case FIM_CICLO: 
                if(delayExecucao_DataHora > TEMPO_ENTRE_ESTADOS_FSM_DATA_HORA){
                    if(maquinaDeEstadosLiberada_DataHora){
                        zeraContadorExecucao_FSM_DataHora();

                        sprintf(mensagemParaDebug,"FSM_DataHora finalizando...\n\r");
                        //escreverMensagemUSB(mensagemParaDebug);

                        horarioFoiAtualizado = SIM;
                        
                        //SetarHoraRTC (int segundo, int minuto, int hora, int dia, int mes, int ano)                        
                        //Epoch = mktime(objetoDataHora);
                        //SetaTempoComEpoch(Epoch);
                        //operacoesParaRtcEmCodigoCorrente();
                        SetarHoraRTC(objetoDataHora.tm_sec,objetoDataHora.tm_min,objetoDataHora.tm_hour,objetoDataHora.tm_mday,objetoDataHora.tm_mon,objetoDataHora.tm_year);   
                        
                        //SetarHoraRTC(10,10,10,10,10,10); 
                        bloqueiaMaquinaDeEstados_DataHora();

                        estadoAtual_DataHora=AGUARDANDO_TAREFA;
                        estadoAnterior_DataHora=FIM_CICLO;
                    }      
                }                
            break;        
            default:
            break;
        }
    }

