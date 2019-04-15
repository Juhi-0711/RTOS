#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <time.h>   
#include <errno.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 1024


#define PORT 8000
#define SA struct sockaddr 
  
// commtion designed for chat between client and server. 
/*char* read_sensor_data()
{
    int sensor_data[5];
    char buffer[1000];
    int i=0;
    char fname[100];
    for(int i = 0; i < 5; i++)
    {
        sprintf(fname, "sensor%d.txt", i+1);
        FILE* fp = fopen(fname, "r");

        if(fp == NULL) 
        {
            printf("error in reading: %s\n", fname);
        }
        
        fscanf(fp, "%d", &(sensor_data[i]));

        fclose(fp);
    }
    sprintf(buffer, "%d,%d,%d,%d,%d", sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4]);
    return buffer;

}*/
/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(int fd, const void*data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = write(fd, data, size)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}


void comm(int sockfd) 
{ 
    char buffer[1000]; 
    char msg[1000];
   	
    // infinite loop for chat 
    while(1)
    {  
        /*if (pa_simple_read(sockfd, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            exit(0);
        }

        if (pa_simple_write(s, buf, (size_t) r, &error) < 0)
             {
                    fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
                    goto finish_play;
            }*/
        read(sockfd, buffer, sizeof(buffer)); 
        printf("recieved from client %s\n",buffer);
        
        printf("Message for client\n"); 
        scanf("%s",msg);
        write(sockfd, msg, sizeof(msg));
      	printf("sent to client: %s\n",msg);

    } 
} 
  

int main(int argc, char *argv[]) 
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
    while(1)
    {
        if ((listen(sockfd, 5)) != 0)
        { 
            printf("Listen failed...\n"); 
            exit(0); 
        } 
        else
            printf("Server listening..\n"); 
        socklen_t len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
        connfd = accept(sockfd, (SA*)&cli, &len); 
        if (connfd < 0) 
        { 
            printf("server acccept failed...\n"); 
            exit(0); 
        } 
        else
            printf("server acccept the client...\n"); 
  
    // commtion for chatting between client and server 
    //comm(connfd);
        if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) 
        {
                fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
               // goto finish_play;
        }
        while(1)
        {  
            uint8_t buf[BUFSIZE];
            ssize_t r;
            //printf("check\n");
            if ((r = read(connfd, buf, sizeof(buf))) <= 0)
                 {
                    if (r == 0) /* EOF */
                            break;
                    fprintf(stderr, __FILE__": read() failed: %s\n", strerror(errno));
                    exit(0);
                }

            if (pa_simple_write(s, buf, (size_t) r, &error) < 0)
            {
                fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
                exit(0);
            }
    
        }
    // After chatting close the socket 
    close(sockfd); 
    } 
}

