#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int pid1, pid2, pid3, pid4, pid5, pid6;

	pid1 = getpid();
	printf("PID = %d\n", pid1);

	pid2 = fork();
	if (pid2 == 0)
	{
		printf("PID = %d, Parent PID = %d\n", getpid(), getppid());
		sleep(30);
		exit(0);
	}
	pid3 = fork();
	if (pid3 == 0)
	{
		printf("PID = %d, Parent PID = %d\n", getpid(), getppid());

		pid5 = fork();
		if (pid5 == 0)
		{
			printf("PID = %d, Parent PID = %d\n", getpid(), getppid());

			pid6 = fork();
			if (pid6 == 0)
			{
				printf("PID = %d, Parent PID = %d\n", getpid(), getppid());
				sleep(30);
				exit(0);
			}

			sleep(30);
			exit(0);
		}
		sleep(30);
		exit(0);
	}

	pid4 = fork();
	if (pid4 == 0)
	{
		printf("PID = %d, Parent PID = %d\n", getpid(), getppid());
		sleep(30);
		exit(0);
	}

sleep(30);
	return 0;
}