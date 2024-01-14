#include <stdio.h>
#include "csapp.h"
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 51200
#define MAX_OBJECT_NUM 20
#define debug 0
typedef struct
{
    char url[MAXLINE];          
    char object[MAX_OBJECT_SIZE];  
    int objsize; 
    int time;     
    sem_t mutex;
    sem_t w;      
    int readcnt;
}cacheline_t;

typedef struct{
    cacheline_t* cacheline[MAX_OBJECT_NUM];
    int used;
} cache_t;

void init_cache();
void free_cache();
int reader(int fd, char* url);
void writer(char* buf, char* url, int size);
int remove_index();