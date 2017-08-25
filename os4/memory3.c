//
//  memory.c
//  
//
//  Created by wangxue on 16/12/15.
//
//
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#define N 1000  		 //page frame number
#define EMPTY -1  	//page frame is NULL

struct  memory{
    int page;
    int reference;
    struct memory *next;
}memory;
struct memory  page_frames[N];   //page frames
struct memory *p;	//remember the one to be replaced
int PageFault_num = 0;	//count the page fault number

void init()	//circle link
{
	int i=0;
	p = &page_frames[i]; 
	while(p)
	{
		p->page = EMPTY;
		//printf("%d\n",p->page);
		p->reference = 0;
		p->next = &page_frames[++i];
		if(i == N)
			break;
		p=p->next;
	}
	page_frames[N-1].next = &page_frames[0];
}
int whetherInMemory(int n)
{
	int i;
	for(i=0;i<N;i++){
		if(n == page_frames[i].page){
			page_frames[i].reference = 1;
			//printf("%d is in the page_frames[%d]\n",n,i);
			return 1;
		}
	}
	return 0;	// not in memory
}
int whetherHaveEmpty(int n)
{
	int i;
	for(i=0;i<N;i++)
		if(page_frames[i].page == EMPTY){
			page_frames[i].page = n;
			page_frames[i].reference = 1;
			PageFault_num++;
			//printf("%d is put into page_frames[%d]\n",n,i);
			return 1;
	}
	return 0;
}
void CLOCK(int n)
{
	while(p){
		if(p->reference == 0){
			p->page = n;
			p->reference = 1;
			p=p->next;
			break;
		}
		p->reference = 0;
		p=p->next;
	}
}

int main()
{
    init();	//init  whole memory 
    FILE *fp;
    if((fp=fopen("workload7","r"))==NULL)
    {
        printf("open file error!");
        return -1;
    }  
    int n,i=0,j=0;  	//read numbers from files into n
    p = &page_frames[0];	//the first one in memory  for clock
    while (fscanf(fp,"%d",&n)!= EOF)    //read from file,run FIFO algorithm
    {
    	j++;
    	if(whetherInMemory(n)==1)
    		continue;
  	if((i<N)&&(whetherHaveEmpty(n)==1)){
  		i++;
  		continue;
  	}
    	CLOCK(n);
    	PageFault_num++;
    }
    // for(i=0;i<N;i++){
    // 	printf("page %d is %d\n", i,page_frames[i].page);
    // }
    float Rate = (float)PageFault_num/(float)j;
    printf("PageFault_num is %d,Rate is %f\n",PageFault_num,Rate);
    return 0;
}
