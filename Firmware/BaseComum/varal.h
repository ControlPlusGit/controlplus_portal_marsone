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

int processaEth(void);
void ProcessaTagsParaOEth (int antena);

void IniciaVaral (void);
void ProcessaEnvioDeTagsNoVaral (void);

//void EnviaBytePara485 (char dado);
//void EnviaBytePara485 (unsigned char dado);
int QuantiaDeTagsPresentesNoBufferDeVaral (void);

void processaWifi ();

//Externada apenas para teste
void EnviaStringParaWifi (char * dado);

void EnviaBufferDeTagsAoPCPorEth_bloqueia (void);
void EnviaUmaTagDoBufferAoPCPorEth_bloqueia (void);

void EnviaDadosRFControlComFIFO (void); //LUCIANO: Prototipei para eliminar um warning.

int posicaoOcupadaNoBufferDeTags (int posicao); //LUCIANO: Prototipei para eliminar um warning.

void EnviaStringPara485 (char * dado);  //LUCIANO: Prototipei para eliminar um warning.
void EnviaArrayDeBytesPara485 (char *dado, int tamanho);

int tagJaPresenteNoBuffer (unsigned char * tag, int posicao, int tamanho);  //LUCIANO: Prototipei para eliminar um warning.

int adicionarTagAoBuffer (int posicao, unsigned char * dados, int tamanho); //LUCIANO: Prototipei para eliminar um warning.

int ObtemElementoDaFifo (char *Elemento);   //LUCIANO: Prototipei para eliminar um warning.

void eliminaTagDoBuffer (int posicao);  //LUCIANO: Prototipei para eliminar um warning.

int tagsPresentesNoBuffer (void);   //LUCIANO: Prototipei para eliminar um warning.

void EnviaDadosHexPara485(char *dado, int tamanhoDados);

extern volatile int LimiteDoContadorDeTX;



#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* VARAL_H */

