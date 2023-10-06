#!/bin/bash

# Compila todos los programas de la carpeta Programas 
gcc /home/"$USER"/Concentrador-RSA/programas/IniciarMediciones*.c -o /home/"$USER"/ejecutables/iniciarmediciones -lbcm2835 -lwiringPi -lm
gcc /home/"$USER"/Concentrador-RSA/programas/LeerSensorNivel*.c -o /home/"$USER"/ejecutables/leersensornivel -lbcm2835 -lwiringPi -lm
#gcc /home/"$USER"/Concentrador-RSA/programas/MedirVertedero*.c -o /home/"$USER"/ejecutables/medirvertedero -lbcm2835 -lwiringPi -lm
gcc /home/"$USER"/Concentrador-RSA/programas/ResetConcentrador.c -o /home/"$USER"/ejecutables/resetconcentrador -lbcm2835 -lwiringPi 
gcc /home/"$USER"/Concentrador-RSA/programas/TestComunicacion.c -o /home/"$USER"/ejecutables/testcomunicacion -lbcm2835 -lwiringPi -lm