#include"server.h"
#include"msg.h"
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
     char buff[512];
     size_t size;
     char *prompt=">";
     while(1){
     memset(buff,0,sizeof(buff));
     write(STDOUT_FILENO,prompt,1);
     size=read(STDIN_FILENO,buff,sizeof(buff));
     if(size<0)continue;
     buff[size-1]='\0';
     if(write_msg(sockfd,buff,sizeof(buff))<0){
      perror("write msg error");
      continue;
     }
     else {
      if(read_msg(sockfd,buff,sizeof(buff))<0){
      perror("read msg error");
      continue;
      }
     else{
       printf("%s\n",buff);
     }
     }

     }  
     close(sockfd);

     return 0;
}
