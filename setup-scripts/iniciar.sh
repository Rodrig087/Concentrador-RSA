#!/bin/bash

# Copiar el directorio "Configuracion" a la ruta /home/rsa/
cp -r configuracion /home/"$USER"/

# Crea los directorios necesarios
mkdir -p /home/"$USER"/ejecutables
mkdir -p /home/"$USER"/log-files
mkdir -p /home/"$USER"/tmp
mkdir -p /home/"$USER"/resultados/eventos-detectados

# Compila todos los programas escritos en C 
sh setup-scripts/compilar.sh

# Copia los scripts de Python a la carpeta /home/rsa/ejecutables
cp /home/"$USER"/Concentrador-RSA/programas/*.py /home/"$USER"/ejecutables/