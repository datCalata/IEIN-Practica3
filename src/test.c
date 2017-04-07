/*
 * test.c
 *
 *  Created on: 14/3/2017
 *      Author: Elena
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunicaciones.h"
#include "tipos.h"

msg_rcv* generadorMsg(int aula_id, char cmd, int aula);
aula* generadorAula();

int main (){
	int result;
	nAulas = 0;
	srand(time(NULL));

	aula* aula1 = (aula*)malloc(sizeof(aula));
	aula1->temperatura = 22;
	aula1->luminosidad = 10;
	aula1->presencia = 's';
	aula1->dni = "50555444K 50444555S 50334455E";

	msg_rcv* mssg1 = (msg_rcv*)malloc(sizeof(msg_rcv));
	mssg1->node_ID = 0x10;
	mssg1->aula_id = -1;
	mssg1->cmd = 'a';
	mssg1->dataAula = aula1;
	mssg1->fin_cmd = 0x0F;

	result = procesaMsg(mssg1);
	printf("procesaMsg devuelve %d\n", result);

	mssg1->cmd = 'm';
	result = procesaMsg(mssg1);
	printf("procesaMsg devuelve %d\n", result);

	free(aula1);
	free(mssg1);

	msg_rcv* msga1 = generadorMsg(0,'a',1);

	result = procesaMsg(msga1);
	printf("procesaMsg devuelve %d(deberia ser 24)\n", result);

	free(msga1);

	msg_rcv* msg0 = generadorMsg(0,'a',1);
	msg_rcv* msg1 = generadorMsg(1,'a',1);
	msg_rcv* msg2 = generadorMsg(2,'a',1);
	msg_rcv* msg3 = generadorMsg(3,'a',1);
	msg_rcv* msg32 = generadorMsg(3,'a',1);
	msg_rcv* msg3e = generadorMsg(3,'a',0);
	msg_rcv* msgd = generadorMsg(2,'d',0);
	msg_rcv* msgd2 = generadorMsg(2,'d',0);
	msg_rcv* msgd3 = generadorMsg(1,'d',0);
	msg_rcv* msgce = generadorMsg(2,'c',0);
	msg_rcv* msgc = generadorMsg(0,'c',0);
	msg_rcv* msgm = generadorMsg(2,'m',0);

	result = procesaMsg(msg0);
	printf("procesaMsg devuelve %d(deberia ser 21)\n", result);

	result = procesaMsg(msg1);
	printf("procesaMsg1 devuelve %d(deberia ser 21)\n", result);

	result = procesaMsg(msg2);
	printf("procesaMsg2 devuelve %d(deberia ser 21)\n", result);

	result = procesaMsg(msg3);
	printf("procesaMsg3 devuelve %d(deberia ser 21)\n", result);

	result = procesaMsg(msg32);
	printf("procesaMsg32 devuelve %d(deberia ser -1)\n", result);

	result = procesaMsg(msg3e);
	printf("procesaMsg3e devuelve %d(deberia ser 0)\n", result);

	result = procesaMsg(msgd);
	printf("procesaMsgd devuelve %d(deberia ser 22)\n", result);

	result = procesaMsg(msgd2);
	printf("procesaMsgd2 devuelve %d(deberia ser -1)\n", result);

	result = procesaMsg(msgd3);
	printf("procesaMsgd3 devuelve %d(deberia ser 22)\n", result);

	result = procesaMsg(msgce);
	printf("procesaMsgc devuelve %d(deberia ser -1)\n", result);

	result = procesaMsg(msgc);
	printf("procesaMsgc devuelve %d(deberia ser 23)\n", result);

	result = procesaMsg(msgm);
	printf("procesaMsgm devuelve %d(deberia ser 24)\n", result);

	//free(msg0->aula_id);
	free(msg0);
	//free(msg1->aula_id);
	free(msg1);
	//free(msg2->aula_id);
	free(msg2);
	//free(msg3->aula_id);
	free(msg3);
	//free(msg32->aula_id);
	free(msg32);
	//free(msg3e->aula_id);
	free(msg3e);
	free(msgd);
	free(msgd2);
	//free(msgd->aula_id);
	free(msgce);
	free(msgc);
	//free(msgm->aula_id);
	free(msgm);
	return 0;
}

msg_rcv* generadorMsg(int aula_id, char cmd, int aula){
	msg_rcv* msg = (msg_rcv*)malloc(sizeof(msg_rcv));
	msg->node_ID = 0x10;
	msg->aula_id = aula_id;
	msg->cmd = cmd;
	if(aula == 1)
		msg->dataAula = generadorAula();
	else
		msg->dataAula = NULL;
	msg->fin_cmd = 0x0F;
	return msg;
}
aula* generadorAula(){
	aula* miAula = (aula*)malloc(sizeof(aula));
	miAula->temperatura = rand()%55;
	miAula->luminosidad = rand()%100;
	if((rand()%2) == 1)
		miAula->presencia = 's';
	else
		miAula->presencia = 'n';
	miAula->dni = "50555444K 50444555S 50334455E";
	return miAula;
}


