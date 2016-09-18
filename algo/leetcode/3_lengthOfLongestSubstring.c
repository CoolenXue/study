#include <stdlib.h>

static int lengthOfLongestSubstring(char * s)
{
	int length = strlen(s);
	int * counter = (int *)calloc(sizeof(int), length);

	int  ch_hash[128] = {0};
	int i;
	for(i = 0; i < 128; i++){
		ch_hash[i] = -1;
	}

	int cur = 0, head = 0;
	int max = 0;

	int rem = length;

	while( head < length && counter[cur] + rem > counter[max]){
		int id = s[head];

		if(ch_hash[id] != -1){
			int old_cur = cur;
			cur = ch_hash[id] + 1;
			for(i = old_cur; i < cur; i++){
				ch_hash[s[i]] = -1;
			}

			if(counter[old_cur] > counter[max]){
				max = old_cur;
			}
		}

		ch_hash[id] = head;
		for(i = cur; i <= head; i++){
			counter[i]++;
		}
		head++;
		rem = length - head;
	}

	if(counter[cur] > counter[max]){
		max = cur;
	}

	return counter[max];
}

int main(int argc, char * argv[])
{
	if(argc != 2){
		printf("usage: cmd <string>\n");
	}

	char  * str = argv[1];
	int len = lengthOfLongestSubstring(str);

	printf("result %d\n", len);
	return 0;
}
