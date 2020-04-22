/*
 *****************************************************************************
 * Copyright by ams AG                                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, T
 * HE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************
 */
/** @file
  * @brief System initialization and main loop.
  *
  * @author Ulrich Herrmann
  * @author Bernhard Breinbauer
  */

/*
 * TODO: FERMI: use external oscillator, check configuration
 * TODO: feature request (bhi): save and loadable reader configuration (all reader settings)
 * NOTE: pic30-gcc (Gcc: 4.0.3; pic: 3.30) produces non-working (no epc reads) binary for AS3980 with -O2, changed to -O1
 */
//#include "uart.h"
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
#include "gprs.h"
//#include "varal.h"
#include "sankyu.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\onewire.h"
#include "lista.h"
#include "auto_teste.h"
#include "config_i2c3.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\mem_i2c_24LC256.h"
#include "tags.h"
#include "empilhadeira.h"
//#include "zigbee.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\zigbee.h"
#include "gps.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\rtc.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\exclusao.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\autoteste.h"


//#if (SYSCLK == SYSCLK_16MHZ)
#if (FEMTO2 || FEMTO2_1)
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV3 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM & SOSCSEL_IO)      // SOSCSEL_IO: get RA4 and RB4 as digital I/O
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

/*
#elif FERMI || RADON
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV & IESO_OFF)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM & SOSCSEL_IO)      // SOSCSEL_IO: get RA4 and RB4 as digital I/O
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)*/

//#elif (__PIC24FJ256GB110__) //MEGA
#elif (__PIC24FJ256DA210__)
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_ON & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV3 & IESO_OFF)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_LPSOSC & WUTSEL_LEG & ALTPMP_ALPMPDIS & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
/*
_CONFIG1( WDTPS_PS32768 & FWPSA_PR128 & ALTVREF_ALTVREDIS & WINDIS_OFF & FWDTEN_OFF & ICS_PGx2 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2( POSCMOD_HS & IOL1WAY_OFF & OSCIOFNC_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV2 & IESO_OFF)
_CONFIG3( WPFP_WPFP255 & SOSCSEL_SOSC & WUTSEL_LEG & ALTPMP_ALTPMPEN & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
*/
#endif

//u32 counter;
char num_serie[20];
char empilhadeira[20];


int dbm_tag_real;

extern void tick(void);
extern void ObtemID (char*);
extern Freq Frequencies;

extern Tag __attribute__((far)) tags_[MAXTAG];


int total_parcial = 0;

u8 cin,clen,cout = 0;

unsigned int total_tags = 0;

void lista_freq_anatel(void);

extern u8 inventoryGen2(void);


void ligaTimer2(void);
extern void uart4Tx(u8);

extern u16 readerInitStatus;
/** FW information which will be reported to host. */
static const char gAS3993FwInfo[] = FIRMWARE_ID"||"HARDWARE_ID;
/** FW information which will be logged on startup. Version information is included in logger. */
static const char gLogStartup[] = FIRMWARE_ID" %hhx.%hhx.%hhx on "HARDWARE_ID"\n";
/** FW version which will be reported to host */
const u32 firmwareNumber = FIRMWARE_VERSION;
//static u8 usedAntenna = 8;
int tem_ze_na_area;


void systemInit(void);

#if 0
static void calcBestSlotSize( void )
{
    u8 round;
    u8 powSlot;
    u8 numTags;

    for ( powSlot = 0; powSlot < 15 ; powSlot ++)
    {
        u8 maxTags=0;
        u16 tagsFound=0;
        for ( round = 0; round < 255 ; round ++)
        {
            numTags = gen2SearchForTags(tags_,16,0,0,powSlot,continueAlways,0,1);
            tagsFound+= numTags;
            if ( numTags > maxTags) maxTags = numTags;
        }
        LOG("found with powSlot=%hhx %x tags, max %hhx\n",
                                      powSlot,        tagsFound,maxTags);
    }
}
#endif



#ifdef TUNER
void mainSenCIN(int select)
{
    if (select) 
        SEN_TUNER_CIN(1);
    else
        SEN_TUNER_CIN(0);

}
void mainSenCLEN(int select)
{
    if (select) 
        SEN_TUNER_CLEN(1);
    else
        SEN_TUNER_CLEN(0);

}
void mainSenCOUT(int select)
{
    if (select) 
        SEN_TUNER_COUT(1);
    else
        SEN_TUNER_COUT(0);

}
#if RADON
void ant1SenCIN(int select)
{
    if (select) 
        SEN_ANT1_CINPIN(1);
    else
        SEN_ANT1_CINPIN(0);

}
void ant1SenCOUT(int select)
{
    if (select) 
        SEN_ANT1_COUTPIN(1);
    else
        SEN_ANT1_COUTPIN(0);

}
#endif

TunerConfiguration mainTuner = 
{
#if TUNER_CONFIG_CIN
    mainSenCIN,
#else
    NULL,
#endif
#if TUNER_CONFIG_CLEN
    mainSenCLEN,
#else
    NULL,
#endif
#if TUNER_CONFIG_COUT
    mainSenCOUT
#else
    NULL,
#endif
};

#if RADON
TunerConfiguration ant1Tuner = 
{
    ant1SenCIN,
    NULL,
    ant1SenCOUT
};
#endif
#endif

/** main function
 * Initializes board, cpu, reader, host communication, ...\n
 * After intialization a loop which waits for commands from host
 * and performs cyclic inventory rounds is entered.
 */


/*
void setaSensibilidade (unsigned char valor){
    signed char rascunho = 0;
    rascunho = rascunho - valor;
    as3993SetSensitivity(rascunho);
}
*/

int main(void){
    char num_serie_velho[20];
    int Resultado;
    CNPU1bits.CN15PUE = 1;

    CNPU2bits.CN16PUE = 1;
    CNPU2bits.CN19PUE = 1;
    CNPU4bits.CN49PUE = 1;
    CNPU4bits.CN56PUE = 1;
    CNPU4bits.CN57PUE = 1;
    CNPU2bits.CN28PUE = 1;
    //GP_9(1);
    //GP_10(1);
    desliga_saida_pa();
    //sel_cap_cin (0);
    //sel_cap_cout (0);
    //sel_cap_clen (0);
    ld_saidas (0,0);

    u32 baudrate, realrate;
            
    systemInit();
    timerInit();
    platformInit();
    spiInit();

    baudrate = BAUDRATE;
//#ifndef PORTAL
//serial    
    /*
     * //Teste Gilson 06/06/17
    uartTxInitialize(SYSCLK, baudrate, &realrate);
    uart2TxInitialize(SYSCLK, baudrate, &realrate);
    uart3TxInitialize(SYSCLK, baudrate, &realrate);
    baudrate = 9600;
    uart4TxInitialize(SYSCLK, baudrate, &realrate);
    */
    //Teste Gilson 06/06/17
    
    //uart4TxInitialize(SYSCLK, baudrate, &realrate);
    uart3TxInitialize(SYSCLK, baudrate, &realrate);
    uart2TxInitialize(SYSCLK, baudrate, &realrate);
    //baudrate = 9600;
    uartTxInitialize(SYSCLK, baudrate, &realrate);
    baudrate = 57600;
    uart4TxInitialize(SYSCLK, baudrate, &realrate);
    
//#endif

#if USE_UART_STREAM_DRIVER
    uartInitialize(SYSCLK, baudrate, &realrate);
#else
    //uartTxInitialize(SYSCLK, baudrate, &realrate);
#endif
    //LOG("\nHello World\n");
    //LOG(gLogStartup, (u8)((firmwareNumber >> 16) & 0xFF), (u8)((firmwareNumber >> 8) & 0xFF), (u8)(firmwareNumber & 0xFF));
    //marte
    sel_led (0,0);
   
    sel_led (0,1);
    liga_led_tag ();
    liga_saida_pa ();
    liga_dir();
    liga_led_zig();
    liga_led_wifi();
    liga_led_3g();
    liga_led_gps();
    liga_buzzer();

    delay_ms (1000);

    desliga_saida_pa ();
 
    desliga_buzzer();
    desliga_led_vm();
    desliga_sirene();

    desliga_led_zig();
    desliga_led_wifi();
    desliga_led_3g();
    desliga_led_gps();
    sel_led (0,0);
    desliga_led_tag ();
    desliga_dir();

    delay_ms(1);
    lista_freq_anatel();
    readerInitStatus = as3993Initialize(Frequencies.freq[0]);
    
    USBModuleDisable();
    initCommands(); // USB report commands



    
    //showError(readerInitStatus, ledBlinkState);
    //LOG("initialize state: %x\n", readerInitStatus);

#if ISO6B
    iso6bInitialize();
#endif
    
    tunerInit(&mainTuner);




    desliga_saida_pa();
    inicia_display ();

    //delay_ms(1000);

    limpa_display();

    desliga_led_tag ();
   


    
    //IFS0bits.AD1IF = 0;             // Limpa flag da interrupÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â§ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â£o
    //IEC0bits.AD1IE = 1;             // Liga a interrupÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â§ÃƒÆ’Ã†â€™Ãƒâ€ Ã¢â‚¬â„¢ÃƒÆ’Ã¢â‚¬Â ÃƒÂ¢Ã¢â€šÂ¬Ã¢â€žÂ¢ÃƒÆ’Ã†â€™ÃƒÂ¢Ã¢â€šÂ¬Ã…Â¡ÃƒÆ’Ã¢â‚¬Å¡Ãƒâ€šÃ‚Â£o

#define normal
    ligaTimer2();
    uartRxInitialize();
    //LigaRX(); //Gilson 18/09/17

    //LigaI2CRelogio();
    IniciaRTC();
    //IniciaMaquinaGPRS();
    //sprintf(empilhadeira, "EFOOO11");
    ///escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena){
    //escreverNoBufferDeFtp("ED000","EA0001", 0, 10, 0);
    //SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano)
    //SetarHoraRTC(0, 26, 14, 1, 8, 18);

    //testeI2C();
    
    //SetaTempoComEpoch(1515410257);
    
    atualizaHoraEData();
    //poe_texto_XY(4,0,stringData);
    //poe_texto_XY(4,12,stringHora);
    num_serie[0] = 0x66;
    num_serie[1] = 0x66;
    num_serie[2] = 0x66;
    num_serie[3] = 0x66;
    ObtemID (num_serie);
    Resultado = 1;
    while((num_serie[0] == 0x66) && (num_serie[1] == 0x66) && 
            (num_serie[2] == 0x66) && (num_serie[3] == 0x66) && Resultado != 0){
        ObtemID (num_serie);
        memcpy(num_serie_velho,num_serie, 12);
        ObtemID (num_serie);
        Resultado = memcmp(num_serie, num_serie_velho, 12);
    }
    

    delay_ms(1000);

    //int x,y,z,k;
    //escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena)
    

    
    
    //inicializaFila();
    //inicializaPilha();
    //inicializaLista();
    inicializa_i2c3();

    //iniciaLogicaDeEmpilhadeira();//Gilson, teste de novo cÃƒÆ’Ã‚Â³digo em 05/06/17
    //logicaDeEmpilhadeira();//Gilson, teste de novo cÃƒÆ’Ã‚Â³digo em 05/06/17
    
    
    //IniciaMaquinaGPRS();
    
    
    /*
    if((PRIMEIRA_VEZ_FIRMWARE != mem_le_24LC256(ADDR_CONFIG_DATA_RTC)) ||
       (strstr(stringHora,"00:00:")))       // verifica se eh a primeira vez que o hardware eh ligado
    {
        mem_escreve_24LC256(ADDR_CONFIG_DATA_RTC, PRIMEIRA_VEZ_FIRMWARE);   // se sim, registra na memoria serial externa
        //SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano)
        SetarHoraRTC(0,0,0,0,0,0);                                            // e inicializa o RTC
        
    }
    */

    //autoTesteLeitor();      // Faz um auto-teste dos submodulos do Leitor antes de iniciar a operacao
    
#ifdef CONTROL_PLUS_AUTOTESTE
    AutoTeste(); //Auto teste da placa 7.16 em fabrica
#endif
    
    limpa_display();
    
    iniciaLogicaDeEmpilhadeira();//Gilson, teste de novo cÃƒÆ’Ã‚Â³digo em 05/06/17
    //as3993SetSensitivity(80);
    as3993SetSensitivity(125);
    
    //SetarHoraRTC(5,10,2,5,15);                                            // e inicializa o RTC
    /*
    while(1){
        atualizaHoraEData();
        operacoesEmCodigoCorrenteParaZigBee();
        //testeI2C();
    }
    */
    //logicaDeEmpilhadeira();//Gilson, teste de novo cÃƒÆ’Ã‚Â³digo em 05/06/17
    //apagaTodosOsEventosArmazenasEmE2PROM();
#ifdef ARCELOR_JUIZ_DE_FORA_EMPILHADEIRA_TRADMAQ_3_5_TONS
    logicaDeEmpilhadeiraV2();
#endif
    
    while(1)                //LUCIANO: loop principal
    {


    }
    
}





/*
void auto_sintonia (void)
{
    static TunerParameters tunerParams = {15, 15, 15};
    TunerConfiguration *tuner = NULL;
    TunerParameters *params = NULL;
    tuner = &mainTuner;
    params = &tunerParams;
    as3993AntennaPower(1);
    tunerMultiHillClimb(tuner, params);
    as3993AntennaPower(0);
    tunerSetTuning(tuner, params->cin, params->clen, params->cout);
}
 */

//void antena_ok (unsigned char antena)
//{
    //powerUpReader();
    //as3993SetBaseFrequency(AS3993_REG_PLLMAIN1, freq);
    //as3993GetReflectedPower();
//}

void lista_freq_anatel(void)
{
    /*
    Frequencies.freq[0]= 902750;
    Frequencies.freq[1]= 903250;
    Frequencies.freq[2]= 903750;
    Frequencies.freq[3]= 904250;
    Frequencies.freq[4]= 904750;
    Frequencies.freq[5]= 905250;
    Frequencies.freq[6]= 905750;
    Frequencies.freq[7]= 906250;
    Frequencies.freq[8]= 906750;
    Frequencies.freq[9]= 907250;
    Frequencies.freq[10]= 907750;
    Frequencies.freq[11]= 914750;
    Frequencies.freq[12]= 915250;
    Frequencies.freq[13]= 915750;
    Frequencies.freq[14]= 916250;
    Frequencies.freq[15]= 916750;
    Frequencies.freq[16]= 917250;
    Frequencies.freq[17]= 917750;
    Frequencies.freq[18]= 918250;
    Frequencies.freq[19]= 918750;
    Frequencies.freq[20]= 919250;
    Frequencies.freq[21]= 919750;
    Frequencies.freq[22]= 920250;
    Frequencies.freq[23]= 920750;
    Frequencies.freq[24]= 921250;
    Frequencies.freq[25]= 921750;
    Frequencies.freq[26]= 922250;
    Frequencies.freq[27]= 922750;
    Frequencies.freq[28]= 923250;
    Frequencies.freq[29]= 923750;
    Frequencies.freq[30]= 924250;
    Frequencies.freq[31]= 924750;
    Frequencies.freq[32]= 925250;
    Frequencies.freq[33]= 925750;
    Frequencies.freq[34]= 926250;
    Frequencies.freq[35]= 926750;
    Frequencies.freq[36]= 927250;
    Frequencies.freq[37]= 927750;
    Frequencies.freq[38]= 928250;
    Frequencies.numFreqs=39;
    */


    Frequencies.freq[0]= 902750;
    Frequencies.freq[1]= 903250;
    Frequencies.freq[2]= 903750;
    Frequencies.freq[3]= 904250;
    Frequencies.freq[4]= 904750;
    Frequencies.freq[5]= 905250;
    Frequencies.freq[6]= 905750;
    Frequencies.freq[7]= 906250;
    Frequencies.freq[8]= 906750;
    Frequencies.freq[9]= 907250;
    Frequencies.freq[10]= 912000;
    Frequencies.freq[11]= 912500;;
    Frequencies.freq[12]= 913000;
    Frequencies.freq[13]= 916250;
    Frequencies.freq[14]= 916750;
    Frequencies.freq[15]= 917250;
    Frequencies.freq[16]= 917750;
    Frequencies.freq[17]= 918250;
    Frequencies.freq[18]= 918750;
    //Frequencies.numFreqs=0;
    Frequencies.numFreqs=19;  
     
    //Frequencies.numFreqs=19;
    
    
    /*
    Frequencies.freq[0]= 902000;
    Frequencies.freq[1]= 902500;
    Frequencies.freq[2]= 903000;
    Frequencies.freq[3]= 904000;
    */
    //Frequencies.freq[4]= 904500;
    //Frequencies.freq[5]= 905000;
    
    
    /*
    Frequencies.freq[0]= 921500;
    Frequencies.freq[1]= 921750;
    Frequencies.freq[2]= 922000;
    Frequencies.freq[3]= 922250;
    Frequencies.numFreqs=4;
    */
    
    /*
    Frequencies.freq[0]= 916500;
    Frequencies.freq[1]= 916750;
    Frequencies.freq[2]= 917000;
    Frequencies.freq[3]= 917250;
    Frequencies.numFreqs=4;
    */
    //Pixys Air??
    /*
    Frequencies.freq[0]= 916800;
    Frequencies.freq[1]= 917291;
    Frequencies.numFreqs=2;
*/
    
    //Frequencies.numFreqs=4;
    

    
    //Ajustes para Antenas da China
    /*
    Frequencies.freq[0]= 902000;
    Frequencies.freq[1]= 904000;
    Frequencies.numFreqs=2;
    */


    //Pixys Air??
    /*
    Frequencies.freq[0]= 921833;
    Frequencies.freq[1]= 922291;
    Frequencies.numFreqs=2;
    */
    
    Frequencies.freq[0]= 902750;//Frequencia otima ate 26/12/18, vou mudar porque le 20 metros ou mais e
// preciso que seja menos, entre 10 e 15 metros no maximo.
    //Frequencies.freq[0]= 910000;
    //Frequencies.freq[0]= 915000;
    //Frequencies.freq[0]= 920000;
    //Frequencies.freq[0]= 925000;
    //Frequencies.freq[0]= 930000;
    //Frequencies.freq[0]= 935000;
    //Frequencies.freq[0]= 935000;
    
    //Frequencies.freq[0]= 916800;
    //Frequencies.freq[0]= 917291;
    //Frequencies.freq[0]= 921833;//BOm para a Pixys Air
    //Frequencies.freq[0]= 922291;

    //Frequencies.freq[0]= 903800;

    Frequencies.numFreqs=0;
    
}




void systemInit(void)
{
#if RUN_ON_AS3994
    _RCDIV = 0;     // FIXME: why is our clock to slow without divider?
#endif
}


u8 applPeripheralReset(void)
{
    as3993Reset();
    return ERR_NONE;
}

u8 applProcessCmd( u8 protocol, u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData )
{
    /*LOG("applProcessCmds(ptcl=%hhx, tx=%hhx, rx=%hhx)\n",
            rxData[0], rxSize, txSize);*/
    return commands(protocol, rxSize, rxData, txSize, txData );
}

u8 applProcessCyclic( u8 * protocol, u16 * txSize, u8 * txData, u16 remainingSize )
{
    return sendTagData( protocol, txSize, txData, remainingSize );
}

const char * applFirmwareInformation()
{
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
u8 cmdReadReg(u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData)
{
    u8 status = ERR_PARAM;
    //LOG("cmdReadReg: cmd:%hhx, rxsize:%hhx, txsize:%hhx\n", rxData[0], rxSize, *txSize);
    if (rxSize < READ_REG_RX_SIZE)
        return status;
    switch (rxData[0])
    {
        case 0:     //read all regs
            status = readRegisters(txSize, txData);
            break;
        case 1:     // read one reg
            status = readRegister(rxData[1], txSize, txData);
            break;
        default:
            status = ERR_PARAM;
    }
    return status;
}

u8 applReadReg( u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData )
{
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
u8 cmdWriteReg (u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData)
{
    //LOG("applWriteReg: addr:%hhx, val:%hhx, rxsize:%hhx, txsize:%hhx\n", rxData[0], rxData[1], rxSize, *txSize);
    if (rxSize < WRITE_REG_RX_SIZE)
        return ERR_PARAM;

    return writeRegister(rxData[0], rxData[1], txSize, txData);
}

u8 applWriteReg( u16 rxSize, const u8 * rxData, u16 * txSize, u8 * txData )
{
    return cmdWriteReg(rxSize, rxData, txSize, txData);
}

#if RUN_ON_AS3994 || __PIC24FJ256GB110__    // check for CPU: fix nightly build
/* TODO: AS3994 does not support bootloader yet. */
void enableBootloader()
{

}
#endif



void tick (void)
{
    static int ContadorParaEnviarArrayXbee = 0;
    static int ContadorParaRemocaoDeTabelaDeExclusao  = 0;
    LED_A3(1);
    //sel_led(4,1);
    //OperacoesParaRtcEmTick();   
    operacoesEmTickParaEmpilhadeira();//Teste Gilson 06/06/17
    //OperacoesParaTickGprs();//Teste Gilson 31/08/17
    operacoesEmTickParaZigBee();//Teste Gilson 06/09/17
    operacacoesEmTickParaGps();//Teste Gilson 11/10/17
    //logicaDeRemocaoDeTabelaDeExclusao();
    //lidaComExpiracaoDoBufferDeRecepcaoDoZigBee();
    //sel_led(4,0);

    //OperacoesParaTickGprs();
    
    ContadorParaEnviarArrayXbee++;
    if (ContadorParaEnviarArrayXbee == 1000)
    {
        ContadorParaEnviarArrayXbee = 0;
        //envia_meu_operador();
        atualizaHoraEData();
        //logicaDeRemocaoDeTabelaDeExclusao();
    }
    
    ContadorParaRemocaoDeTabelaDeExclusao = ContadorParaRemocaoDeTabelaDeExclusao + 1;
    //if (ContadorParaRemocaoDeTabelaDeExclusao == 10000){
    
    if (ContadorParaRemocaoDeTabelaDeExclusao == 100){
        ContadorParaRemocaoDeTabelaDeExclusao = 0;
        //logicaDeRemocaoDeTabelaDeExclusao();
        
    }
    logicaDeRemocaoDeTabelaDeExclusao();
    
    decrementaContadorDeEstouroDeTempoDaRecepcaoDeDadosDaUartDaTabelaDeExclusao();
    LED_A3(0);

    /*
    static unsigned int countTesteAntenas = 0;
    static unsigned char aciona_flag = 0;
    
    //LUCIANO: Implementacao do auto-teste (22/11/2016))
    
    if(countTesteAntenas++ > 500)
    {
        countTesteAntenas = 0;          // Limpa o contador
        if((getResultadoAutoTeste()) && (!getResultadoTesteAntenas()))  // se nao ocorreu nenhum erro no autoteste, porrem ocorreu algum problema com as antenas ou entao o teste nao foi realizado.
        {
            aciona_flag = !aciona_flag; // muda estado da flag
            ld_saidas(3,aciona_flag);   // comanda o acionamento do buzzer conforme a flag 'aciona_flag'
            sel_led(8,aciona_flag);     // comanda o acionamento do LED 8
        }
    }
    
    //Fim da implementacao do auto-teste
    */
}