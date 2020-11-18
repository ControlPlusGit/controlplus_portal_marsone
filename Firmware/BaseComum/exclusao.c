#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#include <exclusao.h>
#include "zigbee.h"
#include "rtc.h"
#include "empilhadeira.h"
#include "portal.h"
#include "uart_driver.h"
#include "p24FJ256DA210.h"
#include "timer.h"
#include "zigbee.h"
#include "perifericos.h"
//#include "exclusao.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\exclusao.h"
#include "C:\Projetos\control-plus\software\fifo\fifo.h"
#include "portal.h"
#include "../Sankyu/AS3993FwSource/AS3993/Firmware/src/wifi.h"
#include "../../../control-plus/Firmware/Sankyu/AS3993FwSource/AS3993/Firmware/src/global.h"
#include "platform.h"

extern char empilhadeira[20];


extern void desliga_led_zig(void);
extern void  liga_led_zig(void);


int EuSouOAlphaDosPortais;

char NomeDoPortal[2];

extern char num_serie[20];

void iniciaExclusao(void){
    NomeDoPortal[0] = num_serie[10];
    NomeDoPortal[1] = num_serie[11];
};

//struct EstruturaTabelaDeExclusao __attribute__((far)) TabelaDeExclusaoSendoRecebida;
struct EstruturaTabelaDeExclusao __attribute__((far)) TabelaDeExclusao;

//__eds__ unsigned char __attribute__((far))

extern int PortaDePortalDeExclusao;

int posicaoLivreNaTabelaDeExclusao(void);
//int envioDeTabelaDeExclusao (void);

volatile char TabelaDeExclusaoOcupadoPorEnvio = 0;


int trataRecepcaoDeTabelaDeExclusao (char *Dado, int Tamanho){
    int Contador;
    for(Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        if (Tamanho > TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){
            return -1;
        }
    /*        
        char* Ponteiro = &TabelaDeExclusao.ContagemParaExpirar[0];
        *Ponteiro++ = *Dado++;
    
        TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos = &TabelaDeExclusaoSendoRecebida.Elementos[0][0];
        TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos = TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos + TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes;
        *TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos = *Dado;
        TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes = TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes + 1;
        if (TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes == QUANTIA_DE_DADOS_DA_TABELA){
            return 0;
        }
        Dado = Dado + 1;
    */
    }

    return -1;
}

void retornaComCheckSumDaTabelaDeExclusao(unsigned int *CheckSumFinal, int *Tamanho){
    unsigned int CheckSum;
    int Contador;
    int COntagemDoTamanho;
    signed char Rascunho;
    
    CheckSum = 0x5555;
    CheckSum = CheckSum ^ 0xF9;
    COntagemDoTamanho = 0;
    *Tamanho = 0;
    for(Contador = 0;Contador < QUANTIA_DE_ELEMENTOS;Contador = Contador + 1){
        if (TabelaDeExclusao.Elementos[Contador][0] != 0){
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Elementos[Contador][0];
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Elementos[Contador][1];
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Elementos[Contador][2];
            
            //Adicionado para avaliacao de Juiz de fora
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Elementos[Contador][3];
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Elementos[Contador][4];
            
            CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.Excluido[Contador];
            //CheckSum = CheckSum ^ (unsigned char)TabelaDeExclusao.ContagemParaExpirar[Contador];
            Rascunho = TabelaDeExclusao.ContagemParaExpirar[Contador];
            CheckSum = CheckSum ^ (unsigned char)Rascunho;
            //CheckSum = CheckSum ^ (unsigned char)(TabelaDeExclusao.ContagemParaExpirar[Contador] && 0x00FF);
            COntagemDoTamanho = COntagemDoTamanho + 7;//5; //Mudei de 5 para 5 para Juiz de Fora
        }
    }
    
    *CheckSumFinal = CheckSum;
    *Tamanho = COntagemDoTamanho;
}

void enviaCabecalhoParaEnvioDeTabelaDeEx(int Tamanho){
    char Rascunho[30];
    int Contador;
    
    memset(Rascunho, 0, 30);
    (void)sprintf(Rascunho, "at+cipsend=%d\r\n", Tamanho);
    for (Contador = 0;Contador < 30;Contador = Contador + 1){
        enviaBytePorZigBee(Rascunho[Contador]);
    }
    delay_ms(10);
}

int envioDeTabelaDeExclusao (void){
    unsigned int CheckSum;
    int Tamanho;
    int Contador;
    int Rascunho;
    signed char RascunhoParaEnvio;
    
    
    TabelaDeExclusaoOcupadoPorEnvio = 1;
    
    if (tempoDeSilencioNoZigBee() < TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE){
        return -1;
    }
    
    EuSouOAlphaDosPortais = 1;
    liga_led_gps(); //PARA DEBUG
    retornaComCheckSumDaTabelaDeExclusao(&CheckSum, &Tamanho);
    Tamanho = Tamanho + 5;//6;//4
    Rascunho = Tamanho;
    Rascunho = Rascunho >> 8;
    CheckSum = CheckSum ^ Rascunho;
    Rascunho = Tamanho; 
    Rascunho = Rascunho & 0x00FF;
    CheckSum = CheckSum ^ Rascunho;

    liga_led_zig();
    
    enviaBytePorZigBee(0xF9);
    enviaWordPorZigBee(Tamanho);
    
    for(Contador = 0;Contador < QUANTIA_DE_ELEMENTOS;Contador = Contador + 1){
        if (TabelaDeExclusao.Elementos[Contador][0] != 0){
            enviaBytePorZigBee(TabelaDeExclusao.Elementos[Contador][0]);
            enviaBytePorZigBee(TabelaDeExclusao.Elementos[Contador][1]);
            enviaBytePorZigBee(TabelaDeExclusao.Elementos[Contador][2]);
            
            //Para Juiz de Fora
            enviaBytePorZigBee(TabelaDeExclusao.Elementos[Contador][3]);
            enviaBytePorZigBee(TabelaDeExclusao.Elementos[Contador][4]);
            enviaBytePorZigBee(TabelaDeExclusao.Excluido[Contador]);
            RascunhoParaEnvio = TabelaDeExclusao.ContagemParaExpirar[Contador];
            enviaBytePorZigBee((unsigned char)RascunhoParaEnvio);
        }
    }
    
    //enviaBytePorZigBee(0);
    //enviaBytePorZigBee(CheckSum);
    enviaWordPorZigBee(CheckSum);
    
    if (Tamanho > 4){
        __asm("NOP");
    }
   
    desliga_led_zig();
    
    TabelaDeExclusaoOcupadoPorEnvio = 0;
    
    
    return 0;
}

int atualizaTabelaDeExclusaoParaEnvioPorWifi (void){
    /*unsigned int CheckSum;
    int Tamanho;
    int Contador;
    int Rascunho;
    signed char RascunhoParaEnvio;
    
    TabelaDeExclusaoOcupadoPorEnvio = 1;
    resetaELimpaAReservaDeEnvioPorWifi();
    
    
    if (tempoDeSilencioNoZigBee() < TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE){
        return -1;
    }
    
    
    EuSouOAlphaDosPortais = 1;
    retornaComCheckSumDaTabelaDeExclusao(&CheckSum, &Tamanho);
    Tamanho = Tamanho + 5;//6;//4
    Rascunho = Tamanho;
    Rascunho = Rascunho >> 8;
    CheckSum = CheckSum ^ Rascunho;
    Rascunho = Tamanho; 
    Rascunho = Rascunho & 0x00FF;
    CheckSum = CheckSum ^ Rascunho;

    liga_led_zig();
    
    adicionaByteAReservaDeDadosDoWifi(0xF9);
    adicionaWordAReservaDeDadosDoWifi(Tamanho);
    
    for(Contador = 0;Contador < QUANTIA_DE_ELEMENTOS;Contador = Contador + 1){
        if (TabelaDeExclusao.Elementos[Contador][0] != 0){
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Elementos[Contador][0]);
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Elementos[Contador][1]);
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Elementos[Contador][2]);
            
            //Para Juiz de Fora
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Elementos[Contador][3]);
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Elementos[Contador][4]);
            adicionaByteAReservaDeDadosDoWifi(TabelaDeExclusao.Excluido[Contador]);
            RascunhoParaEnvio = TabelaDeExclusao.ContagemParaExpirar[Contador];
            adicionaByteAReservaDeDadosDoWifi((unsigned char)RascunhoParaEnvio);
        }
    }
    
    adicionaWordAReservaDeDadosDoWifi(CheckSum);
    
    if (Tamanho > 4){
        __asm("NOP");
    }
   
    desliga_led_zig();
    TabelaDeExclusaoOcupadoPorEnvio = 0;
    */
    return 0;
}

int removeElementoDaTabelaDeExclusao (unsigned char *Epc){
    int Posicao;
    Posicao = elementroPresenteNaTabelaDeExclusao(Epc);
    if (Posicao < 0){
        return -1;
    }
    
    (void)memset(TabelaDeExclusao.Elementos[Posicao], 0, TAMANHO_DOS_ELEMENTOS);
    //TabelaDeExclusao.MomentoQueFoiCriada = retornaComEpoch();
    return Posicao;
}

int adicionaElementoDaTabelaDeExclusao (unsigned char *Epc, char MsbNumeroDoPortal, char LsbNumeroDoPOrtal, int Direcao){
    int Posicao;
//#ifdef PODE_SER_ALPHA
    if ((*Epc != 0x30) && (*Epc != 0x50)){
        return -1;
    }
    
    Posicao = elementroPresenteNaTabelaDeExclusao(Epc);
    if (Posicao < 0){
        Posicao = posicaoLivreNaTabelaDeExclusao();
        if (Posicao < 0){
            return -1;
        }
        
        memcpy(TabelaDeExclusao.Elementos[Posicao], Epc, TAMANHO_EPC_PARA_EMPILHADEIRA);
        TabelaDeExclusao.Elementos[Posicao][3] = MsbNumeroDoPortal;
        TabelaDeExclusao.Elementos[Posicao][4] = LsbNumeroDoPOrtal;
        TabelaDeExclusao.Excluido[Posicao] = Direcao;
        if (Direcao == ENTROU_NA_AREA_SEGURA)
#ifdef PORTAL            
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = NAO_EXPIRA;
#else        
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
#endif            
        else
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_QUE_SAIU_DA_AREA_SEGURA;
            //TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
        
    } else {
        TabelaDeExclusao.Excluido[Posicao] = Direcao;
        //TabelaDeExclusao.MomentoQueFoiCriada = retornaComEpoch();
        if (Direcao == ENTROU_NA_AREA_SEGURA)
#ifdef PORTAL
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = NAO_EXPIRA;
#else
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
#endif
        else
            //TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_QUE_SAIU_DA_AREA_SEGURA;
            TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
        TabelaDeExclusao.Elementos[Posicao][3] = MsbNumeroDoPortal;
        TabelaDeExclusao.Elementos[Posicao][4] = LsbNumeroDoPOrtal;
    }
    
    return Posicao;
}

int elementroPresenteNaTabelaDeExclusao(unsigned char *Epc){
    int Contador;
	
	for(Contador = 0;Contador < LIMITE_DE_PEDESTRES;Contador = Contador + 1){
		if (memcmp(TabelaDeExclusao.Elementos[Contador], Epc, TAMANHO_EPC_PARA_EMPILHADEIRA) == 0){
			return Contador;
		}
	}
	return -1;
}

int posicaoLivreNaTabelaDeExclusao(void){
    int Contador;
	
	for(Contador = 0;Contador < LIMITE_DE_PEDESTRES;Contador = Contador + 1){
		if (TabelaDeExclusao.Elementos[Contador][0] == 0){
			return Contador;
		}
	}
	return -1;    
}

void consideraTabelaDeExclusaoRecebidaComoALocal(void){
    //TabelaDeExclusao = TabelaDeExclusaoSendoRecebida;
}

void processaTabelaDeExclusao(void){
    /*
    time_t TempoAtual, TempoDaTabelaLocal, TempoDaTabelaRecebida;
    //time_t TempoDoItem;
    //TempoAtual = retornaComEpoch();
    //TempoDaTabelaRecebida = TabelaDeExclusaoSendoRecebida.MomentoQueFoiCriada;
    //TempoDaTabelaLocal = Tempo;
    
    if (TempoDaTabelaLocal > TempoDaTabelaRecebida){
        //Envia tabela
        //envioDeTabelaDeExclusao();
    } else {
        //Tabela recebida passa a ser a tabela local.
        consideraTabelaDeExclusaoRecebidaComoALocal();
        //processaElementosDeUmaTabelaDeExclusao();
    }
    */
}

/*
void enviarElementosExcluidosPorZigBee(void){
    int Contador;
    
    for (Contador = 0;Contador < QUANTIA_DE_ELEMENTOS;Contador = Contador + 1){
        if (TabelaDeExclusao.ContagemParaExpirar[Contador] > 0){
            TabelaDeExclusao.ContagemParaExpirar[Contador] = TabelaDeExclusao.ContagemParaExpirar[Contador] - 1;
        }
        
        if (TabelaDeExclusao.ContagemParaExpirar[Contador] == 0 &&
                TabelaDeExclusao.Elementos[Contador][0] != 0 &&
                TabelaDeExclusao.Excluido[Contador] ==  SAIU_NA_AREA_SEGURA){
            removeElementoDaTabelaDeExclusao((unsigned char *)TabelaDeExclusao.Elementos[Contador]);
        }
        
        
        
        
        //if (TabelaDeExclusao.Excluido[Contador] == ENTROU_NA_AREA_SEGURA){
        //     enviaEventoDeQueUmPedestreEntrouNaAreaSeguraNoZigBee((unsigned char *)TabelaDeExclusao.Elementos[Contador],
        //            TabelaDeExclusao.MomentoQueFoiCriada);
        //}  else {
        //    enviaEventoDeQueUmPedestreSaiuNaAreaSeguraNoZigBee((unsigned char *)TabelaDeExclusao.Elementos[Contador],
        //            TabelaDeExclusao.MomentoQueFoiCriada);
        //}

    }
}
*/
int ContadorParaEnvioDeDadosDeExclusao;

void operacoesEmTickParaEnvioDaTabelaDeExclusao(void){
    int Contador;
    //unsigned char Rascunho;
    signed int Rascunho;
    
    if (ContadorParaEnvioDeDadosDeExclusao > 0){
        ContadorParaEnvioDeDadosDeExclusao = ContadorParaEnvioDeDadosDeExclusao - 1;
    }

    // Aqui eu tenho um problema grave de performace que nao acontecia antes.
    if (TabelaDeExclusaoOcupadoPorEnvio == 0){
        for (Contador = 0;Contador < QUANTIA_DE_ELEMENTOS;Contador = Contador + 1){
            Rascunho = TabelaDeExclusao.ContagemParaExpirar[Contador];
            if (TabelaDeExclusao.Excluido[Contador] == SAIU_NA_AREA_SEGURA){
                if (Rascunho > 0){
                    TabelaDeExclusao.ContagemParaExpirar[Contador] = TabelaDeExclusao.ContagemParaExpirar[Contador] - 1;
                }
            }

            //Rotinas para remover elementos que sariam da area segura
            if ((TabelaDeExclusao.ContagemParaExpirar[Contador] == 0) &&
                    ( TabelaDeExclusao.Elementos[Contador][0] != 0) &&
                    (TabelaDeExclusao.Excluido[Contador] ==  SAIU_NA_AREA_SEGURA)){
                removeElementoDaTabelaDeExclusao((unsigned char *)TabelaDeExclusao.Elementos[Contador]);
                TabelaDeExclusao.ContagemParaExpirar[Contador] = -1;
            }
        }
    }
}





void operacaoDeCodigoCorrenteParaExclusao(void){
    if (ContadorParaEnvioDeDadosDeExclusao == 0){
        //enviarElementosExcluidosPorZigBee();
        ContadorParaEnvioDeDadosDeExclusao = TEMPO_PARA_ENVIAR_TABELA_DE_EXCLUSAO;
        //Comentado essa linha impeco o portal de ser alpha, GILSON 21/09/18
#ifdef PODE_SER_ALPHA
#ifdef WIFI
        (void)atualizaTabelaDeExclusaoParaEnvioPorWifi();
#else
        (void)envioDeTabelaDeExclusao();
#endif
#endif
    }
}


union {
    unsigned char bytes[4];
    unsigned long Dado;
} Conversao;


extern char empilhadeira[20];
/*
void formaStringDeComandoDeEnvendoDeMovimentoDePedestre(unsigned char *Saida, unsigned char *Epc, time_t Momento, unsigned char Comando){
    char Dados[10];
    unsigned char CheckSum;
    int Contador;
    Dados[0] = Comando;
    Dados[1] = 10;
    Dados[2] = *Epc;
    Epc = Epc + 1;
    Dados[3] = *Epc;
    Epc = Epc + 1;
    Dados[4] = *Epc;
    Epc = Epc + 1;
    

    CheckSum = 0;
    for (Contador = 0;Contador < 5;Contador = Contador + 1){
        CheckSum = CheckSum + Dados[Contador];
    }
    
    
    Conversao.Dado = Momento;

    for (Contador = 0;Contador < 4;Contador = Contador + 1){
        Dados[5 + Contador] = Conversao.bytes[3 - Contador];
        CheckSum = CheckSum + Conversao.bytes[Contador];
    }
    
    Dados[9] = CheckSum;//CheckSum
    
    for (Contador = 0;Contador < 10;Contador = Contador + 1){
        *Saida = Dados[Contador];
        Saida = Saida + 1;
    }
}
*/
void formaStringDeComandoDeEnvendoDeMovimentoDePedestre(unsigned char *Saida, unsigned char *Epc, time_t Momento, unsigned char Comando){
    char Dados[10];
    unsigned char CheckSum;
    int Contador;
    
    memset(Dados, 0, 10);
    Dados[0] = Comando;
    Dados[1] = 8;//10;
    Dados[2] = *Epc;
    Epc = Epc + 1;
    Dados[3] = *Epc;
    Epc = Epc + 1;
    Dados[4] = *Epc;
    Epc = Epc + 1;
    //Dados[5] = empilhadeira[10];
    Dados[5] = NomeDoPortal[0];
    //Dados[6] = empilhadeira[11];
    Dados[6] = NomeDoPortal[1];

    CheckSum = 0;
    for (Contador = 0;Contador < 7;Contador = Contador + 1){
        CheckSum = CheckSum + Dados[Contador];
    }
    
    Dados[7] = CheckSum;//CheckSum
    
    for (Contador = 0;Contador < 10;Contador = Contador + 1){
        *Saida = Dados[Contador];
        Saida = Saida + 1;
    }
}



//void enviaEpcParaSerInvisivelNoPortalDeExclusao(unsigned char *Epc, time_t Momento){
//void enviaEventoDeQueUmPedestreSaiuNaAreaSegura(unsigned char *Epc, time_t Momento){
void enviaEventoDeQueUmPedestreEntrouNaAreaSeguraNoZigBee(unsigned char *Epc, time_t Momento){
    unsigned char Dados[10];
    int Contador;
    
    //formaStringDeComandoParaEpcSeTornarinvisivel(Dados, Epc, Momento);
    if (*Epc != 0){
        formaStringDeComandoDeEnvendoDeMovimentoDePedestre(Dados, Epc, Momento, 0xF5);

        if (PortaDePortalDeExclusao){
            //_LATG13 = 1;
            for (Contador = 0;Contador < 10; Contador = Contador + 1){
                uart4Tx(Dados[Contador]);
                //uart1Tx(Dados[Contador]);
                delay_us (120);

            }
            //_LATG13 = 0;
        }
    }
}



//void enviaEpcParaDeixarDeSerInvisivelNoPortalDeExclusao(unsigned char *Epc, time_t Momento){
void enviaEventoDeQueUmPedestreSaiuNaAreaSeguraNoZigBee(unsigned char *Epc, time_t Momento){
    unsigned char Dados[10];
    int Contador;

    //formaStringDeComandoParaEpcDeixarDeSerInvisivel(Dados, Epc, Momento);
    if (*Epc != 0){
        formaStringDeComandoDeEnvendoDeMovimentoDePedestre(Dados, Epc, Momento, 0xF6);

        if (PortaDePortalDeExclusao){
            //_LATG13 = 1;
            for (Contador = 0;Contador < 10; Contador = Contador + 1){
                uart4Tx(Dados[Contador]);
                //uart1Tx(Dados[Contador]);
                delay_us (120);
            }
            //_LATG13 = 0;
        }
    }
}


void enviaEventoDeQueUmPedestreEntrouNaAreaSeguraNa485(unsigned char *Epc, time_t Momento){
    volatile unsigned char Dados[10];
    //volatile int Contador;
    
    //formaStringDeComandoParaEpcSeTornarinvisivel(Dados, Epc, Momento);
    if (*Epc != 0){
        //void formaStringDeComandoDeEnvendoDeMovimentoDePedestre(unsigned char *Saida, unsigned char *Epc, time_t Momento, unsigned char Comando)
        formaStringDeComandoDeEnvendoDeMovimentoDePedestre((unsigned char *)Dados, Epc, Momento, 0xF5);

        if (PortaDePortalDeExclusao){
            //Solucao nova com FIFO, apos Fevereiro de 2019
            adicionaItemNaListaDeSaida((unsigned char *)Dados);
            adicionaElementoAFifo((char *)Dados);
            

            //Solucao velha usada em Juiz de Fora ate meados de Fevereiro de 2019
            /*
            adicionaItemNaListaDeSaida(Dados);
            
            aguradaSilencioNa485();
            _LATG13 = 1;
            delay_us(10);
            for (Contador = 0;Contador < 8; Contador = Contador + 1){
                uart1Tx(Dados[Contador]);
            }
            _LATG13 = 0;
            */
        }
    }
}



//void enviaEpcParaDeixarDeSerInvisivelNoPortalDeExclusaoPara485(unsigned char *Epc, time_t Momento){
void enviaEventoDeQueUmPedestreSaiuNaAreaSeguraNa485(unsigned char *Epc, time_t Momento){
    volatile unsigned char Dados[10];
    //volatile int Contador;

    //void formaStringDeComandoDeEnvendoDeMovimentoDePedestre(unsigned char *Saida, unsigned char *Epc, time_t Momento, unsigned char Comando)
    //formaStringDeComandoParaEpcDeixarDeSerInvisivel(Dados, Epc, Momento);
    formaStringDeComandoDeEnvendoDeMovimentoDePedestre((unsigned char *)Dados, Epc, Momento, 0xF6);

    if (PortaDePortalDeExclusao){
        //Solucao nova com FIFO, apos Fevereiro de 2019
        adicionaItemNaListaDeSaida((unsigned char *)Dados);
        adicionaElementoAFifo((char*)Dados);

        //Solucao velha usada em Juiz de Fora ate meados de Fevereiro de 2019
        /*
        adicionaItemNaListaDeSaida(Dados);
        aguradaSilencioNa485();
        _LATG13 = 1;
        delay_us(10);
        for (Contador = 0;Contador < 8; Contador = Contador + 1){
            uart1Tx(Dados[Contador]);
        }
        _LATG13 = 0;
        */
    }
}

void logicaDeRemocaoDeTabelaDeExclusao (void){
	//int Posicao;
    static int Posicao = 0;
    char Rascunho[3];
    int PosicaoAnterior;


    if (TabelaDeExclusaoOcupadoPorEnvio == 0){
        //for (Posicao = 0;Posicao < QUANTIA_MAXIMA_DE_OPERADORES_NO_SISTEMA; Posicao = Posicao + 1){
        PosicaoAnterior = Posicao;
        for (;Posicao < (PosicaoAnterior + 10); Posicao = Posicao + 1){
            if (TabelaDeExclusao.Elementos[Posicao][0] != 0){
#ifdef PORTAL
                if (TabelaDeExclusao.ContagemParaExpirar[Posicao] > 0){
#endif
                    sel_led(8,1);
                    TabelaDeExclusao.ContagemParaExpirar[Posicao]--;
                    if (TabelaDeExclusao.ContagemParaExpirar[Posicao] <= 0){
                    //if ((TabelaDeExclusao.ContagemParaExpirar[Posicao] == 0) || ((TabelaDeExclusao.ContagemParaExpirar[Posicao] <= (0xFFFF - TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO) && (TabelaDeExclusao.ContagemParaExpirar[Posicao] > 0xFF00) ))   ){
                         
                        //sel_led(4,1);
                        //removeElementoDaTabelaDeExclusao(TabelaDeExclusao.Elementos[Posicao]);
                        //Rascunho[0] = TabelaDeExclusao.Elementos[Posicao][0];
                        //Rascunho[1] = TabelaDeExclusao.Elementos[Posicao][1];
                        //Rascunho[2] = TabelaDeExclusao.Elementos[Posicao][2];
                        TabelaDeExclusao.Elementos[Posicao][0] = 0;
                        TabelaDeExclusao.Elementos[Posicao][1] = 0;
                        TabelaDeExclusao.Elementos[Posicao][2] = 0;
                        removeElementoDaTabelaDeExclusao((unsigned char*)Rascunho);
                        TabelaDeExclusao.ContagemParaExpirar[Posicao] = 0;
                        //sel_led(4,0);
                    }
                    sel_led(8,0);
                }
#ifdef PORTAL                
            }
#endif
        }
        Posicao = Posicao + 10;
        if (Posicao >= QUANTIA_MAXIMA_DE_OPERADORES_NO_SISTEMA)Posicao = 0;
    }
}


void logicaDeRemocaoDeTabelaDeExclusaoParaPortal (void){
	int Posicao;
//    char Rascunho[3];
	
	for (Posicao = 0;Posicao < QUANTIA_MAXIMA_DE_OPERADORES_NO_SISTEMA; Posicao = Posicao + 1){
        if (TabelaDeExclusao.Elementos[Posicao][0] != 0){
            if (TabelaDeExclusao.Excluido[Posicao] != 0){
                if (TabelaDeExclusao.ContagemParaExpirar[Posicao] > 0){
                    sel_led(8,1);
                    TabelaDeExclusao.ContagemParaExpirar[Posicao]--;
                    if (TabelaDeExclusao.ContagemParaExpirar[Posicao] == 0){
                        //sel_led(4,1);
                        //removeElementoDaTabelaDeExclusao(TabelaDeExclusao.Elementos[Posicao]);
                        /*
                        Rascunho[0] = TabelaDeExclusao.Elementos[Posicao][0];
                        Rascunho[1] = TabelaDeExclusao.Elementos[Posicao][1];
                        Rascunho[2] = TabelaDeExclusao.Elementos[Posicao][2];
                        removeElementoDaTabelaDeExclusao((unsigned char*)Rascunho);
                        TabelaDeExclusao.ContagemParaExpirar[Posicao] = 0;
                        */
                        //sel_led(4,0);
                        memset(TabelaDeExclusao.Elementos[Posicao], 0, TAMANHO_DOS_ELEMENTOS);
                        TabelaDeExclusao.ContagemParaExpirar[Posicao] = 0;
                    }
                    sel_led(8,0);
                }
            }
        }
	}
}



void despacheDePacotesParaOPcAlphaOuConcentradorDaRedePara485AtravesDeFifo(void){
    
}