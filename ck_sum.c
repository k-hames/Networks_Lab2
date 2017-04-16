/*
 * ck_sum.c
 *
 *  Created on: Apr 10, 2017
 *      Author: Kevin Hames
 */

#include "nw_lab1.h"

unsigned short cksum(unsigned short *buf, int nbytes)
{
register unsigned long sum = 0;

while(nbytes-=2 > 0)
{
	sum += *buf++;
	if (sum & 0xFFFF0000)
    {
    /* carry occurred, so wrap around */
		sum &= 0xFFFF;
		sum++;
    }
}
return (sum & 0xFFFF);
}

