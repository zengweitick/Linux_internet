#ifndef __MSG_H__
#define __MSG_H__
#include<sys/types.h>
typedef struct{
    char head[10];  //for head
    char checknum;//for check
    char buff[512];//for data

 }Msg;

/*send message based on custom protocol and the message put in buff*/   
extern int write_msg(int sockfd,char*buff,size_t len);
/*read a message based on custom protocol and mesage readed in buff*/
extern int read_msg(int sockfd,char*buff,size_t len);




#endif
