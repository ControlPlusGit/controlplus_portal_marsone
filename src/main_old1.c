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
#include "gprs.h"
#include "varal.h"
#include "sankyu.h"

//#define TEM_RASP

//#if (SYSCLK == SYSCLK_16MHZ)
#if (FEMTO2 || FEMTO2_1 || PLACA_TECBAN)
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

//#endif
//#include "AD.h"


//#define TEMPO_PARA_NOVO_ENVIO 1000
//#define TEMPO_POR_SER_OBSTRUIDO 100

//#define TAMANHO_ARRAY_XBEE 20
//volatile char arrayXbee [TAMANHO_ARRAY_XBEE];
//volatile char operador_atual_outro = 0;

#ifdef PORTAL_AMBEV
//unsigned char tags_entrou[5], tags_saiu[5], status_tag_portal[5] = {0};
unsigned char mat_tag[5][6]= {{0},{0}};
unsigned char i, j, flag;
#endif
u32 counter;
int  conta_tags_validas = 0;//738,731,
char empilhadeira[20]= "738D"; //atualizada: 740D,730D,732D,738D,741D,735D,738D,733D,731D,736D,737D,729D
u8 pedestre[3]= {0};
volatile int operador_atual_outro = 0;
int conta_recebe_operador;
volatile int contadorParaEnviarArrayXbee;
volatile char FlagObstrucaoXbee;
volatile int contadorRX;
volatile int contadorTimeOutRX;
int rssi_tag;
int dbm_tag_real;
u8 status_zig = 0;
u8 status_dir = 0;
u8 status_3g = 0;
extern u32 pot_refletida;
u8 dado_enviado = 0;
extern int ContadorParaPulsoDeLigarOModem;
extern int ContadorParaOModemInicar;
extern char stringData[10];
extern char stringHora[10];
extern u16 estados_maquina_envio,estados_maquina_comandos,estados_maquina_acao_gps;
//volatile char arrayXbeeRecebido [TAMANHO_ARRAY_XBEE];

extern void calcula_dbm(int);
extern void tick(void);

extern Freq Frequencies;
//extern Tag tags_[MAXTAG];
extern Tag __attribute__((far)) tags_[MAXTAG];
int st_sai2, st_sai3, st_sai4, st_sai5, st_sai6, st_cin, st_cout, st_ld_pa;
int st_led1, st_led2, st_led2, st_led3, st_led4, st_led5, st_led6, st_led7, st_led8;
int st_clen, st_ant_ab, st_ant_a14, st_ant_b58, at_ant_a12, st_ant_a34, st_ant_b56, st_ant_b78;
int conta_giro = 0;
//unsigned int qt_leituras[8] = {0};
unsigned int qt_tags[8]= {0};
unsigned int conta_ausente_um [8];
unsigned int conta_presente_um[8];
unsigned int total_tags = 0;
unsigned int qt_presentes = 0;
unsigned int conta_operador_ausente = 0;
int atualiza_display = 0;
int antena_atual;
int total_parcial = 0;
//
//unsigned char dbm_tag_e[MAXTAG];
#ifdef BUSCA_TAG
__eds__ unsigned char __attribute__((far))pt_cin[100] = {0};
__eds__ unsigned char __attribute__((far))pt_cout[100] = {0};
__eds__ unsigned char __attribute__((far))pt_clen[100] = {0};
__eds__ unsigned char __attribute__((far))pt_rssi[100] = {0};
#endif

#ifndef PORTAL_AMBEV
__eds__ unsigned char __attribute__((far))tag_atual_e[MAXTAG] = {0};
__eds__ unsigned char __attribute__((far))tag_atual_d[MAXTAG] = {0};
#endif
#ifdef PORTAL_AMBEV
__eds__ unsigned char __attribute__((far))tag_atual[MAXTAG][3] = {{0},{0}};
__eds__ unsigned int __attribute__((far))status_tag_atual[MAXTAG] = {0};
__eds__ unsigned int __attribute__((far))conta_entrada[MAXTAG] = {0};
__eds__ unsigned int __attribute__((far))conta_saida[MAXTAG] = {0};
__eds__ unsigned int __attribute__((far))conta_meio[MAXTAG] = {0};
__eds__ unsigned int __attribute__((far))conta_sumico[MAXTAG] = {0};

const u8 ANTENA_ENTRADA = 1;
const u8 ANTENA_SAIDA = 2;
const u8 TAG_SUMIU = 10;
const u8 TAG_PASSOU = 2;
const u16 STATUS_ENTRADA = 1;
const u16 STATUS_SAIDA = 3;
const u16 STATUS_ENTRANDO = 12;
const u16 STATUS_SAINDO = 32;
const u16 STATUS_SAIU = 321;
const u16 STATUS_ENTROU = 123;
const u16 STATUS_NO_BUFFER = 99;
const u8 QT_LEITURAS = 2;
#endif
/*
#define POWER_NORMAL            1;
extern u8 currentSession;
extern u8 cyclicInventory;
extern u8 fastInventory;
extern u8 autoAckMode;
extern u8 rssiMode;
extern s8 rssiThreshold;
extern u8 readerPowerDownMode;
extern u8 tagDataAvailable;
extern u8 num_of_tags;
extern void powerDownReader(void);
*/
__eds__ unsigned int __attribute__((far)) dbm_tag[MAXTAG][1] = {{0},{0}};
__eds__ unsigned int __attribute__((far)) serie_tag[MAXTAG][3] = {{0},{0}};
//unsigned int acg_tag_atual[MAXTAG]  = {0};
//unsigned int conta_tag_frente[MAXTAG] = {0};
//unsigned int conta_tag_traseira [MAXTAG]= {0};
//unsigned char tags_frontais[MAXTAG];
__eds__ unsigned int __attribute__((far)) conta_leituras_dbm[MAXTAG] = {0};
unsigned char tags_no_meio[5], tags_entrando[5], tags_saindo[5], tags_entrou[5], tags_saiu[5], status_tag_portal[5];
unsigned char matr_entrou[5][3], matr_saiu[5][3];
//unsigned int pneus[MAXTAG] = {0};
#ifdef PORTAL_AMBEV
//unsigned int tags_entrando[5] = {0};
//unsigned int tags_saindo[5] = {0};
unsigned int tags_antena_1[5] = {0};
unsigned int tags_antena_2[5] = {0};
//unsigned int tags_no_meio[5] = {0};
//unsigned int tags_saiu[5] = {0};
//unsigned int tags_entrou[5] = {0};
char entrando[] = "Entra:";
char saindo[] = "Sai:";
#endif
char tag [30]= {0};
char operador [30]= {0};
int evento,rssi,antena,conta_registros = 0;
unsigned char meu_operador[3]= {0};
unsigned char tem_operador[30]= {0};
unsigned char qt_operadores = 0;
unsigned char total_dos_portais = 0;
unsigned char qt_entrou[11]= {0};
unsigned char time_out_operador[30]= {0};
unsigned char time_out_portal[10]= {0};
unsigned char operadores[30][3]= {{0},{0}};

__eds__ unsigned char __attribute__((far)) portais[10][300]= {{0},{0}};

unsigned char stpegapac = 0;
__eds__ unsigned char __attribute__((far)) pacote_chegou[310] = {0};
u8 conta_tentativas_tx = 0;
//unsigned char tags_entrou_corredor[5] = {0};
//unsigned char tags_saiu_corredor[5] = {0};
//unsigned char qt_tags_saiu = 0;
//unsigned char qt_tags_entrou = 0;
unsigned char tem_pacote = 0;
unsigned char qt_pacote = 0;
unsigned char cs_pacote = 0;
unsigned char no_serie = 0;
unsigned char pont_pac = 0;
unsigned char qt_tags_portal = 0;
unsigned char pacote_zerado = 0;
unsigned char qt_pac_total = 0;
long int tensao_an6 = 0;
const int qt_leituras_ad = 200;
int contador_int_ad = 200;
int situacao = 1;
int situacao_anterior = 0;
//int conta_tags_esquerda = 0;
//int conta_tags_direita = 0;
//int conta_leituras_antena_e = 0;
//int conta_leituras_antena_d = 0;
int conta_leituras_operador;
char tres_espacos[]= "   ";
char vinte_espacos[]= "                    ";
long int leitura_ad_anterior = 0;
int diferenca_leituras = 0;
const int QT_SOMA_RSSI = 10;
//int conta_tags_frontal = 0;
int coluna = 5;
int conta_mostra3 = 0;
u8  operador_atual = 0;
u16 conta_inatividade = 0;
//int qt_tags_total = 0;
//int pt_tags_total = 0;
//int conta_f_anterior = 0;
//int conta_t_anterior = 0;
int numero_tag = 0;
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
//char pedestre[] = "pedestres";
//char livre[] = "Livre";
//char parado[] = "Parado";
char desabilitado[] = "D:  ";
char autorizado[] = "A:  ";
char gabriel[] = "G";
char pedro[] = "P";
char ruy[] = "R";

int ja_ligou = 0;
unsigned int primeiro_pedestre = 0;
//int transmitindo;
int conta_giro_frente = 21;
int conta_giro_traseira = 21;
int tem_ze = 0;
int conta_zero_ze = 0;
int linha;
int contador;
int pode_operar = 0;
int pode_operar_anterior = 1;
int muda_display = 0;
unsigned int conta_area_livre;
unsigned char falta_antena[8] = {0};
unsigned int pot_ret_antena[8] = {0};
char barra[] = "/";
unsigned char minimo_leituras = 0;
unsigned char operador_lido = 0;
unsigned char pont_tag_atual = 0;
unsigned char tag_valida = 0;
int conta_envio = 0;
extern u8 status_conexao;
u8 cin,clen,cout = 0;
char status_0[] = "ENCERROU O PROCESSO";
char status_1[] = "MODEM PRESENTE";
char status_2[] = "CONECTOU NA REDE";
char status_3[] = "CONECTOU NA INTERNET";
char status_4[] = "CONECTOU NO FTP";
char status_5[] = "SUBINDO O ARQUIVO";
char status_6[] = "ARQUIVO ENVIADO";
char status_129[] = "MODEM NAO RESPONDE";
char status_130[] = "NAO CONECTA NA REDE";
char status_131[] = "NAO CONECTA NA INTERNET";
char status_132[] = "FTP CN";
char status_133[] = "NAO CONECTA FTP";
char status_134[] = "NAO ABRE ARQUIVO";
char status_135[] = "ERRO NAO LISTADO";
//int todos_a_frente;
//int zona_direita;
//int zona_esquerda;
//int pedestre_esquerda;
//int pedestre_direita;
void trata_tags_excessao(void);
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
//void desliga_saida_4 (void);
void liga_led_zig(void);
void desliga_led_zig(void);
void liga_led_wifi(void);
void desliga_led_wifi(void);
void liga_led_3g(void);
void desliga_led_3g(void);
void liga_led_gps(void);
void desliga_led_gps(void);
void desliga_rele4(void);
void liga_rele4(void);
void desliga_rele3(void);
void liga_rele3(void);
void liga_dir(void);
void desliga_dir(void);
void desliga_saida_4 (void);
//void trata_pacote_portal(void);
void mostra_tags_corredor(void);
void trata_tags_corredor (void);
void pisca_led_3g(void);
void le_sentido(void);
void troca_bytes(void);
void manda_tags(void);

extern u8 inventoryGen2(void);
void sel_led (int,int);
extern void pega_pot_refl (void);
void antena_de_1a4 (void);
void ligaTimer2(void);
extern void uart4Tx(u8);
void LigaRX (void);
void  trata_antenas_laterais(void);
u8 eh_antena_lateral(void);
void atualiza_conta_um(void);
void ativa_alarme_sem_um (void);
void trata_antenas_frente_re (void);
void atualiza_conta_area_livre(void);
void poe_texto_operador(void);
void tem_antena(void);
void mostra_todas_tags(void);
void bloqueia_reh(void);
void libera_reh(void);
void bloqueia_frente(void);
void libera_frente(void);
void desliga_led_vm(void);
void liga_led_vm(void);
void desliga_sirene(void);
void liga_sirene(void);
void trata_pacote_serial(void);
void envia_meu_operador(void);
void pow_off_modem(void);
void pow_on_modem(void);
void registra_evento(void);
void mostra_status_modem(void);
void le_capacete(void);
void busca_tag (void);
void desliga_led_tag(void);
void liga_led_tag(void);
void atualiza_conta_giro(void);
void seta_cod_empilhadeira(void);
void trata_raizen_retro(void);

extern u16 readerInitStatus;
/** FW information which will be reported to host. */
static const char gAS3993FwInfo[] = FIRMWARE_ID"||"HARDWARE_ID;
/** FW information which will be logged on startup. Version information is included in logger. */
static const char gLogStartup[] = FIRMWARE_ID" %hhx.%hhx.%hhx on "HARDWARE_ID"\n";
/** FW version which will be reported to host */
const u32 firmwareNumber = FIRMWARE_VERSION;
//static u8 usedAntenna = 8;

void  mostra_status_modem(void)
{
   poe_texto_XY(3,0,vinte_espacos);
   switch (status_conexao)
   {
        case 0:
            poe_texto_XY(3,0,status_0);
            //liga_led_3g();
            break;
        case 1:
            poe_texto_XY(3,0,status_1);
            //desliga_led_3g();
            break;
        case 2:
            poe_texto_XY(3,0,status_2);
            //liga_led_3g();
            break;
        case 3:
            poe_texto_XY(3,0,status_3);
            //desliga_led_3g();
            break;
        case 4:
            poe_texto_XY(3,0,status_4);
            //liga_led_3g();
            break;
        case 5:
            poe_texto_XY(3,0,status_5);
            //desliga_led_3g();
            break;
        case 6:
            poe_texto_XY(3,0,status_6);
            //desliga_led_3g();
            break;
        case 129:
            poe_texto_XY(3,0,status_129);
            break;
        case 130:
            poe_texto_XY(3,0,status_130);
            break;
        case 131:
            poe_texto_XY(3,0,status_131);
            break;
        case 132:
            poe_texto_XY(3,0,status_132);
            break;
        case 133:
            poe_texto_XY(3,0,status_133);
            break;
        case 134:
            poe_texto_XY(3,0,status_134);
            break;
        case 135:
            poe_texto_XY(3,0,status_135);
            break;
        default:
            poe_texto_XY(3,0,"ST.Desc");
            break;
   }

}
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
    sel_led (antx,1);
    switch (antx)
    {
            case 1:
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(1);                //sel. 1-2
                SEL_A1_2(1);                //sel. A1
            break;
            case 2:
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(1);                //sel. 1-2
                SEL_A1_2(0);                //sel. A2
            break;
            case 3:
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(0);                //sel. 3-4
                SEL_A3_4(1);                //sel. A3
            break;
            case 4:
                SEL_BBA(1);                 //sel. grupo 1-4
                SEL_A1_4(0);                //sel. 3-4
                SEL_A3_4(0);                //sel. A4
            break;
            case 5:
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(1);                //sel. 5-6
                SEL_B5_6(1);                //sel. A5
            break;
            case 6:
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(1);                //sel. 5-6
                SEL_B5_6(0);                //sel. A6
            break;
            case 7:
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(0);                //sel. 7-8
                SEL_B7_8(1);                //sel. A7
            break;
            case 8:
                SEL_BBA(0);                 //sel. grupo 5-8
                SEL_B5_8(0);                //sel. 7-8
                SEL_B7_8(0);                //sel. A8
            break;
    }
}

void sel_cap_cin(int x)
{
    TUNE_CAP2(0);
    TUNE_CAP3(0);
    TUNE_CAP1(x);
}
void sel_cap_cout(int x)
{
    TUNE_CAP1(0);
    TUNE_CAP3(0);
    TUNE_CAP2(x);
}
void sel_cap_clen(int x)
{
    TUNE_CAP1(0);
    TUNE_CAP2(0); 
    TUNE_CAP3(x);
}


void sel_led (int led, int status)
{
    switch (led)
    {
            case 0:     //led = 0 = poe estatus em todos
                LED_A1(status);
                LED_A2(status);
                LED_A3(status);
                LED_A4(status);
                LED_A5(status);
                LED_A6(status);
                LED_A7(status);
                LED_A8(status);
                break;
            case 8:
                LED_A4(status);
                break;
            case 7:
                LED_A3(status);
                break;
            case 6:
                LED_A2(status);
                break;
            case 5:
                LED_A1(status);
                break;
            case 4:
                LED_A8(status);
                break;
            case 3:
                LED_A7(status);
                break;
            case 2:
                LED_A6(status);
                break;
            case 1:
                LED_A5(status);
                break;
    }
}
void pow_on_modem(void)
{
     _LATA2 = 0;
}
void pow_off_modem(void)
{
     _LATA2 = 1;
}
void liga_saida_pa (void)
{
#ifdef RF_NOVA
    ld_pa (0);
#else
    ld_pa (1);
#endif
}
void desliga_saida_pa (void)
{
#ifdef RF_NOVA
    ld_pa (1);
#else
    ld_pa (0);
#endif
}

void ld_pa (int status )
{
    LIGA_PA(status);
}
void liga_led_tag(void)
{
     LED_TAG(1);
}
void desliga_led_tag(void)
{
     LED_TAG(0);
}
void ld_saidas (int saida, int status)
{
    switch (saida)
    {
        case 0:
            SAI_1(status);
            SAI_2(status);
            SAI_3(status);
            SAI_4(status);
            SAI_5(status);
            SAI_6(status);
            SAI_7(status);
            break;
        case 1:
            SAI_1(status);
            break;
        case 2:
            SAI_2(status);
            break;
        case 3:
            SAI_3(status);
            break;
        case 4:
            SAI_4(status);
            break;
        case 5:
            SAI_5(status);
            break;
        case 6:
            SAI_6(status);
            break;
        case 7:
            SAI_7(status);
            break;
    }
}
void bloqueia_reh(void)
{
    poe_texto_XY(2,17,"R");
//#ifdef SANKYU
//    liga_rele1();
//#else
    desliga_rele1();
//#endif
}
void libera_reh(void)
{
    poe_texto_XY(2,17," ");
//#ifdef SANKYU
//    desliga_rele1();
//#else
    liga_rele1();
//#endif
}
void bloqueia_frente(void)
{
    poe_texto_XY(2,16,"F");
    desliga_rele3();
}
void libera_frente(void)
{
    poe_texto_XY(2,16," ");
    liga_rele3();
}
void desliga_led_vm(void)
{
    poe_texto_XY(2,18," ");
    desliga_rele4();
}
void liga_led_vm(void)
{
    poe_texto_XY(2,18,"V");
    liga_rele4();
}
void desliga_sirene(void)
{
    poe_texto_XY(2,19," ");
    desliga_rele2();
}
void liga_sirene(void)
{
    poe_texto_XY(2,19,"S");
    liga_rele2();
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
void liga_rele3(void)
{
    ld_saidas (1,1);
}
void liga_rele4(void)
{
    ld_saidas (2,1);
}
void desliga_rele3(void)
{
    ld_saidas (1,0);
}
void desliga_rele4(void)
{
    ld_saidas (2,0);
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
void liga_led_zig(void)
{
    status_zig = 1 ;
    LED_ZIG(1);
}
void desliga_led_zig(void)
{
    status_zig = 0 ;
    LED_ZIG(0);
}
void liga_led_wifi(void)
{
    LED_WF(1);
}
void desliga_led_wifi(void)
{
    LED_WF(0);
}
void pisca_led_3g(void)
{
    if (status_3g == 1)desliga_led_3g();
    else liga_led_3g();
}
void liga_led_3g(void)
{
    status_3g = 1;
    LED_3G(1);
}
void desliga_led_3g(void)
{
    status_3g = 0;
    LED_3G(0);
}
 void liga_led_gps(void)
{
    LED_GPS(1);
}
void desliga_led_gps(void)
{
    LED_GPS(0);
}
 void liga_dir(void)
{
    status_dir = 1 ;
    SAIDA_DIR(1);
}
void desliga_dir(void)
{
    status_dir = 0 ;
    SAIDA_DIR(0);
}


void limpa_buffer_tags (void)
{
    int x;
    for (x = 0; x < MAXTAG; x++)
        {
            tags_[x].epc[11] = 0;
            tags_[x].epc[10] = 0;
            tags_[x].epc[9] = 0;
            tags_[x].epc[0] = 0;
            tags_[x].epc[1] = 0;
            tags_[x].epc[2] = 0;
            serie_tag[x][0] = 0;
            serie_tag[x][1] = 0;
            serie_tag[x][2] = 0;
        }
}


void limpa_e_troca_antena(void)
{
        limpa_buffer_tags();
        escolhe_antena();
        //sel_led (0,0);
        //sel_antena (antena_atual);
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


/*
void antena_de_1a4 (void)
{
    antena_atual++;
    if (antena_atual > 4)antena_atual = 1;
    sel_led (0,0);
    sel_antena(antena_atual);
}
*/

void registra_evento(void)
{
    //conta_registros++;
    if (conta_registros < 100)
    {
        conta_registros++;
        sprintf(tag,"%02x%02x%02x",pedestre[2],pedestre[1],pedestre[0]);
        sprintf(operador,"%02x%02x%02x", meu_operador[2], meu_operador[1], meu_operador[0]);
        if (situacao == 1)evento = 3;
        else evento = 4;
        antena = antena_atual;
        //rssi = 11;
        escreverNoBufferDeFtp(tag, operador,evento, antena, rssi);
    }
}
/*
void seta_cod_empilhadeira(void)
{
#ifdef SANKYU
//Sankyu:728D=11,729D=12,730D=13,731D=14,732D=15,733D=16,734D=17,735D=18,736D=19,737D=20,738D=21
//740D=22,741D=23
    int x = NUM_SERIE;
    switch (x)
    {
        case 11:
            empilhadeira[0] = "728D";
        break;
        case 12:
            empilhadeira[0] = "729D";
        break;
        case 13:
            empilhadeira[0] = "730D";
        break;
        case 14:
            empilhadeira[0] = "731D";
        break;
        case 15:
            empilhadeira[0] = "732D";
        break;
        case 16:
            empilhadeira[0] = "733D";
        break;
        case 17:
            empilhadeira[0] = "734D";
        break;
        case 18:
            empilhadeira[0] = "735D";
        break;
        case 19:
            empilhadeira[0] = "736D";
        break;
        case 20:
            empilhadeira[0] = "737D";
        break;
        case 21:
            empilhadeira[0] = "738D";
        break;
        case 22:
            empilhadeira[0] = "740D";
        break;
        default://23
            empilhadeira[0] = "741D";
    }

#endif

}
*/
void  trata_raizen_retro(void)
{
    int x,z,y,total_parcial,conta_livre,ja_ligou_sirene = 0;
    while (1)
    {
        antena_atual++;
        if (antena_atual > 5)antena_atual = 1;
        sel_led (0,0);
        sel_antena(antena_atual);
        total_parcial = inventoryGen2();
        troca_bytes();
        if (total_parcial)
        {
            liga_led_tag();
            for (x=0;x < total_parcial ;x++)
            {
                if ((tags_[x].epc[11] == 1)&& (tags_[x].epc[10] == 0)&&(tags_[x].epc[9] == 0)&&
                    (tags_[x].epc[8] == 0)&& (tags_[x].epc[7] == 0)&&(tags_[x].epc[6] == 0)&&
                    (tags_[x].epc[5] == 0)&& (tags_[x].epc[4] == 0)&&(tags_[x].epc[3] == 0)&&
                    (tags_[x].epc[2] == 0)&& (tags_[x].epc[1] == 0)&&(tags_[x].epc[0] == 0))
                {
                    conta_livre = 0;
                    if (ja_ligou_sirene == 0)
                    {
                        liga_led_vm();
                        liga_sirene();
                        delay_ms(250);
                        desliga_sirene();
                        ja_ligou_sirene = 1;
                    }
                }
            }
        }
        else
        {
            conta_livre++;
            desliga_led_tag();
            if (conta_livre == 50)
            {
                ja_ligou_sirene = 0;
                desliga_led_vm();
            }
        }
    }
}

int main(void)
{
    CNPU1bits.CN15PUE = 1;
    CNPU2bits.CN16PUE = 1;
    CNPU2bits.CN19PUE = 1;
    CNPU4bits.CN49PUE = 1;
    CNPU4bits.CN56PUE = 1;
    CNPU4bits.CN57PUE = 1;
    CNPU2bits.CN28PUE = 1;
    //seta_cod_empilhadeira();
    //GP_9(1);
    //GP_10(1);
    desliga_saida_pa();
    sel_cap_cin (0);
    sel_cap_cout (0);
    sel_cap_clen (0);
    ld_saidas (0,0);

    u32 baudrate, realrate;
            
    systemInit();
    timerInit();
    platformInit();
    spiInit();

    baudrate = BAUDRATE;
//#ifndef PORTAL
    uartTxInitialize(SYSCLK, baudrate, &realrate);
    uart2TxInitialize(SYSCLK, baudrate, &realrate);
    uart3TxInitialize(SYSCLK, baudrate, &realrate);
    //baudrate = 9600;
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

//#ifdef AMBEV
    //pow_off_modem();
    delay_ms (1000);
    //pow_on_modem();
//#endif
    //desliga_rele2();
    desliga_saida_pa ();
 
    desliga_buzzer();

    bloqueia_frente();
    bloqueia_reh();
    desliga_led_vm();
    desliga_sirene();

    desliga_led_zig();
    desliga_led_wifi();
    desliga_led_3g();
    desliga_led_gps();
    sel_led (0,0);
    desliga_led_tag ();
    desliga_dir();
 
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
//#if DEMO || PLACA_TECBAN || AJUSTA_ANTENA
//    Frequencies.freq[0]= 915750;
//    Frequencies.numFreqs=1;
//#endif

//#ifndef PORTAL
    /*
    IFS0bits.AD1IF = 0;             // Limpa flag da interrupÃƒÆ’Ã‚Â§ÃƒÆ’Ã‚Â£o
    IEC0bits.AD1IE = 1;             // Liga a interrupÃƒÆ’Ã‚Â§ÃƒÆ’Ã‚Â£o
    tensao_an6 = 0;
    contador_int_ad = qt_leituras_ad;
    inicializa_AD();*/
//#endif
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

#ifndef BUSCA_TAG
    lista_freq_anatel();
#endif

#ifdef BUSCA_TAG
    Frequencies.freq[0]= 915750;
    Frequencies.numFreqs=1;
#endif

    delay_ms(1);
    readerInitStatus = as3993Initialize(Frequencies.freq[0]);

#ifndef PORTAL_AMBEV
    initCommands(); // USB report commands
#endif

#ifdef AJUSTA_ANTENA
    initCommands(); // USB report commands
#endif

#ifdef TUNER
#ifdef PORTAL
    tunerInit_a1_portal(&mainTuner);
#else
    tunerInit(&mainTuner);
#endif    
#ifdef TECBAN
    tunerInit_tec_ban_ax(&mainTuner);
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

 //Obs. ver travamento aqui
#ifdef AJUSTA_ANTENA
    StreamDispatcherInit(SYSCLK);
#if !USE_UART_STREAM_DRIVER
    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    USBDeviceAttach();
#endif
#endif

    desliga_saida_pa();
    inicia_display ();
//#ifdef AMBEV
    delay_ms(1000);
//#endif
    limpa_display();

/*#ifndef ANATEL
    sel_led (0,0);
    antena_atual = 1;
    sel_antena (antena_atual);
    situacao = 1;
#endif*/
    //antena_atual = 1;
    //sel_antena (antena_atual);
    desliga_led_tag ();
    unsigned int conta_leituras = 0;
    int conta_zero_tags = 0;
   
#ifdef PORTAL
    int conta_tags_presentes;
    int conta_tags_atuais;
    int total_tags_momento_portal[8];
    int total_tags_momento;
    int conta_tags_atuais_momento;
#endif

#ifdef DEMO
    poe_texto_XY (1,0,"Control-Up Reader:");
#ifdef PORTAL
    poe_texto_XY (1,0," Control-Up Reader");
    poe_texto_XY (2,0,"Tags:");
#endif    
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
    situacao = 3;
    escolhe_antena();  
#endif

#ifdef ANATEL
    //inicializa_AD();
    poe_texto_XY (1,0,"1:");
    
#endif

#ifdef VARAL
    IniciaVaral();
#endif
    
    //IFS0bits.AD1IF = 0;             // Limpa flag da interrupÃƒÆ’Ã‚Â§ÃƒÆ’Ã‚Â£o
    //IEC0bits.AD1IE = 1;             // Liga a interrupÃƒÆ’Ã‚Â§ÃƒÆ’Ã‚Â£o
#ifdef PLACA_TECBAN
    antena_atual = 0;
    u8 k7_ocupados = 0;
#endif

#ifndef ANATEL
    sel_led (0,0);
    antena_atual = ANTENA_ATUAL;
    sel_antena (antena_atual);
#endif
#define normal
    ligaTimer2();
    //uartRxInitialize();
    LigaRX();
#ifdef AMBEV
    int conta_tag_usp[5] = {0};
    int dbm_tag_usp[5] = {0};
    int num_tag_usp[5] = {0};
    int conta_zero_tag_usp[5] = {0};
#ifndef BUSCA_TAG
    LigaI2CRelogio ();
    IniciaMaquinaGPRS();
    //sprintf(empilhadeira, "EFOOO11");
    ///escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena){
    //escreverNoBufferDeFtp("ED000","EA0001", 0, 10, 0);
    //SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano)
    //SetarHoraRTC(43,12,11,1,16);
    poe_texto_XY(3,0,vinte_espacos);
    poe_texto_XY(3,0,empilhadeira);
    poe_dado_XY(3,15,NUM_SERIE);
    testeI2C();
    poe_texto_XY(4,0,stringData);
    poe_texto_XY(4,12,stringHora);
#endif
    delay_ms(1000);
#endif
    int x,y,z,k;
    //escreverNoBufferDeFtp(char *tag, char *operador, int evento, int rssi, int antena)
#ifdef PORTAL_AMBEV
    poe_texto_XY (1,0,entrando);
    poe_texto_XY (2,0,saindo);
    u8 passa_rapido = 0;
    u8 conta_passada = 0;
#endif
    
    while (1)
    {
#ifdef RAIZEN_RETRO
        trata_raizen_retro();
#endif
#ifdef VARAL
        while (1)
        {
            for (antena_atual = 1;antena_atual < 5;antena_atual++)
            {
                processaCUP();
                //ProcessaEnvioDeTagsNoVaral();
                //ProcessaGPS();
                sel_led (0,0);
                sel_antena (antena_atual);
                //limpa_buffer_tags ();
                total_parcial = 0;
                if (transmitindo == 0)
                {
                    total_parcial = inventoryGen2();                
                }
                if (total_parcial)
                {
                    liga_led_tag();
                    //poe_texto_XY(1,0,"Qt:    ");
                    //poe_dado_XY(1,3,total_parcial);
                    troca_bytes();
                    ProcessaTagsParaOVaral();
                }
                else desliga_led_tag();
            }
        }
#endif

#ifdef BUSCA_TAG
        sel_led (0,0);
        antena_atual = 1;
        sel_antena (antena_atual);
        while (1)
        {
            busca_tag ();
        }
#endif
#ifdef PORTAL_AMBEV
    for (conta_passada = 0;conta_passada < 5;conta_passada++)
    {
        for (y = 0; y < QT_LEITURAS; y++)
        {
            for (k=0;k < MAXTAG; k++)
            {
                tags_[k].epc[11] = 0;
                tags_[k].epc[10] = 0;
                tags_[k].epc[9] = 0;
            }
            total_parcial = inventoryGen2();
            troca_bytes();
            poe_texto_XY(4,17,"   ");
            poe_dado_XY(4,17,status_tag_atual[152]);
            if (total_parcial)
            {
                conta_inatividade = 0;
                liga_led_tag();
                conta_zero_tags = 0;
                if (antena_atual == ANTENA_ENTRADA)
                {
                    poe_texto_XY(3,0,"   ");
                    for (x=0;x < total_parcial  ;x++)
                    {
                        z = tags_[x].epc[11];
                        z &= MAXTAG;

                        if (z)
                        {
                            dbm_tag[z][0] = z;
                            tag_atual[z][0] = z;
                            tag_atual[z][1] = tags_[x].epc[10];
                            tag_atual[z][2] = tags_[x].epc[9];
                            poe_dado_XY(3,0,z);
                            if (passa_rapido == 0)passa_rapido = 1;
                            else passa_rapido = 21;
                            conta_entrada[z]++;
                        }
                    }
                }
                if (antena_atual == ANTENA_SAIDA)
                {
                    poe_texto_XY(4,0,"   ");
                    for (x=0;x < total_parcial  ;x++)
                    {
                        z = tags_[x].epc[11];
                        z &= MAXTAG;

                        if (z)
                        {
                            dbm_tag[z][0] = z;
                            tag_atual[z][0] = z;
                            tag_atual[z][1] = tags_[x].epc[10];
                            tag_atual[z][2] = tags_[x].epc[9];
                            poe_dado_XY(4,0,z);
                            if (passa_rapido == 0)passa_rapido = 2;
                            else passa_rapido = 12;
                            conta_saida[z]++;
                        }
                    }
                }
            }
            else
            {
                if (conta_inatividade < 50000)conta_inatividade++;
                desliga_led_tag();
                conta_zero_tags++;
            }
            if (antena_atual == ANTENA_SAIDA)antena_atual = ANTENA_ENTRADA;
            else antena_atual = ANTENA_SAIDA;
            sel_led (0,0);
            sel_antena (antena_atual);
        }
        
        for (x=0;x < MAXTAG; x++)
        {
            if ( dbm_tag[x][0])
            {
                conta_sumico[x] = 0;
                dbm_tag[x][0] = 0;
                if (conta_saida[x] && conta_entrada[x])
                {
                    LED_A7(1);
                    LED_A8(1);
                    conta_meio[x]++;
                    conta_saida[x] = 0;
                    conta_entrada[x] = 0;
                    if (status_tag_atual[x] == 0)
                    {
                        if (passa_rapido == 21)status_tag_atual[x] = STATUS_SAIU;
                        else if (passa_rapido == 12)status_tag_atual[x] = STATUS_ENTROU;
                        passa_rapido = 0;
                    }
                    //if (status_tag_atual[x] == STATUS_SAIDA)status_tag_atual[x] = STATUS_SAINDO;
                    //else if (status_tag_atual[x] == STATUS_ENTRADA)status_tag_atual[x] = STATUS_ENTRANDO;
                    if (status_tag_atual[x] == STATUS_SAIDA)status_tag_atual[x] = STATUS_SAIU;
                    else if (status_tag_atual[x] == STATUS_ENTRADA)status_tag_atual[x] = STATUS_ENTROU;
                }
                else if (conta_saida[x])
                {
                    LED_A7(1);
                    LED_A8(0);
                    conta_saida[x] = 0;
                    conta_meio[x] = 0;
                    passa_rapido = 0;
                    if (status_tag_atual[x] == 0)status_tag_atual[x] = STATUS_SAIDA;
                    else if (status_tag_atual[x] == STATUS_ENTRANDO)status_tag_atual[x] = STATUS_ENTROU;
                    else if (status_tag_atual[x] == STATUS_SAINDO)status_tag_atual[x] = STATUS_SAIDA;
                    else if (status_tag_atual[x] == STATUS_ENTRADA)status_tag_atual[x] = STATUS_ENTROU;
                }
                else if (conta_entrada[x])
                {
                    LED_A7(0);
                    LED_A8(1);
                    conta_entrada[x] = 0;
                    conta_meio[x] = 0;
                    passa_rapido = 0;
                    if (status_tag_atual[x] == 0)status_tag_atual[x] = STATUS_ENTRADA;
                    else if (status_tag_atual[x] == STATUS_SAINDO)status_tag_atual[x] = STATUS_SAIU;
                    else if (status_tag_atual[x] == STATUS_ENTRANDO)status_tag_atual[x] = STATUS_ENTRADA;
                    else if (status_tag_atual[x] == STATUS_SAIDA)status_tag_atual[x] = STATUS_SAIU;
                }

            }
            else if (tag_atual[x][0])
            {
                conta_sumico[x]++;
                if (conta_sumico[x] == TAG_SUMIU)
                {
                    conta_sumico[x] = 0;
                    status_tag_atual[x] = 0;
                    conta_entrada[x] = 0;
                    conta_saida[x] = 0;
                    conta_meio[x] = 0;
                    tag_atual[x][0] = 0;
                    tag_atual[x][1] = 0;
                    tag_atual[x][2] = 0;
                }
            }
            if (status_tag_atual[x] == STATUS_ENTROU)
            {
                for(i=0; i<5; i++)
                {
                    if(x == matr_entrou[i][0]) break;
                    else if(matr_entrou[i][0] == 0)
                    {
                        liga_dir();
                        status_tag_atual[x] = STATUS_NO_BUFFER;
                        matr_entrou[i][0] =  tag_atual[x][0];
                        matr_entrou[i][1] =  tag_atual[x][1];
                        matr_entrou[i][2] =  tag_atual[x][2];
                        dado_enviado = 0;
                        break;
                    }
                }
            }
            else if (status_tag_atual[x] == STATUS_SAIU)
            {
                for(i=0; i<5; i++)
                {
                    if(x == matr_saiu[i][0]) break;
                    else if(matr_saiu[i][0] == 0)
                    {
                        desliga_dir();
                        status_tag_atual[x] = STATUS_NO_BUFFER;
                        matr_saiu[i][0] =  tag_atual[x][0];
                        matr_saiu[i][1] =  tag_atual[x][1];
                        matr_saiu[i][2] =  tag_atual[x][2];
                        dado_enviado = 0;
                        break;
                    }
                }
            }
        }
    }
            z = 6;
            y = 6;
            for (x= 0; x <5 ; x++)
            {
                if (matr_entrou[x][0])
                {
                    poe_texto_XY(1,y,"   ");
                    poe_dado_XY (1,y,matr_entrou[x][0]);
                    y += 3;
                }

                if (matr_saiu[x][0])
                {
                    poe_texto_XY(2,z,"   ");
                    poe_dado_XY (2,z,matr_saiu[x][0]);
                    z += 3;
                }
            }
            if (dado_enviado == 0)
            {
                manda_tags();
                //limpa_display();
                //poe_texto_XY (1,0,entrando);
                //poe_texto_XY (2,0,saindo);
                 for(x=0; x<5; x++)
                {
                    for(y=0; y<3; y++)
                    {
                        matr_saiu[x][y] = 0;
                        matr_entrou[x][y] = 0;
                    }
                }

            }
            if (conta_zero_tags > 30)
            //if (conta_zero_tags > 60)
            {
                LED_A7(0);
                LED_A8(0);
                limpa_display();
                poe_texto_XY (1,0,entrando);
                poe_texto_XY (2,0,saindo);
                
                conta_zero_tags = 0;
       
                for(x=0; x<5; x++)
                {
                    for(y=0; y<3; y++)
                    {
                        matr_saiu[x][y] = 0;
                        matr_entrou[x][y] = 0;
                    }
                }
            }
#endif
 
#ifdef AJUSTA_ANTENA
        //sel_led (0,0);
        //sel_antena (antena_atual);
        ProcessIO(); // main trigger for operation commands.

        doCyclicInventory();      // do cyclic inventory if necessary

        //total_tags = inventoryGen2();
        //delay_ms (100);
        /*if (antena_atual == 1)antena_atual = 2;
        else antena_atual = 1;
        sel_led (0,0);
        sel_antena (antena_atual);*/
        
         
        if (total_tags)
        {
            liga_led_tag ();
            //y = tags_[0].epc[11];
            //conta_leituras += total_tags;

        }
        else
        {
            desliga_led_tag ();
            y = 0;
        }
        /*if (antena_atual == 8)antena_atual = 0;
        antena_atual++;
        sel_led (0,0);
        sel_antena (antena_atual);*/
        total_tags = 0;
        //LED_A8(1);
        //conta_leituras++;
        /*total_tags = 0;
        poe_texto_XY (1,0,vinte_espacos);
        poe_dado_XY (1,0,y);
        poe_texto_XY (2,0,vinte_espacos);
        poe_dado_XY (2,0,conta_leituras);*/
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
            liga_led_tag ();
            y = tags_[0].epc[11];
        }
        else
        {
            desliga_led_tag ();
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

        if (antena_atual == 1) tunerInit_tec_ban_ax(&mainTuner);
        antena_atual++;
        if (antena_atual > 4)
        {
            tunerInit_tec_ban_a1(&mainTuner);
            antena_atual = 1;
        }
        sel_antena (antena_atual);
        //ProcessIO(); // main trigger for operation commands.

        //doCyclicInventory();      // do cyclic inventory if necessary
        
        while(x < 20)
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
    conta_tags_atuais = 0;
    /*
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
            }*/

            //ProcessIO(); // main trigger for operation commands.

            //doCyclicInventory();      // do cyclic inventory if necessary
    //for (x = 0; x < 9; x++ )
    //{
    //     total_tags_momento_portal[x] = 0;
    //}
    
#ifndef PORTAL
    conta_tags_atuais = 0;
    conta_tags_atuais_momento = 0;
    total_tags_momento = 0;
    for (x = 0; x < MAXTAG; x++ )
    {
        tag_atual_d[x] = 0;
    }
    for (x = 0; x < 20; x++)
    {
#endif
#ifdef PORTAL
    for (z=0; z < MAXTAG; z++)
    {
        tags_[z].epc[11]= 0 ;
    }
     for (z=0; z < MAXTAG; z++)
    {
     dbm_tag[z][0] = 0;
     }
    for (y = 1 ;y < 40;y++)
    {
            delay_ms (10);
            antena_atual++;
            if (antena_atual == 9)antena_atual = 1;
            if (antena_atual == 1)tunerInit_a1_portal(&mainTuner);
            if (antena_atual == 2)tunerInit_a2_portal(&mainTuner);
            if (antena_atual == 3)tunerInit_a3_portal(&mainTuner);
            if (antena_atual == 4)tunerInit_a4_portal(&mainTuner);
            if (antena_atual == 5)tunerInit_a5_portal(&mainTuner);
            if (antena_atual == 6)tunerInit_a6_portal(&mainTuner);
            if (antena_atual == 7)tunerInit_a7_portal(&mainTuner);
            if (antena_atual == 8)tunerInit_a8_portal(&mainTuner);
            delay_ms (10);
            sel_led (0,0);
            sel_antena (antena_atual);
            for (x = 1 ;x < 3;x++)
            {
            
                //ProcessIO(); // main trigger for operation commands.

                //doCyclicInventory();      // do cyclic inventory if necessary
                total_tags = inventoryGen2();
                for (z=0; z < MAXTAG; z++)
                {
                    tags_[z].epc[11] &= MAXTAG;
                    if (tags_[z].epc[9])dbm_tag[tags_[z].epc[11]][0]++;
                    tags_[z].epc[11]= 0 ;
                }
                if (total_tags) liga_led_tag ();
                else  desliga_led_tag ();
                //total_tags_momento_portal[x] = total_tags;
                //if (conta_tags_atuais < total_tags)conta_tags_atuais = total_tags;
            }
    }
        //total_tags_momento = inventoryGen2();
        //conta_tags_atuais = total_tags;
        //conta_tags_atuais = total_tags_momento_portal[1];
        //for (x = 2 ;x < 9;x++)
        //{
        //  if (total_tags_momento_portal[x] > conta_tags_atuais) conta_tags_atuais = total_tags_momento_portal[x];
        //}
        /*
        if (conta_tags_atuais)
        {
            liga_led_tag ();
        }
        else
        {
            desliga_led_tag ();
        }*/
        conta_tags_atuais = 0;
        for (z=0; z < MAXTAG ; z++)
        {
            if (dbm_tag[z][0])conta_tags_atuais++;
        }
        poe_texto_XY (2,5,"   ");
        poe_dado_XY (2,5,conta_tags_atuais);
        //total_tags_momento = 0;
#endif

#ifndef PORTAL
        ProcessIO(); // main trigger for operation commands.
        doCyclicInventory();      // do cyclic inventory if necessary
        
        total_tags_momento = total_tags;

            for (x = 0; x < MAXTAG; x++ )
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
            liga_led_tag ();
            x = y = 0;
            //limpa_display();
            poe_texto_XY (2,0, vinte_espacos);
            poe_texto_XY (3,0, vinte_espacos);
            poe_texto_XY (4,0, vinte_espacos);
           
            poe_texto_XY (1,18,"  ");
            poe_dado_XY (1,18,conta_tags_atuais);
            
            for (x = 0;x < MAXTAG; x++)
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
            desliga_led_tag();
            poe_texto_XY (1,18,"  ");
            poe_dado_XY (1,18,total_tags);
            //desliga_buzzer();
        }
#endif
#endif

#ifdef AMBEV
        int tags_atual = 0;
        //conta_giro = 8;
        atualiza_tags = 1;
        limpa_buffer_tags ();
        pot_ret_antena[1] = 61000;
        pot_ret_antena[2] = 61000;
        pot_ret_antena[3] = 61000;
        pot_ret_antena[4] = 61000;
        pot_ret_antena[5] = 61000;
        while (situacao == 4)
        {
            //total_parcial = inventoryGen2();
            //troca_bytes();
            total_parcial = 0;
            delay_ms (100);
            if (total_parcial)
            {
                if(num_tag_usp[antena_atual] == tags_[0].epc[11])conta_tag_usp[antena_atual]++;
                else conta_tag_usp[antena_atual] = 1;
                dbm_tag_usp[antena_atual] = tags_[0].rssi;
                num_tag_usp[antena_atual] = tags_[0].epc[11];
                poe_texto_XY (antena_atual,0,"A :                 ");
                poe_dado_XY (antena_atual,1,antena_atual);
                poe_dado_XY (antena_atual,3,num_tag_usp[antena_atual]);
                //poe_texto_XY (antena_atual,7,"-");
                //calcula_dbfs(dbm_tag_usp[antena_atual]);
                calcula_dbm(dbm_tag_usp[antena_atual]);
                dbm_tag_real = 66 - dbm_tag_real;
                dbm_tag_usp[antena_atual] = dbm_tag_real;
                
                poe_dado_XY (antena_atual,8,dbm_tag_usp[antena_atual]);
                poe_texto_XY (antena_atual,10,"dbm");
                poe_texto_XY (antena_atual,14,"C:");
                poe_dado_XY (antena_atual,16,conta_tag_usp[antena_atual]);
                
                liga_led_tag();
            }
            else
            {
                conta_zero_tag_usp[antena_atual]++;
                if (conta_zero_tag_usp[antena_atual] == 20)
                {
                    conta_zero_tag_usp[antena_atual] = 0;
                    poe_texto_XY (antena_atual,0,"A :                 ");
                    poe_dado_XY (antena_atual,1,antena_atual);
                }
                desliga_led_tag();
            }
            pega_pot_refl ();
            poe_dado_XY(antena_atual,13,pot_refletida);
            poe_sentido();
            escolhe_antena();
            atualiza_tags = 1;
        }
        for (y=0;y < MAXTAG;y++)
        {
            conta_leituras_dbm[y] = 0;
            dbm_tag[y][0] = 0;
        }
        //poe_sentido();
        escolhe_antena();
        //for(conta_giro = 4;conta_giro > 0;conta_giro--)
        atualiza_conta_giro();
//#ifdef SANKYU
//        if (antena_atual == 5)situacao = 3;
//#endif
        qt_presentes = 0;
        while (conta_giro)
        {
            total_parcial = inventoryGen2();
            ProcessaGPRS ();
            troca_bytes();
            conta_giro--;
#ifdef SANKYU
                if (antena_atual == 6)
                {
                    trata_operador_sankyu();
                    break;
                }
#endif
            if (tem_pacote)
            {
                trata_pacote_serial();
                tem_pacote = 0;
            }
            if (total_parcial)
            {
                liga_led_tag();
                pega_dados_tags ();
                atualiza_conta_um();
                //mostra_todas_tags();
#ifndef SANKYU
#ifndef DUAS_ANTENAS_AMBEV
                if ((situacao == 1)||(situacao == 2))trata_antenas_laterais();
#endif
#endif
                trata_antenas_frente_re ();
                falta_antena[antena_atual] = 0;
                conta_area_livre=0;
                if (situacao != 3)limpa_buffer_tags ();
                qt_presentes++;//
                //liga_led_tag();
                if (qt_presentes >= minimo_leituras)break;
            }
            else desliga_led_tag();
        }
        //ProcessaGPRS ();
#ifdef DISPLAY
        poe_dado_XY(2,18,conta_registros);
        poe_dado_XY(2,15,conta_envio);
        poe_texto_XY(3,0,vinte_espacos);
        mostra_status_modem();
        //poe_dado_XY(3,0,pedestre[2]);
        //poe_dado_XY(3,3,pedestre[1]);
        //poe_dado_XY(3,6,pedestre[0]);
        //poe_dado_XY(3,0,estados_maquina_envio);
        //poe_dado_XY(3,3,estados_maquina_comandos);
        //poe_dado_XY(3,6,estados_maquina_acao_gps);
        //poe_dado_XY (2,18,time_out_portal[3]);
        /*
        poe_texto_XY(3,0,vinte_espacos);
        poe_dado_XY(3,4,portais[4][2]);
        poe_dado_XY(3,8,portais[4][1]);
        poe_dado_XY(3,12,portais[4][0]);
        poe_dado_XY (3,18,time_out_portal[4]);
        //
        poe_texto_XY(4,0,vinte_espacos);
        poe_dado_XY(4,4,operadores[12][2]);
        poe_dado_XY(4,8,operadores[12][1]);
        poe_dado_XY(4,12,operadores[12][0]);
        poe_dado_XY (4,18,time_out_operador[12]);
        */
        if (situacao == 1)poe_texto_XY (2,0,"Fr:  ");
        if (situacao == 2)poe_texto_XY (2,0,"Re:  ");
        if (situacao == 3)poe_texto_XY (2,0,"Pa:  ");
        poe_dado_XY(2,3,qt_presentes);
#endif
        //poe_texto_XY (3,0,"AD:");
        //if (falta_antena[antena_atual])poe_dado_XY(3,2+antena_atual,antena_atual);
        //else poe_texto_XY(3,2+antena_atual," ");
        //atualiza_conta_um();
        if (qt_presentes)
        {
            //if ((situacao == 1)||(situacao == 2))trata_antenas_laterais();
            poe_texto_operador();
            if ((antena_atual == 1)||(antena_atual == 2))conta_zero_tags = 0;
        }
        else
        {
            //tem_antena ();
            if (situacao == 3) conta_zero_tags++;
        }
        if ((situacao == 1) || (situacao == 2))
        {

            if(qt_presentes >=  minimo_leituras)
            {
                if ((antena_atual == 1)||(antena_atual == 2))mostra_rssi_tags();
                conta_zero_tags = 0;
            }
            else if (qt_presentes == 0)
            {
#ifdef SANKYU
                if ((antena_atual == 1)||(antena_atual == 2)||(antena_atual == 3)||(antena_atual == 4)||(antena_atual == 5))
#endif
#ifndef SANKYU
                 if ((antena_atual == 1)||(antena_atual == 2))
#endif
                {
                    conta_zero_tags++;
                    conta_zero_ze = 0;
                    tem_ze = 0;
                    if (conta_zero_tags > 2)
                    {
                        conta_zero_tags = 0;
                        //if (status_display != 4)
                        //{
                        desliga_sirene();
                        //desliga_bip();
                        status_display = 4;
                        atualiza_display_ambev();
                        //}
                    }
                 }
            }

        }

//#ifdef SANKYU
//        trata_operador_sankyu();
//#endif
#ifndef SANKYU
        if (situacao == 3)
        {
            desliga_sirene();
            ja_ligou = 0;
            if (conta_leituras_operador >= minimo_leituras)
            {
                conta_zero_tags = 0;
                //desliga_led_vm();
                conta_leituras_operador = 0;
                pode_operar = 1;
                operador_atual = operador_lido;
                libera_frente();
                libera_reh();
                desliga_led_vm();
                poe_texto_XY (1,0,autorizado);
                poe_dado_XY (1,2,operador_atual);
                //envia_meu_operador();
            }
            else if (conta_zero_tags > 10)
            {
#ifdef DISPLAY
                poe_texto_XY (1,0,desabilitado);
#endif
                bloqueia_reh();
                bloqueia_frente();
                //liga_led_vm ();
               
                operador_atual = 0;
                meu_operador[0] = 0;
                meu_operador[1] = 0;
                meu_operador[2] = 0;
                //envia_meu_operador();
                pode_operar = 0;
            }
            else if (muda_display)
            {
                limpa_display();
                //poe_texto_XY (2,0,"Pa:");
                poe_texto_operador();
                if(!pode_operar)
                {
                    poe_texto_XY (1,0,desabilitado);
                    bloqueia_reh();
                    bloqueia_frente();
                    //liga_led_vm();
                }
                else
                {
                    libera_reh();
                    libera_frente();
                    desliga_led_vm ();
                    poe_texto_operador();
                }
                muda_display = 0;
            }
        }
#endif
        poe_sentido();
#ifndef SANKYU
#ifndef DUAS_ANTENAS_AMBEV
        escolhe_antena();
#endif        
#endif
        atualiza_tags = 1;
#endif
    }
}



void tem_antena (void)
{
    if (antena_atual < 5)
    {
        pega_pot_refl ();
        //if (antena_atual == 4)poe_dado_XY(3,14,pot_refletida);
        //if (antena_atual == 4)poe_dado_XY(3,0,pot_ret_antena[antena_atual]);
        if ((pot_refletida > pot_ret_antena[antena_atual])||(pot_refletida < 1000))falta_antena[antena_atual]++;
        else if (falta_antena[antena_atual] > 0)falta_antena[antena_atual]--;
        if (falta_antena[antena_atual] > 5)falta_antena[antena_atual]=5;
    }
}

void poe_texto_operador(void)
{
    if (situacao == 3)poe_texto_XY (2,0,"Pa:");
    if (situacao == 2)poe_texto_XY (2,0,"Re:");
    if (situacao == 1)poe_texto_XY (2,0,"Fr:");
    if(pode_operar)
    {
        poe_texto_XY (1,0,autorizado);
        poe_dado_XY (1,2,operador_atual);
    }
}

void atualiza_conta_area_livre(void)
{
    if (situacao != 3)
    {
        if ( conta_area_livre < 65000)conta_area_livre++;
        if ( conta_area_livre > 5)poe_texto_XY (1,7,"ALF");
    }
}
void atualiza_conta_um(void)
{
    //conta_ausente_um[1]++;
    if (dbm_tag[1][0])
    {
        //poe_dado_XY (4,14,dbm_tag[1][0]);
        if (conta_presente_um[antena_atual] < 65000)conta_presente_um[antena_atual]++;
        conta_ausente_um[antena_atual] = 0;
    }
    else
    {
        if (conta_ausente_um[antena_atual] < 65000)conta_ausente_um[antena_atual]++;
        conta_presente_um[antena_atual] = 0;
        //if (antena_atual == 1)poe_dado_XY(3,0,conta_ausente_um[antena_atual]);
    }
    //poe_dado_XY (4,18,conta_ausente_um[1]);
}
void trata_antenas_laterais (void)
{
    if ((antena_atual == 3)||(antena_atual == 4))
    {
        if (conta_presente_um[antena_atual])
        {
            qt_presentes = 0 ;
        }
        else
        {
            ativa_alarme_sem_um ();
        }
    }
}

#ifdef SANKYU
void trata_antenas_frente_re (void)
{
    if (conta_ausente_um[antena_atual] > 3)
        {
          ativa_alarme_sem_um ();
        }
}
#endif

#ifndef SANKYU
void trata_antenas_frente_re (void)
{
    if ((antena_atual == 1)||(antena_atual == 2))
    {
        if (conta_ausente_um[antena_atual] > 3)
        {
          ativa_alarme_sem_um ();
        }
    }
}
#endif
void mostra_todas_tags(void)
{
    unsigned char todas_tags,qt_max_linha,proxima_coluna;
    //poe_texto_XY(4,0,vinte_espacos);
    qt_max_linha = 0;
    proxima_coluna = 0;
    //poe_texto_XY(4,0,vinte_espacos);
    //poe_texto_XY(3,antena_atual+3,"   ");
    //poe_dado_XY(3,0,status_display);
    //poe_dado_XY(3,antena_atual+3,conta_ausente_um[antena_atual]);
    for (todas_tags = 0; todas_tags < MAXTAG; todas_tags++)
        {
            if (dbm_tag[todas_tags][0])
            {
                if (qt_max_linha < 5)
                {
                    poe_dado_XY(4,proxima_coluna,todas_tags);
                    proxima_coluna += 3;
                    qt_max_linha++;
                }
            }
        }
}

void ativa_alarme_sem_um (void)
{
    int todas_tags,tags_atuais,qt_tags_saiu,v5,vx,esperando,ky,y;
    if (conta_tags_validas)
    {
#ifdef SANKYU
        bloqueia_reh();
#endif
#ifndef SANKYU
         if (situacao == 1)bloqueia_frente();
            else bloqueia_reh();
#endif
            liga_led_vm ();
            //ja_ligou = 1;
            liga_sirene();
    }
    for (todas_tags = 2; todas_tags < MAXTAG; todas_tags++)
    {
        if (dbm_tag[todas_tags][0])
        {
            qt_presentes = 0 ;
#ifdef DISPLAY
            //poe_dado_XY(4,16,dbm_tag[todas_tags][0]);
            poe_texto_XY (1,6,"         ");
            if (situacao == 1)poe_texto_XY (1,6, "PF:");
            else poe_texto_XY (1,6, "PT:");
            poe_texto_XY (1,9,"  ");
            poe_dado_XY (1,9,todas_tags);
            //poe_dado_XY (1,9,conta_tags_validas);
            //poe_dado_XY (1,9,serie_tag[0][0]);
#endif
            primeiro_pedestre = todas_tags;
            //if (situacao == 1)bloqueia_frente();
            //else bloqueia_reh();
            //liga_led_vm ();
            //ja_ligou = 1;
            //liga_sirene();
            for(ky = 0;ky < conta_tags_validas; ky++)
            {
                if (todas_tags == serie_tag[ky][0])
                {
                    pedestre[0] = serie_tag[ky][0];
                    pedestre[1] = serie_tag[ky][1];
                    pedestre[2] = serie_tag[ky][2];
                    rssi = dbm_tag[ky][0];
                    registra_evento();
                    //conta_registros++;
                }
            }
            //registra_evento();
            //liga_sirene();
            //delay_ms(2); /* */ desliga_sirene(); delay_ms(200); liga_sirene(); delay_ms(2); desliga_sirene();
            delay_ms(100);
            desliga_sirene();
            qt_tags_saiu= 0;
            esperando = 1;
            while (qt_tags_saiu < 50)
            {
                limpa_buffer_tags();
                tags_atuais = inventoryGen2();
                troca_bytes();
                le_sentido();
                if (situacao_anterior != situacao)
                {
                    desliga_led_vm();
                    desliga_sirene();
                    situacao_mudou();
                    escolhe_antena();
#ifdef SANKYU
                    libera_reh();
#endif
#ifndef SANKYU
                    if (situacao == 1)libera_frente();
                    else if (situacao == 2)libera_reh();
                    else
                    {
                        libera_frente();
                        libera_reh();
                        //qt_tags_saiu = 50;
                    }
#endif
                    tags_atuais = 0;
                    qt_tags_saiu = 51;
                }
                if (tags_atuais)
                {
                    //z = 0;
                    liga_led_tag();
                    //poe_dado_XY (1,9,tags_[0].epc[11]);
                    //poe_dado_XY (1,13,tags_atuais);
                    v5 = tags_atuais;
                    for (vx = 0; vx < v5; vx++)
                    {                      //11                                       
                        if ((tags_[vx].epc[11] == 1) && (tags_[vx].epc[10] == 0)&& (tags_[vx].epc[9] == 0))
                        {
                            conta_presente_um[antena_atual]++;
                            tags_atuais = 0;
                            break;
                        }
                        else
                        {
                            pont_tag_atual = vx;
                            conta_ausente_um[antena_atual]++;
                            trata_tags_excessao();
                            if (tag_valida == 0)tags_atuais--;
                            
                        }
                    }
                    if(tags_atuais == 0)qt_tags_saiu++;
                    else qt_tags_saiu = 0;
                }
                if(tags_atuais == 0)
                {
                    desliga_led_tag();
                    qt_tags_saiu++;
                }
                esperando++;
                //poe_dado_XY (1,16,esperando);
            }
            delay_ms(2000);
            limpa_buffer_tags();
            qt_presentes = 0;
            for (y=0;y < MAXTAG;y++)
            {
                conta_leituras_dbm[y] = 0;
                dbm_tag[y][0] = 0;
            }
            /*for (vx = 0; vx < MAXTAG; vx++)
            {
                dbm_tag[vx][0] = 0;
            }*/
            //poe_texto_XY (1,15,"     ");
#ifdef SANKYU
            libera_reh();
#endif
#ifndef SANKYU
            if (situacao == 1)libera_frente();
            else if(situacao == 2)libera_reh();
            else
            {
                libera_frente();
                libera_reh();
            }
#endif
            desliga_led_vm();
#ifdef DISPLAY
            poe_texto_XY (1,6,"         ");
            if (situacao == 1)poe_texto_XY (1,6, "ALF");
            else poe_texto_XY (1,6, "ALR");
#endif
            todas_tags = MAXTAG;
            //break;
        }
    }
}

/*
void ve_tag_valida (int y)
{
    int z;
    for (z = 0; z < 5; z++)
    {
        if (tags_entrou_corredor[z] == tags_[0].epc[11])y--;
    }
}
void trata_pacote_portal(void)
{
    unsigned char x,y,z,k,ja_entrou,ja_saiu = 0;
    if (tem_pacote)
    {
        if (pacote_zerado == 1)
        {

            pacote_zerado = 0;
            //
            //for (x=0;x < 5;x++)
            //{
            //    tags_entrou_corredor[x] = 0;
            //    tags_saiu_corredor[x] = 0;
            //    //qt_tags_saiu = 0;
            //    //qt_tags_entrou = 0;
            //}
        }
        else
        {
            //qt_tags_saiu = 0;
            //qt_tags_entrou = 0;
            y=3;
            for (x=0;x < qt_tags_portal;x++)
            {
                if ( pacote_chegou[y+2] > 127)
                {
                    ja_entrou = 0;
                    for (k=0;k < 5;k++)
                    {
                        if(tags_entrou_corredor[k] == pacote_chegou[y])ja_entrou = 1;
                    }
                    if (ja_entrou == 0)
                    {
                        for (z=0;z < 5;z++)
                        {

                            if (tags_entrou_corredor[z]== 0)
                            {
                                tags_entrou_corredor[z]=pacote_chegou[y];
                                //qt_tags_entrou++;
                                break;
                            }
                        }
                        for (z=0;z < 5;z++)
                        {
                            if (tags_saiu_corredor[z]== pacote_chegou[y])
                            {
                                tags_saiu_corredor[z]=0;
                            }
                        }
                    }
                }
                else
                {
                    ja_saiu = 0;
                    for (k=0;k < 5;k++)
                    {
                        if(tags_saiu_corredor[k] == pacote_chegou[y])ja_saiu = 1;
                    }
                    if (ja_saiu == 0)
                    {
                        for (k=0;k < 5;k++)
                        {
                            if (tags_saiu_corredor[k] == 0)
                            {
                                tags_saiu_corredor[k] = pacote_chegou[y];
                                //qt_tags_saiu++;
                                break;
                            }
                        }
                        for (z=0;z < 5;z++)
                        {
                            if (tags_entrou_corredor[z] == pacote_chegou[y])
                            {
                                tags_entrou_corredor[z]=0;
                            }

                        }
                    }
                }
                y += 3;
            }

        }
        //tem_pacote = 0;
    }
}
void mostra_tags_corredor(void)
{
    unsigned char x,y;
    //limpa_display();
    poe_texto_XY(4,0,vinte_espacos);
    poe_texto_XY(4,0,"E:");
    qt_tags_entrou = 0;
    //poe_dado_XY (3,2,qt_tags_entrou);
    poe_texto_XY(4,4,"-");
    //if (qt_tags_entrou)
    //{
        y=5;
        for (x=0;x < 5;x++)
        {
            if (tags_entrou_corredor[x])
            {
                poe_dado_XY(3,y,tags_entrou_corredor[x]);
                y+=3;
                qt_tags_entrou++;
            }
        }
    //}
    poe_dado_XY (3,2,qt_tags_entrou);

}*/
/*
void lista_tags (unsigned char linha, unsigned char coluna,unsigned char limite,unsigned char qt_tags, char *tags)
{
    if (qt_tags == 0)return;
    int x;
    int y;
    for (x=0; x < MAXTAG ;x++)
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
    for (x=0; x < MAXTAG ;x++)
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
    int x,ky = 0;
    int conta_leituras_ze = 0;
    conta_tags = 0;
    media_dbm = 0;
    //int coluna = 0;
    media_dbm_ze = 0;
    media_dbm_tunel = 0;
    int operarios_atual = 0;
    conta_mostra++;

    for (x = 0; x < MAXTAG; x++)
    {
        if (dbm_tag[x][0])
        {
            if (x == 1)
            {
                if (conta_leituras_dbm[1])
                {
                    media_dbm_ze = dbm_tag[1][0]/conta_leituras_dbm[1];
                    conta_leituras_ze = conta_leituras_dbm[1];
                    conta_leituras_ze +=1;
                    tem_ze = 1;
                    conta_zero_ze = 0;
                }
            }
            else if (media_dbm_ze)
            {
                if (conta_leituras_dbm[x] > conta_leituras_ze )
                {
                    media_dbm = dbm_tag[x][0]/conta_leituras_dbm[x];
                    if (media_dbm <= media_dbm_ze )operarios_atual++;
                    else
                    {
                        primeiro_pedestre = x;
                        conta_tags++;
                            for(ky = 0;ky < conta_tags_validas; ky++)
                            {
                                if (x == serie_tag[ky][0])
                                {
                                    pedestre[0] = serie_tag[ky][0];
                                    pedestre[1] = serie_tag[ky][1];
                                    pedestre[2] = serie_tag[ky][2];
                                    rssi = dbm_tag[ky][0];
                                    //registra_evento();
                                }
                            }
                    }
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

            //dbm_tag[x][0] = 0;
            //conta_leituras_dbm[x] = 0;
            //acg_tag_atual[x] = 0;
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
    if (media_dbm_ze)
    {
        if (conta_tags == 0)
        {
            libera_frente();
            libera_reh();
            desliga_led_vm();
            desliga_sirene();
        }
        //poe_dado_XY (2,14,operarios);
        if (status_display != 2)
        {
                conta_troca_status = 0;
                desliga_sirene();

                //liga_bip_curto_longo();
                status_display = 2;
                atualiza_display_ambev();
        }

    }
    else if (status_display != 4)
        {
                conta_troca_status = 0;
                desliga_sirene();

                //liga_bip_curto_longo();
                status_display = 4;
                atualiza_display_ambev();
        }

 }

void atualiza_display_ambev(void)
{
    switch (status_display)
    {
            case 1:
#ifdef DISPLAY
                    poe_texto_XY (1,6,"         ");
                    if (situacao == 1)poe_texto_XY (1,6, "PF:");
                    else poe_texto_XY (1,6, "PT:");
                    //poe_dado_XY (1,8, primeiro_pedestre);
                    //poe_texto_XY (2,7, "Pare!!!");
#endif
                    if (situacao == 1)bloqueia_frente();
                    else bloqueia_reh();
                    liga_led_vm ();
                    //if (ja_ligou == 0)
                    //{
                        ja_ligou = 1;
                        registra_evento();
                        liga_sirene();
                        delay_ms(250);
                    //}
                    desliga_sirene();

            break;

            case 2:
#ifdef DISPLAY
                    poe_texto_XY (1,5,"         ");
                    if (situacao == 1)poe_texto_XY (1,6, "ZEF  ");
                    else poe_texto_XY (1,6, "ZET  ");
                    //poe_texto_XY (2,6, "Cuidado!!");
                    //desliga_saida_4();
#endif
            break;

            case 4:
                    //limpa_display();
                if (pode_operar)
                {
                    libera_frente();
                    libera_reh();
                    desliga_led_vm();
                }
                else
                {
                    bloqueia_frente();
                    bloqueia_reh();
                    liga_led_vm();
                }
                    //desliga_led_vm();
                    desliga_sirene();
                    ja_ligou = 0;
#ifdef DISPLAY
                    poe_texto_XY (1,5,"        ");
                    if (situacao == 1)poe_texto_XY (1,6,"ALF");
                    else poe_texto_XY (1,6,"ALR");
#endif
            break;

          }
    /*
    poe_texto_XY (4,0,"Op.:");
    if (operador_atual == 0x19)poe_texto_XY (4,4,gabriel);
    if (operador_atual == 0x18)poe_texto_XY (4,4,pedro);
    if (operador_atual == 0x20)poe_texto_XY (4,4,ruy);
    if (operador_atual == 0x07)poe_texto_XY (4,4,"Desc.");
    */
}

/*
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
        //tag_atual_d[2] = 0;
        tag_atual_e[1] = 0;
        //tag_atual_e[2] = 0;
        for (x = 2; x < MAXTAG; x++)
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
 
}*/

#ifndef TEM_RASP
void troca_bytes(void)
{
    int x,y;
    for (x=0;x < MAXTAG;x++)
    {
        if (tags_[x].epc[0] == 0x50 || tags_[x].epc[0] == 0x30)      //somente o marcador de zona de tem o byte 11 em 1
        {                          // ou nossas tags, empilhadeira tem no. de serie gigante
            tags_[x].epc[11]=tags_[x].epc[2];       //vamos trocar os bytes
            tags_[x].epc[10]=tags_[x].epc[1];       //para ficar MSB.MID.LSB
            tags_[x].epc[9]=tags_[x].epc[0];        //empilhadeira = 0.1.2...
            //tags_[x].epc[0] = 0;                    //             50.00.44
            //tags_[x].epc[1] = 0;                    //normal = 0.1.2...9.10.11
            //tags_[x].epc[2] = 0;                    //                00.00.01
        }
        /*
#ifdef EMPILHADEIRA     //empilhadeira sÃƒÆ’Ã‚Â³ aceita tag 1 ou tag com comeÃƒÆ’Ã‚Â§o 0x50 ou 0x30
        else if ((tags_[x].epc[11] == 1) && (tags_[x].epc[10] == 0) && (tags_[x].epc[9] == 0))//ve se eh ZE
            return;
        else
        {
            tags_[x].epc[11] = 0;
            tags_[x].epc[10] = 0;
            tags_[x].epc[9] = 0;
        }
#endif*/
    }
}

void pega_dados_tags (void)
{
    int x =0;
    //numero_tag = 0;
    int dbm_tag_atual = 0;
    int rssi_I = 0;
    int rssi_Q = 0;
    conta_tags_validas = 0;
    //pont_tag_atual = 0;

    for (x = 0; x < MAXTAG; x++)
    {
        pont_tag_atual = x;
        if (tags_[x].epc[11])//11
        {
            /*
            poe_texto_XY (4,0,vinte_espacos);
            poe_dado_XY (4,0,tags_[x].epc[9]);
            poe_dado_XY (4,3,tags_[x].epc[10]);
            poe_dado_XY (4,6,tags_[x].epc[11]);
            */
            if ((tags_[x].epc[11] == 1) && (situacao != 3))   //verifica tags de inicio com 1 (ZE)
            {               //11                        //9
                if ((tags_[x].epc[10]) || (tags_[x].epc[9]))numero_tag = MAXTAG;
                else numero_tag = tags_[x].epc[11];//11
            }
            else numero_tag = tags_[x].epc[11];//11
            numero_tag &= MAXTAG;
            //poe_texto_XY (4,0,vinte_espacos);
            //poe_dado_XY (4,17,numero_tag);
            if (situacao == 3)
            {
                if (tags_[x].epc[9] == 0x50)//9
                {
                    operador_lido = numero_tag;
                    conta_leituras_operador++;
                    meu_operador[2]=tags_[x].epc[9];
                    meu_operador[1]=tags_[x].epc[10];
                    meu_operador[0]=tags_[x].epc[11];
                    return;
                }
            }
            tag_valida = 1;
            trata_tags_excessao();
            if (tag_valida == 1)
            {
                dbm_tag_atual = tags_[x].rssi;
                rssi_I = dbm_tag_atual;
                rssi_I &= 0x0F;
                dbm_tag_atual &= 0xF0;
                rssi_Q = dbm_tag_atual >> 4;
                dbm_tag_atual = rssi_Q + rssi_I;
                dbm_tag[numero_tag][0] = dbm_tag[numero_tag][0] + dbm_tag_atual;
                //tag_valida = 1;
                //trata_tags_excessao();
                conta_leituras_dbm[numero_tag]++;
                serie_tag[conta_tags_validas][2] = tags_[x].epc[9];//9
                serie_tag[conta_tags_validas][1] = tags_[x].epc[10];
                serie_tag[conta_tags_validas][0] = tags_[x].epc[11];//11
                conta_tags_validas++;
            }
            //if (numero_tag == operador_atual)dbm_tag[numero_tag][0] = 0;
            //else if (numero_tag == operador_atual_outro)dbm_tag[numero_tag][0] = 0;
            //conta_leituras_dbm[numero_tag]++;
            //poe_dado_XY (4,17,dbm_tag[numero_tag][0]);
        }
    }
}


void trata_tags_excessao(void)
{
    u8 x,y,z;
    tag_valida = 1;
    if((meu_operador[2]==tags_[pont_tag_atual].epc[9])
     &&(meu_operador[1]==tags_[pont_tag_atual].epc[10])
     &&(meu_operador[0]==tags_[pont_tag_atual].epc[11]))//primeiro vamos ver se eh nosso operador
     {
        tags_[pont_tag_atual].epc[9]=0;
        tags_[pont_tag_atual].epc[10]=0;
        tags_[pont_tag_atual].epc[11]=0;
        tag_valida = 0;
     }
    else if(qt_operadores)                  //se nÃƒÆ’Ã‚Â£o for, ver de outras empilhadeiras
    {
       for (x= 0; x < 30;x++)
       {
           if ( tem_operador[x])
           {
                if ((operadores[x][2] == tags_[pont_tag_atual].epc[9])
                &&(operadores[x][1] == tags_[pont_tag_atual].epc[10])
                &&(operadores[x][0] == tags_[pont_tag_atual].epc[11]))
                 {
                    tags_[pont_tag_atual].epc[9]=0;
                    tags_[pont_tag_atual].epc[10]=0;
                    tags_[pont_tag_atual].epc[11]=0;
                    tag_valida = 0;
                    return;
                 }
           }
       }
    }
    //if(total_dos_portais)              //ve se entrou em algum portal
    //{
        for (x = 0; x < 11; x++)
        {
            if (qt_entrou[x])
            {
                for (y = 0; y < qt_entrou[x]; y++)
                {
                    z = y * 3;
                    if ((tags_[pont_tag_atual].epc[9]== portais[x][z+2])
                    &&(tags_[pont_tag_atual].epc[10]==portais[x][z+1])
                    &&(tags_[pont_tag_atual].epc[11]==portais[x][z]))
                    {
                        tags_[pont_tag_atual].epc[9]=0;
                        tags_[pont_tag_atual].epc[10]=0;
                        tags_[pont_tag_atual].epc[11]=0;
                        tag_valida = 0;
                        return;
                    }
                }
            }
        }
    //}
}
void trata_pacote_serial(void)
{
    unsigned int m,x,y,z,qt_anterior,k = 0;
    if (tem_pacote)
    {
        desliga_led_zig();
        cs_pacote = 0;
        if (no_serie < 11) //no. de serie ate 10 ÃƒÆ’Ã‚Â© portal
        {
            time_out_portal[no_serie]= 20;
            if (pacote_zerado == 1)
            {
                pacote_zerado = 0;
                return;
            }
            qt_anterior = qt_entrou[no_serie];
            qt_entrou[no_serie] += qt_tags_portal;
            //total_dos_portais += qt_tags_portal;
            //poe_dado_XY(2,10,qt_tags_portal);
            //poe_dado_XY(2,12,no_serie);
            //poe_texto_XY(3,0,vinte_espacos);
            //poe_texto_XY(4,0,vinte_espacos);
            y = 3;
            z = 0;
            qt_anterior*=3;
            for (x = 0; x < qt_tags_portal; x++)
            {
                if (pacote_chegou[y+2] >= 128)
                {
                    m = 0;
                    pacote_chegou[y+2] &= 127;
                    for (k = 0;k < 100;k++)
                    {
                        if((portais[no_serie][m+2] == pacote_chegou[y+2])
                        &&(portais[no_serie][m+1] == pacote_chegou[y+1])
                        &&(portais[no_serie][m] == pacote_chegou[y]))
                        {
                            //poe_dado_XY(3,0,pacote_chegou[y+2]);
                            //poe_dado_XY(3,3,pacote_chegou[y+1]);
                            //poe_dado_XY(3,6,pacote_chegou[y]);
                            portais[no_serie][m+2] = 0;
                            portais[no_serie][m+1] = 0;
                            portais[no_serie][m] = 0;
                            qt_entrou[no_serie]--;
                            total_dos_portais--;
                        }
                        m+=3;
                    }
                    
                    //qt_tags_portal--;
                }
                else
                {
                    //poe_dado_XY(4,0,pacote_chegou[y+2]);
                    //poe_dado_XY(4,3,pacote_chegou[y+1]);
                    //poe_dado_XY(4,6,pacote_chegou[y]);
                    portais[no_serie][qt_anterior+2] = pacote_chegou[y+2];
                    portais[no_serie][qt_anterior+1] = pacote_chegou[y+1];
                    portais[no_serie][qt_anterior] = pacote_chegou[y];
                    z += 3;
                    y += 3;
                    qt_anterior+=3;
                }
            }
            //qt_entrou[no_serie] += qt_tags_portal;
            //total_dos_portais += qt_tags_portal;
        }
        else                    //maior que 10 eh empilhadeira
        {
            time_out_operador[no_serie]=5;
            operadores[no_serie][0] = pacote_chegou[5];
            operadores[no_serie][1] = pacote_chegou[4];
            operadores[no_serie][2] = pacote_chegou[3];
            qt_operadores = 1;
            tem_operador[no_serie] = 1;
            //poe_texto_XY(4,0,vinte_espacos);
            //poe_dado_XY(4,0,no_serie);
            //poe_dado_XY(4,4,operadores[no_serie][2]);
            //poe_dado_XY(4,8,operadores[no_serie][1]);
            //poe_dado_XY(4,12,operadores[no_serie][0]);
        }
    }

}
/*
void trata_tags_corredor (void)
{
    unsigned char z = 0;
    for (z=0;z < 5;z++)
    {
        if (tags_entrou_corredor[z] == numero_tag)
        {
            dbm_tag[numero_tag][0] = 0;
        }
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
void lista_freq_anatel(void)
{
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
}

#ifdef ANATEL

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
#ifdef SANKYU
void atualiza_conta_giro(void)
{
        if ((antena_atual == 1)||(antena_atual == 2)||(antena_atual == 5))
        {
            conta_giro = 10;//1
            minimo_leituras = 1;//5
        }
        if (antena_atual == 6)
        {
            conta_giro = 1;
            minimo_leituras = 1;
        }
        if ((antena_atual == 3)||(antena_atual == 4))
        {
            conta_giro = 1;//10
            minimo_leituras = 1;//5
        }
}
#endif

#ifndef SANKYU
void atualiza_conta_giro(void)
{
    if ((antena_atual == 1)||(antena_atual == 2))
        {
#ifdef DUAS_ANTENAS_AMBEV
            conta_giro = 4;
            minimo_leituras = 1;
#else
            conta_giro = 18;
            minimo_leituras = 8;
#endif 
        }
        if (antena_atual == 5)
        {
            conta_giro = 10;
            minimo_leituras = 3;
        }
        if ((antena_atual == 3)||(antena_atual == 4))
        {
#ifdef DUAS_ANTENAS_AMBEV
            conta_giro = 4;
            minimo_leituras = 1;
#else
            conta_giro = 1;
            minimo_leituras = 1;
#endif
        }
}
#endif

#ifdef SANKYU
void escolhe_antena(void)
{
    if (pode_operar == 0)
    {
        antena_atual=6;
        sel_led (0,0);
        sel_antena(antena_atual);
        return;
    }
        
    if (situacao == 1)
    {
        
        if (antena_atual == 2)antena_atual = 3;
        //else if (antena_atual == 2)antena_atual = 5;
        //else if (antena_atual == 5)antena_atual = 1;
        else if (antena_atual == 3)antena_atual = 4;
        else if (antena_atual == 4)antena_atual = 5;
        else if (antena_atual == 5)antena_atual = 2;
        else antena_atual = 2;
        //if (antena_atual == 1)antena_atual = 5;
        //else antena_atual = 1;
    }
    else if (situacao == 2)
    {
        
        if (antena_atual == 1)antena_atual = 3;
        else if (antena_atual == 3)antena_atual = 4;
        else if (antena_atual == 4)antena_atual = 1;
        else antena_atual = 1;
    }
    else antena_atual = 1;
    sel_led (0,0);
    sel_antena(antena_atual);
}
#endif

#ifndef SANKYU
void escolhe_antena(void)
{
    //delay_ms(10);
    if (situacao != 4)
    {
        if (situacao == 1)
        {
#ifdef DUAS_ANTENAS_AMBEV
            if(antena_atual == 1)antena_atual = 3;
            else if (antena_atual == 3)antena_atual = 1;
            //else if (antena_atual == 4)antena_atual = 3;
            else antena_atual = 1;
#else
            if(antena_atual == 1)antena_atual = 4;        
            else if (antena_atual == 3)antena_atual = 1;
            else if (antena_atual == 4)antena_atual = 3;
            else antena_atual = 1;
#endif
        }
        else if (situacao == 2)
        {
#ifdef DUAS_ANTENAS_AMBEV
           if(antena_atual == 2)antena_atual = 4;
           else if (antena_atual == 4) antena_atual = 2;
           //else if (antena_atual == 4) antena_atual = 3;
           else antena_atual = 2;
#else
           if(antena_atual == 2)antena_atual = 4;
           else if (antena_atual == 3) antena_atual = 2;
           else if (antena_atual == 4) antena_atual = 3;
           else antena_atual = 2;
#endif
        }
        else if (situacao == 3)
            {
                //if (antena_atual != 5)tunerInit_A5(&mainTuner);
                antena_atual = 5;
            }

        else antena_atual = 1;
    }
    else if (situacao == 4)
    {
        antena_atual++;
        if (antena_atual > 4)antena_atual=1;
    }
    sel_led (0,0);
    sel_antena(antena_atual);
#ifdef DUAS_ANTENAS_AMBEV
    if (antena_atual == 1)tunerInit_A1(&mainTuner);
    else if (antena_atual == 2)tunerInit_A2(&mainTuner);
    else if (antena_atual == 3)tunerInit_A1(&mainTuner);
    else if (antena_atual == 4)tunerInit_A2(&mainTuner);
    else if (antena_atual == 5)tunerInit_A5(&mainTuner);
#else    
    if (antena_atual == 1)tunerInit_A1(&mainTuner);
    else if (antena_atual == 2)tunerInit_A2(&mainTuner);
    else if (antena_atual == 3)tunerInit_A3(&mainTuner);
    else if (antena_atual == 4)tunerInit_A4(&mainTuner);
    else if (antena_atual == 5)tunerInit_A5(&mainTuner);
#endif
}
#endif

void situacao_mudou(void)
{
    if (situacao_anterior != situacao)
    {
        //if (situacao == 3)tunerInit_A5(&mainTuner);
        //else tunerInit(&mainTuner);
        dbm_tag[1][0] = 0;
        conta_leituras_dbm[1] = 0;
        media_dbm_ze = 0;
        conta_zero_ze = 0;
        tem_ze = 0;

        desliga_buzzer();
        operarios = 0;
  
        status_display = 0;
       
        if(situacao == 3)limpa_display();
        if(situacao == 4)limpa_display();
        if(situacao_anterior == 4)limpa_display();
        muda_display = 1;
        situacao_anterior = situacao;
    }
}

void poe_sentido(void)
{
    le_sentido();
    situacao_mudou();
}

#ifdef SANKYU
void le_sentido(void)
{
    //if (_RG6 == 0)situacao = 2;
    if ((_RG6 == 0)&&(pode_operar))situacao = 2;
    else situacao = 1;
}
#endif

#ifndef SANKYU
void le_sentido(void)
{
    if ((_RG6 == 0)&&(_RG7 == 1)&&(pode_operar == 1) )situacao = 1;
    else if ((_RG6 == 1)&&(_RG7 == 0)&&(pode_operar == 1))situacao = 2;
    else if ((_RG6 == 1)&&(_RG7 == 1))situacao = 3;
    else if ((_RG6 == 0)&&(_RG7 == 0))situacao = 4;
    //else situacao = 1;
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

void envia_meu_operador(void)
{
    int x,y;
    if ( stpegapac > 0)
    {
        conta_tentativas_tx++;
        if (conta_tentativas_tx > 10)
        {
            conta_tentativas_tx = 0;
            stpegapac = 0;
        }
        return;
    }
    //desliga_led_zig();
    //if ( total_dos_portais > 0)
    //{
        for (x = 0; x < 10;x++)
        {
            if (time_out_portal[x] > 0)
            {
                time_out_portal[x]--;
                if (time_out_portal[x] == 0)
                {
                    for (y=0;y<300;y++)
                    {
                        portais[x][y] = 0;
                    }
                    qt_entrou[x]=0;
                }
            }
       // }
    }
    if (qt_operadores > 0)
    {
        for (x = 0; x < 30;x++)
        {
            if (time_out_operador[x] > 0)
            {
                time_out_operador[x]--;
                if (time_out_operador[x] == 0)
                {
                    operadores[x][0] = 0;
                    operadores[x][1] = 0;
                    operadores[x][2] = 0;
                    tem_operador[x]=0;
                }
            }
        }
    }
  
    if (pode_operar)
    {
        liga_led_zig();
        //if (status_zig == 0)liga_led_zig();
        //else desliga_led_zig();
        x = 0xF0;
        uart4Tx(x);   //start byte
        delay_us (120);
        uart4Tx(7);      //quantidade de bytes
        x += 7;
        delay_us (120);
        uart4Tx(NUM_SERIE);   //quem eu sou
        x += NUM_SERIE;
        delay_us (120);
        uart4Tx(meu_operador[2]);   //meu operador
        x += meu_operador[2];
        delay_us (120);
        uart4Tx(meu_operador[1]);
        x += meu_operador[1];
        delay_us (120);
        uart4Tx(meu_operador[0]);
        x += meu_operador[0];
        delay_us (120);
        uart4Tx(x);             //cs
    }
}


#ifdef PORTAL_AMBEV
void manda_tags(void)
{
    unsigned long soma = 0;
    u8 xbee_envia[19];
    unsigned char mtr[5][3];
    unsigned char i, j, x, y, cnt, flag;

        if (status_zig == 0)liga_led_zig();
        else desliga_led_zig();
        dado_enviado++;

        for(i=0; i<5; i++)
            for(j=0; j<3; j++)
                mtr[i][j] = 0;

        //matr_entrou[0][0] = 3;
        //matr_entrou[1][0] = 9;
        //matr_saiu[0][0] = 7;
        //matr_saiu[1][0] = 28;

        cnt = 0;
        flag = 0;

        for(i=0; i<5; i++)
        {
            if(matr_entrou[i][0])
            {
                cnt++;

                for(j=0; j<3; j++)
                    mtr[i][j] = matr_entrou[i][j];

                mtr[i][2] |= 0x80;      //0x80 = 0b10000000
            }
        }

        for(i=0; i<5; i++)
        {
            if(matr_saiu[i][0])
            {
                cnt++;
                for(x=0; x<5; x++)
                {
                    if(mtr[x][0]);

                    else
                    {
                        for(j=0; j<3; j++)
                        mtr[x][j] = matr_saiu[i][j];
                        break;
                    }
                }
            }
        }

        cnt *= 3;
        cnt +=4;

        xbee_envia[0] = 0xF0;
        xbee_envia[1] = cnt;
        //xbee_envia[1] = 0x13;       //total de bytes 0x13 = 19
        xbee_envia[2] = NUM_SERIE;         //numero de serie

        for(i=3; i<cnt-2; i++)
        {
            for(x=0; x<5; x++)
            {
                if(mtr[x][0])
                {
                    flag = 1;
                    for(y=0; y<3; y++)
                    {
                        xbee_envia[i] = mtr[x][y];
                        i++;
                    }
                }
            }
            i--;
        }

        if(!flag)
        {
            xbee_envia[1] += 1;
            cnt++;
            xbee_envia[3] = 0;
        }

        //for(i=0; i<18; i++)
        for(i=0; i<cnt-1; i++)
            soma += xbee_envia[i];

        xbee_envia[cnt-1] = soma&0xFF;
        //xbee_envia[18] = soma&0xFF;
        //for(i=0; i<19; i++)
        for(i=0; i<cnt; i++)
        {
            uart4Tx(xbee_envia[i]);
            delay_us(120);
        }
}

void tick (void)
{
    //contadorParaEnviarArrayXbee = contadorParaEnviarArrayXbee + 1;
    contadorParaEnviarArrayXbee++;
    //if (contadorParaEnviarArrayXbee == conta_inatividade)
    if (conta_inatividade > 150)
    {
        conta_inatividade = 0;
        manda_tags();
        contadorParaEnviarArrayXbee = 0;
    }

}
#endif


void tick (void)
{
#ifdef VARAL
    OperacoesParaTickVaral();
    ProcessaEnvioDeTagsNoVaral();
#endif

#ifdef AMBEV
    OperacoesParaTickGprs();
    contadorParaEnviarArrayXbee++;
    if (contadorParaEnviarArrayXbee == 1000)
    {
        contadorParaEnviarArrayXbee = 0;
        envia_meu_operador();
    }
#endif 
}

/*
void SetaObstrucaoXbee (void){
    FlagObstrucaoXbee = 1;
}

void LimpaObstrucaoXbee (void){
    FlagObstrucaoXbee = 0;
}
 * */


/*
void LigaRX (void)
{
    RPINR18 = 0x1F07;
    IPC2 = 0x4000;
    //U1MODE = 0x8000;
    IEC0bits.U1RXIE = 1;
}
*/
#ifdef BUSCA_TAG
/*
void reseta_as3993(void)
{
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
}
*/


void tunerInit_cin_clen_cout(const TunerConfiguration *config)
{
    tunerSetTuning(config, cin, clen, cout);
}

void le_capacete(void)
{
    u16 x,rssi,y,conta_lido,conta_tentativa = 0;
    cin = 1;
    clen = 18;
    cout = 10;
    tunerInit_cin_clen_cout(&mainTuner);
    while (1)
    {
        delay_ms(50);
        total_tags = inventoryGen2();
        troca_bytes();
        conta_tentativa++;
         if (total_tags)
         {

                    if ((tags_[0].epc[11] == 0x95)
                         && (tags_[0].epc[10] == 0x01)
                         && (tags_[0].epc[9] == 0x30))
                    {
                        conta_lido++;
                        liga_led_tag();
                        y = tags_[0].rssi & 0x0F;
                        tags_[0].rssi &= 0xf0;
                        rssi = tags_[0].rssi >> 4;
                        rssi += y;
                        poe_texto_XY(3,0,vinte_espacos);
                        poe_dado_XY(3,10,total_tags);
                        poe_texto_XY(3,0,"RSSI:");
                        poe_dado_XY (3,5,rssi);
                        poe_texto_XY(4,0,vinte_espacos);
                        poe_dado_XY (4,0,conta_tentativa);
                        poe_texto_XY(4,6,"=");
                        poe_dado_XY (4,7,conta_lido);
                    }
          }
         else  desliga_led_tag();
    }
}

void busca_tag (void)
{
    le_capacete();
    u16 x,rssi,y,conta_maior,conta_leituras,conta_tentativas = 0;
    poe_texto_XY(4,0,"Procurando...");
    pt_rssi[0] = 0;
    for (cin = 0; cin < 32; cin++)
    {
        for (clen = 0; clen < 32; clen+=2)
        {
             for (cout = 0; cout < 32; cout+=2)
            {
                tunerInit_cin_clen_cout(&mainTuner);
                delay_ms (100);
                rssi = 0;
                conta_leituras = 0;
                poe_texto_XY(1,0,vinte_espacos);
                poe_dado_XY(1,0,cin);
                poe_dado_XY(1,3,clen);
                poe_dado_XY(1,6,cout);
                poe_dado_XY(1,9,x);
                for(conta_tentativas = 0; conta_tentativas < 10;conta_tentativas++)
                {
                    //delay_ms (20);
                    //tunerInit_cin_clen_cout(&mainTuner);
                    total_tags = inventoryGen2();
                    troca_bytes();

                    if (total_tags)
                    {
                        poe_dado_XY(1,17,total_tags);
                        liga_led_tag();
                        if ((tags_[0].epc[11] == 0x95)
                         && (tags_[0].epc[10] == 0x01)
                         && (tags_[0].epc[9] == 0x30))
                        {
                            conta_leituras++;
                            y = tags_[0].rssi & 0x0F;
                            tags_[0].rssi &= 0xf0;
                            rssi += tags_[0].rssi >> 4;
                            rssi += y;
                            //poe_dado_XY(2,0, rssi);
                            x++;
                        }
                    }
                    else desliga_led_tag();
                }
                 if (pt_rssi[0] < rssi)
                 {
                    pt_rssi[0] = rssi;
                    pt_clen[0] = clen;
                    pt_cout[0] = cout;
                    pt_cin[0] = cin;
                    poe_texto_XY(2,0,vinte_espacos);
                    poe_dado_XY(2,0,pt_cin[0]);
                    poe_dado_XY(2,3,pt_clen[0]);
                    poe_dado_XY(2,6,pt_cout[0]);
                    //poe_texto_XY(2,10,"X:");
                    poe_dado_XY (2,9,conta_leituras);
                    poe_texto_XY(2,12,"RSSI:");
                    poe_dado_XY (2,17,pt_rssi[0]);
                    //le_capacete();
                 }
                 else if ((pt_rssi[0] == rssi) && (conta_leituras > conta_maior))
                 {
                    conta_maior = conta_leituras;
                    pt_rssi[0] = rssi;
                    pt_clen[0] = clen;
                    pt_cout[0] = cout;
                    pt_cin[0] = cin;
                    poe_texto_XY(2,0,vinte_espacos);
                    poe_dado_XY(2,0,pt_cin[0]);
                    poe_dado_XY(2,3,pt_clen[0]);
                    poe_dado_XY(2,6,pt_cout[0]);
                    //poe_texto_XY(2,10,"X:");
                    poe_dado_XY (2,9,conta_leituras);
                    poe_texto_XY(2,12,"RSSI:");
                    poe_dado_XY (2,17,pt_rssi[0]);
                    //le_capacete();
                }
            }
        }
    }

    if (x)
    {/*
        maior = pt_rssi[0];
        pt_maior = 0;
        for (y=1;y < x;y++)
        {
            if ( maior < pt_rssi[y])
            {
                maior = pt_rssi[y];
                pt_maior = y;
            }
        }
         poe_texto_XY(1,0,vinte_espacos);
         poe_texto_XY(4,0,vinte_espacos);
         poe_dado_XY(1,0,pt_cin[pt_maior]);
         poe_dado_XY(1,3,pt_clen[pt_maior]);
         poe_dado_XY(1,6,pt_cout[pt_maior]);
         poe_texto_XY(4,0,"X:");
         poe_dado_XY (4,3,x);
         poe_texto_XY(4,12,"RSSI:");
         poe_dado_XY (4,17,maior);*/
         while(1);
    }
    else poe_texto_XY(4,0,"nao achou nada....");
    while(1);
}
#endif
void LigaRX (void){
    RPINR18 = 0x1F07;//RX1
    RPINR19 = 0x0004;//RX2
    RPINR17 = 0x1800;//RX3
    RPINR27 = 0x001B;//RX0
    IPC2 = 0x4000;
    //U1MODE = 0x8000;

    IEC0bits.U1RXIE = 1;
    IEC1bits.U2RXIE = 1;
    IEC5bits.U3RXIE = 1;
    IEC5bits.U4RXIE = 1;
};
