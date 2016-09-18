void insert_sort(int * array, int length)
{
	int i;
	for(i = 1; i < length; i++){
		int val = array[i];
		int j = i;
		while(array[j-1] > val && j > 0){
			array[j] = array[j-1];
			j--;
		}

		array[j] = val;
	}
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
}

#if 0
void bubble_sort(int * array, int length)
{
	int i;
	for(i = 0; i < length - 1; i++){
		int j;
		for(j = length - 1; j > i; j--){
			if(array[j] < array[j-1]){
				int tmp = array[j];
				array[j] = array[j-1];
				array[j-1] = tmp;
			}
		}
	}
}
#else
void bubble_sort(int * array, int length)
{
	int i = 0;
	while( i < length - 1){
		int pos = length - 1;
		int j;
		for( j = length - 1; j > i; j--){
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
#endif

void quick_sort(int * array, int length)
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

static int merge(int * array, int * tmp, int start, int mid, int end)
{

	int i = start, j = mid + 1;
	int k = start;

	while(i <= mid && j <= end){
		if(array[i] <= array[j]){
			tmp[k++] = array[i++];
		}else{
			tmp[k++] = array[j++];
		}
	}

	while(i <= mid){
		tmp[k++] = array[i++];
	}

	while(j <= end){
		tmp[k++] = array[j++];
	}

	for(i = 0; i < k; i++){
		array[i] = tmp[i];
	}

	return 0;
}

void merge_sort(int * array, int length)
{
	int * array_tmp = malloc(sizeof(int) * length);

	int len;
	for(len = 1; len < length; len = len * 2){
		int i;
		for(i = 0; i < length; i += len * 2){
			int end = i + len * 2 - 1;
			if(end > length - 1){
				end = length - 1;
			}

			int mid = i + len - 1;
			if(mid >= end){
				continue;
			}
			merge(array, array_tmp, i, i + len - 1, end);
		}
	}
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

