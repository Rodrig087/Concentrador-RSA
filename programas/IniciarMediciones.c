//Compilar:
//gcc IniciarMediciones.c -o iniciarmediciones-lbcm2835 -lwiringPi 
//gcc IniciarMediciones.c -o /home/rsa/Ejecutables/iniciarmediciones -lbcm2835 -lwiringPi

/*-------------------------------------------------------------------------------------------------------------------------
Autor: Milton Munoz
Fecha de creacion: 23/10/06
Observaciones:
Incia las mediciones en todos los sensores enviando la instruccion a la direccion broadcast.
       
-------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <bcm2835.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


//Declaracion de constantes
#define P0 0																	//Pin 11 GPIO
#define MCLR 28																	//Pin 38 GPIO
#define LEDTEST 29 																//Pin 40 GPIO																						
#define TIEMPO_SPI 100
#define FreqSPI 200000

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

//Declaracion de variables
unsigned short i;
unsigned int x;

unsigned short idPet;
unsigned short funcionPet;
unsigned short subFuncionPet;
unsigned short numDatosPet;


unsigned char payloadPet[255];
unsigned char payloadResp[255];


int rawTemp;
unsigned int temperaturaInt;
float temperaturaFloat, temperaturaResultado;
unsigned char *ptrRawTemp, *ptrTemperaturaResultado;
short signoTemp;


//Declaracion de funciones
int ConfiguracionPrincipal();
void EnviarSolicitud(unsigned short id, unsigned short funcion, unsigned short subFuncion, unsigned short numDatos, unsigned char* payload); //C:0xA1    F:0xF1														//C:0xA4	F:0xF4
void Salir();						


int main(){
 
	//Inicializa las variables:
	i = 0;
	x = 0;
			
	//Configuracion principal:
	ConfiguracionPrincipal();
	
	//Datos de prueba:
	idPet = 255;
	funcionPet = 1;
	subFuncionPet = 2;
	numDatosPet = 0;
			
	EnviarSolicitud(idPet, funcionPet, subFuncionPet, numDatosPet, payloadPet);
	
	while(1){}
	
}

//**************************************************************************************************************************************
//Configuracion:

int ConfiguracionPrincipal(){
	
    //Configuracion libreria bcm2835:
	if (!bcm2835_init()){
		printf("bcm2835_init fallo. Ejecuto el programa como root?\n");
		return 1;
    }
    if (!bcm2835_spi_begin()){
		printf("bcm2835_spi_begin fallo. Ejecuto el programa como root?\n");
		return 1;
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);					//Clock divider RPi 3		
	bcm2835_spi_set_speed_hz(FreqSPI);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
		
	//Configuracion libreria WiringPi:
    wiringPiSetup();
	pinMode(MCLR, OUTPUT);
	pinMode(LEDTEST, OUTPUT);
	
	
	//Enciende el pin LEDTEST:
	digitalWrite (LEDTEST, HIGH);
		
}

//**************************************************************************************************************************************
//Comunicacion RPi-dsPIC:
//C:0xA1	F:0xF1
void EnviarSolicitud(unsigned short id, unsigned short funcion, unsigned short subFuncion, unsigned short numDatos, unsigned char* payload){
	
	bcm2835_delayMicroseconds(200);
	
	//Envia la cabecera: [id, funcion, subfuncion, #Datos]:
	bcm2835_spi_transfer(0xA0);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	bcm2835_spi_transfer(id);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	bcm2835_spi_transfer(funcion);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	bcm2835_spi_transfer(subFuncion);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	bcm2835_spi_transfer(numDatos);
	bcm2835_spi_transfer(0x00);     //Envia este byte para simular el MSB de la variable numDatos. Es poco probable que una solicitud tenga un pyload de mas de 255 bytes.
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	
	//Envia el payload:
	for (i=0;i<numDatos;i++){
        bcm2835_spi_transfer(payload[i]);
        bcm2835_delayMicroseconds(TIEMPO_SPI);
    }
		
	//Envia el delimitador de fin de trama:
	bcm2835_spi_transfer(0xF0);	
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	
	//Imprime la solicitud:
	printf("\n>Solicitud de medicion enviada");
	
	//Enciende el LEDTEST:
	digitalWrite (LEDTEST, HIGH);
	
	Salir();
	
}

//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Procesos locales:

void Salir(){
	
	bcm2835_spi_end();
	bcm2835_close();
	printf("\nAdios...\n");
	exit (-1);
}


//**************************************************************************************************************************************





