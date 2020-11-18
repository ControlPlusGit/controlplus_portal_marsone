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
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*! \file
 *
 *  \author M. Arpa
 *  \author Wolfgang Reichart
 *  \author R. Veigl
 *
 *  \brief i2c driver implementation for Microchip PIC24F series
 *
 *
 *  This is the implementation file for the i2c driver which supports the
 *  Microchip PIC24F series.
 *
 */

/*!
 * i2c driver module
 */
/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#define USE_AND_OR
#include <p24Fxxxx.h>
#include <i2c.h>
#include <string.h>
#include "ams_types.h"
#include "ams_stream.h"
#include "ams_math_macros.h"
#include "GenericTypeDefs.h"
#include "errno.h"
#include "i2c_driver.h"
#include "logger.h"

/*
******************************************************************************
* DEFINES
******************************************************************************
*/

#define I2C_SPEED_100KHZ        100000ULL
#define I2C_SPEED_400KHZ        400000ULL
#define I2C_SPEED_1000KHZ       1000000ULL
#define I2C_SPEED_3400KHZ       3400000ULL

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/
static i2cConfig_t myCfgData;
static u32 mySystemClock;

/*
******************************************************************************
* LOCAL FUNCTION DECLARATION
******************************************************************************
*/

static u8 getI2cBrgValue (u8 clockMode);
static u16 getI2cBrgValueFromCustomSpeed(u32 customClockSpeed);

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
static u8 getI2cBrgValue (u8 clockMode)
{
    u8 i2cxBrg;
    switch(clockMode)
    {
    case I2C_SCK_100KHZ:
        i2cxBrg = getI2cBrgValueFromCustomSpeed(I2C_SPEED_100KHZ);
        break;
    case I2C_SCK_400KHZ:
        i2cxBrg = getI2cBrgValueFromCustomSpeed(I2C_SPEED_400KHZ);        
        break;
    case I2C_SCK_1000KHZ:
        i2cxBrg = getI2cBrgValueFromCustomSpeed(I2C_SPEED_1000KHZ);        
        break;
    case I2C_SCK_3400KHZ:
        i2cxBrg = getI2cBrgValueFromCustomSpeed(I2C_SPEED_3400KHZ);
        break;
    default:
        i2cxBrg = getI2cBrgValueFromCustomSpeed(I2C_SPEED_100KHZ);
        break;
    }
    return i2cxBrg;
}

static u16 getI2cBrgValueFromCustomSpeed (u32 customClockSpeed)
{
    // The calculation of the BRG value is only an approach. For exact clock speed
    // the result should be verified by measuring.
    //
    // formula: i2cxBrg = (SYSCLK/Fscl) - (SYSCLK/10000000) - 1
    //                    (  first    ) - (   second     term  )
    //
    // The first term: 'SYSCLK/Fscl' should be rounded to the nearest value,
    // so the macro DIV_ROUND_NEAREST is used.
    //
    // The second term: '(SYSCLK/10000000) - 1' can reach only a few number of
    // values (3, 2 or 1). The value should also be rounded to the nearest value.
    // So there are 3 cases:
    //
    // case: SYSCLK >= 15000000:
    //       15000000/10000000 = 1.5 -> round up to 2    (-2 - 1 => -3)
    //
    // case:  5000000 <= SYSCLK < 15000000:
    //        5000000/10000000 = 0.5 -> round up to 1
    //       14999999/10000000 = 1.49 -> round down to 1 (-1 - 1 => -2)
    //
    // case: SYSCLK < 5000000:
    //       4999999/10000000 = 0.49 -> round down to 0  (-0 - 1 => -1)

    if (mySystemClock >= 15000000ULL)
    {
        return (DIV_ROUND_NEAREST(mySystemClock, customClockSpeed) - 3) & 0x1FF;
        //return ((mySystemClock / customClockSpeed) - 3) & 0x1FF;
    }
    else if (mySystemClock < 15000000ULL && mySystemClock >= 5000000ULL)
    {
        return (DIV_ROUND_NEAREST(mySystemClock, customClockSpeed) - 2) & 0x1FF;
        //return ((mySystemClock / customClockSpeed) - 2) & 0x1FF;
    }
    else // (mySystemClock < 5000000ULL)
    {
        return (DIV_ROUND_NEAREST(mySystemClock, customClockSpeed) - 1) & 0x1FF;
        //return ((mySystemClock / customClockSpeed) - 1) & 0x1FF;
    }
}

static void i2cStart (void)
{
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        IdleI2C1(); /* wait for idle bus */
        StartI2C1();
        while (I2C1CONbits.SEN); /* wait until start sequence is completed */
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        IdleI2C2(); /* wait for idle bus */
        StartI2C2();
        while (I2C2CONbits.SEN); /* wait until start sequence is completed */
    }
    else
    {
        DBG_ASSERT(0);
    }

}

static void i2cStop (void)
{
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        StopI2C1(); /* send stop condition */
        while (I2C1CONbits.PEN); /* wait until stop sequence is completed */
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        StopI2C2(); /* send stop condition */
        while (I2C2CONbits.PEN); /* wait until stop sequence is completed */
    }
    else
    {
        DBG_ASSERT(0);
    }
}

static void i2cRestart (void)
{
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        RestartI2C1(); /* repeated start */
        while (I2C1CONbits.RSEN); /* wait until start sequence is completed */
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        RestartI2C2(); /* repeated start */
        while (I2C2CONbits.RSEN); /* wait until start sequence is completed */
    }
    else
    {
        DBG_ASSERT(0);
    }
}

static void i2cIdle (void)
{
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        IdleI2C1();
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        IdleI2C2();
    }
    else
    {
        DBG_ASSERT(0);
    }
}

static u8 i2cWriteByte (u8 data)
{
    u8 retVal = 0;

    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        MasterWriteI2C1(data);
        while (I2C1STATbits.TRSTAT); /* wait until byte is ack/nak ed */
        if (I2C1STATbits.ACKSTAT)
        {
            retVal = -1; /* NAK case */
        }
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        MasterWriteI2C2(data);
        while (I2C2STATbits.TRSTAT); /* wait until byte is ack/nak ed */
        if (I2C2STATbits.ACKSTAT)
        {
            retVal = -1; /* NAK case */
        }
    }
    else
    {
        DBG_ASSERT(0);
    }
    return retVal;
}

static u16 i2cReadNBytes (u16 nRead, u8 *read)
{
    u16 rxed = 0;

    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        rxed = MastergetsI2C1(nRead, read, 10000);
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        rxed = MastergetsI2C2(nRead, read, 10000);
    }
    else
    {
        DBG_ASSERT(0);
    }
    return (rxed == 0 ? nRead : 0);
}


static u16 i2cTxAllButStopCondition (u8 slaveAddr, const u8 * txData, u16 numberOfBytesToTx)
{
    u16 txed = 0; /* failure */

    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        IdleI2C1(); /* wait for idle bus */
        StartI2C1();
        while (I2C1CONbits.SEN); /* wait until start sequence is completed */
        MasterWriteI2C1(slaveAddr << 1);
        while (I2C1STATbits.TRSTAT); /* wait until address is txed and ack/nak rxed */
        if (I2C1STATbits.ACKSTAT == 0) /* address accepted by slave */
        {
            /* transmit data */
            while(txed < numberOfBytesToTx)
            {
                MasterWriteI2C1(*txData);
                while (I2C1STATbits.TRSTAT); /* wait until byte is ack/nak ed */
                if (I2C1STATbits.ACKSTAT)
                {
                    break; /* early exit */
                }
                txed++;
                txData++;
            }
        }
        IdleI2C1();
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        IdleI2C2(); /* wait for idle bus */
        StartI2C2();
        while (I2C2CONbits.SEN); /* wait until start sequence is completed */
        MasterWriteI2C2(slaveAddr << 1);
        while (I2C2STATbits.TRSTAT); /* wait until address is txed and ack/nak rxed */
        if (I2C2STATbits.ACKSTAT == 0) /* address accepted by slave */
        {
            /* transmit data */
            while(txed < numberOfBytesToTx)
            {
                MasterWriteI2C2(*txData);
                while (I2C2STATbits.TRSTAT); /* wait until byte is ack/nak ed */
                if (I2C2STATbits.ACKSTAT)
                {
                    break; /* early exit */
                }
                txed++;
                txData++;
            }
        }
        IdleI2C2();
    }
    return txed;
}

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/

void i2cDeserialiseConfig( i2cConfig_t * config, const u8 * data )
{
    config->i2cModule = data[0];
    config->i2cAddressMode = data[1];
    config->i2cClockMode = data[2];

    if(config->i2cClockMode < I2C_SCK_CUSTOM)
    {
        config->i2cCustomClockSpeed = 0;
    }
    else
    {
        config->i2cCustomClockSpeed = AMS_STREAM_I2C_CONFIG_GET_CUSTOM_CLK_SPEED(data);
    }
}

void i2cSerialiseConfig( const i2cConfig_t * config, u8 * data )
{
    data[0] = config->i2cModule;
    data[1] = config->i2cAddressMode;
    data[2] = config->i2cClockMode;

    if(config->i2cClockMode == I2C_SCK_CUSTOM)
    {
        AMS_STREAM_I2C_CONFIG_SET_CUSTOM_CLK_SPEED(data, config->i2cCustomClockSpeed);
    }
}


s8 i2cInitialize (u32 sysClk, const i2cConfig_t* const cfgDataIn, i2cConfig_t *cfgDataOut)
{
    u16 myI2cAddressMode = I2C_7BIT_ADD;
    u16  i2cxBrg;
    s8 retVal = ERR_NONE;

    // FIXME: currently we assume a sysclk of 16MHz!! For further use
    mySystemClock = sysClk;

    if (cfgDataOut != NULL)
    {
        /* fill outgoing config with current configuration values */
        cfgDataOut->i2cAddressMode      = myCfgData.i2cAddressMode;
        cfgDataOut->i2cClockMode        = myCfgData.i2cClockMode;
        cfgDataOut->i2cModule           = myCfgData.i2cModule;
        cfgDataOut->i2cCustomClockSpeed = myCfgData.i2cCustomClockSpeed;
    }

    myCfgData.i2cAddressMode        = ((i2cConfig_t*)cfgDataIn)->i2cAddressMode;
    myCfgData.i2cClockMode          = ((i2cConfig_t*)cfgDataIn)->i2cClockMode;
    myCfgData.i2cModule             = ((i2cConfig_t*)cfgDataIn)->i2cModule;
    myCfgData.i2cCustomClockSpeed   = ((i2cConfig_t*)cfgDataIn)->i2cCustomClockSpeed;

    if (I2C_ADDRESS_MODE_10_BIT == myCfgData.i2cAddressMode)
    {
        myI2cAddressMode = I2C_10BIT_ADD;
    }

    if(myCfgData.i2cClockMode == I2C_SCK_CUSTOM)
    {
        i2cxBrg = getI2cBrgValueFromCustomSpeed(myCfgData.i2cCustomClockSpeed);
    }
    else
    {
        i2cxBrg = getI2cBrgValue(myCfgData.i2cClockMode);
    }

    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        /* configure i2c1 block to be i2c master */
        CloseI2C1(); /* make sure i2c is off before (re-)configuring */
        OpenI2C1((I2C_ON | myI2cAddressMode) , i2cxBrg);
        IdleI2C1();
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        /* configure i2c2 block to be i2c master */
        CloseI2C2(); /* make sure i2c is off before (re-)configuring */
        OpenI2C2((I2C_ON | myI2cAddressMode) , i2cxBrg);
        IdleI2C2();
    }
    else
    {
        /* assertion due to parameter error */
        DBG_ASSERT(0);
        retVal = ERR_PARAM;
    }
    I2C_LOG("\nI2C-C M=%hhx, A=%hhx, C=%hhx, BRG=%hhx\n", myCfgData.i2cModule, myCfgData.i2cAddressMode, myCfgData.i2cClockMode, i2cxBrg);
    return retVal;
}

u16 i2cTx (u8 slaveAddr, const u8 * data, u16 numberOfBytesToTx)
{
    u16 txed = i2cTxAllButStopCondition(slaveAddr, data, numberOfBytesToTx);
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        StopI2C1(); /* send stop condition */
        while (I2C1CONbits.PEN); /* wait until stop sequence is completed */
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        StopI2C2(); /* send stop condition */
        while (I2C2CONbits.PEN); /* wait until stop sequence is completed */
    }
    return txed;
}


u16 i2cRx (u8 slaveAddr, const u8 * txData, u16 numberOfBytesToTx, u8 * rxData, u16 numberOfBytesToRx)
{
    u16 rxed = 0; /* failure */
    u16 txed = i2cTxAllButStopCondition(slaveAddr, txData, numberOfBytesToTx);
    if (txed != 1)
    {
        if (I2C1_MODULE == myCfgData.i2cModule)
        {
            StopI2C1(); /* send stop condition */
            while (I2C1CONbits.PEN); /* wait until stop sequence is completed */
        }
        else if (I2C2_MODULE == myCfgData.i2cModule)
        {
            StopI2C2();
            while (I2C2CONbits.PEN);
        }
        return rxed;
    }
    if (I2C1_MODULE == myCfgData.i2cModule)
    {
        RestartI2C1(); /* repeated start */
        while (I2C1CONbits.RSEN); /* wait until start sequence is completed */
        MasterWriteI2C1(slaveAddr << 1 | 1); /* read has LSB = 1 */
        while (I2C1STATbits.TRSTAT); /* wait until address is txed and ack/nak rxed */
        if (I2C1STATbits.ACKSTAT == 0) /* address accepted by slave */
        {
            /* receive data */
            //MI2C1_Clear_Intr_Status_Bit;
            rxed = MastergetsI2C1(numberOfBytesToRx, rxData, 10000);
        }
        IdleI2C1();
        StopI2C1(); /* send stop condition */
        while (I2C1CONbits.PEN); /* wait until stop sequence is completed */
    }
    else if (I2C2_MODULE == myCfgData.i2cModule)
    {
        RestartI2C2(); /* repeated start */
        while (I2C2CONbits.RSEN); /* wait until start sequence is completed */
        MasterWriteI2C2(slaveAddr << 1 | 1); /* read has LSB = 1 */
        while (I2C2STATbits.TRSTAT); /* wait until address is txed and ack/nak rxed */
        if (I2C2STATbits.ACKSTAT == 0) /* address accepted by slave */
        {
            /* receive data */
            //MI2C2_Clear_Intr_Status_Bit;
            rxed = MastergetsI2C2(numberOfBytesToRx, rxData, 10000);
        }
        IdleI2C2();
        StopI2C2(); /* send stop condition */
        while (I2C2CONbits.PEN); /* wait until stop sequence is completed */
    }
    return (rxed == 0 ? numberOfBytesToRx : 0);
}

s8 i2cRxTx ( u16 numberOfBytesToTx, const u8 * txData, u16 numberOfBytesToRx, u8 * rxData, BOOL sendStartCond, BOOL sendStopCond )
{
    u16 txed = 0;
    u16 rxed = 0;
    u16 toTx;

    I2C_LOG("\nI2C-T W=%hhx, R=%hhx, STA=%hhx, STP=%hhx\n", numberOfBytesToTx, numberOfBytesToRx, sendStartCond, sendStopCond);
    I2C_LOGDUMP(txData, numberOfBytesToTx);

    /* 1.) check if wen need to send a start condition. */
    if (sendStartCond)
    {
        /* send a start cond */
        i2cStart();
        if (numberOfBytesToRx)
        {
            /*
             * In case this is a read command, we need to resend the device address.
             * The device address (prepared for reading) is already prepared on the host side.
             * -> Decrement the write count because the last entry in the write buffer is the
             * device address prepared for reading.
             */
            numberOfBytesToTx--;
        }
    }

    toTx = numberOfBytesToTx;

    /* 2.) do the write transfer */
    for (; numberOfBytesToTx > 0; numberOfBytesToTx--, txData++)
    {
        if (i2cWriteByte(*txData))
        {
            i2cStop();
            I2C_LOG("I2C-T(-1)\n");
            return ERR_IO; /* this was a NAK */
        }
        txed++;
    }
    i2cIdle();

    /* 3.) do the read transfer if necessary */
    if (numberOfBytesToRx)
    {
        if (sendStartCond)
        {
            i2cRestart();
            /* send the device address (already prepared in buffer on the host side) */
            if (i2cWriteByte(*txData))
            {
                i2cStop();
                I2C_LOG("I2C-T(-2)\n");
                return ERR_IO; /* this was a NAK */
            }
        }
        rxed = i2cReadNBytes(numberOfBytesToRx, rxData);
        I2C_LOG("I2CR\n");
        I2C_LOGDUMP(rxData, numberOfBytesToRx);
    }
    i2cIdle();

    /* 4.) send a top condition if necessary */
    if (sendStopCond)
    {
        i2cStop();
    }
    I2C_LOG("I2C-T rxed=%hhx, nRead=%hhx, txed=%hhx, toTx=%hhx\n", rxed, numberOfBytesToRx, txed, toTx);
    return ((rxed == numberOfBytesToRx && txed == toTx) ? ERR_NONE : ERR_IO);
}

