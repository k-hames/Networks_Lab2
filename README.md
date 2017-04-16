# Networks_Lab2
Networks lab illustrating the operation of protocols in a multi layer model. This lab consists of four layers with both transmit and recieve functions and allows data from a text file to mimick packet transfers between devices. Implemented using C programming. 

Additions to lab 1: 
  Added CheckSum & Bit Stuffing to Layer 2 
  
  Layer Breakdown
  
  Layer 1: transmits or receives data packets between two nodes (given by professor)
  
  Layer 2: Attactches layer 2 hdr and trl providing a wide range of information to each frame
  
  Layer 3: Splits the data into frames with a hdr and payload (hdr representing the IP address of destination)
  
  Layer 4: Reads data from a txt file and stores in a buffer for future use
