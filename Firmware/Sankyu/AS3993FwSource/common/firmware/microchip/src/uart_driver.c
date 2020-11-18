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
/*
 *      PROJECT:   AS1130 MCU board firmware
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*! \file
 *
 *  \author Wolfgang Reichart 
 *
 *  \brief uart driver implementation for PIC24FJ64
 *
 */
/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
//#include <p24Fxxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include "p24FJ256DA210.h"
#include "errno.h"
#include "uart_driver.h"
#include "usb_hal_pic24.h"
#include "uart.h"
#include "global.h"
#ifdef UART_RECEIVE_ENABLED
#include "Compiler.h"
#include "string.h"
#include "auto_teste.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\gps.h"
#include "empilhadeira.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\zigbee.h"
#include "timer.h"
#include "C:\projetos\control-plus\Firmware\Sankyu\AS3993FwSource\AS3993\Firmware\src\wifi.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\eth.h"
#include "C:\Projetos\control-plus\software\fifo\gerenciaPacotes.h"
#include "C:\Projetos\control-plus\Firmware\BaseComum\portal.h"
#include "appl_commands.h"
#include "timer_driver.h"
#include "platform.h"
#include "setup_usb.h"
#include "FSM_DataHora.h"
#endif
/*
 *
 *
******************************************************************************
* LOCAL DEFINES
******************************************************************************
*/
s8 uartRxNBytes( u8 *, u16 *);
s8 uartRx2NBytes( u8 *, u16 *);
s8 uartRx3NBytes( u8 *, u16 * numBytes);

extern char ComandoObtidoParaAEmpilhadeira;
//extern char bufferRxEthernet[TAMANHO_BUFFER_RX_ETHERNET];

extern volatile int operador_atual_outro;
extern int conta_recebe_operador;
extern void desliga_led_zig(void);
extern void  liga_led_zig(void);
void uart4Tx(u8);
extern void poe_dado_XY(int,int,int);
extern u8 status_dir;
extern void desliga_dir();
extern void liga_dir();
extern void compararRespostaDoModemEmTempoReal(char *);

#ifdef UART_RECEIVE_ENABLED
//#define _U1RXInterrupt
#endif

extern unsigned char pacote_chegou[];
//extern unsigned char tags_entrou_corredor[];
//extern unsigned char tags_saiu_corredor[];
//extern unsigned char qt_tags_saiu;
//extern unsigned char qt_tags_entrou ;

extern unsigned char qt_pacote;
extern unsigned char cs_pacote;
extern unsigned char no_serie;
extern unsigned char pont_pac;
extern unsigned char qt_tags_portal;
extern unsigned char pacote_zerado;
extern unsigned char qt_pac_total;
extern u8 conta_tentativas_tx;
/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/
#ifdef UART_RECEIVE_ENABLED
static u8 rxBuffer[UART_RX_BUFFER_SIZE];
u8 __attribute__((far)) rx2Buffer[UART_RX_BUFFER_SIZE];
u8 __attribute__((far)) rx3Buffer[UART_RX_BUFFER_SIZE];
u8 __attribute__((far)) rx4Buffer[UART_RX_BUFFER_SIZE];

static u16 rxCount;
static u16 rx2Count;
static u16 rx3Count;
static u16 rx4Count;

static u8 rxError;
/*
static u8 rx2Error;
static u8 rx3Error;
static u8 rx4Error;
 * */

extern struct EstruturaProtocolo Teste;

#endif


//******************************************************************************
//* GLOBAL FUNCTIONS
//******************************************************************************

char teste[20];
int contadorDeTeste;


s8 uartTxInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 br1, br2;
    u16 breg;
    
    contadorDeTeste = 0;

    /* Disable UART for configuration */
    UART_WRITE_REG(MODE, 0x0);
    UART_WRITE_REG(STA, 0x0);

    /* Setup UART registers */
    /* equation according to the datasheet:
       (sysclk / (16 * baudrate)) - 1
     */
    breg = (sysclk / (16 * baudrate)) - 1;

    /* round up/down w/o using floating point maths */
    br1 = sysclk / (16 * (breg + 1));
    br2 = sysclk / (16 * (breg + 2));

    /* check which of the two values produce fewer error rate */
    if ((br1 - baudrate) > (baudrate - br2))
    {
        UART_WRITE_REG(BRG, breg + 1);
        if ( actbaudrate )
        {
            *actbaudrate = br2;
        }
    }
    else
    {
        UART_WRITE_REG(BRG, breg);
        if ( actbaudrate )
        {
            *actbaudrate = br1;
        }
    }

    //IEC4bits.U1ERIE = 1;//GILSON
    
    /* Enable UART */
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8000);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x0400);
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção ativado
    IEC0bits.U1RXIE = 1;

    return ERR_NONE;
}

#ifdef UART_RECEIVE_ENABLED
s8 uartRx1Initialize ()
{
    rxCount = 0;
    rxError = ERR_NONE;
    /* clear receive buffer */
    U1RX_Clear_Intr_Status_Bit;
    while ( DataRdyUART1() )
    {
        ReadUART1();
    }
    
    //IPC2BITS.U1RXIP = 7;
    
    return ERR_NONE;
}
#endif

s8 uartRx3Initialize(){
    rxCount = 0;
    rxError = ERR_NONE;
    /* clear receive buffer */
    U3RX_Clear_Intr_Status_Bit;
    while ( DataRdyUART3() )
    {
        ReadUART3();
    }
    //IPC20BITS.U3RXIP = 7;
    
    return ERR_NONE;
}

s8 uartRx2Initialize(){
    rxCount = 0;
    rxError = ERR_NONE;
    /* clear receive buffer */
    U2RX_Clear_Intr_Status_Bit;
    while ( DataRdyUART2() )
    {
        ReadUART2();
    }
    //IPC7BITS.U2RXIP = 1;

    return ERR_NONE;
}

s8 uartRx4Initialize(){
    rxCount = 0;
    rxError = ERR_NONE;
    /* clear receive buffer */
    U4RX_Clear_Intr_Status_Bit;
    while ( DataRdyUART4() )
    {
        ReadUART4();
    }
    //IPC20BITS.U3RXIP = 7;
    //IPC20BITS.U3RXIP = 7;

    return ERR_NONE;
}

/*
s8 uartInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 actbaud;
    s8 result = uartTxInitialize(sysclk, baudrate, &actbaud);
    *actbaudrate = actbaud;
#ifdef UART_RECEIVE_ENABLED
    if (result == ERR_NONE)
    {
        result = uartRxInitialize();
    }
#endif
    return result;
}
 * */


//s8 uartTxDeinitialize ()
//{
//    /* disable UART */
//    UART_WRITE_REG(MODE, 0x0);
//    UART_WRITE_REG(STA, 0x0);
//    U1STA = 0;
//
//    return ERR_NONE;
//}


//s8 uartDeinitialize ()
//{
//    return uartTxDeinitialize();
//}

s8 uart2TxInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 br1, br2;
    u16 breg;

    /* Disable UART for configuration */
    UART2_WRITE_REG(MODE, 0x0);
    UART2_WRITE_REG(STA, 0x0);

    /* Setup UART registers */
    /* equation according to the datasheet:
       (sysclk / (16 * baudrate)) - 1
     */
    breg = (sysclk / (16 * baudrate)) - 1;

    /* round up/down w/o using floating point maths */
    br1 = sysclk / (16 * (breg + 1));
    br2 = sysclk / (16 * (breg + 2));

    /* check which of the two values produce fewer error rate */
    if ((br1 - baudrate) > (baudrate - br2))
    {
        UART2_WRITE_REG(BRG, breg + 1);
        if ( actbaudrate )
        {
            *actbaudrate = br2;
        }
    }
    else
    {
        UART2_WRITE_REG(BRG, breg);
        if ( actbaudrate )
        {
            *actbaudrate = br1;
        }
    }
    
    //IEC4bits.U2ERIE = 1;//GILSON

    /* Enable UART */
    UART2_WRITE_REG(MODE, UART2_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART2_WRITE_REG(STA, UART2_READ_REG(STA) | 0x0400);
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção ativado
    IEC1bits.U2RXIE = 1;

    return ERR_NONE;
}

s8 uart3TxInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 br1, br2;
    u16 breg;

    /* Disable UART for configuration */
    UART3_WRITE_REG(MODE, 0x0);
    UART3_WRITE_REG(STA, 0x0);

    /* Setup UART registers */
    /* equation according to the datasheet:
       (sysclk / (16 * baudrate)) - 1
     */
    breg = (sysclk / (16 * baudrate)) - 1;

    /* round up/down w/o using floating point maths */
    br1 = sysclk / (16 * (breg + 1));
    br2 = sysclk / (16 * (breg + 2));

    /* check which of the two values produce fewer error rate */
    if ((br1 - baudrate) > (baudrate - br2))
    {
        UART3_WRITE_REG(BRG, breg + 1);
        if ( actbaudrate )
        {
            *actbaudrate = br2;
        }
    }
    else
    {
        UART3_WRITE_REG(BRG, breg);
        if ( actbaudrate )
        {
            *actbaudrate = br1;
        }
    }

    //IEC5bits.U3ERIE = 1;//GILSON

    /* Enable UART */
    UART3_WRITE_REG(MODE, UART3_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART3_WRITE_REG(STA, UART3_READ_REG(STA) | 0x0400);
    
    
    U3STAbits.URXISEL = 0; // modo de interrupção = 1 ou mais caracteres no buffer
    U3STAbits.ADDEN = 0; // detecção de endereço desabilitado
    //U3MODEbits.BRGH = 1; // high speed modo baud rate
    U3MODEbits.STSEL = 0; // 1 stop bit
    U3MODEbits.PDSEL = 0; // 8 bits de dados sem paridade
    U3MODEbits.RXINV = 0; // não inverter polaridade do rx
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção ativado
    IEC5bits.U3RXIE = 1; // habilita RX3
    

    return ERR_NONE;
}

s8 uart4TxInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 br1, br2;
    u16 breg;

    /* Disable UART for configuration */
    UART4_WRITE_REG(MODE, 0x0);
    UART4_WRITE_REG(STA, 0x0);

    /* Setup UART registers */
    /* equation according to the datasheet:
       (sysclk / (16 * baudrate)) - 1
     */
    breg = (sysclk / (16 * baudrate)) - 1;

    /* round up/down w/o using floating point maths */
    br1 = sysclk / (16 * (breg + 1));
    br2 = sysclk / (16 * (breg + 2));

    /* check which of the two values produce fewer error rate */
    if ((br1 - baudrate) > (baudrate - br2))
    {
        UART4_WRITE_REG(BRG, breg + 1);
        if ( actbaudrate )
        {
            *actbaudrate = br2;
        }
    }
    else
    {
        UART4_WRITE_REG(BRG, breg);
        if ( actbaudrate )
        {
            *actbaudrate = br1;
        }
    }
    /* Enable UART */
    UART4_WRITE_REG(MODE, UART4_READ_REG(MODE) | 0x8000);//8100
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART4_WRITE_REG(STA, UART4_READ_REG(STA) | 0x0400);
    //UART4_WRITE_REG(STA, UART4_READ_REG(STA) | 0x0400);
    //IEC0bits.U1RXIE = 1;
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção ativado
    IEC5bits.U4RXIE = 1;

    return ERR_NONE;
}

void uart1Tx (u8 dado){
    U1TXREG = dado;
    //while( (U1STA & 0x0100) );
    while( (U1STA & 0x0100) == 0);
}

void uart2Tx (u8 dado){
    if(debugInterfaceEthernet_Silent || debugInterfaceEthernet){
        //uart3Tx(dado);
    }
    U2TXREG = dado;
    //while( (U2STA & 0x0100) );
    while( (U2STA & 0x0200) );
}


void uart3Tx (u8 dado){
    U3TXREG = dado;
    //while( (U3STA & 0x0100) );
    while( (U3STA & 0x0200) );
}

void uart4Tx (u8 dado){
    U4TXREG = dado;
    //while( (U4STA & 0x0100) ){
    while( (U4STA & 0x0200) ){

    };
}


u16 uartRx2NumBytesAvailable ()
{
    u16 num;
    DisableIntU2RX;
    num = rx2Count;
    EnableIntU2RX;
    return num;
}


u16 uartRx3NumBytesAvailable ()
{
    u16 num;
    DisableIntU3RX;
    num = rx3Count;
    EnableIntU3RX;
    return num;
}

u16 uartRx4NumBytesAvailable ()
{
    u16 num;
    DisableIntU4RX;
    num = rx4Count;
    EnableIntU4RX;
    return num;
}


s8 uartRxNBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU4RX;
    if ( rxCount <= maxCopy )
    { /* copy whole received data into buffer */
	*numBytes = rxCount;
	memcpy( buffer, rxBuffer, rxCount );
	rxCount = 0; /* reset receive buffer */
    }
    else /* copy only the first *numBytes to buffer  */
    {
	memcpy( buffer, rxBuffer, maxCopy );
	rxCount -= maxCopy; /* remove the copied bytes from internal buffer */
	memmove( rxBuffer, rxBuffer + maxCopy, rxCount );
    }
    EnableIntU4RX;
    return ERR_NONE;
}

s8 uartRx2NBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU2RX;
    if ( rx2Count <= maxCopy )
    { /* copy whole received data into buffer */
	*numBytes = rx2Count;
	//memcpy( buffer, rx2Buffer, rx2Count );
	rx2Count = 0; /* reset receive buffer */
    }
    else /* copy only the first *numBytes to buffer  */
    {
	//memcpy( buffer, rx2Buffer, maxCopy );
	rx2Count -= maxCopy; /* remove the copied bytes from internal buffer */
	//memmove( rx2Buffer, rx2Buffer + maxCopy, rx2Count );
    }
    EnableIntU2RX;
    return ERR_NONE;
}

void LimpaBufferRX3 (void){
    //DisableIntU3RX;
    memset(rx3Buffer, 0, UART_RX_BUFFER_SIZE);
    rx3Count = 0; /* reset receive buffer */
    //EnableIntU3RX;

}

s8 uartRx3NBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU3RX;
    if ( rx3Count <= maxCopy )
    { /* copy whole received data into buffer */
	*numBytes = rx3Count;
	memcpy( buffer, rx3Buffer, rx3Count );
	rx3Count = 0; /* reset receive buffer */
    }
    else /* copy only the first *numBytes to buffer  */
    {
	memcpy( buffer, rx3Buffer, maxCopy );
	rx3Count -= maxCopy; /* remove the copied bytes from internal buffer */
	memmove( rx3Buffer, rx3Buffer + maxCopy, rx3Count );
    }
    EnableIntU3RX;
    //IEC5bits.U3ERIE = 1;
    return ERR_NONE;
}

s8 uartRx4NBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU4RX;
    if ( rx4Count <= maxCopy )
    { /* copy whole received data into buffer */
	*numBytes = rx4Count;
	memcpy( buffer, rx4Buffer, rx4Count );
	rx4Count = 0; /* reset receive buffer */
    }
    else /* copy only the first *numBytes to buffer  */
    {
	memcpy( buffer, rx4Buffer, maxCopy );
	rx4Count -= maxCopy; /* remove the copied bytes from internal buffer */
	memmove( rx4Buffer, rx4Buffer + maxCopy, rx4Count );
    }
    EnableIntU4RX;
    return ERR_NONE;
}




/*
s8 uartTxByte ( u8 dat )
{
    uartTx( dat );
    return ERR_NONE;
}
*/
/*
s8 uartTxString ( const char * text )
{
    while ( *text != '\0' )
    {
        uartTx( *text );
        ++text;
    }
    return ERR_NONE;
}
*/
/*
s8 uartTxNBytes ( const u8 * buffer, u16 length )
{
    while ( length-- )
    {
        uartTx( *buffer );
        ++buffer;
    }

    return ERR_NONE;
}
 * */


//#define UART_RECEIVE_ENABLED
#ifdef UART_RECEIVE_ENABLED

u16 uartRxNumBytesAvailable ()
{
    u16 num;
    DisableIntU4RX;//1
    num = rxCount;
    EnableIntU4RX;//1
    return num;
}

/*
s8 uartRxNBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU1RX;
    if ( rxCount <= maxCopy )
    {  copy whole received data into buffer
	*numBytes = rxCount;
	memcpy( buffer, rxBuffer, rxCount );
	rxCount = 0;  reset receive buffer 
    }
    else  copy only the first *numBytes to buffer 
    {
	memcpy( buffer, rxBuffer, maxCopy ); 
	rxCount -= maxCopy;  remove the copied bytes from internal buffer 
	memmove( rxBuffer, rxBuffer + maxCopy, rxCount );
    }
    EnableIntU1RX;
    return ERR_NONE;
}
*/

/*
void _U4RXInterrupt (void)
{
    U1RX_Clear_Intr_Status_Bit;

    while(DataRdyUART1())
    {
        if (rxCount >= UART_RX_BUFFER_SIZE || U1STAbits.OERR)
        {
            rxError = ERR_NOMEM;
            U1STAbits.OERR = 0;
            break;
        }
        contadorRX
        rxBuffer[rxCount] = ReadUART1();
        rxCount++;
        contadorParaEnviarArrayXbee = 0;
    }
}

 * */
#endif

void INTERRUPT _U1ErrInterrupt (void){
    //int data;     //LUCIANO: Variavel declarada e nao utilizada. Eliminei um warning.

    IFS4bits.U1ERIF = 0;

    if (U1STAbits.OERR)
        U1STAbits.OERR = 0;

    if (U1STAbits.FERR)
        U1STAbits.FERR = 0;

    if (U1STAbits.PERR)
        U1STAbits.PERR = 0;


    //data = ReadUART1();// leio para tentar tratar os erros.

}


void INTERRUPT _U1RXInterrupt (void){ // Porta usada para receber dados de Wifi
    
    U1RX_Clear_Intr_Status_Bit;
    
#ifdef PORTAL
    int x;
    while(DataRdyUART1()){
        // /*
        if ( U1STAbits.OERR){
            U1STAbits.OERR = 0;
        } else {
         // */
            x = ReadUART1();
            //lidaComComandoDOPC(ReadUART1());
            //Solucao nova com FIFO, apos Fevereiro de 2019
            trataRecepcaoDeDadosPela485((unsigned char)x);
            //Solucao velha usada em Juiz de Fora ate meados de Fevereiro de 2019
            //lidaComComandoDOPC(x);
            
        }
    }
    
#endif
}

void INTERRUPT _U2ErrInterrupt (void){
    int data;

    IFS4bits.U2ERIF = 0;

    if (U2STAbits.OERR)
        U2STAbits.OERR = 0;

    if (U2STAbits.FERR)
        U2STAbits.FERR = 0;

    if (U2STAbits.PERR)
        U2STAbits.PERR = 0;

    while(U2STAbits.URXDA == 1){ //Enquanto houver dados disponíveis //
        U2STAbits.URXDA = 0;
        data = U2RXREG;
    }
}


void INTERRUPT _U2RXInterrupt (void){ // Porta usada para receber dados de ETH
    //U2RX_Clear_Intr_Status_Bit; // limpa flag de interrupção da uart2;
    unsigned char dado;
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção ativado
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 0;
  
    IEC0bits.T1IE = 0;
    IEC0bits.T2IE = 0;
    IEC0bits.T3IE = 0;
    
    //IEC0bits.SPI1IE = 0;
    //IEC2bits.SPI2IE = 0;
    //IEC5bits.SPI3IE = 0;
    
    IEC3bits.RTCIE = 0;
    
    U2STAbits.OERR = 0;
    U2STAbits.FERR = 0;
    U2STAbits.PERR = 0;
    
    //u8 dado;
    int posicao = 0;
    int EsperaChegadaDeDados = 0; 
    
    posicao = 0;
    EsperaChegadaDeDados = 0;
    while(EsperaChegadaDeDados < 500){
        EsperaChegadaDeDados = EsperaChegadaDeDados + 1;
        while(U2STAbits.URXDA == 1){ //Enquanto houver dados disponíveis //
            U2STAbits.URXDA = 0;
            dado = U2RXREG;
            bufferRxEthernet[posicao] = dado;
            recebeDadoNaMaquinaDeEstados_DataHora(dado);             
          
            posicao = posicao + 1;
            EsperaChegadaDeDados = 0;
        }
    }
    //bufferRxEthernet[posicao] = '\0';
    zeraPonteiroDoBufferNaMaquinaDeEstados_DataHora();
    //enviaDadosParaEthPortais(bufferRxEthernet, strlen(bufferRxEthernet));
    //memset(bufferRxEthernet, NULL, 20);
    int posicaoAux = 0;
    if(debugInterfaceEthernet || debugInterfaceEthernet_Silent){
        while(posicaoAux < posicao){
            uart3Tx(bufferRxEthernet[posicaoAux++]);         
        }        
    }    
    
    IEC0bits.T1IE = 1;
    IEC0bits.T2IE = 1;
    IEC0bits.T3IE = 1;
    
    IEC1bits.U2RXIE = 1;
    
    //IEC0bits.SPI1IE = 1;
    //IEC2bits.SPI2IE = 1;
    //IEC5bits.SPI3IE = 1;
        
    //bufferRxEthernet[posicao] = NULL;
}


void INTERRUPT _U3ErrInterrupt (void){
    int data;

    IFS5bits.U3ERIF = 0;

    if (U3STAbits.OERR)
        U3STAbits.OERR = 0;

    if (U3STAbits.FERR)
        U3STAbits.FERR = 0;

    if (U3STAbits.PERR)
        U3STAbits.PERR = 0;

    while(DataRdyUART3()){
        data = ReadUART3();// leio para tentar tratar os erros.
    }
    //limpaBufferRxUSB();
    
}

//void INTERRUPT uart3RxIsr (void){
void INTERRUPT _U3RXInterrupt (void){ //Porta usada para receber dados da USB
    //U3RX_Clear_Intr_Status_Bit;
    
   // unsigned char dado;
    int posicao = 0;
//    unsigned char str[100];
             
    INTCON1bits.NSTDIS = 1;  // an inhamento de interrupção desativado
    IFS5bits.U3RXIF = 0;
    IEC5bits.U3RXIE = 0;
  
    IEC0bits.T1IE = 0;
    IEC0bits.T2IE = 0;
    IEC0bits.T3IE = 0;
    
    //IEC0bits.SPI1IE = 0;
    //IEC2bits.SPI2IE = 0;
    //IEC5bits.SPI3IE = 0;
    
    IEC3bits.RTCIE = 0;
    
    U3STAbits.OERR = 0;
    U3STAbits.FERR = 0;
    U3STAbits.PERR = 0;
    
    //u8 dado;
    //int posicao = 0;
    int EsperaChegadaDeDados = 0; 
    
    posicao = 0;
    EsperaChegadaDeDados = 0;
    while(EsperaChegadaDeDados < 500){
        EsperaChegadaDeDados = EsperaChegadaDeDados + 1;
        while(U3STAbits.URXDA == 1){ //Enquanto houver dados disponíveis //
            U3STAbits.URXDA = 0;
            bufferRxUSB[posicao] = U3RXREG;
            posicao = posicao + 1;
            EsperaChegadaDeDados = 0;
        }
    }
    if(posicao > 0){
        bufferRxUSB[posicao] = 0;
    }
    IEC0bits.T1IE = 1;
    IEC0bits.T2IE = 1;
    IEC0bits.T3IE = 1;
    
    IEC5bits.U3RXIE = 1;
    
    //IEC0bits.SPI1IE = 1;
    //IEC2bits.SPI2IE = 1;
    //IEC5bits.SPI3IE = 1;
    
}


extern int ResultadoProtocolo;

void INTERRUPT _U4RXInterrupt (void) // Zigbee
{
    U4RX_Clear_Intr_Status_Bit;
}


void INTERRUPT _U4TXInterrupt (void){
    
    
}
