#include"udp.h"
int sockfd;

int main(int argc,char**argv)
{
 if(argc<3)
 {
  printf("Usage:%s \n",argv[0]);
  exit(1);   
 }
if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
 {
  perror("socket error");
  exit(1);
 }
 struct sockaddr_in serveraddr;
 //clientaddr.sin_family=AF_INET;
 //clientaddr.sin_port=htons(atoi(argv[2]));
 //clientaddr.sin_addr.s_addr=INADDR_ANY;
 memset(&serveraddr,0,sizeof(serveraddr));
 serveraddr.sin_family=AF_INET;
 serveraddr.sin_port=htons(atoi(argv[2]));
 inet_pton(AF_INET,argv[1],(struct sockaddr*)&serveraddr.sin_addr.s_addr);
 
 char buffer[1024]="Hello server";
 if(sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
    perror("sendto error");
    exit(1);
  }
 else{
    //recive the message from server
    memset(buffer,0,sizeof(buffer));
    size_t size;
    if((recv(sockfd,buffer,sizeof(buffer),0))<0){
       perror("recv error");
       exit(1);
      }
    else{
       printf("%s\n",buffer);
      }
   }
  close(sockfd);
  return 0;
}
