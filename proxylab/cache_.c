#include "cache.h"


cache_t cache;
sem_t mutex;
sem_t w;  

int readcnt, rwtime;

void init_cache(){
    Sem_init(&mutex,0,1);
    Sem_init(&w,0,1);
    cache.used = 0;
    readcnt = 0;
    rwtime = 0;
    for(int i = 0; i < MAX_OBJECT_NUM ; i++){
        cache.cacheline[i] = Malloc(sizeof(cacheline_t));
        cache.cacheline[i] -> time = 0;
        cache.cacheline[i] -> objsize = 0;
    }    
}


void free_cache(){
    for(int i = 0; i < MAX_OBJECT_NUM ; i++){
        Free(cache.cacheline[i]);
    }
}


int reader(int fd, char* url){
    int ret = 0;

    P(&mutex);
    readcnt++;
    if(readcnt == 1){
        P(&w);
    }
    V(&mutex);

    for(int i = 0; i < MAX_OBJECT_NUM; i++){
        if(!strcmp(cache.cacheline[i]->url, url) ){
            if(debug) printf("-------im reading on %d,wants to write on %d!\n",i,fd);
            Rio_writen(fd, cache.cacheline[i]->object, cache.cacheline[i]->objsize);
            ret = 1;
            rwtime++;
            break;
        }
        else{
            if(debug) printf("%s,,,,,%s\n",cache.cacheline[i]->url,url);
        }
    }

    P(&mutex);
    readcnt--;
    if(readcnt == 0){
        V(&w);
    }
    V(&mutex);

    return ret;
}

int remove_index(){
    int mintime = 0x7f7f7f7f;
    int index = -1;
    for(int i = 0; i < MAX_OBJECT_NUM; i++){
        if(cache.cacheline[i]->time < mintime){
            mintime = cache.cacheline[i]->time;
            index = i;
        }
    }
    return index;
}


void writer(char* buf, char* url, int size){
    P(&w);
    rwtime++;
    if(cache.used >= MAX_OBJECT_NUM){
        int i = remove_index();
        if(debug) printf("imwriting on%d!\n",i);
        strcpy(cache.cacheline[i]->url, url);
        strcpy(cache.cacheline[i]->object, buf);
        cache.cacheline[i]->time = rwtime;
        cache.cacheline[i]->objsize = size;
    }
    else{
        strcpy(cache.cacheline[cache.used]->url, url);
        strcpy(cache.cacheline[cache.used]->object, buf);
        cache.cacheline[cache.used]->time = rwtime;
        cache.cacheline[cache.used]->objsize = size;
        cache.used++;
        if(debug) printf("imwriting on%d!\n",cache.used);
    }   
    V(&w);
}
