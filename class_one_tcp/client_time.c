#include"server.h"
int main(int argc,char **argv){
    if(argc<3){
       printf("Usage:%s ip port \n",argv[0]);
       exit(1);
       }
     /*step1 create socket*/
     int sockfd=socket(AF_INET,SOCK_STREAM,0);
     if(sockfd<0){
       perror("socket error");
        exit(1);
          }
     /*step2 connect*/
      struct sockaddr_in clientaddr;
      memset(&clientaddr,0,sizeof(clientaddr));
      clientaddr.sin_family=AF_INET;
      clientaddr.sin_port=htons(atoi(argv[2]));
     // this is diferent from server ,we should point special ip 
     inet_pton(AF_INET,argv[1],&clientaddr.sin_addr.s_addr);
     if(connect(sockfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0){
          perror("connect error");
          exit(0);
         }
      /*use I/O function to read and write socket*/
      char buffer[1024];
      memset(buffer,0,sizeof(buffer));
      size_t size;
      if((size=read(sockfd,buffer,sizeof(buffer)))<0){
            perror("read error");
            exit(1);
      }
      if(write(STDOUT_FILENO,buffer,size)!=size){
       perror("write error");
       exit(1);
       }
       close(sockfd);

     return 0;
}
