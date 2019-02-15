#include"server.h"
#include"msg.h"
/*output the connected client's message*/
void out_addr(struct sockaddr_in *clientaddr){
  int port=ntohs(clientaddr->sin_port);
  char ip[16];
  memset(ip,0,sizeof(ip));
  inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
  printf("client :%s(%d) connected\n",ip,port);
}
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
  if(sig==SIGCHLD){
     printf("child dead ....\n");
     wait(0);
   }


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
     if(signal(SIGCHLD,sig_handler)==SIG_ERR){
      perror("signal sigchild error");
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
  while(1){
       int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);
       if(fd<0){
           perror("accept error");
           continue;
         }
  /*step5 :fork childprocess to use I/O function(write/read) to contact each other*/
     pid_t pid=fork();
     if(pid<0){
       perror("fork error");
       continue;
     }
     else if(pid==0){
     out_addr(&clientaddr);
     do_service(fd);
     close(fd);
     break;
     }
   else{
  /*step 6:close socket fd not sockfd*/
    close(fd);
   }
         }
   
   
   
    
  return 0;

}
