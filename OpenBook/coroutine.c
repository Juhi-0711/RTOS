#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h> 


//shared memory
pthread_t thread_id[10]; 
long int thread_count = 0;
pthread_mutex_t thread_locks[10]; 
pthread_mutex_t main_lock;
char name[1000];


//to check if name has prefix attached
int check(char * msg)
{
	char prefix[100]="Ms";
	int i=0,f=1;
	for(i=0;i<2;i++)
	{
		if(msg[i]!=prefix[i])
			f=0;
	}
	return f;
}


//creating co-routine
int createCo(void *(*f)(void *)){

	pthread_create(&(thread_id[thread_count++]), NULL, f, NULL);
	//printf("main: %d\n",pthread_self());
	pthread_mutex_lock(&main_lock);

	return thread_count - 1;
}

//sending value to co-routine
void sendCo(int cr_id, char * msg){

	if(check(msg))
	{
		strcpy(name, msg);
		//printf("name:%s\n ",name);
		//printf("id: %d\n", cr_id);
		//printf("main: %d\n",pthread_self());
		pthread_mutex_unlock(&(thread_locks[cr_id]));
		pthread_mutex_lock(&main_lock);
	}
	else
	{
		strcpy(name, " ");
		//printf("name:%s\n ",name);
	}
}

//pausing co-routine
void pauseCo(int cr_id){
	//printf("sub: %d\n",pthread_self());
	pthread_mutex_unlock(&main_lock);
	pthread_mutex_lock(&(thread_locks[cr_id]));
	
}

//killing co-routine
void killCo(int cr_id){
	pthread_cancel(thread_id[cr_id]);
	pthread_mutex_destroy(&(thread_locks[cr_id])); 
}


//printing names with correct prefix
void* print_name(void *arg) 
{ 
	//printf("print_msg\n");
	int cr_id = thread_count - 1;
	//printf("name:%s\n ",name);
	//printf("%d\n",pthread_self());
	//thread_count=thread_count-1;
	//printf("ID %ld\n", thread_count);
	//printf("cr_id: %d\n",cr_id);
	while(1)
	{

		printf("recieved message %s\n", name); 
		sleep(1);
		pauseCo(cr_id);
		 
	}

	return NULL; 
} 


////////////// MAIN

int main(void) 
{ 	
	// Initialising mutexes
		
	int i = 0; 
	for(i = 0; i < 10; i++){
		pthread_mutex_init(&(thread_locks[i]), NULL);
		pthread_mutex_lock(&(thread_locks[i]));
	}

	pthread_mutex_lock(&main_lock);

	// Create some co routines 
	int cr1 = createCo(&print_name);
	//sendCo(cr1, "A");
	int cr3 = createCo(&print_name);
	sendCo(cr1, "A");
	sendCo(cr3, "ABC");
	sendCo(cr1, "PQR");


	// Kill the routines
	killCo(cr1);
	//killCo(cr2);
	killCo(cr3);
	//killCo(cr3);

		
	return 0; 
} 