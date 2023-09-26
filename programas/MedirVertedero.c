//Compilar:
//gcc MedirVertedero.c -o /home/rsa/Ejecutables/medirvertedero -lbcm2835 -lwiringPi  

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <bcm2835.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

unsigned short idVertedero;

int main(int argc, char *argv[]){
	
	idVertedero = (char)(atoi(argv[1]));
	
	switch (idVertedero){
		  case 1:
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 1");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 1");
					delay (1000);
					break;	
		  case 2:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 2");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 2");
					delay (1000);
					break;			
		  case 3:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 3");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 3");
					delay (1000);
					break;
		  case 4:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 4");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 4");
					delay (1000);
					break;
		  case 5:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 5");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 5");
					delay (1000);
					break;
		  case 6:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 6");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 6");
					delay (1000);
					break;
		  case 7:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 7");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 7");
					delay (1000);
					break;	
		  case 8:
					printf("hola");
					system("sudo /home/rsa/Ejecutables/iniciarsensornivel 8");
					delay (1000);
					system("sudo /home/rsa/Ejecutables/leersensornivel 8");
					delay (1000);
					break;					
	}
						
}