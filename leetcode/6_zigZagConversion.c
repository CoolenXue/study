#include<stdlib.h>

static char* convert(char* s, int numRows) 
{
	if(s == NULL || numRows == 0){
		return NULL;
	}

	int len = strlen(s);
	char * ret = (char *)malloc(len + 1);

	if(numRows == 1){
		strncpy(ret, s, len);
		ret[len] = '\0';
		return ret;
	}

	typedef struct s_pos{
		int x;
		int y;
	}t_pos;
	t_pos * pos = (t_pos *)malloc(sizeof(t_pos) * len);

	int * rows_num = (int *)malloc(sizeof(int) * numRows);

	// for id 0
	pos[0].x = 0;
	pos[0].y = 0;
	rows_num[0] = 1;

	int x = 0, y = 0, flag = 0;
	int i;
	for( i = 1; i < len; i++){
		if(flag == 0){
			x++;
			y = (i - x)/2 ;
		}else{
			y++;
			x = i - 2 * y;
		}

		pos[i].x = x;
		pos[i].y = rows_num[x];
		rows_num[x]++;

		if(i % (numRows - 1) == 0){
			flag = 1 - flag;
		}
	}

	int sum = 0;
	for(i = 0; i < numRows; i++){
		int tmp = rows_num[i];
		rows_num[i] = sum;
		sum += tmp;
	}

	for(i = 0; i < len; i++){
		int id = rows_num[pos[i].x] + pos[i].y;
		ret[id] = s[i];
	}

	ret[len] = '\0';

	return ret;
}

int main(int argc, char * argv[])
{
	if(argc != 3){
		printf("usage: cmd <str> <rows>\n");
	}

	char * str = argv[1];
	int rows = atoi(argv[2]);

	printf("result %s\n", convert(str, rows));
	return 0;
}
