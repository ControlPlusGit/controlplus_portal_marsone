#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <p24FJ256DA210.h>
#include "appl_commands.h"
#include "empilhadeira.h"
#include "perifericos.h"
#include "as3993_public.h"
#include "global.h"
#include "uart_driver.h"
#include "timer.h"
#include "zigbee.h"
#include "as3993.h"
#include "mem_i2c_24LC256.h"
#include "rtc.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\exclusao.h" 
#include "zigbee.h"


extern char num_serie[20];

int silencioNoZigBee;

int ContadorParaExpirarOsDadosObtidosPorZigBee;

unsigned char PacoteDeTesteZigBee[TAMANHO_DO_PACOTE_DE_TESTE_DO_ZIGBEE];
struct EstruturaProtocolo Teste;
//struct EstruturaTabelaDeExclusao TabelaDeExclusaoSendoRecebida;
//__eds__ struct EstruturaTabelaDeExclusao TabelaDeExclusao;
//__eds__ struct __attribute__((far)) EstruturaTabelaDeExclusao TabelaDeExclusao;
struct EstruturaTabelaDeExclusao TabelaDeExclusao;

int tempoDeSilencioNoZigBee(void);
void zeraAContagemDeSilencioNoZigBee(void);
void incrementaContagemDeSilencioNoZigBee(void);

//int elementroPresenteNaTabelaDeExclusao(char *Epc);
int posicaoLivreNaTabelaDeExclusao(void);
int trataRecepcaoDeTabelaDeExclusao (char *Dado, int Tamanho);
int envioDeTabelaDeExclusao (void);
void processaTabelaDeExclusao(void);
//int adicionaElementoDaTabelaDeExclusao (char *Epc);
//int removeElementoDaTabelaDeExclusao (char *Epc);



void iniciaEstruturaDoProtocolo (void){
    sel_led(6,1);
    Teste.Comando = 0;
    Teste.Estado = 0;
    Teste.Tamanho = 0;
    Teste.ContagemDoCorpo = 0;
    ContadorParaExpirarOsDadosObtidosPorZigBee = 0;
    //delay_ms(1);
    sel_led(6,0);
}

void operacoesEmTickParaZigBee (void){
    sel_led(5,1);//Apaga leds
    incrementaContagemDeSilencioNoZigBee();
    if (ContadorParaExpirarOsDadosObtidosPorZigBee > 0){
        ContadorParaExpirarOsDadosObtidosPorZigBee = ContadorParaExpirarOsDadosObtidosPorZigBee - 1;
        if (ContadorParaExpirarOsDadosObtidosPorZigBee == 0){
            ContadorParaExpirarOsDadosObtidosPorZigBee = -1;
            iniciaEstruturaDoProtocolo();
            //(void)envioDeTabelaDeExclusao();
        }
        
    }
    
    sel_led(5,0);//Apaga leds
}

void operacoesEmCodigoCorrenteParaZigBee (void){
    if (tempoDeSilencioNoZigBee() > 15000){
        zeraAContagemDeSilencioNoZigBee();
        //(void)envioDeTabelaDeExclusao();
    }
}

void enviaTextoDeLogDaOperacao (char *Texto){
    //enviaMensagemAoMonitorPorZigBee(Texto, COMANDO_DE_LOG_ZIG_BEE);
}

void enviaMensagemAoMonitorPorZigBee(char *Mensagem, unsigned char Comando){
    int Contador;
    int Limite;
    liga_led_zig();
    Limite = trataEnvioDeProtocolo((unsigned char *)Mensagem, strlen(Mensagem), 
            Comando, PacoteDeTesteZigBee);
    for(Contador = 0;Contador < Limite;Contador = Contador + 1){
        uart4Tx(PacoteDeTesteZigBee[Contador]);
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }
    desliga_led_zig();    
}

void enviaBytePorZigBee(unsigned char Dado){
    uart4Tx(Dado);
    //delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);    
}

void enviaWordPorZigBee(unsigned int Dado){
    enviaBytePorZigBee((unsigned char)(Dado >> 8));
    enviaBytePorZigBee((unsigned char)Dado);
}


void enviaMensagemBinariaAoMonitorPorZigBee(char *Mensagem, int Tamanho, unsigned char Comando){
    //int Contador;
    //int Limite;
	unsigned int CheckSum;
    //int TamanhoParaRetornar;
    
    liga_led_zig();
    //Limite = trataEnvioDeProtocolo((unsigned char *)Mensagem, Tamanho, Comando, PacoteDeTesteZigBee);
    /*
    for(Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        uart4Tx(PacoteDeTesteZigBee[Contador]);
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }
    */
	
	CheckSum = Comando;
    enviaBytePorZigBee(Comando);
    enviaWordPorZigBee(Tamanho + 4);
	CheckSum = CheckSum + Tamanho;
	Tamanho = Tamanho - 4;
	
	while(Tamanho != 0){
        enviaBytePorZigBee(*Mensagem);
		CheckSum = CheckSum + *Mensagem;
		Mensagem = Mensagem + 1;
		Tamanho = Tamanho - 1;
	}
	
    enviaWordPorZigBee(CheckSum);
    desliga_led_zig();    
    //return 0;
}

#define QUANTIA_DE_ELEMENTOS 100//250
#define TAMANHO_DOS_ELEMENTOS 7//4//5
#define QUANTIA_DE_DADOS_DA_TABELA (QUANTIA_DE_ELEMENTOS * TAMANHO_DOS_ELEMENTOS)
#define TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE ((QUANTIA_DE_ELEMENTOS * TAMANHO_DOS_ELEMENTOS) + QUANTIA_DE_ELEMENTOS * 3)

//unsigned char Corpo[10];
//__eds__ unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];
unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];

char Saida;
int COntagemDeTempoParaExpirarOBufferDeRecepcao;

void lidaComExpiracaoDoBufferDeRecepcaoDoZigBee (void){
    int Contador;
    if (COntagemDeTempoParaExpirarOBufferDeRecepcao == 0){
        //memset(Corpo, 0, 10);
        for(Contador = 0;Contador < TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE;Contador = Contador + 1){
            CorpoDoComando[Contador] = 0;
        }
        iniciaEstruturaDoProtocolo();
        COntagemDeTempoParaExpirarOBufferDeRecepcao = -1;
    } else {
        COntagemDeTempoParaExpirarOBufferDeRecepcao = COntagemDeTempoParaExpirarOBufferDeRecepcao - 1;
    }
}

void ResetaTempoParaExpirarOBufferDoZigBee (void){
    //sel_led(3,1);
    COntagemDeTempoParaExpirarOBufferDeRecepcao = 10;//10;
    ContadorParaExpirarOsDadosObtidosPorZigBee = TEMPO_PARA_EXPIRAR_OS_DADOS_DO_ZIGBEE;
    //sel_led(3,0);
}

void enviarOperadorPorZigBee(void){
    /*
    unsigned char x;
    int Contador;
    liga_led_zig();
    x = 0xF0;
    uart4Tx(x);   //start byte
    delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    uart4Tx(10);      //quantidade de bytes
    x += 10;
    delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    
    for (Contador = 0;Contador < TAMANHO_DO_NOME_DA_EMPILHADEIRA;Contador =Contador + 1){
        uart4Tx(NomeDaEmpilhadeira[Contador]);
        x = x + NomeDaEmpilhadeira[Contador];
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }

    //uart4Tx(0x50);
    for (Contador = 0;Contador < TAMANHO_EPC_PARA_EMPILHADEIRA;Contador =Contador + 1){
        uart4Tx(MeuOperador.Epc[Contador]);
        x = x + MeuOperador.Epc[Contador];
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }
    uart4Tx(x);             //cs
    desliga_led_zig();
     * */
}

void enviaIdPorZigBee(char *Id){
    //00 00 17 c7 ac fc
    char Nome[TAMANHO_DO_NOME_NA_REDE_ZB];
    char Saida[30];
    Nome[TAMANHO_DO_NOME_NA_REDE_ZB - 1] = 0;
    mem_leDados_24LC256(ADDR_NOME_NA_REDE_ZB, (unsigned char *)&Nome[0], TAMANHO_DO_NOME_NA_REDE_ZB);
    Nome[TAMANHO_DO_NOME_NA_REDE_ZB - 1] = 0;
    sprintf(Saida, "%s%s", Id, Nome);
    //enviaMensagemAoMonitorPorZigBee(Id, COMANDO_DE_ID_ZIG_BEE);
    while(100 < tempoDeSilencioNoZigBee());
    enviaMensagemAoMonitorPorZigBee(Saida, COMANDO_DE_ID_ZIG_BEE);
    //mem_escreveDados_24LC256(ADDR_NOME_NA_REDE_ZB, Nome, TAMANHO_DO_NOME_NA_REDE_ZB);
}


extern ResultadoProtocolo;


void TrataRecepcaoDeDadosDeZigBee(unsigned char Dado){
    int Resultado;
    int Contador;
    int Indice;
    char Rascunho[5];
    //sprintf(Rascunho, "%X", Dado);
    //enviaTextoDeLogDaOperacao("Dado recebido = ");
    //char Nome[TAMANHO_DO_NOME_NA_REDE_ZB];
    //uart4Tx(Dado);
    
    //delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    //uart4Tx('\n');
    //delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    
    //ContadorParaExpirarOsDadosObtidosPorZigBee = TEMPO_PARA_EXPIRAR_OS_DADOS_DO_ZIGBEE;
            
    //ResetaTempoParaExpirarOBufferDoZigBee();
    //int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned char *CheckSum){
    //int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum){
    //sel_led(8,1);
    
    
    
    //Resultado = trataRecepcaoNoProcolo (Dado, &Teste.Estado, &Teste.Comando, &Teste.Tamanho, NULL/*&Corpo[Teste.ContagemDoCorpo](*/, &Teste.ContagemDoCorpo, &Teste.CheckSum);
    
    Resultado = ResultadoProtocolo;
    
    //uart4Tx(Dado);
    //sel_led(8,0);
    if (Teste.Comando != 0xF9){
        goto finaliza;
    }
    
    if (Resultado == 1){
        
    //enviaMensagemAoMonitorPorZigBee("Comando recebido", Teste.Comando);
    //enviaTextoDeLogDaOperacao("Comando recebido\n");
        if(Teste.Comando == 0xF9){
            //if (trataRecepcaoDeTabelaDeExclusao((char *)&Corpo[3], Teste.Tamanho) > -1){//Essa linha esta errada
                    //processaTabelaDeExclusao();
            //}
            //trataRecepcaoDeTabelaDeExclusao((char *)&Corpo[3], Teste.Tamanho);

            if (Teste.Tamanho > TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){
                goto finaliza;
            }
            //char* Ponteiro = &TabelaDeExclusao.ContagemParaExpirar[0];
            Indice = 0;
            //for(Contador = 2;Contador < Teste.ContagemDoCorpo;Contador = Contador + 5){
            for(Contador = 4;Contador < Teste.ContagemDoCorpo;Contador = Contador + 7){// DE 5 para 7 para Juiz de Fora
                //sel_led(4,1);
                if ((CorpoDoComando[4 + Contador] == 0x30) || (CorpoDoComando[4 + Contador] == 0x50)){
                    Rascunho[0] = CorpoDoComando[4 + Contador];
                    Rascunho[1] = CorpoDoComando[3 + Contador];
                    Rascunho[2] = CorpoDoComando[2 + Contador];

                    //if(CorpoDoComando[1 + Contador] == 0){
                    if(CorpoDoComando[Contador - 1] == 0){
                        //sel_led(7,1);
                        liga_led_wifi();
                        //adicionaElementoDaTabelaDeExclusao(Rascunho);
                        //sel_led(7,0);
                        desliga_led_wifi();
                    } else {
                        //sel_led(4,1);
                        removeElementoDaTabelaDeExclusao(Rascunho);
                        //sel_led(4,0);
                    }
                }
                
                /*
                TabelaDeExclusao.Elementos[Indice][0] = CorpoDoComando[4 + Contador];
                TabelaDeExclusao.Elementos[Indice][1] = CorpoDoComando[3 + Contador];
                TabelaDeExclusao.Elementos[Indice][2] = CorpoDoComando[2 + Contador];
                TabelaDeExclusao.Excluido[Indice] = CorpoDoComando[1 + Contador];
                TabelaDeExclusao.ContagemParaExpirar[Indice] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO            ;
                */
                Indice = Indice + 1;
                if (Indice >= QUANTIA_DE_ELEMENTOS){
                    Indice = 0;
                }
                //sel_led(4,0);
            }
            iniciaEstruturaDoProtocolo();
            memset(CorpoDoComando, 0, TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE);
            
            goto finaliza;
        }
        
        //enviaMensagemAoMonitorPorZigBee("Comando recebido", Teste.Comando);
        //enviaTextoDeLogDaOperacao("Comando recebido\n");
        /*
        switch(Teste.Comando){
            
            case 0xF0:
                //(void)cadastrarOperador(&Corpo[5]);
                break;
                
            case 0xF1://Monitor envia um operador para a maquina
                for(Contador = 0;Contador < TAMANHO_EPC_PARA_EMPILHADEIRA;Contador = Contador + 1){
                    MeuOperador.Epc[Contador] = Corpo[Contador];
                }
                MeuOperador.TempoDeValidade = TEMPO_DE_VALIDADE_DE_OPERADOR;
                //enviaMensagemAoMonitorPorZigBee("Operador obtido", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Operador obtido\n");
                break;
                
            case 0xF2:
                CintoVirtual = Corpo[0];
                if (CintoVirtual != 0){
                    //enviaMensagemAoMonitorPorZigBee("Cinto virtual colocado", COMANDO_DE_LOG_ZIG_BEE);
                    enviaTextoDeLogDaOperacao("Cinto virtual colocado");
                } else {
                    //enviaMensagemAoMonitorPorZigBee("Cinto virtual retirado", COMANDO_DE_LOG_ZIG_BEE);
                    enviaTextoDeLogDaOperacao("Cinto virtual retirado\n");
                }
                break;
                
            case 0xF3:
                //enviaMensagemAoMonitorPorZigBee("Pedestre virtual adicionado", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Pedestre virtual adicionado\n");
                //(void)adicionarPedestre(Corpo, Corpo[3], Corpo[4], 0);
                //Cuidado com o parametro Indice, vou que acertar isso no futuro
                break;

            case 0xF4:
                //enviaMensagemAoMonitorPorZigBee("Pedestre virtual adicionado", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Pedestre virtual adicionado\n");
                DirecaoVirtual = Corpo[0];
                break;
                
            case 0xF5:
                //(void)cadastrarPedestreAListaDoPortalDeExclusao(Corpo);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre passou a ser invisiveil", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Um pedestre passou a ser invisiveil\n");
                break;

            case 0xF6:
                //(void)removePedestreDaListaDoPortalDeExclusaoPorEpc(Corpo);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre deixou de ser invisivel", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Um pedestre deixou de ser invisivel\n");
                break;
              
            case 0xF7:
                as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, Corpo[0]);
                break;

            case 0xF8:
                //mem_escreveDados_24LC256(ADDR_NOME_NA_REDE_ZB, Corpo, TAMANHO_DO_NOME_NA_REDE_ZB);
                //mem_leDados_24LC256(ADDR_NOME_NA_REDE_ZB, &Nome[0], TAMANHO_DO_NOME_NA_REDE_ZB);
                break;
                

            case 0xF9://Receber/enviar tabela de exclusao
                //if (trataRecepcaoDeTabelaDeExclusao((char *)&Corpo[0], Teste.Tamanho) > -1){//Essa linha esta errada
                    //processaTabelaDeExclusao();
                //}
                break;
                
                
            case 0xFE:
                break;
            
            case 0xFF:
                //FF-06-FF-FF-FF-03
                //
                if ((Corpo[0] == 'A') && (Corpo[1] == 'A') && (Corpo[2] == 'A')){
                    //enviaTextoDeLogDaOperacao("BroadCast recebido\n");
                    //enviaTextoDeLogDaOperacao(NomeDaEmpilhadeira);
                    //enviaTextoDeLogDaOperacao(num_serie);
                    enviaIdPorZigBee(num_serie);
                } else { 
                    enviaTextoDeLogDaOperacao("Falha ao obter broadcast\n");
                }
                break;
            default:
                //enviaMensagemAoMonitorPorZigBee("Comando nao reconhecido\n", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Comando nao reconhecido\n");
                break;
        }
        */
        //memset(Corpo, 0, 10);
        //iniciaEstruturaDoProtocolo();
        goto finaliza;
    }

    if (Resultado == -1){
finaliza:        
        //memset(Teste, 0, sizeof(Teste));
        //memset(Corpo, 0, 10);
        for(Contador = 0;Contador < TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE;Contador = Contador + 1){
            CorpoDoComando[Contador] = 0;
        }
        iniciaEstruturaDoProtocolo();
    }
}

int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum){
    unsigned char Rascunho;
    //__eds__ unsigned int RascunhoCheckSum;
    unsigned int RascunhoCheckSum;
    int TamanhoTemporario;
    static unsigned char DadoAnterior;
    //__eds__ static unsigned char DadosDebug[20];
    static unsigned char DadosDebug[20];
    //__eds__ static unsigned int  DadosCheckSum[20];
    static unsigned int  DadosCheckSum[20];
    static unsigned char ContagemDebug;
    int Contador;
    
    *CheckSum = *CheckSum ^ Dado;
	*Tamanho = *Tamanho - 1;
	switch (*Estado){
		case ESPERANDO_CABECA:
            //sel_led(8,1);
            ContagemDebug = 0;
            Rascunho = Dado & 0xFF;
            if (Rascunho == 0xF9){
                *Estado = ESPERANDO_TAMANHO_MSB;
                *CheckSum = 0x5555;
                *CheckSum = *CheckSum ^ Dado;
                //printf("Obtem cabeca = %02X\n", Dado);
                *Comando = Dado;
                *ContadorDoCorpo = 0;
            }
            //sel_led(8,0);
			break;
        case ESPERANDO_TAMANHO_MSB:
            //sel_led(7,1);
            *Tamanho = Dado;
            *Tamanho = *Tamanho << 8;
            *Estado = ESPERANDO_TAMANHO_LSB;
            RascunhoCheckSum = Dado;
            RascunhoCheckSum = RascunhoCheckSum << 8;
            *CheckSum = *CheckSum ^ RascunhoCheckSum;
            break;
            
		case ESPERANDO_TAMANHO_LSB:
			*Estado = PEGANDO_O_CORPO;
			//printf("Obtem tamanho = %d\n", Dado);
			*Tamanho = *Tamanho + (Dado - 2);//3 //2
            //sel_led(7,0);
            if (*Tamanho >= TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){//16
                *Estado = ESPERANDO_CABECA;
                *Tamanho = 0;
                return -1;
            }
			break;
		case PEGANDO_O_CORPO:
            //return 0;
            //sel_led(4,1);
            TamanhoTemporario = *Tamanho;
			if (TamanhoTemporario != 0){
				//*Corpo = Dado;
                //Corpo[Teste.ContagemDoCorpo] = Dado;
                if ((TamanhoTemporario < /*16*/ TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE) && (TamanhoTemporario > -1)){//16
                    CorpoDoComando[TamanhoTemporario] = Dado;
                    //printf("Obtem corpo = %c\n", Dado);
                    *ContadorDoCorpo = *ContadorDoCorpo + 1;
                } else {
                    *Estado = ESPERANDO_CABECA;
                    *Tamanho = 0;
                    //sel_led(8,1);
                    //sel_led(4,1);
                    
                }
                
			} 
            else {
                
                RascunhoCheckSum = DadoAnterior;
                RascunhoCheckSum = RascunhoCheckSum << 8;
                RascunhoCheckSum = RascunhoCheckSum | Dado;
                
				*CheckSum = *CheckSum ^ ((~Dado) & 0x00FF);
				//printf("CheckSum CG = %d e DD %d\n", *CheckSum, Dado);
				
				*Estado = ESPERANDO_CABECA;
				//if (Dado == *CheckSum){
                
                /*
                if (ContagemDebug < 3){
                    return -1;
                }
                *CheckSum = DadosCheckSum[ContagemDebug - 2];
                */

                //sel_led(4,0);
                if (ContagemDebug < 3){
                    Contador = (20 - (ContagemDebug));
                    *CheckSum = DadosCheckSum[Contador];
                } else {
                    *CheckSum = DadosCheckSum[ContagemDebug - 2];
                }

                
                
                ContagemDebug = 0;
                if (RascunhoCheckSum == *CheckSum){
					//return 1;
                    //sel_led(6,1);
                    //sel_led(8,1);
                    
                    return 1;
				} else {
					//return -1;
                    //sel_led(8,1);
                    //sel_led(7,1);
                    
                    return -1;
				}
			}
            
			break;
		default:
			*Estado = ESPERANDO_CABECA;
            *Tamanho = 0;
			break;
	}
    
    // /*
    DadoAnterior = Dado;
    DadosDebug[ContagemDebug] = Dado;
    DadosCheckSum[ContagemDebug] = *CheckSum;
    ContagemDebug = ContagemDebug + 1;
    if (ContagemDebug >= 20)ContagemDebug = 0;
    // */

    /*
    DadoAnterior = Dado;
    if (ContagemDebug >= 20){
        for (Contador = 1;Contador < 21;Contador = Contador + 1){
            DadosDebug[Contador - 1] =  DadosDebug[Contador];
            DadosCheckSum[Contador - 1] =  DadosCheckSum[Contador];
        }
        DadosDebug[ContagemDebug] = Dado;
        DadosCheckSum[ContagemDebug] = *CheckSum;
        
    } else {
        
        DadosDebug[ContagemDebug] = Dado;
        DadosCheckSum[ContagemDebug] = *CheckSum;
        ContagemDebug = ContagemDebug + 1;
    }
    */
    

    
    
	return 0;
}


/*
int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, int *Tamanho,
        unsigned char *Corpo, int *ContadorDoCorpo, unsigned char *CheckSum){
    unsigned char Rascunho;
    zeraAContagemDeSilencioNoZigBee();
	*CheckSum = *CheckSum ^ Dado;
	*Tamanho = *Tamanho - 1;
	switch (*Estado){
		case ESPERANDO_CABECA:
            Rascunho = Dado & 0xF0;
            if (Rascunho == 0xF0){
                *Estado = ESPERANDO_TAMANHO;
                *CheckSum = Dado;
                //printf("Obtem cabeca = %02X\n", Dado);
                *Comando = Dado;
                *ContadorDoCorpo = 0;
            }
			break;
		case ESPERANDO_TAMANHO:
			*Estado = PEGANDO_O_CORPO;
			//printf("Obtem tamanho = %d\n", Dado);
			*Tamanho = Dado - 2;//2
            if (*Tamanho > 16){
                *Estado = ESPERANDO_CABECA;
                *Tamanho = 0;
            }
			break;
		case PEGANDO_O_CORPO:
			if (*Tamanho != 0){
				// *Corpo = Dado;
                CorpoDoComando[*ContadorDoCorpo] = Dado;
				//printf("Obtem corpo = %c\n", Dado);
				*ContadorDoCorpo = *ContadorDoCorpo + 1;
			} else {
				*CheckSum = *CheckSum - Dado;
				//printf("CheckSum CG = %d e DD %d\n", *CheckSum, Dado);
				
				*Estado = ESPERANDO_CABECA;
				if (Dado == *CheckSum){
					return 1;
				} else {
					return -1;
				}
			}
			break;
		default:
			*Estado = ESPERANDO_CABECA;
			break;
	}
	return 0;
}
*/


int trataEnvioDeProtocolo (unsigned char *Dado, int Tamanho, unsigned char Comando, unsigned char *Saida){
	unsigned char CheckSum;
    int TamanhoParaRetornar;
	
	*Saida = Comando;
	Saida = Saida + 1;
	CheckSum = Comando;
	//printf("\n%02X", Comando);
	//printf("|C=%02X|",CheckSum);
	
	Tamanho = Tamanho + 3;
	*Saida = Tamanho;
    TamanhoParaRetornar = Tamanho;
	Saida = Saida + 1; 
	//printf("%02X", Tamanho);
	CheckSum = CheckSum + Tamanho;
	//printf("|C=%02X|",CheckSum);
	
	Tamanho = Tamanho - 3;
	
	while(Tamanho != 0){
		//printf("%02X", *Dado);
		*Saida = *Dado;
		CheckSum = CheckSum + *Dado;
		//printf("|C=%02X|",CheckSum);
		//Saida = Saida + 1;
		Saida = Saida + 1;
		Dado = Dado + 1;
		Tamanho = Tamanho - 1;
	}
	
	*Saida = CheckSum;
	//printf("%02X\n", CheckSum);
	Saida = Saida + 1;
	return TamanhoParaRetornar;
}


int tempoDeSilencioNoZigBee(void){
    return silencioNoZigBee;
}

void zeraAContagemDeSilencioNoZigBee(void){
    silencioNoZigBee = 0;
}

void incrementaContagemDeSilencioNoZigBee(void){
    if (silencioNoZigBee < TEMPO_MAXIMO_DE_SILENCIO_DO_ZIGBEE){
        silencioNoZigBee = silencioNoZigBee + 1;
    }
}


int trataRecepcaoDeTabelaDeExclusao (char *Dado, int Tamanho){
    /*
    int Contador;
    for(Contador = 0;Contador < Tamanho;Contador = Contador + 1){
        TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos = &TabelaDeExclusaoSendoRecebida.Elementos[0][0];
        TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos 
                = TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos + TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes;
        *TabelaDeExclusaoSendoRecebida.PonteiroParaOsDadosRecebidos = *Dado;
        TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes = TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes + 1;
        if (TabelaDeExclusaoSendoRecebida.QuantiaDeDadosPresentes == QUANTIA_DE_DADOS_DA_TABELA){
            return 0;
        }
        Dado = Dado + 1;
    }
    */
    return -1;
}

int envioDeTabelaDeExclusao (void){
    //enviaMensagemBinariaAoMonitorPorZigBee((char *)&TabelaDeExclusao.MomentoQueFoiCriada, TAMANHO_DOS_DADOS_NA_TABELA, 0xF9);    
    return -1;
}

int removeElementoDaTabelaDeExclusao (unsigned char *Epc){
    int Posicao;
    int Contador;
    Posicao = elementroPresenteNaTabelaDeExclusao(Epc);
    if (Posicao < 0){
        return -1;
    }
    //sel_led(4,1);
    liga_led_gps();
    //(void)memset(TabelaDeExclusao.Elementos[Posicao], 0, TAMANHO_DOS_ELEMENTOS);
    //(void)memset(TabelaDeExclusao.Elementos[Posicao], 0, TAMANHO_DOS_ELEMENTOS);
    for(Contador = 0;Contador < TAMANHO_DOS_ELEMENTOS;Contador = Contador + 1){
        TabelaDeExclusao.Elementos[Posicao][Contador] = 0;
        TabelaDeExclusao.Excluido[Posicao] = 0;
        TabelaDeExclusao.ContagemParaExpirar[Posicao] = 0;
    }
    //sel_led(4,0);
    desliga_led_gps();
    //TabelaDeExclusao.MomentoQueFoiCriada = retornaComEpoch();
    return Posicao;
}

/*
int adicionaElementoDaTabelaDeExclusao (char *Epc){
    int Posicao;
    volatile int Contador;
    
    Posicao = elementroPresenteNaTabelaDeExclusao(Epc);
    if (Posicao < 0){
        Posicao = posicaoLivreNaTabelaDeExclusao();
        if (Posicao < 0){
            return -1;
        }
        

        //if (elementroPresenteNaTabelaDeExclusao(Epc) >= 0){
//            return -1;
//        }

         
        //memcpy(TabelaDeExclusao.Elementos[Posicao], Epc, TAMANHO_EPC_PARA_EMPILHADEIRA);
        for(Contador = 0;Contador < TAMANHO_DOS_ELEMENTOS;Contador = Contador + 1){
            TabelaDeExclusao.Elementos[Posicao][Contador] = *Epc;
            Epc = Epc + 1;
        }
        
        TabelaDeExclusao.Excluido[Posicao] = 0;
        TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
        
        //TabelaDeExclusao.MomentoQueFoiCriada = retornaComEpoch();
    } else {
        TabelaDeExclusao.ContagemParaExpirar[Posicao] = TEMPO_PARA_EXPIRAR_TAG_DA_TABELA_DE_EXCLUSAO;
    }
    return Posicao;
    
}
*/

int elementroPresenteNaTabelaDeExclusao(unsigned char *Epc){
    int Contador;
    int SubContador;
    char Resultado;
    
    char EpcDeComparacao[3];
    
    EpcDeComparacao[0] = *Epc;
    Epc = Epc + 1;
    EpcDeComparacao[1] = *Epc;
    Epc = Epc + 1;
    EpcDeComparacao[2] = *Epc;
	
	for(Contador = 0;Contador < LIMITE_DE_PEDESTRES;Contador = Contador + 1){
        Resultado = 0;
        //for (SubContador = 0;SubContador < TAMANHO_EPC_PARA_EMPILHADEIRA;SubContador = SubContador + 1){
            //if (TabelaDeExclusao.Elementos[Contador][SubContador] == *Epc){
                //Resultado = Resultado | 1;
            //}
            //Epc = Epc + 1;
            //Resultado = memcmp(TabelaDeExclusao.Elementos[Contador], Epc, 3);
            /*
            if ((TabelaDeExclusao.Elementos[Contador][0] == *Epc++) && 
                (TabelaDeExclusao.Elementos[Contador][1] == *Epc++) &&
                (TabelaDeExclusao.Elementos[Contador][2] == *Epc)){
                Resultado = 1;
            } else {
                Resultado = 0;
            }
            Epc = Epc - 2;
            */
            if ((TabelaDeExclusao.Elementos[Contador][0] == EpcDeComparacao[0]) && 
                (TabelaDeExclusao.Elementos[Contador][1] == EpcDeComparacao[1]) &&
                (TabelaDeExclusao.Elementos[Contador][2] == EpcDeComparacao[2])){
                //Resultado = 1;
                return Contador;
            } else {
                Resultado = 0;
            }
            if (Resultado == 1){
                return Contador;
            }
            
        //}
		//if (memcmp(TabelaDeExclusao.Elementos[Contador], Epc, TAMANHO_EPC_PARA_EMPILHADEIRA) == 0){
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

void processaElementosDasTabelaDeExclusao(void){
    //TabelaDeExclusaoSendoRecebida
    //TabelaDeExclusao
}


void processaTabelaDeExclusao(void){
    /*
    time_t TempoAtual, TempoDaTabelaLocal, TempoDaTabelaRecebida;
    //time_t TempoDoItem;
    TempoAtual = retornaComEpoch();
    //TempoDaTabelaRecebida = TabelaDeExclusaoSendoRecebida.MomentoQueFoiCriada;
    TempoDaTabelaLocal = Tempo;
    
    if (TempoDaTabelaLocal > TempoDaTabelaRecebida){
        //Envia tabela
        envioDeTabelaDeExclusao();
    } else {
        //Tabela recebida passa a ser a tabela local.
        consideraTabelaDeExclusaoRecebidaComoALocal();
        //processaElementosDeUmaTabelaDeExclusao();
    }
    */
    
}

void logicaDeRemocaoDeTabelaDeExclusao (void){
	int Posicao;
    char Rascunho[3];
    static ContadorDeChamadas;
	
    if (ContadorDeChamadas > 2){
        ContadorDeChamadas = 0;

        for (Posicao = 0;Posicao < QUANTIA_MAXIMA_DE_OPERADORES_NO_SISTEMA; Posicao = Posicao + 1){
            if (TabelaDeExclusao.Elementos[Posicao][0] != 0){
                if (TabelaDeExclusao.ContagemParaExpirar[Posicao] > 0){
                    //sel_led(8,1);
                    TabelaDeExclusao.ContagemParaExpirar[Posicao]--;
                    if (TabelaDeExclusao.ContagemParaExpirar[Posicao] < 1){

                        sel_led(8,1);
                        //sel_led(4,1);
                        //removeElementoDaTabelaDeExclusao(TabelaDeExclusao.Elementos[Posicao]);
                        /*
                        Rascunho[0] = TabelaDeExclusao.Elementos[Posicao][0];
                        Rascunho[1] = TabelaDeExclusao.Elementos[Posicao][1];
                        Rascunho[2] = TabelaDeExclusao.Elementos[Posicao][2];
                        removeElementoDaTabelaDeExclusao(Rascunho);
                        */
                        memset(TabelaDeExclusao.Elementos[Posicao], 0, TAMANHO_DOS_ELEMENTOS);
                        TabelaDeExclusao.ContagemParaExpirar[Posicao] = 0;
                        //sel_led(4,0);
                    }
                    sel_led(8,0);
                }
            }
        }
        
    } else {
        ContadorDeChamadas = ContadorDeChamadas + 1;
    }
}
