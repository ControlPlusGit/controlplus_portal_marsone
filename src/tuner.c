#include "as3993_config.h"
#include "global.h"
#include "uart.h"
#include "tuner.h"
#include "as3993_public.h"
#include "platform.h"
#include "logger.h"
#include "timer.h"
#include "spi_driver.h"
#include "tuner.h"
#include <math.h>
#include "appl_commands.h"
#include "as3993.h"

extern int rssi_tag;
extern int dbm_tag_real;
extern Freq Frequencies;
void calcula_dbm(int);
//#define EMP1
#ifdef TUNER

#define TUNERDEBUG 0

#if TUNERDEBUG && (USE_LOGGER == LOGGER_ON)
#define TLOG dbgLog /*!< macro used for printing debug messages */
#define TLOGDUMP dbgHexDump /*!< macro used for dumping buffers */
#else
//#define TLOG(...) /*!< macro used for printing debug messages if USE_LOGGER is set */
//#define TLOGDUMP(...) /*!< macro used for dumping buffers if USE_LOGGER is set */
#endif


extern char Capacitores[6];

/*------------------------------------------------------------------------- */
void tunerDebug();
u8 tunerCheckConfig(const TunerConfiguration * config, u8 component);

/*------------------------------------------------------------------------- */

#ifdef PORTAL
void tunerInit_a1_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 6, 14, 11);
}
void tunerInit_a2_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 1, 7, 5);
}
void tunerInit_a3_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 4, 13, 4);
}
void tunerInit_a4_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 2, 11, 4);
}
void tunerInit_a5_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 2, 18, 16);
}
void tunerInit_a6_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 8, 13, 10);
}
void tunerInit_a7_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 2, 21, 16);
}
void tunerInit_a8_portal(const TunerConfiguration *config)
{

    tunerSetTuning(config, 0, 26, 21);
}
#endif
void tunerInit(const TunerConfiguration *config){
    //if (Frequencies.freq[0] != 925000){
        //Frequencies.freq[0]= 925000;
        ///Frequencies.numFreqs=0;
        //(void)as3993Initialize(Frequencies.freq[0]);
    //}
   

    
#ifdef SANKYU_JOAO_MONLEVADE_EMPILHADEIRA_7_5_TONS
    tunerSetTuning(config, 2, 7, 9);//Esse ajuste ficou com mais de 15 metros em Juiz de Fora no final de 2018
#endif 
    
#ifdef ARCELOR_JUIZ_DE_FORA_EMPILHADEIRA_TRADMAQ_3_5_TONS
    //tunerSetTuning(config, 2, 12, 12);//Ajustes para Antenas da China
#endif
    
    
    //tunerSetTuning(config, 2, 7, 9);//Esse ajuste ficou com mais de 15 metros em Juiz de Fora no final de 2018
    //tunerSetTuning(config, 2, 12, 12);//Ajustes para Antenas da China
    //tunerSetTuning(config, 1, 4, 4);//Teste para reduzir a leitura me Juiz De Do Fora no final de 2018
    
    //////tunerSetTuning(config, Capacitores[0], Capacitores[1], Capacitores[2]);
    tunerSetTuning(config, 1, 4, 4); //Valores obtidos com a varredura de capacitores...
    
    
    
    //tunerSetTuning(config, 1, 4, 4);
    //tunerSetTuning(config, 2, 7, 9); //antena 1 //Usando na Pixis Air e ficou mutio bom
    //tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
   
    //tunerSetTuning(config, 0, 11, 4);//antena 2, ficou bom para juiz de fora
    
    //tunerSetTuning(config, 0, 9, 5);//Estava com esse ajuste na primeira viagem de juzi de fora
    //tunerSetTuning(config, 1, 18, 10);
    //tunerSetTuning(config, 10, 26, 3);
    
    //tunerSetTuning(config, 0, 10, 6);//O melhor valor para Pixis Air
    //tunerSetTuning(config, 1, 18, 10);
    
}


void tunerAntena1(const TunerConfiguration *config){
    tunerSetTuning(config, 0, 22, 14); // Valores encontrados com varredura de capacitores
}

void tunerAntena2(const TunerConfiguration *config){
    tunerSetTuning(config, 2, 13, 12); // Valores encontrados com varredura de capacitores
}

void tunerAntena3(const TunerConfiguration *config){
    tunerSetTuning(config, 3, 12, 0); // Valores encontrados com varredura de capacitores
}

void tunerAntena4(const TunerConfiguration *config){
    tunerSetTuning(config, 3, 22, 12); // Valores "chutados"...
}

void tunerPadrao(const TunerConfiguration *config){
    tunerSetTuning(config, 0, 0, 0); // Valores encontrados com varredura de capacitores
}


void tunerInitTraseiraParaEmpilhadeiraComBaseComum(const TunerConfiguration *config){
    //tunerSetTuning(config, 0, 11, 4);
    //tunerSetTuning(config, 2, 7, 9);//Ruim
    //tunerSetTuning(config, 0, 10, 6);//Mais ou menos
    //tunerSetTuning(config, 10, 26, 3);//Ruim
    //tunerSetTuning(config, 0, 9, 5);//5 metros, estava em 23/11/18 em juiz com leitura fraca
    //tunerSetTuning(config, 0, 23, 11);
    //tunerSetTuning(config, 1, 18, 10);//Melhor ajuste ate o momento
    //tunerSetTuning(config, 1, 4, 4);
    //tunerSetTuning(config, 2, 12, 12);
    
    //tunerSetTuning(config, 0, 10, 6);//O melhor valor para Pixis Air
    
#ifdef SANKYU_JOAO_MONLEVADE_EMPILHADEIRA_7_5_TONS
    tunerSetTuning(config, 1, 18, 10);
#endif 
    
#ifdef ARCELOR_JUIZ_DE_FORA_EMPILHADEIRA_TRADMAQ_3_5_TONS
    tunerSetTuning(config, 2, 13, 12); // Valores encontrados com varredura de capacitores
    //tunerSetTuning(config, 1, 18, 10);
    //tunerSetTuning(config, 2, 12, 12);
    //tunerSetTuning(config, 2, 7, 9);
#endif
    
    //comentei porque o ajuste já é feito acima...
    //////tunerSetTuning(config, Capacitores[3], Capacitores[4], Capacitores[5]);
    
    //tunerSetTuning(config, 0, 10, 6);//Muito bom
    //tunerSetTuning(config, 0, 23, 11);
    //tunerSetTuning(config, 1, 4, 4);
    
}

void tunerInitCapacete(const TunerConfiguration *config){
    
    //tunerSetTuning(config, 2, 7, 9);
    //tunerSetTuningnfig, 2, 12, 12);
    //tunerSetTuning(config, 1, 4, 4);
    //tunerSetTuning(config, 0, 23, 11);
    //tunerSetTuning(config, 0, 11, 4);
    //tunerSetTuning(config, 0, 9, 5);
    //tunerSetTuning(config, 1, 18, 10);
    //tunerSetTuning(config, 10, 26, 3);//Menor leitura
    tunerSetTuning(config, 0, 10, 6);//Muito bom
    //tunerSetTuning(config, 1, 1, 1);
    //tunerSetTuning(config, 0, 0, 0);
    
}

void sintonizaCapacitores(const TunerConfiguration *config, u8 Cin, u8 Cen, u8 Cout)
{
   
    //tunerSetTuning(config, 2, 12, 12);
    //tunerSetTuning(config, 1, 4, 4);
     tunerSetTuning(config, Cin, Cin, Cout); //antena 1
   //tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
   //tunerSetTuning(config, 0, 11, 4);//antena 2
}


void tunerInitTraseiraJuizDeFora(const TunerConfiguration *config)
{
    //if (Frequencies.freq[0] != 940000){
        //Frequencies.freq[0]= 940000;
        //Frequencies.numFreqs=0;
        //(void)as3993Initialize(Frequencies.freq[0]);
    //}

    //tunerSetTuning(config, 2, 12, 12);//Ajustes para Antenas da China
    //tunerSetTuning(config, 1, 4, 4);
    //tunerSetTuning(config, 2, 7, 9); //antena 1
    //tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
   
    //tunerSetTuning(config, 0, 11, 4);//antena 2, ficou bom para juiz de fora
    
    //tunerSetTuning(config, 0, 9, 5);//Estava com esse ajuste na primeira viagem de juzi de fora
    //tunerSetTuning(config, 1, 18, 10);
    //tunerSetTuning(config, 10, 26, 3);
    
    //tunerSetTuning(config, 0, 10, 6);
    //tunerSetTuning(config, 16, 16, 16);
    tunerSetTuning(config, 1, 18, 10);//Esse ajuste ficou com mais de 15 metros em Juiz de Fora no final de 2018
    //tunerSetTuning(config, 2, 7, 9);//Teste para reduzir a leitura me Juiz De Do Fora no final de 2018
}

void tunerInitOperador(const TunerConfiguration *config){
    //tunerSetTuning(config, 1, 4, 4);
    //tunerSetTuning(config, 0, 23, 11);
    //tunerSetTuning(config, 10, 26, 3);
    //tunerSetTuning(config, 16, 16, 16);
    tunerSetTuning(config, 0, 10, 6);
}

void tunerInit_tec_ban_ax(const TunerConfiguration *config)
{
tunerSetTuning(config, 10, 26, 3);
}

void tunerInit_tec_ban_a1(const TunerConfiguration *config)
{
tunerSetTuning(config, 10, 26, 6);
}

    //tunerSetTuning(config, 16, 30, 3);
    //tunerSetTuning(config, 10, 26, 3);
    //tunerSetTuning(config, 0, 12,12 );
    //tunerSetTuning(config, 0, 9, 5);


void tunerInitFrenteSankyu(const TunerConfiguration *config)
{
   
    tunerSetTuning(config, 2, 12, 12);
    //tunerSetTuning(config, 1, 4, 4);
    // tunerSetTuning(config, 2, 7, 9); //antena 1
   //tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
   //tunerSetTuning(config, 0, 11, 4);//antena 2
}


void tunerInitTraseiraSankyu(const TunerConfiguration *config)
{
   
    //tunerSetTuning(config, 1, 18, 10);//Para Pixis LR
    
    //tunerSetTuning(config, 1, 4, 4);
   tunerSetTuning(config, 2, 7, 9); //antena 1
   //tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
   //tunerSetTuning(config, 0, 11, 4);//antena 2
}

/*
void tunerInit_S5_A1(const TunerConfiguration *config)
{
    tunerSetTuning(config,3, 15, 10);
    //return;
}
void tunerInit_S5_A2(const TunerConfiguration *config)
{
    tunerSetTuning(config, 2, 13, 6);
    //return;
}
void tunerInit_S5_A3(const TunerConfiguration *config)
{
    tunerSetTuning(config, 3, 21, 5);
    //return;
}
void tunerInit_S5_A4(const TunerConfiguration *config)
{
    tunerSetTuning(config, 2, 15, 5);
    //return;
}
void tunerInit_S6_A1(const TunerConfiguration *config)
{
    tunerSetTuning(config, 0, 10, 6);
    //return;
}
void tunerInit_S6_A2(const TunerConfiguration *config)
{
    tunerSetTuning(config, 0, 17, 11);
    //return;
}
void tunerInit_S6_A3(const TunerConfiguration *config)
{
    tunerSetTuning(config, 3, 15, 10);
    //return;
}
 */


void tunerInit_A1(const TunerConfiguration *config)
{
    //tunerSetTuning(config, 0, 12, 12);
    tunerSetTuning(config, 2, 12, 12); //antena 1
    //tunerSetTuning(config, 0, 24, 10); //antena 1 cabo grosso
}
void tunerInit_A2(const TunerConfiguration *config)
{
    //tunerSetTuning(config, 2, 14, 6);
    tunerSetTuning(config, 0, 11, 4);//antena 2
}
void tunerInit_A3(const TunerConfiguration *config)
{
    //tunerSetTuning(config, 0, 16, 0);
    tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
}
void tunerInit_A4(const TunerConfiguration *config)
{
    //tunerSetTuning(config, 0, 18, 4);
    tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
}
void tunerInit_A5(const TunerConfiguration *config)
{
    //tunerSetTuning(config, 0, 0, 12);
    tunerSetTuning(config, 0, 23, 11);//antena 3,4,5
}



/*------------------------------------------------------------------------- */
/**
 * Checks if component is available in current tuner configuration. If it is
 * available the return value is 0 if there is no such component the return
 * value is 1.
 * @param component tuner cap component (TUNER_Cin, TUNER_Clen, TUNER_Cout)
 * @return 0 If configuration matches, 1 if component is invalid.
 */
u8 tunerCheckConfig(const TunerConfiguration *config, u8 component)
{
    u8 ret = 0;
    switch (component)
    {
        case TUNER_CIN:
            if (config->cinSenFn == 0)
                ret = 1;
            break;
        case TUNER_CLEN:
            if (config->clenSenFn == 0)
                ret = 1;
            break;
        case TUNER_COUT:
            if (config->coutSenFn == 0)
                ret = 1;
            break;
        default:
            ret = 1;
    }
    if (ret == 1)
    {
        //TLOG("Tuner: invalid component: %hhx\n", component);
    }
    return ret;
}

#if TUNERDEBUG
/*------------------------------------------------------------------------- */
void tunerDebug(const TunerConfiguration *config)
{
    s16 val;
    s16 noiseval;
    s16 ch_val_i;
    s16 ch_val_q;
    s16 rf;
    u8 i;
    if (config->coutSenFn)
    for ( i = 0; i < 32; i++)
    {
        tunerSetCap(config, TUNER_COUT,i);
        noiseval = as3993GetReflectedPowerNoiseLevel();
        val = as3993GetReflectedPower( );
        ch_val_i = (s8)((val&0xff) - (noiseval&0xff));
        ch_val_q = (s8)(((val>>8)&0xff) - ((noiseval>>8)&0xff));
        rf = sqrt(ch_val_i * ch_val_i + ch_val_q * ch_val_q);
        TLOG("i=%hhd val_i=%hx val_q=%hx rf=%hx\n", i, ch_val_i, ch_val_q, rf);
    }
    if (config->cinSenFn)
    for ( i = 0; i < 32; i++)
    {
        tunerSetCap(config, TUNER_CIN,i);
        noiseval = as3993GetReflectedPowerNoiseLevel();
        val = as3993GetReflectedPower( );
        ch_val_i = (s8)((val&0xff) - (noiseval&0xff));
        ch_val_q = (s8)(((val>>8)&0xff) - ((noiseval>>8)&0xff));
        rf = sqrt(ch_val_i * ch_val_i + ch_val_q * ch_val_q);
        TLOG("i=%hhd val_i=%hx val_q=%hx rf=%hx\n", i, ch_val_i, ch_val_q, rf);
    }
    if (config->clenSenFn)
    for ( i = 0; i < 32; i++)
    {
        tunerSetCap(config, TUNER_CLEN,i);
        noiseval = as3993GetReflectedPowerNoiseLevel();
        val = as3993GetReflectedPower( );
        ch_val_i = (s8)((val&0xff) - (noiseval&0xff));
        ch_val_q = (s8)(((val>>8)&0xff) - ((noiseval>>8)&0xff));
        rf = sqrt(ch_val_i * ch_val_i + ch_val_q * ch_val_q);
        TLOG("i=%hhd rf=%hx\n",i,rf);
    }
}
#endif

/*------------------------------------------------------------------------- */
void tunerSetCap(const TunerConfiguration *config,  u8 component, u8 val )
{
    spiConfig_t tunerConfig, oldConfig;

    if (tunerCheckConfig(config, component))
        return;     //we have an invalid component.

    /* reconfigure spi to 2MHz */
    tunerConfig.frequency = 2000000ULL;
    tunerConfig.instance = SPI1;
    tunerConfig.clockPhase = 1;
    tunerConfig.clockPolarity = 0;
    tunerConfig.deviceId = 0;
    spiInitialize(SYSCLK, &tunerConfig, &oldConfig);

    switch (component)
    {
        case TUNER_CIN:
            config->cinSenFn(1);
            sel_cap_cin(1);
            break;
        case TUNER_CLEN:
            config->clenSenFn(1);
            sel_cap_clen(1);
            break;
        case TUNER_COUT:
            config->coutSenFn(1);
            sel_cap_cout(1);
            break;
    }

    spiTxRx(&val, 0, 1);

    switch (component)
    {
        case TUNER_CIN:
            config->cinSenFn(0);
            sel_cap_cin(0);
            break;
        case TUNER_CLEN:
            config->clenSenFn(0);
            sel_cap_clen(0);
            break;
        case TUNER_COUT:
            config->coutSenFn(0);
            sel_cap_cout(0);
            break;
    }
    /* restore spi to default */
    spiInitialize(SYSCLK, &oldConfig, 0);
}
u16 tunerGetReflected(void)
{
    u16 r; u32 refl;
    s8 i, q;
#ifdef POWER_DETECTOR
    as3993CyclicPowerRegulation();
#endif
    delay_us(300);
    r = as3993GetReflectedPower();
    i = r & 0xff;
    q = r >> 8;
    refl = (i * i) + (q * q);

    r = refl;
    if (refl > 0xffff) r= 0xffff;

    return r;
}

u8 tunerClimbOneParam(const TunerConfiguration *config,  u8 el, u8 *elval, u16 *reflectedPower, u16 *maxSteps)
{
    u16 refl, start = *reflectedPower;
    s8 dir = 0;
    s8 add = 0;
    u8 improvement = 3;
    u8 bestelval;

    if (tunerCheckConfig(config, el))
        return 0;     //we have an invalid component.

    if (*maxSteps == 0)
    {
        return 0;
    }

    //TLOG("start: el %hhx val %hhx r %hx\n",el,*elval,*reflectedPower);
    if ( *elval != 0 )
    {
        tunerSetCap(config, el, *elval - 1 ); 
        refl = tunerGetReflected();
        //TLOG("-1:  r %hx\n",refl);
        if ( refl <= *reflectedPower )
        {
            *reflectedPower = refl;
            dir = -1;
        }
    }

    if ( *elval < 31 )
    {
        tunerSetCap(config, el, *elval + 1 ); 
        refl = tunerGetReflected();
        //TLOG("+1:  r %hx\n",refl);
        if ( refl <= *reflectedPower )
        {
            *reflectedPower = refl;
            dir = 1;
        }
    }

    /* if it's possible we try to change the value by 2 and check what direction is better.
     * This improves tuning in the case of a local minima. */
    if ( *elval > 1 )
    {
        tunerSetCap(config, el, *elval - 2 );
        refl = tunerGetReflected();
        //TLOG("-2:  r %hx\n",refl);
        if ( refl <= *reflectedPower )
        {
            *reflectedPower = refl;
            dir = -1;
            add = -1;
        }
    }

    if ( *elval < 30 )
    {
        tunerSetCap(config, el, *elval + 2 );
        refl = tunerGetReflected();
        //TLOG("+2:  r %hx\n",refl);
        if ( refl <= *reflectedPower )
        {
            *reflectedPower = refl;
            dir = 1;
            add = 1;
        }
    }

    *elval += add;
    *elval += dir;
    bestelval = *elval;
    //TLOG("decision:  dir %hhx add %hhx\n",dir, add);

    if (dir!=0)
    {
        (*maxSteps)--;
        while ( improvement && *maxSteps )
        {
            if ( *elval == 0 ) break;
            if ( *elval == 31 ) break;
            tunerSetCap(config, el, *elval + dir ); 
            refl = tunerGetReflected();
            //TLOG("t %hhx:  r %hx\n",*elval + dir, refl);
            if ( refl <= *reflectedPower )
            {
                (*maxSteps)--;
                *reflectedPower = refl;
                *elval += dir;
                bestelval = *elval;
                improvement = 3; /* we don't want to stop when a local minima is hit, so we
                continue tuning for 3 more steps, even if they are worse than the one before.
                If it does not improve in this 3 steps, we will go back to the best value. */
            }else{
                improvement--;
            }
        }
    }
    *elval = bestelval;
    tunerSetCap(config, el, *elval);
    //TLOG("result val %hhx: r %hx\n",*elval, *reflectedPower);
    return start > (*reflectedPower);
}

void tunerOneHillClimb(const TunerConfiguration *config,  TunerParameters *p, u16 maxSteps)
{
    u8 improvement = 1;

    tunerSetTuning(config, p->cin, p->clen, p->cout);

    p->reflectedPower = tunerGetReflected();

    while (maxSteps && improvement)
    {
        improvement = 0;
        if (config->cinSenFn)
            improvement |= tunerClimbOneParam(config, TUNER_CIN, &p->cin, &p->reflectedPower, &maxSteps);
        if (config->clenSenFn)
            improvement |= tunerClimbOneParam(config, TUNER_CLEN, &p->clen, &p->reflectedPower, &maxSteps);
        if (config->coutSenFn)
            improvement |= tunerClimbOneParam(config, TUNER_COUT, &p->cout, &p->reflectedPower, &maxSteps);
    }

    tunerSetTuning(config, p->cin, p->clen, p->cout);
}

static const u8 tunePoints[3] = {5,16,26};

void tunerMultiHillClimb(const TunerConfiguration *config,  TunerParameters *res )
{
    TunerParameters p;
    u8 i,j;

    tunerSetTuning(config, res->cin, res->clen, res->cout);

    res->reflectedPower = tunerGetReflected();

    for( i = 0; i < 27; i++ )
    {
        j=i;
        if (config->cinSenFn)
            p.cin  = tunePoints[j%3]; j/=3;
        if (config->clenSenFn)
            p.clen = tunePoints[j%3]; j/=3;
        if (config->coutSenFn)
            p.cout = tunePoints[j%3]; j/=3;
        tunerOneHillClimb(config, &p, 30);
        //TLOG("new refl: %hx, prev refl: %hx, cin: %hhx, count: %hhx\n", p.reflectedPower, res->reflectedPower, p.cin, p.cout );
        if ( p.reflectedPower < res->reflectedPower )
        {
            *res = p;
        }
    }

    tunerSetTuning(config, res->cin, res->clen, res->cout);
}

void tunerTraversal(const TunerConfiguration *config,  TunerParameters *res )
{
    u8 l=0,o=0,i=0;
    u16 refl;
    res->reflectedPower = 32767;
    if (config->clenSenFn)
    for (l = 0; l < 32; l++)
    {
        tunerSetCap(config, TUNER_CLEN,l);
        //TLOG("%hhx\n",l);
        if (config->coutSenFn)
        for (o = 0; o < 32; o++)
        {
            tunerSetCap(config, TUNER_COUT,o);
            if (config->cinSenFn)
            for (i = 0; i < 32; i++)
            {
                tunerSetCap(config, TUNER_CIN,i);
                refl = tunerGetReflected();

                if ( refl < res->reflectedPower )
                {
                    res->cin  = i;
                    res->cout = o;
                    res->clen = l;
                    res->reflectedPower = refl;
                    //TLOG("cin=%hhx,cout=%hhx,clen=%hhx r=%hx\n",res->cin,res->cout,res->clen,res->reflectedPower);
                }
            }
        }
    }
    tunerSetTuning(config, res->cin, res->clen, res->cout);
    //TLOG("cin=%hhx,cout=%hhx,clen=%hhx r=%hx\n",res->cin,res->cout,res->clen,res->reflectedPower);
}

void tunerSetTuning(const TunerConfiguration *config, u8 cin, u8 clen, u8 cout )
{
    //TLOG("setTuning %hhx %hhx %hhx\n", cin, clen, cout);
    if (config->cinSenFn)
        tunerSetCap(config, TUNER_CIN, cin);
    if (config->clenSenFn)
        tunerSetCap(config, TUNER_CLEN, clen);
    if (config->coutSenFn)
        tunerSetCap(config, TUNER_COUT, cout);
}
#endif


/*
void calcula_dbfs(rssi_tag)
{
    int q,i;
    q = (rssi_tag & 0xF0) >> 4;
    i = (rssi_tag & 0x0F);

    //double rssiVal = sqrt ((double)(q*q + i*i))/sqrt(2.0);
        // the following formula is taken from the proper RSSI related application notes.
    int highRSSI = q>i?q:i;
    int lowRSSI = q<i?q:i;
    signed int deltaRSSI = highRSSI - lowRSSI;
    dbm_tag_real = 2*highRSSI + 10 * log10(1+pow(10,-deltaRSSI/10));
    //dbm_tag_real = (((dbm_tag_real / 33) * 100.0));
        //         33  = 2 * 15     + 10 * log10(1+1)
        // maximum value of the above formala goes intto percentage calculation

    //QByteArray res;
    //res.append((uchar) ((rssiVal / 33) * 100.0));
    //res.append(q*2);
    //res.append(i*2);
    //return res;

}
*/
void calcula_dbm(rssi_tag)
{
    int q,i;
    q = (rssi_tag & 0xF0) >> 4;
    i = (rssi_tag & 0x0F);

    dbm_tag_real = sqrt ((q*q + i*i))/1.414;
    //dbm_tag_real = sqrt (q*q + i*i);
        // the following formula is taken from the proper RSSI related application notes.
    /*
    int highRSSI = q>i?q:i;
    int lowRSSI = q<i?q:i;
    int deltaRSSI = highRSSI - lowRSSI;
    dbm_tag_real = 2*highRSSI + 10 * log10(1+pow(10,-deltaRSSI/10));
    */
        //         33  = 2 * 15     + 10 * log10(1+1)
        // maximum value of the above formala goes intto percentage calculation
/*
    QByteArray res;
    res.append((uchar) ((rssiVal / 33) * 100.0));
    res.append(q*2);
    res.append(i*2);
    return res;
*/
}



