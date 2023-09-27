#!/bin/bash

# Copiar el directorio "Configuracion" a la ruta /home/rsa/
cp -r configuracion /home/rsa/

# Crea los directorios necesarios
mkdir -p /home/rsa/ejecutables
mkdir -p /home/rsa/log-files
mkdir -p /home/rsa/tmp
mkdir -p /home/rsa/resultados/eventos-detectados

# Compila todos los programas escritos en C 
sh setup-scripts/compilar.sh

# Copia los scripts de Python a la carpeta /home/rsa/ejecutables
cp /home/rsa/Acelerografo-RSA/programas/*.py /home/rsa/ejecutables/