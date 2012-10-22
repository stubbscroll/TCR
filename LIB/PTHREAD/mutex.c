/* pthread example with mutex */
#include <pthread.h>
#include <stdio.h>

#define THREAD 5

int ids[THREAD];
int sum;
pthread_mutex_t mutex;

void *hello(void *ptr) {
	int id=*((int *)ptr);
	int val=(id+1)*(id+1);
	printf("hello from %d with partial result %d\n",id,val);
	printf("%d tries to update sum\n",id);
	pthread_mutex_lock(&mutex);
	sum+=val;
	pthread_mutex_unlock(&mutex);
	printf("%d updated sum!\n",id);
	pthread_exit(NULL);
	return NULL;
}

int main() {
	pthread_t threads[THREAD];
	int i,rc;
	pthread_mutex_init(&mutex,NULL);
	for(sum=i=0;i<THREAD;i++) {
		ids[i]=i;
		printf("creating thread %d\n",i);
		rc=pthread_create(&threads[i],NULL,hello,ids+i);
		if(rc) printf("error creating thread %d\n",i);
	}
	printf("trying to join threads\n");
	for(i=0;i<THREAD;i++) pthread_join(threads[i],NULL);
	printf("threads joined, answer is %d\n",sum);
	pthread_mutex_destroy(&mutex);
	return 0;
}
