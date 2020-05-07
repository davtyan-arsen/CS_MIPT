#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"

// structs describing the start and the end of a segment
typedef struct {
    int start;
    int end;
} Segment;

pthread_mutex_t sumAddLock;
long result = 0;

void* CalculateSegmentSum(void* segment) {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    // each thread receives a corresponding segment sruct and finds the sum of its elements
    Segment* currentSegment = (Segment*)segment;
    long currentSegmentSum = 0;
    for(int i = currentSegment->start; i <= currentSegment->end; i++) {
        currentSegmentSum += i;
    }

    // after calculating the sum of a segment, each thread locks the mutex,
    // adds the sum to the result and unlocks it
    pthread_mutex_lock(&sumAddLock);
    result += currentSegmentSum;
    pthread_mutex_unlock(&sumAddLock);

    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = begin.tv_sec - end.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("thread %ld - %.9fs\n", (long)pthread_self(), elapsed);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    struct timespec mainBegin, mainEnd;
    clock_gettime(CLOCK_REALTIME, &mainBegin);

    int N = atoi(argv[1]);
    int threadCount = atoi(argv[2]); // number of threads to be used

    // main program allocates structs for each segment and initializes them
    int range = N / threadCount;
    Segment* segments = (Segment*)malloc(threadCount * sizeof(Segment));

    int i;
    for (i = 0; i < threadCount; i++) {
        segments[i].start = range * i + 1;
        segments[i].end = range * (i + 1);
    }
    segments[threadCount - 1].end = N;

    pthread_mutex_init(&sumAddLock, NULL);

    // each thread receives a corresponding segment
    pthread_t* tid = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    for (i = 0; i < threadCount; ++i)
        pthread_create(&tid[i], NULL, CalculateSegmentSum, &segments[i]);
    for (i = 0; i < threadCount; ++i)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&sumAddLock);

    printf("result is %ld\n", result);

    free(segments);
    free(tid);

    clock_gettime(CLOCK_REALTIME, &mainEnd);
    double mainElapsed = mainEnd.tv_sec - mainBegin.tv_sec;
    mainElapsed += (mainEnd.tv_nsec - mainBegin.tv_nsec) / 1000000000.0;
    printf("Main program - %.9fs\n", mainElapsed);
}
