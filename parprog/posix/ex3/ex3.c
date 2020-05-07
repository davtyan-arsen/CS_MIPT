#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "pthread.h"
#include "semaphore.h"
#include "time.h"
#include "math.h"

const int partitionFreq = 100;

// structs describing the start and the end of a segment
typedef struct {
    double start;
    double end;
} Segment;

sem_t resultReady;
pthread_mutex_t sumAddLock;
double result = 0;

double f(double x) {
    return exp(x);
}

void* PrintResult() {
    int runningThreadsCount = 1;
    // master thread checks in a while loop if the number of running slave threads is 0
    // if it is then the result is ready to be shown
    while (runningThreadsCount != 0) {
        sem_getvalue(&resultReady, &runningThreadsCount);
    };

    printf("thread %ld - result is %f\n", (long)pthread_self(), result);
}

void* CalculateSegmentSum(void* segment) {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    // each thread receives a corresponding segment sruct and finds the integral(sum) on it
    Segment* currentSegment = (Segment*)segment;
    double currentSegmentIntegral = 0;

    // using the "Trapezoidal rule"
    // f(x)dx = (x_rigth - x_left) * f( (x_left + x_right)/2 )
    double range = (currentSegment->end - currentSegment->start) / partitionFreq;
    for (int i = 0; i < partitionFreq; i++) {
        double left = currentSegment->start + range * i;
        double right = left + range;
        double average = f((left + right) / 2.);
        currentSegmentIntegral += range * average;
    }

    // after calculating the integral on segment, each thread locks the mutex,
    // adds it to the result and unlocks the mutex
    pthread_mutex_lock(&sumAddLock);
    result += currentSegmentIntegral;
    // after a slave thread added its part to the result
    // it decrements the number of running slave threads
    sem_wait(&resultReady);
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

    double leftBound = atof(argv[1]);
    double rightBound = atof(argv[2]);
    double segmentLength = atof(argv[3]);

    int threadCount = (int)ceil((rightBound - leftBound) / segmentLength + 1);

    // main program allocates structs for each segment and initializes them
    Segment* segments = (Segment*)malloc((threadCount - 1) * sizeof(Segment));

    int i;
    for (i = 0; i < threadCount - 1; i++) {
        segments[i].start = leftBound + segmentLength * i;
        segments[i].end = segments[i].start + segmentLength;
    }
    segments[threadCount - 2].end = rightBound;

    // setting the number of running slave threads to (threadCount - 1)
    sem_init(&resultReady, 0, threadCount - 1);
    pthread_mutex_init(&sumAddLock, NULL);

    // each thread receives a corresponding segment
    pthread_t* tid = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    // master thread doesn't do any calculations
    pthread_create(&tid[0], NULL, PrintResult, NULL);
    for (i = 0; i < threadCount - 1; ++i)
        pthread_create(&tid[i], NULL, CalculateSegmentSum, &segments[i]);
    for (i = 0; i < threadCount - 1; ++i)
        pthread_join(tid[i], NULL);

    sem_destroy(&resultReady);
    pthread_mutex_destroy(&sumAddLock);

    free(segments);
    free(tid);

    clock_gettime(CLOCK_REALTIME, &mainEnd);
    double mainElapsed = mainEnd.tv_sec - mainBegin.tv_sec;
    mainElapsed += (mainEnd.tv_nsec - mainBegin.tv_nsec) / 1000000000.0;
    printf("Main program - %.9fs\n", mainElapsed);
}
