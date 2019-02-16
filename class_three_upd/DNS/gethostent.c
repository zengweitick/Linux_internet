#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<memory.h>
#include<string.h>
void out_addr(struct hostent* h){
 printf("hostname: %s\n",h->h_name);
 printf("hosttype: %s\n",h->h_addrtype == AF_INET ? "IPV4": "IPV6");
 char ip[16];
 memset(ip,0,sizeof(ip));
 inet_ntop(h->h_addrtype,h->h_addr_list[0],ip,sizeof(ip));
 printf("ip address :%s\n",ip);

}
int main(int argc,char **argv){
 if(argc<2){
  fprintf(stderr,"Usage :%s",argv[0]);
  exit(1);
 }
 struct hostent *h;
 while((h=gethostent())!=NULL){
  if(!strcmp(argv[1],h->h_name)){
   out_addr(h);
   exit(0);
  }
  else {
  int i=0;
  while(h->h_aliases[i]!=NULL){
   if(!strcmp(argv[1],h->h_aliases[i++])){
     out_addr(h);
     exit(0);
   }
   }
 }
 }
 endhostent();
 printf("no %s exist\n",argv[1]);
 return 0;
}
