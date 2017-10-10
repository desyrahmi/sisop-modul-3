#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int i,length=10;
	system("clear");
	for(i=length;i>0;i--)
	{
		printf("%d",i);
		fflush(stdout);
		sleep(1);
		system("clear");
	}
	system("cvlc bagimu-negri.mp3");
}
