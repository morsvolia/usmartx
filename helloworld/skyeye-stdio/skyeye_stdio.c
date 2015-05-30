/*   
 *  skyeye_stdio.c  
 *  this file defines standard input/output on AT91X40 in SkyEye 
 *  for Atmel AT91 input/output
 *
 *  Bugs report:     li ming  ( lmcs00@mails.tsinghua.edu.cn )
 *  Last modified:   2003-02-02 
 *
 */

#include "skyeye_stdio.h"
#include "string.h"

/* this file is to be used in ucosII on Skyeyey like <stdio.c> in linux */
#include "at91_usart.h"

void skyeye_putc( char ch )
{
	at91_uart_put_char( ch );
	return;	
}

char skyeye_getc(void)
{
	char ret;
	at91_uart_get_char( &ret );
	return ret;
}

void skyeye_puts( char * s )
{
	int len = strlen( s );	
	at91_uart_send_string( s, len );
	return;
}

char * skyeye_gets( char * s )
{
	char c;
	char * src = s;
	int count = 0;
	while(1)
	{
		c = skyeye_getc();
		
		if ( c == '\n' )
		{
			*src = '\0';
			skyeye_putc( c );
			break;
		}
		
		if ( c == '\b' )
		{
			if ( count <= 0 )
				continue;
			count--;
			src--;
			skyeye_putc( '\b' );
			skyeye_putc( ' ' );
			skyeye_putc( '\b' );
			continue;					
		}
		count++;
		*src++ = c;
		skyeye_putc( c );
	}
	
	return s;
}
