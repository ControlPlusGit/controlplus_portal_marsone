
#include "perifericos.h"
#include "eth.h"
//#include "main.c"


int ContadorDeTempoDasCancelas;
int ContadorDeTempoParaManterCancelaDestravada;

void iniciaCancelas(void){
    ContadorDeTempoDasCancelas = 0;
    ContadorDeTempoParaManterCancelaDestravada = 0;
}

void operacoesEmTickParaCancelas(void){
    if(statusDeConexaoTCP == NOT_CONNECTED){
        travaCancelaDoPortalSemTemporizacao();
        return;
    }
    if (ContadorDeTempoDasCancelas > 0){
        ContadorDeTempoDasCancelas = ContadorDeTempoDasCancelas - 1;
        if (ContadorDeTempoDasCancelas == 0){
            destravaCancelaDoPortal();
        }
    }
    if(ContadorDeTempoParaManterCancelaDestravada > 0){
        ContadorDeTempoParaManterCancelaDestravada = ContadorDeTempoParaManterCancelaDestravada - 1;
    }
}

void liga_rele_externo1(void){
    ld_saidas (4,1);
}

void liga_rele_externo2(void){
    ld_saidas (7,1);
}

void desliga_rele_externo1(void){
    ld_saidas (4,0);
}

void desliga_rele_externo2(void){
    ld_saidas (7,0);
}

void travaCancelaDoPortal(void){
    desliga_rele_externo1();
    desliga_rele_externo2();
    ContadorDeTempoDasCancelas = 10000; //trava a cancela durante 10 segundos
}

void destravaCancelaDoPortal(void){
    liga_rele_externo1();
    liga_rele_externo2();
    ContadorDeTempoDasCancelas = 0;
    ContadorDeTempoParaManterCancelaDestravada = 3000;
}

void travaCancelaDoPortalSemTemporizacao(void){
    desliga_rele_externo1();
    desliga_rele_externo2();
    ContadorDeTempoDasCancelas = 500; //trava a cancela durante 0,5 segundo
}
