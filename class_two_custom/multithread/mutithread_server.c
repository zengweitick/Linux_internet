#include"server.h"
#include"msg.h"
/*service work*/
void do_service(int fd){
   char buff[512];
   /*communicate with client each other*/ 
  while(1){
   memset(&buff,0,sizeof(buff));
   printf("Start read and write......\n");
   size_t size;
   if((size=read_msg(fd,buff,sizeof(buff)))<0){
     perror("read error");
     break;
    }
   else if(size==0)break;
   else {
      printf("read string:%s\n",buff);
      if(write_msg(fd,buff,sizeof(buff))<0){
        if(errno==EPIPE)break;
        perror("write error");
       }
    }
  
           } 
}
int sockfd;
/*signal*/
void sig_handler(int sig){
  if(sig==SIGINT){
    printf("server close\n");
    close(sockfd);
    exit(1);
 }
}
void out_fd(int fd){
 struct sockaddr_in addr;
 socklen_t len=sizeof(addr);
// use getpeername to get client'information
if(getpeername(fd,(struct sockaddr*)&addr,&len)<0){
  perror("getpeername error");
  exit(1) ;
}
char ip[16];
memset(ip,0,sizeof(ip));
int port=ntohs(addr.sin_port);
inet_ntop(AF_INET,&addr.sin_addr.s_addr,ip,sizeof(ip));
printf("%16s(%5d) closed!\n",ip,port); 
}
void *th_fn(void *arg){
  int fd=*(int*)arg;
  do_service(fd);
  out_fd(fd);
  close(fd);
 return (void*)0;
   }

int main(int argc, char **argv){
      if(argc<2){
       printf("argc error\n");
       exit(1);
       }
     if(signal(SIGINT,sig_handler)==SIG_ERR){
      perror("signal sigint error");
       exit(1);
      }
   /*step1 create socket*/
    sockfd=socket(AF_INET,SOCK_STREAM,0);
   /*step2 bind socket with address(include ip and port) */
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(atoi(argv[1]));
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
       perror("bind error");
       exit(1);
   }
   /*step3 listen( we can point special port but not here) and tell system some connect_request from client,and put the request into queue(sockfd) size =10*/
    if(listen(sockfd,10)<0){
     perror("listen erro");
     exit(1);
    }
  /*step4 accept :get a connect from client in queue and return new sockfd(not equel with sockfd ) if no connect request in queue ,the interrap until connect request come up*/
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len=sizeof(clientaddr);
  /*set pthread separate character not to use pthread_join(just beacuse it can cause main thread interrupt and can't get accepts from queue)*/
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);  
  while(1){
       int fd=accept(sockfd,NULL,NULL);
       if(fd<0){
           perror("accept error");
           continue;
         }
  /*step5 :create childthread to use I/O function(write/read) to contact each other*/
       pthread_t th;
       int err;
//create pthread with the separated state
       if((err=pthread_create(&th,&attr,th_fn,&fd))!=0){
              perror("pthread create error");
         }
       pthread_attr_destroy(&attr);
   }
   
   
   
    
  return 0;

}
