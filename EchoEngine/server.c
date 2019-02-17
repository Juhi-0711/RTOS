#include<unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct msg_buffer { 
  int qid;
    long msg_type; 
    char msg_text[1024]; 
}; 

int main(void)
{
  struct msg_buffer msg_rcv, msg_snd;
  key_t key;
  int myQid;
  //char dataToBeRead[1024];
  if((key= ftok("/Users/yashpal/Desktop/Sem8/RTOS/EchoServer/server.c", 65))==-1) //generating unique key
  {
    perror("ftok");
    exit(1);
  }
  if((myQid = msgget(key, 0666 | IPC_CREAT))==-1) //creating the common server queue
  {
    perror("msgget");
    exit(1);
  }
  printf("SERVER ID %d\n", myQid);
  while(1)
  {
  msgrcv(myQid, &msg_rcv, sizeof(struct msg_buffer), 0, 0); 

  printf("Got: %s\n", msg_rcv.msg_text); //got message
  

  if(msg_rcv.msg_text[0]<='z' && msg_rcv.msg_text[0]>='a') //Change capitalisation
      msg_rcv.msg_text[0]= (char)(msg_rcv.msg_text[0]-32);
    msg_snd=msg_rcv;

    printf("Sending %s\n", msg_snd.msg_text); 
    int client_id=msg_rcv.qid;

    if(msgsnd(client_id, &msg_snd, sizeof(struct msg_buffer),0)==-1) //reply to the client through the message queue with id= client_ID (as specified in the incoming request)
    {
      perror("msgsnd");
      exit(1);
    }
    
    printf("Sent\n");
  }



}
