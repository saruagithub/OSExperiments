#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
typedef struct PCB
{
	int ID;//pthread id
	int Priority;
	int CPUTime;//pthread have occupied cpu time
	int AllTime;// other cpu time that pthread need
	int state;//,new1,ready2,running3,terminated4
}PCB;
PCB ptd_pcb[20];	//20 pthread _PCB


void PCB_printf(PCB p)
{
	printf("ptd_PCB ID: %d\nPriority: %d\nCPUTime: %d\nAllTime: %d\n",p.ID,p.Priority, p.CPUTime,p.AllTime);
	switch(p.state){
		case 1:
			printf("new\n\n");
			break;
		case 2:
			printf("ready\n\n");
			break;
		case 3:
			printf("running\n\n");
			break;
		case 4:
			printf("terminated\n\n");
			break;
		default:
			printf("error\n\n");
			break;
	}
}
void ptd_pcbinit(void* ii)	//pthread init PCB
{
	int i = *((int *)ii);
	ptd_pcb[i].ID = i;
	ptd_pcb[i].Priority = rand()%20+1;
	ptd_pcb[i].CPUTime = 0;
	ptd_pcb[i].AllTime = rand()%5+1;
	ptd_pcb[i].state = 1;
    printf("ptd_PCB ID: %d\nPriority: %d\nCPUTime: %d\nAllTime: %d\nstate: %d\n\n",ptd_pcb[i].ID,ptd_pcb[i].Priority, ptd_pcb[i].CPUTime,ptd_pcb[i].AllTime,ptd_pcb[i].state);
}

void FCFS()		//FCFS algorithm
{
    int i;
	for(i=0;i<20;i++)
	{
		ptd_pcb[i].state = 2;
		printf("%d PCB  is ready!\n", ptd_pcb[i].ID);
		while(ptd_pcb[i].AllTime!=0)		//running in CPU
		{
			ptd_pcb[i].state = 3;
			ptd_pcb[i].AllTime--;
			ptd_pcb[i].CPUTime++;
			printf("Thread %d CPUTime is %d\n",ptd_pcb[i].ID,ptd_pcb[i].CPUTime);
		}
		if(ptd_pcb[i].state == 4)
			printf("%d  pthread is  terminated\n",ptd_pcb[i].ID);
	}
}
void SJF_sort()
{
    int i,j;
    for(i=0;i<19;i++)
        for(j=0;j<19-i;j++)
            if(ptd_pcb[j].AllTime>ptd_pcb[j+1].AllTime)
            {
                PCB temp;
                temp = ptd_pcb[j];
                ptd_pcb[j] = ptd_pcb[j+1];
                ptd_pcb[j+1] = temp;
            }   //bubble sort
    FCFS();
}


void RR_Run()
{
    int i=0;
    while(ptd_pcb[i].AllTime>0){
        ptd_pcb[i].state = 3;
        ptd_pcb[i].AllTime--;
        ptd_pcb[i].CPUTime++;
        printf("Thread %d CPUTime is %d\n",ptd_pcb[i].ID,ptd_pcb[i].CPUTime);
        if(ptd_pcb[i].AllTime==0){
            ptd_pcb[i].state = 4;
            printf("%d  pthread is  terminated\n",ptd_pcb[i].ID);
        }
        i=(i+1)%20;
        while(ptd_pcb[i].state==4){
            i=(i+1)%20;
        }
    }
    printf("All is terminated!\n");
}

int main()
{
	int i;
	pthread_t ptd[20]; 	//20 pthreads	
	srand(time(0));
	for(i=0;i<20;i++){
		pthread_create(&ptd[i],NULL,(void *)ptd_pcbinit,(void *)&i);//creat pthread
        pthread_join(ptd[i],NULL);//in order to create all pthread and printf in order
    }
//    printf("FCFS************FCFS\n\n\n");
//	FCFS();   //FCSC algorithm
//    printf("SJF**************SJF\n\n");
//    SJF_sort();
    printf("RR_Run**************RR_Run\n\n");
    RR_Run();
	while(1){
		sleep(5);
		break;
	}
	return 0;
}
