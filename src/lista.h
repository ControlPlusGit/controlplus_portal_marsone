/******************************************************************************
* CONTROL-UP - Controle de Sistemas Ltda.                                     *
*******************************************************************************
* Projeto : Leitor                                                            *
* Arquivo : lista.H                                                           *
* Descricao : Header do lista.c                                               *
* Plataforma: WINDOWS 10                                                      *
* Compilador: Microchip MPLAB X xc16 v.1.24                                   *
* PIC : PIC24FJ256DA210                                                       *
* Versao :                                                                    *
* Data : 25/10/2016                                                           *
* Autor : Luciano Mendes Almeida                                              *
*******************************************************************************/

#ifndef __LISTA_H
#define __LISTA_H

/*--------------------------------------------------------------------*/
/*                   ---+++>>> Constantes <<<+++---	                  */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/*                     ---+++>>> Defines <<<+++---	                  */
/*--------------------------------------------------------------------*/

/*********************************************************************/
// ATENCAO: Sempre que alterar o valor de NUM_MAX_ELEMENTOS_FILA     //
// ou NUM_MAX_CARACTERES_CADA_ELEMENTO, nao esquecer de alterar      //
// tambem o heap reservado no MPLAB para o projeto.                  //
// Va em Propriedades do projeto (File->project properties)          //
// clique na categoria pic30-Id, encontre a opcao heap               //
// e configure para o resultado do produto desta duas constantes     //
// VALOR = NUM_MAX_ELEMENTOS_FILA * NUM_MAX_CARACTERES_CADA_ELEMENTO //
// Corrija o valor  tanto para o DEBUG quanto para o RELEASE!        //
/*********************************************************************/
/*********************************************************************/
#define NUM_MAX_ELEMENTOS 150                                        //
#define NUM_MAX_CARACTERES_CADA_ELEMENTO 13                          //
/*********************************************************************/
/*********************************************************************/

/*--------------------------------------------------------------------*/
/*                     ---+++>>> Structs <<<+++---	                  */
/*--------------------------------------------------------------------*/

typedef struct _lista
{
    char buffer[NUM_MAX_CARACTERES_CADA_ELEMENTO];
}Lista;

typedef struct _no
{
    void *dado;
    struct _no *proximo;
}No;

typedef struct _listaLigada
{
    No *comeco;
    No *fim;
    No *atual;
}ListaLigada;

typedef ListaLigada Fila;
typedef ListaLigada Pilha;

typedef void(*_DISPLAY_)(void*);
typedef int(*COMPARA)(void*,void*);

/*--------------------------------------------------------------------*/
/*                    ---+++>>> Prototipagem <<<+++---	              */
/*--------------------------------------------------------------------*/

void inicializaListaLigada(ListaLigada *lista);
void adicionaComeco(ListaLigada *lista, void *dado);
void adicionaFinal(ListaLigada *lista, void* dado);
unsigned int tamanhoDados(ListaLigada *lista);
unsigned int quantoRestaEspaco(ListaLigada *lista);
unsigned int heapEstaVazio(ListaLigada *lista);
int comparaDados(Lista *l1, Lista *l2);
No *buscaDado(ListaLigada *lista, COMPARA comparaDados , void* dado);
void apagaNo(ListaLigada *lista, No *no);
void mostraDado(ListaLigada *lista, _DISPLAY_ mostra);
void mostra(Lista* lista);

void inicializaLista(void);
unsigned int adicionaComecoLista(ListaLigada *lista, void *dado);
unsigned int adicionaFinalLista(ListaLigada *lista, void* dado);
void insereDadoComecoLista(char *dado);
void insereDadoFinalLista(char *dado);
void deletaDadoLista(Lista *lista);
void mostraDadoLista(void);
unsigned int tamanhoDadosLista(void);
unsigned int quantoRestaEspacoLista(void);
unsigned int listaEstaVazia(void);

void inicializaFila(void);
unsigned int enfilera(Fila *fila, void *no);
void *retiraDaFila(Fila *fila);
void insereDadoFila(char *dado);
void insereTagNaFila(unsigned int indice);
void *deletaDadoFila(void);
unsigned int liberaDadoFila(void);
void mostraDadoFila(void);
unsigned int tamanhoDadosFila(void);
unsigned int quantoRestaEspacoFila(void);
unsigned int filaEstaVazia(void);

void inicializaPilha(void);
unsigned int push(Pilha *pilha, void* dado);
void *pop(Pilha *pilha);
void insereDadoPilha(char *dado);
void *deletaDadoPilha(void);
unsigned int liberaDadoPilha(void);
void mostraDadoPilha(void);
unsigned int tamanhoDadosPilha(void);
unsigned int quantoRestaEspacoPilha(void);
unsigned int pilhaEstaVazia(void);

unsigned int hex2asc(char* dado, unsigned int strSize);

/*--------------------------------------------------------------------*/
/*            ---+++>>> Fim do arquivo LISTA.H <<<+++---              */
/*--------------------------------------------------------------------*/
#endif

