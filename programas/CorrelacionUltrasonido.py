import numpy as np
from scipy import signal
from scipy.signal import correlate
from scipy.signal import butter,bessel,filtfilt, lfilter
import math

#******************************************************************************
#Constantes:
sizeTramaShort = 702
sizeTramaInt = 350
periodoMuestreo = 5
#Variables:
factorResample = 10
periodoResample = periodoMuestreo/factorResample
dix = 25*factorResample
senal1 = []
senal2 = []
#******************************************************************************

#******************************************************************************
#Funciones:
def RemuestrearOffset(arrayMuestras, offset):
    #Convierte el array a formato np:
    npMuestras = np.array(arrayMuestras)
    resampleMuestras = signal.resample(npMuestras, len(npMuestras) * factorResample)
    resampleMuestras = resampleMuestras - offset
    return resampleMuestras
#******************************************************************************	
    
#*****************************************************************************
#Carga la senal de referencia: 
rutaArchivoReferencia = "/home/rsa/configuracion/"
nombreArchivoReferencia = "Referencia_Btt-39-41-3.npy"
pathArchivoReferencia = rutaArchivoReferencia + nombreArchivoReferencia
referencia = np.load(pathArchivoReferencia)
##Abre el archivo binario::
nombreArchivo2 = input("Ingrese el nombre del segundo archivo: ")
rutaCarpeta = "/home/rsa/mediciones/vertederos/"
path2 = rutaCarpeta + str(nombreArchivo2) + ".dat"
#Abre y convierte el segundo archivo:
f = open(path2, "rb")
tramaDatosShort = np.fromfile(f, np.int8, sizeTramaShort)
f.close()
#*****************************************************************************

#*****************************************************************************
#Convierte el vector de datos tipo short en un vector de tipo int
for i in range(0, sizeTramaInt - 1):
    datoIntLSB = tramaDatosShort[i * 2] 
    datoIntMSB = tramaDatosShort[(i * 2)+1]
    datoInt = ((datoIntMSB << 8) & 0xFF00) + ((datoIntLSB) & 0xFF)
    senal2.append(datoInt)
#*****************************************************************************

#*****************************************************************************
#Obtiene los bytes de temperatura de la trama:
temperaturaLSB = tramaDatosShort[sizeTramaShort - 2]
temperaturaMSB = tramaDatosShort[sizeTramaShort - 1]
temperaturaRaw = ((temperaturaMSB << 8) & 0xFF00) + ((temperaturaLSB) & 0xFF)
#Verifica si la temperatura es negativa:
if (temperaturaRaw & 0x8000):
    signoTemp = -1
    temperaturaRaw = ~temperaturaRaw + 1
else:
    signoTemp = 1
#Calcula la parte entera de la temperatura:
temperaturaInt = (temperaturaRaw >> 4) * signoTemp
#Calcula la parte decimal de la temperatura:
temperaturaFloat = ((temperaturaRaw & 0x000F) * 625) / 10000.0
#Calcula la temperatura:
dTemp = -0.5
temperaturaSensor = temperaturaInt + temperaturaFloat + dTemp
Vsonido = 331.45 * math.sqrt(1 + (temperaturaSensor / 273))
#*****************************************************************************

#******************************************************************************
#Procesamiento de la señal:
banProcesar = 's'
if (banProcesar=='s'):
    
    print('Procesando...')
    
    #Normaliza las senales:
    senal1 = RemuestrearOffset(referencia, 0)
    #senal2 = RemuestrearOffset(senal2, 508)
    senal2 = RemuestrearOffset(senal2, 517)
    nsamples = np.size(senal1)
        
    #Calcula la correlacion cruzada de la senal ultrasonica con la senal de referencia:
    xcorr = correlate(senal1, senal2)
    dt = np.arange(1-nsamples, nsamples)
    recovered_time_shift = dt[xcorr.argmax()]
    
    #Calculo del desfase en us:
    desfaseTiempo = recovered_time_shift*-1*periodoResample
    dA = -80.5
    TOF = 1375 + desfaseTiempo + dA
    Distancia = 0.5*(TOF/1e6)*Vsonido*1000
    
    print("Desfase [num muestras]: %d" % recovered_time_shift) 
    print("Periodo muestreo [us]: %f" % (periodoResample)) 
    print("Temperatura [gC]: %f" % temperaturaSensor) 
    print("Vsonido [m/seg]: %f" % Vsonido)
    print("Desfase [us]: %f" % desfaseTiempo) 
    print("TOF [us]: %f" % TOF) 
    print("Distancia [mm]: %f" % Distancia) 
    
#******************************************************************************