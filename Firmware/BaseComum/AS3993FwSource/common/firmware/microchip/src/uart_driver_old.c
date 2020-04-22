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
#include <p24Fxxxx.h>
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

extern volatile int operador_atual_outro;

#ifdef UART_RECEIVE_ENABLED
#define uart1RxIsr _U1RXInterrupt
#endif

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/
#ifdef UART_RECEIVE_ENABLED
static u8 rxBuffer[UART_RX_BUFFER_SIZE];
static u16 rxCount;
static u8 rxError;
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
    UART_WRITE_REG(STA, UART_READ_REG(STA) | 0x2400);
    IEC0bits.U1RXIE = 1;

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


void uartTx (u8 dado){
    U1TXREG = dado;
    //while( (U1STA & 0x0200) );
}


s8 uartTxByte ( u8 dat )
{
    uartTx( dat );
    return ERR_NONE;
}

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
    { /* copy whole received data into buffer
	*numBytes = rxCount;
	memcpy( buffer, rxBuffer, rxCount );
	rxCount = 0; /* reset receive buffer 
    }
    else /* copy only the first *numBytes to buffer 
    {
	memcpy( buffer, rxBuffer, maxCopy ); 
	rxCount -= maxCopy; /* remove the copied bytes from internal buffer 
	memmove( rxBuffer, rxBuffer + maxCopy, rxCount );
    }
    EnableIntU1RX;
    return ERR_NONE;
}
*/

/*
void INTERRUPT uart1RxIsr (void)
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

void LigaRX (void)
{
    RPINR18 = 0x1F07;
    IPC2 = 0x4000;
    U1MODE = 0x8000;
    IEC0bits.U1RXIE = 1;
}

void INTERRUPT uart1RxIsr (void)
{
    //char temporario;
    U1RX_Clear_Intr_Status_Bit;


    //operador_atual_outro = 2;

    operador_atual_outro = U1RXREG;
    //operador_atual_outro = ReadUART1();

    //break;

    if(DataRdyUART1())
    {
        /*
        if (rxCount >= UART_RX_BUFFER_SIZE || U1STAbits.OERR)
        {
            rxError = ERR_NOMEM;
            U1STAbits.OERR = 0;
            break;
        }
         * */


        //arrayXbeeRecebido[contadorRX] = ReadUART1();
        //contadorRX = contadorRX + 1;
        //


        //rxBuffer[rxCount] = ReadUART1();
        //rxCount++;
        if (U1STAbits.OERR){
            U1STAbits.OERR = 0;
            //break;
        } else {
            //recebeOperador(ReadUART1());

            //recebeOperador(ReadUART1());

            
            //operador_atual_outro = ReadUART1();
            //if (temporario != 0)
                //operador_atual_outro = 2;
            
            


            
        }

        //resetContadores();
    }

}

//#endif

