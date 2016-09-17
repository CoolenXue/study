void bubble_sort(int * array, int length)
{
	int i;
	for(i = 0; i < length; i++){
		int j;
		for( j = length - 1; j > i; j--){
			if(array[j] < array[j-1]){
				int tmp;
				tmp = array[j]; 
				array[j] = array[j-1];
				array[j-1] = tmp;
			}
		}
	}
}

void bubble_sort1(int * array, int length)
{
	int i = 0;
	while( i < length - 1){
		int pos = length -1;

		int j;
		for( j = pos; j > i; j--){
			if(array[j] < array[j-1]){
				int tmp;
				tmp = array[j]; 
				array[j] = array[j-1];
				array[j-1] = tmp;

				pos = j;
			}
		}

		i = pos;
	}
}
