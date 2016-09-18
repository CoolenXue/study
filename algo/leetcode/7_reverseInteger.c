#include <stdlib.h>

static int reverse(int x) 
{
	int num_i;
	int sign;

	if( x >= 0){
		sign = 1;
		num_i = x;
	}else{
		sign = -1;
		num_i = x * (-1);
	}

	long num_o = 0;	// note the output
	while(num_i > 0){
		int mod = num_i % 10;
		num_i = num_i / 10;

		num_o = num_o * 10 + mod;
	}

	if(num_o > (int)0x7fffffff){
		num_o = 0;
	}

	return num_o * sign;
}

int main(int argc, char * argv[])
{
	if(argc != 2){
		printf("usage: cmd <integer>\n");
		return -1;
	}

	int input = atoi(argv[1]);
	int output = reverse(input);
	printf("result %d\n", output);

	return 0;
}
