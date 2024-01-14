/*
刘松瑞 2100011819
本代码模拟了缓存的进出
本simulator读入参数v实现verbose的输出
也实现了help指令
*/
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "cachelab.h"
/*---------------结构体---------------*/
//缓存行结构体
struct cacheline{
    int valid_bit;
    int recent_ref;//最近一次引用的行数
    int tag;
    int set;
};
//缓存信息结构体
struct cacheinfo{
    int num_set;
    int num_line;
    int num_bit;
} cacheinfo;
//打印缓存行结构体。未使用，用于检测输入
void printinfo(struct cacheinfo cach){
     printf("%d %d %d\n",cach.num_set,cach.num_line,cach.num_bit);
 }
//实现帮助
void usage()
{
    printf("Options:\n");
    printf("\t-h         Print this help message.\n");
    printf("\t-v         Optional verbose flag.\n");
    printf("\t-s <num>   Number of set index bits.\n");
    printf("\t-E <num>   Number of lines per set.\n");
    printf("\t-b <num>   Number of block offset bits.\n");
    printf("\t-t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("\tlinux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("\tlinux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}
/*---------------全局变量---------------*/
struct cacheline** Mycache;//模拟缓存
struct cacheinfo cache;//存储信息
int accumulate[3];//记录hit, miss, eviction
int v = 0;
int u = 0;
FILE *task;
/*---------------缓存信息---------------*/
void make_cacheinfo(int argc, char **argv){
    int opt;
    while((opt = getopt(argc,argv,"hvs:E:b:t:"))!=-1){
        switch(opt){
            case 'h':
                u = 1;
                break;
            case 'v':
                v = 1;
                break;
            case 's':
                cache.num_set = atoi(optarg);
                break;
            case 'E':
                cache.num_line = atoi(optarg);
                break;
            case 'b':
                cache.num_bit = atoi(optarg);
                break;
            case 't':
                task = fopen(optarg,"r");
                break;
            case ':':
                break;
        }
    }
    //printinfo(cache);
}
/*---------------初始化缓存---------------*/
void build_cache(){
    if(u) return;
    Mycache = (struct cacheline**)malloc(sizeof(struct cacheline*) * (0x1<<cache.num_set));
    for(int i = 0 ; i < (0x1<<cache.num_set) ; i++){
        Mycache[i] = (struct cacheline*)malloc(sizeof(struct cacheline) * cache.num_line);
        for(int j = 0; j < cache.num_line ; j++){
            Mycache[i][j].valid_bit = 0;
            Mycache[i][j].set = i;
            Mycache[i][j].tag = -1;
        }
    }
}
/*---------------不同缓存操作---------------*/
//不命中需要更新
int update(long unsigned address,int length,int line_num){
    int s = (int)((address >> cache.num_bit) & ((0x1 << cache.num_set) - 1));
    int t = (int)(address >> (cache.num_bit + cache.num_set));
    int min_ref = 0x7f7f7f7f; 
    int min_pos = 0;
    for (int j = 0; j < cache.num_line ; j++){
        if (Mycache[s][j].tag == -1){
            Mycache[s][j].recent_ref = line_num;
            Mycache[s][j].valid_bit = 1;
            Mycache[s][j].set = s;
            Mycache[s][j].tag = t;
            if (v) printf(" miss,stored in set %d",s);
            accumulate[1]++;
            return 1;
        }
        else if(min_ref > Mycache[s][j].recent_ref){
            min_ref = Mycache[s][j].recent_ref;
            min_pos = j;
        }
    }
    Mycache[s][min_pos].recent_ref = line_num;
    Mycache[s][min_pos].valid_bit = 1;
    Mycache[s][min_pos].set = s;
    Mycache[s][min_pos].tag = t;
    if (v) printf(" miss eviction,stored in set %d",s);
    accumulate[1]++;
    accumulate[2]++;
    return 2;
}
//L操作
int load(long unsigned address,int length,int line_num){
    int s = (int)((address >> cache.num_bit) & ((0x1 << cache.num_set) - 1));
    int t = (int)(address >> (cache.num_bit + cache.num_set));
    for (int j = 0; j < cache.num_line ; j++){
        if (Mycache[s][j].valid_bit == 1 && Mycache[s][j].tag == t){
            Mycache[s][j].recent_ref = line_num;
            if (v) printf(" hit,stored in set %d",s);
            accumulate[0]++;
            return 0;
        }
    }
    return update(address,length,line_num);
}
//S操作
int store(long unsigned address,int length,int line_num){
    int s = (int)((address >> cache.num_bit) & ((0x1 << cache.num_set) - 1));
    int t = (int)(address >> (cache.num_bit + cache.num_set));
    for (int j = 0; j < cache.num_line ; j++){
        if (Mycache[s][j].valid_bit == 1 && Mycache[s][j].tag == t){
            Mycache[s][j].recent_ref = line_num;
            if (v) printf(" hit,stored in set %d",s);
            accumulate[0]++;
            return 0;
        }
    }
    return update(address,length,line_num);
}
//M操作
int modify(long unsigned address,int length,int line_num){
    load(address,length,line_num);
    store(address,length,line_num);
    return 0;
}
/*---------------处理命令---------------*/
void handle_task(){
    if(u) return;
    char operation[2];
    long unsigned address;
    int length;
    int line_num = 0;
    while(fscanf(task,"%s %lx,%d",operation,&address,&length) == 3){
        line_num++;
        switch(operation[0]){
            case 'I'://忽略I
                break;
            case 'M':
                if (v) printf("M %lx,%d",address,length);
                modify(address,length,line_num);
                break;
            case 'L':
                if (v) printf("L %lx,%d",address,length);
                load(address,length,line_num);
                break;
            case 'S':
                if (v) printf("S %lx,%d",address,length);
                store(address,length,line_num);
                break;
        }
        if (v) printf("\n");
    }
}
/*---------------主函数---------------*/
int main(int argc, char **argv){
    accumulate[0]=0;
    accumulate[1]=0;
    accumulate[2]=0;
    make_cacheinfo(argc,argv);
    if(u){ usage(); return 0;}
    build_cache();
    handle_task();
    printSummary(accumulate[0], accumulate[1], accumulate[2]);
    return 0;
}