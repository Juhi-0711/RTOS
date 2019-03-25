#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <time.h>   
#include <arpa/inet.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr

// write the sensor data along with timestamp
void write_to_file(char* fname, char* data) 
{
	FILE* fp = fopen(fname, "a");

	if(fp == NULL) {
		printf("ERROR\n");
	}

	fprintf(fp, "%lu,%s\n", (unsigned long)time(NULL), data);

	fclose(fp);
}

// calls server at regular intervals for data 
void comm(int sockfd) 
{ 
    char buffer[10]="get"; 
    char sensor_data[1000];	
    while(1) 
    { 
        write(sockfd, buffer, sizeof(buffer)); 
        read(sockfd, msg, sizeof(sensor_data)); 
        printf("From Server : %s\n", sensor_data); 
        write_to_file("./sensor_data", sensor_data);
	    sleep(2);//sync with the sensor data updating frequency 
    } 
} 
  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client to server 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
 
    comm(sockfd); 
  
    // close the socket 
    close(sockfd); 
} 
