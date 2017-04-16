/*
 * layer3.c
 *
 *  Created on: Feb 15, 2017
 *      Author: Kevin Hames
 */


#include "nw_lab1.h"

buffer layer3(buffer buf, char mode, int debug)
{
	int size = 4800; // 120 bytes per frame 40 frames
	int c = 0; // count to track location of data buffer
	buffer t;
	t.data = (char *) malloc(size);
	t.B_count = buf.B_count;

	if(mode == 't')
	{
		// Debug: Prints Input of layer 3
		if(debug == 1)
		{
			printf("\nPre Layer 3 Transmit Function Test\n");
			for(int i = 0; i < 100*(buf.B_count / MSS) + (buf.B_count % MSS + 40 - 1); i++)
			{
				printf("%c", *(buf.data + i));
			}
			// printf("\n");
		}


		// Sets hdr (40 bytes) to 0 and data (60 bytes) to data in buffer
		for(int i =0; i < 120*(buf.B_count / 60 + 1); i++)
		{
			if(i%100>=40)
			{
				*(t.data+i) = *(buf.data + c);
				c++;
			}
			else
			{
				*(t.data+i) = '0';
			}
		}

		// Debug: Prints output for layer 3
		if(debug == 1)
		{
			printf("\nPost Layer 3 Transmit Function Test\n");
			for(int i=0; i<buf.B_count / MSS; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<100; j++)
				{
					printf("%c", *(t.data +j + i*100));
				}
				printf("\n");
			}
			printf("Frame %i: ", t.B_count / MSS);
			for(int k=0; k < t.B_count % MSS + 40 - 1; k++)
			{
				printf("%c", *(t.data + k + (t.B_count / MSS)*100));
			}
			printf("\n");
		}


		return t;
	}
	else if (mode == 'r')
	{
		// Debug: Prints input for layer 3
		if(debug == 1)
		{
			printf("\nPre Layer 3 Receive Function Test\n");
			for(int i=0; i<buf.B_count / MSS; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<100; j++)
				{
					printf("%c", *(buf.data +j + i*100));
				}
				printf("\n");
			}
			printf("Frame %i: ", buf.B_count / MSS);
			for(int k=0; k < buf.B_count % MSS + 40 - 1; k++)
			{
				printf("%c", *(buf.data + k + (buf.B_count / MSS)*100));
			}
			printf("\n");
		}

		int rsize = 100*(buf.B_count / MSS) + (buf.B_count % MSS + 40 - 1); // total byte count of frames
		// strips layer 3 hdr
		for(int i=0; i < rsize; i++)
		{
			if(i%100>=40)
			{
				*(t.data + c) = *(buf.data + i);
				c++;
			}
		}

		// Debug: Prints output of layer 3
		if(debug == 1)
		{
			printf("\nPost Layer 3 Receive Function Test\n");
			for(int i = 0; i < rsize; i++)
			{
				printf("%c", *(t.data + i));
			}
			printf("\n");
		}

		return t;
	}
	else
		return buf;
}
