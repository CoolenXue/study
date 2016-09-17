// find insert point from the tail
void insert_sort(int * array, int length)
{
	int i;
	for(i = 1; i < length; i++){
		int cur = array[i];
		int j = i;
		while( j > 0 && array[j-1] > cur){
		   array[j] = array[j-1];
		   j--;
		}

		array[j] = cur;
	}
}

// find insert point from the head
void insert_sort1(int * array, int length)
{
	int i;
	for( i = 1; i < length; i++){
		int cur = array[i]; 
		int j;
		for( j = 0; j < i; j++){
			if(array[j] > array[i]){
				break;
			}
		}

		int k;
		for( k = i; k > j; k--){
			array[k] = array[k-1];
		}

		array[j] = cur;
	}
}
