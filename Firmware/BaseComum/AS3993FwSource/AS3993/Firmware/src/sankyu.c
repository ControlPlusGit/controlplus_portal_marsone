/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : sankyu.C                                                          *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 31/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

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
#include "perifericos.h"
#include "tags.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/

extern unsigned char meu_operador[];
extern u8  operador_atual;
extern int pode_operar;
extern unsigned int conta_operador_ausente;
extern int total_parcial;
extern Tag __attribute__((far)) tags_[];
extern char desabilitado[];
extern char autorizado[];

/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/

void  trata_operador_sankyu(void)
{
    if (total_parcial == 0)
    {
        desliga_led_tag();
        conta_operador_ausente++;
        //LUCIANO: MODIFICACAO - diminuicao do debounce de ausencia do operador para ~2 segundos.
        if (conta_operador_ausente > 12)
        //if (conta_operador_ausente > 6) //LUCIANO: cada 3 unidades correspondem a 1 segundo
        //LUCIANO: FIM DA MODIFICACAO
        {
            operador_atual = 0;
            meu_operador[0] = 0;
            meu_operador[1] = 0;
            meu_operador[2] = 0;
            pode_operar = 0;
            poe_texto_XY (1,0,desabilitado);
            bloqueia_reh();
            bloqueia_frente();
            liga_led_vm();
        }
    }
    //LUCIANO: MODIFICACAO 26/09/2016 - Agora para liberar o condutor para usar a empilhadeira o sensor de banco (ou cinto) que eh a entrada _RG7 deve estar acionada.
    //else if (tags_[0].epc[9]==0x50) //LUCIANO: Se identificou um operador com autorizacao para conduzir a empilhadeira
    //else if ((tags_[0].epc[9]==0x50) && (_RG7 == 0)) //LUCIANO: Se identificou um operador com autorizacao para conduzir a empilhadeira
    //LUCIANO: Modifiquei os indices do vetor, pois nao ha mais a inversao de bytes na funcao troca_bytes()
    else if ((verifSeEhCondutor(0)) && (_RG7 == 0)) //LUCIANO: Se identificou um operador com autorizacao para conduzir a empilhadeira
    //Fim da modificacao
    {
        liga_led_tag();
        conta_operador_ausente = 0;
        //LUCIANO: Modifiquei os indices do vetor, pois nao ha mais a inversao de bytes na funcao troca_bytes()
        //meu_operador[2]=tags_[0].epc[9];
        //meu_operador[1]=tags_[0].epc[10];
        //meu_operador[0]=tags_[0].epc[11];
        meu_operador[2]=tags_[0].epc[0];
        meu_operador[1]=tags_[0].epc[1];
        meu_operador[0]=tags_[0].epc[2];
        //operador_atual = tags_[0].epc[11];
        operador_atual = tags_[0].epc[2];
        //Fim da modificacao
        libera_reh();
        libera_frente();
        pode_operar = 1;
        desliga_led_vm();
        poe_texto_XY (1,0,autorizado);
        poe_dado_XY (1,2,operador_atual);
    }
}

/*--------------------------------------------------------------------*/
/*            ---+++>>> Fim do arquivo sankyu.C <<<+++---             */
/*--------------------------------------------------------------------*/		
