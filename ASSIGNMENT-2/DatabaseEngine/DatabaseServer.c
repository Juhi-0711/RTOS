#include <errno.h> 
#include <arpa/inet.h>   
#include <netinet/in.h> 
#include <sys/time.h> 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <fcntl.h>       

    
#define TRUE   1 
#define FALSE  0 
#define PORT 8888 

    
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int socketM , addrlen , socketN , socketClient[30] , 
          clientMax = 30 , activity, i , readValue , sd;  
    int sdMax;  
    struct sockaddr_in serverAddress;  
        
    char readBuffer[1025];  
        
    //socket descriptors 
    fd_set fdsRead;  
        

    char *message = "Hellooo.... \r\n";  
    
    //initialise
    for (i = 0; i < clientMax; i++)  
    {  
        socketClient[i] = 0;  
    }  
        
    //create a master socket 
    if( (socketM = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //set master socket to allow multiple connections , 

    if( setsockopt(socketM, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    //type of socket created 
    serverAddress.sin_family = AF_INET;  
    serverAddress.sin_addr.s_addr = INADDR_ANY;  
    serverAddress.sin_port = htons( PORT );  
        
    //bind the socket to localhost port 8888 
    if (bind(socketM, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
        //max 3 pending connections
    if (listen(socketM, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    addrlen = sizeof(serverAddress);  
    puts("Waiting for connections ...");  

   
    int fdJoint;
    fdJoint = open("joint.txt", O_RDWR);
    
    int fdProject;
    fdProject = open("project.txt", O_RDWR);
    int ls;


    char *hello;// = "Send request";
    char data_file[1024];

        
    while(TRUE)  
    {  
        FD_ZERO(&fdsRead);  

   
        FD_SET(socketM, &fdsRead);  
        sdMax = socketM; 

        for ( i = 0 ; i < clientMax ; i++)  
        {  
         
            sd = socketClient[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &fdsRead);  
                
 
            if(sd > sdMax)  
                sdMax = sd;  
        }

        activity = select( sdMax + 1 , &fdsRead , NULL , NULL , NULL);  
        
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        } 
            

        if (FD_ISSET(socketM, &fdsRead))  
        {  
            if ((socketN = accept(socketM, (struct sockaddr *)&serverAddress, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , socketN , inet_ntoa(serverAddress.sin_addr) , ntohs(serverAddress.sin_port));  
                
            puts("Welcome message sent successfully");  
                
            //add new socket to array of sockets 
            for (i = 0; i < clientMax; i++)  
            {  
              
                if( socketClient[i] == 0 )  
                {  
                    socketClient[i] = socketN;  
                    printf("Adding to list of sockets as %d\n" , i);  
                        
                    hello = "Send request";
                  
                    break;  
                }  
            }  
        }  
            
        // IO operation on some other socket
        for (i = 0; i < clientMax; i++)  
        {  
            sd = socketClient[i];  
                
        
            if (FD_ISSET( sd , &fdsRead))  
            {  
          
                memset(readBuffer, 0, 1024);
                if ((readValue = read( sd , readBuffer, 1024)) == 0)  
                {  
                  
                    getpeername(sd , (struct sockaddr*)&serverAddress , (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(serverAddress.sin_addr) , ntohs(serverAddress.sin_port));  
                 
                    close( sd );  
                    socketClient[i] = 0;  
                    
                }  
                    
                //Echo back the message that came in 
                else
                {  

                    printf("Got: %s\n", readBuffer);

                    char *textRcv = strtok (readBuffer," "); 
        
                    char file[1024];
                    
                    while(textRcv != NULL)
                    {
                        strcpy(file,textRcv);
                        textRcv = strtok (NULL, " ");
                    }

                    size_t len = strlen(file) - 1;
                    
                    if (*file && file[len] == '\n')  
                    {
                        file[len] = '\0';
                    }
                    
                    FILE *fp;
                    fp = fopen(file, "r");
                    
                    if (fp == NULL) 
                    { 
                        strcpy(data_file, "Data not found");
                    } 
                    else
                    { 
                        fgets( data_file, 50, fp ); 
                    } 
                    
                    fclose(fp);

                   
                    send(sd , data_file , strlen(data_file) , 0 );
                    

                    memset(readBuffer, 0, 1024);   
                    memset(data_file, 0, 1024);   


                    printf("Sent\n");

                }  
            }  
        }  
    }  
        
    return 0;  
}
