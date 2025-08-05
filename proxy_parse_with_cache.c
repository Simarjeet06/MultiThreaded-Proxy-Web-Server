#include "proxy_parse.h"
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>
 


#define MAX_CLIENTS 10

typedef struct cached_element cached_element;
struct cached_element{
    char* data;
    int len;
    char* url;
    time_t lru_time_track;
    cached_element* next;
};

cached_element find(char* url);
int add_cached_element( char* data, int size,char* url);
void remove_cached_element(char* url);

int port_number=8080;
int proxy_socketId;
pthread_t tid[MAX_CLIENTS];
sem_t semaphore;
pthread_mutex_t lock;

cached_element* head;
int cache_size;

int main(int argc,char* argv[]){
    int client_socketId,client_len;
    struct sockaddr_in server_addr,client_addr;
    sem_init(&semaphore, 0,MAX_CLIENTS);
    pthread_mutex_init(&lock, NULL);
    if(argv==2){
        port_number=atoi(argv[1]);
    }else{
        printf("too few arguments\n");
        exit(1);
    }
    printf("Proxy server started on port %d\n", port_number);
    proxy_socketId = socket(AF_INET, SOCK_STREAM, 0);
    if(proxy_socketId < 0){
        perror("Error creating socket");
        exit(1);        
    }
    int reuse=1;
    if(setsockopt(proxy_socketId, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        perror("setsockopt failed\n");
    }
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_number);
    if(bind(proxy_socketId, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("port is not available\n");
        exit(1);
    }
    printf("Binding on port %d\n", port_number);
    int listen_status = listen(proxy_socketId, MAX_CLIENTS);
    if(listen_status < 0){
        perror("Error listening on socket");
        exit(1);
    }
    int i=0;
    int Connected_socketId[MAX_CLIENTS];
    while(1){
        bzero((char*)&client_addr, sizeof(client_addr));
        client_len = sizeof(client_addr);
        client_socketId = accept(proxy_socketId, (struct sockaddr*)&client_addr,(socklen_t*)&client_len);
        if(client_socketId < 0){
            perror("Not able to accept client connection");
            exit(1);
        }
       else{
        Connected_socketId[i] = client_socketId;
       }


}
}

