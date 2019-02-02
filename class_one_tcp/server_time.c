#include"server.h"

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
    long t=time(0);
    char *s=ctime(&t);
    size_t size=strlen(s)*sizeof(char);
    if(write(fd,s,size)!=size)perror("write error");
    
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
  while(1){
       int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);
       if(fd<0){
           perror("accept error");
           continue;
         }
  /*step5 :use I/O function(write/read) to contact each other*/
     out_addr(&clientaddr);
     do_service(fd);
  /*step 6:close socket fd not sockfd*/
    close(fd);
         }
   
   
   
    
  return 0;

}
