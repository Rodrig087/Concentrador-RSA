#from matplotlib.widgets import Cursor
import matplotlib.pyplot as plt
import numpy as np
import time
import os
#import errno

#Variables:
sizeTramaShort = 702
sizeTramaInt = 350
tramaDatosInt = []

#*****************************************************************************
# Obtiene el ultimo archivo binario guardado en el directorio:
directorio = '/home/rsa/mediciones/vertederos/'
# Obtenemos una lista de archivos en el directorio
archivos = os.listdir(directorio)
# Filtramos solo los archivos regulares, excluyendo directorios
archivos_regulares = [archivo for archivo in archivos if os.path.isfile(os.path.join(directorio, archivo))]
# Ordenamos la lista de archivos por fecha de modificación (el más reciente primero)
archivos_regulares.sort(key=lambda x: os.path.getmtime(os.path.join(directorio, x)), reverse=True)
# Verificamos si hay archivos en el directorio
if archivos_regulares:
    # Tomamos el nombre del archivo más reciente
    ultimo_archivo = archivos_regulares[0]
    print(f'El último archivo agregado es: {ultimo_archivo}')
else:
    print('No hay archivos en el directorio.')
#*****************************************************************************

#*****************************************************************************
# Abre el archivo binario
path = directorio + ultimo_archivo
f = open(path, "rb")
tramaDatosShort = np.fromfile(f, np.int8, sizeTramaShort)
#*****************************************************************************

print("Graficando... ") 

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
temperaturaSensor = temperaturaInt + temperaturaFloat
#Imprime los valores de temperatura:
#print("   Temperatura LSB: " + hex(temperaturaLSB))
#print("   Temperatura MSB: " + hex(temperaturaMSB))
#print("   Temperatura Raw: " + str(temperaturaRaw))
print("   Temperatura sensor: " + str(temperaturaSensor))
#*****************************************************************************

#*****************************************************************************
#Convierte el vector de datos tipo short en un vector de tipo int
for i in range(0, sizeTramaInt - 1):
    #datoIntLSB = ((tramaDatosShort[i * 2] << 8) & 0xFF00)
    #datoIntMSB = ((tramaDatosShort[(i * 2)+1]) & 0xFF)
    #datoInt = datoIntLSB + datoIntMSB
    datoIntLSB = tramaDatosShort[i * 2] 
    datoIntMSB = tramaDatosShort[(i * 2)+1]
    datoInt = ((datoIntMSB << 8) & 0xFF00) + ((datoIntLSB) & 0xFF)
    tramaDatosInt.append(datoInt)

datoPrueba = tramaDatosInt[184]

print("   DatoInt: " + hex(datoPrueba))

plt.plot(tramaDatosInt)
plt.show()

