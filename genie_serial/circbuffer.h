#ifndef _CIRCBUFFER_H
#define _CIRCBUFFER_H
#define BUFFERSIZE 160
struct circBuffer{
	char buffer[BUFFERSIZE];
	unsigned short head; // location for write in one char
	unsigned short tail; // location for read out one char
	short len;
};

static inline int circBuffer_isEmpty(struct circBuffer *buf)
{
	return((buf->tail == buf->head) && (buf->len == 0));
}
void circBuffer_init(struct circBuffer *buf);
char circBuffer_get(struct circBuffer *buf);
void circBuffer_put(struct circBuffer *buf,char ch);
#endif
