/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : perifericos.H                                                     *
* Descricao : Header do perifericos.c                                         *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v.1.24                                   *
* PIC : PIC24FJ256DA210                                                       *
* Versao :                                                                    *
* Data : 31/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

#ifndef __PERIF_H
#define __PERIF_H

/*--------------------------------------------------------------------*/
/*                   ---+++>>> Constantes <<<+++---	                  */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                     ---+++>>> Defines <<<+++---	                  */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                     ---+++>>> Structs <<<+++---	                  */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                    ---+++>>> Prototipagem <<<+++---	              */
/*--------------------------------------------------------------------*/

void desliga_led_vm(void);
void liga_led_vm(void);
void desliga_sirene(void);
void liga_sirene(void);
void liga_rele1(void);
void liga_rele2(void);
void desliga_rele1(void);
void desliga_rele2(void);
void liga_rele3(void);
void liga_rele4(void);
void desliga_rele3(void);
void desliga_rele4(void);
void liga_saida_4(void);
void desliga_saida_4(void);
void liga_buzzer(void);
void desliga_buzzer(void);
void liga_led_zig(void);
void desliga_led_zig(void);
void liga_led_wifi(void);
void desliga_led_wifi(void);
void pisca_led_3g(void);
void liga_led_3g(void);
void desliga_led_3g(void);
void liga_led_gps(void);
void desliga_led_gps(void);
void liga_dir(void);
void desliga_dir(void);
void sel_led(int led, int status);
void pow_on_modem(void);
void pow_off_modem(void);
void liga_saida_pa (void);
void desliga_saida_pa (void);
void ld_pa (int status );
void liga_led_tag(void);
void desliga_led_tag(void);
void ld_saidas(int saida, int status);
void sel_antena (int antx);
void sel_cap_cin(int x);
void sel_cap_cout(int x);
void sel_cap_clen(int x);
void bloqueia_reh(void);
void libera_reh(void);
void bloqueia_frente(void);
void libera_frente(void);
void LigaRX(void);

/*--------------------------------------------------------------------*/
/*         ---+++>>> Fim do arquivo perifericos.H <<<+++---           */
/*--------------------------------------------------------------------*/
#endif
