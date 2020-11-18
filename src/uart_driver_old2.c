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
#include "p24FJ256DA210.h"
#include "errno.h"
#include "uart_driver.h"
#include "usb_hal_pic24.h"
#include "uart.h"
#include "global.h"
#ifdef UART_RECEIVE_ENABLED
#include "Compiler.h"
#include "string.h"
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
s8 uartRx3NBytes( u8 *, u16 *);

extern volatile int operador_atual_outro;
extern int conta_recebe_operador;
extern void desliga_led_zig(void);
extern void  liga_led_zig(void);
void uart4Tx(u8);
extern void poe_dado_XY(int,int,int);
extern u8 status_dir;
extern void desliga_dir();
extern void liga_dir();

#ifdef UART_RECEIVE_ENABLED
//#define _U1RXInterrupt
#endif
extern unsigned char stpegapac;
extern unsigned char pacote_chegou[];
//extern unsigned char tags_entrou_corredor[];
//extern unsigned char tags_saiu_corredor[];
//extern unsigned char qt_tags_saiu;
//extern unsigned char qt_tags_entrou ;
extern unsigned char tem_pacote;
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

#endif

//#ifdef AMBEV

//******************************************************************************
//* GLOBAL FUNCTIONS
//******************************************************************************

s8 uartTxInitialize (u32 sysclk, u32 baudrate, u32* actbaudrate)
{
    u32 br1, br2;
    u16 breg;

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
    /* Enable UART */
    UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x0400);
    //IEC0bits.U1RXIE = 1;

    return ERR_NONE;
}

#ifdef UART_RECEIVE_ENABLED
s8 uartRxInitialize ()
{
    rxCount = 0;
    rxError = ERR_NONE;
    /* clear receive buffer */
    U1RX_Clear_Intr_Status_Bit;
    while ( DataRdyUART1() )
    {
        ReadUART1();
    }

    return ERR_NONE;
}
#endif

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
    /* Enable UART */
    UART2_WRITE_REG(MODE, UART2_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART2_WRITE_REG(STA, UART2_READ_REG(STA) | 0x0400);
    //IEC0bits.U1RXIE = 1;

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
    /* Enable UART */
    UART3_WRITE_REG(MODE, UART3_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART3_WRITE_REG(STA, UART3_READ_REG(STA) | 0x0400);
    //IEC0bits.U1RXIE = 1;

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
    UART4_WRITE_REG(MODE, UART4_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(MODE, UART_READ_REG(MODE) | 0x8100);
    //UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    UART4_WRITE_REG(STA, UART4_READ_REG(STA) | 0x0400);
    //IEC0bits.U1RXIE = 1;

    return ERR_NONE;
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

s8 uartRxNBytes ( u8 * buffer, u16 * numBytes )
{
    u16 maxCopy = *numBytes;
    DisableIntU1RX;
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
    EnableIntU1RX;
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



void uart4Tx (u8 dado){
    U4TXREG = dado;
    while( (U4STA & 0x0100) );
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
/*
u16 uartRxNumBytesAvailable ()
{
    u16 num;
    DisableIntU1RX;
    num = rxCount;
    EnableIntU1RX;
    return num;
}
*/
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

void INTERRUPT _U1RXInterrupt (void)
{
    u8 x;
    x = ReadUART1();
    U1RX_Clear_Intr_Status_Bit;
    U1STAbits.OERR = 0;

}


void INTERRUPT _U2RXInterrupt (void){
    U2RX_Clear_Intr_Status_Bit;

    if(DataRdyUART2()){
        if (rx2Count >= UART_RX_BUFFER_SIZE || U2STAbits.OERR){
            U2STAbits.OERR = 0;

        } else {
            rx2Buffer[rx2Count] = ReadUART2();
            rx2Count++;

        }
    }

}


//void INTERRUPT uart3RxIsr (void){
void INTERRUPT _U3RXInterrupt (void){
    U3RX_Clear_Intr_Status_Bit;

    if(DataRdyUART3()){
        if (rx3Count >= UART_RX_BUFFER_SIZE || U3STAbits.OERR){
            U3STAbits.OERR = 0;

        } else {
            rx3Buffer[rx3Count] = ReadUART3();
            rx3Count++;

        }
    }

}

#ifdef PORTAL_AMBEV
void INTERRUPT _U4RXInterrupt (void)
{
    int x;
    U4RX_Clear_Intr_Status_Bit;
    x = ReadUART4();
    U4STAbits.OERR = 0;
}
#endif

#ifdef AMBEV
void INTERRUPT _U4RXInterrupt (void)
{
    U4RX_Clear_Intr_Status_Bit;
    //liga_led_zig();
    if(DataRdyUART4())
    {
        if (U4STAbits.OERR)
        {
            U4STAbits.OERR = 0;
            //break;
        }
        else
        {
            unsigned char x;
            x = ReadUART4();
            if (tem_pacote == 0)
            {
                pacote_chegou[pont_pac] = x;
                //pont_pac++;
                switch (stpegapac)
                {
                    case 0:
                    cs_pacote += x;
                    if (x == 0xF0)
                    {
                        pont_pac = 1;
                        stpegapac = 1;
                    }
                    break;
                case 1:
                    pont_pac++;
                    cs_pacote += x;
                    qt_tags_portal = x;
                    qt_pac_total = x;
                    if ((qt_pac_total > 250) || (qt_pac_total < 5) )
                    {
                        pont_pac = 0;
                        stpegapac = 0;
                        cs_pacote = 0;
                    }
                    else
                    {
                        qt_pacote = x - 2;
                        stpegapac = 2;
                    }
                    break;
                case 2:
                    pont_pac++;
                    cs_pacote += x;
                    no_serie = x;
                    qt_pacote--;
                    stpegapac = 3;
                    qt_tags_portal -= 4;
                    pacote_zerado = 0;
                    if (qt_tags_portal == 1)pacote_zerado = 1;
                    else if(qt_tags_portal == 2)
                    {
                        pont_pac = 0;
                        stpegapac = 0;
                        cs_pacote = 0;
                    }
                    else qt_tags_portal /= 3;
                    if (qt_tags_portal > 100)qt_tags_portal = 100;
                    break;
                case 3:
                    qt_pacote--;
                    pont_pac++;
                    //stpegapac = 4;
                    if ( qt_pacote == 0)
                    {
                        pont_pac = 0;
                        stpegapac = 0;
                        conta_tentativas_tx = 0;
                        if (cs_pacote == x)
                        {
                            if (status_dir == 0)liga_dir();
                            else desliga_dir();
                            tem_pacote = 1;
                        }
                        else tem_pacote = 0;
                        cs_pacote = 0;
                    }
                    else  cs_pacote += x;
                break;
                default:
                    pont_pac = 0;
                    stpegapac = 0;
                    cs_pacote = 0;
                break;

                }
            }
        }
    }
}

#endif

