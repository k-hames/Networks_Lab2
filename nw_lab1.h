/*
 * nw_lab1.h
 *
 *  Created on: Feb 15, 2017
 *      Author: Kevin Hames
 */

#ifndef NW_LAB1_H_
#define NW_LAB1_H_

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 2048
#define MSS 60

typedef struct
{
	char* data;
	int B_count;
} buffer;

buffer layer4(buffer t_buf, char mode, int debug);
buffer layer3(buffer buffer, char mode, int debug);
buffer layer2(buffer t_buffer, char mode, int debug);
unsigned short cksum(unsigned short *buf, int nbytes);
int transmit ();
int receive();

#endif /* NW_LAB1_H_ */
