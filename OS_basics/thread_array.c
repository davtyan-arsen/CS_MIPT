// ***
//  Compiling: gcc thread_array.c -lpthread -O3
//
//  NumberOfThreads     1       2       4       8(have only 4 cores)
//  WallTime            ~0,4    ~0,2    ~0,1    ~0,1
//  Boost               1       2       4       4
// ***

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define ARRAY_LENGTH 100000000
#define MAX_VALUE 500

struct SegmentOfArray
{
    int first;
    int last;
    float average;
};

int* array;

float averageValue = 0;
float dispersion = 0;

void* threadAverage(void* dummy)
{
    struct SegmentOfArray segment = *(struct SegmentOfArray*)dummy;
    for(int i = segment.first; i <= segment.last; i++)
        segment.average += array[i];

    segment.average /= (segment.last - segment.first + 1);
    averageValue += segment.average;
}

void* threadDispersion(void* dummy)
{
    struct SegmentOfArray segment = *(struct SegmentOfArray*)dummy;
    for(int i = segment.first; i <= segment.last; i++)
        dispersion += (array[i] - averageValue) * (array[i] - averageValue);
}

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time, NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main(int argc, char const *argv[])
{
    int result;

    array = (int*)malloc(ARRAY_LENGTH * sizeof(int));
    for(int i = 0; i < ARRAY_LENGTH; i++)
    {
        array[i] = rand() % MAX_VALUE;
    }

    int threadCount;
    scanf("%d", &threadCount);
    if((ARRAY_LENGTH % threadCount) != 0)
    {
        printf("Inserted number of threads is unacceptable\n");
        return -1;
    }

    int segmentLength = ARRAY_LENGTH / threadCount;

    double start = get_wall_time();

    pthread_t * thids = malloc(threadCount * sizeof(pthread_t));

    struct SegmentOfArray* segment = (struct SegmentOfArray*)malloc(threadCount * sizeof(struct SegmentOfArray));
    for(int i = 0; i < threadCount; i++)
	{
        segment[i].first = segmentLength * i;
        segment[i].last = segment[i].first + segmentLength - 1;
        segment[i].average = 0;

        result = pthread_create(&thids[i], NULL, threadAverage, &segment[i]);
	}

    for(int i = 0; i < threadCount; i++)
        result = pthread_join(thids[i], (void**)NULL);

    averageValue = averageValue / threadCount;

    for(int i = 0; i < threadCount; i++)
	{
        result = pthread_create(&thids[i], NULL, threadDispersion, &segment[i]);
	}

    for(int i = 0; i < threadCount; i++)
        result = pthread_join(thids[i], (void**)NULL);

    dispersion = dispersion / ARRAY_LENGTH;

    printf("average value = %f\n", averageValue);
    printf("dispersion = %f\n", dispersion);

    double finish = get_wall_time();
    double time = finish - start;
    printf("time spent = %f\n", time);

    return 0;
}
