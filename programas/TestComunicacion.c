
//Compilar:
//gcc TestSPI.c -o testspi -lbcm2835 -lwiringPi 
//gcc TestSPI.c -o /home/rsa/Ejecutables/testspi -lbcm2835 -lwiringPi

/*-------------------------------------------------------------------------------------------------------------------------
Autor: Milton Munoz
Fecha de creacion: 21/08/03
Observaciones:
Funcion 1: Inicio de medicion.
Funcion 2: Lectura de datos.
Funcion 3: Escritura de datos.
Funcion 4: Test comunicacion
        Subfuncion 1: Test SPI (sumRecibido=1645)
        Subfuncion 2: Test RS485 (sumRecibido=1805)
-------------------------------------------------------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <bcm2835.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


//Declaracion de constantes
#define IDConcentrador 1
#define P0 0																	//Pin 11 GPIO
#define MCLR 28																	//Pin 38 GPIO
#define LEDTEST 29 																//Pin 40 GPIO																						
#define TIEMPO_SPI 100
#define FreqSPI 200000

//Definicion del color de los mensajes
#define RED   "\x1B[31m"
#define ORANGE  "\033[33m"
#define GREEN "\033[32m"
#define RESET "\x1B[0m"

//Declaracion de variables
unsigned int i, x;
unsigned char bufferSPI;

unsigned char idPet;
unsigned char funcionPet;
unsigned char subFuncionPet;
unsigned int numDatosPet;

unsigned char idResp;
unsigned char funcionResp;
unsigned char subFuncionResp;
unsigned int numDatosResp;
unsigned char *ptrNumDatosResp; //OJO: Funciono cambiando short por char

unsigned char payloadPet[20];
unsigned char payloadResp[2600];

unsigned int sumEnviado;
unsigned int sumRecibido;

char menuNodo;

FILE *fp;


//Declaracion de funciones
int ConfiguracionPrincipal();													
void EnviarSolicitud(unsigned char id, unsigned char funcion, unsigned char subFuncion, unsigned char numDatos, unsigned char* payload);
void RecibirCabeceraRespuesta();
void RecibirPayloadNodos(unsigned int numBytesPyload, unsigned char* pyloadRS485);
void RecibirPayloadConcentrador(unsigned int numBytesPyload, unsigned char* pyloadConcentrador);
void TestComunicacionSPI(unsigned char* pyloadRS485);	
void CrearArchivo(unsigned short idConc, unsigned short idNodo);
void GuardarTrama(unsigned char* tramaRS485, unsigned int longitudTrama); 										
void ImprimirInformacion();
void Salir();						

double elapsed_time;

//**************************************************************************************************************************************
//************************************************************** Principal *************************************************************
//**************************************************************************************************************************************
int main(int argc, char *argv[]) {
  
	//Inicializa las variables:
	i = 0;
	x = 0;
	
	idPet = 0;
	funcionPet = 0;
	subFuncionPet = 0;
	numDatosPet = 0;
	idResp = 0;
	funcionPet = 0;
	subFuncionResp = 0;
	numDatosResp = 0;
	ptrNumDatosResp = (unsigned char *) & numDatosResp;
	
	//Verifica si la peticion va dirigida al concentrador o a un nodo:
	idPet = (char)(atoi(argv[1]));
	if (idPet==0){
		funcionPet = 4;
		subFuncionPet = 1;
		numDatosPet = 5;
		payloadPet[0] = 1;
		payloadPet[1] = 2;
		payloadPet[2] = 3;
		payloadPet[3] = 4;
		payloadPet[4] = 5;
	} else {
		funcionPet = 4;
		subFuncionPet = 2;
		numDatosPet = 5;
		payloadPet[0] = 1;
		payloadPet[1] = 2;
		payloadPet[2] = 3;
		payloadPet[3] = 4;
		payloadPet[4] = 5;
	}
	
		
	//Configuracion principal:
	ConfiguracionPrincipal();
			
	sumEnviado = 0;
	sumRecibido = 0;
	
	elapsed_time = 0;

	clock_t start_time = clock();
	EnviarSolicitud(idPet, funcionPet, subFuncionPet, numDatosPet, payloadPet);
	
	while(1){
		clock_t end_time = clock();
		elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		if (elapsed_time >= 5.0) {
			printf("\n>Timeout: " RED "Nodo %d no responde\n" RESET, idPet);
			Salir();
		}
	}
	return 0;
}
//**************************************************************************************************************************************


//**************************************************************************************************************************************
//************************************************************** Funciones *************************************************************
//**************************************************************************************************************************************

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
    pinMode(P0, INPUT);
	pinMode(MCLR, OUTPUT);
	pinMode(LEDTEST, OUTPUT);
	wiringPiISR (P0, INT_EDGE_RISING, RecibirCabeceraRespuesta);
	
	//Enciende el pin LEDTEST:
	digitalWrite (LEDTEST, HIGH);
		
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Funcion para imprimir en pantalla los datos relevantes
void ImprimirInformacion(){
	
	//Imprime la solicitud:
	printf("\nTrama enviada:");
	printf("\n Cabecera: %d %d %d %d", idPet, funcionPet, subFuncionPet, numDatosPet);
	printf("\n Payload: ");
	for (i=0;i<numDatosPet;i++){
        printf("%#02X ", payloadPet[i]);
    }
	printf("\n Sumatoria control = " GREEN "%d OK" RESET, sumEnviado);
	
	//Imprime la respuesta:
	printf("\n\nTrama recibida:");
	printf("\n Cabecera: %d %d %d %d", idResp, funcionResp, subFuncionResp, numDatosResp);
	printf("\n Payload: ");
	for (i=0;i<numDatosResp;i++){
		printf("%#02X ", payloadResp[i]);
	}
		
	//Comprueba la sumatoria de control en las solicitudes de testeo de comunicacion:
	if (funcionPet==4){
				
		//Test comunicacion SPI
		if (subFuncionPet==1){
			if (sumRecibido==1645){
			printf("\n Sumatoria control = " GREEN "%d OK" RESET, sumRecibido);
			}else{
				printf("\n Sumatoria control = " ORANGE "%d Fail" RESET, sumRecibido);
			}
		}
		//Test comunicacion RS485:
		if (subFuncionPet==2){
			//0XB*9=99dec
			if (sumRecibido==(99+idPet)){
			printf("\n Sumatoria control = " GREEN "%d OK" RESET, sumRecibido);
			}else{
				printf("\n Sumatoria control = " ORANGE "%d Fail" RESET, sumRecibido);
			}		
		}
	}
		
	Salir();
	
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Funcion para enviar la solicitud (C:0xA0 F:0xF0):
void EnviarSolicitud(unsigned char id, unsigned char funcion, unsigned char subFuncion, unsigned char numDatos, unsigned char* payload){
	
	//sumatoria de control:
	for (i=0;i<numDatos;i++){
        sumEnviado = sumEnviado + payload[i];
    }
	
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
	printf("\n>Solicitud enviada");
	
	//Enciende el LEDTEST:
	digitalWrite (LEDTEST, HIGH);
	
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Funcion para resibir la cabecera de respuesta C:0xA1 F:0xF1
void RecibirCabeceraRespuesta(){
	
	bcm2835_delayMicroseconds(200);
	
	//Recupera la cabecera: [id, funcion, subfuncion, lsbNumDatos, msbNumDatos]:
	bcm2835_spi_transfer(0xA1);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	idResp = bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	funcionResp = bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	subFuncionResp = bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	*(ptrNumDatosResp) = bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	*(ptrNumDatosResp+1) = bcm2835_spi_transfer(0x00);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	bcm2835_spi_transfer(0xF1);	

	delay(25); //**Este retardo es muy importante**
	
	//Se recupera el payload de la respuesta:
	if (idResp==0){
		//Recupera el payload para tetear la comunicacion SPI con el concentrador:
		//TestComunicacionSPI(payloadResp); 
		RecibirPayloadConcentrador(numDatosResp, payloadResp);	
		//Por ahora solo hay una opcion disponible en el concentrador. Si se desea agregar mas se puede agregar un switch case o un if.
	} else {
		//Recupera el payload enviado por los nodos.
		RecibirPayloadNodos(numDatosResp, payloadResp);	
		//CrearArchivo(IDConcentrador, idResp);
		//GuardarTrama(payloadResp, numDatosResp);
	}
	
	//sumatoria de control. Disponible solo para las operaciones de testeo de la comunicacion SPI y RS485:
	if (funcionResp==4){
		for (i=0;i<numDatosResp;i++){
			sumRecibido = sumRecibido + payloadResp[i];
		}
	}
	
	//Imprime mensaje de respuesta:
	printf("\n>Respuesta recibida\n");
			
	//Apaga el LEDTEST:
	digitalWrite (LEDTEST, LOW);
	
	//Imprime la informacion de solicitud y respuesta:
	ImprimirInformacion();
	
	Salir();
		
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Funcion para recibir el pyload de respuesta de los nodos (C:0xA2 F:0xF2):
void RecibirPayloadNodos(unsigned int numBytesPyload, unsigned char* pyloadRS485){
	
	//printf("\nRecuperando pyload...\n");
	bcm2835_spi_transfer(0xA2);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	for (i=0;i<numBytesPyload;i++){
        bufferSPI = bcm2835_spi_transfer(0x00);
        pyloadRS485[i] = bufferSPI;
        bcm2835_delayMicroseconds(TIEMPO_SPI);
    }
	bcm2835_spi_transfer(0xF2);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	
}	
//**************************************************************************************************************************************

//**************************************************************************************************************************************
//Funcion para testear la comunicacion SPI con el concentrador (C:0xA3 F:0xF3):
void RecibirPayloadConcentrador(unsigned int numBytesPyload, unsigned char* pyloadConcentrador){
	
	//printf("\nRecuperando pyload...\n");
	bcm2835_spi_transfer(0xA3);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	for (i=0;i<numBytesPyload;i++){
        bufferSPI = bcm2835_spi_transfer(0x00);
        pyloadConcentrador[i] = bufferSPI;
        bcm2835_delayMicroseconds(TIEMPO_SPI);
    }
	bcm2835_spi_transfer(0xF3);
	bcm2835_delayMicroseconds(TIEMPO_SPI);
	
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
void CrearArchivo(unsigned short idConc, unsigned short idNodo){

	char nombreArchivo[50];
	char idArchivo[8];
	char ext[5];
		
	
	//Realiza la concatenacion para obtner el nombre del archivo:			
	strcpy(nombreArchivo, "/home/rsa/Resultados/");
	sprintf(idArchivo, "C%0.2dN%0.2d_", idConc, idNodo); 
	strcpy(ext, ".dat");
	strcat(nombreArchivo, idArchivo);
	strcat(nombreArchivo, ext); 
	
	//Crea el archivo binario:
	printf("Se ha creado el archivo: %s\n", nombreArchivo);
	fp = fopen (nombreArchivo, "wb+");
	
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
void GuardarTrama(unsigned char* tramaRS485, unsigned int longitudTrama){
		
	unsigned int outFwrite;
					
	//Guarda la trama en el archivo binario:
	if (fp!=NULL){
		do{
		outFwrite = fwrite(tramaRS485, sizeof(char), longitudTrama, fp);
		} while (outFwrite!=longitudTrama);
		fflush(fp);
	} else {
		printf("No se pudo escribir el archivo binario\n");
	}
		
}
//**************************************************************************************************************************************

//**************************************************************************************************************************************
void Salir(){
	bcm2835_spi_end();
	bcm2835_close();
	printf("\n\nAdios...\n");
	exit (-1);
}
//**************************************************************************************************************************************




