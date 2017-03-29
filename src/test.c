/*
 * test.c
 *
 *  Created on: 14/3/2017
 *      Author: Elena
 */


#include <stdio.h>
#include <stdlib.h>
#include "comunicaciones.h"
#include "tipos.h"


int main (){
	int result;
	nAulas = 0;

	aula* aula1 = (aula*)malloc(sizeof(aula));
	aula1->temperatura = 22;
	aula1->luminosidad = 10;
	aula1->presencia = 's';
	aula1->dni = "50555444K 50444555S 50334455E";

	msg_rcv* mssg1 = (msg_rcv*)malloc(sizeof(msg_rcv));
	mssg1->node_ID = 0x10;
	mssg1->aula_id = 3;
	mssg1->cmd = 'a';
	mssg1->dataAula = aula1;
	mssg1->fin_cmd = 0x0F;

	result = procesaMsg(mssg1);
	printf("procesaMsg devuelve %d\n", result);

	mssg1->cmd = 'm';
	result = procesaMsg(mssg1);
	printf("procesaMsg devuelve %d\n", result);

	free(aula1);
	free (mssg1);

	return 0;
}
