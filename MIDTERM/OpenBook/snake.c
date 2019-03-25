#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>

pthread_t thread_id[4]; 
int thread_count = 0;

pthread_mutex_t thread_locks[4]; 
pthread_mutex_t main_lock;

char str[1000];
int winner = 0;



// Create a coroutine
int create(void *(*f)())
{
	pthread_create(&(thread_id[thread_count++]), NULL, f, NULL);
	pthread_mutex_lock(&main_lock);
	return thread_count - 1;
}

// Send data to coroutine
void nextRound(int cr_id)
{
	pthread_mutex_unlock(&(thread_locks[cr_id]));
	pthread_mutex_lock(&main_lock);
}


// Kill a coroutine
void kill(int cr_id)
{
	pthread_cancel(thread_id[cr_id]);
	pthread_mutex_destroy(&(thread_locks[cr_id])); 
}

void pauseCo(int cr_id){
	//printf("sub: %d\n",pthread_self());
	pthread_mutex_unlock(&main_lock);
	pthread_mutex_lock(&(thread_locks[cr_id]));
	
}

void* players()
{
	srand(time(0)); 
	int cr_id = thread_count - 1;
	int level = 0;
	int dice;
	while(1)
	{

		dice = rand() % 6 + 1;
		level += dice;
		if(level >= 100)
		{
			winner = cr_id + 1;
			printf("Winner: Player %d\n", cr_id+1);

		}
		pauseCo(cr_id);
		// printf("%d %d\n", cr_id+1, level);

	}

}


int main() 
{ 	
	// Initialize
	int i = 0; 
	for(i = 0; i < 4; i++)
	{
		pthread_mutex_init(&(thread_locks[i]), NULL);
		pthread_mutex_lock(&(thread_locks[i]));
	}

	pthread_mutex_lock(&main_lock);

	struct timeval start, finish;


	// Create players
	int cr1 = create(&players);
	int cr2 = create(&players);
	int cr3 = create(&players);
	int cr4 = create(&players);

	gettimeofday(&start, NULL);


	while(1)
	{
		if(winner > 0)
			break;
		nextRound(cr1);
		if(winner > 0)
			break;
		nextRound(cr2);
		if(winner > 0)
			break;
		nextRound(cr3);
		if(winner > 0)
			break;
		nextRound(cr4);

	}

    gettimeofday(&finish, NULL);

	// printf("Time taken: %d\n", winner);
	long seconds = (finish.tv_sec - start.tv_sec); 
    long micros = ((seconds * 1000000) + finish.tv_usec) - (start.tv_usec);

    printf("%ld seconds %ld microseconds\n", seconds, micros);

	// Kill the routines
	kill(cr1);
	kill(cr2);
	kill(cr3);
	kill(cr4);

	return 0; 
} 