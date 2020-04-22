/* 
 * File:   i2c.h
 * Author: none
 *
 * Created on 9 de Junho de 2015, 17:24
 */

#ifndef I2C_H
#define	I2C_H

//void SetarHoraRTC (int minuto, int hora, int dia, int mes, int ano);
int CompararHoraDadaComADoRTC (int minutoDado, int horaDado, int diaDado, int mesDado, int anoDado);
void LigaI2CMemoria (void);


int EscreveArray (int endereco, char *dados, char quantia);
int EscreverNaEEprom (int endereco, unsigned char dado);
int LerDadosDaEEprom (int endereco, unsigned char *dado);
void LigaI2CRelogio (void); //LUCIANO: Prototipei para eliminar um warning.
void testeI2C (void);       //LUCIANO: Prototipei para eliminar um warning.

extern int contadori2c;
extern char strDiaDaSemana[10];
extern char strMes[10];
//extern char strAno[10];

extern char stringData[20];
extern char stringHora[20];
extern int minuto;

extern int ContadorDePOR;
extern int ContadorDeBOR;
extern int ContadorDeEXTR;
extern int ContadorDeTRAPR;
extern int ContadorDeIOPUWR;
extern unsigned int ContadorDeEnviosBemSucedidosGPRS;
extern unsigned int ContadorDeTentativasDeEnvioGPRS;

void ZeraContadoresDeReset (void);
void RegistraSucessoGPRS (void);
void RegistraTentativaGPRS (void);
void ZeraContadoresGPRS (void);
void AtualizaContadoresDeReset (void);

void ObtemContadoresVolit (void);
void SalvaCOntadoresVolit (void);


#define VELOCIDADE_I2C 100000

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

