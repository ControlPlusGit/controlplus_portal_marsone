/* 
 * File:   cancelas.h
 * Author: Marcos
 *
 * Created on 12 de Novembro de 2019, 16:56
 */

#ifndef CANCELAS_H
#define	CANCELAS_H

#include "perifericos.h"
#include "eth.h"

void travaCancelaDoPortal(void);
void destravaCancelaDoPortal(void);
void travaCancelaDoPortalSemTemporizacao(void);
void operacoesEmTickParaCancelas(void);
void iniciaCancelas(void);



#endif	/* CANCELAS_H */

