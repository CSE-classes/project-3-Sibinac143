#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_THREADS 4
#define MAX 1024

void *sub_string(void *);
int readf(FILE *fp);

int total = 0;
int nlocal, n1, n2;
char *s1, *s2;
FILE *fp;

pthread_mutex_t total_lock;

int main(int argc, char *argv[])
{
    int i, rc;
    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&total_lock, NULL);

    if (readf(fp) < 0) {
        printf("Error reading file.\n");
        return 1;
    }

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, sub_string, (void *)(long)i);
        if (rc) {
            printf("ERROR: return error from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            printf("ERROR: return error from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    printf("the occurences of s2 in s1 is %d\n", total);

    pthread_mutex_destroy(&total_lock);
    free(s1);
    free(s2);

    return 0;
}

int readf(FILE *fp)
{
    fp = fopen("strings.txt", "r");
    if (fp == NULL) {
        printf("ERROR: can't open strings.txt!\n");
        return -1;
    }

    s1 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL) {
        printf("ERROR: Out of memory!\n");
        fclose(fp);
        return -1;
    }

    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s2 == NULL) {
        printf("ERROR: Out of memory!\n");
        free(s1);
        fclose(fp);
        return -1;
    }

    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);

    fclose(fp);

    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    n1 = strlen(s1);
    n2 = strlen(s2) - 1;

    if (n1 > 0 && s1[n1 - 1] == '\n') {
        s1[n1 - 1] = '\0';
        n1--;
    }

    if (n2 > 0 && s2[n2 - 1] == '\n') {
        s2[n2 - 1] = '\0';
        n2--;
    }

    if (n1 < n2) {
        return -1;
    }

    nlocal = n1 / NUM_THREADS;

    return 0;
}

void *sub_string(void *threadid)
{
    int tid = (int)(long)threadid;

    int start = tid * nlocal;
    int end = start + nlocal;

    if (tid == NUM_THREADS - 1) {
        end = n1;
    } else {
        end = end + n2 - 1;
        if (end > n1) {
            end = n1;
        }
    }

    int local_total = 0;

    for (int i = start; i <= end - n2; i++) {
        int match = 1;

        for (int j = 0; j < n2; j++) {
            if (s1[i + j] != s2[j]) {
                match = 0;
                break;
            }
        }

        if (match) {
            local_total++;
        }
    }

    pthread_mutex_lock(&total_lock);
    total += local_total;
    pthread_mutex_unlock(&total_lock);

    pthread_exit(NULL);
}






