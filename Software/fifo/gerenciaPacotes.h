#ifndef GERENCIA_DE_PACOTES_H
#define GERENCIA_DE_PACOTES_H

void iniciaMaquinaDeEnvioDePacotes (void);
int trataMaquinaDeEnvioDePacotes (int Estado);
void trataComunicacoes(void);
void adicionaPessoaEntrando(void);
void adicionaPessoaSaindo(void);

void trataRecepcaoDeDadosPela485(char Dado);

void formaPacoteDeConfirmacaoParaComparacao(int IdDoPortal);
void formaPaoteDeReenvioParaComparacao(int IdDoPortal);

void operacoesParaTickParaGerenciaDePacotes(void);

#endif
