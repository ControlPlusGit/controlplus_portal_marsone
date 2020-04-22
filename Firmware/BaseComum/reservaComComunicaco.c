#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_MAXIMO_DAS_MENSAGENS 50
char LinkPCParaEquipamento[TAMANHO_MAXIMO_DAS_MENSAGENS];
char LinkEquipamentoParaPC[TAMANHO_MAXIMO_DAS_MENSAGENS];
FILE *Reserva;

int adicionaEvento(char *Evento){
	int Contador;
	Reserva = fopen("reservaEquipamento.txt", "a");
	
	while (*Evento != 0){
		fputc(*Evento, Reserva);
		Evento = Evento + 1;
		Contador = Contador + 1;
		if (Contador >= TAMANHO_MAXIMO_DAS_MENSAGENS){
			return -1;
		}
	}
	
	fclose(Reserva);
	return 0;
}

int eventoPresentes(void){
	int Tamanho;
	Reserva = fopen("reservaEquipamento.txt", "r");
	fseek(Reserva, 0L, SEEK_END);
	Tamanho = ftell(Reserva);
	fclose(Reserva);
	return Tamanho;
}

char retornaDadoDeEvento(void){
	static char ContadorDoPonteiro;
	char Dado;
	Reserva = fopen("reservaEquipamento.txt", "r");
	fseek(Reserva, ContadorDoPonteiro, 0);
	Dado = fgetc(Reserva);
	fclose(Reserva);
	return Dado;
}

void apagaTodosOsEventos(void){
	Reserva = fopen("reservaEquipamento.txt", "w");
	fclose(Reserva);
}

void recebeDadosEquipamento(char *Dados){
	char Str[TAMANHO_MAXIMO_DAS_MENSAGENS];
	Reserva = fopen("reservaEquipamento.txt", "a");
	while(*Dados != 0){
		fputc(*Dados, Reserva);
		Dados = Dados + 1;
	}
	fclose(Reserva);
}

void recebeDadosPC(char *Dados){
	char Str[TAMANHO_MAXIMO_DAS_MENSAGENS];
	Reserva = fopen("reservaPC.txt", "a");
	while(*Dados != 0){
		fputc(*Dados, Reserva);
		Dados = Dados + 1;
	}
	fclose(Reserva);
}

void enviaDadosEquipamento(char *Dados){
	(void)sprintf(LinkEquipamentoParaPC, "%s", Dados);

	//recebeDadosPC(LinkEquipamentoParaPC);
	
}

void enviaReservaDoEquipamentoAoPC(){
	int Contador, TamanhoDaMemoria;
	int Dado;
	Reserva = fopen("reservaEquipamento.txt", "r");
	/*
	fseek(Reserva, 0, SEEK_END);
	TamanhoDaMemoria = ftell(Reserva);
	fseek(Reserva, 0, SEEK_SET)
	;
	for(Contador = 0;Contador < TamanhoDaMemoria;Contador = Contador + 1){
		Dado = fgetc(Reserva);
		printf(Dado);
		
	}
	*/
	
//	Dado = fgetc(Reserva);
	do {
		Dado = fgetc(Reserva);
		printf("%c", Dado);		
	} while (Dado != EOF);
	printf("\n");
	fclose(Reserva);
}

void enviaDadosPC(char *Dados){
	(void)sprintf(LinkPCParaEquipamento, "%s", Dados);
	if (strcmp(LinkPCParaEquipamento, "haDados?") == 0){
		printf("comando ha dados?\n");
		if (eventoPresentes()){
			printf("SIN\n");
		} else {
			printf("NAO\n");
		}
	} 

	if (strcmp(LinkPCParaEquipamento, "enviarDados") == 0){
		printf("comando enviar dados\n");
		enviaReservaDoEquipamentoAoPC();
	} 

	if (strcmp(LinkPCParaEquipamento, "ApagarDados") == 0){
		printf("comando apagar dados\n");
		apagaTodosOsEventos();
	} 


}


int main (char *argc, int argv){
	char Str[TAMANHO_MAXIMO_DAS_MENSAGENS];

	adicionaEvento("Teste");
	adicionaEvento("123");
	adicionaEvento("abc");
		
	enviaDadosPC("haDados?");
	enviaDadosPC("enviarDados");
	enviaDadosPC("ApagarDados");
	
	Reserva = fopen("reserva.txt", "a");
	sprintf(Str, "Teste");
	fwrite(Str, 1, sizeof(Str), Reserva);
	printf("oi mundo!");
	return -1;
	
}
