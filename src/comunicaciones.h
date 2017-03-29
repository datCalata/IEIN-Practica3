/*
 * comunicaciones.h
 *
 *  Created on: 7 de mar. de 2017
 *      Author: jcala
 */

#ifndef COMUNICACION_H_
#define COMUNICACION_H_
#include "tipos.h"

typedef enum{true, false} boolean;


int recPacket(char msg[]);


int procesaMsg(msg_rcv* msg);

#endif /* COMUNICACION_H_ */
