#include "uart.h"
#include "add.h"

int main(void)
{
	uart_print("hello ", 6);
	uart_print("world", 5);

	int len1 = uart_len;
	int len2 = add(6,5);

	return 0;
}
