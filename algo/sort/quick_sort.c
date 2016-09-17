void quick_sort(int * array, int length)
{
	if(length <= 1){
		return;
	}

	int i = 0, j = length -1;
	int key = array[0];

	while(1){
		while(array[j] > key && j != i){
			j--;
		}

		if(j == i){
			break;
		}

		array[i++] = array[j];

		while(array[i] <= key && i != j){
			i++;
		}

		if(i == j){
			break;
		}

		array[j--] = array[i];
	}

	array[i] = key;

	quick_sort(array, i);
	quick_sort(array + i + 1, length - i - 1); 
}
