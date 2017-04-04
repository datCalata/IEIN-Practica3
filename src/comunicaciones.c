#include "comunicaciones.h"
#include <stdio.h>
#include <stdlib.h>
#define MY_NODE 0x10

int checkMsg(msg_rcv* msg);
int aniadeAula(aula* miAula);
int borraAula(int id_aula);
int muestraAula(int id_aula);
int muestraTodo(void);
boolean compruebaAula(aula* miAula);
aula* getAula(int id_aula);
int hazMsg(msg_rcv* msg);
int checkAniadeAula(msg_rcv* msg);
int checkAulaID(msg_rcv* msg);
void imprimirAula(aula* miAula);

int procesaMsg(msg_rcv* msg){
	int check = checkMsg(msg);
	if(check == 0 || check == 20)
		return check;
	return hazMsg(msg);
}

int checkMsg(msg_rcv* msg){
	//Check fin_cmd
	if(msg->fin_cmd != 0x0F)
		return 0;
	//checkNode
	if(msg->node_ID != MY_NODE)
		return 20;
	return 1;
}

int hazMsg(msg_rcv* msg){
	switch(msg->cmd){
	case 'a':
		if(checkAniadeAula(msg) == 1){
			msg->dataAula->aula_id = msg->aula_id;
			return aniadeAula(msg->dataAula);
		}
		return 0;
	case 'd':
		if(checkAulaID(msg) == 1)
			return borraAula(msg->aula_id);
		return 0;
	case 'c':
		if(checkAulaID(msg) == 1)
			return muestraAula(msg->aula_id);
		return 0;
	case 'm':
		return muestraTodo();
	default:
		printf("Mensaje incorrecto");
		printf("El CMD Recivido no es valido");
		return 0;
	}
}

int checkAniadeAula(msg_rcv* msg){
	if(msg->aula_id < 0){
		printf("Error de ID negativo \n");
		return 0;
	}
	if(msg->dataAula == NULL){
		printf("Error dataAula es NULL \n");
		return 0;
	}
	return 1;
}

int checkAulaID(msg_rcv* msg){
	if(msg->aula_id < 0){
		printf("Error de ID negativo \n");
		return 0;
	}
	return 1;
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
int muestraAula(int id_aula){
	//Comprovamos que el aula está registrada y obtenermos un puntero
	aula* aulaMostrar = getAula(id_aula);
	if(aulaMostrar != NULL){
		imprimirAula(aulaMostrar);
		return 23;
	}
	return -1;
}
int borraAula(int id_aula){
	//Comprovamos que el aula está registrada y obtenermos un puntero
	aula* listaAulas = NULL;
	aula* aulaBorrar = NULL;
	int num = 0;
	//Hacemos una estructura paralela en la que almacenamos lo que no borramos
	for(int i = 0; i < nAulas; i++){
		if(id_aula != datosAulas[i].aula_id){
			//NOTA: HACEMOS UN ALLOC DIRECTAMENTE PORQUE INICIALIZAMOS EL PUNT

			listaAulas = realloc(listaAulas,(num+1)*sizeof(aula));
			listaAulas[num].aula_id = datosAulas[i].aula_id;
			listaAulas[num].dni = datosAulas[i].dni;
			listaAulas[num].luminosidad = datosAulas[i].luminosidad;
			listaAulas[num].presencia = datosAulas[i].presencia;
			listaAulas[num].temperatura = datosAulas[i].temperatura;
			num++;
		}else{
			aulaBorrar = datosAulas+i;
		}
	}
	//Verificamos existe un valor que coincide con el parametro
	if(num == (nAulas-1)){
		free(datosAulas);
		datosAulas = listaAulas;
		free(aulaBorrar);
		nAulas = num;
		return 22;
	}else{
		printf("Error no se ha podido borrar\n");
		return -1;
	}
}
int muestraTodo(void){
	aula* miAula = NULL;
	if(nAulas == 0){
		printf("No hay datos que imprimir \n");
		return -1;
	}
	for(int i = 0; i < nAulas; i++){
		miAula = datosAulas+i;
		imprimirAula(miAula);
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
aula* getAula(int id_aula){
	if(nAulas == 0 || datosAulas == NULL)
		return NULL;
	aula* aulaDevolver = NULL;
	for(int i = 0; i < nAulas; i++){
		if(datosAulas[i].aula_id == id_aula)
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
