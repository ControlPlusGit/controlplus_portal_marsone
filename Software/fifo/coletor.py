import time
import serial

Ser = serial.Serial('COM24', 57600, timeout=0)


DadosObtidos = []

UmaVez = 0


def ColetaMisoEEnviaParaPortaSerial():
    global Ser
    Arq = open('miso.txt', 'r')
    for linha in Arq.readlines():
        #print(linha)
        for Dado in linha:
            Ser.write(bytes([ord(Dado)]))
    Arq.close()
    apagaArquivo()
    Saida = Ser.readlines()
    
    if len(Saida) != 0:
        Arq = open('mosi.txt', 'a')
        for Linha in Saida:
            #Arq.write(Linha)
            #print(Linha)
            #Linha = Linha.decode()
            for Dado in Linha:
                #print("Procesando")
                print(int(Dado))
                Arq.write(chr(Dado))
        Arq.close()
    #print("Terminou processamento!")
    time.sleep(1)


def coletaDadosDeArquivo():
    global UmaVez
    Arq = open('miso.txt', 'r')
    for linha in Arq.readlines():
        print(linha)
    Arq.close()
    apagaArquivo()
    pedeReenvio()
    #confirmaRecepcao()
    if UmaVez == 0:
        pass
        #pedeReenvio()
        #UmaVez = 1
    else:
        pass
        #confirmaRecepcao()

def retornaComOCheckSumDeUmPacote(Pacote):
    Tamanho = len(Pacote)
    Resultado = 0
    for Contador in range(0, Tamanho):
        Resultado = Resultado + Pacote[Contador]
    Resultado = Resultado & 0x000000FF
    return Resultado

def pedeReenvio():
    Pacote = []
    Arq = open('mosi.txt', 'w')

    Pacote.append(0xFD)
    Pacote.append(0x06)
    Pacote.append(0xAA)

    Pacote.append(0xFF)
    Pacote.append(0xFF)

    Pacote.append(retornaComOCheckSumDeUmPacote(Pacote))

    for Dado in Pacote:
        Arq.write(chr(Dado))
    Arq.close()

def confirmaRecepcao():
    Pacote = []
    Arq = open('mosi.txt', 'w')

    Pacote.append(0xFE)
    Pacote.append(0x06)
    Pacote.append(0x55)

    Pacote.append(0xAA)
    Pacote.append(0xBB)

    Pacote.append(retornaComOCheckSumDeUmPacote(Pacote))

    for Dado in Pacote:
        Arq.write(chr(Dado))
    Arq.close()

def apagaArquivo():
    Arq = open('miso.txt', 'w')
    Arq.close()


while (True):
    #coletaDadosDeArquivo()
    ColetaMisoEEnviaParaPortaSerial()
