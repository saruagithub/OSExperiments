//
//  message1.c
//  
//
//  Created by wangxue on 16/11/12.
//
//
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>

//message queue
//typedef struct mymsg
//{
//    long mytype;
//    unsigned char mytext[128];
//}mymsg;

int main()
{
    pid_t pid0,pid1,pid2;
    FILE* fp;
    int msyID;
    //set parent
    pid0=fork();
    if(pid0==0)
    {
        printf("pid0 = %d\n", getpid());
        
        //pid0 set new queue
        int key = 1;
        int msyID=msgget((key_t)key,0660 | IPC_CREAT);
        printf("queue id is:%d\n",msyID);
        
        pid1=fork();//new pid1
        if(pid1==0){
            printf("pid1 = %d,parent=%d\n", getpid(),getppid());
            //write into file
            fp=NULL;
            fp=fopen("t2.txt","a+");
            if(fp==NULL){
                printf("Error opening\n");
                return -1;
            }
            char s0[5]={'1','2','3','4','5'};
            fwrite(&s0,sizeof(s0),1,fp);
            fclose(fp);
            fp=NULL;
            
            //write into message queue
            char new1[2]={'o','k'};
            msgsnd(msyID,&new1,sizeof(new1),IPC_NOWAIT);
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
            
            //read news from queue
            char getnew[2];
            msgrcv(msyID,&getnew,sizeof(getnew),0,IPC_NOWAIT);
            for(int i=0;i<2;i++){
                printf("%c ",getnew[i]);
            }
            //read file
            if((getnew[0]=='o')&&(getnew[1]=='k')){
                fp=fopen("t2.txt","r");
                if(fp==NULL){
                    printf("Error opening\n");
                    return -1;
                }
                char c;
                while(fscanf(fp,"%c",&c)!=EOF)
                    printf("%c ",c);
                printf("\n");
                fclose(fp);
            }
            else
                printf("Is not ok\n");
                exit(0);
            }
        else{
            wait(NULL);
            printf("pid2 complete!\n");
        }
    }//pid1&pid2 finish
    else{
        wait(NULL);
        printf("pid0 complete!\n");
    }
    return 0;
}
