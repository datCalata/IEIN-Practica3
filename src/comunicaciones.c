#include "comunicaciones.h"
#include <stdio.h>
#include <stdlib.h>

boolean integridadPaquete(char msg[],int *devolver);
boolean checkNode(char nodeMessage);
int checkCmd(char cmd);
int longitudArray(char msg[]);
int checkMsg(msg_rcv* msg);
int aniadeAula(aula* miAula);
int borraAula(aula* miAula);
int muestraAula(aula* miAula);
int muestraTodo(void);

boolean compruebaAula(aula* miAula);
	aula* getAula(aula* miAula);


int hazMsg(aula* miAula,char cmd);


#define MY_NODE 0x10

int recPacket(char msg[]) {
	int devolver = 0;
	if (integridadPaquete(msg, &devolver) == false) {
		printf("Paquete Erroneo");
		return 0;
	}
	if(checkNode(msg[0]) == false){
		printf("No es para mi, pero es valido");
		return 20;
	}
	return devolver;
}

boolean integridadPaquete(char msg[], int *devolver) {
	//Probamos a calcular la longitud del array
	int longArray = longitudArray(msg);
	if (longArray != -1){
		//Comprovamos que la info en cmd tenga sentido
		*devolver = checkCmd(msg[1]);
		if (*devolver != -1)
			return true;
	}
	return false;
}

int longitudArray(char msg[]) {
	int l = 0;
	while (1) {
		if (msg[l] == 0x0F && l >= 3 && l < 14) {
			printf("Paquete Bueno, con final y longitud aceptable( %d )", l+1);
			return l;
		}
		if (msg[l] == 0x0F && l < 3) {
			printf("Paquete Erroneo, demasiado corto para ser bueno");
			return -1;
		}
		if (l >= 14){
			printf("Paquete Erroneo, demasiado largo para ser bueno");
			return -1;
		}
		l++;
	}
}

boolean checkNode(char nodeMessage) {
	if (nodeMessage == MY_NODE)
		return true;
	return false;
}

int checkCmd(char cmd) {
	switch (cmd) {
	case 'a':
		return 11;
	case 'd':
		return 12;
	case 'c':
		return 13;
	case 'm':
		return 14;
	default:
		printf("CMD Incorrecto");
		return -1;
	}
}

int procesaMsg(msg_rcv* msg){
	int check = checkMsg(msg);
	if(check == 0 || check == 20)
		return check;
	return hazMsg(msg->dataAula,msg->cmd);
}

int checkMsg(msg_rcv* msg){
	//Check Aula_ID correcto
	/*if(msg->aula_id != msg->dataAula->aula_id)
		return 0;*/
	//Check cmd
	if(checkCmd(msg->cmd) == -1)
		return 0;
	//check aula
	if((msg->dataAula) == NULL)
		return 0;
	//checkNode
	if(msg->node_ID != MY_NODE)
		return 20;
	return 1;
}

int hazMsg(aula* miAula,char cmd){
	switch(cmd){
	case 'a':
		return aniadeAula(miAula);
	case 'd':
		return borraAula(miAula);
	case 'c':
		return muestraAula(miAula);
	case 'm':
		return muestraTodo();
	default:
		printf("Mensaje incorrecto");
		printf("NO DEBERIA ESTAR AQUI");
		return 0;
	}
}

int aniadeAula(aula* miAula){
	//Comprovamos que cumple las condiciones de no repeticion
	if(compruebaAula(miAula) == false){
		return -1;
	}
	//Aula Correcta
	//Intentamos reservar memoria
	//NOTA: COMO NO ESTA INICIALIZADO DATOSAULAS ES PELIGROSO HACER UN REALLOC
	//DIRECTAMENTE POR ESO USAMOS UN MALLOC AUNQUE PUEDE NO SER NECESARIO
	if(nAulas == 0){
		datosAulas = (aula*)malloc(sizeof(aula));
	}else{
		datosAulas = (aula*)realloc(datosAulas,(nAulas+1)*sizeof(aula));
	}

	//Comprovamos que se ha reservado correctamente
	if(datosAulas == NULL){
		printf("No hay suficiente memoria!");
		return -1;
	}
	//Allocamos en memoria
	datosAulas[nAulas].aula_id = miAula->aula_id;
	datosAulas[nAulas].dni = miAula->dni;
	datosAulas[nAulas].luminosidad = miAula->luminosidad;
	datosAulas[nAulas].presencia = miAula->presencia;
	datosAulas[nAulas].temperatura = miAula->temperatura;
	//Contamos el aula en el contador
	nAulas++;
	return 21;
}
int muestraAula(aula* miAula){
	//Comprovamos que el aula está registrada y obtenermos un puntero
	aula* aulaMostrar = getAula(miAula);
	if(aulaMostrar != NULL){
		imprimirAula(aulaMostrar);
		return 23;
	}
	return -1;
}
int borraAula(aula* miAula){
	//Comprovamos que el aula está registrada y obtenermos un puntero
	aula* listaAulas = NULL;
	aula* aulaBorrar = NULL;
	int num = 0;
	//Hacemos una estructura paralela en la que almacenamos lo que no borramos
	for(int i = 0; i < nAulas; i++){
		if(miAula->aula_id != datosAulas[i].aula_id){
			//NOTA: HACEMOS UN ALLOC DIRECTAMENTE PORQUE INICIALIZAMOS EL PUNT
			num++;
			listaAulas = realloc(listaAulas,num*sizeof(aula));
		}else{
			aulaBorrar = datosAulas+i;
		}
	}
	//Verificamos existe un valor que coincide con el parametro
	if(!aulaBorrar && num == (nAulas-1)){
		datosAulas = listaAulas;
		free(aulaBorrar);
		nAulas = num;
		return 22;
	}else{
		printf("Error no se ha podido borrar");
		return -1;
	}
}
int muestraTodo(void){
	if(nAulas == 0){
		printf("No hay datos que imprimir");
		return -1;
	}
	for(int i = 0; i < nAulas; i++){
		imprimirAula(datosAulas[i]);
	}
	return 24;
}

void imprimirAula(aula* miAula){
	printf("Aula id:\t%d\n",miAula->aula_id);
	printf("\ttemperatura:\t%d\n",miAula->temperatura);
	printf("\tluminosidad:\t%d\n",miAula->luminosidad);
	printf("\tpresencia:\t%c\n",miAula->presencia);
	printf("\tdni:\t%s\n",miAula->dni);
	return;
}
aula* getAula(aula* miAula){
	if(nAulas == 0 || datosAulas == NULL)
		return NULL;
	aula* aulaDevolver = NULL;
	for(int i = 0; i < nAulas; i++){
		if(datosAulas[i].aula_id == miAula->aula_id)
			aulaDevolver = datosAulas+i;
			return aulaDevolver;
	}
	return NULL;
}

boolean compruebaAula(aula* miAula){
	if(nAulas == 0 || datosAulas == NULL)
		return true;
	for(int i = 0; i < nAulas; i++){
		if(datosAulas[i].aula_id == miAula->aula_id)
			return false;
	}
	return true;
}
