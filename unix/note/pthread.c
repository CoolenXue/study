#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>    // must include this for 'posixoptions'
#include<string.h>    // must include this for 'strerror', Segment fault otherwise.

#ifdef _POSIX_THREADS
#define PTHREAD_SUPPORT 1
#else
#define PTHREAD_SUPPORT 0
#endif

static void print_id(char * title)
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	printf("[%10s]: pid 0x%x tid 0x%x\n", title, tid);
}

static void * id_test_routine(void * arg)
{
	int ret = pthread_detach(pthread_self()); 		// detach here or inside thread?
	if( ret != 0){
		printf("[%10s] detach id_test thread fail: %s\n", "id_test", strerror(ret));
	}

	print_id("id_test");
	sleep(10);
	printf("id_test thread return\n");
	return (void *)3;
}

static void _cleanup_cb(void * arg)
{
	printf("tid 0x%x cleanup: %s\n", pthread_self(), (char *)arg);
}

static void * tid1_func(void * arg)
{
	printf("tid1 0x%x start\n", pthread_self());
	pthread_cleanup_push(_cleanup_cb, "tid1 first cleanup handler");
	pthread_cleanup_push(_cleanup_cb, "tid1 second cleanup handler");
	printf("tid1 push\n");
	if(arg){
		printf("tid1 0x%x return\n", pthread_self());
		return (void *)1;
	}

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	printf("tid1 0x%x return after pop\n", pthread_self());
	return (void *)1;
}

static void * tid2_func(void * arg)
{
	sleep(4);

	printf("tid2 0x%x start\n", pthread_self());
	pthread_cleanup_push(_cleanup_cb, "tid2 first cleanup handler");
	pthread_cleanup_push(_cleanup_cb, "tid2 second cleanup handler");
	printf("tid2 push\n");
	if(arg){
		printf("tid2 0x%x return\n", pthread_self());
		pthread_exit((void *)2);
	}

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	printf("tid2 0x%x return after pop\n", pthread_self());
	pthread_exit((void *)2);
}

int main(int argc, char ** argv)
{
	// supporting test
    printf("Is pthread supported(compile time)? %d\n", PTHREAD_SUPPORT);
    printf("Is pthread supported(run time)? %d\n", sysconf(_SC_THREADS));


	// id test, detach test
    pthread_t tid_id_test;
    int ret = pthread_create(&tid_id_test, NULL, id_test_routine, NULL);
	if(ret != 0){
		printf("[%10s] create id_test thread fail: %s\n", "main", strerror(ret));
	}

	print_id("main");

	//sleep(5);
	ret = pthread_detach(tid_id_test); 		// Q: detach here or inside thread? A: just ensure it is called before 'join'.
	if( ret != 0){
		printf("[%10s] detach id_test thread fail: %s\n", "main", strerror(ret));
	}

	void * id_test_ret;
	ret = pthread_join(tid_id_test, &id_test_ret);
	if(ret != 0){
		printf("[%10s] join id_test thread fail: %s\n", "main", strerror(ret));
	}else{
		printf("pthread_join(0x%x, 0x%x)\n", tid_id_test,(int)id_test_ret);
	}

	// exit test
	pthread_t tid1, tid2;
	void * tid1_ret, *tid2_ret;

	pthread_create(&tid1, NULL, tid1_func, (void *)1);
	pthread_create(&tid2, NULL, tid2_func, (void *)1);

	pthread_join(tid1, &tid1_ret); 
	printf("pthread_join(0x%x, 0x%x)\n", tid1,(int)tid1_ret);
	pthread_join(tid2, &tid2_ret);
	printf("pthread_join(0x%x, 0x%x)\n", tid2,(int)tid2_ret);

	while(1){
		sleep(5);
	}
    return 0;
}
