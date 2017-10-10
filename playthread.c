#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
/*******************************************************
*compile dengan cara gcc -pthread -o [output] input.c *
*******************************************************/

pthread_t tid[2];//inisialisasi array untuk menampung thread dalam kasusu ini ada 2 thread

int length=157;//inisialisasi jumlah looping
void* playandcount(void *arg)
{
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	if(pthread_equal(id,tid[0]))//thread untuk menjalankan counter
	{
		system("clear");
		for(iter=length;iter>0;iter--)
		{
			printf("%i",iter);
			fflush(stdout);
			sleep(1);
			system("clear");
		}
	}
	else if(pthread_equal(id,tid[1]))
	{
		system("cvlc bagimu-negri.mp3");
	}
	return NULL;
}
int main(void)
{
	int i=0;
	int err;
	while(i<2)//looping membuat thread 2x
	{
		err=pthread_create(&(tid[i]),NULL,&playandcount,NULL);//membuat thread
		if(err!=0)//cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success");
		}
		i++;
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}
