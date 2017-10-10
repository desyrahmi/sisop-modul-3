#include<stdio.h>
#include<pthread.h>

int i=0;
pthread_t tid[2];
pthread_mutex_t lock;

void *tulis(void *ptr)
{
	pthread_mutex_lock(&lock);
	i=i+1;
	printf("Thread %d dimulai\n",i);
	printf("Tekan enter untuk mengakhiri thread\n");
	getchar();
	printf("Thread %d selesai\n",i);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main()
{
	int j,err;
	for(j=0;j<2;j++)
	{
		err=pthread_create(&tid[j],NULL,&tulis,NULL);
		if(err==0)printf("Thread Created\n");
	}
	for(j=0;j<2;j++)
	{
		pthread_join(tid[j],NULL);
	}
}
