//
//  Philo.c
//  
//
//  Created by wangxue on 16/12/6.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 5
#define TRUE 1

#define p(x) sem_wait(&x)
#define v(x) sem_post(&x)

/*
 sem_wait(&x)
 sem_wait是一个原子操作，它的作用使从信号量的值中减"1",
 但它永远会先等待该信好两为一个非零值才开始做减法
 sem_post(&x)
 在不冲突的情况下，给信号量的值加上一个"1"
 */

sem_t forks[MAX];//记录叉子的状态，0代表干净，1代表脏
pthread_mutex_t mutex;

void init ( )
{
    int i;
    //初始话信号量，叉子的初始状态是脏(1)
    for ( i = 0 ; i < MAX ; i++ )
        sem_init(&forks[i],0,1);
}

//描述哲学家的操作的函数
void * philosopher ( void* arg )
{
    int* p = (int*)arg;
    int x = *p;//获得哲学家的id
    while ( TRUE )
    {
        pthread_mutex_lock(&mutex);
        p(forks[x]);//如果叉子脏，那么由当前拿叉子的人擦干净拿来
        p(forks[(x+1)%MAX]);//同上
        printf ( "the %dth philosopher having meal...\n" , x );
        sleep(1);//哲学家吃啊吃...
        v(forks[x]);//吃饭后弄脏了叉子
        v(forks[(x+1)%MAX]);//同上
        pthread_mutex_unlock(&mutex);
        sleep(2);//哲学家等自己变饿
    }
}

int main ( )
{
    init ();//初始化叉子的状态
    pthread_t id[MAX];//用来存每个哲学家线程的id
    int tid[MAX];//用来存每个哲学家的id
    int i;
    for ( i = 0 ; i < MAX ; i++ )
    {
        tid[i] = i;
        //创建一个新的线程
        pthread_create ( &id[i] , NULL , philosopher , &tid[i] );
    }
    //设置主线程阻塞，等待每个哲学家线程
    for ( i = 0 ; i < MAX ; i++ )
        pthread_join ( id[i] , NULL );
}
