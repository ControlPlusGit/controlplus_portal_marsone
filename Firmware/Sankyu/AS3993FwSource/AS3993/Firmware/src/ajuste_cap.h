/* 
 * File:   ajuste_cap.h
 * Author: Marcos
 *
 * Created on 11 de Fevereiro de 2020, 19:44
 */

#ifndef AJUSTE_CAP_H
#define	AJUSTE_CAP_H

void iniciaVarreduraDeCapacitores(void);
void buscaTagPorVarreduraDeCapacitores(void);
void enviaDadosParaUSBserial(char mensagem[100], int tamanho);
void initAutoSintoniaPorReflexao(void);

//void setaTunerTeste(const TunerConfiguration *config);



#endif	/* AJUSTE_CAP_H */

