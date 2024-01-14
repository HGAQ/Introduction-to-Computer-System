/*
刘松瑞 2100011819
A web proxy using Get method to handle the http requests
*/

#include <stdio.h>
#include "csapp.h"
#include "sbuf.h"
#include "cache.h"
/* Recommended max cache and object sizes */
#define SBUFSIZE 512
#define NTHREADS 16
#define DEBUG 0


/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = 
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = 
    "Connection: close\r\n";
static const char *proxy_connection_hdr = 
    "Proxy-Connection: close\r\n";

sbuf_t sbuf;
void doit(int fd);
void read_requesthdrs(rio_t *rp, char* server, 
    char* method, char* hostname, char* port, char* uri);
int parse_url(char* url, char* hostname, char* port, char* uri);
void* thread(void* vargp);

/*
- main
- the main function
*/
int main(int argc,char** argv)
{
    //write(STDOUT_FILENO,".............\n",sizeof(".............\n"));
    int listenfd, *connfdp; // define listen & connection socket description
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    char hostname[MAXLINE], port[MAXLINE];
    //check cmd line
    if (argc != 2){
        fprintf(stderr, "usage: %s <port>\n",argv[0]);
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);

    init_cache();

    listenfd = Open_listenfd(argv[1]);
    if (DEBUG) printf("listen successfully\n");

    //sbuf_init(&sbuf, SBUFSIZE);
    //for (i = 0; i < NTHREADS; i++){
    //    Pthread_create(&tid, NULL, thread, NULL);
    //}

    while(1){
        clientlen = sizeof(clientaddr);
        connfdp = (int *)malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, 
        hostname, MAXLINE, port, MAXLINE, 0);
        if (DEBUG) printf(
            "!!!Accepted connection from (%s %s).\n", hostname, port);
        //sbuf_insert(&sbuf, connfd);
        Pthread_create(&tid, NULL, thread, connfdp);
    }

    free_cache();
    return 0;
}
/*
- thread
- using thread to deal with cocurrent requests
*/
void* thread(void* vargp){
    int connfd = *((int *)vargp);
    free(vargp);
    Pthread_detach(pthread_self());
    doit(connfd);
    Close(connfd);
    return NULL;
}


/*
- doit
- recieve the msg from client and transmit msg to server, 
- and finally, return result to client again
-  return nothing
*/
void doit(int fd){
    char buf[MAXLINE], method[MAXLINE], url[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], port[MAXLINE], uri[MAXLINE];
    char server[MAXLINE];
    int n, serverfd;
    int totalsize = 0;
    char get[3*MAXLINE];
    char towrite[MAX_OBJECT_SIZE];
    rio_t rio, server_rio;
    //first, i'm the server

    //) rio preparation,get the request  
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE)){
        fprintf(stderr, "reedline error: %s\n", strerror(errno));
        return;
    }
    //) get the method, url, version
    if(sscanf(buf,"%s %s %s",method, url, version) != 3){
        fprintf(stderr, "parameter error: %s\n", strerror(errno));
        return;
    }
    if (DEBUG) printf("!!%s %s %s\n",method, url, version);
    //) chaeck if it is GET
    if (strcasecmp(method, "GET")){
        fprintf(stderr, "method error: %s\n", strerror(errno));
        return;
    }   
    //) check cache
    if(reader(fd, url)){
        if(DEBUG) printf("!! cache hits");
        return;
    }
    if (DEBUG) printf("!!Client connection succeed and GET\n");
    //) get hostname, port, uri
    parse_url(url, hostname, port, uri);
    //) get request headers
    read_requesthdrs(&rio, server, method, hostname, port, uri);
    if (DEBUG) printf("\n!!!\n%s\n!!!\n",server);
    
    // now, i'm the client
    // connect with server
    serverfd =  Open_clientfd(hostname,port);
    if(serverfd < 0){
        printf("server connection failed\n");
        return;
    }
    //rio preparation and send GET request to server
    Rio_readinitb(&server_rio, serverfd);
    sprintf(get, "GET %s HTTP/1.0\r\n%s", uri, server);
    //collect the response and send to cache and client
    if(rio_writen(serverfd, get, strlen(get))!=strlen(get)){
        fprintf(stderr, "riowriten error: %s\n", strerror(errno));
        return;
    }
    while((n = Rio_readlineb(&server_rio, buf, MAXLINE))){
        if(n == 0){
            break;
        }
        if(n == -1){
            fprintf(stderr, "readline error: %s\n", strerror(errno));
            return;
        }
        if (DEBUG) printf("!!get %d B from server\n", n);
        if(totalsize + n < MAX_OBJECT_SIZE){
            strcpy(towrite + totalsize, buf);
        }
        totalsize += n;
        rio_writen(fd, buf, n);
    }
    if(totalsize < MAX_OBJECT_SIZE){
        writer(towrite, url, totalsize);
    }
    Close(serverfd);
}




/*
- parse_uri
- translate uri into hostname, port and uri.
- return -1 for error, 0 for normal
*/
int parse_url(char* url, char* hostname, char* port, char* uri){
    // http://www.baidu.com:80/xx/xx?ab&cd
    char* pos_http = strstr(url,"http://");
    //there is no http:// in uri
    if (pos_http != url ){
        // www.baidu.com:80/xx/xx?ab&cd
        return -1;
    }
    char *pos_web = strstr(url,"//") + 2;
    char *pos_port = strstr(pos_web, ":");
    char *pos_slash = strstr(pos_web, "/");
    //there is no port pos
    if (pos_port == NULL)
    {
        strcpy(port, "80");
        strcpy(uri, pos_slash);
        // http://www.baidu.com/xx/xx?ab&cd
        *pos_slash = '\0';
        // hostname: www.baidu.com
        strcpy(hostname, pos_web);
        *pos_slash = '/';
    }
    else
    {
        // http://www.baidu.com:80/xx/xx?ab&cd
        //        |            |  |  
        // uri: xx/xx?ab&cd
        strcpy(uri, pos_slash);
        *pos_slash = '\0';
        strcpy(port, pos_port + 1);
        *pos_port = '\0';                
        strcpy(hostname, pos_web);
        *pos_slash = '/';
        *pos_port = ':';   

    }
    if (DEBUG) printf("!!%s,,%s,,%s\n",hostname,port,uri);
    return 0;
}

/*
- read_requesthdrs
- read the headers from the clients, just ignore
- return void
*/
void read_requesthdrs(rio_t *rp, char* server, 
char* method, char* hostname, char* port, char* uri){
    char buf[MAXLINE];
    Rio_readlineb(rp, buf, MAXLINE);
    while(Rio_readlineb(rp, buf, MAXLINE) > 0){
        if (!strcmp(buf,"\r\n")){
            break;
        }
        if (
            strstr(buf,"Host:") != NULL || 
            strstr(buf,"User-Agent:") != NULL ||
            strstr(buf,"Connection:") != NULL || 
            strstr(buf,"Proxy-Connection:") != NULL
        ){
            continue;
        }
        strcat(server, buf);
    }
    strcat(server, "Host: ");
    strcat(server, hostname);
    strcat(server, ":");
    strcat(server, port);
    strcat(server,"\r\n");
    strcat(server, user_agent_hdr);
    strcat(server, connection_hdr);
    strcat(server, proxy_connection_hdr);
    strcat(server,"\r\n");
    return;
}

