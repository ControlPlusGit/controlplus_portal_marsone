import serial
import tkinter
from tkinter import ttk
import time
from time import gmtime, strftime
import datetime
import threading

Serial = serial.Serial()

Janela = tkinter.Tk()

baudRate485 = 57600

Janela.title('Avaliador de portais')
CaixaDa485 = ttk.Frame(Janela)
Log = tkinter.Listbox(Janela, height=30, width=120, font="Times 8")
CaixaDaSerial = ttk.Frame(CaixaDa485)
#Log.place(x=0,y=0)

#CaixaDaSerial = ttk.Frame(Janela)


def listarPortasSeriais():
   resultado = []
   portas = ['COM%s' % (i + 1) for i in range(256)]
   for porta in portas:
      try:
         s = serial.Serial(porta)
         resultado.append(porta)
         s.close()
      except:
         pass
   return resultado

def abreFechaSerial485 ():
    global Serial
    if Serial.isOpen():
        Serial.close()
        botaoAbreFechaSerial485.config(text="Abre Serial")
        return 0
    else:
        try:
            Serial = serial.Serial(ComboSerial.get(), baudRate485, timeout=1)
            botaoAbreFechaSerial.config(text="Fecha Serial")
        except:
            print('erro ao abrir a serial')

         
def listarPortasSeriais485():
   ComboSerial['values'] = listarPortasSeriais()
   try:
      ComboSerial.current(0)
   except:
      pass





ComboSerial = ttk.Combobox(CaixaDaSerial, state='readonly')
botaoAtualizaSeriais485 = tkinter.Button(CaixaDaSerial, text="Atualiza seriais", command=listarPortasSeriais485)
botaoAbreFechaSerial485 = tkinter.Button(CaixaDaSerial, text="Abre Serial", command=abreFechaSerial485)

botaoAtualizaSeriais485.pack(side='left')
ComboSerial.pack(side='left')
botaoAbreFechaSerial485.pack(side='left')
CaixaDaSerial.pack(side='left')

Log.pack()
#Label485 = tkinter.Label(CaixaDa485, text='Log da 485')
CaixaDa485.pack()

#CaixaDaSerial485.pack();


def lidaComASerialDa485():
   global Serial
   global BaudRate485
   while (True):
        try:
            if Serial.isOpen():
                NumeroDeDadosASeremLidos = Serial.inWaiting()
                leitura = Serial.read(NumeroDeDadosASeremLidos)
                if len(leitura) != 0:
                    for s in leitura:
                       print(s)
                        #if AvalizadorJuizDeFora.processaDadosRecebidosPor485(s) == 1:
                           #Saida = AvalizadorJuizDeFora.interpretaDadosRecebidosPor485(PacoteRecebidoPor485)
                           #LogDa485.insert(tkinter.END, Saida)
                Porta = ComboSerial.get()
                if (Serial.name != Porta):
                    if Porta != '':
                        print(Serial.name)
                        print(Porta)
                        Serial.close()
                        Serial = serial.Serial(Porta, baudRate485, timeout=1)
            else:
                pass
                #Porta = Serial485.get()
                #Serial485 = serial.Serial(Porta, baudRate485, timeout=1)
        except:
            #print('erro na leitura da serial')
            time.sleep(1)


def enviaComandoParaOLeitor(Comando):
   pass  


def eecebidoComandoDoLeitor(Comando):
   pass






Janela.geometry("800x600")
Janela.resizable(width=False, height=False)

thread485 = threading.Thread(target=lidaComASerialDa485, daemon=True)
thread485.start()


Janela.mainloop()
