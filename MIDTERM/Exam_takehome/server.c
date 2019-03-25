#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <time.h>   

#define PORT 9000 
#define SA struct sockaddr 
  
// commtion designed for chat between client and server. 
char* read_sensor_data()
{
    int sensor_data[5];
    char buffer[1000];
    int i=0;
    for(int i = 0; i < 5; i++)
    {
        sprintf(filename, "sensor%d.txt", i+1);
        FILE* fp = fopen(filename, "r");

        if(fp == NULL) 
        {
            printf("error in reading: %s\n", filename);
        }
        
        fscanf(fp, "%d", &(sensor_data[i]));

        fclose(fp);
    }
    sprintf(buffer, "%d,%d,%d,%d,%d", sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4]);
    return buffer;

}
void comm(int sockfd) 
{ 
    char buffer[1000]; 
    char sensor_info[1000];
    char filename[100];		
    // infinite loop for chat 
    while(1)
    {  
        read(sockfd, buffer, sizeof(buffer)); 
        sensor_info= read_sensor_data();
        write(sockfd, sensor_info, sizeof(buffer)); 
      	printf("sent to client: %s\n",buffer);

    } 
} 
  

int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // commtion for chatting between client and server 
    comm(connfd); 
  
    // After chatting close the socket 
    close(sockfd); 
} 

