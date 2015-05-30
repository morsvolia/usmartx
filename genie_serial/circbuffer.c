#include "arch-nxp/lpc2k_bsp.h"
#include "circbuffer.h"

void circBuffer_init(struct circBuffer *buf)
{ // reset buffer head,tail index and length to zero
	buf->head=0;
	buf->tail=0;
	buf->len=0;
}
char circBuffer_get(struct circBuffer *buf)
{
 char retch=0;
	if(!circBuffer_isEmpty(buf)) // DO buffer empty?
	{
		retch=buf->buffer[buf->tail]; // dequeue one char from tail
		disableVectorIntr();
		if(buf->tail < BUFFERSIZE -1) // reach end of buffer ?
			buf->tail++;
		else
			buf->tail=0;
		buf->len--; // adjust buffer length data member
		enableVectorIntr();
	}
	return(retch);
}

void circBuffer_put(struct circBuffer *buf,char ch)
{
 	disableVectorIntr();
	if(buf->head < BUFFERSIZE ) // head index end of buffer?
	{
		buf->buffer[buf->head]=ch;
		buf->head++;		
	}
	else	
	{
		buf->head=0; // warp around to start of buffer
		buf->buffer[0]=ch;
	}
	if(buf->len < BUFFERSIZE -1) // if buffer is not full in size
		buf->len++;	// adjust buffer length data member
	enableVectorIntr();
}
