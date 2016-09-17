int binary_search(int * array, int length, int key)
{
	int low = 0, high = length - 1;
	while(low <= high){
		int mid = (low + high)/2;
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
