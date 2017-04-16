/*
 * layer2.c
 *
 *  Created on: Feb 19, 2017
 *      Author: Kevin Hames
 */

#include "nw_lab1.h"

buffer layer2(buffer t_buffer, char mode, int debug)
{
	buffer t_buffer_new;
	t_buffer_new.data = (char *) malloc(4800);
	t_buffer_new.B_count = t_buffer.B_count;
	int last_frame;
	int lf_count;
	if(t_buffer.B_count == MSS) // handles special case error when file size = MSS
	{
		last_frame= 0;
		lf_count = MSS;
	}
	else
	{
		last_frame= t_buffer.B_count / MSS;
		lf_count = t_buffer.B_count % MSS + 60 - 1; // data + hdr & trl bits = size of last frame
	}
	int frame_count = 0;
	int chk_sum = 0;
	unsigned short *buf = (unsigned short *) malloc(1800);
	int nbytes;

	// Transmit Mode
	if(mode == 't')
	{

	int stuf_count = 0; // Keeps track of number of stuffed bytes for indexing
	int lf_b_count = lf_count; // used to store Byte count as 3 chars

		// Debug: Prints incoming data from layer 3
		if(debug == 1)
		{
			printf("\nPre Layer 2 Transmit Function Test\n");
			for(int i=0; i<last_frame; i++)
			{
				printf("Frame %i: ", i);
				for(int j=0; j<100; j++)
				{
					printf("%c", *(t_buffer.data +j + i*100));
				}
				printf("\n");
			}
			printf("Frame %i: ", last_frame);
			for(int j=0; j<t_buffer_new.B_count % MSS + 40 -1; j++) // last frame (+40 for hdr)
			{
				printf("%c", *(t_buffer.data +j + last_frame*100));
			}
			printf("\n");
		}

		// Adds Layer 2 hdr and trlr to transmission buffer
		for(int i = 0; i<last_frame; i++)
		{
			for(int j=0; j<(120 + frame_count);j++)
			{
				if(j==0) // STX
				{
					*(t_buffer_new.data + i*120 + stuf_count) = (char) 2;
				}
				else if(j==1) // Byte Count
				{
					*(t_buffer_new.data + i*120 + 1 + stuf_count) = '1';
					*(t_buffer_new.data + i*120 + 2 + stuf_count) = '2';
					*(t_buffer_new.data + i*120 + 3 + stuf_count) = '0';
					j = j + 2;
				}
				else if(j == 4) // ACK
					*(t_buffer_new.data + i*120 + j + stuf_count) = '0';
				else if (j == 5) // Seq#
					*(t_buffer_new.data + i*120 + j + stuf_count) = (char) (i%10 + 48);
				else if (j == 6) // LFrame
					*(t_buffer_new.data + i*120 + j + stuf_count) = '0';
				else if (j < 17) // RESVD
				{
					*(t_buffer_new.data + i*120 + j + stuf_count) = '0';
				}
				else if (j < 57) // L3H
					*(t_buffer_new.data + i*120 + j + stuf_count) = *(t_buffer.data + i*100 + j-17);
				else if (j<(117 + frame_count)) // DATA
				{
					// Byte Stuffing condition
					if(*(t_buffer.data + i*100 + j - 17 - frame_count) == (char) 2 || *(t_buffer.data + i*100 + j - 17 - frame_count) == (char) 3 || *(t_buffer.data + i*100 + j - 17 - frame_count) == (char) 16)
					{
						*(t_buffer_new.data + i*120 + j + stuf_count) = (char) 16;
						*(t_buffer_new.data + i*120 + j + stuf_count + 1) = *(t_buffer.data + i*100 + j-17-frame_count);
						frame_count++;
						j++;
					}
					else
						*(t_buffer_new.data + i*120 + j + stuf_count) = *(t_buffer.data + i*100 + j-17-frame_count);
				}
				else if (j < 119 + frame_count) // ChkSum
				{
					// increments count field
					if(frame_count < 10)
					{
					for(int b = 0; b < frame_count; b++)
						*(t_buffer_new.data + i*120 + 3 + stuf_count) = (char) (*(t_buffer_new.data + i*120 + 3 + stuf_count)+1);
					}
					else
					{
						for(int b=0; b < frame_count / 10; b++)
							*(t_buffer_new.data + i*120 + 2 + stuf_count) = (char) (*(t_buffer_new.data + i*120 + 3 + stuf_count)+1);
						for(int b = 0; b < frame_count % 10; b++)
							*(t_buffer_new.data + i*120 + 3 + stuf_count) = (char) (*(t_buffer_new.data + i*120 + 3 + stuf_count)+1);
					}

					nbytes = 120 + frame_count - 4;
					// convert char buffer to unsigned short for chechksum calculation
					for(int k = 0; k < nbytes; k++)
					{
						*(buf + k) = (unsigned short) (*(t_buffer_new.data + i*120 + k + 1 + stuf_count)-48);
					}
					if(nbytes % 2 == 1) // ensures even number of bytes
					{
						*(buf + nbytes) = '\0';
						nbytes++;
					}
					chk_sum = cksum(buf, nbytes);
					*(t_buffer_new.data + i*120 + j + stuf_count) = (char) ((chk_sum % 100) / 10 + 48);
					*(t_buffer_new.data + i*120 + j + stuf_count + 1) = (char) (chk_sum % 10 + 48);
					j++;
				}
				else	// ETX
				{
					*(t_buffer_new.data + i*120 + j + stuf_count) = (char) 3;
				}
			}

			stuf_count += frame_count; // Allows for proper increment of pointer locations ie non static frame size
			frame_count = 0; // Resets frame size to default
		}



		for(int j=0; j<lf_count + frame_count;j++) // last frame special case
		{
		if(j==0) // STX
			*(t_buffer_new.data + last_frame*120 + stuf_count) = (char) 2;
		else if(j==1) // Byte Count (changed to fix bug)
		{
			*(t_buffer_new.data + last_frame*120 + 1 + stuf_count) = (char) (lf_b_count / 100 + 48);
			lf_b_count = lf_b_count % 100;
			*(t_buffer_new.data + last_frame*120 + 2 + stuf_count) = (char) (lf_b_count / 10  + 48);
			*(t_buffer_new.data + last_frame*120 + 3 + stuf_count) = (char) (lf_b_count % 10 + 48);
			j = j + 2;
		}
		else if(j == 4) // ACK
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = '0';
		else if (j == 5) // Seq#
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = (char) (last_frame%10 + 48);
		else if (j == 6) // LFrame
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = '1';
		else if (j < 17) // RESVD
		{
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = '0';
		}
		else if (j < 57) // L3H
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = *(t_buffer.data + last_frame*100 + j-17);
		else if (j< lf_count - 3 + frame_count) // DATA
			if(*(t_buffer.data + last_frame*100 + j - 17 - frame_count) == (char) 2 || *(t_buffer.data + last_frame*100 + j - 17 - frame_count) == (char) 3 || *(t_buffer.data + last_frame*100 + j - 17 - frame_count) == (char) 16)
			{
				*(t_buffer_new.data + last_frame*120 + j + frame_count + stuf_count) = (char) 16;
				*(t_buffer_new.data + last_frame*120 + j + frame_count + stuf_count + 1) = *(t_buffer.data + last_frame*100 + j-17-frame_count);
				frame_count++;
				j++;
			}
			else
				*(t_buffer_new.data + last_frame*120 + j + frame_count + stuf_count) = *(t_buffer.data + last_frame*100 + j-17-frame_count);
		else if (j < lf_count -1 + frame_count) // ChkSum
		{
			// increments count field
			if(frame_count < 10)
			{
			for(int b = 0; b < frame_count; b++)
				*(t_buffer_new.data + last_frame*120 + 3 + stuf_count) = (char) (*(t_buffer_new.data + last_frame*120 + 3 + stuf_count)+1);
			}
			else
			{
				for(int b=0; b < frame_count / 10; b++)
					*(t_buffer_new.data + last_frame*120 + 2 + stuf_count) = (char) (*(t_buffer_new.data + last_frame*120 + 3 + stuf_count)+1);
				for(int b = 0; b < frame_count % 10; b++)
					*(t_buffer_new.data + last_frame*120 + 3 + stuf_count) = (char) (*(t_buffer_new.data + last_frame*120 + 3 + stuf_count)+1);
			}

			// CHKSUM
			nbytes = lf_count + frame_count - 4;
			// convert char buffer to unsigned short for chechksum calculation
			for(int k = 0; k < nbytes; k++)
			{
				*(buf + k) = (unsigned short) (*(t_buffer_new.data + last_frame*120 + k + 1 + stuf_count)-48);
			}
			if(nbytes % 2 == 1) // ensures even number of bytes
			{
				*(buf + nbytes) = '\0';
				nbytes++;
			}
			chk_sum = cksum(buf, nbytes);
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = (char) ((chk_sum % 100) / 10 + 48);
			*(t_buffer_new.data + last_frame*120 + j + stuf_count + 1) = (char) (chk_sum % 10 + 48);
			j++;
		}
		else	// ETX
			*(t_buffer_new.data + last_frame*120 + j + stuf_count) = (char) 3;
		}

	// Debug: prints outgoing data from layer 2
	if(debug==1)
	{
		printf("\nPost Layer 2 Transmit Function Test");
		int total_count=0;
		for(int i=0; i < last_frame + 1;i++)
		{
			frame_count = (int) ((*(t_buffer_new.data + 1 + total_count)-48) * 100);
			frame_count += (int) ((*(t_buffer_new.data + 2 + total_count)-48) * 10);
			frame_count += (int) (*(t_buffer_new.data + 3 + total_count)-48);
			printf("\nFrame %i: ", i);
			for(int j=0; j<frame_count; j++)
			{
				printf("%c", *(t_buffer_new.data + j + total_count));
			}
			total_count += frame_count;
		}
	}

	return t_buffer_new;
	}
	// Receiving Mode
	else if(mode == 'r')
	{

		// Debug: prints incoming data for layer 2
		if(debug==1)
		{
			printf("\nPre Layer 2 Receive Function Test");
			int t_count=0;
			int l_frame = 0;
			int frame = 0;
			while(l_frame == 0)
			{
				if(*(t_buffer.data + t_count + 6) == '1')
					l_frame = 1;
				frame_count = (int) ((*(t_buffer.data + 1 + t_count)-48) * 100);
				frame_count += (int) ((*(t_buffer.data + 2 + t_count)-48) * 10);
				frame_count += (int) (*(t_buffer.data + 3 + t_count)-48);
				printf("\nFrame %i: ", frame);
				for(int j=0; j<frame_count; j++)
				{
					printf("%c", *(t_buffer.data + j + t_count));
				}
				t_count += frame_count;
				frame++;
			}
			printf("\n");
		}

		int total_count = 0; // used for indexing (will contain location of STX of a frame at any given time)
		int c=0; //  outgoing buffer location
		int size=0; // for calculating Byte Count of original file
		int lf = 0; // controls the while loop

		// Strip layer 2 hdr and trl (with destuffing)
		while(lf==0)
		{
			if(*(t_buffer.data + total_count + 6) == '1')
				lf = 1;
			frame_count = (int) ((*(t_buffer.data + 1 + total_count)-48) * 100);
			frame_count += (int) ((*(t_buffer.data + 2 + total_count)-48) * 10);
			frame_count += (int) (*(t_buffer.data + 3 + total_count)-48);

			//ChkSum
			nbytes = frame_count - 4;
			// convert char buffer to unsigned short for chechksum calculation
			for(int k = 0; k < nbytes; k++)
			{
				*(buf + k) = (unsigned short) (*(t_buffer.data + total_count + k + 1)-48);
			}
			if(nbytes % 2 == 1) // ensures even number of bytes
			{
				*(buf + nbytes) = '\0';
				nbytes++;
			}
			chk_sum = cksum(buf, nbytes);
			// Error Message for mismatched checksums
			if( (char) (chk_sum % 10 + 48) != *(t_buffer.data + total_count + frame_count - 2) || (char) (chk_sum % 100 /10 + 48) != *(t_buffer.data + total_count + frame_count - 3))
				printf("\nChecksums do not match: T Checksum %c %c R Checksum %c %c",*(t_buffer.data + total_count + frame_count - 3), *(t_buffer.data + total_count + frame_count - 2), (char) (chk_sum % 100 / 10 + 48), (char) (chk_sum % 10 + 48));

			for(int i=total_count + 17; i<(frame_count + total_count - 3);i++)
			{
				if(*(t_buffer.data + i) == (char) 16)
				{
					i++;
					*(t_buffer_new.data + c) = *(t_buffer.data + i);
					c++;
				}
				else
				{
					*(t_buffer_new.data + c) = *(t_buffer.data + i);
					c++;
				}
			}
			total_count += frame_count;
			size += 40;
		}

		t_buffer_new.B_count = c - size;

		// Debug: prints outgoing data from layer 2
		if(debug ==1)
		{
			printf("\nPost Layer 2 Receive Function Test");
			for(int i=0; i<c; i++)
			{
				if(i % 100 == 0)
					printf("\nFrame: %i: ", i/100);
				printf("%c", *(t_buffer_new.data + i));
			}
		}

		return t_buffer_new;
	}
	else
		return t_buffer;
}
