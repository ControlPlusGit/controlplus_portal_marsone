#include <stdio.h>
#include <string.h>
#include "eth.h"
//#include "lcd.h"
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
//#include "sankyu.h"
#include "onewire.h"
//#include "lista.h"
#include "config_i2c3.h"
#include "mem_i2c_24LC256.h"
#include "tags.h"
#include "empilhadeira.h"
#include "rtc.h"
#include "exclusao.h"
#include "portal.h"
#include "global.h"
#include "cancelas.h"
#include "as3993.h"
#include "ajuste_cap.h"
#include "setup_usb.h"
#include "i2c.h"
//#include "barreiraIR.h"

#include "FSM_DataHora.h"
#include "barreiraIR.h"

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

extern unsigned char statusDeOperacaoDoLeitorRFID;

#define TAMANHO_DA_RESERVA_DE_EVENTO 2000
char ReservaDeEventos[TAMANHO_DA_RESERVA_DE_EVENTO];
int ContadorDaReservaDeEventos;

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



void descartaEventoColetado(void){
    if (ContadorDaReservaDeEventos > 0){
        memset(ReservaDeEventos, 0, TAMANHO_DA_RESERVA_DE_EVENTO);
        ContadorDaReservaDeEventos = 0;
    }
}


void InicializaAS3993(void){ 
    TerminaInvetorio();
    delay_ms(500);
    //Frequencies.freq[0] = 902750;
    Frequencies.freq[0] = 915000; //teste do ajuste automatico
    readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    
    
    tunerInit(&mainTuner);
    Frequencia = frequenciaDeOperacao;
    SetaFrequencias();
    as3993SetSensitivity(sensibilidade); //Ajuste normal
    
}



//******************************************************************************
//************************** PROGRAMA PRINCIPAL ********************************
//******************************************************************************

int main(void){
    //char num_serie_velho[20];
    int contador;
    //int Resultado;
    
    u32 baudrate, realrate;
    
    CNPU1bits.CN15PUE = 1;
    CNPU2bits.CN16PUE = 1;
    CNPU2bits.CN19PUE = 1;
    CNPU4bits.CN49PUE = 1;
    CNPU4bits.CN56PUE = 1;
    CNPU4bits.CN57PUE = 1;
    CNPU2bits.CN28PUE = 1;
    
    statusDeConexaoTCP = NOT_CONNECTED;
    
    desliga_saida_pa();
    ld_saidas(0, 0);

    //systemInit();
    timerInit();
    platformInit();
    spiInit();
    
    INTCON1bits.NSTDIS = 1; //habilita o aninhamento de interrupcoes
    
    baudrate = BAUDRATE;
    
    uart3TxInitialize(SYSCLK, baudrate, &realrate);// Porta USB

    uart2TxInitialize(SYSCLK, baudrate, &realrate);// Ethernet
    
    uartTxInitialize(SYSCLK, baudrate, &realrate); // wifi 
    
    uart4TxInitialize(SYSCLK, baudrate, &realrate);
   
    ligaTodosOsLeds();
    delay_ms (1000);

    delay_ms(1);
    lista_freq_anatel();
    Frequencies.freq[0] = 915000;
    readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    
    USBModuleDisable();
    initCommands(); // USB report commands
    
    delay_ms(1000);
    
    desliga_led_rede();
    desliga_led_3g();
    desliga_led_gps();
    desliga_led_zig();
    desliga_led_tag();
    desliga_saida_pa();
    desliga_buzzer();
    sel_led(0,0);
    
    tunerInit(&mainTuner);

    ligaTimer2();
    uartRx1Initialize();
    uartRx2Initialize();
    uartRx3Initialize();
    uartRx4Initialize();
    
    IniciaRTC();
    inicializa_i2c3();
    
//    atualizaHoraEData();
    
//    num_serie[0] = 0x66;
//    num_serie[1] = 0x66;
//    num_serie[2] = 0x66;
//    num_serie[3] = 0x66;
//    
//    ObtemID(num_serie);
    
//    Resultado = 1;
    
//    while ((num_serie[0] == 0x66) && (num_serie[1] == 0x66) &&
//            (num_serie[2] == 0x66) && (num_serie[3] == 0x66) && Resultado != 0) {
//        ObtemID(num_serie);
//        memcpy(num_serie_velho, num_serie, 12);
//        ObtemID(num_serie);
//        Resultado = memcmp(num_serie, num_serie_velho, 12);
//    }

         
    iniciaExclusao();
    iniciaCancelas();    
    iniciaPortalFrango();
    desligaTodosOsLeds();

    char mensagem[100];

    sprintf(mensagem, "Obtendo Parametros Salvos na Memoria\r\n");
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    obtemParametrosDaMemoria();
    
    sprintf(mensagem, "Exibindo Parametros Obtidos...\r\n\r\n");
    enviaDadosParaUSBserial(mensagem, strlen(mensagem));
    exibirParametrosObtidos();
    
    num_serie[0] = idDoLeitor[0];
    num_serie[1] = idDoLeitor[1];
    num_serie[2] = idDoLeitor[2];
    num_serie[3] = idDoLeitor[3];    

    travaCancelaDoPortal();
    aguardoPrimeiraConexaoTCP(); 
    destravaCancelaDoPortal();
    InitSensorIR();

    inicializaMaquinaDeEstados_DataHora();
    habilitaMaquinaDeEstados_DataHora();

    atualizaHoraEData();

    if(readerInitStatus){
        Frequencies.freq[0] = 915000;
        readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    }
    
    Frequencia = frequenciaDeOperacao;
    SetaFrequencias();
    as3993SetSensitivity(sensibilidade); //Ajuste normal
    
    comecaInvetorio();
    

//******************************************************************************
//**************************** LOOP PRINCIPAL **********************************
//******************************************************************************

    while (1) {
        
        operacoesParaRtcEmCodigoCorrente();
        acoesEmCodigoCorrentePortalFrango();
        checaNecessidadeDeTrocaDeIPRemoto();
        MonitoraPassagemValidaPeloPortal();

        if (TempoParaDesligarBuzzer == 0) {
            if(alarmeFaltaDeRedeEthernet == 0){
                desliga_buzzer();
            }
        }

        asm("CLRWDT");

        const int TABELA_DE_ANTEAS[] = {
            0,1,2,3,4,1,2,3,4
        };
        int Antena;
        
        if (ContadorParaUmSegundo > 1000){
            ContadorParaUmSegundo = 0;
            enviaKeepAliveParaEthernet(statusDeOperacaoDoLeitorRFID);
            contaIntervaloDeKeepAlive();
            contaIntevaloEntreTrocaDeRemoteIP(); 
        }
        
        //comecaInvetorio();

        for(antena_atual = 1; antena_atual <= numeroDeAntenasAtivas; antena_atual = antena_atual + 1){

            sel_led(0, 0);
            Antena = TABELA_DE_ANTEAS[antena_atual];                  
            sel_led(Antena, 1);
            sel_antena(Antena);
            ajustaSintoniaEmFuncaoDaAntenaEmPortal(antena_atual);
            setaSensibilidade(sensibilidade);

            for (contador = 1; contador < (repeticaoNaLeitura + 1); contador = contador + 1) {
                //if(modoDeOperacao == OPERACAO_COM_MULTIPLAS_LEITURAS) total_parcial = inventorioSimplificadoComPausa();
                //if(modoDeOperacao == OPERACAO_LEITURAS_INITERRUPTAS) total_parcial = inventorioSimplificado();
                //if(modoDeOperacao == OPERACAO_COM_LEITURA_UNICA) total_parcial = inventoryGen2();
                
                //_T2IE = 0;
                total_parcial = inventoryGen2();
                //_T2IE = 1;
                
                if(total_parcial){
                    setaSinaleiro(SINALEIRO_AMARELO);
                    setaPedestreNaAntenaRFID();
                    if(ContadorDeTempoParaManterCancelaDestravada == 0){
                        travaCancelaDoPortal();
                    }
                    liga_led_tag();
                    liga_buzzer();
                    TempoParaDesligarBuzzer = TEMPO_PARA_DESLIGAR_BUZZER;

                    if(Antena & 0x01){
                        PreTratamentoDeTagsParaOPortalFrango(1, Antena);
                    }else{
                        PreTratamentoDeTagsParaOPortalFrango(2, Antena);
                    }
                    desliga_led_tag();             
                }
            }
            sel_led(0, 0);
            //if (modoDeOperacao == OPERACAO_COM_LEITURA_UNICA) TerminaInvetorio();
        }
        if(atrasoParaDegradarLeitura != 0){
            delay_ms(atrasoParaDegradarLeitura);
        }
        //TerminaInvetorio();
        
        desliga_saida_pa();
        delay_ms(50);
        liga_saida_pa();
        
    }
}

void lista_freq_anatel(void){

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
    Frequencies.freq[0] = 902750; //Frequencia otima ate 26/12/18, vou mudar porque l e 20 metros ou mais e
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
    LerSensorIR();

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
//    if (ContadorParaUmSegundo > 1000){
//        ContadorParaUmSegundo = 0;
//        enviaKeepAliveParaEthernet(1);
//        contaIntervaloDeKeepAlive();
//        contaIntevaloEntreTrocaDeRemoteIP();        
//        //alertaSemConexaoComEthernet();
//    }
    
    ContadorPara500ms = ContadorPara500ms + 1;
    if (ContadorPara500ms > 500){
        ContadorPara500ms = 0;
        setaStatusDoLedDeEthernet();
//        if(statusDeOperacaoDoLeitorRFID == STATUS_NORMAL && statusDeConexaoTCP == NOT_CONNECTED){
//            _LATB3 = !_LATB3; // SINALEIRO VERMELHO
//        }
    }
        
    ContadorPara100ms = ContadorPara100ms + 1;
    if(ContadorPara100ms > 100){
        ContadorPara100ms = 0;
        CommandHandleModuloEthernet();
        commandHandlerPortaUSB();
    }

}