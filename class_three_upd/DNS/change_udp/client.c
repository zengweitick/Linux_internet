#include"udp.h"
int sockfd;
int is_host(struct hostent *host,char *name){
 if(!strcmp(host->h_name,name))return 1;
 int i=0;
 while(host->h_aliases[i] != NULL){
    if(!strcmp(host->h_aliases[i++],name))return 1;
  }
 return 0;
}
unsigned int get_ip_by_name(char *name){
 unsigned int ip=0;
 struct hostent *host;
 while((host=gethostent())!=NULL){
   if(is_host(host,name)){
     memcpy(&ip,host->h_addr_list[0],4);
     break;
   }
  }
 endhostent();
 return ip;


}
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
 memset(&serveraddr,0,sizeof(serveraddr));
 serveraddr.sin_family=AF_INET;
 serveraddr.sin_port=htons(atoi(argv[2]));
 //inet_pton(AF_INET,argv[1],(struct sockaddr*)&serveraddr.sin_addr.s_addr);
 unsigned int ip=get_ip_by_name(argv[1]);
 if(ip !=0){
    serveraddr.sin_addr.s_addr=ip;
  } else{
    inet_pton(AF_INET, argv[1],&serveraddr.sin_addr.s_addr);
  }
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
