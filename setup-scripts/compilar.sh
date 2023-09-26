#!/bin/bash

# Compila todos los programas de la carpeta programas 
gcc programas/IniciarSensorNivel*.c -o /home/"$USER"/ejecutables/iniciarsensornivel -lbcm2835 -lwiringPi 
gcc programas/LeerSensorNivel*.c -o /home/"$USER"/ejecutables/leersensornivel -lbcm2835 -lwiringPi 
gcc programas/MedirVertedero*.c -o /home/"$USER"/ejecutables/medirvertedero -lbcm2835 -lwiringPi  
gcc programas/ResetConcentrador.c -o /home/"$USER"/ejecutables/resetconcentrador -lbcm2835 -lwiringPi 
gcc programas/TestSPI.c -o /home/"$USER"/ejecutables/testspi -lbcm2835 -lwiringPi