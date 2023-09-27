#!/bin/bash

# Compila todos los programas de la carpeta Programas 
gcc /home/"$USER"/Concentrador-RSA/programas/IniciarSensorNivel*.c -o /home/"$USER"/ejecutables/iniciarsensornivel -lbcm2835 -lwiringPi -lm
gcc /home/"$USER"/Concentrador-RSA/programas/LeerSensorNivel*.c -o /home/"$USER"/ejecutables/leersensornivel -lbcm2835 -lwiringPi -lm
gcc /home/"$USER"/Concentrador-RSA/programas/MedirVertedero*.c -o /home/"$USER"/ejecutables/medirvertedero -lbcm2835 -lwiringPi -lm
gcc /home/"$USER"/Concentrador-RSA/programas/ResetConcentrador.c -o /home/"$USER"/ejecutables/resetconcentrador -lbcm2835 -lwiringPi 
gcc /home/"$USER"/Concentrador-RSA/programas/TestSPI.c -o /home/"$USER"/ejecutables/testspi -lbcm2835 -lwiringPi -lm