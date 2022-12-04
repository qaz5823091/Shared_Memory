#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SHMKEY ((key_t) 1234)
#define PERMS 0666
#define MAX_SIZE 1024

char* intToCharArray(int number);
void charArrayToInt(char arr[], int *a, int *b);

int main()
{
	int a, b;
	int shm_id;
	char *data;

	if ((shm_id = shmget(SHMKEY, MAX_SIZE + 1, PERMS)) < 0) {
		perror("shmget failed!");
		exit(1);
	}

	if (( data = shmat(shm_id, NULL, 0)) == (void *) - 1) {
		perror("shmat failed!");
		exit(1);
	}

	data[MAX_SIZE] = 1;
	printf("P2 > get %s\n", data);
	charArrayToInt(data, &a, &b);
	sleep(1);

	char *temp_arr = intToCharArray(a + b);
	strcpy(data, temp_arr);
	printf("P2 > pass %d\n", a + b);
	sleep(1);

	a = 0;
	b = 0;

	exit(getpid());
	return 0;
}

char* intToCharArray(int number) {
	char *result = malloc(MAX_SIZE);
	sprintf(result, "%d", number);

	return result; 
}

void charArrayToInt(char arr[], int *a, int *b) {
	sscanf(arr, "%d %d", a, b);

	return ;
}