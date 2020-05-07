#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

void* ShowThreadID() {
    printf("Hello world! This is thread %ld \n", (long)pthread_self());
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // Command line argument is the number of threads to be created
    int threadCount = atoi(argv[1]);
    pthread_t* tid = (pthread_t*)malloc(threadCount * sizeof(pthread_t));

    int i;
    for (i = 0; i < threadCount; ++i)
        pthread_create(&tid[i], NULL, ShowThreadID, NULL);
    for (i = 0; i < threadCount; ++i)
        pthread_join(tid[i], NULL);
}
