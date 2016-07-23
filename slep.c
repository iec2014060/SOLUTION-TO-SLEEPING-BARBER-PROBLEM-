#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
int total_seat;
int Total_time;
int count=1;
int tim;
int cut_time;
int t;
sem_t waiting_customer,mutex,barber_status,aaaa;
void *customer(void *ram);
void *barber(void *ram);
int main()
{
printf("enter number of seat\n");
scanf("%d",&total_seat);
printf("total time for shop will be open\n");
scanf("%d",&Total_time);
printf("cutting time of a customer\n");
scanf("%d",&cut_time);
sem_init(&aaaa,0,0);
int res;
res=sem_init(&waiting_customer,0,0);
if (res != 0) {
perror("Semaphore initialization failed");
exit(EXIT_FAILURE);
}
res=sem_init(&mutex,0,1);
if (res != 0) {
perror("Semaphore initialization failed");
exit(EXIT_FAILURE);
}
res=sem_init(&barber_status,0,1);
if (res != 0) {
perror("Semaphore initialization failed");
exit(EXIT_FAILURE);
}
pthread_t thread[100],thread2;
int t[100],j=0;
printf("enter customer time\n");
while(1)
{
scanf("%d",&t[j]);
if(t[j]==-1)
{
break;
}
j++;
}
int a,b,c,e;
a=pthread_create(&thread2,NULL,barber,NULL);
if (a!= 0) {
perror("thread creation failed");
exit(EXIT_FAILURE);
}
for(b=0;b<j;b++)
{
a=pthread_create(&thread[b],NULL,customer,(void*)t[b]);
if(a!=0)
{
perror("create customer thread");
exit(EXIT_FAILURE);
}
}
c=pthread_join(thread2,NULL);
if (c != 0) {
perror("Thread join failed");
exit(EXIT_FAILURE);
}
exit(EXIT_SUCCESS);
sem_destroy(&waiting_customer);
sem_destroy(&mutex);
sem_destroy(&barber_status);
eixit(EXIT_SUCCESS);
}
void *customer(void *ram)
{
int q;
q=(int)ram;
sleep(q);
sem_wait(&mutex);
if(q==t)
{
sem_wait(&aaaa);
sem_post(&aaaa);
}
int p;
sem_getvalue(&waiting_customer,&p);
if(p<total_seat)
{
printf("customer (%dth) enter in barber shop at time [%d] and total number of customer waiting in barber shop =%d\n",count++,q,p+1);
sem_post(&waiting_customer);
sem_post(&mutex);
sem_wait(&barber_status);
tim=q;
}
else
{
sem_post(&mutex);
printf("---all seat full return customer *[%d]* \n",count++);
}
pthread_exit(NULL);
}
void *barber(void *ram)
{
printf("*shop open*at time=[0]\n");
int n,i=0;
while(i<Total_time)
{
sem_wait(&waiting_customer);
sem_post(&aaaa);
sem_wait(&mutex);
if(i<tim)
{
printf("awoke up at time=[%d]\n",tim);
i=tim;
}
sem_getvalue(&waiting_customer,&n);
sem_post(&mutex);
printf("barber busy: one on chair and no. of waiting custumer=[%d] at time [%d]\n",n,i);
i=i+cut_time;
t=i;
sem_wait(&aaaa);
sleep(cut_time);
sem_post(&barber_status);
printf("complete cutting at time [%d]\n",i);
}
sem_wait(&mutex);
printf("*barber shop close at time*=[%d]\n",i);
pthread_exit(NULL);
}
