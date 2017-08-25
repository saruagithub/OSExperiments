//
//  message1.c
//  
//
//  Created by wangxue on 16/11/1.
//
//
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    pid_t pid0,pid1,pid2;
    pid0=fork();
    if(pid0==0)
    {
        printf("pid0 = %d\n", getpid());
        //pid0 set pipe
        int fd[2];
        int pipe0=pipe(fd);
        
        pid1=fork();//new pid1
        if(pid1==0){
            printf("pid1 = %d,parent=%d\n", getpid(),getppid());
            //write into file
            FILE* fp=NULL;
            fp=fopen("t1.txt","a+");
            if(fp==NULL){
                printf("Error opening\n");
                return -1;
            }
            char s0[5]={'1','2','3','4','5'};
            fwrite(&s0,sizeof(s0),1,fp);
            fclose(fp);
            fp=NULL;
            
            //write into pipe
            char buf1[2]={'o','k'};
            int w=write(fd[1],&buf1,2);
            exit(0);
        }
        else{
            wait(NULL);
            printf("pid1 complete!\n");
        }
        
        
        // new process p2
        pid2 = fork();
        if(pid2==0){
            printf("pid2 = %d,parent=%d\n", getpid(),getppid());
            char buf0[2];
            read(fd[0],&buf0,2);
            for(int i=0;i<2;i++){
                printf("%c ",buf0[i]);
            }
            //read file
            if((buf0[0]=='o')&&(buf0[1]=='k')){
                FILE *fpp;
                fpp=fopen("t1.txt","r");
                if(fpp==NULL){
                    printf("Error opening\n");
                    return -1;
                }
                char c;
                while(fscanf(fpp,"%c",&c)!=EOF)
                    printf("%c ",c);
                printf("\n");
                fclose(fpp);
            }
            else
                printf("Is not ok\n");
             exit(0);
            }
        else{
            wait(NULL);
            printf("pid2 complete!\n");
        }
    }//pid1&pid2
    else{
        wait(NULL);
        printf("pid0 complete!\n");
    }
    return 0;
}
