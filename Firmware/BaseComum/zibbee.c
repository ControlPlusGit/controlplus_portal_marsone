#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <p24FJ256DA210.h>
#include "appl_commands.h"
//#include "empilhadeira.h"
//#include "C:\projetos\control-plus\Firmware\Sankyu\080116\Sankyu\AS3993FwSource\AS3993\Firmware\src\empilhadeira.h"
#include "perifericos.h"
#include "as3993_public.h"
#include "global.h"
#include "uart_driver.h"
#include "as3993.h"
#include "timer.h"

#include "zigbee.h"
#include "mem_i2c_24LC256.h"
#include "rtc.h"
#include "portal.h"
#include "exclusao.h"

extern char num_serie[20];

int silencioNoZigBee;
int ContadorParaExpirarOsDadosObtidosPorZigBee;

struct EstruturaProtocolo __attribute__((far)) Teste;
unsigned char PacoteDeTesteZigBee[TAMANHO_DO_PACOTE_DE_TESTE_DO_ZIGBEE];

//unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];//Era 10
char Saida;
int COntagemDeTempoParaExpirarOBufferDeRecepcao;


int tempoDeSilencioNoZigBee(void);
void zeraAContagemDeSilencioNoZigBee(void);
void incrementaContagemDeSilencioNoZigBee(void);

int ResultadoProtocolo;


//__eds__ unsigned char __attribute__((far)) BufferDeRecepcaoDaUartDaTabelaDeExclusao[1000];
unsigned char __attribute__((far)) BufferDeRecepcaoDaUartDaTabelaDeExclusao[1500];
int ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao = 0;
int ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao;
#define TEMPO_PARA_ESTOURO_DE_TEMPO_DA_RECEPCAO_NA_UART_DA_TABELA_DE_EXCLUSA0 500

void recebeDadosDaUartDaTabelaDeExclusao(unsigned char Dado){
    int Contador;
    
    if (ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao > 0){
        BufferDeRecepcaoDaUartDaTabelaDeExclusao[ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao] = Dado;
        ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao = ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao + 1;
        //ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao = TEMPO_PARA_ESTOURO_DE_TEMPO_DA_RECEPCAO_NA_UART_DA_TABELA_DE_EXCLUSA0;
    }

    if (Dado == 0xF9){ 
        if (ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao == 0){
            BufferDeRecepcaoDaUartDaTabelaDeExclusao[ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao] = Dado;
            ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao = ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao + 1;
            //ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao = TEMPO_PARA_ESTOURO_DE_TEMPO_DA_RECEPCAO_NA_UART_DA_TABELA_DE_EXCLUSA0;
        }
    }
    
   
    
    if (ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao >= 1500){
        //memset(BufferDeRecepcaoDaUartDaTabelaDeExclusao, 0, 1000);
        for(Contador = 0;Contador < 1000;Contador = Contador + 1){
            BufferDeRecepcaoDaUartDaTabelaDeExclusao[Contador] = 0;
        }
        ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao = 0;
        //ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao = TEMPO_PARA_ESTOURO_DE_TEMPO_DA_RECEPCAO_NA_UART_DA_TABELA_DE_EXCLUSA0;
    }
    
    
}

int haDadosASeremProcessadosNoBufferDeRecepcaoDaUartDaTabelaDeExclusao(void){
    
    if(ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao > 0) return 0;
    return -1;
    //if (ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao == 0)return 0;
    //return -1;
}

void decrementaContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao(void){
    if (ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao > 0){
        ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao  = ContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao  - 1;
    }
    
}

void processaBufferDaUartDaTabelaDeExclusao(void){
    int Contador;
    
    if (ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao != 0){
        if ( haDadosASeremProcessadosNoBufferDeRecepcaoDaUartDaTabelaDeExclusao() > -1){
            for (Contador = 0;Contador < ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao;Contador = Contador + 1){
                ResultadoProtocolo = trataRecepcaoNoProcolo (BufferDeRecepcaoDaUartDaTabelaDeExclusao[Contador], &Teste.Estado, &Teste.Comando, &Teste.Tamanho, NULL/*&Corpo[Teste.ContagemDoCorpo](*/, &Teste.ContagemDoCorpo, &Teste.CheckSum);
            }

            //memset(BufferDeRecepcaoDaUartDaTabelaDeExclusao, 0, 1000);
            for(Contador = 0;Contador < 1000;Contador = Contador + 1){
                BufferDeRecepcaoDaUartDaTabelaDeExclusao[Contador] = 0;
            }
            ContadorDoBuffferDeRecepcaoDaUartDaTabelaDeExclusao = 0;
        }
    }
}
 


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


/*
void zeraAContagemDeSilencioNoZigBee (void){
    if (tempoDeSilencioNoZigBee() > 15000){
        zeraAContagemDeSilencioNoZigBee();
        (void)envioDeTabelaDeExclusao();
    }
}
*/

void enviaTextoDeLogDaOperacao (char *Texto){
    //enviaMensagemAoMonitorPorZigBee(Texto, COMANDO_DE_LOG_ZIG_BEE);
}
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


//#define QUANTIA_DE_ELEMENTOS 100
#define TAMANHO_DOS_ELEMENTOS 5
#define QUANTIA_DE_DADOS_DA_TABELA (QUANTIA_DE_ELEMENTOS * TAMANHO_DOS_ELEMENTOS)
#define TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE ((QUANTIA_DE_ELEMENTOS * TAMANHO_DOS_ELEMENTOS) + QUANTIA_DE_ELEMENTOS * 3)

//unsigned char Corpo[10];
//__eds__ unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];
unsigned char __attribute__((far)) CorpoDoComando[TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE];

char Saida;
int COntagemDeTempoParaExpirarOBufferDeRecepcao;

void lidaComExpiracaoDoBufferDeRecepcaoDoZigBee (void){
    //int Contador;
    if (COntagemDeTempoParaExpirarOBufferDeRecepcao == 0){
        memset(CorpoDoComando, 0, TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE);
        /*
        for(Contador = 0;Contador < TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE;Contador = Contador + 1){
            Corpo[Contador] = 0;
        }
        */
        iniciaEstruturaDoProtocolo();
        COntagemDeTempoParaExpirarOBufferDeRecepcao = -1;
    } else {
        COntagemDeTempoParaExpirarOBufferDeRecepcao = COntagemDeTempoParaExpirarOBufferDeRecepcao - 1;
    }
}

void ResetaTempoParaExpirarOBufferDoZigBee (void){
    COntagemDeTempoParaExpirarOBufferDeRecepcao = 5;//10;
    ContadorParaExpirarOsDadosObtidosPorZigBee = TEMPO_PARA_EXPIRAR_OS_DADOS_DO_ZIGBEE;
}

void enviarOperadorPorZigBee(void){
   /* unsigned char x;
    int Contador;
    liga_led_zig();
    x = 0xF0;
    uart4Tx(x);   //start byte
    delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    uart4Tx(10);      //quantidade de bytes
    x += 10;
    delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    
    for (Contador = 0;Contador < 5;Contador =Contador + 1){
        uart4Tx(NomeDaEmpilhadeira[Contador]);
        x = x + NomeDaEmpilhadeira[Contador];
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }

    //uart4Tx(0x50);
    for (Contador = 0;Contador < 5;Contador =Contador + 1){
        uart4Tx(MeuOperador.Epc[Contador]);
        x = x + MeuOperador.Epc[Contador];
        delay_us (TEMPO_PARA_ENVIO_DE_BYTE_POR_ZIGBEE);
    }
    uart4Tx(x);             //cs
    desliga_led_zig();*/
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


extern int ResultadoProtocolo;
extern int EuSouOAlphaDosPortais;

void trataRecepcaoDeDadosDeZigBee(unsigned char Dado){
    int Resultado;
    int Contador;
    int Indice;
    
    union {
        unsigned char bytes[4];
        unsigned long Dado;
    } Conversao;
    
    
    //extern struct EstruturaTabelaDeExclusao __attribute__((far)) TabelaDeExclusao;
    
    unsigned char Rascunho[7];//5//3
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
    //Resultado = trataRecepcaoNoProcoloPC (Dado, &Teste.Estado, &Teste.Comando, &Teste.Tamanho, &Corpo[Teste.ContagemDoCorpo], &Teste.ContagemDoCorpo, &Teste.CheckSum);
    
    //Trocado para mudar a forma com que recebe os dados em 26/07/18
    //Resultado = trataRecepcaoNoProcolo (Dado, &Teste.Estado, &Teste.Comando, &Teste.Tamanho, NULL, &Teste.ContagemDoCorpo, &Teste.CheckSum);
    processaBufferDaUartDaTabelaDeExclusao();
    Resultado = ResultadoProtocolo;
    
    
    
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
            EuSouOAlphaDosPortais = 0;
            Indice = 0;
            //for(Contador = 2;Contador < Teste.ContagemDoCorpo;Contador = Contador + 5){
            for(Contador = 3;Contador < Teste.ContagemDoCorpo;Contador = Contador + 7){
                //*Ponteiro++ = Corpo[3 + Contador];
                //c[0] = CorpoDoComando[4 + Contador];
                //Rascunho[1] = CorpoDoComando[3 + Contador];
                //Rascunho[2] = CorpoDoComando[2 + Contador];
                
                Rascunho[0] = CorpoDoComando[5 + Contador];
                Rascunho[1] = CorpoDoComando[4 + Contador];
                Rascunho[2] = CorpoDoComando[3 + Contador];
                Rascunho[3] = CorpoDoComando[2 + Contador];
                Rascunho[4] = CorpoDoComando[1 + Contador];
                
                Rascunho[5] = CorpoDoComando[0 + Contador];
                Rascunho[6] = CorpoDoComando[Contador - 1];
                
                //LED_REDE = 1;
                //if(CorpoDoComando[1 + Contador] == 0){
                //if(CorpoDoComando[0 + Contador] == 0){
                    //sel_led(7,1);
                    //(void)adicionaElementoDaTabelaDeExclusao (&CorpoDoComando[2 + Contador], ENTROU_NA_AREA_SEGURA);
                    if ((unsigned char)Rascunho[5] == 0){
                        (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], ENTROU_NA_AREA_SEGURA);
                    } else {
                        (void)adicionaElementoDaTabelaDeExclusao ((unsigned char *)Rascunho, Rascunho[3], Rascunho[4], SAIU_NA_AREA_SEGURA);
                    }
                    
                    //sel_led(7,0);
                /*
                } else {
                    //sel_led(4,1);
                    removeElementoDaTabelaDeExclusao((unsigned char *)Rascunho);
                    //sel_led(4,0);
                }
                */
                //LED_REDE = 0;
                
                /*/
                TabelaDeExclusao.Elementos[Indice][0] = CorpoDoComando[4 + Contador];
                TabelaDeExclusao.Elementos[Indice][1] = CorpoDoComando[3 + Contador];
                TabelaDeExclusao.Elementos[Indice][2] = CorpoDoComando[2 + Contador];
                TabelaDeExclusao.Excluido[Indice] = CorpoDoComando[1 + Contador];
                TabelaDeExclusao.ContagemParaExpirar[Indice] = CorpoDoComando[Contador];
                */
                Indice = Indice + 1;
                if (Indice >= QUANTIA_DE_ELEMENTOS){
                    Indice = 0;
                }
            }
            
            if(Teste.Comando == 0xFA){//Data e hora por epoch
                
                for (Contador = 0;Contador < 4;Contador = Contador + 1){
                    Conversao.bytes[Contador] = CorpoDoComando[3 - Contador];
                }
                
                SetaTempoComEpoch(Conversao.Dado);
            }
            
            goto finaliza;
        }

        /*
        switch(Teste.Comando){
            
            case 0xF0:
                (void)cadastrarOperador(&Corpo[5]);
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
                (void)adicionarPedestre(Corpo, Corpo[3], Corpo[4], 0);//Cuidado com o parametro Indice, vou que acertar isso no futuro
                break;

            case 0xF4:
                //enviaMensagemAoMonitorPorZigBee("Pedestre virtual adicionado", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Pedestre virtual adicionado\n");
                DirecaoVirtual = Corpo[0];
                break;
                
            case 0xF5:
                //adicionaItemNaTabelaDeInvisiveis(Corpo);
                (void)adicionaElementoDaTabelaDeExclusao (Corpo, ENTROU_NA_AREA_SEGURA);
                //(void)cadastrarPedestreAListaDoPortalDeExclusao(Corpo);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre passou a ser invisiveil", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Um pedestre passou a ser invisiveil\n");
                break;

            case 0xF6:
                //(void)removePedestreDaListaDoPortalDeExclusaoPorEpc(Corpo);
                (void)adicionaElementoDaTabelaDeExclusao (Corpo, SAIU_NA_AREA_SEGURA);
                //enviaMensagemAoMonitorPorZigBee("Um pedestre deixou de ser invisivel", COMANDO_DE_LOG_ZIG_BEE);
                enviaTextoDeLogDaOperacao("Um pedestre deixou de ser invisivel\n");
                break;
              
            case 0xF7:
                as3993SingleWrite(AS3993_REG_RFOUTPUTCONTROL, Corpo[0]);
                break;

            case 0xF8:
                mem_escreveDados_24LC256(ADDR_NOME_NA_REDE_ZB, Corpo, TAMANHO_DO_NOME_NA_REDE_ZB);
                //mem_leDados_24LC256(ADDR_NOME_NA_REDE_ZB, &Nome[0], TAMANHO_DO_NOME_NA_REDE_ZB);
                break;
                
            case 0xF9://Receber/enviar tabela de exclusao
                if (trataRecepcaoDeTabelaDeExclusao((char *)&Corpo[3], Teste.Tamanho) > -1){//Essa linha esta errada
                    processaTabelaDeExclusao();
                }
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
        goto finaliza;
    }

    if (Resultado == -1){
        //memset(Teste, 0, sizeof(Teste));
finaliza:        
        memset(CorpoDoComando, 0, TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE);//10
        /*
        for(Contador = 0;Contador < TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE;Contador = Contador + 1){
            Corpo[Contador] = 0;
        }
        */
        iniciaEstruturaDoProtocolo();
    }
}

int trataRecepcaoNoProcolo (unsigned char Dado, int *Estado, unsigned char *Comando, unsigned int *Tamanho, unsigned char *Corpo, int *ContadorDoCorpo, unsigned int *CheckSum){
    //unsigned char Rascunho;
    unsigned int RascunhoCheckSum;
    int TamanhoTemporario;
    static unsigned char DadoAnterior;
	*CheckSum = *CheckSum ^ Dado;
    static unsigned char DadosDebug[20];
    static unsigned int  DadosCheckSum[20];
    static unsigned char ContagemDebug;
    static int TamanhoDebug; 
    
    
	*Tamanho = *Tamanho - 1;
	switch (*Estado){
		case ESPERANDO_CABECA:
CABECA:           
            ContagemDebug = 0;
            //Rascunho = Dado & 0xF0;
            //if (Rascunho == 0xF0){
            if (Dado == 0xF9){
                *Estado = ESPERANDO_TAMANHO_MSB;
                *CheckSum = 0x5555;
                *CheckSum = *CheckSum ^ Dado;
                //printf("Obtem cabeca = %02X\n", Dado);
                *Comando = Dado;
                *ContadorDoCorpo = 0;
            }
			break;
        case ESPERANDO_TAMANHO_MSB:
            *Tamanho = Dado;
            if (*Tamanho >= (TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE >> 8)){
                *Estado = ESPERANDO_CABECA;
                *Tamanho = 0;
                goto CABECA;
            } else {
                *Tamanho = *Tamanho << 8;
                *Estado = ESPERANDO_TAMANHO_LSB;
            }
            break;
            
		case ESPERANDO_TAMANHO_LSB:
			*Estado = PEGANDO_O_CORPO;
			//printf("Obtem tamanho = %d\n", Dado);
			//*Tamanho = *Tamanho + (Dado - 2);//3//2
            Dado = Dado - 2;//3//2
            *Tamanho = *Tamanho + Dado;
            TamanhoDebug = *Tamanho;
            if (*Tamanho >= TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE){//16
                *Estado = ESPERANDO_CABECA;
                *Tamanho = 0;
                goto CABECA;
            }
			break;
		case PEGANDO_O_CORPO:
            TamanhoTemporario = *Tamanho;
			if (TamanhoTemporario != 0){
				//*Corpo = Dado;
                //Corpo[Teste.ContagemDoCorpo] = Dado;
                if ((TamanhoTemporario < TAMANHO_MAXIMO_DE_RECEPCAO_DE_PACOTE) && (TamanhoTemporario > -1)){//16
                    CorpoDoComando[TamanhoTemporario] = Dado;
                    //printf("Obtem corpo = %c\n", Dado);
                    *ContadorDoCorpo = *ContadorDoCorpo + 1;
                } else {
                    *Estado = ESPERANDO_CABECA;
                    *Tamanho = 0;
                    goto CABECA;
                }
                
			} else {
                RascunhoCheckSum = DadoAnterior;
                RascunhoCheckSum = RascunhoCheckSum << 8;
                RascunhoCheckSum = RascunhoCheckSum | Dado;
				*CheckSum = *CheckSum ^ ((~Dado) & 0x00FF);
                
				//printf("CheckSum CG = %d e DD %d\n", *CheckSum, Dado);
				
				*Estado = ESPERANDO_CABECA;
				//if (Dado == *CheckSum){
                if (ContagemDebug < 2){
                    if (ContagemDebug == 1){
                        ContagemDebug = 19;
                    } else {
                        ContagemDebug = 18;
                    }
                } else {
                    ContagemDebug = ContagemDebug - 2;
                }
                *CheckSum = DadosCheckSum[ContagemDebug];
                //*CheckSum = DadosCheckSum[ContagemDebug - 2];

                RascunhoCheckSum = Dado;
                *CheckSum = *CheckSum & 0x00FF;

                
                if (RascunhoCheckSum == *CheckSum){
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
    DadoAnterior = Dado;
    DadosDebug[ContagemDebug] = Dado;
    DadosCheckSum[ContagemDebug++] = *CheckSum;
    if (ContagemDebug >= 20)ContagemDebug = 0;
    
	return 0;
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



