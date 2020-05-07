#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "time.h"
#include "math.h"

const int partitionFreq = 100;

// structs describing the start and the end of a segment
typedef struct {
    double start;
    double end;
} Segment;

pthread_mutex_t sumAddLock;
double result = 0;

void* CalculateSegmentSum(void* segment) {
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);

    // each thread receives a corresponding segment sruct and finds the integral(sum) on it
    Segment* currentSegment = (Segment*)segment;
    double currentSegmentIntegral = 0;

    // using the "Trapezoidal rule"
    // xdx = (x_rigth - x_left) * (x_left + x_right) / 2
    double range = (currentSegment->end - currentSegment->start) / partitionFreq;
    for (int i = 0; i < partitionFreq; i++) {
        double average = currentSegment->start + range * (i + 0.5);
        currentSegmentIntegral += range * average;
    }

    // after calculating the integral on segment, each thread locks the mutex,
    // adds it to the result and unlocks the mutex
    pthread_mutex_lock(&sumAddLock);
    result += currentSegmentIntegral;
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

    int threadCount = (int)ceil((rightBound - leftBound) / segmentLength);

    // main program allocates structs for each segment and initializes them
    Segment* segments = (Segment*)malloc(threadCount * sizeof(Segment));

    int i;
    for (i = 0; i < threadCount; i++) {
        segments[i].start = leftBound + segmentLength * i;
        segments[i].end = segments[i].start + segmentLength;
    }
    segments[threadCount - 1].end = rightBound;

    pthread_mutex_init(&sumAddLock, NULL);

    // each thread receives a corresponding segment
    pthread_t* tid = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    for (i = 0; i < threadCount; ++i)
        pthread_create(&tid[i], NULL, CalculateSegmentSum, &segments[i]);
    for (i = 0; i < threadCount; ++i)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&sumAddLock);

    printf("result is %f\n", result);

    free(segments);
    free(tid);

    clock_gettime(CLOCK_REALTIME, &mainEnd);
    double mainElapsed = mainEnd.tv_sec - mainBegin.tv_sec;
    mainElapsed += (mainEnd.tv_nsec - mainBegin.tv_nsec) / 1000000000.0;
    printf("Main program - %.9fs\n", mainElapsed);
}
