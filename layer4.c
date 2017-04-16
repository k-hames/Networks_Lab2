/*
 * layer4.c
 *
 *  Created on: Feb 15, 2017
 *      Author: Kevin Hames
 */

#include "nw_lab1.h"

buffer layer4(buffer t_buf, char mode, int debug)
{
	// Data Buffer & file Pointers
	buffer b;
	b.data = (char *) malloc(MAXSIZE);
	FILE *fp;

	if(mode == 't')
	{
		// Debug: Layer 4 started
		if(debug == 1)
			printf("\nPre Layer 4 Transmit Function Test\n");

		// Opens sendfile for transmission and reads into a buffer
		fp = fopen("sendfile.txt","r");

		fread(b.data,MAXSIZE,1,fp);

		// Calculates Buffer Size
		fseek(fp,0L,SEEK_END);
		b.B_count = ftell(fp);
		rewind(fp);


		fclose(fp);

		// Debug: Prints output of layer 4
		if(debug == 1)
		{
			printf("\nPost Layer 4 Transmit Function Test\n");
			for(int i=0; i<b.B_count-1; i++)
			{
				printf("%c", *(b.data+i));
			}
			printf("\n");
		}

		return b;
	}
	else if(mode=='r')
	{
		// Debug: Prints input of layer 4
		if(debug == 1)
		{
			printf("\nPre Layer 4 Receive Function Test\n");
					for(int i=0; i<t_buf.B_count-1; i++)
					{
						printf("%c", *(t_buf.data+i));
					}
					printf("\n");
		}

		// opens recvfile and writes the contents of the buffer
		fp = fopen("recvfile.txt", "w");
		fwrite(t_buf.data,1,t_buf.B_count-1,fp);
		fclose(fp);

		// Debug: Layer 4 completed
		if(debug == 1)
			printf("\nPost Layer 4 Receive Function Test (check recfile.txt)\n");

		return b;
	}
	else
	{
		buffer temp;
		return temp;
	}
}


