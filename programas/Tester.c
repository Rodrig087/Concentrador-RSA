//Compilar:
//gcc Tester.c -o /home/pi/Ejecutables/tester -lbcm2835 -lwiringPi 
//gcc Tester.c -o tester -lbcm2835 -lwiringPi 

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <bcm2835.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


unsigned int contador;


int main(int argc, char *argv[]) {
	
	contador = 0;

	while(1){	
		
		    printf("\n");
			printf("\nPrueba numero: %d\n", contador); 
			//system("sudo ./inspeccionarevento 1 1768190");
			system("sudo ./leerds18");
			delay (1000);
			
			contador++;
			//printf("\n");
			//delay (1000);
	
	}

}

