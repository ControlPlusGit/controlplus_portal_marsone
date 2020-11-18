#include "uart_driver.h"
#include "GenericTypeDefs.h"
#include "platform.h"
#include "timer.h"
#include "gps.h"
#include "gprs.h"
#include "i2c.h"    //LUCIANO: Inclui i2c.h para eliminar warnings.
#include "rtc.h"
#include "perifericos.h"

// CONSTANTES


enum ESTADOS_MAQUINA_GPS {
    ESTADO_ESPERA_R,
    ESTADO_ESPERA_M,
    ESTADO_ESPERA_C,
    ESTADO_ESPERA_VIRGULA_1,
    ESTADO_ACQUISITANDO_HORA,
    ESTADO_ESPERA_VIRGULA_2,
    ESTADO_ACQUISITANDO_VALIDADE,
    ESTADO_ESPERA_VIRGULA_3,
    ESTADO_ACQUISITANDO_LATITUDE,
    ESTADO_ESPERA_VIRGULA_4,
    ESTADO_ACQUISITANDO_POLO_LATITUDE,
    ESTADO_ESPERA_VIRGULA_5,
    ESTADO_ACQUISITANDO_LONGITUDE,
    ESTADO_ESPERA_VIRGULA_6,
    ESTADO_ACQUISITANDO_POLO_LONGITUDE,
    ESTADO_ESPERA_VIRGULA_7,
    ESTADO_ACQUISITANDO_VELOCIDADE,
    ESTADO_ESPERA_VIRGULA_8,
    ESTADO_ACQUISITANDO_CURSO,
    ESTADO_ESPERA_VIRGULA_9,
    ESTADO_ACQUISITANDO_DATA,
    ESTADO_ESPERA_VIRGULA_10,
    ESTADO_ACQUISITANDO_VARIACAO_MAGNETICA,
    ESTADO_ESPERA_VIRGULA_11,
    ESTADO_ACQUISITANDO_CHECKSUM,
    ESTADO_OCIOSO_MAQUINA_GPS
} MAQUINA_GPS;



// VARIAVEIS
int estadoMaquinaGps;

char stringValidadeGPS[TAMANHO_MAXIMO_STRING_VALIDADE_GPS];
int contadorValidadeGPS;
char stringHoraGPS[TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS];
int contadorStringHoraGPS;
char stringDataGPS[TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS];
int contadorStringDataGPS;
char stringLatitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];
int contadorstringLatitudeGPS;
char stringLongitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];
int contadorstringLongitudeGPS;

char StringResultadoValidadeGPS[TAMANHO_MAXIMO_STRING_VALIDADE_GPS];
char stringResultadoLatitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];
char stringResultadoLongitudeGPS[TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS];

int segundos;
int minutos;
int horas;
int dia;
int mes;
int ano;

int flagSetarHora;

//PROTOTIPOS
void ResetaBufferGPS (void);
void ConverteStringGPSParaNumero(void);
int AjusteDeFusoHorario (void);

//TABELAS

const unsigned char DIAS_DOS_MESES[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

//ROTINAS

void recebeDadosDoGps(unsigned char Dado){
    if (VelocidadeFinal < LIMITE_DE_VELOCIDADE_GPS){
        liga_led_gps();
    } else {
        desliga_led_gps();
    }
    
    pegarVelocidadeDoGPS(Dado);
    if (VelocidadeFinal < LIMITE_DE_VELOCIDADE_GPS){
        desliga_led_gps();
    } else {
        liga_led_gps();
    }
    
    
}

// *****************************************************************8
// interface GPS >> RTC
// *****************************************************************8
int ObtemSegundosDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

int ObtemMinutosDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

int ObtemHorasDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

int ObtemDiaDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

int ObtemMesDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

int ObtemAnoDoGPS (void){
    return 0;   //LUCIANO: Adicionei um return para eliminar um warning.
}

// *****************************************************************8




int MaquinaDeEstadosGPS (int estado, char dado){
    switch (estado){
        case ESTADO_ESPERA_R:
            _TRISC13 = 0;
            _LATC13 = 0;
            
            if (dado == 'R')
                return ESTADO_ESPERA_M;
            break;
        case ESTADO_ESPERA_M:
            _TRISC13 = 0;
            _LATC13 = 1;
            desliga_led_gps();
            if (dado == 'M')
                return ESTADO_ESPERA_C;

                break;
        case ESTADO_ESPERA_C:
            liga_led_gps();
            if (dado == 'C')
                return ESTADO_ESPERA_VIRGULA_1;

                break;
        case ESTADO_ESPERA_VIRGULA_1:
            contadorStringHoraGPS = 0;
            if (dado == ',')
                return ESTADO_ACQUISITANDO_HORA;

                break;

        case ESTADO_ACQUISITANDO_HORA:
            contadorstringLatitudeGPS = 0;
            contadorValidadeGPS = 0;
            if ( dado == ',' ){
                return ESTADO_ACQUISITANDO_VALIDADE;
            }

            if ( dado == '.' ){
                return ESTADO_ESPERA_VIRGULA_2;
            }


            if (contadorStringHoraGPS < TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS){
                stringHoraGPS[contadorStringHoraGPS] = dado;
                contadorStringHoraGPS = contadorStringHoraGPS + 1;
            }

            break;
            
        case ESTADO_ESPERA_VIRGULA_2:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_VALIDADE;
                break;

        case ESTADO_ACQUISITANDO_VALIDADE:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_LATITUDE;
            if (contadorValidadeGPS < TAMANHO_MAXIMO_STRING_VALIDADE_GPS){
                stringValidadeGPS[contadorValidadeGPS] = dado;
                contadorValidadeGPS = contadorValidadeGPS + 1;
                
            }

            break;

        case ESTADO_ESPERA_VIRGULA_3:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_LATITUDE;

                break;



        case ESTADO_ACQUISITANDO_LATITUDE:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_POLO_LATITUDE;

            if (contadorstringLatitudeGPS < TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS){
                stringLatitudeGPS[contadorstringLatitudeGPS] = dado;
                contadorstringLatitudeGPS = contadorstringLatitudeGPS + 1;
            }

                break;

        case ESTADO_ESPERA_VIRGULA_4:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_POLO_LATITUDE;

                break;

        case ESTADO_ACQUISITANDO_POLO_LATITUDE:
            contadorstringLongitudeGPS = 0;
            if (dado == ',')
                return ESTADO_ACQUISITANDO_LONGITUDE;
                    break;
            
        case ESTADO_ESPERA_VIRGULA_5:
                break;


        case ESTADO_ACQUISITANDO_LONGITUDE:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_POLO_LONGITUDE;

            if (contadorstringLongitudeGPS < TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS){
                stringLongitudeGPS[contadorstringLongitudeGPS] = dado;
                contadorstringLongitudeGPS = contadorstringLongitudeGPS + 1;
            }

                break;

        case ESTADO_ESPERA_VIRGULA_6:
                break;


        case ESTADO_ACQUISITANDO_POLO_LONGITUDE:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_VELOCIDADE;
                break;
        case ESTADO_ESPERA_VIRGULA_7:
                break;
                
        case ESTADO_ACQUISITANDO_VELOCIDADE:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_CURSO;
            
            return ESTADO_ESPERA_VIRGULA_8;
                break;
        case ESTADO_ESPERA_VIRGULA_8:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_CURSO;
                break;
        case ESTADO_ACQUISITANDO_CURSO:
            if (dado == ',')
                return ESTADO_ACQUISITANDO_DATA;
            return ESTADO_ESPERA_VIRGULA_9;
                break;
        case ESTADO_ESPERA_VIRGULA_9:
            contadorStringDataGPS = 0;
            if (dado == ',')
                return ESTADO_ACQUISITANDO_DATA;
                break;
        case ESTADO_ACQUISITANDO_DATA:
            if (dado == ','){
                return ESTADO_ACQUISITANDO_VARIACAO_MAGNETICA;
                //ConverteStringGPSParaNumero();
                //ResetaBufferGPS();
                //return ESTADO_ESPERA_R;
            }

            if (contadorStringDataGPS < TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS){
                stringDataGPS[contadorStringDataGPS] = dado;
                contadorStringDataGPS = contadorStringDataGPS + 1;
            }
            


        case ESTADO_ESPERA_VIRGULA_10:
                break;
        case ESTADO_ACQUISITANDO_VARIACAO_MAGNETICA:
                ConverteStringGPSParaNumero();
                StringResultadoValidadeGPS[0] = stringValidadeGPS[0];
                if (stringValidadeGPS[0] == 'A'){
                    AjusteDeFusoHorario();

                    if (CompararHoraDadaComADoRTC(minutos, horas, dia, mes, ano)){
                        flagSetarHora = 1;
                        //SetarHoraRTC(minutos, horas, dia, mes, ano);
                    }

                    sprintf(stringResultadoLongitudeGPS, "-%s", stringLongitudeGPS);
                    sprintf(stringResultadoLatitudeGPS, "-%s", stringLatitudeGPS);
                            
                } else {
                    // Adicionado a pedido do pessoal da Volit
                    desliga_led_gps();
                    sprintf(stringResultadoLongitudeGPS, "0.000");
                    sprintf(stringResultadoLatitudeGPS, "0.000");

                    
                }
                ResetaBufferGPS();
                return ESTADO_ESPERA_R;

                break;
        case ESTADO_ESPERA_VIRGULA_11:
                break;
        case ESTADO_ACQUISITANDO_CHECKSUM:
            break;
        case ESTADO_OCIOSO_MAQUINA_GPS:
            break;

        default:
            break;


    }
    return estado;
}


void ResetaBufferGPS (void){
    memset(stringValidadeGPS, 0, TAMANHO_MAXIMO_STRING_VALIDADE_GPS);
    memset(stringHoraGPS, 0, TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS);
    memset(stringDataGPS, 0, TAMANHO_MAXIMO_STRING_DATA_E_HORA_GPS);
    memset(stringLatitudeGPS, 0, TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS);
    memset(stringLongitudeGPS, 0, TAMANHO_MAXIMO_STRING_LATITUDE_E_LONGITUDE_GPS);
    contadorStringDataGPS = 0;
    contadorstringLongitudeGPS = 0;
}

void ConverteStringGPSParaNumero(void){
    int rascunho;

    horas = stringHoraGPS[0];
    horas = horas - '0';
    horas = horas * 10;

    rascunho = stringHoraGPS[1];
    rascunho = rascunho - '0';
    horas = horas + rascunho;

    minutos = stringHoraGPS[2];
    minutos = minutos - '0';
    minutos = minutos * 10;

    rascunho = stringHoraGPS[3];
    rascunho = rascunho - '0';
    minutos = minutos + rascunho;

    segundos = stringHoraGPS[4];
    segundos = segundos - '0';
    segundos = segundos * 10;

    rascunho = stringHoraGPS[5];
    rascunho = rascunho - '0';
    segundos = segundos + rascunho;

    dia = stringDataGPS[0];
    dia = dia - '0';
    dia = dia * 10;

    rascunho = stringDataGPS[1];
    rascunho = rascunho - '0';
    dia = dia + rascunho;

    mes = stringDataGPS[2];
    mes = mes - '0';
    mes = mes * 10;

    rascunho = stringDataGPS[3];
    rascunho = rascunho - '0';
    mes = mes + rascunho;

    ano = stringDataGPS[4];
    ano = ano - '0';
    ano = ano * 10;

    rascunho = stringDataGPS[5];
    rascunho = rascunho - '0';
    ano = ano + rascunho;
    //ano = ano + 2000; //apenas para simpliicar a conferencia de bicestualidade.

    
}

int AjusteDeFusoHorario (void){
    horas = horas - 3;
    if (horas >= 0)
        return 0;

    horas = horas + 3;
    if (horas == 0){
        horas = 21;
    }
    if (horas == 1){
        horas = 22;
    }
    if (horas == 2){
        horas = 23;
    }



    dia = dia - 1;
    if (dia > 0)
        return 0;
    mes = mes - 1;
    if (mes < 1){
        ano = ano - 1;
    }
    dia = DIAS_DOS_MESES[mes - 1];
    if ( ( ( (ano % 4) == 0) && ( (ano % 100) != 0) ) || ( (ano % 400) == 0) ){
        if (mes == 2)
            dia = 29;
    }
    return 0;
}

void iniciaGPS (void){
    estadoMaquinaGps = ESTADO_ESPERA_R;
    sprintf(stringResultadoLongitudeGPS, "0.000");
    sprintf(stringResultadoLatitudeGPS, "0.000");
    
}

void ProcessaNaInterrupcaoDeUart(char dado){
    estadoMaquinaGps = MaquinaDeEstadosGPS(estadoMaquinaGps, dado);
}

void ProcessaGPS(void){
    if (flagSetarHora){
        flagSetarHora = 0;
        SetarHoraRTC(0, minutos, horas, dia, mes, ano);
    }

    /*
    char string[200];
    int contador;
    //sprintf(string, "$GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70");


    memset(string, 0, 200);

    contador = uartRx2NumBytesAvailable();
    uartRx2NBytes(string, contador);


    contador = 0;
    


    ResetaBufferGPS();
    
    while (string[contador] != 0x00){
        estadoMaquinaGps = MaquinaDeEstadosGPS(estadoMaquinaGps, string[contador]);
        contador = contador + 1;
    }


    ResetaBufferGPS();
     *
     * */

    /*

    sprintf(string, "GPRMC,,,,,,,,,,,,,,,,,,,,,,,,,,,,");

    contador = 0;
    while (string[contador] != 0x00){
        estadoMaquinaGps = MaquinaDeEstadosGPS(estadoMaquinaGps, string[contador]);
        contador = contador + 1;
    }

    sprintf(string, "GPRMC,A,A,A,A,A,A,A,A,A,AA,A,A,,,,,,,,,,,,,,,");

    ResetaBufferGPS();

    contador = 0;
    while (string[contador] != 0x00){
        estadoMaquinaGps = MaquinaDeEstadosGPS(estadoMaquinaGps, string[contador]);
        contador = contador + 1;
    }

    ResetaBufferGPS();
     * */

}

int EstadoParaPegar;
char SubStringVelocidade[6];
//float VelocidadeFinal;
int VelocidadeFinal;

#define ESTADO_LED_DESLIGADO_GPS            0
#define ESTADO_LED_PISCANDO__ACESSO_GPS     1
#define ESTADO_LED_PISCANDO__APAGADO_GPS    2
#define ESTADO_LED_LIGADO_GPS               3
int EstadoLedGps;
int ContadorPiscaGps;

#define TEMPO_PARA_PISCA_LED_GPS 500

void iniciaVelocidadeGps(void){
    VelocidadeFinal = 0;
    //as3993SetSensitivity(125);
    EstadoLedGps = ESTADO_LED_PISCANDO__ACESSO_GPS;
    EstadoLedGps = ESTADO_LED_DESLIGADO_GPS;
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
                //VelocidadeFinal = strtod(SubStringVelocidade, NULL);
                VelocidadeFinal = atoi(SubStringVelocidade);
                
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
    if (VelocidadeFinal < LIMITE_DE_VELOCIDADE_GPS){
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