/* Prototype for layer1(); DO NOT mess with the next line, or change it in any way */
int  layer1(char *frameptr, int nbytes, char mode, int debug);
#include "nw_lab1.h"

/* Outline of receive function is shown below */

int receive ()

{
	buffer t_buffer;
	t_buffer.data = (char *) malloc(4800);
	t_buffer.B_count = 0;
	int debug = 1;
	int count = 0;
	int total_count = 0;
	/* Call Dr. Kulkarni's layer 1 function once per frame (see skeleton code below)
		Before call to layer 1, input parameters must be set as foll:
			rxbuf is your char pointer to your reception buffer in which
			 layer1 will store the incoming frame.
			The nbytes parameter will be ignored during reception
			'r' means receive mode; DO NOT change this value.
			_DEBUG sets debug mode; valid values are 1 (true), and 0 (false)

			 Output parameter retcode reports the number of bytes successfully
			 received over the PHY channel; a negative retcode value implies 
			 that there was a reception error
	*/
	
	int	retcode;
	int last_frame = 0;
	while (last_frame != 1) { // while still transmitting frames

		retcode = layer1(t_buffer.data + total_count, 0, 'r', debug); /* Function call to layer1 */

		/* if retcode < 0, then Layer 1 did not receive the frame
			so, display an error message and terminate the program
			otherwise, continue in the while loop till the last frame has
			been received
		*/
		if(retcode < 0)
			return -1;
		
		if(*(t_buffer.data + total_count + 6) == '1') // last frame end loop (Fixed bug by using last frame bit)
		{
			last_frame = 1;
		}
		total_count += retcode;
		count++;
	}
	t_buffer.B_count = total_count + 1; // Extra byte added in ubuntu

	// Debug: Prints data received from layer 1
	/*
	if(debug == 1)
	{
		printf("\nPost Layer 1 Receive Function Test\n");
		for(int i=0; i<count-1; i++)
		{
			printf("Frame %i: ", i);
			for(int j=0; j<120; j++)
			{
				printf("%c", *(t_buffer.data +j + i*120));
			}
			printf("\n");
		}
		printf("Frame %i: ", count-1);
		for(int k=0; k <= retcode; k++)
			printf("%c", *(t_buffer.data + k + (count-1)*120));
		printf("\n");
	} */
	if(debug==1)
	{
		printf("\nPost Layer 1 Receive Function Test");
		int total_count=0;
		int frame_count=0;
		for(int i=0; i < count;i++)
		{
			frame_count = (int) ((*(t_buffer.data + 1 + total_count)-48) * 100);
			frame_count += (int) ((*(t_buffer.data + 2 + total_count)-48) * 10);
			frame_count += (int) (*(t_buffer.data + 3 + total_count)-48);
			printf("\nFrame %i: ", i);
			for(int j=0; j<frame_count; j++)
			{
				printf("%c", *(t_buffer.data + j + total_count));
			}
			total_count += frame_count;
		}
	}

	/* Call your layer 2 function */
	t_buffer = layer2(t_buffer, 'r', debug);
	/* Call your layer 3 function */
	t_buffer = layer3(t_buffer, 'r', debug);
	/* Call your layer 4 function */
	layer4(t_buffer, 'r', debug);

  return 0;
}
