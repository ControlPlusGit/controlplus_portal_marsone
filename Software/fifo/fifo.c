#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"

char TabelaFifo [TAMANHO_MAXIMO_DA_TABELA][TAMANHO_MAXIMO_DO_ITEM];

void movimentaPilhaAbaixo (void){
	int Contador;

	for (Contador = 0; Contador < (TAMANHO_MAXIMO_DA_TABELA - 1);Contador = Contador + 1){
		memcpy(TabelaFifo[Contador],TabelaFifo[Contador + 1], TAMANHO_MAXIMO_DO_ITEM);
		memset(&TabelaFifo[Contador + 1][0], 0, TAMANHO_MAXIMO_DO_ITEM);
	}
}

int removePrimeiroItem(void){
	memset(TabelaFifo, 0, TAMANHO_MAXIMO_DO_ITEM);
	movimentaPilhaAbaixo();
	return 0;
}

int procuraPosicaoLivreNaFifo(void){
	int Contador;

	for (Contador = 0;Contador < TAMANHO_MAXIMO_DA_TABELA;Contador = Contador + 1){
		if (TabelaFifo[Contador][0] == 0) return Contador;
	}	
	return -1;
}

int adicionaElementoAFifo(char *Item){
	int Posicao;

	Posicao = procuraPosicaoLivreNaFifo();
	if (Posicao < 0) return -1;
	memcpy(&TabelaFifo [Posicao][0], Item, TAMANHO_MAXIMO_DO_ITEM);
	return 0;
}

int retornaComItemDaLista(char *Item){
	if (TabelaFifo [0][0] != 0){
		memcpy(Item, &TabelaFifo [0][0], TAMANHO_MAXIMO_DO_ITEM);
		return 0;
	}
	return -1;
}

/*
void main (void){
	char Item[TAMANHO_MAXIMO_DO_ITEM];
	adicionaElementoAFifo("123");
	adicionaElementoAFifo("456");
	adicionaElementoAFifo("789");

	//removePrimeiroItem();

	
	while(1){
		if (retornaComItemDaLista(Item) > -1){
			printf("%s\n", Item);
			removePrimeiroItem();
		}
	}
}
*/