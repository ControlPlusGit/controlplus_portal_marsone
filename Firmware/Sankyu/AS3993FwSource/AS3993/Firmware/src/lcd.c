
#include "lcd.h"
#include "timer.h"
#include "platform.h"
#include "ams_types.h"
#include "global.h"
//#include "string.h"
//#include "stdio.h"
//#include "stdlib.h"
extern unsigned int qt_leituras[8];
extern unsigned int qt_tags[8];
extern unsigned int total_tags;
extern int atualiza_display;

//poe dado no display
void poe_dado_display (unsigned char dado_d)
{
    int x;
    x = dado_d & 1;
    //LAT_LEDA5_AA34_RL2(x);      //SAIDA BD4
    SAIDA_BD4(x);                 //SAIDA BD4
    x = dado_d & 2;
    x = x>>1;
    //LAT_LEDA6_AB56_LPA(x);      //SAIDA BD5
    SAIDA_BD5(x);                 //SAIDA BD5
    x = dado_d & 4;
    x = x>>2;
    //LAT_LEDA7_AB78_CAP1(x);     //saida BD6
    SAIDA_BD6(x);                 //SAIDA BD6
    x = dado_d & 8;
    x = x>>3;
    //LAT_LEDA8_S1_CAP2_CAP3(x);  //saida BD7
    SAIDA_BD7(x);                 //SAIDA BD7
}

void niblle_com_display (unsigned char niblle)
{
    poe_dado_display (niblle);
    //SAIDA_ELCD(0);
    SAIDA_RS(0);                    //RS_PIN = 0;
    delay_us(1);
    SAIDA_ELCD(1);                  //E_PIN = 1;// Clock command in
    delay_us(10);
    SAIDA_ELCD(0);                  //E_PIN = 0;
    delay_us(1);
    delay_ms(2);
}

void niblle_dado_display (unsigned char niblle)
{
    poe_dado_display (niblle);
    //SAIDA_ELCD(0);
    SAIDA_RS(1);                    
    delay_us(1);
    SAIDA_ELCD(1);                 
    delay_us(10);
    SAIDA_ELCD(0);
    delay_us(100);
}
void minha_inicia_display (void)
{
    niblle_com_display (0x0F);
    delay_us(1);

    niblle_com_display (0x02);
    niblle_com_display (0x02);
    niblle_com_display (0x08);
    delay_ms(1);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x08);
    delay_ms(1);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x01);
    delay_ms(1);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x06);
    delay_ms(1);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x02);
    delay_ms(1);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x0C);
    delay_ms(1);
    /*
    poe_dado_display (0x00); 
    SAIDA_ELCD(0);
    SAIDA_RS(0);
    delay_ms (20);
    poe_dado_display (0x03);

    delay_us(1);
    SAIDA_ELCD(1);
    delay_us(10);
    SAIDA_ELCD(0);
    delay_ms(15);

    SAIDA_ELCD(1);
    delay_us(10);
    SAIDA_ELCD(0);
    delay_us(100);
    
    SAIDA_ELCD(1);
    delay_us(10);
    SAIDA_ELCD(0);
    delay_ms(15);

    poe_dado_display (0x02);
    SAIDA_ELCD(1);
    delay_us(10);
    SAIDA_ELCD(0);
    delay_us(100);
    delay_ms(15);

    
    //niblle_com_display (0x02);
    //niblle_com_display (0x00);
    //BusyXLCD();
    //niblle_com_display (0x02);
    //niblle_com_display (0x00);
    //BusyXLCD();
    
    niblle_com_display (0x02);
    niblle_com_display (0x08);
    delay_ms(5);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x08);
    delay_ms(5);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x01);
    delay_ms(5);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x06);
    delay_ms(5);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x02);
    delay_ms(5);
    //BusyXLCD();
    niblle_com_display (0x00);
    niblle_com_display (0x0C);
    delay_ms(5);
    //BusyXLCD();
     */
}

void WriteDataXLCD(unsigned char data)
{
    niblle_dado_display (data>>4);
    niblle_dado_display (data);

}


void WriteCmdXLCD(unsigned char cmd)
{
    niblle_com_display (cmd>>4);
    niblle_com_display (cmd);

}


unsigned char BusyXLCD(void)
{
  delay_ms(1);
  return 0;
}


void putsXLCD(char *buffer)
{
        while(*buffer)                  // Write data to LCD up to null
        {
                WriteDataXLCD(*buffer); // Write character to LCD
                buffer++;               // Increment buffer
                delay_us (10);
        }
        return;
}

void SetDDRamAddr(unsigned char DDaddr)
{

        DDaddr = DDaddr | 0b10000000;
        niblle_com_display (DDaddr>>4);
        niblle_com_display (DDaddr);

}
void limpa_display (void)
{
    WriteCmdXLCD(0x01);
    BusyXLCD();
}
void retorna_display (void)
{
    WriteCmdXLCD(0x02);
    BusyXLCD();
}
/*
void mostra_tags_display(void )
{
    int antena = 0;
    int mostra = 1;
    int linha,coluna;
    char no_tags []="Procurando...";
    if (atualiza_display)
    {
        atualiza_display = 0;
        limpa_display();
    }

        while (antena <= 7)
        {
            if (mostra > 4)break;
                   
            switch (mostra)
            {
                    case(1):
                        if (qt_tags[antena] == 0)break;
                        linha = 1;
                        coluna = 0;
                        poe_dado_XY (linha,coluna,(antena+1));
                        coluna++;
                        poe_texto_XY (linha,coluna,":");
                        coluna++;
                        poe_dado_XY (linha,coluna,qt_tags[antena]);
                        coluna++;
                        poe_texto_XY (linha,coluna,"-");
                        coluna = 4;
                        poe_texto_XY (linha,coluna,"   ");
                        poe_dado_XY (linha,coluna,qt_leituras[antena]);
                        mostra++;
                    break;

                    case(2):
                        if (qt_tags[antena] == 0)break;
                        linha = 1;
                        coluna = 8;
                        poe_dado_XY (linha,coluna,(antena+1));
                        coluna++;
                        poe_texto_XY (linha,coluna,":");
                        coluna++;
                        poe_dado_XY (linha,coluna,qt_tags[antena]);
                        coluna++;
                        poe_texto_XY (linha,coluna,"-");
                        coluna = 12;
                        poe_texto_XY (linha,coluna,"   ");
                        poe_dado_XY (linha,coluna,qt_leituras[antena]);
                        mostra++;
                    break;
                    
                    case(3):
                        if (qt_tags[antena] == 0)break;
                        linha = 2;
                        coluna = 0;
                        poe_dado_XY (linha,coluna,(antena+1));
                        coluna++;
                        poe_texto_XY (linha,coluna,":");
                        coluna++;
                        poe_dado_XY (linha,coluna,qt_tags[antena]);
                        coluna++;
                        poe_texto_XY (linha,coluna,"-");
                        coluna = 4;
                        poe_texto_XY (linha,coluna,"   ");
                        poe_dado_XY (linha,coluna,qt_leituras[antena]);
                        mostra++;
                    break;

                    case(4):
                        if (qt_tags[antena] == 0)break;
                        linha = 2;
                        coluna = 8;
                        poe_dado_XY (linha,coluna,(antena+1));
                        coluna++;
                        poe_texto_XY (linha,coluna,":");
                        coluna++;
                        poe_dado_XY (linha,coluna,qt_tags[antena]);
                        coluna++;
                        poe_texto_XY (linha,coluna,"-");
                        coluna = 12;
                        poe_texto_XY (linha,coluna,"   ");
                        poe_dado_XY (linha,coluna,qt_leituras[antena]);
                        mostra++;
                    break;
                    default:
                    break;
            }
            antena++;
        }
        //antena++;
    //}
    if (mostra == 1)
    {
        limpa_display();
        poe_texto_XY (1,0,no_tags);
        atualiza_display = 1;
        return;
    }
}
*/

void inicia_display (void)
{
    char controlup []="Control-Up";
    char marte1 []="Mars One";

    minha_inicia_display ();
    poe_texto_XY (1,5,controlup);
    poe_texto_XY (2,6,marte1);
    
}

void poe_dado_XY (unsigned char linha, unsigned char coluna, long int dado)
{
//#ifdef DISPLAY
    char dado_em_ascii[7];
    long int temp = 0;
    int idx = 0;
    int poe_zero=6;
        if (dado == 0)
        {
        dado_em_ascii[idx] = dado + 0x30;
        idx++;
        dado_em_ascii[idx] = NULL;
        poe_texto_XY (linha,coluna,dado_em_ascii);
        return;
        }
        temp = dado/100000;
        if (temp)
        {
            dado_em_ascii[idx] = temp + 0x30;
            idx++;
            dado -= (temp * 100000);
            poe_zero = 5;
        }
        temp = dado/10000;
        if (temp)
        {
            dado_em_ascii[idx] = temp + 0x30;
            idx++;
            dado -= (temp * 10000);
            poe_zero = 4;
        }
        else if (poe_zero == 5)
        {
             dado_em_ascii[idx] = 0x30;
             idx++;
        }
        temp = dado/1000;
        if (temp)
        {
            dado_em_ascii[idx] = temp + 0x30;
            idx++;
            dado -= (temp * 1000);
            poe_zero = 3;
        }
        else if (poe_zero < 6)
        {
             dado_em_ascii[idx] = 0x30;
             idx++;
        }
        temp = dado/100;
        if (temp)
        {
            dado_em_ascii[idx] = temp + 0x30;
            idx++;
            dado -= (temp * 100);
            poe_zero=2;
        }
        else if (poe_zero < 6)
        {
             dado_em_ascii[idx] = 0x30;
             idx++;
        }
        temp = dado/10;
        if (temp)
        {
            dado_em_ascii[idx] = temp + 0x30;
            idx++;
            dado -= (temp * 10);
        }
        else if (poe_zero < 6)
        {
             dado_em_ascii[idx] = 0x30;
             idx++;
        }
    dado_em_ascii[idx] = dado + 0x30;
    idx++;
    dado_em_ascii[idx] = NULL;
    poe_texto_XY (linha,coluna,dado_em_ascii);
//#endif
}

void poe_texto_XY (unsigned char linha, unsigned char coluna, char *texto)
{
//#ifdef DISPLAY
    poe_cursor_XY (linha,coluna);
    putsXLCD(texto);
//#endif
}

void poe_cursor_XY (unsigned char linha, unsigned char coluna)
{
    unsigned char base = 0x80;
    if (linha == 2) base = 0xC0;
    if (linha == 3) base = 0x94;
    if (linha == 4) base = 0xD4;
    base += coluna;
    SetDDRamAddr(base);
    BusyXLCD();

}
