/* 
 * File:   varal.h
 * Author: none
 *
 * Created on 14 de Janeiro de 2015, 17:19
 */

#ifndef VARAL_H
#define	VARAL_H


extern volatile int EstadoDeRecepcaoDaMaquinaCUP;
extern int EstadoTransmissaoDaMaquinaCUP;
extern int transmitindo;

//int maquinaDeEstadosDeRecepcaoCUP (int *estado, int dado, u16 *crc);
int maquinaDeEstadosDeRecepcaoCUP (int dado);
void OperacoesParaTickVaral (void);
int processaCUP(void);
char RotinasDeInterrupcaoDeEnvio (void);
void ProcessaTagsParaOVaral (int antena);

void IniciaVaral (void);
void ProcessaEnvioDeTagsNoVaral (void);

void EnviaBytePara485 (char dado);
int QuantiaDeTagsPresentesNoBufferDeVaral (void);
void EnviaStringPara485 (char * dado);          //LUCIANO: Prototipei essa funcao para eliminar um warning.
int posicaoOcupadaNoBufferDeTags (int posicao); //LUCIANO: Prototipei essa funcao para eliminar um warning.

extern void ld_saidas (int saida, int status);  //LUCIANO: Prototipei essa funcao para eliminar um warning.
int tagsPresentesNoBuffer (void);               //LUCIANO: Prototipei essa funcao para eliminar um warning.

extern void liga_rele1(void);                   //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void desliga_rele1(void);                //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void liga_rele2(void);                    //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void desliga_rele2(void);                //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void liga_rele3(void);                   //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void desliga_rele3(void);                //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void liga_rele4(void);                   //LUCIANO: Prototipei essa funcao para eliminar um warning.
extern void desliga_rele4(void);                //LUCIANO: Prototipei essa funcao para eliminar um warning.


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* VARAL_H */

