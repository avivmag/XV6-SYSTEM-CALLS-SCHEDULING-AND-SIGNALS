#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"
struct perf;

void consumeCpu();
void blockingSleep();
void mixed();

int NUM_OF_PROCESSES = 30;

int
main(int argc, char *argv[])
{
	schedp(2);
	int i, pid = 1;
	for(i = 0; pid != 0 && i < NUM_OF_PROCESSES; i++)
		pid = fork();

	priority(getpid());

	if(pid == 0)
	{
		switch(i%3)
		{
			case 0:
				consumeCpu();
			break;
			case 1:
				blockingSleep();
			break;
			case 2:
				mixed();
			break;
			default:
			break;
		}
	}

	if(pid != 0)
	{
		int sumWaiting = 0, sumRunning = 0, sumTurnaround = 0;
		int s = 0;
		int *status = &s;
		struct perf pr;
		struct perf *prf = &pr;

		for(i = 0; i < NUM_OF_PROCESSES; i++)
		{
			int pid = wait_stat(status, prf);

			sumWaiting += prf->stime;
			sumRunning += prf->rutime;
			sumTurnaround += (prf->ttime - prf->ctime);

			printf(2, "%d - waiting: %d\n", pid, prf->stime);
			printf(2, "%d - running: %d\n", pid, prf->rutime);
			printf(2, "%d - turnaround: %d\n", pid, (prf->ttime - prf->ctime));
		}

		printf(2, "average waiting: %d\n", sumWaiting / NUM_OF_PROCESSES);
		printf(2, "average running: %d\n", sumRunning / NUM_OF_PROCESSES);
		printf(2, "average turnaround: %d\n", sumTurnaround / NUM_OF_PROCESSES);
	}
	
	exit(0);
	return 0;
} 
 
void consumeCpu()
{
	int i, startingUpTime = uptime();
	while(uptime() - startingUpTime <= 30)
		for(i = 0; i < 1000; i++);
}

void blockingSleep()
{
	int i;
	for(i = 0; i < 30; i++)
		sleep(1);
}

void mixed()
{
	int i;
	for(i = 0; i < 5; i++)
	{
		int startingUpTime = uptime();
		while(uptime() - startingUpTime <= 5)
			for(i = 0; i < 1000; i++);
		sleep(1);
	}
}