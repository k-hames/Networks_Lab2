/*****************************************************************************
 * YOUR NAME HERE							     *
 * main.c								     *
 * Copyright (C) Your Name 2017 <YourEmail@villanova.edu>		     *
 * 									     *
 * This program generates a single executable for sender-receiver	     *
 * communication. 							     *
 *									     *
 *****************************************************************************/
/*****************************************************************************
 * COMPILATION INSTRUCTIONS (COMMAND LINE)						     *
 * =======================================				     *
 *									     *
 * To compile each .c src file (in gcc) without linking ..		     *
 *	In Linux/Unix: gcc  -c  file.c  -o  file.o			     *
 *	In Windows   : gcc  -c  file.c  -o  file.o			     *
 *									     *
 * To link all .o object files (in gcc) and create an executable	     *
 *	In Linux/Unix: gcc  file1.o  file2.o .. filen.o  -o  a.out	     *
 *	In Windows   : gcc  file1.o  file2.o .. filen.o  -lwsock32 -lws2_32  *
 *								-o  a.exe    *
 *****************************************************************************/

#include <string.h>
#include <stdio.h>
#include "nw_lab1.h"


int main(int argc, char *argv[])
{	
	/* Check to see if the program was invoked with command-line correct parameters */
	if (argc != 2) {
		printf("\nERROR: Missing or incorrect number of program parameters\n");
		printf("USAGE: <program name> MODE\n");
		printf("where, MODE = 's' (Sender) or 'r' (Receiver)\n");
		return (1);
	}

	char mode = argv[1][0];	/* First character in argument MUST be 's' or 'r' */
	int retcode = 0;

	switch (mode) {
		
		case 's':
		case 'S': /* We are sending data from Application!    */
			  /* So, call your transmit() function here   */
			  retcode = transmit();
			  if(retcode != 0) // error transmitting
				  return -1;
			  break;

		case 'r':
		case 'R': /* We are receiving Data from Application! */
			  /* So, call your receive() function here   */
			  retcode = receive();
			  if(retcode != 0) // error receiving
				  return -1;
			  break;
	}


	return 0;

}




