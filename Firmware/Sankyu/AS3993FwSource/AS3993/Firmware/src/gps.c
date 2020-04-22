//#include "regex.h"
#include <stdio.h>
#include <stdlib.h>
#include "perifericos.h"
#include "as3993_public.h"
int EstadoParaPegar;
char SubStringVelocidade[6];
float VelocidadeFinal;

#define ESTADO_LED_DESLIGADO_GPS            0
#define ESTADO_LED_PISCANDO__ACESSO_GPS     1
#define ESTADO_LED_PISCANDO__APAGADO_GPS    2
#define ESTADO_LED_LIGADO_GPS               3
int EstadoLedGps;
int ContadorPiscaGps;

#define TEMPO_PARA_PISCA_LED_GPS 500

void iniciaVelocidadeGps(void){
    VelocidadeFinal = 0;
    as3993SetSensitivity(125);
    EstadoLedGps = ESTADO_LED_PISCANDO__ACESSO_GPS;
    EstadoLedGps = ESTADO_LED_DESLIGADO_GPS;
}

void recebeDadosDoGps(unsigned char Dado){
    
}

int pegarVelocidadeDoGPS (char dado){
    switch(EstadoParaPegar){
        case 0:
            if (dado == 'N'){
                EstadoParaPegar = 1;
            }
            break;
        case 1:
            if (dado == ','){
                EstadoParaPegar = 2;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 2:
            if ((dado >= '0') && (dado <= '9')){
                EstadoParaPegar = 3;
                SubStringVelocidade[0] = dado;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 3:
            if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
                EstadoParaPegar = 4;
                SubStringVelocidade[1] = dado;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 4:
            if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
                EstadoParaPegar = 5;
                SubStringVelocidade[2] = dado;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 5:
            if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
                EstadoParaPegar = 6;
                SubStringVelocidade[3] = dado;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 6:
            if ((dado >= '0') && (dado <= '9')){
                EstadoParaPegar = 7;
                SubStringVelocidade[4] = dado;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 7:
            if (dado == ','){
                EstadoParaPegar = 8;
            } else {
                EstadoParaPegar = 0;
            }
            break;
        case 8:
            if (dado == 'K'){
                EstadoParaPegar = 8;
                SubStringVelocidade[5] = 0;
                return 1;
            } else {
                EstadoParaPegar = 0;
            }
            break;
            
        default:
            EstadoParaPegar = 0;

    }
                
            
    /*
	if (EstadoParaPegar == 0){
		if (dado == 'N'){
			EstadoParaPegar = 1;
			return 0;
		}
	}
	if (EstadoParaPegar == 1){
		if (dado == ','){
			EstadoParaPegar = 2;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 2){
		if ((dado >= '0') && (dado <= '9')){
			EstadoParaPegar = 3;
			SubStringVelocidade[0] = dado;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 3){
		if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
			EstadoParaPegar = 4;
			SubStringVelocidade[1] = dado;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 4){
		if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
			EstadoParaPegar = 5;
			SubStringVelocidade[2] = dado;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 5){
		if (((dado >= '0') && (dado <= '9')) || (dado == '.')) {
			EstadoParaPegar = 6;
			SubStringVelocidade[3] = dado;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 6){
		if ((dado >= '0') && (dado <= '9')){
			EstadoParaPegar = 7;
			SubStringVelocidade[4] = dado;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 7){
		if (dado == ','){
			EstadoParaPegar = 8;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
	if (EstadoParaPegar == 8){
		if (dado == 'K'){
			EstadoParaPegar = 8;
			SubStringVelocidade[5] = 0;
			return 0;
		} else {
			EstadoParaPegar = 0;
		}
		return 0;
	}
    return 0;
    */
    return 0;
}

/*
void AtualizarPontenciaDeRFIDEmFuncaoDaVelocidadeDaMaquina(void){
    signed char Sensibilidade;
    desliga_buzzer();
    if (VelocidadeFinal < 7){
        Sensibilidade = as3993GetSensitivity();
        if (Sensibilidade != 80){
            as3993SetSensitivity(80);
            liga_buzzer();
        }
    } else {
        Sensibilidade = as3993GetSensitivity();
        if (Sensibilidade != 125){
            as3993SetSensitivity(125);
            liga_buzzer();
        }
    }
}
*/

/*
void rotinaParaLidarComInterrupcaoDaSerialParaGps (void){
    int x;        //LUCIANO: Variavel declarada e nao utilizada. Eliminei um warning.
    
    while(DataRdyUART1()){
        x = ReadUART1();
        //(void)maquinaDeEstadosDeRecepcaoCUP(x);
        if (pegarVelocidadeDoGPS((char)x) != 0){
            VelocidadeFinal = strtod(SubStringVelocidade, NULL);
        }
        U1RX_Clear_Intr_Status_Bit;
        U1STAbits.OERR = 0;
    }
}
*/

/*
int main (int argv, char *argc[]){
	char st[10] = "1.050";
	const char ex[50] = "$GPVTG,054.7,T,034.4,M,005.5,N,0.102,K*48";
	int contador;
	teste();
	//enviaMovimentoParaWiegand("123", 1, 7);
	//printf("F = %4.8f\n", strtod(st, NULL));

	for(contador = 0;contador < 50;contador = contador + 1){
		(void)pegarVelocidadeDoGPS(ex[contador]);
		printf("%d", EstadoParaPegar);
	}

	printf("F = %4.8f\n", strtod(SubStringVelocidade, NULL));


	return 0;
}
*/

void atualizaLedGps(void){
    if (EstadoLedGps == ESTADO_LED_DESLIGADO_GPS){
        desliga_led_gps();
    }

    if (EstadoLedGps == ESTADO_LED_LIGADO_GPS){
        liga_led_gps();
    }

}

void operacacoesEmTickParaGps (void){
    if (EstadoLedGps == ESTADO_LED_PISCANDO__ACESSO_GPS || EstadoLedGps == ESTADO_LED_PISCANDO__APAGADO_GPS){
        if (ContadorPiscaGps > 0){
            ContadorPiscaGps = ContadorPiscaGps - 1;
        } else {
            ContadorPiscaGps = TEMPO_PARA_PISCA_LED_GPS;
            if (EstadoLedGps == ESTADO_LED_PISCANDO__APAGADO_GPS){
                EstadoLedGps = ESTADO_LED_PISCANDO__ACESSO_GPS;
                desliga_led_gps();
            } else {
                EstadoLedGps = ESTADO_LED_PISCANDO__APAGADO_GPS;
                liga_led_gps();
            }
        }
    }
}