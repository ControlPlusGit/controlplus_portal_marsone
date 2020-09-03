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
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
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

#include "lcd.h"
#include "as3993_config.h"
#include "platform.h"
#include "stream_dispatcher.h"
#include "usb_hid_stream_driver.h"
#include "logger.h"
#include "uart_driver.h"
#include "errno.h"
#include "as3993_public.h"
#include "as3993.h"
#include "gen2.h"
#include "global.h"
#include "timer.h"
#include "appl_commands.h"
#include "tuner.h"
#include "AD.h"


//#define TEMPO_PARA_NOVO_ENVIO 1000
//#define TEMPO_POR_SER_OBSTRUIDO 100

//#define TAMANHO_ARRAY_XBEE 20
//volatile char arrayXbee [TAMANHO_ARRAY_XBEE];
//volatile char operador_atual_outro = 0;
volatile int operador_atual_outro = 0;

volatile int contadorParaEnviarArrayXbee;
volatile char FlagObstrucaoXbee;
volatile int contadorRX;
volatile int contadorTimeOutRX;
//volatile char arrayXbeeRecebido [TAMANHO_ARRAY_XBEE];




extern Freq Frequencies;
extern Tag tags_[MAXTAG];
int st_sai2, st_sai3, st_sai4, st_sai5, st_sai6, st_cin, st_cout, st_ld_pa;
int st_led1, st_led2, st_led2, st_led3, st_led4, st_led5, st_led6, st_led7, st_led8;
int st_clen, st_ant_ab, st_ant_a14, st_ant_b58, at_ant_a12, st_ant_a34, st_ant_b56, st_ant_b78;
unsigned int qt_leituras[8] = {0};
unsigned int qt_tags[8]= {0};
unsigned int total_tags = 0;
int atualiza_display = 0;
int antena_atual;
#define  QT_MAX_TAGS 31
//unsigned char dbm_tag_e[QT_MAX_TAGS];
unsigned char tag_atual_e[QT_MAX_TAGS];
unsigned int dbm_tag[QT_MAX_TAGS] = {0};
unsigned char tag_atual_d[QT_MAX_TAGS] = {0};
unsigned int acg_tag_atual[QT_MAX_TAGS]  = {0};
unsigned int conta_tag_frente[QT_MAX_TAGS] = {0};
unsigned int conta_tag_traseira [QT_MAX_TAGS]= {0};
//unsigned char tags_frontais[QT_MAX_TAGS];
unsigned int conta_leituras_dbm[QT_MAX_TAGS] = {0};
//unsigned int pneus[QT_MAX_TAGS] = {0};
//unsigned int tags_entrando[QT_MAX_TAGS] = {0};
//unsigned int tags_saindo[QT_MAX_TAGS] = {0};
//unsigned int tags_saiu[QT_MAX_TAGS] = {0};
//unsigned int tags_entrou[QT_MAX_TAGS] = {0};
long int tensao_an6 = 0;
const int qt_leituras_ad = 200;
int contador_int_ad = 200;
int situacao = 1;
int situacao_anterior = 0;
int conta_tags_esquerda = 0;
int conta_tags_direita = 0;
int conta_leituras_antena_e = 0;
int conta_leituras_antena_d = 0;
int conta_leituras_operador;
char tres_espacos[]= "   ";
char vinte_espacos[]= "                    ";
long int leitura_ad_anterior = 0;
int diferenca_leituras = 0;
const int QT_SOMA_RSSI = 10;
int conta_tags_frontal = 0;
int coluna = 5;
int conta_mostra3 = 0;
int  operador_atual = 0;
//int qt_tags_total = 0;
//int pt_tags_total = 0;
int conta_f_anterior = 0;
int conta_t_anterior = 0;
//int deslocados = 0;
//int retirados = 1;
//int pt_deslocamento = 0;
int atualiza_tags = 1;
int status_display = 0;
int status_display_anterior = 1;
int conta_tempo_buzzer = 0;
int conta_tags = 0;
unsigned int media_dbm = 0;
int media_dbm_ze = 0;
int media_dbm_tunel = 0;
int operarios = 0;
int conta_mostra = 0;
int conta_mostra2 = 0;
int parar_leitura = 0;
int espera_usb = 0;
#define QT_TROCA_ST 10
int conta_troca_status = QT_TROCA_ST;
//char frente[] ="Frente:";
//char traseira[] = "Traseira:";
char pedestre[] = "pedestres";
char livre[] = "Livre";
char parado[] = "Parado";
char area_livre[] = "Area livre";
char desabilitado[] = "Desabilitado";
char autorizado[] = "Autorizado:";
char gabriel[] = "Gabriel";
char pedro[] = "Pedro";
int transmitindo;
//char entrando[] = "Entra:";
//char saindo[] = "Sai:";
int conta_giro_frente = 21;
int conta_giro_traseira = 21;
int tem_ze = 0;
int conta_zero_ze = 0;
int linha;
int contador;
int pode_operar = 0;
int pode_operar_anterior = 1;
int muda_display = 0;
int todos_a_frente;
int zona_direita;
int zona_esquerda;
int pedestre_esquerda;
int pedestre_direita;

void mostra_rssi_tags(void);
void situacao_mudou(void);
void lista_tags_frontais(void);
void mostra_area_display(void);
void retira_tags_repetidas(void);
void ordena_lista_tags(void);
void liga_buzzer(void);
void desliga_buzzer(void);
void liga_rele1(void);
void liga_rele2(void);
void desliga_rele1(void);
void desliga_rele2(void);
void poe_sentido(void);
void atualiza_display_ambev(void);
void trata_parado(void);
void parado_linha1(void);
void parado_linha2(void);
void poe_dado_tag_display(int);
void escolhe_antena(void);
void pega_dados_tags (void);
void analise_lateral(void);
void lista_tags (unsigned char, unsigned char,unsigned char,unsigned char, char *);
void atualiza_relacao_pneus(void);
void  acerta_antena_anatel (void);
void acerta_freq_anatel(void);
void lista_freq_anatel(void);
void limpa_buffer_tags (void);
void desliga_saida_4 (void);

extern u8 inventoryGen2(void);
void sel_led (int,int);
extern u16 pega_pot_refl (void);
void antena_de_1a4 (void);

//#define TEM_RASP

#if (SYSCLK == SYSCLK_16MHZ)
#if (FEMTO2 || FEMTO2_1 || PLACA_TECBAN)
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV3 & IESO_ON)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM & SOSCSEL_IO)      // SOSCSEL_IO: get RA4 and RB4 as digital I/O
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

#elif FERMI || RADON
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV & IESO_OFF)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM & SOSCSEL_IO)      // SOSCSEL_IO: get RA4 and RB4 as digital I/O
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_SOSC & DSBOREN_OFF & DSWDTEN_OFF)

#elif (__PIC24FJ256GB110__) //MEGA
_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & COE_OFF & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_NONE & DISUVREG_OFF & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL_96MHZ_ON & PLLDIV_NODIV & IESO_OFF)
_CONFIG3(WPFP_WPFP0 & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)

#endif

#endif

extern Tag tags_[MAXTAG];
extern u16 readerInitStatus;
/** FW information which will be reported to host. */
static const char gAS3993FwInfo[] = FIRMWARE_ID"||"HARDWARE_ID;
/** FW information which will be logged on startup. Version information is included in logger. */
static const char gLogStartup[] = FIRMWARE_ID" %hhx.%hhx.%hhx on "HARDWARE_ID"\n";
/** FW version which will be reported to host */
const u32 firmwareNumber = FIRMWARE_VERSION;
//static u8 usedAntenna = 8;

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

// morse codes for hex numbers
/*
static const u8 codes[0x10] = 
{
    // upper 3 bits length, lower 5 bits shorts/longs
    0xbf, // 0 -----
    0xaf, // 1 .---
    0xa7, // 2 ..---
    0xa3, // 3 ...--
    0xa1, // 4 ....-
    0xa0, // 5 .....
    0xb0, // 6 -....
    0xb8, // 7 --...
    0xbc, // 8 ---..
    0xbe, // 9 ----.
    0x41, // a .-
    0x88, // b -...
    0x8a, // c -.-.
    0x64, // d -..
    0x20, // e .
    0x82, // f ..-.
};
*/

/** This function will morse an error code on either the #MCULED() or on #LED1()*/
/*
static void morse_nibble( u8 nibble )
{
    u8 i, cod, code_len;

    if (nibble > 0xf) nibble = 0x89;  -..- = "x" 

    cod = codes[nibble];
    code_len = cod >> 5;

    for ( i = code_len; i>0; i-- )
    {
        if (NUMLEDS == 0)       // if there are no status LEDs use the mcu led for morsing
            MCULED(LEDON);
        else
            LED1(LEDON);
        if ( cod & (1<<(i-1)) ) 
        {
            LOG("-");
            delay_ms(450);
        }
        else
        {
            LOG(".");
            delay_ms(150);
        }
        if (NUMLEDS == 0)
            MCULED(LEDOFF);
        else
            LED1(LEDOFF);
        delay_ms(150);
    }
    LOG(" %hhx\n", nibble);
    delay_ms(450);
}
*/

/**
 * This function displays error codes. If the number of LEDs available on the
 * board is sufficient, those LEDs will be used to show the error code binary
 * coded. If there are too few LEDs on the board the error code will be morsed
 * via morse_nibble();
 * @param errorCode The error code to display.
 * @param blinkState Indicates if the LEDs should be turned on or off (for blinking).
 */
/*
static void showError( u8 errorCode, u8 blinkState )
{
    if (errorCode == 0)
    {
        MCULED(blinkState);
        return;
    }

    if (NUMLEDS < 2)
    {   // if we do not have enough LEDs to show error codes, morse them on LED1
        morse_nibble(errorCode);
        delay_ms(1000);
    }
    else
    {
        MCULED(errorCode & 0x01 ? blinkState : LEDOFF);
        LED1(errorCode & 0x02 ? blinkState : LEDOFF);
        LED2(errorCode & 0x04 ? blinkState : LEDOFF);
        LED3(errorCode & 0x08 ? blinkState : LEDOFF);
    }
}
*/

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

void sel_antena (int antx)
{
    //if (situacao == 1)tunerInit_A1(&mainTuner);
    //if (situacao == 2)tunerInit_A3(&mainTuner);
    //if (situacao == 3)tunerInit_A1(&mainTuner);
     //if (antx > 8)return;
    sel_led (antx,1);
   /*
    LAT_LEDA1_ANTAB_S2(1);
    st_ant_ab = 1;
    LAT_LEDA2_AA14_S3(1);
    st_ant_a14 = 1;
    LAT_LEDA3_AB58_S4(1);
    st_ant_b58 = 1;
    LAT_LEDA4_AA12_RL1(1);
    at_ant_a12 = 1;
    LAT_LEDA5_AA34_RL2(1);
    st_ant_a34 = 1;
    LAT_LEDA6_AB56_LPA(1);
    st_ant_b56 = 1;
    LAT_LEDA7_AB78_CAP1(1);
    st_ant_b78 = 1;

    SAIDA_SEL_AX(1);
    delay_us(10);
    SAIDA_SEL_AX(0);
    delay_us(10);
    */
    LAT_LEDA8_S1_CAP2_CAP3(st_clen);    //preserva o clen
    //
    switch (antx)
    {
            case 1:
                LAT_LEDA1_ANTAB_S2(1);  //sel. grupo 1-4
                st_ant_ab = 1;
                LAT_LEDA2_AA14_S3(1);   //sel. 1-2
                st_ant_a14 = 1;
                LAT_LEDA4_AA12_RL1(1);  //sel. A1
                at_ant_a12 = 1;
                //tunerInit_A1(&mainTuner);
            break;
            case 2:
                LAT_LEDA1_ANTAB_S2(1);  //sel. grupo 1-4
                st_ant_ab = 1;
                LAT_LEDA2_AA14_S3(1);   //sel. 1-2
                st_ant_a14 = 1;
                LAT_LEDA4_AA12_RL1(0);  //sel. A2
                at_ant_a12 = 0;
                //tunerInit_A2(&mainTuner);
            break;
            case 3:
                LAT_LEDA1_ANTAB_S2(1);  //sel. grupo 1-4
                st_ant_ab = 1;
                LAT_LEDA2_AA14_S3(0);   //sel. 3-4
                st_ant_a14 = 0;
                LAT_LEDA5_AA34_RL2(1);
                st_ant_a34 = 1;
                //tunerInit_A3(&mainTuner);
            break;
            case 4:
                LAT_LEDA1_ANTAB_S2(1);  //sel. grupo 1-4
                st_ant_ab = 1;
                LAT_LEDA2_AA14_S3(0);   //sel. 3-4
                st_ant_a14 = 0;
                LAT_LEDA5_AA34_RL2(0);
                st_ant_a34 = 0;
                //tunerInit_A4(&mainTuner);
            break;
            case 5:
                LAT_LEDA1_ANTAB_S2(0);  //sel. grupo 5-8
                st_ant_ab = 0;
                LAT_LEDA3_AB58_S4(1);   //sel. 5-6
                st_ant_b58 = 1;
                LAT_LEDA6_AB56_LPA(1);  //sel. A5
                st_ant_b56 = 1;
            break;
            case 6:
                LAT_LEDA1_ANTAB_S2(0);  //sel. grupo 5-8
                st_ant_ab = 0;
                LAT_LEDA3_AB58_S4(1);   //sel. 5-6
                st_ant_b58 = 1;
                LAT_LEDA6_AB56_LPA(0);  //sel. A6
                st_ant_b56 = 0;
            break;
            case 7:
                LAT_LEDA1_ANTAB_S2(0);  //sel. grupo 5-8
                st_ant_ab = 0;
                LAT_LEDA3_AB58_S4(0);   //sel. 6-8
                st_ant_b58 = 0;
                LAT_LEDA7_AB78_CAP1(1);  //sel. A7
                 st_ant_b78 = 1;
            break;
            case 8:
                LAT_LEDA1_ANTAB_S2(0);  //sel. grupo 5-8
                st_ant_ab = 0;
                LAT_LEDA3_AB58_S4(0);   //sel. 6-8
                st_ant_b58 = 0;
                LAT_LEDA7_AB78_CAP1(0);  //sel. A8
                st_ant_b78 = 0;
            break;
    }
    SAIDA_SEL_AX(1);
    delay_us(10);
    SAIDA_SEL_AX(0);
}

void sel_cap_cin(int x)
{
    LAT_LEDA1_ANTAB_S2(st_sai2);
    LAT_LEDA2_AA14_S3(st_sai3);
    LAT_LEDA3_AB58_S4(st_sai4);
    LAT_LEDA4_AA12_RL1(st_sai5);
    LAT_LEDA5_AA34_RL2(st_sai6);
    LAT_LEDA6_AB56_LPA(st_ld_pa);
    //
    LAT_LEDA8_S1_CAP2_CAP3(0); //desativa Cout
    st_cout = 0;
    //
    LAT_LEDA7_AB78_CAP1(x);     //Ativa/desativa Cin
    st_cin = x;
    SAIDA_SEL_OUT1(1);
    delay_us(10);
    SAIDA_SEL_OUT1(0);
}
void sel_cap_cout(int x)
{
    LAT_LEDA1_ANTAB_S2(st_sai2);
    LAT_LEDA2_AA14_S3(st_sai3);
    LAT_LEDA3_AB58_S4(st_sai4);
    LAT_LEDA4_AA12_RL1(st_sai5);
    LAT_LEDA5_AA34_RL2(st_sai6);
    LAT_LEDA6_AB56_LPA(st_ld_pa);
    //
    LAT_LEDA7_AB78_CAP1(0);     //desativa Cin
    st_cin = 0;
    //
    LAT_LEDA8_S1_CAP2_CAP3(x);     //Ativa/desativa Cout
    SAIDA_SEL_OUT1(1);
    delay_us(10);
    SAIDA_SEL_OUT1(0);
}
void sel_cap_clen(int x)
{
    sel_cap_cin(0);     //desliga Cin e Cout
    sel_cap_cout(0);
    //
    LAT_LEDA1_ANTAB_S2(st_ant_ab);
    LAT_LEDA2_AA14_S3(st_ant_a14);
    LAT_LEDA3_AB58_S4(st_ant_b58);
    LAT_LEDA4_AA12_RL1(at_ant_a12);
    LAT_LEDA5_AA34_RL2(st_ant_a34);
    LAT_LEDA6_AB56_LPA(st_ant_b56);
    LAT_LEDA7_AB78_CAP1(st_ant_b78);
    //

    LAT_LEDA8_S1_CAP2_CAP3(x);     //Ativa/desativa Clen
    st_clen = x;
    SAIDA_SEL_AX(1);
    delay_us(10);
    SAIDA_SEL_AX(0);
}

void sel_led (int led, int status)
{
    LAT_LEDA1_ANTAB_S2(st_led1);
    LAT_LEDA2_AA14_S3(st_led2);
    LAT_LEDA3_AB58_S4(st_led3);
    LAT_LEDA4_AA12_RL1(st_led4);
    LAT_LEDA5_AA34_RL2(st_led5);
    LAT_LEDA6_AB56_LPA(st_led6);
    LAT_LEDA7_AB78_CAP1(st_led7);
    LAT_LEDA8_S1_CAP2_CAP3(st_led8);
    switch (led)
    {
            case 0:     //led = 0 = poe estatus em todos
                st_led1 = status;
                st_led2 = status;
                st_led2 = status;
                st_led3 = status;
                st_led4 = status;
                st_led5 = status;
                st_led6 = status;
                st_led7 = status;
                st_led8 = status;
                LAT_LEDA1_ANTAB_S2(status);
                LAT_LEDA2_AA14_S3(status);
                LAT_LEDA3_AB58_S4(status);
                LAT_LEDA4_AA12_RL1(status);
                LAT_LEDA5_AA34_RL2(status);
                LAT_LEDA6_AB56_LPA(status);
                LAT_LEDA7_AB78_CAP1(status);
                LAT_LEDA8_S1_CAP2_CAP3(status);
                break;
            case 8:
                LAT_LEDA1_ANTAB_S2(status);
                 st_led1 = status;
                break;
            case 7:
                LAT_LEDA2_AA14_S3(status);
                 st_led2 = status;
                break;
            case 6:
                LAT_LEDA3_AB58_S4(status);
                 st_led3 = status;
                break;
            case 5:
                LAT_LEDA4_AA12_RL1(status);
                 st_led4 = status;
                break;
            case 4:
                LAT_LEDA5_AA34_RL2(status);
                 st_led5 = status;
                break;
            case 3:
                LAT_LEDA6_AB56_LPA(status);
                 st_led6 = status;
                break;
            case 2:
                LAT_LEDA7_AB78_CAP1(status);
                 st_led7 = status;
                break;
            case 1:
                LAT_LEDA8_S1_CAP2_CAP3(status);
                 st_led8 = status;
                break;
    }
SAIDA_S_LEDS(1);
delay_us(10);
SAIDA_S_LEDS(0);
}
void liga_saida_pa (void)
{
    ld_pa (1);
}
void desliga_saida_pa (void)
{
    ld_pa (0);
}

void ld_pa (int status )
{
    LAT_LEDA1_ANTAB_S2(st_sai2);
    LAT_LEDA2_AA14_S3(st_sai3);
    LAT_LEDA3_AB58_S4(st_sai4);
    LAT_LEDA4_AA12_RL1(st_sai5);
    LAT_LEDA5_AA34_RL2(st_sai6);
    LAT_LEDA7_AB78_CAP1(st_cin);
    LAT_LEDA8_S1_CAP2_CAP3(st_cout);
    //
    LAT_LEDA6_AB56_LPA(status);
    st_ld_pa = status;
    SAIDA_SEL_OUT1(1);
    delay_us(10);
    SAIDA_SEL_OUT1(0);
}

void ld_saidas (int saida, int status)
{
    LAT_LEDA1_ANTAB_S2(st_sai2);
    LAT_LEDA2_AA14_S3(st_sai3);
    LAT_LEDA3_AB58_S4(st_sai4);
    LAT_LEDA4_AA12_RL1(st_sai5);
    LAT_LEDA5_AA34_RL2(st_sai6);
    LAT_LEDA7_AB78_CAP1(st_cin);
    LAT_LEDA8_S1_CAP2_CAP3(st_cout);
    LAT_LEDA6_AB56_LPA(st_ld_pa);

    switch (saida)
    {
        case 0:
            LAT_LEDA1_ANTAB_S2(status);
            LAT_LEDA2_AA14_S3(status);
            LAT_LEDA3_AB58_S4(status);
            LAT_LEDA4_AA12_RL1(status);
            LAT_LEDA5_AA34_RL2(status);
            st_sai2 = status;
            st_sai3 = status;
            st_sai4 = status;
            st_sai5 = status;
            st_sai6 = status;
            break;
        case 2:
            LAT_LEDA1_ANTAB_S2(status);
            st_sai2 = status;
            break;
        case 3:
            LAT_LEDA2_AA14_S3(status);
            st_sai3 = status;
            break;
        case 4:
            LAT_LEDA3_AB58_S4(status);
            st_sai4 = status;
            break;
        case 5:
            LAT_LEDA4_AA12_RL1(status);
            st_sai5 = status;
            break;
        case 6:
            LAT_LEDA5_AA34_RL2(status);
            st_sai6 = status;
            break;
    }
    SAIDA_SEL_OUT1(1);
    delay_us(10);
    SAIDA_SEL_OUT1(0);
}

void liga_rele1(void)
{
    ld_saidas (5,1);
}
void liga_rele2(void)
{
    ld_saidas (6,1);
}
void desliga_rele1(void)
{
    ld_saidas (5,0);
}
void desliga_rele2(void)
{
    ld_saidas (6,0);
}
void liga_saida_4 (void)
{
    ld_saidas (4,1);
}
void desliga_saida_4 (void)
{
    ld_saidas (4,0);
}
void liga_buzzer(void)
{
    ld_saidas (3,1);
}

void desliga_buzzer(void)
{
    ld_saidas (3,0);
}

void limpa_buffer_tags (void)
{
    int x;
    for (x = 0; x < QT_MAX_TAGS; x++)
        {
            tags_[x].epc[11] = 0;
        }
}
void limpa_e_troca_antena(void)
{
        limpa_buffer_tags();
        sel_led (0,0);
        sel_antena (antena_atual);
}

#ifdef DEMO
void poe_dado_tag_display(int contador)
{
    poe_dado_XY (linha,coluna, contador);
    //conta_leituras_dbm[contador]+= tag_atual_d[contador];
    coluna += 2;
    poe_texto_XY(linha,coluna,":");
    coluna++;
    poe_dado_XY (linha,coluna,conta_leituras_dbm[contador]);
}
#endif
/** main function
 * Initializes board, cpu, reader, host communication, ...\n
 * After intialization a loop which waits for commands from host
 * and performs cyclic inventory rounds is entered.
 */
#ifdef AMBEV

/*
void antena_de_1a4 (void)
{
    antena_atual++;
    if (antena_atual > 4)antena_atual = 1;
    sel_led (0,0);
    sel_antena(antena_atual);
}
*/
#endif
int main(void)
{
    //unsigned int total_tags_anterior = 0;
    desliga_saida_4();
    desliga_rele1();
    desliga_rele2();
    desliga_buzzer();

    SAIDA_ELCD(0);
    SAIDA_RS(0);
    desliga_saida_pa();
    sel_cap_cin (0);
    sel_cap_cout (0);
    sel_cap_clen (0);
    ld_saidas (0,0);
    
    u32 counter = 0;
    //BOOL ledBlinkState = 0;
    u32 baudrate, realrate;
    //liga_buzzer();
    //liga_rele1();
    //liga_rele2();
            
    systemInit();
    timerInit();
    platformInit();
    spiInit();

    

    //MCULED(LEDOFF);
    //MCULED(LEDON);
    desliga_saida_pa();
    //LED1(LEDOFF);
    //LED2(LEDOFF);
    //LED3(LEDOFF);
    //LED4(LEDOFF);

    baudrate = BAUDRATE;
    uartTxInitialize(SYSCLK, baudrate, &realrate);

#if USE_UART_STREAM_DRIVER
    uartInitialize(SYSCLK, baudrate, &realrate);
#else
    //uartTxInitialize(SYSCLK, baudrate, &realrate);
#endif
    //LOG("\nHello World\n");
    //LOG(gLogStartup, (u8)((firmwareNumber >> 16) & 0xFF), (u8)((firmwareNumber >> 8) & 0xFF), (u8)(firmwareNumber & 0xFF));
    //marte
    sel_led (0,0);
    //desliga_rele1();
    //delay_ms (1000);
    //ld_saidas (6,1);
    sel_led (0,1);
    ld_saidas (2, 1);
    liga_saida_pa ();
    SAIDA_DIR(1);
    delay_ms (1000);
    //desliga_rele2();
    desliga_saida_pa ();
    sel_led (0,0);
    ld_saidas (2, 0);
    SAIDA_DIR(0);
    //desliga_buzzer();
    //sel_antena (2);
    //
    // Fill frequencies interval used in usb_commands.c with values
    // European channels since we are developping in Europe, values can be changed using GUI
    /*
    Frequencies.freq[0]= 866900;
    Frequencies.freq[1]= 865700;
    Frequencies.freq[2]= 866300;
    Frequencies.freq[3]= 867500;
    Frequencies.numFreqs=4;
    */
  

  
   /*
    Frequencies.freq[0]= 913750;
    Frequencies.freq[1]= 914250;
    Frequencies.freq[2]= 914750;
    Frequencies.freq[3]= 915250;
    Frequencies.freq[4]= 915750;
    Frequencies.freq[5]= 916250;
    Frequencies.freq[6]= 916750;
    Frequencies.freq[7]= 917250;
    Frequencies.freq[8]= 917750;
    Frequencies.freq[9]= 918250;
    Frequencies.freq[10]= 918750;
    Frequencies.freq[11]= 919250;
    Frequencies.freq[12]= 919750;
    Frequencies.freq[13]= 920250;
    Frequencies.freq[14]= 920750;
    Frequencies.numFreqs=15;
    */
#if DEMO || PLACA_TECBAN || AJUSTA_ANTENA
    Frequencies.freq[0]= 915750;
    Frequencies.numFreqs=1;
#endif

    IFS0bits.AD1IF = 0;             // Limpa flag da interrupção
    IEC0bits.AD1IE = 1;             // Liga a interrupção
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    inicializa_AD();
#ifdef ANATEL
    antena_atual = 1;
    sel_antena (antena_atual);
    sel_led (0,0);
    while (contador_int_ad != 0);
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    while (contador_int_ad != 0);
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    while (contador_int_ad != 0);
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    while (contador_int_ad != 0);
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    acerta_freq_anatel ();
#endif
     
#ifdef AMBEV
    Frequencies.freq[0]= 915000;
    Frequencies.numFreqs=1;
#endif
    
    delay_ms(1);
    readerInitStatus = as3993Initialize(Frequencies.freq[0]);

    initCommands(); /* USB report commands */
#ifdef TUNER
    tunerInit(&mainTuner);
#ifdef TECBAN
    tunerInit_tec_ban(&mainTuner);
#endif
#if RADON
    tunerInit(&ant1Tuner);
#endif
#endif
    
    //showError(readerInitStatus, ledBlinkState);

    //LOG("initialize state: %x\n", readerInitStatus);

#if ISO6B
    iso6bInitialize();
#endif

    StreamDispatcherInit(SYSCLK);
#if !USE_UART_STREAM_DRIVER
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    USBDeviceAttach();
#endif
    //MCULED(LEDOFF);
    //MCULED(LEDON);
    desliga_saida_pa();
    //auto_sintonia ();
    inicia_display ();
    //minha_inicia_display ();
    delay_ms(1000);
    limpa_display();

#ifndef ANATEL
    sel_led (0,0);
    antena_atual = 1;
    sel_antena (antena_atual);
    situacao = 1;
#endif
    //antena_atual = 1;
    //sel_antena (antena_atual);
    ld_saidas (2, 0);
    int conta_giro = 0;
    int x;
    int y;
    int conta_zero_tags = 0;
    int tags_atual = 0;
#ifndef AMBEV
    int conta_tags_presentes;
    int conta_tags_atuais;
    int total_tags_momento;
    int conta_tags_atuais_momento;
#endif
    int total_parcial = 0;
    

#ifdef DEMO
    poe_texto_XY (1,0,"Control-Up Reader:");
#endif

#ifdef TECBAN
    poe_texto_XY (1,0,"TecBan / Control-Up");
    poe_texto_XY (2,0,"Monitor de K7:");
    unsigned char gaveta1[] = "G1:";
    unsigned char gaveta2[] = "G2:";
    unsigned char gaveta3[] = "G3:";
    unsigned char gaveta4[] = "G4:";
    unsigned char cinco_espacos[] = "     ";
    int  tag_g1,tag_g2,tag_g3,tag_g4 = 0;
    poe_texto_XY (3,0, gaveta1);
    poe_texto_XY (3,10, gaveta2);
    poe_texto_XY (4,0, gaveta3);
    poe_texto_XY (4,10, gaveta4);
#endif
    //poe_sentido();
    //escolhe_antena();

#ifdef AMBEV
    //inicializa_AD();
    poe_sentido();
    escolhe_antena();
    if (situacao == 3)poe_texto_XY (1,7, parado);
    if (situacao == 1)poe_texto_XY (1,7, area_livre);
    if (situacao == 2)poe_texto_XY (1,7, area_livre);
    
#endif

#ifdef ANATEL
    //inicializa_AD();
    poe_texto_XY (1,0,"1:");
    
#endif
    
    //IFS0bits.AD1IF = 0;             // Limpa flag da interrupção
    //IEC0bits.AD1IE = 1;             // Liga a interrupção
#ifdef PLACA_TECBAN
    antena_atual = 0;
    u8 k7_ocupados = 0;
#endif

#ifndef ANATEL
    sel_led (0,0);
    antena_atual = 1;
    sel_antena (antena_atual);
#endif
#define normal

    ligaTimer2();
    uartRxInitialize();
    while (1)
  {
#ifdef AJUSTA_ANTENA
        //sel_led (0,0);
        //sel_antena (antena_atual);
        ProcessIO(); // main trigger for operation commands.

        doCyclicInventory();      // do cyclic inventory if necessary



        if (total_tags)
        {
            ld_saidas (2, 1);
            //y = tags_[0].epc[11];
        }
        else
        {
            ld_saidas (2, 0);
            //y = 0;
        }
#endif
#ifdef ANATEL
        total_tags = 0;
        x = 0;
        //acerta_freq_anatel ();
        /*
        acerta_antena_anatel ();
        if (espera_usb == 1)
        {
        ProcessIO(); // main trigger for operation commands.

        doCyclicInventory();      // do cyclic inventory if necessary
        }
         */
        //if (parar_leitura != 1)
        //{
            //while((x < 5) && (total_tags == 0))
            //{
                total_tags = inventoryGen2();
                //x++;
            //}
        //}
        //x = 0;
     
        if (total_tags)
        {
            ld_saidas (2, 1);
            y = tags_[0].epc[11];
        }
        else
        {
            ld_saidas (2, 0);
            y = 0;
        }
        if (antena_atual == 1)poe_dado_XY (1,2,y);
        if (contador_int_ad == 0)
        {
            if (tensao_an6 >= 200)tensao_an6 = tensao_an6/qt_leituras_ad;
            else tensao_an6 = 0;
            leitura_ad_anterior = tensao_an6;
            poe_texto_XY (2,0,"     ");
            poe_dado_XY (2,0,leitura_ad_anterior);
            leitura_ad_anterior = tensao_an6 = 0;
            contador_int_ad = qt_leituras_ad;
        }
        
#endif

#ifdef TECBAN
        total_tags = 0;
        x = 0;
        
        antena_atual++;
        if (antena_atual > 4)antena_atual = 1;
        sel_antena (antena_atual);
        //ProcessIO(); // main trigger for operation commands.

        //doCyclicInventory();      // do cyclic inventory if necessary
        
        while(x < 30)
        {
            total_tags = inventoryGen2();
            if (total_tags)break;
            x++;
        }
        
        x = 0;
       
        if (total_tags)y = tags_[0].epc[11];
        else y = 0;
        poe_texto_XY (2,14,cinco_espacos);
        //poe_dado_XY (2,14,y);
        //poe_texto_XY (2,16,"-");
        k7_ocupados = 0;
        if (tag_g1)k7_ocupados++;
        if (tag_g2)k7_ocupados++;
        if (tag_g3)k7_ocupados++;
        if (tag_g4)k7_ocupados++;
        poe_dado_XY (2,14,k7_ocupados);
        if (antena_atual == 1)
        {
            //if (tag_g1 != y)
            //{
            poe_texto_XY (3,3, cinco_espacos);
            tag_g1 = y;
            if (y != 0)poe_dado_XY (3,3,y);
            //}
        }
        if (antena_atual == 2)
        {
            //if (tag_g2 != y)
            //{
            poe_texto_XY (3,13, cinco_espacos);
            tag_g2 = y;
            if (y != 0)poe_dado_XY (3,13,y);
            //}
        }
        if (antena_atual == 3)
        {
            //if (tag_g3 != y)
            //{
            poe_texto_XY (4,3, cinco_espacos);
            tag_g3 = y;
            if (y != 0)poe_dado_XY (4,3,y);
            //}
        }
        if (antena_atual == 4)
        {
            //if (tag_g4 != y)
            //{
            poe_texto_XY (4,13, cinco_espacos);
            tag_g4 = y;
            if (y != 0)poe_dado_XY (4,13,y);
            //}
        }
#endif

#ifdef DEMO
    total_tags = 0;
    atualiza_tags = 1;
        if (counter)
            {
                counter--;
            }
            else
            {
                counter = (10000ULL<<(readerInitStatus?3:0));
                if (readerInitStatus)
                { // If MCU is powered before AS3993 has power it is best to try to init again
                    readerInitStatus = as3993Initialize(Frequencies.freq[0]);
                }
            }

            //ProcessIO(); // main trigger for operation commands.

            //doCyclicInventory();      // do cyclic inventory if necessary
    conta_tags_atuais = 0;
    conta_tags_atuais_momento = 0;
    total_tags_momento = 0;
    for (x = 0; x < QT_MAX_TAGS; x++ )
    {
        tag_atual_d[x] = 0;
    }
    for (x = 0; x < 20; x++)
    {
        ProcessIO(); // main trigger for operation commands.

        doCyclicInventory();      // do cyclic inventory if necessary
        //total_tags_momento = inventoryGen2();
        total_tags_momento = total_tags;

            for (x = 0; x < QT_MAX_TAGS; x++ )
            {
                if (tag_atual_d[x])
                {

                    y = tag_atual_d[x];
                    conta_leituras_dbm[y]++;
                    conta_tags_atuais_momento++;
                }

            }
        if (conta_tags_atuais_momento > conta_tags_atuais)conta_tags_atuais = conta_tags_atuais_momento;
    }

        if (total_tags)
        {
            linha = 2;
            coluna = 0;
            conta_zero_tags = 0;
            conta_tags_presentes = 0;
            ld_saidas (2, 1);
            x = y = 0;
            //limpa_display();
            poe_texto_XY (2,0, vinte_espacos);
            poe_texto_XY (3,0, vinte_espacos);
            poe_texto_XY (4,0, vinte_espacos);
           
            poe_texto_XY (1,18,"  ");
            poe_dado_XY (1,18,conta_tags_atuais);
            
            for (x = 0;x < QT_MAX_TAGS; x++)
            {
                if (conta_leituras_dbm[x])
                {
                    conta_tags_presentes++;
                    switch (conta_tags_presentes)
                    {
                        case 2:
                            coluna = 10;
                            break;
                        case 3:
                            coluna = 0;
                            linha = 3;
                            break;
                        case 4:
                            coluna = 10;
                            break;
                        case 5:
                            coluna = 0;
                            linha = 4;
                            break;
                        case 6:
                            coluna = 10;
                            break;
                    }
                    if (conta_tags_presentes <= 6)poe_dado_tag_display(x);
                }
            }
            //liga_buzzer();
        }
        else
        {
            ld_saidas (2, 0);
            poe_texto_XY (1,18,"  ");
            poe_dado_XY (1,18,total_tags);
            //desliga_buzzer();
        }
#endif

#ifdef AMBEV
        total_tags = 0;
        tags_atual = 0;
        //conta_giro = 8;
        atualiza_tags = 1;
        limpa_buffer_tags ();
        //poe_sentido();
        //escolhe_antena();
        
        for(conta_giro = 4;conta_giro > 0;conta_giro--)
        {
            
            /*if (counter)
            {
                counter--;
            }
            else
            {
                counter = (10000ULL<<(readerInitStatus?3:0));
                if (readerInitStatus)
                { // If MCU is powered before AS3993 has power it is best to try to init again
                    readerInitStatus = as3993Initialize(Frequencies.freq[0]);
                }
            }
            */
            //poe_sentido();
            //escolhe_antena();
            //ProcessIO(); // main trigger for operation commands.
            //doCyclicInventory();      // do cyclic inventory if necessary
   
            total_parcial = inventoryGen2();
            tags_atual += total_parcial;
            if (total_parcial)ld_saidas (2, 1);
            else ld_saidas (2, 0);

            if (situacao != 3)
            {
                if (situacao == 1)
                {
                    if (conta_giro == 4)
                    {
                        pega_dados_tags ();
                        antena_atual = 1;
                        limpa_e_troca_antena();
                        //limpa_buffer_tags();
                        //sel_led (0,0);
                        //sel_antena (antena_atual);
                    }
                    if (conta_giro == 2)
                    {
                        pega_dados_tags ();
                        antena_atual = 2;
                        limpa_e_troca_antena();
                        //limpa_buffer_tags();
                        //sel_led (0,0);
                        //sel_antena (antena_atual);
                    }
                }
                if (situacao == 2)
                {
                    if (conta_giro == 4)
                    {
                        pega_dados_tags ();
                        antena_atual = 3;
                        limpa_e_troca_antena();
                        //limpa_buffer_tags();
                        //sel_led (0,0);
                        //sel_antena (antena_atual);
                    }
                    if (conta_giro == 2)
                    {
                        pega_dados_tags ();
                        antena_atual = 4;
                        limpa_e_troca_antena();
                        //limpa_buffer_tags();
                        //sel_led (0,0);
                        //sel_antena (antena_atual);
                    }
                }
                
            }
            //if (total_tags)break;
            //conta_giro--;
        }
        total_tags = tags_atual;
        //conta_giro = 8;
        if (total_tags)
        {
#ifndef TEM_RASP
            pega_dados_tags ();
#endif
            conta_zero_tags = 0;
            //ld_saidas (2, 1);
            //liga_buzzer();
        }
        else
        {
            //ld_saidas (2, 0);
            if (situacao == 3) conta_zero_tags++;
            //desliga_buzzer();
        }

        if ((situacao == 1) || (situacao == 2))
        {
            if((conta_leituras_antena_e > 10) || (conta_leituras_antena_d > 10))
            {
                analise_lateral();
                conta_leituras_antena_e = 0;
                conta_leituras_antena_d = 0;
                mostra_rssi_tags();
                conta_zero_tags = 0;
            }
            else if (total_tags == 0)
            {
                conta_zero_tags++;
                conta_zero_ze = 0;
                tem_ze = 0;
                if (conta_zero_tags > 20)
                {
                    conta_zero_tags = 0;
                    if (status_display != 4)
                    {
                    desliga_buzzer();
                    //desliga_bip();
                    status_display = 4;
                    atualiza_display_ambev();
                    }
                }
            }
        }
        if (situacao == 3)
        {
            desliga_rele1();
            desliga_rele2();
            desliga_buzzer();
            desliga_saida_4();
            if ((conta_leituras_operador > 10) || (conta_zero_tags > 10))
            {
                conta_zero_tags = 0;
                conta_leituras_operador = 0;
                if (tags_[0].epc[11] == 9) pode_operar = 1;
                else if (tags_[0].epc[11] == 8) pode_operar = 1;
                else
                {
                    pode_operar = 0;
                    operador_atual = 0;
                }

                if ((pode_operar != pode_operar_anterior) || (operador_atual != tags_[0].epc[11] ))
                {
                    pode_operar_anterior = pode_operar;
                    limpa_display();
                    poe_texto_XY (1,7, parado);
                    if(!pode_operar)poe_texto_XY (2,4,desabilitado);
                    else
                    {
                        y = tags_[0].epc[11];
                        poe_texto_XY (2,0,autorizado);
                        if (tags_[0].epc[11] == 9)poe_texto_XY (2,11,gabriel);
                        if (tags_[0].epc[11] == 8)poe_texto_XY (2,11,pedro);
                        operador_atual = tags_[0].epc[11];
                       
                    }
                }
                tags_[0].epc[11] = 0;
                //dbm_tag[operador_atual] = 0;
                //conta_leituras_dbm[operador_atual] = 0;
                //acg_tag_atual[operador_atual] = 0;
            }
            else if (muda_display)
            {
                limpa_display();
                poe_texto_XY (1,7, parado);
                if(!pode_operar)poe_texto_XY (2,4,desabilitado);
                else
                    {
                        poe_texto_XY (2,0,autorizado);
                        if (operador_atual == 9)poe_texto_XY (2,11,gabriel);
                        if (operador_atual == 8)poe_texto_XY (2,11,pedro);
                    }
                muda_display = 0;
            }
        }
        poe_sentido();
        escolhe_antena();
        atualiza_tags = 1;
    #endif
    }
}

#ifdef AMBEV

/*
void lista_tags (unsigned char linha, unsigned char coluna,unsigned char limite,unsigned char qt_tags, char *tags)
{
    if (qt_tags == 0)return;
    int x;
    int y;
    for (x=0; x < QT_MAX_TAGS ;x++)
    {
        y = tags[x];
        if (y)
        {
            poe_dado_XY(linha,coluna,y);
            coluna += 3;
            if (coluna > 18)
            {
                coluna = 0;
                linha++;
            }
            if (linha > limite) break;
        }
        tags[x] = 0;
    }
}
void atualiza_relacao_pneus(void)
{
    int x;
    int y;
    //total_tags = 0;
    for (x=0; x < QT_MAX_TAGS ;x++)
    {      
        if (tags_[x].epc[11])
        {
            y = tags_[x].epc[11];
            pneus[y] = tags_[x].epc[11];
            //total_tags++;
        }
    }
}
*/
void mostra_rssi_tags(void)
{
    int x = 0;
    int conta_leituras_ze = 0;
    conta_tags = 0;
    media_dbm = 0;
    //int coluna = 0;
    media_dbm_ze = 0;
    media_dbm_tunel = 0;
    int operarios_atual = 0;
    conta_mostra++;

    for (x = 0; x < QT_MAX_TAGS; x++)
    {
        if (dbm_tag[x])
        {
            if (x == 1)
            {
                if (conta_leituras_dbm[1])
                {
                    media_dbm_ze = dbm_tag[1]/conta_leituras_dbm[1];
                    conta_leituras_ze = conta_leituras_dbm[1];
                    tem_ze = 1;
                    conta_zero_ze = 0;
                }
            }
            else if (media_dbm_ze)
            {
                if (conta_leituras_dbm[x] > conta_leituras_ze )
                {
                    media_dbm = dbm_tag[x]/conta_leituras_dbm[x];
                    if (media_dbm <= media_dbm_ze )operarios_atual++;
                    else conta_tags++;
                }
                else operarios_atual++;
            }
            else
            {
                if ((conta_zero_ze < 30) && (tem_ze))conta_zero_ze++;
                else
                {
                    media_dbm_ze = 0;
                    tem_ze = 0;
                    conta_tags++;
                }
            }
            
            dbm_tag[x] = 0;
            conta_leituras_dbm[x] = 0;
            acg_tag_atual[x] = 0;
        }
    }
    
    if (conta_tags)
    {
        conta_troca_status = 0;
        //liga_buzzer();
        status_display = 1;
        atualiza_display_ambev();
        return;
    }
    if (operarios <= operarios_atual)
    {
        conta_mostra = 0;
        operarios = operarios_atual;
    }
    else if (conta_mostra > 10)
    {
        conta_mostra = 0;
        operarios = operarios_atual;
    }
    
    if (media_dbm_ze)
    {
        //poe_dado_XY (2,14,operarios);
        if (status_display != 2) 
        {
                conta_troca_status = 0;
                desliga_buzzer();
                
                //liga_bip_curto_longo();
                status_display = 2;
                atualiza_display_ambev();
        }
      
    }
 
 }

void atualiza_display_ambev(void)
{
    switch (status_display)
    {
            case 1:
                    limpa_display();
                    if (situacao == 1)poe_texto_XY (1,2, "Pedestres a frente");
                    else poe_texto_XY (1,2, "Pedestres atras");
                    poe_texto_XY (3,7, "Pare!!!");
                    liga_buzzer();
                    delay_ms(2000);
                    desliga_buzzer();

            break;

            case 2:
                    limpa_display();
                    if (situacao == 1)poe_texto_XY (1,0, "ZE frente");
                    else poe_texto_XY (1,0, "ZE traseira");
                    poe_texto_XY (2,6, "Cuidado!!");
            break;

            case 4:
                    limpa_display();
                    desliga_rele1();
                    desliga_rele2();
                    desliga_saida_4();
                    desliga_buzzer();
                    poe_texto_XY (1,5, area_livre);
            break;

          }
    poe_texto_XY (4,0,"Op.:");
    if (operador_atual == 9)poe_texto_XY (4,4,gabriel);
    if (operador_atual == 8)poe_texto_XY (4,4,pedro);
    poe_texto_XY (4,18,"  ");
    poe_dado_XY (4,18,operador_atual_outro);
}

void analise_lateral(void)
{
    int x,y;
    conta_tags_direita = 0;
    conta_tags_esquerda = 0;
    todos_a_frente = 0;
    zona_direita = 0;
    zona_esquerda = 0;
    pedestre_esquerda = 0;
    pedestre_direita = 0;

    if (conta_leituras_antena_d || conta_leituras_antena_e);
    {
        if (tag_atual_d[1])zona_direita = 1;
        if (tag_atual_e[1])zona_esquerda = 1;
        tag_atual_d[1] = 0;
        tag_atual_e[1] = 0;
        for (x = 2; x < QT_MAX_TAGS; x++)
        {
            if (tag_atual_d[x])
            {
                if (zona_direita == 0) pedestre_direita++;
                conta_tags_direita++;
            }
            if (tag_atual_e[x])
            {
                if (zona_esquerda == 0) pedestre_esquerda++;
                conta_tags_esquerda++;
            }
            if (tag_atual_d[x] != tag_atual_e[x])y++;
            tag_atual_d[x] = 0;
            tag_atual_e[x] = 0;
        }
        if (y == 0) todos_a_frente = 1;

        if (status_display == 2)
        {
            if (zona_direita != zona_esquerda)
            {
                if (zona_direita)
                {
                    liga_rele2();
                    desliga_rele1();
                    poe_texto_XY (1,12,"direita ");
                }
                if (zona_esquerda)
                {
                    liga_rele1();
                    desliga_rele2();
                    poe_texto_XY (1,12,"esquerda");
                }
            }
            else
            {
                liga_rele1();
                liga_rele2();
                poe_texto_XY (1,12,"        ");
            }
        }
        if (status_display == 1)
        {
            desliga_rele2();
            desliga_rele1();
            liga_saida_4 ();
            if (pedestre_direita != pedestre_esquerda)
            {
                if (pedestre_direita)poe_texto_XY (2,5,"a direita ");
                if (pedestre_esquerda)poe_texto_XY (2,5,"a esquerda");
            }
            else poe_texto_XY (2,5,"          ");
        }
    }
}

#ifndef TEM_RASP
void pega_dados_tags (void)
{
    int x =0;
    int numero_tag = 0;
    int dbm_tag_atual = 0;
    int rssi_I = 0;
    int rssi_Q = 0;

    for (x = 0; x < QT_MAX_TAGS; x++)
    {
        if (tags_[x].epc[11])
        {
            if ((antena_atual == 1) ||(antena_atual == 4))
            {
                conta_leituras_antena_e++;
                numero_tag = tags_[x].epc[11];
                tag_atual_e[numero_tag] = tags_[x].epc[11];
                //if (tags_entrando[numero_tag]);
                //else tags_entrando[numero_tag] = tag_atual_e[numero_tag];
                //if (tags_saindo[numero_tag] == tag_atual_e[numero_tag])
                //{
                //    tags_saindo[numero_tag] = 0;
                //    tags_saiu[numero_tag] = tag_atual_e[numero_tag];
                //}
                //if (conta_tags_esquerda < 19)conta_tags_esquerda++;
                //else conta_tags_esquerda = 0;
               
            }
            else if ((antena_atual == 2) ||(antena_atual == 3))
            {
                conta_leituras_antena_d++;
                numero_tag = tags_[x].epc[11];
                tag_atual_d[numero_tag] = tags_[x].epc[11];
                //if (tags_saindo[numero_tag]);
                //else tags_saindo[numero_tag] = tag_atual_d[numero_tag];
                //if (tags_entrando[numero_tag] == tag_atual_d[numero_tag])
                //{
                //    tags_entrando[numero_tag] = 0;
                //    tags_entrou[numero_tag] = tag_atual_d[numero_tag];
                //}
                //if (conta_tags_direita < 19)conta_tags_direita++;
                //else conta_tags_direita = 0;
            }
            else if (antena_atual == 5)
            {
                numero_tag = tags_[x].epc[11];
                conta_leituras_operador++;
            }
            numero_tag &= 0x1F;
            acg_tag_atual[numero_tag] += tags_[x].agc;
            dbm_tag_atual = tags_[x].rssi;;
            rssi_I = dbm_tag_atual;
            rssi_I &= 0x0F;
            dbm_tag_atual &= 0xF0;
            rssi_Q = dbm_tag_atual >> 4;
            dbm_tag_atual = rssi_Q + rssi_I;
            dbm_tag[numero_tag] = dbm_tag[numero_tag] + dbm_tag_atual;
            if (numero_tag == operador_atual)dbm_tag[numero_tag] = 0;
            conta_leituras_dbm[numero_tag]++;
        }
    }
}
#endif 
/*
void lista_tags_frontais(void)
{
            int pt_tag_e = conta_tags_esquerda - 1;
            int pt_tag_d = conta_tags_direita - 1;
            int qt_tag_d = conta_tags_direita;
            int qt_tag_e = conta_tags_esquerda;

            conta_tags_frontal = 0;

            while (qt_tag_d)
            {
                while (qt_tag_e)
                {
                    if (tag_atual_d[pt_tag_d] == tag_atual_e[pt_tag_e])
                    {
                        tags_frontais[conta_tags_frontal]= tag_atual_d[pt_tag_d];
                        conta_tags_frontal++;
                        break;
                    }
                qt_tag_e--;
                pt_tag_e--;
                }
                pt_tag_d--;
                qt_tag_d--;
                qt_tag_e = conta_tags_esquerda;
                pt_tag_e = conta_tags_esquerda-1;
            }
}
*/
/*
void ordena_lista_tags(void)
{
    int x,y,aux = 0;

    if (conta_tags_direita)
    {
      for( x = 0; x < conta_tags_direita; x++ )
        {
            for( y = x + 1; y < conta_tags_direita; y++ )
            {
                if ( tag_atual_d[x] > tag_atual_d[y] )
                {
                    aux = tag_atual_d[x];
                    tag_atual_d[x] = tag_atual_d[y];
                    tag_atual_d[y] = aux;
                }
            }
        }
    }
    //x,y,aux = 0;
    if (conta_tags_esquerda)
    {
      for( x = 0; x < conta_tags_esquerda; x++ )
        {
            for( y = x + 1; y < conta_tags_esquerda; y++ )
            {
                if ( tag_atual_e[x] > tag_atual_e[y] )
                {
                    aux = tag_atual_e[x];
                    tag_atual_e[x] = tag_atual_e[y];
                    tag_atual_e[y] = aux;
                }
            }
        }
    }
}
*/
/*
void retira_tags_repetidas(void)
{
    pt_tags_total = 0;
    retirados = 0;
    if (conta_tags_direita > 1)
    {
        qt_tags_total = conta_tags_direita;
        while (qt_tags_total > 1)
        {
           if (tag_atual_d[pt_tags_total] == tag_atual_d[pt_tags_total + 1])//compara epc
           {
              tag_atual_d[pt_tags_total + 1] = 0;
              pt_deslocamento = pt_tags_total + 1;
              //poe_dado_XY (4,0,pt_deslocamento);
              //retirados++;
              deslocados = conta_tags_direita - 1;
              conta_tags_direita--;
              //if (dbm_tag_d[pt_tags_total] < dbm_tag_d[pt_tags_total + 1])//compara dbm
              //    dbm_tag_d[pt_tags_total] = dbm_tag_d[pt_tags_total + 1];
              while (deslocados)
              {
                //poe_dado_XY (4,5,deslocados);
                //poe_dado_XY (4,10,pt_deslocamento);
                tag_atual_d[pt_deslocamento] = tag_atual_d[pt_deslocamento + 1];//
                pt_deslocamento++;
                deslocados--;
              }
           }
           qt_tags_total--;
           pt_tags_total++;
        }
    }
    pt_tags_total = 0;
    retirados = 0;
    if (conta_tags_esquerda > 1)
    {
        qt_tags_total = conta_tags_esquerda;
        while (qt_tags_total > 1)
        {
           if (tag_atual_e[pt_tags_total] == tag_atual_e[pt_tags_total + 1])//compara epc
           {
              tag_atual_e[pt_tags_total + 1] = 0;
              pt_deslocamento = pt_tags_total + 1;
              //poe_dado_XY (4,0,pt_deslocamento);
              //retirados++;
              deslocados = conta_tags_esquerda - 1;
              conta_tags_esquerda--;
              //if (dbm_tag_e[pt_tags_total] < dbm_tag_e[pt_tags_total + 1])//compara dbm
              //    dbm_tag_e[pt_tags_total] = dbm_tag_e[pt_tags_total + 1];
              while (deslocados)
              {
                //poe_dado_XY (4,5,deslocados);
                //poe_dado_XY (4,10,pt_deslocamento);
                tag_atual_e[pt_deslocamento] = tag_atual_e[pt_deslocamento + 1];//
                pt_deslocamento++;
                deslocados--;
              }
           }
           qt_tags_total--;
           pt_tags_total++;
        }
    }
 }
*/

/*
void mostra_area_display(void)
{
    if ((conta_leituras_antena_e == 1) && (conta_leituras_antena_d == 1) )
    {
        if ((conta_tags_esquerda) || (conta_tags_direita) )
        {
            ordena_lista_tags();
            retira_tags_repetidas();
            lista_tags_frontais();
        }
        
        conta_leituras_antena_e = 0;
        conta_leituras_antena_d = 0;

        poe_texto_XY (2,0, vinte_espacos);
        poe_texto_XY (2,0, "E:");
        poe_dado_XY (2,2,conta_tags_esquerda);
        poe_texto_XY (2,5, "D:");
        poe_dado_XY (2,7,conta_tags_direita);
        if (situacao == 2) poe_texto_XY (2,10, "R:");
        else poe_texto_XY (2,10, "F:");
        poe_dado_XY (2,12,conta_tags_frontal);
        mostra_rssi_tags();
        conta_tags_direita = 0;
        conta_tags_esquerda = 0;
        conta_tags_frontal = 0;
            
    }
}
*/

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
#endif
#ifdef ANATEL

void lista_freq_anatel(void)
{
    Frequencies.freq[0]= 902000;
    Frequencies.freq[1]= 902500;
    Frequencies.freq[2]= 903000;
    Frequencies.freq[3]= 903500;
    Frequencies.freq[4]= 904000;
    Frequencies.freq[5]= 904500;
    Frequencies.freq[6]= 905000;
    Frequencies.freq[7]= 905500;
    Frequencies.freq[8]= 906000;
    Frequencies.freq[9]= 906500;
    Frequencies.freq[10]= 907000;
    Frequencies.freq[11]= 907500;
    Frequencies.freq[12]= 915000;
    Frequencies.freq[13]= 915500;
    Frequencies.freq[14]= 916000;
    Frequencies.freq[15]= 916500;
    Frequencies.freq[16]= 917000;
    Frequencies.freq[17]= 917500;
    Frequencies.freq[18]= 918000;
    Frequencies.freq[19]= 918500;
    Frequencies.freq[20]= 919000;
    Frequencies.freq[21]= 919500;
    Frequencies.freq[22]= 920000;
    Frequencies.freq[23]= 920500;
    Frequencies.freq[24]= 921000;
    Frequencies.freq[25]= 921500;
    Frequencies.freq[26]= 922000;
    Frequencies.freq[27]= 922500;
    Frequencies.freq[28]= 923000;
    Frequencies.freq[29]= 923500;
    Frequencies.freq[30]= 924000;
    Frequencies.freq[31]= 924500;
    Frequencies.freq[32]= 925000;
    Frequencies.freq[33]= 925500;
    Frequencies.freq[34]= 926000;
    Frequencies.freq[35]= 926500;
    Frequencies.freq[36]= 927000;
    Frequencies.freq[37]= 927500;
    Frequencies.freq[38]= 928000;

    Frequencies.numFreqs=39;
}

void acerta_freq_anatel (void)
{
    while (contador_int_ad != 0);
    //if (contador_int_ad == 0)
    //{
        if (tensao_an6 >= 200)tensao_an6 = tensao_an6/qt_leituras_ad;
        else tensao_an6 = 0;
        //diferenca_leituras = abs (tensao_an6 - leitura_ad_anterior);
        //if (diferenca_leituras > 5)
        //{
        leitura_ad_anterior = tensao_an6;
        if (tensao_an6 <=100)
        {
            sel_led (0,0);
            sel_led (1,1);
            Frequencies.freq[0]= 902000;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if ((tensao_an6 >105)&&(tensao_an6 <200))
        {
            sel_led (0,0);
            sel_led (2,1);
            Frequencies.freq[0]= 905000;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if ((tensao_an6 >205)&&(tensao_an6 <300))
        {
            sel_led (0,0);
            sel_led (3,1);
            Frequencies.freq[0]= 907500;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if ((tensao_an6 >305)&&(tensao_an6 <400))
        {
            sel_led (0,0);
            sel_led (4,1);
            Frequencies.freq[0]= 915000;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if ((tensao_an6 >405)&&(tensao_an6 <500))
        {
            sel_led (0,0);
            sel_led (5,1);
            Frequencies.freq[0]= 921000;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if ((tensao_an6 >505)&&(tensao_an6 <600))
        {
            sel_led (0,0);
            sel_led (6,1);
            Frequencies.freq[0]= 928000;
            Frequencies.numFreqs=1;
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        else if (tensao_an6 > 605)
        {
            sel_led (0,0);
            sel_led (7,1);
            lista_freq_anatel();
            //delay_ms(1);
            //readerInitStatus = as3993Initialize(Frequencies.freq[0]);
        }
        //}
        contador_int_ad = qt_leituras_ad;
        tensao_an6 = 0;
    //}
}
/*
void acerta_antena_anatel (void)
{
    if (contador_int_ad == 0)
    {
        tensao_an6 = tensao_an6/qt_leituras_ad;
        parar_leitura = 0;
        espera_usb = 0;
        if (tensao_an6 <=50)
        {
            antena_atual++;
            if (antena_atual > 8)antena_atual = 1;
        }
        else
        {
            antena_atual = tensao_an6/50;
            if ((antena_atual >= 9)&&(antena_atual <= 12))
            //if (antena_atual >= 9)
            {
                antena_atual = 0;
                parar_leitura = 1;
            }
            else if (antena_atual > 12)
            {
                antena_atual = 1;
                espera_usb = 1;
                parar_leitura = 1;
            }
        }
        sel_led (0,0);
        sel_antena (antena_atual);
        contador_int_ad = qt_leituras_ad;
        tensao_an6 = 0;
    }
}
*/
#endif

#ifdef AMBEV
void escolhe_antena(void)
{
    if (situacao == 1)
    {
        if(antena_atual == 1) antena_atual = 2;
        else antena_atual = 1;
    }
    else if (situacao == 2)
    {
        if (antena_atual == 3) antena_atual = 4;
        else antena_atual = 3;
    }
    else if (situacao == 3) antena_atual = 5;
    
    else antena_atual = 1;
    
    sel_led (0,0);
    sel_antena(antena_atual);
}

void situacao_mudou(void)
{
    if (situacao_anterior != situacao)
    {
        if (situacao == 1)tunerInit_A1(&mainTuner);
        if (situacao == 2)tunerInit_A3(&mainTuner);
        if (situacao == 3)tunerInit_A1(&mainTuner);
        //if (situacao == 5)tunerInit_A1(&mainTuner);
        
        dbm_tag[1] = 0;
        conta_leituras_dbm[1] = 0;
        media_dbm_ze = 0;
        conta_zero_ze = 0;
        tem_ze = 0;
        
        desliga_buzzer();
        operarios = 0;
        total_tags = 0;
        conta_tags_direita = 0;
        conta_tags_esquerda = 0;
        conta_tags_frontal = 0;
        conta_leituras_antena_e = 0;
        conta_leituras_antena_d = 0;
        conta_t_anterior = 0;
        conta_f_anterior = 0;
        status_display = 0;
        //pode_operar = 1;
        //pode_operar_anterior = 0;
        muda_display = 1;
        situacao_anterior = situacao;
    }
}

void poe_sentido(void)
{
if (contador_int_ad == 0)
        {
            if (tensao_an6 >= 200)tensao_an6 = tensao_an6/qt_leituras_ad;
            else tensao_an6 = 0;
            if(tensao_an6 > leitura_ad_anterior)diferenca_leituras = tensao_an6 - leitura_ad_anterior;
            else if (tensao_an6 < leitura_ad_anterior)diferenca_leituras = leitura_ad_anterior - tensao_an6;
            else diferenca_leituras = 0;
            if (diferenca_leituras > 3)
            {
                //situacao_anterior = situacao;
                leitura_ad_anterior = tensao_an6;
                
                if (tensao_an6 > 400)
                {
                    if (pode_operar)situacao = 1;
                    else situacao = 3;
                    //situacao_mudou();
                }
                else if (tensao_an6 < 200)
                {
                    if (pode_operar)situacao = 2;
                    else situacao = 3;
                    //situacao_mudou();
                }
                else situacao = 3;
                situacao_mudou();
            }

            contador_int_ad = qt_leituras_ad;
            tensao_an6 = 0;
        }
}
#endif

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


//chamada a cada interrupcao do timer 2

void recebeOperador (u8 dado){
    operador_atual_outro = dado;
}

/*
void resetContadores (void){
    contadorTimeOutRX = 0;
    contadorParaEnviarArrayXbee = 0;
}
 * */
void tick (void){
    //int contador;
    

    //(void)sprintf(arrayXbee, "teste um dois t 2");

    /*
    contadorTimeOutRX = contadorTimeOutRX + 1;
    if (contadorTimeOutRX == 100){//fecha pacote, vai para o tratamento
        contadorRX = 0;
        contadorTimeOutRX = 0;
        if (contadorRX <= TAMANHO_ARRAY_XBEE){ //se maior que o tamanho do array, eh invalido
            //tratamento aqui


        }
    }
     * */

    //contadorParaEnviarArrayXbee = contadorParaEnviarArrayXbee + 1;
    contadorParaEnviarArrayXbee++;
    if (contadorParaEnviarArrayXbee == 1000){
        contadorParaEnviarArrayXbee = 0;

        uartTx(operador_atual);

        /*
        if(DataRdyUART1()){
            _LATC8 = 1;
            operador_atual_outro = ReadUART1();
            _LATC8 = 0;
        }
         * */

        //operador_atual_outro = 1;
        

        /*
        for (contador = 0;contador < TAMANHO_ARRAY_XBEE;contador = contador + 1){
            (void)uartTxString(arrayXbee [contador]);
        }
         * */
    }
}

/*
void SetaObstrucaoXbee (void){
    FlagObstrucaoXbee = 1;
}

void LimpaObstrucaoXbee (void){
    FlagObstrucaoXbee = 0;
}
 * */
