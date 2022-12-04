#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define SHMKEY ((key_t) 1234)
#define PERMS 0666
#define MAX_SIZE 1024

char* intToCharArray(int a, int b);
void forkToChild(pid_t pid);
void charArrayToInt(char arr[], int *number);

int main()
{
	pid_t parent = getpid();
	pid_t pid;

	int a, b, result;
	int shm_id;
	char *data;
	srand(time(NULL));

	if ((shm_id = shmget(SHMKEY, MAX_SIZE + 1, IPC_CREAT | PERMS)) < 0) {
		perror("shmget failed!");
		exit(1);
	}

	if ((data = shmat(shm_id, NULL, 0)) == (void *) - 1) {
		perror("shmat failed!");
		exit(1);
	}
	while (1) {
		//printf("Please enter two integer: ");
		//scanf("%d %d", &a, &b);
		a = rand() % 100 + 1;
		b = rand() % 100 + 1;
		printf("P1 > pass %d and %d\n", a, b);
		char *temp_arr = intToCharArray(a, b);
		strcpy(data, temp_arr);
		sleep(1);

		pid = fork();
		forkToChild(pid);
		waitpid(pid, NULL, 0);

		data[MAX_SIZE] = 1;
		printf("P1 > get %s\n", data);
		charArrayToInt(data, &result);
		printf("\n%d + %d = %d\n\n", a, b, result);
		sleep(3);
	}
}

char* intToCharArray(int a, int b) {
	char *result = malloc(MAX_SIZE);
	sprintf(result, "%d %d", a, b);

	return result; 
}

void forkToChild(pid_t pid) {
	if (pid < 0) {
		perror("Fork failed!");
		exit(0);
	}
	else if (pid == 0) {
		execl("./p2", "p2", NULL);
	}

	return ;
}

void charArrayToInt(char arr[], int *number) {
	sscanf(arr, "%d", number);

	return ;
}

