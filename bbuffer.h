/*
 * This header file is for programming #3. Contains values for the bbuffer.c to use and contains the prototypes for its functions
 * Info:
 * Alan Ta
 * 819357091
 * cssc2363
 * CS570 Summer
 * Program 3: Producer & Consumer Problem
 * Due: 08/02/2020
*/

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define BUFFER_SIZE 5   // rounded buffer size

#define PRODUCERS 3  // number of producers
#define CONSUMERS 5  // number of consumers

#define ITERATIONS 60   // number of actions producers/consumers perform
#define PRODUCER_ITERATIONS (ITERATIONS / PRODUCERS)  //number of actions each producer performs
#define CONSUMER_ITERATIONS (ITERATIONS / CONSUMERS) //number of actions each consumer performs

typedef struct {
    int value[BUFFER_SIZE];
    int next_in, next_out;
} buffer_t;    // struct of bounded buffer


buffer_t buffer;   //global variable: rounded buffer

pthread_t consumer_tid[CONSUMERS], producer_tid[PRODUCERS]; // producer and consumer threads

// A function that initializes the semaphores with sem_init.
// Initializes the empty, full and mutex
void initilization();

// A function called by the producer and inserts an item into the buffer array
// It sem_wait(&empty), locks down the critical region with the mutex, and sem_post(&full)
int insert_item(int item, long int id);

// A function called by the consumer and removes an item from the buffer array
// It sem_wait(&full), locks down the critical region with the mutex, and sem_post(&empty)
int remove_item(int *item, long int id);

// A function that that acts like a producer and will add items/data into the array buffer
// Calls the insert_item function when inserting
void * producer(void *param);

// A function that that acts like a consumer and will remove items/data into the array buffer
// Calls the remove_item function when removing
void * consumer(void *param);


