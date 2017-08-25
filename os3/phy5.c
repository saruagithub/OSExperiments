#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#define M 5
pthread_mutex_t chopsticks[M] = {PTHREAD_MUTEX_INITIALIZER};
pthread_mutex_t room = PTHREAD_MUTEX_INITIALIZER; //used to lock the room when room_people is 4
int room_people=0;
time_t first,second;

void think(int i)
{
	usleep(2);
	printf("%d  am thinking!\n", i);
}
void wait_room(int i)
{
	if(room_people==4)
		pthread_mutex_lock(&room);
	room_people++;
	printf("%d is going into the room!\n",i);
}
void wait_cho(pthread_mutex_t t, int i)
{
	pthread_mutex_lock(&t);
	printf("%d is locked\n", i);
}
void eat(int i){
	sleep(2);
    second = time(NULL);
    printf("time: %f seconds",difftime(second,first));
    //getch();
    printf("%d  am eatting!\n", i);
    
}
void signal_room(int i)
{
	if(room_people==4)
		pthread_mutex_unlock(&room);
	room_people--;
	printf("%d is going out from the room\n",i);
}
void signal_cho(pthread_mutex_t t,int i){
	pthread_mutex_unlock(&t);
	printf("%d is unlocked\n",i);
}
void philosopher(void* id)	//philosopher alogrithm
{
	int i = *((int*)id);
    printf("i is %d\n",i);
	printf("%d philosopher_pthread creat!\n",i);
    first = time(NULL); //pthread create time
	while(1){	
		think(i);
		wait_room(i);
		// if(pthread_mutex_trylock(&chopsticks[i]) == 0){
		// 	continue;
		// }
		wait_cho(chopsticks[i],i);
//		 if(pthread_mutex_trylock(&chopsticks[(i+1)%5]) == 0){
//		 	pthread_mutex_unlock(&chopsticks[i]);
//		 	continue;
//		 }
		wait_cho(chopsticks[(i+1)%5],(i+1)%5);
		//printf("%d fetch right chopsticks\n", i);
		eat(i);
		signal_cho(chopsticks[i],i);
		signal_cho(chopsticks[(i+1)%5],(i+1)%5);
		signal_room(i);
        break;
	}
}
int main()
{
	int i;
	// for(i=0;i<5;i++){
	// 	// sem_init(&chopsticks[i],1,1);
	// 	(pthread_mutex_t)chopsticks[i] = PTHREAD_MUTEX_INITIALIZER;
	// }// init the mutex
	// sem_init(&room);
    srand(time(0));
	pthread_t philosopher_ptd[5];// five philosopher
	int id[] = {0,1,2,3,4};
	for(i=0;i<5;i++){
		pthread_create(&philosopher_ptd[i],NULL,(void  *)philosopher,(void*)(&id[i]));
		//philosopher(philosopher_ptd[i],i);
		//pthread_join(philosopher_ptd[i],NULL);
	}
	while(1){
		usleep(1200);
		break;
	}
	return 0;

}
