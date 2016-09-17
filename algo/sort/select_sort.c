// select one
// select the min/max index in [i, length) and switch value with i. 
void select_sort(int * array, int length)
{
	int i;
	for(i = 0; i < length - 1; i++){
		int k = i;
		int j;
		for( j = i + 1; j < length; j++){
			if( array[j] < array[k]){
				k = j;
			}
		}

		if( k != i ){
			int tmp;
			tmp = array[k];
			array[k] = array[i];
			array[i] = tmp;
		}
	}
}

// select two a time
void select_sort1(int * array, int length)
{
	int i;
	for( i = 0; i < length/2; i++){
		int min = i;
		int max = length - 1 - i;

		int j;
		for( j = i; j < length - i; j++){
			if(array[j] < array[min]){
				min = j;
			}
			if(array[j] > array[max]){
				max = j;
			}
		}

		int tmp;
		if( min != i){
			tmp = array[i];
			array[i] = array[min];
			array[min] = tmp;
		}

		if( max == i){
			max = min;
		}

		if( max != length - 1 - i){
			tmp = array[length-1-i];
			array[length-1-i] = array[max];
			array[max] = tmp;
		}
	}
}
