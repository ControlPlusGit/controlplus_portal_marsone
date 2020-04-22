#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fifo.h"
#include "gerenciapacotes.h"

int IdDoPortal = 0xAABB;

void main (void){
	iniciaMaquinaDeEnvioDePacotes();
	formaPacoteDeConfirmacaoParaComparacao(IdDoPortal);
	formaPaoteDeReenvioParaComparacao(IdDoPortal);

	adicionaPessoaEntrando();
	adicionaPessoaSaindo();
	adicionaPessoaEntrando();

	trataComunicacoes();
}

