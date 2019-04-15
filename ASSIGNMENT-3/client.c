#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <time.h>   
#include <arpa/inet.h>
#define MAX 80 
#define PORT 8000
#define SA struct sockaddr

// write the sensor data along with timestamp
/*void write_to_file(char* fname, char* data) 
{
	FILE* fp = fopen(fname, "a");

	if(fp == NULL) {
		printf("ERROR\n");
	}

	fprintf(fp, "%lu,%s\n", (unsigned long)time(NULL), data);

	fclose(fp);
}*/

// calls server at regular intervals for data 
void comm(int sockfd) 
{ 
    char buffer[1000]; 
    char msg[1000];	
    while(1) 
    { 

        printf("Type message to sensor...\n");
        scanf("%s",msg);
        write(sockfd, msg, sizeof(msg)); 
        printf("waiting for sensor...\n");
        read(sockfd, buffer, sizeof(buffer)); 
        printf("Message from server: %s\n",buffer);
      //  printf("to Server : %s\n", sensor_data); 
       // write_to_file("./sensor_data", sensor_data);
	  //  sleep(2);//sync with the sensor data updating frequency 
    } 
} 
  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 


    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
    pa_simple *s = NULL;
    int ret = 1;
    int error;
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 

    //servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("172.16.128.253"); 
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT); 
  
    // connect the client to server 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 


    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
        exit(0);
    }
    while(1) 
    { 

         if(pa_simple_read(s, buf, sizeof(buf), &error) < 0) 
        {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            exit(0);
        }

        if (pa_simple_write(sockfd, buf, (size_t) r, &error) < 0)
        {
                    fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
                    goto finish_play;
        }
    }
 
    //comm(sockfd); 
  
    // close the socket 
    close(sockfd); 
} 
