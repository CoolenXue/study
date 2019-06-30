#include <stdlib.h>
#include <time.h>

static void select_slow_sort(int * array, int length)
{
	int i;
	for(i = 0; i < length - 1; i ++){
		int j;
		for(j = i + 1; j < length; j++){
			if(array[j] < array[i]){
				int tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
}

static void select_fast_sort(int * array, int length)
{
	int i;
	for(i = 0; i < length - 1; i++){
		int min = i;
		int j;
		for(j = i + 1; j < length; j++){
			if(array[j] < array[min]){
				min = j;
			}
		}

		if(min != i){
			int tmp = array[i];
			array[i] = array[min];
			array[min] = tmp;
		}
	}
}

static void bubble_slow_sort(int * array, int length)
{
	int i;
	for( i = length - 1; i > 0 ; i--){
		int j;
		for(j = 0; j < i; j++){
			if(array[j] > array[j+1]){
				int tmp = array[j];
				array[j] = array[j+1];
				array[j+1] = tmp;
			}
		}
	}
}

static void bubble_fast_sort(int * array, int length)
{
	int i = 0;
	while( i < length - 1){
		int pos = length - 1;
		int j;
		for(j = length - 1; j > i; j--){
			if(array[j] < array[j-1]){
				int tmp = array[j];
				array[j] = array[j-1];
				array[j-1] = tmp;
				pos = j;
			}
		}

		i = pos;
	}
}

static void insert_sort(int * array, int length)
{
	int i;
	for(i = 1; i < length; i++){
		int val = array[i];
		int j;
		for(j = i ; j > 0; j--){
			if(array[j - 1] > val){
				array[j] = array[j-1];
			}else{
				break;
			}
		}

		array[j] = val;
	}
}

static void quick_sort(int * array, int length)
{
	if(length <= 1){
		return;
	}

	int key = array[0];
	int i = 0, j = length - 1;
	while(i < j){
		while(array[j] > key && i < j){
			j--;
		}
		if(i == j){
			break;
		}
		array[i] = array[j];

		while(array[i] <= key && i < j){
			i++;
		}
		if( i == j){
			break;
		}
		array[j] = array[i];
	}
	array[i] = key;

	quick_sort(&array[0], i);
	quick_sort(&array[i+1], length - 1 -i);
}

static void merge_sort(int * array, int length)
{
	sleep(5);
}

static int _cmpint(const void * p1, const void * p2)
{
	return (*(int *)p1) - (*(int *)p2);
}

static void clib_sort(int * array, int length)
{
	qsort(array, length, sizeof(int), _cmpint);
}

typedef struct s_sort_algo{
	char *name;
	void ( *algo )(int *array, int length);
}t_sort_algo;

#define DECLARE_ALGO(name, algo) \
{	\
	name, 	\
	algo,	\
}

static t_sort_algo algo_list[] = {
	DECLARE_ALGO("qsort", clib_sort),
	DECLARE_ALGO("select_slow", select_slow_sort),
	DECLARE_ALGO("select_fast", select_fast_sort),
	DECLARE_ALGO("bubble_slow", bubble_slow_sort),
	DECLARE_ALGO("bubble_fast", bubble_fast_sort),
	DECLARE_ALGO("insert", insert_sort),
	DECLARE_ALGO("quick", quick_sort),
	DECLARE_ALGO("merge", merge_sort),
};

#define DATA_LEN 0x10000
void main(int argc, char **argv)
{
	int len = DATA_LEN;
	int * data = malloc(len *sizeof(int));
	if(!data){
		printf("[ERROR]: malloc fail\n");
		return;
	}

	srand(clock());
	int i;
	for(i = 0; i <len; i++){
		data[i] = rand();
	}

	int * array = malloc(len * sizeof(int));
	if(!array){
		printf("[ERROR]: malloc array fail\n");
		return;
	}

	int * result = malloc(len * sizeof(int));
	if(!result){
		printf("[ERROR]: malloc result fail\n");
		return;
	}


	for(i = 0; i < sizeof(algo_list)/sizeof(algo_list[0]); i++){
		memcpy(array, data, len * sizeof(int));

		struct timespec tp_s;
		clock_gettime(CLOCK_REALTIME, &tp_s);

		algo_list[i].algo(array, len);

		struct timespec tp_e;
		clock_gettime(CLOCK_REALTIME, &tp_e);

		printf("[%s]: cost %d ms\n", algo_list[i].name, (tp_e.tv_sec - tp_s.tv_sec) *1000 + (tp_e.tv_nsec - tp_s.tv_nsec)/1000000);

		if(!strcmp(algo_list[i].name, "qsort")){
			memcpy(result, array, len * sizeof(int));
		}

		if(memcmp(result, array, len * sizeof(int))){
			printf("[%s]: sort fail\n", algo_list[i].name);
		}
	}

	if(data){
		free(data);
	}
	if(array){
		free(array);
	}
}
