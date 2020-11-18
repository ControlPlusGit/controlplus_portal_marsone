#include "as3993_config.h"
#include "platform.h"
/*
#include "C:\projetos\Ambve\common\firmware\microchip\include\stream_dispatcher.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\usb_hid_stream_driver.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\logger.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\uart_driver.h"
#include "C:\projetos\Ambve\common\firmware\microchip\include\errno.h"
 * */
#include "as3993_public.h"
#include "as3993.h"
#include "gen2.h"
#include "global.h"
#include "timer.h"
#include "appl_commands.h"
#include "tuner.h"
//#include "main.h"
#include "i2c.h"

#include "varal.h"



// *****************************************************************************
//  Constantes e definicoes
// *****************************************************************************

/*
//Essas definicoes diferem do midleware, vou deixa-las por referencia
#define POSICAO_TAGS_LEFT 1
#define POSICAO_TAGS_IN_REPLY 2
#define POSICAO_RSSI 4
#define POSICAO_FREQUENCIA_HIGH 5
#define POSICAO_FREQUENCIA_MID 6
#define POSICAO_FREQUENCIA_LOW 7
#define POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC 8
*/

#define CRC_INICIAL 0xAA55;
//const u32 MEU_ID = 0x10000000;
const u32 MEU_ID = 0x0000030;



#define POSICAO_TAGS_LEFT                       1
#define POSICAO_TAGS_IN_REPLY                   2
#define POSICAO_RSSI                            6
#define POSICAO_FREQUENCIA_HIGH                 9
#define POSICAO_FREQUENCIA_MID                  8
#define POSICAO_FREQUENCIA_LOW                  7
#define POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC     10
#define POSICAO_INICIAL_TAG                     11



#ifdef VARAL
    #define TAMANHO_BUFFER_DE_TAGS 180 //25 //MAXTAG
    #define TAMANHO_DO_BUFFER_DE_TX 200
#else
    #define TAMANHO_BUFFER_DE_TAGS 2 //MAXTAG
    #define TAMANHO_DO_BUFFER_DE_TX 20
#endif

#define TAMANHO_TAG 10 // 40

#define VALOR_DE_TEMPO_PARA_TIMEOUT_DE_RECEPCAO_CUP 1000

#define TEMPO_PARA_MUDAR_A_DIRECAO_DA_485 5  //10

#define TAMANHO_EPC 24 // era 23 para a AMBEV


enum ESTADOS_CUP {
    CABECA_1,
    CABECA_2,
    CABECA_3,
    CABECA_4,
    CABECA_5,
    CABECA_6,
    CABECA_7,
    CABECA_8,
    CABECA_9,
    CABECA_10,
    CABECA_11,
    CABECA_12,
    CABECA_13,
    CABECA_14,
    CABECA_15,
    CABECA_16,
    CABECA_17,
    IDLE,

    SETAR_SAIDA_1, //porta
    SETAR_SAIDA_2, //Nivel

    PEGAR_PINO_PULSO_BAIXO,
    PEGAR_MSB_PULSO_BAIXO,
    PEGAR_LSB_PULSO_BAIXO,
    PEGAR_PINO_PULSO_ALTO,
    PEGAR_MSB_PULSO_ALTO,
    PEGAR_LSB_PULSO_ALTO,


    ENVIAR_TAGS_1,
    ENVIAR_TAGS_2,
    ENVIAR_TAGS_3,
    ENVIAR_TAGS_4,


    FINAL
};

#define NUMERO_DE_PINOS_DE_IO 8

unsigned int tempoParaPulsoAlto[NUMERO_DE_PINOS_DE_IO];
unsigned int tempoParaPulsoBaixo[NUMERO_DE_PINOS_DE_IO];
unsigned char pinoASerAcionado;

extern Tag tags_[MAXTAG];

// *****************************************************************************

volatile int EstadoDeRecepcaoDaMaquinaCUP;
int EstadoTransmissaoDaMaquinaCUP;

int transmitindo;

//int contadorDeTimeOutDeRecepcao;


u8 __attribute__((far)) BufferTX[TAMANHO_DO_BUFFER_DE_TX];
volatile int ContadorDeTX;
volatile int LimiteDoContadorDeTX;

//__eds__ unsigned char __attribute__(space(__eds__)) BufferDeTags[TAMANHO_BUFFER_DE_TAGS][TAMANHO_TAG];
unsigned char __attribute__((far)) BufferDeTags[TAMANHO_BUFFER_DE_TAGS][TAMANHO_TAG];
//unsigned char __attribute__(space(__eds__)) BufferDeTags[TAMANHO_BUFFER_DE_TAGS][TAMANHO_TAG];

u16 QuantidadeDeDadosRecebidos;
volatile u16 chksum_Recebido;
volatile u16 chksum_gerado;

int crcRecebido;
int crcASerEnviado;

u8 FlagParaEnviarTags; //Quando em um, eu envio e elimino todas as tags registradas na RAM.
u8 EnviandoDados;

volatile int TagSendoTratada;
volatile int tagsAEnviar;

volatile int FlagParaEnviarCRC;

volatile int ContadorDeTagsPresentesNoBuffer;


int contadorParaMudancaDeDirecaoDa485;

unsigned long contadorParaResponderADescoberta;

int comandoRecebido;
#define TAMANHO_MAXIMO_DE_PARAMETRO_RECEBIDO_CUP 100 //Deve ser menor que o buffer de recepcao
char __attribute__((far)) parametroRecebido[TAMANHO_MAXIMO_DE_PARAMETRO_RECEBIDO_CUP];

char flagParaEnvioDeTagsSemBloquearOCodigo;
char flagParaEnvioDoCRC;
int contadorDeTagProcesada;

int PortaSelecionada;

#define TEMPO_PARA_TIMEOUT_RECEPCAO 400
volatile int contadorTimeOutRecepcaoCUP;

char flagDeDadosRecebidos;
int contadorParaEnviarOMeuID;

//volatile u16 crc;

void ResetaBufferTX (void);
void CabecaCUP (u32 destino, int tamanho);
int adicionarTagAoBuffer (int posicao, unsigned char * dados, int tamanho);
int tagJaPresenteNoBuffer (unsigned char * tag, int posicao, int tamanho);
int posicaoLivreNoBufferDeTags (void);
void EnviaBufferDeTagsAoPC (void);
void EnviaCRC (void);
void SetaA485NoSentidoPIC_PC (void);
int QuantiaDeDadosAEnviaDoBufferDeTags (void);

//#define TESTE_CARGA 1

// *****************************************************************************
// Rotinas
// *****************************************************************************

void IniciaVaral (void){
    flagParaEnvioDeTagsSemBloquearOCodigo = 0;
    contadorDeTagProcesada = 0;
    contadorTimeOutRecepcaoCUP = TEMPO_PARA_TIMEOUT_RECEPCAO;
    _LATA2 = 1;
    EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
    _LATA2 = 0;
}

void ProcessaEnvioDeTagsNoVaral (void){
    if (flagParaEnvioDeTagsSemBloquearOCodigo){
        //_LATA2 = 1;
#ifdef TESTE_CARGA
        EnviaBufferDeTagsAoPC_PARA_TESTE();
#else
        EnviaBufferDeTagsAoPC();
#endif
    }

    if ((flagParaEnvioDoCRC == 1) && (ContadorDeTX == 0) ){
        EnviaCRC();
        transmitindo = 0;
        flagParaEnvioDoCRC = 0;
        _LATA2 = 1;
        EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
        _LATA2 = 0;
    }

    //if ((contadorDeTagProcesada == TAMANHO_BUFFER_DE_TAGS) && (ContadorDeTX == 0) && (flagParaEnvioDoCRC == 0)){
    if ((contadorDeTagProcesada > (TAMANHO_BUFFER_DE_TAGS - 1) ) && (ContadorDeTX == 0) && (flagParaEnvioDoCRC == 0)){
        
        flagParaEnvioDoCRC = 1;
        IniciaVaral();
    }

    
}

int QuantiaDeTagsPresentesNoBufferDeVaral (void){
    int contador, soma;
    
    soma = 0;
    for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        if (posicaoOcupadaNoBufferDeTags(contador) != -1){
            soma = soma + 1;
        }
    }

    return soma;
}

void ResetContadorDeEnvidoDoMeuID (void){
    unsigned int x; // limito o ID a 128 possibilidades
    x = MEU_ID;
    x = x & 0x007F;
    contadorParaEnviarOMeuID = x;
    //contadorParaEnviarOMeuID = MEU_ID;
}

//Envia um byte e se acabar, desliga a interrupcao
char RotinasDeInterrupcaoDeEnvio (void){

    unsigned char dado = BufferTX[ContadorDeTX];
    ContadorDeTX = ContadorDeTX + 1;
    //if ((ContadorDeTX == -1) || (ContadorDeTX == 0xFFFF))
    SetaA485NoSentidoPIC_PC();
    
    if ( (ContadorDeTX >= LimiteDoContadorDeTX) || (ContadorDeTX > TAMANHO_DO_BUFFER_DE_TX)  ){
        ResetaBufferTX();
        //EnviaCRC();
        //Pode ser que tenha que esperar o fim do envio do CRC
        //FlagParaEnviarCRC = 1;
        //_LATG13 = 0;//DIRECAO
        ContadorDeTX = 0;
        IEC0bits.U1TXIE = 0;
    }
    return dado;
    
}

void ResetaBufferTX (void){
    IEC0bits.U1TXIE = 0;
    LimiteDoContadorDeTX = 0;

    //Cuidado
    //FlagParaEnviarTags = 0;
}


//Envio por polling, soh usar para testes.
void EnviaBytePara485 (char dado){
    //while (_LATG13 != 0);
    _LATG13 = 1;
    
    U1TXREG = dado;
    while( (!U1STAbits.TRMT ) );
    _LATG13 = 0;
}


void EnviaStringPara485 (char * dado){
    while (*dado != 0){
        EnviaBytePara485(*dado);
        dado = dado + 1;
    }
}


void OperacoesParaTickVaral (void)
{
    int contador;
    //if (contadorDeTimeOutDeRecepcao > 0)
        //contadorDeTimeOutDeRecepcao = contadorDeTimeOutDeRecepcao - 1;

    contador = 0;

    while (contador < NUMERO_DE_PINOS_DE_IO){
        if (tempoParaPulsoAlto[contador]){
            tempoParaPulsoAlto[contador] = tempoParaPulsoAlto[contador] - 1;
        }

        if (tempoParaPulsoAlto[contador] == 1) {
            ld_saidas(contador, 0);
        }

        if (tempoParaPulsoBaixo[contador]){
            tempoParaPulsoBaixo[contador] = tempoParaPulsoBaixo[contador] - 1;
        }
        
        if (tempoParaPulsoBaixo[contador] == 1) {
            ld_saidas(contador, 1);
        }

        contador = contador + 1;

    }

    

    if (contadorParaMudancaDeDirecaoDa485)
    {
        contadorParaMudancaDeDirecaoDa485 = contadorParaMudancaDeDirecaoDa485 - 1;
        if (contadorParaMudancaDeDirecaoDa485 == 0)
        {
            _LATG13 = 0;
        }
    }

    if (contadorTimeOutRecepcaoCUP)
    {
        contadorTimeOutRecepcaoCUP = contadorTimeOutRecepcaoCUP - 1;
    }
    else
    {
        _LATA2 = 1;
        EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
        contadorTimeOutRecepcaoCUP = TEMPO_PARA_TIMEOUT_RECEPCAO;
        _LATA2 = 0;
    }


    if (contadorParaEnviarOMeuID){
        contadorParaEnviarOMeuID = contadorParaEnviarOMeuID - 1;
    }




    
    //contadorDeTimeOutDeRecepcao = VALOR_DE_TEMPO_PARA_TIMEOUT_DE_RECEPCAO_CUP;
}


void SetaA485NoSentidoPIC_PC (void){
    _LATG13 = 1; //DIRECAO
    contadorParaMudancaDeDirecaoDa485 = TEMPO_PARA_MUDAR_A_DIRECAO_DA_485;

}

void LigaTX (void){
    //U1TXInterrupt
    //_LATC8 = 1; DIRECAO
    SetaA485NoSentidoPIC_PC();
    IEC0bits.U1TXIE = 1;
};

void resetaContador (void){
    //T3CONbits.TON = 0;
    /*
    slowTimerMsValue = 1;   //start with 1ms to get immediate stop at 0ms delays
    TMR3 = 0;
    _T3IF = 0;
    _T3IE = 1;
    T3CONbits.TON = 1;

    TimerMsValue = 0;
     * */
};

u16 valorAtualDOContador (){
   //return TimerMsValue;
    return 0;
}

void pararContador (){
    //_T3IE = 0;
    T3CONbits.TON = 0;

}



void EnviaPacoteParaSerial (const char * data, int tamanho){
    int contador;
    for (contador = 0;contador < tamanho;contador = contador + 1){
        BufferTX[contador] = *data;
        //uartTx(*data);
        crcASerEnviado = crcASerEnviado ^ *data;
        data = data + 1;
    }
    ContadorDeTX = 0;
    LimiteDoContadorDeTX = tamanho;

    /*
    if (LimiteDoContadorDeTX < TAMANHO_DO_BUFFER_DE_TX){
        BufferTX[contador] = 0;//Dummy
        BufferTX[contador + 1] = 0;//Dummy
        LimiteDoContadorDeTX = LimiteDoContadorDeTX + 2;
    }
     * */

    LigaTX();
}


void ResetaEnvio (void){
    FlagParaEnviarTags = 0;
}

int ResetaCheckSum (void){
    return 0;
};

int GeraCheckSum (int chk, char  * dado, int tamanho){
    int contador;

    for (contador = 0;contador < tamanho;contador = contador + 1){
        chk = chk + *dado;
        dado = dado + 1;
    }

    return chk;
};


int ProcessaComandosTagsPresentes (void){
    //u16 toTx;
    //u8 protocol;
    //u8 buf;

    //int posicao;

    //EnviaBufferDeTagsAoPC();
    //LimpaBufferDeTags();
    FlagParaEnviarTags = 1;
    TagSendoTratada = 0;
    //while(IEC0bits.U1TXIE);
    //(void)applProcessCyclic(&protocol, &toTx, buf, (FlagParaEnviarTags));

    //(void)applProcessCyclic(&protocol, &LimiteDoContadorDeTX, BufferTX, TAMANHO_DO_BUFFER_DE_TX);


    /*
    if (LimiteDoContadorDeTX != 0){
        LigaTX();
    }
    FlagParaEnviarTags = 0;
    */


     //tagsAEnviar = ContadorDeTagsPresentesNoBuffer;
    

    return 0;
}

int ProcessaComandoDeQuestionamentoDeParamentros (void){

    return 0;
}

int ProcessaComandoDeSetagemDeParamentros (void){
    //Realizar passear e setar os parametros recebidos do middle e que vem do aplicativo.
    return 0;

}

int ProcesaComandoNumeroDeSerie (void){
    // MEU_ID
    return 0;
}



//contadorParaResponderADescoberta

int RespondeADescobertaNoVaral (unsigned long meuid, long destino){

    //reseta contador de bytes recebidos
    //espera algumas cetenas de micro segundos.
    //Se nao chegou nada, envio o ID e o CRC dele.



    int crc;
    int contador;

    crc = CRC_INICIAL;

    char pacote[64];

    pacote[0] = 0x43;//C

    pacote[1] = 0x55;//U
    pacote[2] = 0x50;//P
    pacote[3] = 0x01;//Versao

    pacote[4] = (u8)((destino) >> 24);//Remetente
    pacote[5] = (u8)((destino) >> 16);
    pacote[6] = (u8)((destino) >> 8);
    pacote[7] = (u8)((destino));//LSB

    pacote[8] = (u8)((MEU_ID) >> 24);//Remetente
    pacote[9] = (u8)((MEU_ID) >> 16);
    pacote[10] = (u8)((MEU_ID) >> 8);
    pacote[11] = (u8)((MEU_ID));//LSB

    pacote[12] = 0x00;//Quantidade de dados
    pacote[13] = 0x00;//LSB

    //tamanho = tamanho + 2;// do CRC
    //pacote[12] = (char)(tamanho >> 8);
    //pacote[13] = (char)(tamanho & 0xFF);

    tagsAEnviar = tagsAEnviar + 2;
    //pacote[12] = (char)(tagsAEnviar >> 8);
    //pacote[13] = (char)(tagsAEnviar & 0xFF);

    chksum_gerado = ResetaCheckSum();
    //chksum_gerado = GeraCheckSum(chksum_gerado, &pacote, 14);
    chksum_gerado = GeraCheckSum(chksum_gerado, pacote, 14);    //LUCIANO: 'pacote' eh uma string, nao precisa do & para passar o endereco. Eliminei um warning.

    pacote[15] = (char)chksum_gerado;
    pacote[14] = (char)(chksum_gerado >> 8);

    for (contador = 0; contador < 14;contador = contador + 1){
        crcASerEnviado = crcASerEnviado ^ pacote[contador];
    }

    //pacote[15] = (char)crc;
    //pacote[14] = (char)(crc >> 8);


    //crc = GeraCheckSum(pacote, 13);
    EnviaPacoteParaSerial(pacote, 16);

    crcASerEnviado = ResetaCheckSum();

    transmitindo = 0;
    
    return(0);  //LUCIANO: Adicionei este retorno. Eliminei um warning.
}


int processaCUP(void){
    switch (comandoRecebido){
        case 0x41:
           ProcessaComandosTagsPresentes();

           tagsAEnviar = tagsPresentesNoBuffer();
           //tagsAEnviar = tagsAEnviar *  TAMANHO_TAG;
           //tagsAEnviar = tagsAEnviar *  34;

           tagsAEnviar = QuantiaDeDadosAEnviaDoBufferDeTags();
           CabecaCUP(1, tagsAEnviar);
           crcRecebido = CRC_INICIAL;
           //EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;



           while (ContadorDeTX != 0);

           IniciaVaral();

           flagParaEnvioDeTagsSemBloquearOCodigo = 1;

           //EnviaBufferDeTagsAoPC();
           //EnviaBufferDeTagsAoPC_PARA_TESTE();

           //while (ContadorDeTX != 0);
           //EnviaCRC();

            // O comando soh retirado apos ser tratado.
            comandoRecebido = 0x00;

           break;

        case 0x43:
            EstadoDeRecepcaoDaMaquinaCUP = SETAR_SAIDA_1;
            // O comando soh retirado apos ser tratado.
            
            comandoRecebido = 0x00;

            break;


        case 0x44:
            if (tempoParaPulsoBaixo != 0){
                ld_saidas(pinoASerAcionado, 0);
                comandoRecebido = 0x00;
            }
            
            break;

        case 0x45:
            if (tempoParaPulsoAlto != 0){
                ld_saidas(pinoASerAcionado, 1);
                comandoRecebido = 0x00;
            }

            break;


        case 0xFF:
            if (contadorParaEnviarOMeuID == 0){
                if (flagDeDadosRecebidos){
                    RespondeADescobertaNoVaral(MEU_ID, 0x0001);
                    IniciaVaral();
                    // O comando soh retirado apos ser tratado.
                    comandoRecebido = 0x00;
                } else {
                    ResetContadorDeEnvidoDoMeuID();
                }
                
            }
            break;

        default:
            // Caso comando invalido
            // O comando soh retirado apos ser tratado.
            comandoRecebido = 0x00;

            break;
    }
    
    //Cada comando soh eh tratado uma vez por recepcao
    //comandoRecebido = 0x00;
    return 0;
}

void CabecaCUP (u32 destino, int tamanho){
    int crc;
    int contador;

    crc = CRC_INICIAL;

    char pacote[64];

    pacote[0] = 0x43;//C

    pacote[1] = 0x55;//U
    pacote[2] = 0x50;//P
    pacote[3] = 0x01;//Versao

    /*
    pacote[4] = 0x00;//Destinatario
    pacote[5] = 0x00;
    pacote[6] = 0x00;
    pacote[7] = 0x01;//LSB
    */
    pacote[4] = (u8)((destino) >> 24);//Destinatario
    pacote[5] = (u8)((destino) >> 16);
    pacote[6] = (u8)((destino) >> 8);
    pacote[7] = (u8)((destino));//LSB

    pacote[8] = (u8)((MEU_ID) >> 24);//Remetente
    pacote[9] = (u8)((MEU_ID) >> 16);
    pacote[10] = (u8)((MEU_ID) >> 8);
    pacote[11] = (u8)((MEU_ID));//LSB

    pacote[12] = 0x00;//Quantidade de dados
    pacote[13] = 0x03;//LSB

    tamanho = tamanho + 2;// do CRC
    //pacote[12] = (char)(tamanho >> 8);
    //pacote[13] = (char)(tamanho & 0xFF);

    tagsAEnviar = tagsAEnviar + 2;
    pacote[12] = (char)(tagsAEnviar >> 8);
    pacote[13] = (char)(tagsAEnviar & 0xFF);
    //pacote[12] = 0;
    //pacote[13] = 0;

    chksum_gerado = ResetaCheckSum();
    //chksum_gerado = GeraCheckSum(chksum_gerado, &pacote, 14);
    chksum_gerado = GeraCheckSum(chksum_gerado, pacote, 14);   //LUCIANO: 'pacote' eh uma string, nao precisa do & para passar o endereco. Eliminei um warning.

    pacote[15] = (char)chksum_gerado;
    pacote[14] = (char)(chksum_gerado >> 8);

    for (contador = 0; contador < 14;contador = contador + 1){
        crcASerEnviado = crcASerEnviado ^ pacote[contador];
    }

    //pacote[15] = (char)crc;
    //pacote[14] = (char)(crc >> 8);


    //crc = GeraCheckSum(pacote, 13);
    //EnviaPacoteParaSerial(pacote, 15);
    EnviaPacoteParaSerial(pacote, 16);

    crcASerEnviado = ResetaCheckSum();
}


//Nessa rotina passo as tags da estrutura da AMS para string a serem enviadas no varal.
void ProcessaTagsParaOVaral (int antena){
    //char __attribute__((far)) rascunho[200];
    char rascunho[200];     //LUCIANO: Exclui o __attribute__((far)). Eliminei um warning.
    int contador;
    int contador_de_tags;
    int tamanho;

    char resultado;

    memset(rascunho, 0, 200);


    /*
    sprintf(&rascunho[POSICAO_INICIAL_TAG], "00112233445566778899AA");


    rascunho[POSICAO_TAGS_LEFT] = 0;
    rascunho[POSICAO_TAGS_IN_REPLY] = 1;
    rascunho[POSICAO_RSSI] = 0x80;//RSSI
    rascunho[POSICAO_FREQUENCIA_LOW] = 0x38;
    rascunho[POSICAO_FREQUENCIA_MID] = 0xF6;
    rascunho[POSICAO_FREQUENCIA_HIGH] = 0x0D;

    rascunho[POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC] = 11;


    adicionarTagAoBuffer(0, rascunho, (22 + 10) );
    */

    //Comentando partindo daqui para testes
    for (contador_de_tags = 0; contador_de_tags < MAXTAG;contador_de_tags = contador_de_tags + 1){
        resultado = 0;
        for (contador = 0;contador < tags_[contador_de_tags].epclen;contador = contador + 1){
            resultado = resultado | tags_[contador_de_tags].epc[contador];
        }
        
        if (resultado != 0){ // Tem tag na posicao?
            rascunho[POSICAO_TAGS_LEFT] = 0;
            rascunho[POSICAO_TAGS_IN_REPLY] = 1;
            //rascunho[POSICAO_RSSI] = 0x80;//RSSI
            rascunho[POSICAO_RSSI] = antena;//
            rascunho[POSICAO_FREQUENCIA_LOW] = 0x38;
            rascunho[POSICAO_FREQUENCIA_MID] = 0xF6;
            rascunho[POSICAO_FREQUENCIA_HIGH] = 0x0D;

            //rascunho[POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC] = 11;
            rascunho[POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC] = 12;

            //tamanho = tags_[contador_de_tags].epclen + POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1;
            tamanho = tags_[contador_de_tags].epclen + POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1;
            for (contador = 0;contador < tamanho;contador = contador + 1){
                //rascunho[contador + POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1] = tags_[contador_de_tags].epc[contador];
                rascunho[contador + POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC + 1] = tags_[contador_de_tags].epc[contador];
                //sprintf(&rascunho[contador * 2], "%02X", tags_[contador_de_tags].epc[contador]);
            }


            resultado = 0;
            for (contador = 0;contador < (TAMANHO_BUFFER_DE_TAGS - 1 );contador = contador + 1) {
                //int tagJaPresenteNoBuffer (unsigned char * tag, int posicao, int tamanho){
                //if ( tagJaPresenteNoBuffer (rascunho, contador, (tags_[contador_de_tags].epclen * 2 ) ) == 0){
                //if ( tagJaPresenteNoBuffer ((unsigned char *)rascunho, contador, tamanho ) == 0){
                if ( tagJaPresenteNoBuffer ((unsigned char *)rascunho, contador, tamanho ) == 0){   //LUCIANO: Apliquei um cast na variavel rascunho. Eliminei um warning.
                    resultado = resultado | 1;
                }
                
            }

            if (resultado == 0){
                contador = posicaoLivreNoBufferDeTags();
                if (contador != -1){
                    //adicionarTagAoBuffer(contador, rascunho, tamanho );
                    adicionarTagAoBuffer(contador, (unsigned char *)rascunho, tamanho );    //LUCIANO: Apliquei um cast na variavel rascunho. Eliminei um warning.
                    //adicionarTagAoBuffer(contador, rascunho, (tags_[contador_de_tags].epclen * 2) );
                }
            }
        }
    }
    //Comentando interrompido daqui para testes

}


void acionaSaida(int porta, int nivel){
    switch (porta){
        case 1:
            if (nivel){
                liga_rele1();
            } else {
                desliga_rele1();
            }
            break;

        case 2:
            if (nivel){
                liga_rele2();
            } else {
                desliga_rele2();
            }

            break;

        case 3:
            if (nivel){
                liga_rele3();
            } else {
                desliga_rele3();
            }
            break;

        case 4:
            if (nivel){
                liga_rele4();
            } else {
                desliga_rele4();
            }
            break;
    }
}

// ****************************************************************************
//  CHAMAR NA ROTINA DE INTERRUPCAO DA SERIAL
// ****************************************************************************
//
//
// EXEMPLO DE USO, ESTA NO MAIN.C E POR POLLING EU OBTIA OS DADOS RECEBIDOS
// estado_CUP = maquinaDeEstadosDeRecepcaoCUP(estado_CUP, buf[dado]);
//

//volatile u16 crc;
int maquinaDeEstadosDeRecepcaoCUP (int dado){
    int cmp;

    //Ocorreu processamento, logo atualizo o timeout
    //_TRISG12 = 0;
    //_LATG12 = 1;
    contadorTimeOutRecepcaoCUP = TEMPO_PARA_TIMEOUT_RECEPCAO;

    flagDeDadosRecebidos = 1;


   switch (EstadoDeRecepcaoDaMaquinaCUP){
       case CABECA_1:
           if (dado == 0x43){
               _LATA2 = 1;
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_2;
               chksum_gerado = ResetaCheckSum();
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
               _LATA2 = 0;
           }
           else EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           break;

       case CABECA_2:
           if (dado == 0x55){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_3;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }
           break;

       case CABECA_3:
           if (dado == 0x50){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_4;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }
           break;

       case CABECA_4:
           if (dado == 0x01){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_5;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }

           break;

       case CABECA_5:
           cmp = (int)(0x00FF & (MEU_ID >> 24) );
           if ( (dado ==  cmp) || ((u8)dado == 0xFF) ){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_6;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }
           //CabecaCUP(0,0);
           //estado = CABECA_1;
           break;

       case CABECA_6:
           cmp = (int)(0x00FF & (MEU_ID >> 16) );
           if ( (dado == cmp)  || ((u8)dado == 0xFF) ){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_7;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }

           //CabecaCUP(0,0);
           //if (dado == 0x55)estado = CABECA_6;
           //else estado = CABECA_1;

           break;

       case CABECA_7:
           cmp = (int)(0x00FF & (MEU_ID >> 8) );
           if ( (dado == cmp) || ((u8)dado == 0xFF) ){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_8;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }
           break;

       case CABECA_8:
           cmp = (int)(0x00FF & MEU_ID);
           if ( (dado == cmp) || ((u8)dado == 0xFF) ){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_9;
               //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
               chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           }
           else {
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               //_LATA2 = 0;
           }
           break;

       case CABECA_9:
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_10;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           //else estado = CABECA_1;
           break;

       case CABECA_10:
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_11;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           //else estado = CABECA_1;
           break;

       case CABECA_11:
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_12;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           //else estado = CABECA_1;
           break;

       case CABECA_12:
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_13;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           //else estado = CABECA_1;
           break;

       case CABECA_13://MSB quantidade de dado
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_14;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           QuantidadeDeDadosRecebidos = dado;
           QuantidadeDeDadosRecebidos = QuantidadeDeDadosRecebidos << 8;

           break;

       case CABECA_14://LSB quantidade de dado
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_15;
           //chksum_gerado = GeraCheckSum(chksum_gerado, &dado, 1);
           chksum_gerado = GeraCheckSum(chksum_gerado, (char *)dado, 1);    //LUCIANO: 'dado' eh uma string. Nao precisa do & para passar seu endereco. Eliminei um warning.
           QuantidadeDeDadosRecebidos = QuantidadeDeDadosRecebidos | dado;

           break;

       case CABECA_15://MSB CRC
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_16;
           chksum_Recebido = dado;
           chksum_Recebido = chksum_Recebido << 8;

           break;

       case CABECA_16://LSB CRC
           chksum_Recebido = chksum_Recebido | (unsigned char)dado;
           if (chksum_gerado == chksum_Recebido){
            //CabecaCUP(0,0);
            EstadoDeRecepcaoDaMaquinaCUP = CABECA_17;
           } else {
            EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
            //_LATA2 = 0;
           }

           break;

       case CABECA_17:
            //_LATA2 = 0;
           if (dado == 0){
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           }


           //tempoParaPulsoAlto = 0;
           //tempoParaPulsoBaixo = 0;

           if (dado == 0x44){
               EstadoDeRecepcaoDaMaquinaCUP = PEGAR_PINO_PULSO_BAIXO;
               comandoRecebido = dado;
               break;
           }

           if (dado == 0x45){
               EstadoDeRecepcaoDaMaquinaCUP = PEGAR_PINO_PULSO_ALTO;
               comandoRecebido = dado;
               break;
           }


           if (dado == 0x43){
               EstadoDeRecepcaoDaMaquinaCUP = SETAR_SAIDA_1;
           } else {
               transmitindo = 1;
               comandoRecebido = dado;
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
               ResetContadorDeEnvidoDoMeuID();

           }

           
           //_LATA2 = 0;
           //EstadoDeRecepcaoDaMaquinaCUP = IDLE;


           //Em vez de tratar aqui, o faco na rotina processarCup
           /*
           if (dado == 0x41){

               ProcessaComandosTagsPresentes();
               CabecaCUP(1, (tagsAEnviar *  TAMANHO_TAG) );
               crcRecebido = CRC_INICIAL;
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           }

           if ((u8)dado == 0x43){

               EstadoDeRecepcaoDaMaquinaCUP = SETAR_SAIDA_1;
           }

           if ((u8)dado == 0xFF){
               crcRecebido = CRC_INICIAL;
               //resetaContador();
               //while (valorAtualDOContador() < 100);
               //delay_ms(4000);
               //pararContador();
               CabecaCUP(1, 0);
               EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           }

           */


           break;
           
       case IDLE:
           break;

       case ENVIAR_TAGS_1:


            EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
            break;

       case SETAR_SAIDA_1:
           PortaSelecionada = dado;
           EstadoDeRecepcaoDaMaquinaCUP = SETAR_SAIDA_2;
           break;

        case SETAR_SAIDA_2:
            acionaSaida(PortaSelecionada, dado);
            EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
            break;

       case PEGAR_PINO_PULSO_BAIXO:
           EstadoDeRecepcaoDaMaquinaCUP = PEGAR_MSB_PULSO_BAIXO;
           pinoASerAcionado = dado;
           break;

       case PEGAR_MSB_PULSO_BAIXO:
           tempoParaPulsoBaixo[pinoASerAcionado] = dado;
           tempoParaPulsoBaixo[pinoASerAcionado] = tempoParaPulsoBaixo[pinoASerAcionado] << 8;


           EstadoDeRecepcaoDaMaquinaCUP = PEGAR_LSB_PULSO_BAIXO;
           break;


       case PEGAR_LSB_PULSO_BAIXO:
            tempoParaPulsoBaixo[pinoASerAcionado] = tempoParaPulsoBaixo[pinoASerAcionado] + dado;
            EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           break;

       case PEGAR_PINO_PULSO_ALTO:
           EstadoDeRecepcaoDaMaquinaCUP = PEGAR_MSB_PULSO_ALTO;
           pinoASerAcionado = dado;
           break;


       case PEGAR_MSB_PULSO_ALTO:
           tempoParaPulsoAlto[pinoASerAcionado] = dado;
           tempoParaPulsoAlto[pinoASerAcionado] = tempoParaPulsoAlto[pinoASerAcionado] << 8;
           EstadoDeRecepcaoDaMaquinaCUP = PEGAR_LSB_PULSO_ALTO;

           break;

       case PEGAR_LSB_PULSO_ALTO:
           tempoParaPulsoAlto[pinoASerAcionado] = tempoParaPulsoAlto[pinoASerAcionado] + dado;
            EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           break;

       default:
           EstadoDeRecepcaoDaMaquinaCUP = CABECA_1;
           break;
   }

   //_LATG12 = 0;

   //ResetContadorDeEnvidoDoMeuID();
   return 0;
};

int CUPComandoEnviarTagsPresentes (int estado, int dado){
    //int cmp;      //LUCIANO: Variavel declarada e nao utilizada. Eliminei um warning.

    switch (estado){

        default:
            estado = 0;
            break;
    }
    return estado;
};

/*
void ProcessaEnvioDeTag (void){
    if (FlagParaEnviarTags != 0){
        if (IEC0bits.U1TXIE == 0){
            if (tagArmazenadas != 0){
                //EnviaPacoteParaSerial(BufferDeTags[tagArmazenadas - 1][TAMANHO_TAG], TAMANHO_BUFFER_DE_TAGS);
                EnviaPacoteParaSerial(&BufferDeTags[tagArmazenadas - 1], TAMANHO_BUFFER_DE_TAGS);
                eliminaTagDoBuffer(tagArmazenadas - 1);
                tagArmazenadas = tagArmazenadas - 1;

                if (((tagArmazenadas - 1)== -1) || ((tagArmazenadas - 1) == 0xFFFF)){
                  FlagParaEnviarTags = 0;
                }

            }
        }
    }
}
*/

void EnviaCRC (void){
    int crc_temporario;
    u8 data[2];
    crc_temporario = crcASerEnviado;
    data[0] = (u8)(crc_temporario  & 0xFF);
    data[1] = (u8)((crc_temporario >> 8)  & 0xFF);
    //EnviaPacoteParaSerial(data,2);
    EnviaPacoteParaSerial((const char *)data,2);    //LUCIANO: Apliquei um cast em data (string).Eliminei um warning.

    /*
    if (TagSendoTratada > (TAMANHO_BUFFER_DE_TAGS - 1)){
        TagSendoTratada = TagSendoTratada + 1;
        TagSendoTratada = 0;
        ResetaEnvio();

        data[0] = (u8)(crc_temporario  & 0xFF);
        data[1] = (u8)((crc_temporario >> 8)  & 0xFF);

        //Dummy, apenas para o controle de direcao.
        //data[2] = 0;
        //data[3] = 0;
        EnviaPacoteParaSerial(data,2);
        //EnviaPacoteParaSerial(data,4);

    }
     * */
}

// *************************************************************************
// *************************************************************************
// Manipulacao de memoria
// *************************************************************************
// *************************************************************************

int tagJaPresenteNoBuffer (unsigned char * tag, int posicao, int tamanho){
    //int contador;

    if (memcmp (&BufferDeTags[posicao][0], tag, tamanho) == 0)
        return 0;


    /*
    for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        //if (memcmp (&BufferDeTags[contador][posicao], &tag[posicao], tamanho) == 0)
        if (memcmp (&BufferDeTags[contador][posicao], &tag[posicao], tamanho) == 0)
            return 0;
    }
    */
    return -1;
}

int adicionarTagAoBuffer (int posicao, unsigned char * dados, int tamanho){
    int contador;
    if (posicao > TAMANHO_BUFFER_DE_TAGS)
        return -1;
    for (contador = 0;contador < tamanho;contador = contador + 1){
        BufferDeTags[posicao][contador] = *dados;
        dados = dados + 1;
    }
    return posicao + 1;
}

//Retorna uma posicao livre no buffer de tags.
int posicaoLivreNoBufferDeTags (void){
    int contador;
    int posicao;
    int dado;


    for (posicao = 0;posicao < TAMANHO_BUFFER_DE_TAGS;posicao = posicao + 1){
        dado = 0;
        //for (contador = 0;(contador < TAMANHO_TAG) && (dado != 0);contador = contador + 1){
        for (contador = 0;(contador < TAMANHO_TAG);contador = contador + 1){
            dado = dado | BufferDeTags[posicao][contador];
        }

        if ((dado == 0) && (posicao < TAMANHO_BUFFER_DE_TAGS) ){
            return posicao;
        }
    }
    return -1;
}

//Posicao ocucapada?
int posicaoOcupadaNoBufferDeTags (int posicao){
    int contador;
    //int dado;

    for (contador = 0;(contador < TAMANHO_TAG);contador = contador + 1){
        //dado = BufferDeTags[posicao][contador];
        //if (dado != 0)
            //return posicao;

        if (BufferDeTags[posicao][contador])
            return posicao;
    }

    return -1;
}


void eliminaTagDoBuffer (int posicao){
    (void)memset(BufferDeTags[posicao], 0, TAMANHO_TAG);
}

void EnviaBufferDeTagsAoPC_PARA_TESTE (void){
    if (ContadorDeTX == 0){
        EnviaBytePara485(TAMANHO_EPC);
        memset(BufferDeTags[0], 1, TAMANHO_EPC);
        BufferDeTags[0][0] = 0x55;
        BufferDeTags[0][22] = (unsigned char)contadorDeTagProcesada;

            BufferDeTags[0][POSICAO_TAGS_LEFT] = 0;
            BufferDeTags[0][POSICAO_TAGS_IN_REPLY] = 1;
            BufferDeTags[0][POSICAO_RSSI] = 0x80;//RSSI
            BufferDeTags[0][POSICAO_FREQUENCIA_LOW] = 0x38;
            BufferDeTags[0][POSICAO_FREQUENCIA_MID] = 0xF6;
            BufferDeTags[0][POSICAO_FREQUENCIA_HIGH] = 0x0D;

            BufferDeTags[0][POSICAO_TAMANHO_EPC_MAIS_TAMANHO_PC] = 11;

        //EnviaPacoteParaSerial(BufferDeTags[0], TAMANHO_EPC);
        EnviaPacoteParaSerial((const char *)BufferDeTags, TAMANHO_EPC); //LUCIANO: Apliquei um cast em BufferDeTags (string). Eliminei um warning.

        contadorDeTagProcesada = contadorDeTagProcesada + 1;
    }

}
void EnviaBufferDeTagsAoPC (void){
    if (ContadorDeTX == 0){
        /*
        if (posicaoOcupadaNoBufferDeTags(contadorDeTagProcesada) != -1){
            EnviaBytePara485(23);
            EnviaPacoteParaSerial(BufferDeTags[contadorDeTagProcesada], 23);
            eliminaTagDoBuffer(contadorDeTagProcesada);
        }
        contadorDeTagProcesada = contadorDeTagProcesada + 1;
        */
        if (posicaoOcupadaNoBufferDeTags(contadorDeTagProcesada) != -1){
            EnviaBytePara485(TAMANHO_EPC);
            //EnviaPacoteParaSerial(BufferDeTags[contadorDeTagProcesada], TAMANHO_EPC);
            EnviaPacoteParaSerial((const char *)BufferDeTags[contadorDeTagProcesada], TAMANHO_EPC); //LUCIANO: Apliquei um cast em BufferDeTags (string). Eliminei um warning.
            eliminaTagDoBuffer(contadorDeTagProcesada);
            contadorDeTagProcesada = contadorDeTagProcesada + 1;
        } else {
            contadorDeTagProcesada = 0xFF;
        }


    }

}


void EnviaBufferDeTagsAoPC_bloqueia (void){
    int contador;
    //int contador_envio;

    /*
    //Teste para formatacao da carga
    EnviaBytePara485(strlen(BufferDeTags[0]));
    EnviaPacoteParaSerial(BufferDeTags[0], strlen(BufferDeTags[0]));
    while (LimiteDoContadorDeTX != 0);
    */

    //Envio de carga formatada
    for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        if (posicaoOcupadaNoBufferDeTags(contador) != -1){
            //EnviaStringPara485(BufferDeTags[contador]);
            //EnviaPacoteParaSerial(BufferDeTags[contador], 0x0C);


            //EnviaBytePara485(strlen(BufferDeTags[contador]));
            //EnviaPacoteParaSerial(BufferDeTags[contador], strlen(BufferDeTags[contador]));

            EnviaBytePara485(TAMANHO_EPC);
            //EnviaPacoteParaSerial(BufferDeTags[contador], TAMANHO_EPC);
            EnviaPacoteParaSerial((const char *)BufferDeTags[contador], TAMANHO_EPC);   //LUCIANO: Apliquei um cast em BufferDeTags (string). Eliminei um warning.
            eliminaTagDoBuffer(contador);

            while (LimiteDoContadorDeTX != 0);
        }
    }


    /*
    //Envio de carga nao formatada
    for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        if (posicaoOcupadaNoBufferDeTags(contador) != -1){
            //EnviaStringPara485(BufferDeTags[contador]);
            //EnviaPacoteParaSerial(BufferDeTags[contador], 0x0C);
            EnviaPacoteParaSerial(BufferDeTags[contador], strlen(BufferDeTags[contador]));
            while (LimiteDoContadorDeTX != 0);
        }
    }
    */
}

void LimpaBufferDeTags (void){
    int contador;

    for (contador = 0;contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        memset(BufferDeTags[contador], 0,TAMANHO_TAG );
    }
}


int tagsPresentesNoBuffer (void){
    int contador;
    int resultado;

    resultado = 0;
    for (contador = 0; contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        if (posicaoOcupadaNoBufferDeTags(contador) != -1)
                resultado = resultado + 1;
    }

    //tagsAEnviar = resultado;
    //resultado = resultado * TAMANHO_TAG;
    return resultado;
}

int QuantiaDeDadosAEnviaDoBufferDeTags (void){
    int contador;
    int resultado;

    resultado = 0;
    for (contador = 0; contador < TAMANHO_BUFFER_DE_TAGS;contador = contador + 1){
        if (posicaoOcupadaNoBufferDeTags(contador) != -1){
                //resultado = resultado + strlen(BufferDeTags[contador]);
                resultado = resultado + TAMANHO_EPC;
                resultado = resultado + 1; // Adicao para ter previamente o byte de quantia de dados da tag
        }

    }

#ifdef TESTE_CARGA
    resultado = 24 * TAMANHO_BUFFER_DE_TAGS; // APENAS PARA TESTE DE CARGA
#endif

    //tagsAEnviar = resultado;
    //resultado = resultado * TAMANHO_TAG;
    return resultado;

}

