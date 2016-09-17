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

int merge_sort(int * array, int length)
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

	return 0;
}
