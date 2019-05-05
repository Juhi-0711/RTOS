#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <time.h>   
#include <arpa/inet.h>


pthread_t thread_id[4]; 
int cnt = 0;

pthread_mutex_t thread_locks[4]; 
pthread_mutex_t main_lock;

char str[100] = "\0";

// int player1[4] = {0};
// int player2[4] = {0};
// int player3[4] = {0};
// int player4[4] = {0};

// int pawns[4][4] = {{0,0,0,0}, {13,13,13,13}, {26,26,26,26}, {39,39,39,39}};
int pawns[4][4] = {0};

int board[56] = {0};



void runPlayer(int crid, char *msg)
{
	strcpy(str, msg);
	pthread_mutex_unlock(&(thread_locks[crid]));
	pthread_mutex_lock(&main_lock);
}

// Kill a co routine
void kill(int crid)
{
	pthread_cancel(thread_id[crid]);
	pthread_mutex_destroy(&(thread_locks[crid])); 
}

int create(void *(*f)())
{
	pthread_create(&(thread_id[cnt++]), NULL, f, NULL);
	pthread_mutex_lock(&main_lock);
	return cnt - 1;
}




int inHouse(int crid)
{
	int c=0;
	for(int i=0;i<4;i++)
	{
		if(pawns[crid][i] == 0)
			c++;
	}
	return c;
}


void showPawns()
{
	printf("\n");
	for(int i=0;i<4;i++)
	{
		printf("Player %d: ", i+1);
		for(int j=0;j<4;j++)
		{
			printf("%d ", pawns[i][j]);
		}
		printf("\n");
	}
}

void displace(crid, p)
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(crid == i)
			{
				break;
			}
			if(pawns[i][j] == pawns[crid][p])
			{
				pawns[i][j] = 0;
			}
		}
	}
}


void *players()
{
	char ch;
	srand(time(0));
	int crid = cnt - 1;

	int pmoved;

	int dice;

	while(1)
	{
		pthread_mutex_unlock(&main_lock);
		pthread_mutex_lock(&(thread_locks[crid]));	
		
		// cout<<"Plaoll dice";
		printf("\nPlayer %d roll dice (press enter)\n", crid + 1);
		scanf("%lc", &ch);

		dice = rand() % 6 + 1;

		printf("Player %d rolled %d\n", crid+1, dice);

		if(inHouse(crid) == 4)
		{
			if(dice == 6)
			{
				printf("Removing first pawn\n");
				
				printf("Player %d roll dice (press enter)\n", crid + 1);
				scanf("%c", &ch);

				dice = rand() % 6 + 1;

				printf("Player %d rolled %d\n", crid+1, dice);

				pawns[crid][0] = (pawns[crid][0] + dice) % 52 + crid*13;
				pmoved = 0;
			}
			else
			{
				// cout<<"Nothing\n";
			}
		}

		else if(inHouse(crid) >= 1 && inHouse(crid) <= 3)	
		{
			// bool pawn;
			if(dice == 6)
			{
				int np;
				printf("Want to remove new pawn?? (enter 0 for no, 1 for yes)\n");
				scanf("%d", &np);

				//cout<<"Roll";
				if(np == 1)
				{				
					// dice = rand() % 6 + 1;
					// pawns[inHouse()] += dice;

					//cout<<"Roll";
					// dice = ;

					printf("Player %d roll dice (press enter)\n", crid + 1);
					scanf("%c", &ch);

					dice = rand() % 6 + 1;
					printf("Player %d rolled %d\n", crid+1, dice);

					pawns[crid][4-inHouse(crid)] = (pawns[crid][4-inHouse(crid)] + dice) % 52 + crid*13 ;

					pmoved = 4-inHouse(crid);
				}
				if(np == 0)
				{
					if(inHouse(crid) == 3)
					{
						pawns[crid][0] = (pawns[crid][0] + dice) % 52;
					}
					else
					{
						int p;

						printf("Which pawn to move\n");
						scanf("%d", &p);

						pawns[crid][p-1] = (pawns[crid][p-1] + dice) % 52 ;

						pmoved = p-1;
					}
				}

			}
			// else
			
			else if(dice < 6)
			{
				if(inHouse(crid) == 3)
				{
					pawns[crid][0] = (pawns[crid][0] + dice) % 52;
				}
				else
				{
					int p;

					printf("Which pawn to move\n");
					scanf("%d", &p);

					pawns[crid][p-1] = (pawns[crid][p-1] + dice) % 52 ;

					pmoved = p-1;
				}
			}
			
		}

		else if(inHouse(crid) == 0)
		{
			int p;
			printf("Which pawn to move\n");

			scanf("%d", &p);
			pawns[crid][p-1] = (pawns[crid][p-1] + dice) % 52 ;

			pmoved = p-1;
		}

		displace(crid, pmoved);

		showPawns();

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

	// struct timeval start, finish;


	// Create some co routines 
	int cr1 = create(&players);
	int cr2 = create(&players);
	int cr3 = create(&players);
	int cr4 = create(&players);

	// gettimeofday(&start, NULL);


	while(1)
	{
		runPlayer(cr1, "W");
		
		runPlayer(cr2, "W");
		
		runPlayer(cr3, "W");
		
		runPlayer(cr4, "W");

	}


	// Kill the routines
	kill(cr1);
	kill(cr2);
	kill(cr3);
	kill(cr4);

	return 0; 
} 