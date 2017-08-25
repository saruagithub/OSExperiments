#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void *prim(){
    int n=100;
    printf("prim 2,3\n");
    for(int i=4;i<=n;i++){
        for(int j=2;j<i;j++)
            if(i%j==0)
                break;
            else if(i%j!=0&&j!=i-1)
                continue;
            else
                printf("prim:%d,",i);
    }
}

void *fibonacci(){
    int n=10;
    long int fib[n];  fib[0] = 0;  fib[1] = 1;
    for(int i=2; i<n; i++)
        fib[i] = fib[i-1] + fib[i-2];
    printf("\n");
    for(int i=0; i<n; i++)
        printf("  %ld,", fib[i]);
    printf("\n");
}

int main(int argc,char *argv[])
{
	pid_t pid1,pid2,pid3,pid4,pid5;
	pid1=fork();
	
    if(pid1==0)
	{
	printf("pid1 = %d\n", getpid());

	//child2 process
	pid2=fork(); 
	if(pid2<0)
	{
		fprintf(stderr,"Fork failed");
		exit(-1);		
	}
	else if(pid2==0){
        printf("\npid2=%d, Parent pid=%d\n",getpid(),getppid());
		execlp("/bin/ls","ls",NULL);
	}
	else{
		wait(NULL);
		printf("child2 complete!\n");
		//sleep(30);
		//exit(0);
	}

	//new child3
	pid3=fork();
	if(pid3<0)
	{
		fprintf(stderr,"Fork failed");
		exit(-1);		
	}
	else if(pid3==0){
        printf("pid3=%d,Parent=%d\n",getpid(),getppid());
		pid4 = fork();
		if (pid4 == 0)
		{
			printf("PID4 = %d, Parent PID = %d\n", getpid(), getppid());
            
            //new thread
            pthread_t ptd1,ptd2;
            pthread_create(&ptd1,NULL,prim,NULL);
            pthread_join(ptd1,NULL);
            pthread_create(&ptd2,NULL,fibonacci,NULL);
            pthread_join(ptd2,NULL);
			//sleep(10);
			exit(0);
		}
        else{
            wait(NULL);
            printf("child4 complete!\n");
        }
        
		pid5 = fork();
		if (pid5 == 0)
		{
			printf("PID5 = %d, Parent PID = %d\n", getpid(), getppid());
            execlp("./print",NULL);
			sleep(2);
			//exit(0);
		}
        else{
            wait(NULL);
            printf("child5 complete!\n");
            exit(0);
        }
    }
	else{
		wait(NULL);
		printf("child3 complete!\n");
		exit(0);
	}
	wait(NULL);
	exit(0);
	}
    else{
        wait(NULL);
        printf("pid1 complete!\n");
        //sleep(30);
        exit(0);
    }
    return 0;
}
	
