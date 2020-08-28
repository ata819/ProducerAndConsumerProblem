/*
 * Implements the producer and consumer functions and behaviors
 * Info:
 * Alan Ta
 * 819357091
 * cssc2363
 * CS570 Summer
 * Program 3: Producer & Consumer Problem
 * Due: 08/02/2020
*/

#include <stdio.h>
#include <stdlib.h>
#include "bbuffer.h"

sem_t empty;
sem_t full;
sem_t mutex;

// Initializes the semaphores needed
void initilization()
{
    sem_init(&mutex,0, 1); // Created to allow lockdown of the critical regions
    sem_init(&full, 0, 0); // A semaphore used to control the amount of items/data in the buffer array
    sem_init(&empty, 0 , BUFFER_SIZE); // A semaphore used to limit the amount of items inserted in the array based on the buffer size
}

/**
 * insert_item - thread safe(?) function to insert items to the bounded buffer
 * @param item the value to be inserted
 * @return 0 in case of sucess -1 otherwise
 */
// A function called by the producer and inserts an item into the buffer array
int insert_item(int item, long int id)
{

    sem_wait(&empty); // Decrements the empty semaphore value. If 0, buffer is full
    sem_wait(&mutex); // Decrements the mutex, locks down the critical region
    buffer.value[buffer.next_in] = item;
    printf("producer %ld: inserted item %d into buffer index %d\n", id, item, buffer.next_in);
    buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;
    sem_post(&mutex); // Releases the critical region by incrementing the mutex
    sem_post(&full); // Increments the full semaphore
    return 0;
}

/**
 * remove_item - thread safe(?) function to remove items to the bounded buffer
 * @param item the address of the variable that the removed value will be written
 * @return 0 in case of sucess -1 otherwise
 */
// A function called by the consumer and removes an item from the buffer array
int remove_item(int *item, long int id)
{

    sem_wait(&full); // Decrements the full semaphore. If the full - 0 then the buffer array is empty and cannot remove
    sem_wait(&mutex); // Decrements the mutex, locks down the critical region
    *item = buffer.value[buffer.next_out];
    buffer.value[buffer.next_out] = -1;
    printf("consumer %ld: removed item %d from buffer index %d\n", id, *item, buffer.next_out);
    buffer.next_out = (buffer.next_out + 1) % BUFFER_SIZE;
    sem_post(&mutex); // Releases the critical region by incrementing the mutex
    sem_post(&empty); // Increments the empty semaphore
    return 0;
}

/**
 * producer - will iterate PRODUCER_ITERATION times and call the corresponding
 * function to insert an integer to the bounded buffer
 * @param param an integer id of the producer used to distinguish between the
 * multiple producer threads
 * @return nothing
 */
void * producer(void *param)
{
    int item, i;
    long int id = (long int)param;

    printf("producer %d started!\n", id);
    i = PRODUCER_ITERATIONS;
    while (i--) {
        sleep(rand() % 3);

        item = rand() % 10000;
        if (insert_item(item, id))
            printf("Error while inserting to buffer\n");
    }

    pthread_exit(0);
}

/**
 * consumer - will iterate CONSUMER_ITERATION times and call the corresponding
 * function to remove an integer from the bounded buffer
 * @param param an integer id of the producer used to distinguish between the
 * multiple consumer threads
 * @return nothing
 */
void * consumer(void *param)
{
    int item, i;
    long int id = (long int)param;

    printf("consumer %d started!\n", id);
    i = CONSUMER_ITERATIONS;
    while (i--) {
        sleep(rand() % 3);

        if (remove_item(&item, id))
            printf("Error while removing from buffer\n");
    }

    pthread_exit(0);
}

