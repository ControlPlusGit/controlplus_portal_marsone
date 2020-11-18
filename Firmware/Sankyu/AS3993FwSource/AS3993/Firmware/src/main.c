#include <stdio.h>
#include <string.h>
#include "eth.h"
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
#include "gprs.h"
#include "sankyu.h"
#include "onewire.h"
#include "lista.h"
#include "config_i2c3.h"
#include "mem_i2c_24LC256.h"
#include "tags.h"
#include "empilhadeira.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\rtc.h"
#include "exclusao.h"
#include "portal.h"
#include "global.h"
#include "cancelas.h"
#include "as3993.h"
#include "ajuste_cap.h"
#include "setup_usb.h"
#include "i2c.h"

#include "FSM_DataHora.h"

#ifdef __PIC24FJ256DA210__

    _CONFIG1( WDTPS_PS1 & FWPSA_PR32 & ALTVREF_ALTVREDIS & WINDIS_OFF & 
              FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_ON & JTAGEN_OFF )

    _CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & 
              FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV3 & IESO_OFF )

    _CONFIG3( WPFP_WPFP0 & SOSCSEL_LPSOSC & WUTSEL_LEG & ALTPMP_ALPMPDIS & 
              WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM )
        
#endif

char num_serie[ 20 ];
    
char empilhadeira[ 20 ];

int dbm_tag_real;

extern void tick( void );

extern void ObtemID( char * );

extern Freq Frequencies;

extern Tag __attribute__( ( far ) ) tags_[ MAXTAG ];

int total_parcial = 0;

u8 cin, clen, cout = 0;

unsigned int total_tags = 0;

void lista_freq_anatel( void );

extern u8 inventoryGen2( void );

void ligaTimer2( void );

extern void uart4Tx( u8 );

extern u16 readerInitStatus;

/** FW information which will be reported to host. */

static const char gAS3993FwInfo[] = FIRMWARE_ID "||" HARDWARE_ID;

/** FW information which will be logged on startup. Version information is included in logger. */

static const char gLogStartup[] = FIRMWARE_ID" %hhx.%hhx.%hhx on "HARDWARE_ID"\n";

/** FW version which will be reported to host */

const u32 firmwareNumber = FIRMWARE_VERSION;

//static u8 usedAntenna = 8;

int tem_ze_na_area;

int TempoParaDesligarBuzzer;

#define  TEMPO_PARA_DESLIGAR_BUZZER 100

int antena_atual;

int ContadorParaUmSegundo;

int ContadorPara500ms;

int ContadorPara100ms;

char ContadorParaUmCentessimo;

int ContadorParaEnviarArrayXbee;

int ContadorParaRemocaoDeTabelaDeExclusao;

extern int timeoutRespostaKeepAlive;

extern unsigned char statusDeConexaoTCP;

extern int ContadorDeTempoParaManterCancelaDestravada;

void systemInit( void );

int trataOEnvioDaTabelaDeExclusaoEmCodigoCorrente( int QuantiaDeDados );

void mainSenCIN(int select) {
    if (select)
        SEN_TUNER_CIN(1);
    else
        SEN_TUNER_CIN(0);

}

void mainSenCLEN(int select) {
    if (select)
        SEN_TUNER_CLEN(1);
    else
        SEN_TUNER_CLEN(0);

}

void mainSenCOUT(int select) {
    if (select)
        SEN_TUNER_COUT(1);
    else
        SEN_TUNER_COUT(0);

}

TunerConfiguration mainTuner = {

    mainSenCIN,

    mainSenCLEN,
    
    mainSenCOUT
            
};

void trataTagsParaAKalunga( int Antena ) {
    
    int Contador;
    
    int SubContador;
    
    int Indice;
    
    char Mensagem[ 100 ];

    for ( Contador = 0; Contador < MAXTAG; Contador = Contador + 1 ) {
    
        if ( tags_[Contador].epclen != 0 ) { // Tem tag na posicao?
        
            Indice = sprintf( Mensagem, "ID=1059,Antena=%d,Etiqueta=", Antena );
            
            for ( SubContador = 0; SubContador < 12; SubContador = SubContador + 1 ) {
                            
                Indice = Indice + sprintf( &Mensagem[ Indice ], "%02X", tags_[ Contador ].epc[ SubContador ] );
            
            }
            
            Mensagem[ Indice ] = 0x0A;
            
            Indice = Indice + 1;
            
            for ( SubContador = 0; SubContador < Indice; SubContador = SubContador + 1 ) {
            
                uart3Tx( Mensagem[ SubContador ] );
            
            }
            
        }
        
    }
    
}

void PreTratamentoDeTagsParaOPortalFrango(int Antena, int AntenaReal) {
    int Contador;
    //char rascunho;

    for (Contador = 0; Contador < MAXTAG; Contador = Contador + 1) {
        if (tags_[Contador].epclen != 0) { // Tem tag na posicao?
            
            if ((tags_[Contador].epc[0] == 0x30) || (tags_[Contador].epc[0] == 0x50)) {
                (void) TrataTagDePortalFrango(&tags_[Contador].epc[0], Antena, AntenaReal); //Pego apenas os 3 ultimos bytes
            }
            memset(tags_[Contador].epc, 0, EPCLENGTH);
            memset(tags_[Contador].rn16, 0, 2);
            memset(tags_[Contador].pc, 0, 2);
            memset(tags_[Contador].handle, 0, 2);
            tags_[Contador].epclen = 0;
            tags_[Contador].agc = 0;
            
        }
    }
}

int leUmaEtiquetaParaTesteDeCapacete(int TempoDeLeitura, char *EpcCapturado) {
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

void calibracaoDoTestadorDeCapacete(void) {
    /*int Resultado[6];
    int Antena;
    char Texto[2];
    int Contador;

    while (1) {
        comecaInvetorio();
        while (1) {
            poe_texto_XY(1, 0, "  Lidos         ");
            poe_texto_XY(2, 0, "                ");

            for (Contador = 0; Contador < 100; Contador = Contador + 1) {
                for (Antena = 1; Antena < 6; Antena = Antena + 1) {
                    sel_led(Antena, 1);
                    sel_antena(Antena);
                    //memset(Etiqueta, 0, TAMANHO_EPC_PARA_EMPILHADEIRA );
                    //Resultado = leUmaEtiquetaParaTesteDeCapacete(100, Etiqueta);
                    Resultado[Antena] = Resultado[Antena] | inventorioSimplificado();
                    //Resultados[Antena - 1] = Resultado;
                    sel_led(Antena, 0);
                }
            }

            for (Antena = 1; Antena < 6; Antena = Antena + 1) {
                if (Resultado[Antena] > 0) {
                    liga_buzzer();
                    Texto[0] = '0';
                    Texto[1] = 0;
                    Texto[0] = Texto[0] + Antena;
                    poe_texto_XY(2, 1 + Antena, Texto);

                    desliga_buzzer();
                    desliga_led_tag();
                }
            }

            Texto[0] = 0;
            for (Antena = 1; Antena < 6; Antena = Antena + 1) {
                if (Resultado[Antena] > 0) {
                    Texto[0] = Texto[0] | 1;
                }
                Resultado[Antena] = 0;
            }
            if (Texto[0] > 0) {
                delay_ms(100);
            }
        }
        TerminaInvetorio();
    }*/
}

void testadorDeCapacete(void) {
    /*int Resultado;
    char Etiqueta[TAMANHO_EPC_PARA_EMPILHADEIRA];
    int Antena;
    int Resultados[5];
    int Contador;
    char Texto[2];

    //niciaPortalFrango();

    tunerInitCapacete(&mainTuner);
    calibracaoDoTestadorDeCapacete();

    while (1) {
        poe_texto_XY(1, 0, "  Control Plus  ");
        poe_texto_XY(2, 0, "Coloque capacete");
        if (!_RG6) {
            poe_texto_XY(1, 0, " Em testes      ");
            poe_texto_XY(2, 0, "                ");
            comecaInvetorio();
            for (Antena = 1; Antena < 6; Antena = Antena + 1) {
                sel_led(Antena, 1);
                sel_antena(Antena);
                memset(Etiqueta, 0, TAMANHO_EPC_PARA_EMPILHADEIRA);
                Resultado = leUmaEtiquetaParaTesteDeCapacete(100, Etiqueta);
                Resultados[Antena - 1] = Resultado;
                if (Resultado > -1) {
                    liga_led_tag();
                    liga_buzzer();
                    delay_ms(10);
                }
                sel_led(Antena, 0);
                desliga_led_tag();
                desliga_buzzer();
            }
            //poe_texto_XY(1,1,"Solto");
            TerminaInvetorio();

            Resultado = 1;
            for (Contador = 0; Contador < 5; Contador = Contador + 1) {
                if (Resultados[Contador] != 1)Resultado = -1;
                //Resultado = Resultado | Resultados[Contador];
            }

            poe_texto_XY(1, 0, "Testando        ");
            poe_texto_XY(2, 0, "                ");
            if (Resultado > -1) {
                poe_texto_XY(1, 0, " Sucesso ");
                liga_buzzer();
                delay_ms(300);
                desliga_buzzer();

            } else {
                poe_texto_XY(1, 0, "Falha nas tags:");
                for (Contador = 0; Contador < 5; Contador = Contador + 1) {
                    if (Resultados[Contador] < 0) {
                        Texto[0] = '1';
                        Texto[1] = 0;
                        Texto[0] = Texto[0] + Contador;
                        poe_texto_XY(2, 1 + Contador, Texto);
                    }
                }
                //poe_texto_XY(1,0," Sucesso ");
                liga_buzzer();
                delay_ms(100);
                desliga_buzzer();
                delay_ms(50);
                liga_buzzer();
                delay_ms(100);
                desliga_buzzer();
            }
            delay_ms(1000);
        }
    }*/

}

#ifdef RF_IDEIAS

void IniciaLeitorRfIdeias(void) {
    obtemParametrosSalvosNaEeprom();
}

void trataLeituraRfIdeias(int Antena) {
    int Contador;
    int SubContador;
    int Indice;
    char Mensagem[100];


    liga_led_tag();
    liga_buzzer();

    for (Contador = 0; Contador < MAXTAG; Contador = Contador + 1) {
        if (tags_[Contador].epclen != 0) { // Tem tag na posicao?
            atualizaHoraEData();
            memset(Mensagem, 0, 100);
            Indice = sprintf(Mensagem, "%d", Antena);
            //Epc
            for (SubContador = 0; SubContador < 12; SubContador = SubContador + 1) {
                Indice = Indice + sprintf(&Mensagem[Indice], "%02X", tags_[Contador].epc[SubContador]);
            }
            //Hora e Data
            Indice = Indice + sprintf(&Mensagem[Indice], "%s%s", stringHora, stringData);
            Mensagem[Indice] = 0x0A;

            Indice = Indice + 1;
            for (SubContador = 0; SubContador < Indice; SubContador = SubContador + 1) {
                //uart4Tx(Mensagem[SubContador]);
                uart3Tx(Mensagem[SubContador]);
                //uart2Tx(Mensagem[SubContador]);
                //uart1Tx(Mensagem[SubContador]);
            }
        }
    }
    desliga_led_tag();
    desliga_buzzer();
}

void setaAntenaELed(int Antena) {
    sel_led(0, 0);
    sel_led(Antena, 1);
    sel_antena(Antena);
}

void LeitorRfIdeias(void) {
    int Contador;
    int RepeticaoNaLeitura = 10;
    int QuantiaDeTagsObtidas;
    int Antena;

    tunerInit(&mainTuner);
    as3993SetSensitivity(SensibilidadeDaAntena);
    atualizaHoraEData();

    comecaInvetorio();
    while (1) {
        asm("CLRWDT");

        operacoesParaRtcEmCodigoCorrente();
        for (Antena = 1; Antena < (NumeroDeAntenasLidas + 1); Antena = Antena + 1) {
            setaAntenaELed(Antena);
            if (ModoDeOperacao == OPERACAO_COM_LEITURA_UNICA) {
                comecaInvetorio();
                for (Contador = 1; Contador < (RepeticaoNaLeitura + 1); Contador = Contador + 1) {
                    QuantiaDeTagsObtidas = inventorioSimplificadoComPausa();
                    if (QuantiaDeTagsObtidas)trataLeituraRfIdeias(Antena);
                }

                if (ModoDeOperacao != OPERACAO_COM_LEITURA_UNICA)comecaInvetorio();
            } else {
                for (Contador = 1; Contador < (RepeticaoNaLeitura + 1); Contador = Contador + 1) {
                    if (ModoDeOperacao == OPERACAO_LEITURAS_INITERRUPTAS)QuantiaDeTagsObtidas = inventorioSimplificado();
                    if (ModoDeOperacao == OPERACAO_COM_MULTIPLAS_LEITURAS)QuantiaDeTagsObtidas = inventorioSimplificadoComPausa();
                    if (QuantiaDeTagsObtidas)trataLeituraRfIdeias(Antena);
                }
            }
        }
    }
}


#endif

void ligaTodosOsLeds(void) {
    sel_led(0, 0);

    sel_led(0, 1);
    liga_led_tag();
    liga_saida_pa();
    liga_dir();
    liga_led_zig();
    liga_led_rede();
    liga_led_3g();
    liga_led_gps();
    liga_buzzer();

}

void desligaTodosOsLeds(void) {
    desliga_saida_pa();

    desliga_buzzer();
    desliga_led_vm();
    desliga_sirene();

    desliga_led_zig();
    desliga_led_rede();
    desliga_led_3g();
    desliga_led_gps();
    sel_led(0, 0);
    desliga_led_tag();
    desliga_dir();

}

/*
void testesRelesInterTravamento(void) {
    _TRISA7 = 0; //SAIDA LED A8
    while (1) {
        //ld_saidas(4, 0);
        ld_saidas(7, 1);
        //GP_15(1);
        delay_ms(1000);
        //ld_saidas(4, 1);
        ld_saidas(7, 0);
        //GP_15(0);
        delay_ms(1000);
    }
}
*/

/*
void maquinaDeEnvioDeDadosPorWifi(void) {
#define TAMANHO_DA_RESERVA_LOCAL 12
    static int Estado = 0;
    //int OcupacaoDoWifi;
    static char Rascunho[TAMANHO_DA_RESERVA_LOCAL];
    static int ContadorDoRascunho;
    
    
    static int Estados[20];
    static int ContadorEstados = 0;
    
    
    Estados[ContadorEstados] = Estado;
    ContadorEstados = ContadorEstados + 1;
    if (ContadorEstados >= 20){
        ContadorEstados = 0;
    }
    
#ifdef WIFI

#endif
    
    
    if (TimeOutEthPortal >= TIMEOUT_ETH_PORTAIS){ 
        if (ContadorReservaEthPortais > 0){
            lidaComATrocaDePortalAlpha();
            if (ContadorReservaEthPortais <= TAMANHO_DA_RESERVA_LOCAL){
                //memcpy(Rascunho, ReservaEthPortais, ContadorReservaEthPortais);
                memmove(Rascunho, ReservaEthPortais, ContadorReservaEthPortais);
                ContadorDoRascunho = ContadorReservaEthPortais;
                ContadorReservaEthPortais = 0;
            }
        }
        resetNaReservaDeDadosDaEth();
    }
    
#ifdef PODE_SER_ALPHA
    if (disponivelParaSerOAlpha() < 1){
        switch (Estado) {
            case 0:
                OcupacaoDoWifi = enviaParaTodosOsPortaisAPresencaDesseLeitor(-1);
                if (OcupacaoDoWifi < 0) {
                    Estado = 1;
                }
                break;
            case 1:
                //O envio da tabela de exclusao sera feito no momento em que obter uma reposta de algum membro da rede.
                OcupacaoDoWifi = enviaATabelaDeExclusaoParaAsEmpilhadeirasPresentes(-1);
                if (OcupacaoDoWifi < 0) {
                    Estado = 2;
                }

                break;
            case 2:
                //Repassa aas mensagens de movimento para o SARS 192.168.1.200:8000
                if (ContadorDoRascunho > 0){
                    enviaFluxoDeDadosPorWifi("192.168.1.200", 8000, Rascunho, ContadorDoRascunho);
                    int FimDaString;
                    char UltimoNumeroDoIp;
                    char RascunhoDoUltimoNumeroDoIp;
                    FimDaString = strlen(MeuIpWifi);
                    FimDaString = FimDaString - 1;
                    UltimoNumeroDoIp = MeuIpWifi[FimDaString];
                    if (UltimoNumeroDoIp == 2)RascunhoDoUltimoNumeroDoIp = 1;
                    else RascunhoDoUltimoNumeroDoIp = 2;
                    MeuIpWifi[FimDaString] = RascunhoDoUltimoNumeroDoIp;
                    enviaFluxoDeDadosPorWifi(MeuIpWifi, 8000, Rascunho, ContadorDoRascunho);
                    MeuIpWifi[FimDaString] = UltimoNumeroDoIp;
                    memset(Rascunho, 0, TAMANHO_DA_RESERVA_LOCAL);
                    ContadorDoRascunho = 0;
                }
                Estado = 0;
                break;
            default:
                Estado = 0;
                break;
        }
    }
    
    trataRecepcaoDeDadosDeEmpilhadeiraPorWifi();
    LimpaEResetaARecepcaoDeDadosDoWifi();
#endif     
}
*/


#define TAMANHO_DA_RESERVA_DE_EVENTO 2000
char ReservaDeEventos[TAMANHO_DA_RESERVA_DE_EVENTO];
int ContadorDaReservaDeEventos;

void repassaAsMensagensDeMovimentoDosPortaisParaOSars(void) {
    /*int Contador;
    if (TimeOutEthPortal >= TIMEOUT_ETH_PORTAIS) {
        if (ContadorDaReservaDeEventos > 0) {
            //(void)memcpy(BufferDeSaida, ReservaEthPortais, ContadorReservaEthPortais);
            //enviaFluxoDeDadosParaUartWifi(ReservaEthPortais, ContadorReservaEthPortais);
            //enviaFluxoDeDadosPorWifi("192.168.1.200", 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
            enviaFluxoDeDadosPorWifi("10.159.158.10", 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
            for (Contador = 0; Contador < ContadorDaReservaDeEventos; Contador = Contador + 1) {
                trataRecepcaoDeDadosDeZigBee(ReservaDeEventos[Contador]);
            }
            //resetNaReservaDeDadosDaEth();
        }
    }*/
}

void coletaEventosDaEth(void){
    /*
    int Contador;
    if (TimeOutEthPortal >= TIMEOUT_ETH_PORTAIS){ 
        if (ContadorReservaEthPortais > 0){
            if (disponivelParaSerOAlpha() < 1){
                liga_led_gps();
                liga_buzzer();
                delay_ms(10);
                if (ContadorReservaEthPortais <= TAMANHO_DA_RESERVA_DE_EVENTO){
                    for (Contador = 0;Contador < ContadorReservaEthPortais;Contador = Contador + 1){
                        //lidaComComandoDOPC(ReservaEthPortais[Contador], 0);
                        lidaComComandoDOPC(ReservaEthPortais[Contador], 0);//Conferir se o parametro de interface esta correto
                    }
                    //memmove(ReservaDeEventos, ReservaEthPortais, ContadorReservaEthPortais);
                    //ContadorDaReservaDeEventos = ContadorReservaEthPortais;
                    memmove(&ReservaDeEventos[ContadorDaReservaDeEventos], ReservaEthPortais, ContadorReservaEthPortais);
                    ContadorDaReservaDeEventos = ContadorDaReservaDeEventos + ContadorReservaEthPortais;
                    //resetNaReservaDeDadosDaEth();
                    ContadorReservaEthPortais = 0;
                }
                desliga_led_gps();
                desliga_buzzer();
            }
            //resetNaReservaDeDadosDaEth();
        }
        resetNaReservaDeDadosDaEth();
    }
    */
}

void repasseDeEventoParaOSars(void){
    /*if (ContadorDaReservaDeEventos > 0){
        //enviaFluxoDeDadosPorWifi("192.168.1.200", 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
        enviaFluxoDeDadosPorWifi("10.159.158.10", 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
    }*/
}

void repasseDeEventoParaOOutroLeitorAlfa(void){
    if (ContadorDaReservaDeEventos > 0){
        /*
        extern char __attribute__((near)) MeuIpEth[TAMANHO_DA_STRING_DE_IP];
        int FimDaString;
        char UltimoNumeroDoIp;
        char RascunhoDoUltimoNumeroDoIp;
        
        //FimDaString = strlen(MeuIpWifi);
        FimDaString = strlen(MeuIpEth);
        FimDaString = FimDaString - 1;
        //UltimoNumeroDoIp = MeuIpWifi[FimDaString];
        UltimoNumeroDoIp = MeuIpEth[FimDaString];
        if (UltimoNumeroDoIp == '2')RascunhoDoUltimoNumeroDoIp = '1';
        else RascunhoDoUltimoNumeroDoIp = '2';
        //MeuIpWifi[FimDaString] = RascunhoDoUltimoNumeroDoIp;
        MeuIpEth[FimDaString] = RascunhoDoUltimoNumeroDoIp;
        //enviaFluxoDeDadosPorWifi(MeuIpWifi, 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
        //enviaFluxoDeDadosPorWifi(MeuIpWifi, 9000, ReservaDeEventos, ContadorDaReservaDeEventos);
        enviaFluxoDeDadosPorWifi(MeuIpEth, 8000, ReservaDeEventos, ContadorDaReservaDeEventos);
        //MeuIpWifi[FimDaString] = UltimoNumeroDoIp;
        MeuIpEth[FimDaString] = UltimoNumeroDoIp;
        */
        
        enviaDadosParaEthPortais(ReservaDeEventos, ContadorDaReservaDeEventos);
    }
}

void descartaEventoColetado(void){
    if (ContadorDaReservaDeEventos > 0){
        memset(ReservaDeEventos, 0, TAMANHO_DA_RESERVA_DE_EVENTO);
        ContadorDaReservaDeEventos = 0;
    }
}

int enviaTabelaDeExclusaoParaUmaEmpilhadeira(int Indice){
    /*extern const char TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[QUANTIA_MAXIMA_DE_IPS_CADASTRADOS][TAMANHO_MAXIMO_STRING_IP];
    //extern char __attribute__((far)) ReservaDeEnvioDaUartDeWifi[TAMANHO_DA_RESERVA];
    //extern int QuantiaDeDadosParaEnviarPorWifiPorInterrupcao;


    if (strlen(TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice]) > 0){
        atualizaTabelaDeExclusaoParaEnvioPorWifi();
        
        disparaOEnvioDeFLuxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice]
                , 9000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
        
        disparaOEnvioDeFLuxoDeDadosPorWifi((char *)TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[Indice]
                , 8000, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);


        delay_ms(5);
        enviaFluxoDeDadosPorPolling(ReservaDeEnvioDaUartDeWifi, QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
        //(void)trataOEnvioDaTabelaDeExclusaoEmCodigoCorrente(QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
        //disparaOEnvioDaReservaDeDadosDoWifi(QuantiaDeDadosParaEnviarPorWifiPorInterrupcao);
        return 0;
    }
    return -1;
     */ 
    return 0;
}

int trataOEnvioDaTabelaDeExclusaoEmCodigoCorrente(int QuantiaDeDados){
    /*static int ContadorDeEnvio;
    static int QuantiaDeDadosHaEnviar;
    int ContadorDePassadasDeEnvio;
    extern char __attribute__((far)) ReservaDeEnvioDaUartDeWifi[TAMANHO_DA_RESERVA];
    
    if (QuantiaDeDados > 0){
        QuantiaDeDadosHaEnviar = QuantiaDeDados;
        return 0;
    } else {
        if (QuantiaDeDadosHaEnviar < 1){
            resetaELimpaAReservaDeEnvioPorWifi();
            ContadorDeEnvio = 0;
            QuantiaDeDadosHaEnviar = 0;
            return 0;
        }
        for (ContadorDePassadasDeEnvio = 0;ContadorDePassadasDeEnvio < 5;ContadorDePassadasDeEnvio = ContadorDePassadasDeEnvio + 1){
            if (ContadorDeEnvio < QuantiaDeDadosHaEnviar){
                enviaFluxoDeDadosPorPolling(&ReservaDeEnvioDaUartDeWifi[ContadorDeEnvio], 1);
                uart4Tx(ReservaDeEnvioDaUartDeWifi[ContadorDeEnvio]);
                ContadorDeEnvio =  ContadorDeEnvio + 1;
                return -1;
            } else {
                resetaELimpaAReservaDeEnvioPorWifi();
                ContadorDeEnvio = 0;
                QuantiaDeDadosHaEnviar = 0;
                return 0;
            }
        }
    }
    return -1;*/
    return 0;
}

void maquinaDeEnvioDeDadosPorWifi2(void) {
    /*lidaComATrocaDePortalAlpha();
    trataRecepcaoDeDadosDeZigBee(0);
    confereAConexaoDoWifi();
#ifdef PODE_SER_ALPHA    
    static int EstadoDeTarefa = 0;
    static int IndiceDeEnvioDeEmpilhadeira;
    
    extern const char TABELA_COM_OS_IPS_DAS_EMPILHADEIRAS[QUANTIA_MAXIMA_DE_IPS_CADASTRADOS][TAMANHO_MAXIMO_STRING_IP];
    extern char __attribute__((far)) ReservaDeEnvioDaUartDeWifi[TAMANHO_DA_RESERVA];
    
    //lidaComATrocaDePortalAlpha();

    
    coletaEventosDaEth();
    if (disponivelParaSerOAlpha() < 1){
        //coletaEventosDaEth();
        repasseDeEventoParaOSars();
        // repasseDeEventoParaOOutroLeitorAlfa();
    }
    descaraEventoColetado();
    
    
    //if (pacoteSendoEnviadoPelaUartDeWifi() == 0){
    //if (trataOEnvioDaTabelaDeExclusaoEmCodigoCorrente(0) == 0){

    
    coletaEventosDaEth();
    
    //trataRecepcaoDeDadosDeZigBee(0);
    
    if (disponivelParaSerOAlpha() < 1){
        
        //trataRecepcaoDeDadosDeEmpilhadeiraPorWifi();
        switch(EstadoDeTarefa){
            case 0:
                //enviaPerguntaDePresencaParaAsEmpilhadeirasNaArea();
                   
                while (enviaTabelaDeExclusaoParaUmaEmpilhadeira(IndiceDeEnvioDeEmpilhadeira) == -1){
                    IndiceDeEnvioDeEmpilhadeira = IndiceDeEnvioDeEmpilhadeira + 1;
                    if (IndiceDeEnvioDeEmpilhadeira < 0)IndiceDeEnvioDeEmpilhadeira = 0;
                    if (IndiceDeEnvioDeEmpilhadeira >= QUANTIA_MAXIMA_DE_IPS_CADASTRADOS)
                        IndiceDeEnvioDeEmpilhadeira = 0;
                    
                    //EstadoDeTarefa = 2;
                    //goto SAIDA;
                    
                    
                    if (IndiceDeEnvioDeEmpilhadeira == 0){
                        EstadoDeTarefa = 2;
                        //EstadoDeTarefa = 1;
                        goto SAIDA;
                    }
                }
                IndiceDeEnvioDeEmpilhadeira = IndiceDeEnvioDeEmpilhadeira + 1;
                if (IndiceDeEnvioDeEmpilhadeira < 0)IndiceDeEnvioDeEmpilhadeira = 0;
                if (IndiceDeEnvioDeEmpilhadeira >= QUANTIA_MAXIMA_DE_IPS_CADASTRADOS)
                    IndiceDeEnvioDeEmpilhadeira = 0;
                if (IndiceDeEnvioDeEmpilhadeira == 0){
                    EstadoDeTarefa = 2;
                }
SAIDA:                
                break;
                
            case 1:
                if (pacoteSendoEnviadoPelaUartDeWifi() == 0){
                    if (trataOEnvioDaTabelaDeExclusaoEmCodigoCorrente(0) == 0){
                        if (IndiceDeEnvioDeEmpilhadeira == 0)EstadoDeTarefa = 2;
                        else EstadoDeTarefa = 0;
                    }
                }

                break;
            case 2:
                //enviaParaTodosOsPortaisAPresencaDesseLeitor(-1);
                enviaParaTodosOsPortiasAPresencaDesseLeitorPorPolling();
                //EstadoDeTarefa = -1;
                EstadoDeTarefa = 3;
                break;
                
            case 3:
                repasseDeEventoParaOSars();
                descartaEventoColetado();
                //EstadoDeTarefa = 0;
                EstadoDeTarefa = 4;
                break;
            case 4:
                //repasseDeEventoParaOOutroLeitorAlfa();
                //descartaEventoColetado();
                trabalharNaBuscaDoMeuIpWifi();
                EstadoDeTarefa = 0;
                break;
            default:
                EstadoDeTarefa = -1;
                break;
        }
        //delay_ms(70);
        //EstadoDeTarefa = EstadoDeTarefa + 1;
        if (EstadoDeTarefa >= 4)EstadoDeTarefa = 0;
        if (EstadoDeTarefa < 0)EstadoDeTarefa = 0;
    }
#endif
     */  
}

//******************************************************************************
//************************** PROGRAMA PRINCIPAL ********************************
//******************************************************************************

int main(void){
    char num_serie_velho[20];
#ifdef PORTAL
    int contador;
#endif
    
    int Resultado;
    u32 baudrate, realrate;
    
    statusDeConexaoTCP = NOT_CONNECTED;

    CNPU1bits.CN15PUE = 1;
    CNPU2bits.CN16PUE = 1;
    CNPU2bits.CN19PUE = 1;
    CNPU4bits.CN49PUE = 1;
    CNPU4bits.CN56PUE = 1;
    CNPU4bits.CN57PUE = 1;
    CNPU2bits.CN28PUE = 1;
    
    desliga_saida_pa();

    ld_saidas(0, 0);

    systemInit();
    timerInit();
    platformInit();
    spiInit();
    
    baudrate = BAUDRATE;
    
    uart3TxInitialize(SYSCLK, baudrate, &realrate);// Porta USB

    uart2TxInitialize(SYSCLK, baudrate, &realrate);// Ethernet
   
    ligaTodosOsLeds();
    //delay_ms (1000);

    delay_ms(1);
    lista_freq_anatel();
    //Frequencies.freq[0] = 902750;
    Frequencies.freq[0] = 915000; //teste do ajuste automatico

    readerInitStatus = as3993Initialize(Frequencies.freq[0]);

    USBModuleDisable();
    initCommands(); // USB report commands
    
    desliga_led_rede();
    desliga_led_3g();
    desliga_led_gps();
    desliga_led_zig();
    desliga_led_tag();
    sel_led(0,0);
    
    tunerInit(&mainTuner);

    desliga_saida_pa();
    //inicia_display();

    //delay_ms(1000);

    //limpa_display();

    desliga_led_tag();
    
    ligaTimer2();
    uartRx1Initialize();
    uartRx2Initialize();
    uartRx3Initialize();
    uartRx4Initialize();
    
    IniciaRTC();
    
    inicializa_i2c3();
    
    atualizaHoraEData();
    
    num_serie[0] = 0x66;
    num_serie[1] = 0x66;
    num_serie[2] = 0x66;
    num_serie[3] = 0x66;
    
    ObtemID(num_serie);
    
    Resultado = 1;
    
    while ((num_serie[0] == 0x66) && (num_serie[1] == 0x66) &&
            (num_serie[2] == 0x66) && (num_serie[3] == 0x66) && Resultado != 0) {
        ObtemID(num_serie);
        memcpy(num_serie_velho, num_serie, 12);
        ObtemID(num_serie);
        Resultado = memcmp(num_serie, num_serie_velho, 12);
    }

    iniciaExclusao(); 
    
    desliga_buzzer();
    desliga_led_rede();
    desliga_led_3g();
    desliga_led_gps();
    
    iniciaCancelas();
    as3993SetSensitivity(125); //Ajuste normal
    
    iniciaPortalFrango();

    desligaTodosOsLeds();

    char mensagem[100];

    //obtemParametrosSalvosNaEeprom();
    sprintf(mensagem, "Obtendo Parametros Salvos na Memoria\r\n");
    
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    obtemParametrosDaMemoria();

    //exibe obtemParametrosSalvosNaEeprom();
    
    sprintf(mensagem, "Exibindo Parametros Obtidos...\r\n\r\n");
    
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    
    exibirParametrosObtidos();

    travaCancelaDoPortal();
    
    aguardoPrimeiraConexaoTCP();
    
    destravaCancelaDoPortal();

    inicializaMaquinaDeEstados_DataHora();
    habilitaMaquinaDeEstados_DataHora();

    atualizaHoraEData();

    //comecaInvetorio();
    Frequencia = frequenciaDeOperacao;
    SetaFrequencias();

//******************************************************************************
//**************************** LOOP PRINCIPAL **********************************
//******************************************************************************

    while (1) {

        operacoesParaRtcEmCodigoCorrente();
        
        acoesEmCodigoCorrentePortalFrango();

        checaNecessidadeDeTrocaDeIPRemoto();

        if (TempoParaDesligarBuzzer == 0) {
            if(alarmeFaltaDeRedeEthernet == 0){
                desliga_buzzer();
            }
        }

        asm("CLRWDT");

        const int TABELA_DE_ANTEAS[] = {
            0,1,3,2,4,1,3,2,4
        };
        int Antena;

        comecaInvetorio();

        for (antena_atual = 1; antena_atual <= numeroDeAntenasAtivas; antena_atual = antena_atual + 1) {

            sel_led(0, 0);
            Antena = TABELA_DE_ANTEAS[antena_atual];                  
            sel_led(Antena, 1);
            sel_antena(Antena);
            ajustaSintoniaEmFuncaoDaAntenaEmPortal(antena_atual);
            setaSensibilidade(sensibilidade);

            for (contador = 1; contador < (repeticaoNaLeitura + 1); contador = contador + 1) {
                sel_led(0, 0);

                //Antena = antena_atual;
                //Antena = TABELA_DE_ANTEAS[antena_atual];

                sel_led(Antena, 1);
                sel_antena(Antena);
                
                if (modoDeOperacao == OPERACAO_COM_MULTIPLAS_LEITURAS)
                    total_parcial = inventorioSimplificadoComPausa();
                if (modoDeOperacao == OPERACAO_LEITURAS_INITERRUPTAS)
                    total_parcial = inventorioSimplificado();
                if (modoDeOperacao == OPERACAO_COM_LEITURA_UNICA)
                    total_parcial = inventoryGen2();
                
                if (total_parcial) {
                    setaSinaleiro(SINALEIRO_AMARELO);
                    if(ContadorDeTempoParaManterCancelaDestravada == 0){
                        travaCancelaDoPortal();
                    }
                    
                    liga_led_tag();
                    
                    liga_buzzer();
                    
                    TempoParaDesligarBuzzer = TEMPO_PARA_DESLIGAR_BUZZER;

                    if (Antena & 0x01) {
                        PreTratamentoDeTagsParaOPortalFrango(1, Antena);
                    } else {
                        PreTratamentoDeTagsParaOPortalFrango(2, Antena);
                    }
                    
                    desliga_led_tag();
                    
                }
            }
            sel_led(0, 0);
            if (modoDeOperacao == OPERACAO_COM_LEITURA_UNICA)TerminaInvetorio();
            //TerminaInvetorio();
        }
        if (atrasoParaDegradarLeitura != 0) {
            delay_ms(atrasoParaDegradarLeitura);
        }
        //realizaBeepDeComandoFrango();
        TerminaInvetorio();
    }
}

void lista_freq_anatel(void) {

    Frequencies.freq[0] = 902750;
    Frequencies.freq[1] = 903250;
    Frequencies.freq[2] = 903750;
    Frequencies.freq[3] = 904250;
    Frequencies.freq[4] = 904750;
    Frequencies.freq[5] = 905250;
    Frequencies.freq[6] = 905750;
    Frequencies.freq[7] = 906250;
    Frequencies.freq[8] = 906750;
    Frequencies.freq[9] = 907250;
    Frequencies.freq[10] = 912000;
    Frequencies.freq[11] = 912500;
    ;
    Frequencies.freq[12] = 913000;
    Frequencies.freq[13] = 916250;
    Frequencies.freq[14] = 916750;
    Frequencies.freq[15] = 917250;
    Frequencies.freq[16] = 917750;
    Frequencies.freq[17] = 918250;
    Frequencies.freq[18] = 918750;
   
    Frequencies.numFreqs = 19;

    Frequencies.freq[0] = 902750; //Frequencia otima ate 26/12/18, vou mudar porque le 20 metros ou mais e

    Frequencies.numFreqs = 0;

}

void systemInit(void) {
#if RUN_ON_AS3994
    _RCDIV = 0; // FIXME: why is our clock to slow without divider?
#endif
}

u8 applPeripheralReset(void) {
    as3993Reset();
    return ERR_NONE;
}

u8 applProcessCmd(u8 protocol, u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData) {
    /*LOG("applProcessCmds(ptcl=%hhx, tx=%hhx, rx=%hhx)\n",
            rxData[0], rxSize, txSize);*/
    return commands(protocol, rxSize, rxData, txSize, txData);
}

u8 applProcessCyclic(u8 * protocol, u16 * txSize, u8 * txData, u16 remainingSize) {
    return sendTagData(protocol, txSize, txData, remainingSize);
}

const char * applFirmwareInformation() {
    return gAS3993FwInfo;
}

/*!This function reads one or all registers from the AS3993. See also applReadReg().\n
  The format of the report from the host is as follows:
  <table>
    <tr><th>   Byte</th><th>       0</th><th>       1</th></tr>
    <tr><th>Content</th><td>  mode  </td><td>reg_addr</td></tr>
  </table>
  Where mode = 0 requests to read all registers and mode = 1 requests to read only
  the register with address reg_addr. If mode = 0 reg_addr is ignored. \n
  If mode = 0 the device sends back:
  <table>
    <tr>
        <th>Byte</th>
        <th>0</th>
        <th>..</th>
        <th>29</th>
        <th>30</th>
        <th>31</th>
        <th>..</th>
        <th>36</th>
        <th>37</th>
        <th>38</th>
        <th>48</th>
    </tr>
    <tr>
        <th>Content</th>
        <td>reg 0x00</td>
        <td>..</td>
        <td>reg 0x1D</td>
        <td>reg 0x22</td>
        <td>reg 0x29</td>
        <td>..</td>
        <td>reg 0x2E</td>
        <td>reg 0x33</td>
        <td>reg 0x35</td>
        <td>..</td>
        <td>reg 0x3F</td>
    </tr>
  </table>
  If mode = 1 the device sends back:
  <table>
    <tr><th>   Byte</th><th>        0</th></tr>
    <tr><th>Content</th><td>reg value</td></tr>
  </table>
 
  returns ERR_NONE if operation was successful, ERR_PARAM is invalid mode was set.
 */
u8 cmdReadReg(u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData) {
    u8 status = ERR_PARAM;
    //LOG("cmdReadReg: cmd:%hhx, rxsize:%hhx, txsize:%hhx\n", rxData[0], rxSize, *txSize);
    if (rxSize < READ_REG_RX_SIZE)
        return status;
    switch (rxData[0]) {
        case 0: //read all regs
            status = readRegisters(txSize, txData);
            break;
        case 1: // read one reg
            status = readRegister(rxData[1], txSize, txData);
            break;
        default:
            status = ERR_PARAM;
    }
    return status;
}

u8 applReadReg(u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData) {
    return cmdReadReg(rxSize, rxData, txSize, txData);
}

/*!This function writes one register on the AS3993. See also applWriteReg(). \n
  The format of the report from the host is as follows:
  <table>
    <tr><th>   Byte</th><th>       0</th><th>    1</th></tr>
    <tr><th>Content</th><td>reg_addr</td><td>value</td></tr>
  </table>
  if reg_addr > 0x80, then an immediate command is executed.\n
  The device sends back:
  <table>
    <tr><th>   Byte</th><th>0</th></tr>
    <tr><th>Content</th><td>0</td></tr>
  </table>

  returns ERR_NONE if operation was successful.
 */
u8 cmdWriteReg(u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData) {
    //LOG("applWriteReg: addr:%hhx, val:%hhx, rxsize:%hhx, txsize:%hhx\n", rxData[0], rxData[1], rxSize, *txSize);
    if (rxSize < WRITE_REG_RX_SIZE)
        return ERR_PARAM;

    return writeRegister(rxData[0], rxData[1], txSize, txData);
}

u8 applWriteReg(u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData) {
    return cmdWriteReg(rxSize, rxData, txSize, txData);
}

#if RUN_ON_AS3994 || __PIC24FJ256GB110__    // check for CPU: fix nightly build

/* TODO: AS3994 does not support bootloader yet. */
void enableBootloader() {

}
#endif



void tick(void) {

    executaMaquinaDeEstados_DataHora();

    operacoesEmTickParaEth();
    //operacoesDeTickParaWifi();
    operacoesEmTickParaCancelas();

    //LED_A3(1);
    //sel_led(4,1);
    OperacoesParaRtcEmTick();
    
    trataTimeOutEthPortais();

    ContadorParaEnviarArrayXbee++;
    if (ContadorParaEnviarArrayXbee == 1000) {
        ContadorParaEnviarArrayXbee = 0;
        //envia_meu_operador();
        atualizaHoraEData();
        //logicaDeRemocaoDeTabelaDeExclusao();
    }

    ContadorParaRemocaoDeTabelaDeExclusao = ContadorParaRemocaoDeTabelaDeExclusao + 1;
    //if (ContadorParaRemocaoDeTabelaDeExclusao == 10000){

    if (ContadorParaRemocaoDeTabelaDeExclusao == 100) {
        ContadorParaRemocaoDeTabelaDeExclusao = 0;
        //logicaDeRemocaoDeTabelaDeExclusao();

    }
    
    //logicaDeRemocaoDeTabelaDeExclusao();

    //decrementaContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao();
    //LED_A3(0);

    //acoesEmTickParaPortalFrango();
    if (TempoParaDesligarBuzzer != 0) {
        TempoParaDesligarBuzzer = TempoParaDesligarBuzzer - 1;
    }

    ContadorParaUmCentessimo = ContadorParaUmCentessimo + 1;
    if (ContadorParaUmCentessimo > 100) {
        //sel_led(7,0);
        ContadorParaUmCentessimo = 0;
        acoesEmTickACadaSegundoParaPortalFrango();
        //operacoesEmTickParaEnvioDaTabelaDeExclusao();
        //operacoesParaTickParaGerenciaDePacotes();
        //sel_led(7,1);
    }
    
    ContadorParaUmSegundo = ContadorParaUmSegundo + 1;
    if (ContadorParaUmSegundo > 1000){
        ContadorParaUmSegundo = 0;
        enviaKeepAliveParaEthernet(1);
        contaIntervaloDeKeepAlive();
        contaIntevaloEntreTrocaDeRemoteIP();        
        //alertaSemConexaoComEthernet();
    }
    
    ContadorPara500ms = ContadorPara500ms + 1;
    if (ContadorPara500ms > 500){
        ContadorPara500ms = 0;
        setaStatusDoLedDeEthernet();
    }
        
    ContadorPara100ms = ContadorPara100ms + 1;
    if(ContadorPara100ms > 100){
        ContadorPara100ms = 0;
        CommandHandleModuloEthernet();
        commandHandlerPortaUSB();
    }

}