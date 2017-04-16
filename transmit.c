/* Prototype for layer1(); DO NOT mess with the next line, or change it in any way */
int  layer1(char *frameptr, int nbytes, char mode, int debug);
#include "nw_lab1.h"
/* Outline of transmit function is shown below */

int transmit ()

{
	buffer data_buffer;
	buffer t_buffer;
	int debug = 1;

	/* Call your layer 4 function (Application) */
	data_buffer = layer4(data_buffer,'t',debug);
	/* Call your layer 3 function */
	t_buffer = layer3(data_buffer,'t',debug);
	/* Call your layer 2 function */
	t_buffer = layer2(t_buffer, 't', debug);

	/* Call Dr. Kulkarni's layer 1 function (see skeleton code below), once for every
	   frame. Therefore, if your layer 2 constructed three frames, call layer 1 thrice,
	   once per frame
	*/

	int	retcode;
	int total_frames;
	if(t_buffer.B_count == MSS)
		total_frames= 0;
	else
		total_frames= t_buffer.B_count / MSS;
	int frame_count = 0;
	int total_count = 0;

	for (int i= 0; i < total_frames + 1; ++i) {

		/* Before call to layer 1, input parameters must be set as foll:
			txbuf is your char pointer to your transmission buffer
			nbytes is the no. of bytes in your layer 2 frame
			't' means transmit mode; DO NOT change this value.
			_DEBUG sets debug mode; valid values are 1 (true), and 0 (false)

			 Output parameter retcode reports the number of bytes successfully
			 transmitted over the PHY channel; there is no implied guarantee
			 that it was received by the receiver.
		*/

		frame_count = (int) ((*(t_buffer.data + 1 + total_count)-48) * 100);
		frame_count += (int) ((*(t_buffer.data + 2 + total_count)-48) * 10);
		frame_count += (int) (*(t_buffer.data + 3 + total_count)-48);

		retcode = layer1(t_buffer.data + total_count, frame_count, 't', debug); /* Function call to layer1 */

		total_count += frame_count;
		frame_count = 0;

		/* if retcode < nbytes, then Layer 1 transmitted fewer bytes than layer2 supplied
			so, display an error message and terminate the program
			otherwise, continue transmitting in the loop
		*/
		if(retcode < frame_count)
			return -1;
	}

	return 0;
}
