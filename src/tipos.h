/*
 * tipos.h
 *
 *  Created on: 15/3/2017
 *      Author: Elena
 */

#ifndef TIPOS_H_
#define TIPOS_H_


typedef struct aula_def aula;
typedef struct msg_def msg_rcv;

struct msg_def{ //definicion de un mensaje
	char node_ID;
	int aula_id;
	char cmd;
	aula* dataAula;
	char fin_cmd;
};

struct aula_def{ //definicion de un aula
	int aula_id;
	int temperatura;
	int luminosidad;
	char presencia;
	char* dni;
};

int nAulas;
aula* datosAulas;

#endif /* TIPOS_H_ */
