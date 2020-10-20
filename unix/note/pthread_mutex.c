#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>

typedef struct msg_s{
	struct msg_s * next;
	char ch;
}msg_t;

static msg_t * _msg_list;
static pthread_mutex_t _list_mutex;
static pthread_cond_t _list_cond;

static void * _thread_producer(void * arg)
{
	pthread_detach(pthread_self());

	printf("Enter thread producer, input chars please...\n");
	char ch;
	while(ch = getchar()){
		if(ch == '\n'){
			continue;
		}
		msg_t * msg = malloc(sizeof(msg_t));
		msg->ch = ch;
		
		pthread_mutex_lock(&_list_mutex);
		msg->next = _msg_list;
		_msg_list = msg;
		printf("[producer]: add %c\n", msg->ch);
		pthread_cond_signal(&_list_cond);
		pthread_mutex_unlock(&_list_mutex);

		if(ch == 'q'){
			break;
		}
	}

	return (void *)0;
}



int main(int argc, char ** argv)
{
	pthread_t tid_producer;
	int ret = pthread_create(&tid_producer, NULL, _thread_producer, NULL);
	if( ret != 0){
		printf("create thread producer fail: %s\n", strerror(ret));
	}

	while(1){
		pthread_mutex_lock(&_list_mutex);
		if(_msg_list == NULL){
			pthread_cond_wait(&_list_cond, &_list_mutex);
		}

		msg_t * msg = _msg_list;
		while(msg != NULL && msg->ch != 'q'){
			printf("[main]: del %c\n", msg->ch);
			_msg_list = msg->next;
			free(msg);
			msg = _msg_list;
		}

		if(msg != NULL && msg->ch == 'q'){
			printf("[main]: over\n");
			free(msg);
			_msg_list = NULL;
			pthread_mutex_unlock(&_list_mutex);
			break;
		}

		pthread_mutex_unlock(&_list_mutex);
	}

	return 0;
}
