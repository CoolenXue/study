#include "uart.h"

static char _uart_buf[30] = "data_test";
int uart_len = 0;

static int uart_putc(char ch)
{
	if(uart_len > 30){
		return -1;
	}

	_uart_buf[uart_len] = ch;
	uart_len++;
	return 0;
}

int uart_print(char * str, int len)
{
	int i;
	for(i = 0; i < len; i++){
		uart_putc(*(str + i));
	}

	return 0;
}
