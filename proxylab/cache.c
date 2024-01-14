#include "cache.h"

cache_t cache;

int rwtime;

void init_cache(){
    cache.used = 0;
    rwtime = 0;
    for(int i = 0; i < MAX_OBJECT_NUM ; i++){
        cache.cacheline[i] = Malloc(sizeof(cacheline_t));
        cache.cacheline[i] -> time = 0;
        cache.cacheline[i] -> objsize = 0;
        cache.cacheline[i] -> readcnt = 0;
        Sem_init(&cache.cacheline[i] -> mutex,0,1);
        Sem_init(&cache.cacheline[i] -> w,0,1);
    }    
}

void free_cache(){
    for(int i = 0; i < MAX_OBJECT_NUM ; i++){
        Free(cache.cacheline[i]);
    }
}

int reader(int fd, char* url){
    int ret = 0;
    for(int i = 0; i < MAX_OBJECT_NUM; i++){
        if(!strcmp(cache.cacheline[i]->url, url) ){
            P(&cache.cacheline[i]->mutex);
            cache.cacheline[i]->readcnt++;
            if(cache.cacheline[i]->readcnt == 1){
                P(&cache.cacheline[i]->w);
            }
            V(&cache.cacheline[i]->mutex);
            Rio_writen(fd, cache.cacheline[i]->object, cache.cacheline[i]->objsize);
            ret = 1;
            if(debug) printf("-------im reading on %d!\n",i);
            P(&cache.cacheline[i]->mutex);
            cache.cacheline[i]->readcnt--;
            if(cache.cacheline[i]->readcnt == 0){
                V(&cache.cacheline[i]->w);
            }
            V(&cache.cacheline[i]->mutex);
        }
        else{
            if(debug) printf("%s,,,,,%s\n",cache.cacheline[i]->url, url);
        }
    }
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
    
    rwtime++;
    if(cache.used >= MAX_OBJECT_NUM){
        int i = remove_index();
        P(&cache.cacheline[i]->w);
        if(debug) printf("......imwriting on%d!\n",i);
        strcpy(cache.cacheline[i]->url, url);
        strcpy(cache.cacheline[i]->object, buf);
        cache.cacheline[i]->time = rwtime;
        cache.cacheline[i]->objsize = size;
        V(&cache.cacheline[i]->w);
    }
    else{
        P(&cache.cacheline[cache.used]->w);
        strcpy(cache.cacheline[cache.used]->url, url);
        strcpy(cache.cacheline[cache.used]->object, buf);
        cache.cacheline[cache.used]->time = rwtime;
        cache.cacheline[cache.used]->objsize = size;
        V(&cache.cacheline[cache.used]->w);
        cache.used++;
        if(debug) printf("././././.imwriting on%d!\n",cache.used);
    }   
}

