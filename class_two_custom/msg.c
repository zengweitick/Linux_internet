#include"msg.h"
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<memory.h>
static unsigned char msg_check(Msg * message)
{
 unsigned char s=0;
 int i;
 for(i=0;i<sizeof(message->head);i++)s+=message->head[i];
 for(i=0;i<sizeof(message->buff);i++)s+=message->buff[i];
 return s;


}
int write_msg(int sockfd,char*buff,size_t len){
   Msg message;
   memset(&message,0,sizeof(message));
   strcpy(message.head,"linux");
   memcpy(message.buff,buff,len);
   message.checknum=msg_check(&message);    
   if(write(sockfd,&message,sizeof(message))!=sizeof(message)){
      return -1;
    }
}
int read_msg(int sockfd,char*buff,size_t len){
     Msg message;
     memset(&message,0,sizeof(message));
     size_t size;
     if((size=read(sockfd,&message,sizeof(message)))<0){
       return -1;
      }
     else if(size==0){
       return 0;
       }
  unsigned char s=msg_check(&message);
//check the received message if complete,and check the check
  if((s==(unsigned char)message.checknum)&&(!strcmp("linux",message.head))){
    memcpy(buff,message.buff,len);
    
    return sizeof(message);
}

    return -1;
 }




