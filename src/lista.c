/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : lista.C                                                           *
* Descricao :                                                                 *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v1.24                                    *
* PIC : PIC24FJ256DA210                                                       *
* Versao : 1.00                                                               *
* Data : 25/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Includes <<<+++---	                      */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "as3993_public.h"
#include "global.h"

#include "lcd.h"
#include "auto_teste.h"

#include "lista.h"

/*--------------------------------------------------------------------*/
/*                 ---+++>>> Variaveis <<<+++---                      */
/*--------------------------------------------------------------------*/

Fila fila;
Pilha pilha;
ListaLigada listaLigada;
extern Tag __attribute__((far)) tags_[MAXTAG];

/*--------------------------------------------------------------------*/
/*               ---+++>>> FUNCOES() <<<+++---                        */
/*--------------------------------------------------------------------*/
/************************************************************/
/*   ---+++>>> void insereDadoPilha(char *dado) <<<+++---   */
/************************************************************/
// Funcao: void insereDadoPilha(char *dado)
// Decricao: Rotina de testes de insercao de dados na pilha.
// @param: *dado - ponteiro para tipo char
void insereDadoPilha(char *dado)
{
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    strcpy(lista->buffer,dado);
    lista->buffer[NUM_MAX_CARACTERES_CADA_ELEMENTO - 1] = '\0';

    if(!buscaDado(&pilha,(int (*)(void*, void*))comparaDados, lista))
	{
        if(!push(&pilha, (Lista*)lista))
            ;
        else
        {
        	free(lista);
        }
    }
    else
    {
        free(lista);
    }
}

/*******************************************************/
/*   ---+++>>> void *deletaDadoPilha(void) <<<+++---   */
/*******************************************************/
// Funcao: void *deletaDadoPilha(void)
// Decricao: Rotina de testes de exclusao de dados na pilha.
// @return: *dado - ponteiro para tipo char
void *deletaDadoPilha(void)
{
	void *dado;
	
    dado = pop(&pilha);
	
	return(dado);
}

/**************************************************************/
/*   ---+++>>> unsigned int liberaDadoPilha(void) <<<+++---   */
/**************************************************************/
// Funcao: unsigned int liberaDadoPilha(void)
// Decricao: Libera o dado da pilha aplicando o free() apos o uso.
// @return: 0 se dado eliminado com sucesso, 1 se heap ja estava vazio.
unsigned int liberaDadoPilha(void)
{
    Lista *dadoRetirado = NULL;

    dadoRetirado = deletaDadoPilha();
    if(dadoRetirado)
    {   //TODO: o dado deve ser utilizado aqui antes de ser aplicado o free() nele.
        
        free(dadoRetirado);
        dadoRetirado = NULL;
        return 0;
    }
    else
        return 1;
}

/******************************************************/
/*   ---+++>>> void mostraDadoPilha(void) <<<+++---   */
/******************************************************/
// Funcao: void mostraDadoPilha(void)
// Decricao: Mostra os dados da pilha.
void mostraDadoPilha(void)
{
    mostraDado(&pilha, (_DISPLAY_) mostra);
}

/****************************************************************/
/*   ---+++>>> unsigned int tamanhoDadosPilha(void) <<<+++---   */
/****************************************************************/
// Funcao: unsigned int tamanhoDadosPilha(void)
// Decricao: Mostra o tamanho ocupado pela pilha.
// @return: retorna quantos elementos tem na pilha.
unsigned int tamanhoDadosPilha(void)
{
    return(tamanhoDados(&pilha));
}

/*********************************************************************/
/*   ---+++>>> unsigned int quantoRestaEspacoPilha(void) <<<+++---   */
/*********************************************************************/
// Funcao: unsigned int quantoRestaEspacoPilha(void)
// Decricao: Mostra o quanto ainda tem de espaco na pilha.
// @return: retorna quanto de espaco tem pilha.
unsigned int quantoRestaEspacoPilha(void)
{
    return(quantoRestaEspaco(&pilha));
}

/*************************************************************/
/*   ---+++>>> unsigned int pilhaEstaVazia(void) <<<+++---   */
/*************************************************************/
// Funcao: unsigned int pilhaEstaVazia(void)
// Decricao: Verifica se a pilha esta vazia.
// @return: retorna quanto de espaco tem pilha.
unsigned int pilhaEstaVazia(void)
{
    return(heapEstaVazio(&pilha));
}

/***********************************************************/
/*   ---+++>>> void insereDadoFila(char *dado) <<<+++---   */
/***********************************************************/
// Funcao: void insereDadoFila(char *dado)
// Decricao: Rotina de testes de insercao de dados na fila.
// @param: *dado - ponteiro para tipo char
void insereDadoFila(char *dado)
{
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    strcpy(lista->buffer,dado);
    lista->buffer[NUM_MAX_CARACTERES_CADA_ELEMENTO - 1] = '\0';
    
    if(!buscaDado(&fila,(int (*)(void*, void*))comparaDados, (char*)lista))
	{
        if(!enfilera(&fila, (Lista*)lista))
            ;
        else
        {
        	free(lista);
        }
    }
    else
    {
        free(lista);
    }
}

/******************************************************/
/*   ---+++>>> void *deletaDadoFila(void) <<<+++---   */
/******************************************************/
// Funcao: void insereTagNaFila(unsigned int indice)
// Decricao: Rotina para inserir a EPC da TAG na fila.
// @param: indice - indice da TAG
void insereTagNaFila(unsigned int indice)
{
    unsigned int i = 0;
    char buffer[4] = {0};
    const unsigned char EPC_LENGTH = 12;
    char dado[25] = {0};
    
    memset(dado,0,strlen(dado));                                // limpamos o buffer de dados
    for(i=0; i < EPC_LENGTH; i++)
    {
        itoa(buffer,(unsigned long)tags_[indice].epc[i],16);    // converte o conteudo do EPC para ascii na base 16
        strcat(dado,buffer);                                    // e armazena no buffer
    }
    
    hex2asc(dado, strlen(dado));                                // converte para ASCII (essa funcao so converte se todos os caracteres forem imprimiveis)
    
    dado[NUM_MAX_CARACTERES_CADA_ELEMENTO - 1] = '\0';          // garantimos o final da string em seu ultimo elemento

    insereDadoFila(dado);                                       // inserimos o EPC da TAG na FILA
}

/******************************************************/
/*   ---+++>>> void *deletaDadoFila(void) <<<+++---   */
/******************************************************/
// Funcao: void *deletaDadoFila(void)
// Decricao: Rotina de testes de exclusao de dados na fila.
// @return: *dado - ponteiro para tipo char
void *deletaDadoFila(void)
{
	void *dado;
	
    dado = retiraDaFila(&fila);
	
	return(dado);
}

/*************************************************************/
/*   ---+++>>> unsigned int liberaDadoFila(void) <<<+++---   */
/*************************************************************/
// Funcao: unsigned int liberaDadoFila(void)
// Decricao: Libera o dado da fila aplicando o free() apos o uso.
// @return: 0 se dado eliminado com sucesso, 1 se heap ja estava vazio.
unsigned int liberaDadoFila(void)
{
    /*Lista *dadoRetirado = NULL;

    dadoRetirado = deletaDadoFila();
    if(dadoRetirado)
    {   //TODO: o dado deve ser utilizado aqui antes de ser aplicado o free() nele.
        poe_texto_XY(4,5,(char*)dadoRetirado);
        //Fim do TODO
        free(dadoRetirado);
        dadoRetirado = NULL;
        return 0;
    }
    else*/
        return 1;
}

/*****************************************************/
/*   ---+++>>> void mostraDadoFila(void) <<<+++---   */
/*****************************************************/
// Funcao: void mostraDadoFila(void)
// Decricao: Mostra os dados da fila.
void mostraDadoFila(void)
{
    mostraDado(&fila, (_DISPLAY_) mostra);
}

/***************************************************************/
/*   ---+++>>> unsigned int tamanhoDadosFila(void) <<<+++---   */
/***************************************************************/
// Funcao: unsigned int tamanhoDadosFila(void)
// Decricao: Mostra o tamanho ocupado pela fila.
// @return: retorna quantos elementos tem na fila.
unsigned int tamanhoDadosFila(void)
{
    return(tamanhoDados(&fila));
}

/********************************************************************/
/*   ---+++>>> unsigned int quantoRestaEspacoFila(void) <<<+++---   */
/********************************************************************/
// Funcao: unsigned int quantoRestaEspacoFila(void)
// Decricao: Mostra o quanto ainda tem de espaco na fila.
// @return: retorna quanto de espaco tem fila.
unsigned int quantoRestaEspacoFila(void)
{
    return(quantoRestaEspaco(&fila));
}

/************************************************************/
/*   ---+++>>> unsigned int filaEstaVazia(void) <<<+++---   */
/************************************************************/
// Funcao: unsigned int filaEstaVazia(void)
// Decricao: Verifica se a fila esta vazia.
// @return: retorna quanto de espaco tem fila.
unsigned int filaEstaVazia(void)
{
    return(heapEstaVazio(&fila));
}

/******************************************************************/
/*   ---+++>>> void insereDadoComecoLista(char *dado) <<<+++---   */
/******************************************************************/
// Funcao: void insereDadoComecoLista(char *dado)
// Decricao: Rotina de testes de insercao de dados no comeco da lista.
// @param: *dado - ponteiro para tipo char
void insereDadoComecoLista(char *dado)
{
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    strcpy(lista->buffer,dado);
    lista->buffer[NUM_MAX_CARACTERES_CADA_ELEMENTO - 1] = '\0';

    if(!buscaDado(&listaLigada,(int (*)(void*, void*))comparaDados, lista))
	{
        if(!adicionaComecoLista(&listaLigada, (Lista*)lista))
            ;
        else
        {
        	free(lista);
        }
    }
    else
    {
        free(lista);
    }
}

/*****************************************************************/
/*   ---+++>>> void insereDadoFinalLista(char *dado) <<<+++---   */
/*****************************************************************/
// Funcao: void insereDadoFinalLista(char *dado)
// Decricao: Rotina de testes de insercao de dados no final da lista.
// @param: *dado - ponteiro para tipo char
void insereDadoFinalLista(char *dado)
{
    Lista *lista = (Lista*)malloc(sizeof(Lista));
    strcpy(lista->buffer,dado);
    lista->buffer[NUM_MAX_CARACTERES_CADA_ELEMENTO - 1] = '\0';

    if(!buscaDado(&listaLigada,(int (*)(void*, void*))comparaDados, lista))
	{
        if(!adicionaFinalLista(&listaLigada, (Lista*)lista))
            ;
        else
        {
        	free(lista);
        }
    }
    else
    {
        free(lista);
    }
}

/**************************************************************/
/*   ---+++>>> void deletaDadoLista(Lista *lista) <<<+++---   */
/**************************************************************/
// Funcao: void deletaDadoLista(Lista *lista)
// Decricao: Rotina de testes de exclusao de dados na lista.
// @param: *lista - ponteiro para tipo Lista
void deletaDadoLista(Lista *lista)
{
    if(buscaDado(&listaLigada,(int (*)(void*, void*))comparaDados, lista))
    {
        No *no = buscaDado(&listaLigada,(int (*)(void*, void*))comparaDados, lista);
        if (no)
        {
            apagaNo(&listaLigada, no);
        }
    }
    else
        ;
}

/******************************************************/
/*   ---+++>>> void mostraDadoLista(void) <<<+++---   */
/******************************************************/
// Funcao: void mostraDadoLista(void)
// Decricao: Mostra os dados da lista.
void mostraDadoLista(void)
{
    mostraDado(&listaLigada, (_DISPLAY_) mostra);
}

/****************************************************************/
/*   ---+++>>> unsigned int tamanhoDadosLista(void) <<<+++---   */
/****************************************************************/
// Funcao: unsigned int tamanhoDadosLista(void)
// Decricao: Mostra o tamanho ocupado pela lista.
// @return: retorna quantos elementos tem na lista.
unsigned int tamanhoDadosLista(void)
{
    return(tamanhoDados(&listaLigada));
}

/*********************************************************************/
/*   ---+++>>> unsigned int quantoRestaEspacoLista(void) <<<+++---   */
/*********************************************************************/
// Funcao: unsigned int quantoRestaEspacoLista(void)
// Decricao: Mostra o quanto ainda tem de espaco na lista.
// @return: retorna quanto de espaco tem lista.
unsigned int quantoRestaEspacoLista(void)
{
    return(quantoRestaEspaco(&listaLigada));
}

/*************************************************************/
/*   ---+++>>> unsigned int listaEstaVazia(void) <<<+++---   */
/*************************************************************/
// Funcao: unsigned int listaEstaVazia(void)
// Decricao: Verifica se a lista esta vazia.
// @return: retorna quanto de espaco tem lista.
unsigned int listaEstaVazia(void)
{
    return(heapEstaVazio(&listaLigada));
}

/********************************************************************/
/*   ---+++>>> void inicializaLista(ListaLigada *lista) <<<+++---   */
/********************************************************************/
// Funcao: void inicializaLista(ListaLigada *lista)
// Decricao: Inicializa a lista
// @param: poteiro da lista ligada
void inicializaListaLigada(ListaLigada *lista)
{
    lista->comeco = NULL;
    lista->fim = NULL;
    lista->atual = NULL;
}

/******************************************************/
/*   ---+++>>> void inicializaLista(void) <<<+++---   */
/******************************************************/
// Funcao: void inicializaLista(void)
// Decricao: Inicializa a lista
void inicializaLista(void)
{
    inicializaListaLigada(&listaLigada);
}

/********************************************************************************************/
/*   ---+++>>> unsigned int adicionaComecoLista(ListaLigada *lista, void *dado) <<<+++---   */
/********************************************************************************************/
// Funcao: unsigned int adicionaComecoLista(ListaLigada *lista, void *dado)
// Decricao: Adiciona um dado no comeco da lista
// @param: lista
// @param: dado
// @return: boolean - 1 se nao adicionou e 0 se dado foi adicionado a lista
unsigned int adicionaComecoLista(ListaLigada *lista, void *dado)
{
    if(quantoRestaEspaco(lista))
    {
        adicionaComeco(lista, dado);
        return(0);
    }
    else
    {
        return(1);
    }
}

/*******************************************************************************************/
/*   ---+++>>> unsigned int adicionaFinalLista(ListaLigada *lista, void *dado) <<<+++---   */
/*******************************************************************************************/
// Funcao: unsigned int adicionaFinalLista(ListaLigada *lista, void *dado)
// Decricao: Adiciona um dado no final da lista
// @param: lista
// @param: dado
// @return: 1 se nao foi adicionado e 0 se dado foi adicionado a lista
unsigned int adicionaFinalLista(ListaLigada *lista, void *dado)
{
    if(quantoRestaEspaco(lista))
    {
        adicionaFinal(lista, dado);
        return(0);
    }
    else
    {
        return(1);
    }
}

/******************************************************************/
/*   ---+++>>> int comparaDados(Lista *l1, Lista *l2) <<<+++---   */
/******************************************************************/
// Funcao: int comparaDado(Lista *l1, Lista *l2)
// Decricao: Compara elementos do heap
// @param: Lista *l1
// @param: Lista *l2
// @return: 0 se os dados forem iguais
int comparaDados(Lista *l1, Lista *l2)
{
    return(strcmp(l1->buffer, l2->buffer));
}

/*******************************************************************************************/
/*   ---+++>>> No *buscaDado(ListaLigada *lista, COMPARA compara , void* dado) <<<+++---   */
/*******************************************************************************************/
// Funcao: No *buscaDado(ListaLigada *lista, COMPARA compara , void* dado)
// Decricao: Busca um dado no heap
// @param: ListaLigada *lista
// @param: COMPARA compara
// @param: void* dado
// @return: ponteiro tipo No se dado encontrado, ou NULL se nenhum dado foi encontrado
No *buscaDado(ListaLigada *lista, COMPARA compara , void* dado)
{
    No *no = lista->comeco;
    while(no != NULL)
    {
        if(compara(no->dado, dado) == 0)
        {
            return no;
        }
        no = no->proximo;
    }
    return NULL;
}

/********************************************************************/
/*   ---+++>>> void apagaNo(ListaLigada *lista, No *no) <<<+++---   */
/********************************************************************/
// Funcao: void apagaNo(ListaLigada *lista, No *no)
// Decricao: Apaga um no do heap
// @param: lista
// @param: no
void apagaNo(ListaLigada *lista, No *no)
{
    Lista *dadoEliminado = NULL;

    if (no == lista->comeco)
    {
        if (lista->comeco->proximo == NULL)
        {
            lista->comeco = lista->fim = NULL;
        }
        else
        {
            lista->comeco = lista->comeco->proximo;
        }
    }
    else
    {
        No *tmp = lista->comeco;
        while (tmp != NULL && tmp->proximo != no)
        {
            tmp = tmp->proximo;
        }
        if (tmp != NULL)
        {
            tmp->proximo = no->proximo;
        }
    }
    dadoEliminado = no->dado;
    free(dadoEliminado);
    dadoEliminado = NULL;
    free(no);
}

/**********************************************************************************/
/*   ---+++>>> void mostraDado(ListaLigada *lista, _DISPLAY_ display) <<<+++---   */
/**********************************************************************************/
// Funcao: void mostraDado(ListaLigada *lista, _DISPLAY_ display)
// Decricao: Mostra os dados da Lista Ligada
// @param: lista
// @param: mostra
void mostraDado(ListaLigada *lista, _DISPLAY_ mostra)
{
    No *atual = lista->comeco;
    
    if(atual != NULL)
    {
    	printf("Dados:\n");
	}
	else
	{
		printf("Heap vazio!\n");
	}
	
	while(atual != NULL)
    {
        mostra(atual->dado);
        atual = atual->proximo;
    }
}

/*****************************************************/
/*   ---+++>>> void mostra(Lista* lista) <<<+++---   */
/*****************************************************/
// Funcao: void mostra(Lista* lista)
// Decricao: Mostra um dado presente no heap
// @param: lista
void mostra(Lista* lista)
{
    printf("%s\n", lista->buffer);
}

/***********************************************************/
/*   ---+++>>> void inicializaFila(Fila *fila) <<<+++---   */
/***********************************************************/
// Funcao: void inicializaFila(Fila *fila)
// Decricao: Inicializa a fila tipo FIFO
void inicializaFila(void)
{
    inicializaListaLigada(&fila);
}

/***************************************************************/
/*   ---+++>>> void enfilera(Fila *fila, void *no) <<<+++---   */
/***************************************************************/
// Funcao: void enfilera(Fila *fila, void *no)
// Decricao: Adiciona um dado a fila tipo FIFO
// @param: fila
// @param: no
// @return: retorna 0 se dado foi adicionado com sucesso,
//         e retorna 1 se a fila estiver cheia.
unsigned int enfilera(Fila *fila, void *no)
{
    if(quantoRestaEspaco(fila))
    {
        adicionaComeco(fila, no);
        return(0);
    }
    else
    {
        return(1);
    }
}

/**********************************************************/
/*   ---+++>>> void *retiraDaFila(Fila *fila) <<<+++---   */
/**********************************************************/
// Funcao: void *retiraDaFila(Fila *fila)
// Decricao: Remove um dado a fila tipo FIFO
// @param: fila
// @return: ponteiro para o dado retirado da fila
void *retiraDaFila(Fila *fila)
{
    No *temp = fila->comeco;
    void *dado;

    if(fila->comeco == NULL)
    {
        dado = NULL;
    }
    else if(fila->comeco == fila->fim)
    {
        fila->comeco = fila->fim = NULL;
        dado = temp->dado;
        free(temp);
    }
    else
    {
        while(temp->proximo != fila->fim)
        {
            temp = temp->proximo;
        }
        fila->fim = temp;
        temp = temp->proximo;
        fila->fim->proximo = NULL;
        dado = temp->dado;
        free(temp);
    }
    return dado;
}

/**************************************************************/
/*   ---+++>>> void inicializaPilha(Pilha *pilha) <<<+++---   */
/**************************************************************/
// Funcao: void inicializaPilha(Pilha *pilha)
// Decricao: Inicializa a pilha tipo FILO
void inicializaPilha(void)
{
    inicializaListaLigada(&pilha);
}

/***********************************************************************/
/*   ---+++>>> unsigned int push(Pilha *pilha, void* dado) <<<+++---   */
/***********************************************************************/
// Funcao: unsigned int push(Pilha *pilha, void* dado)
// Decricao: Adiciona um dado da pilha tipo FILO
// @param: pilha
// @param: dado
// @return: 0 se dado foi inserido na pilha, e 0 se nao foi adicionado
unsigned int push(Pilha *pilha, void* dado)
{
    if(quantoRestaEspaco(pilha))
    {
        adicionaComeco(pilha, dado);
        return(0);
    }
    else
    {
        return(1);
    }
}

/***************************************************/
/*   ---+++>>> void *pop(Pilha *pilha) <<<+++---   */
/***************************************************/
// Funcao: void *pop(Pilha *pilha)
// Decricao: retira um dado da pilha tipo FILO
// @param: pilha
// @return: ponteiro para o dado retirado da pilha
void *pop(Pilha *pilha)
{
    No *no = pilha->comeco;

    if (no == NULL) {
        return NULL;
    }
    else if(no == pilha->fim)
    {
        pilha->comeco = pilha->fim = NULL;
        void *dado = no->dado;
        free(no);
        return dado;
    }
    else
    {
        pilha->comeco = pilha->comeco->proximo;
        void *dado = no->dado;
        free(no);
        return dado;
    }
}

/*******************************************************************************/
/*   ---+++>>> void adicionaComeco(ListaLigada *lista, void *dado) <<<+++---   */
/*******************************************************************************/
// Funcao: void adicionaComeco(ListaLigada *lista, void *dado)
// Decricao: Adiciona dado no comeco
// @param: lista
// @param: dado
void adicionaComeco(ListaLigada *lista, void *dado)
{
    No *no = (No*) malloc(sizeof(No));
    no->dado = dado;
    if(lista->comeco == NULL)
    {
        lista->fim = no;
        no->proximo = NULL;
    }
    else
    {
        no->proximo = lista->comeco;
    }
    lista->comeco = no;
}

/******************************************************************************/
/*   ---+++>>> void adicionaFinal(ListaLigada *lista, void* dado) <<<+++---   */
/******************************************************************************/
// Funcao: void adicionaFinal(ListaLigada *lista, void* dado)
// Decricao: Adiciona dado no fim
// @param: lista
// @param: dado
void adicionaFinal(ListaLigada *lista, void* dado)
{
    No *no = (No*) malloc(sizeof(No));
    no->dado = dado;
    no->proximo = NULL;
    if (lista->comeco == NULL)
    {
        lista->comeco = no;
    }
    else
    {
        lista->fim->proximo = no;
    }
    lista->fim = no;
}

/*************************************************************************/
/*   ---+++>>> unsigned int tamanhoDados(ListaLigada *lista) <<<+++---   */
/*************************************************************************/
// Funcao: unsigned int tamanhoDados(ListaLigada *lista)
// Decricao: Retorna quantos elementos estao ocupando o heap.
// @param: lista
// @return: Retorna a quantidade de elementos presentes no heap
unsigned int tamanhoDados(ListaLigada *lista)
{
    unsigned int i = 0;
    No *no = lista->comeco;

    if(lista->comeco == NULL)
        return(0);
    else
    {

        while(no != NULL)
        {
            i++;
            no = no->proximo;
        }
        return(i);
    }
}

/******************************************************************************/
/*   ---+++>>> unsigned int quantoRestaEspaco(ListaLigada *lista) <<<+++---   */
/******************************************************************************/
// Funcao: unsigned int quantoRestaEspaco(ListaLigada *lista)
// Decricao: Retorna o quanto resta de espaco no heap
// @param: lista
// @return: Retorna o numero de elementos possiveis para adicionar no heap
unsigned int quantoRestaEspaco(ListaLigada *lista)
{
    return(NUM_MAX_ELEMENTOS - tamanhoDados(lista));
}

/**************************************************************************/
/*   ---+++>>> unsigned int heapEstaVazio(ListaLigada *lista) <<<+++---   */
/**************************************************************************/
// Funcao: unsigned int heapEstaVazio(ListaLigada *lista)
// Decricao: Verifica se a fila esta vazia
// @param: lista
// @return: Retorna 1 se a fila estiver vazia. Retorna 0 se nao estiver vazia
unsigned int heapEstaVazio(ListaLigada *lista)
{
    if(lista->comeco == NULL)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

/****************************************************/
/*   ---+++>>> void hex2asc(char* dado) <<<+++---   */
/****************************************************/
/* Funcao: void hex2asc(char* dado)
 * Descricao: Funcao que converte caracteres hexadecimais para ASCII (apenas se todos os caracteres forem imprimiveis)
 * @param: *dado - string de dados em hexadecimal
 * @param: strSize - comprimento da string
 * @return: 0 se houve algum erro, e 1 se teve sucesso
 */
unsigned int hex2asc(char* dado, unsigned int strSize)
{
	unsigned int i,j = 0;
    unsigned char nibble1 = 0;
    unsigned char nibble2 = 0;
    char temp[strSize];
    
    memset(temp,0,strlen(temp));
    
    for(i=0, j=0; i < strlen(dado); i+=2, j++)
    {	
    	if(*(dado+i+1) == '\0')							// verifica se o proximo elemento eh NULL
    		break;										// se sim, entao sai do loop
    		
    	if((*(dado+i) >= '0') && (*(dado+i) <= '9'))
    		nibble1 = *(dado+i) - '0';
    	else if((*(dado+i) >= 'A') && (*(dado+i) <= 'F'))
    	{
    		nibble1 = *(dado+i) - 'A';
    		nibble1 = nibble1 + 10;
		}
        else
            return 0;
    		
    	if((*(dado+i+1) >= '0') && (*(dado+i+1) <= '9'))
    		nibble2 = *(dado+i+1) - '0';
    	else if((*(dado+i+1) >= 'A') && (*(dado+i+1) <= 'F'))
    	{
    		nibble2 = *(dado+i+1) - 'A';
    		nibble2 = nibble2 + 10;
		}
        else
            return 0;
    		   	
    	temp[j] = (nibble1 << 4) + nibble2;
    	
    	if((temp[j] < ' ') || (temp[j] > '~'))			// verifica se esta fora da faixa de caracteres imprimiveis
    		return 0;									// se estiver... retorna
    	
    }
    temp[j] = '\0';										// finaliza a nova string com NULL
    
    strcpy(dado,temp);									// copia para o ponteiro de origem
    
    return 1;
}

/*--------------------------------------------------------------------*/
/*             ---+++>>> Fim do arquivo LISTA.C <<<+++---             */
/*--------------------------------------------------------------------*/		
