void insert_sort(int * array, int length)
{
	return 0;
}

void select_sort(int * array, int length)
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

	return 0;
}

void bubble_sort(int * array, int length)
{
	return 0;
}

void quick_sort(int * array, int length)
{
	return 0;
}

void merge_sort(int * array, int length)
{
	return 0;
}

int binary_search(int * array, int length, int key)
{
	int low = 0, high = length - 1;
	while(low <= high){
		int mid = (low + high) / 2;
		if(key == array[mid]){
			return mid;
		}

		if(key < array[mid]){
			high = mid - 1;
		}else{
			low = mid + 1;
		}
	}

	return -1;
}

