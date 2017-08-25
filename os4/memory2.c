//
//  memory.c
//  
//
//  Created by wangxue on 16/12/6.
//
//
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#define N 1000 		 //page frame number
#define EMPTY -1  	//page frame is NULL

struct  memory{
    int page;
    //int reference;
    struct memory *next;
    struct  memory *pre;
}memory;

struct memory  page_frames[N];   //page frames
struct  memory *head; 	//head pointer
struct  memory *tail;	//tail pointer
int PageFault_num = 0;	//count the page fault number

void init()
{
	int i=1;
	struct memory *p;
	page_frames[0].page = EMPTY;
	//page_frames[0].reference==0;
	page_frames[0].next = &page_frames[1];
	page_frames[0].pre = NULL;
	head = &page_frames[0];

	p = head; 
	while(p)
	{
		p=p->next;
		if(i == N)
		 	break;
		p->page = EMPTY;
		//printf("%d\n",p->page);
		//p->reference = 0;
		p->pre = &page_frames[i-1];
		p->next = &page_frames[++i];
	}
	tail = &page_frames[N-1];
	page_frames[N-1].next = NULL;
	page_frames[N-1].pre = &page_frames[N-2];
}
int whetherInMemory_LRU(int n,int i)
{
	struct memory *p;
	p=head;
	while(p){
		if(n==p->page){
//			printf("i is %d\n", i);
//			printf("%d is in memory\n",n);
			if(i<N){	//not full but n is in the memory
				struct memory *pi;
				pi=head;
				while(pi){
					if(pi->page == EMPTY)
					{
						if(pi->pre->page == (p->page))	//if n is in the last,there is no need to replace
                            return 1;
							//break;
						else{
							if(p==head){	//n is the same with head
								head = head->next;
								head->pre = NULL;
								p->pre = pi->pre;
								pi->pre->next = p;
								//p->pre = (pi->pre);
								p->next = pi;
								pi->pre = p;
                                return 1;
								//break;
							}
							else{
								p->pre->next = p->next;
								p->next->pre = p->pre;
								pi->pre->next = p;
								p->pre = pi->pre;
								p->next = pi;
								pi->pre = p;
                                return 1;
								//break;
							}
						}
					}
					pi=pi->next;
				}
			}
			else	//mempry is full,only need to change the place
			{
				if(p == head){
					head = p->next;
					head->pre = NULL;
					tail->next = p;
					p->pre=tail;
					p->next = NULL;
					tail = p;
                    return 1;
				}
                if(p == tail){
                    return 1;
                }
				p->pre->next = p->next;
				p->next->pre = p->pre;
				tail->next = p;
				p->pre = tail;
				p->next = NULL;
				tail = p;
                return 1;
			}
		}
		p=p->next;
	}
	//for print
//	p=head;
//	while(p){
//		printf("********page is %d\n",p->page);
//		p=p->next;
//	}
	return 0;
}

int whetherHaveEmpty(int n)
{
	int i;
	struct memory *p;
	p=head;
	while(p){
		if(p->page == EMPTY){
			p->page = n;
			//p->reference = 1;
			//printf("%d is put into memory\n",n );
			PageFault_num++;
			return 1;
		}
		p=p->next;
	}
	return 0;
}

void LRU(int n)
{
	struct memory *p;
	head->page = n;
	//printf("%d，%d\n",n,head->page);
	p=head->next;
	head->next = NULL;
	head->pre = tail;
	tail->next = head;
    tail = head;
	head = p;
	//printf("%d\n",head->page);
	head->pre = NULL;
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
    while (fscanf(fp,"%d",&n)!= EOF)    //read from file,run FIFO algorithm
    {
    	j++;
	if(whetherInMemory_LRU(n,i)==1)
    		continue;
  	if((i<N)&&(whetherHaveEmpty(n)==1)){
  		i++;
  		continue;
  	}
    	LRU(n);
    	PageFault_num++;
    }
//    struct memory *p;
//    p=head;
//	while(p){
//		printf("page is %d\n",p->page);
//		p=p->next;
//	}
   float Rate = (float)PageFault_num/(float)j;
    printf("PageFault_num is %d,Rate is %f\n",PageFault_num,Rate);
    return 0;
}
