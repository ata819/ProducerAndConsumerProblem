Info:
Alan Ta
819357091
cssc2363
CS570 Summer
Program 3: Producer & Consumer Problem
Due: 08/02/2020
Program Description: The purpose of this program assignment was to understand the producer & consumer problem as well as understanding the use of semaphores after learning about mutexes. The benefits a semaphore has in multiprocessing as well as its uses for mutual exclusion

Files included:
bbuffer.c
bbuffer.h
testbbuffer.c
README

Libraries:
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

How to run:
:make

:./testbbuffer // Will execute the program and begin the producer and consumer simulation

:make clean

On a successful execution:
consumer 0 started!
producer 0 started!
consumer 2 started!
consumer 1 started!
producer 1 started!
producer 2 started!
consumer 4 started!
producer 0: inserted item 3215 into buffer index 0
consumer 3 started!
producer 0: inserted item 7530 into buffer index 1
producer 0: inserted item 7392 into buffer index 2
consumer 1: removed item 3215 from buffer index 0
producer 1: inserted item 2897 into buffer index 3
producer 0: inserted item 6592 into buffer index 4
producer 2: inserted item 5249 into buffer index 0
... ...

// Changing the number of producers/consumers or buffer size can be accessed in bbuffer.h

Design & Code explanation:
So this program is pretty simple in terms of amount of code provided by me. The large majority of code was provided by the professor as a set up. Because the theory of Producers and Consumers is so well known, information regarding its nature was readily available and informative. I gain most of my insight into how to approach this assignemnt from reading the ch.2semaphores.txt in the Weekly Readings

So the trick is to:
1. Prevent the buffer from overflowing/data overwritten
2. Prevent the removal of items/data when the buffer is empty
3. Maintain mutual exclusion in the critical regions

This is all done using 3 semaphores: full, empty, and mutex
sem_init(&mutex, 0, 1) acts like a lock with the value beginning at 1. So before an item is being inserted or removed from the buffer, the sem_wait is called to mutex in order to lock down the critical region from other threads. This works as the value of mutex is 1 so when one thread sem_wait(&mutex), it is decremented to 0 and if another thread tries to sem_wait(&mutex) again, it will pause as the value of the semaphore cannot be negative. After the work has been done, sem_post(&mutex) is called and increments the value to 1, allowing other threads to have a go.
This maintain mutual exclusion between threads dealing with the buffer

sem_init(&empty, 0, BUFFER_SIZE) prevents the buffer from being overflown with constant producers. The BUFFER_SIZE is defined in the header file, lets say it's 5. So in the insert_item function, sem_wait(&empty) is first to get called and it decrements the initial value of the buffer by 1. So if the size is 5, after 5 calls the empty semaphore would have a value of 0. Since its 0, it cannot be decremented anymore and will cause the thread to be paused until the value is incremented by the consumer. This prevents the buffer from overflowing as the size of the buffer is always the initial value for the empty semaphore. The value of empty can be incremented by the consumer after it has removed an item (sem_post(&empty))

sem_init(&full, 0, 0) prevents an empty buffer from having an item removed. Since the initial value of full is 0 at the beginning of the program, if a thread tries to remove an item from the empty buffer with sem_wait(&full), it cannot as the value of the full semaphore is 0 and cannot be a negative number. This pauses that thread and waits for the value to be > 0 like when a producer increments the value after inserting an item.

The empty and full semaphores work together as when one is decremented in a function, the other is incremented. This forms a kind of balance between them and the balance allows the buffer to remain protected from overflow and empty removal.
Lets say for the insert_item function for the producer:
sem_wait(&empty) is called, so the BUFFER_SIZE is decremented. The mutex is called to lock, do work, then unlocks. Then the sem_post(&full) is called as now there is 1 more item in the buffer and thus should be incremented in order to reflect the buffer's current state.
With that in mind, now if we call the remove_item function for the consumer:
Understand that we have:
empty = 4
full = 1

When sem_wait(&full) is called, it decrements the value back to 0. The mutex then locks, does work, and then unlocks. The sem_post(&empty) is called to increment the value back to 5.

There we just added an item, then removed it using semaphores. This idea can continue with overflowing, as the value of empty would have decremented to 0 and cannot decrement anymore. Same goes for an empty buffer as the value of full was already 0 so it cannot be decremented at either.

___________________________________________________________________________________________
Please answer the following questions to explain your implementation (Directly write your answer below the questions).

1. Which are the critical regions need to be protected for mutual exclusion? Why are they? 
The critical regions are when the producer inserts the item and when the consumer removes them. These regions are important because they involve the sensitive action of altering the buffer. Because we are deal with threads, the act of removing/inserting in the same index is a possibility. Threads can run the same functions and the problem arises when two or more threads try to change data in the same spot. If this were to happen, data could be overwritten so any thread function code that changes data in anyway has to be controlled using mutexes and semaphores.
By using Semaphores and Mutexes, this blocks other threads from manipulating data while another thread is occupying the resources/critical region. I view like a door way to enter or leave. Because there is only one doorway, a person can leave and enter at the same time. So there is a door hopper by the door (the mutex) and he/she controls who gets to go in or leaves. They go one at a time and if someone is entering/leaving then the others must wait for their turn.


2. There are 7 requirements your program should achieve: 

1)	When a producer thread inserts a new item into the buffer, a message should be print to screen showing which thread (with ID) insert which item (with value) to which buffer slot (with next_in)
2)	When a consumer thread removes a new item from the buffer, a message should be print to screen showing which thread (with ID) remove which item (with value) from which buffer slot (with next_out)
3)	Producers must block if the buffer is full,
4)	Consumers must block if the buffer is empty,
5)	No two/more producers insert items to the same buffer slot,
6)	No two/more consumers remove the items from the same buffer slot,
7)	The messages must be printed in the order showing the real execution scenarios.

Please explain how they are satisfied in your design (you need to explain one by one!)

2_1) 
    Because I set the set sem_wait(&empty) then sem_wait(&mutex), before printing the message and inserting the item, when the message does print, it should be grouped in with the actual insertion when its prints. Grouping the message with the actual insertion of the item enables accurate messaging and the lock down of the critical region ensures the item gets inserted with out other threads overwriting it. The order of sem_wait's mutex and empty matter as, I first need to sem_wait empty to prevent deadlock. With this prevention, I can now lock down the critical region using the mutex and insert the item
    
2_2) 
    The consumer follows the same vein as the producer, as there is a sem_wait(&full) and sem_wait(&mutex) at the beginning of the remove_item function. This first decrements the full value. Notice that the value of &full must be larger than 0, else this wont work. By decrementing it first, this prevents a dead lock between a possible other consumer. Then I lock down the mutex (decrement from 1 to 0) and allow the thread to perform the consumer's job of removing from the buffer
  
2_3)
    The producer does block if the buffer is full by a teamwork of the insert_item and remove_item. They work together by how each handles the full and empty semaphores. By initializing empty has the value of buffersize which is 5, when insert_item, the empty semaphore is decremented by 1 each time. Since the buffer is 5, it can only be decremented 5 times before reaching 0. This is the trick as semaphores cannot be negative numbers. Knowing that, as the program continue to insert_items into the buffer, it can only go as far til it reaches 0. After that, the semaphore cannot allow the insert_item to inserting any more into the buffer thus pausing it for the consumer to add more, incrementing the empty value and pushing father from 0. 

2_4)
    The consumer works similarly to producer in terms of preventing/blocking when the buffer is empty. This is actually simpler than the producer, as the full semaphore begins at 0. Following the same idea, at the beginning of the program, the full semaphore is 0 (sem_init(&full, 0, 0)). In the remove_item() function begins with a sem_wait(&full), thus decrementing the full semaphore value. If the program just began, full's value would be 0, thus sem_wait(&full) cannot decrement the value as it would give a negative value. So the semaphore will pause and wait for full to be > 0. And the only way for that to happen is when an item is inserted and the sem_post(&full) is called to add 1 to the full value. 

2_5)
    In order to prevent two or more producers from inserting into the same buffer, its the job of the mutex to lock down the critical region and allow the threads to do their job. Lets say there are two producers trying to insert into the buffer. If one reaches the sem_wait(&mutex), it will decrement the value from 1 to 0. This locks down access from the other threads as if the second then calls sem_wait(&mutex), 0 cannot be decremented thus pausing that thread. Back to the first thread, with the critical region locked down, the thread can insert into the buffer, change the next buffer.next_in, and print the message on screen. After these are done, it then unlocks the critical region with sem_post(&mutex) incremented to 1.
    I believe that if two threads hit the sem_wait() at the same exact time, both are given random times before allowing to try again. 

2_6)
    This follows the same exact format as the producers. For consumers, there is a mutex that locks down the critical region so only a single thread can be remove an item. When the critical region is locked down, the item is removed, the message is printed out, and the next buffer.next_out is selected. The mutex then opens up and allows other threads to do their jobs

2_7)
    The message does print in order of execution because of the print statement provided by the professor. Each time a producer/consumer would be created, the print statement would be provided right after, thus allowing the message to be printed in real time order of execution. The same goes for the messages of insertion or removal as they are locked in by the mutex semaphore with the actual removal or insertion.



3) Why you think your design is the most effective one? Is there any other design also works but less effective? 
I do believe my design is the most effective because it the fundamental way of doing it. After studying semaphores and the producer and consumer problems with the links provided by the professor (ch.2semaphores.txt), I feel this design is such a fundamental concept that it is the best approach to this problem. Given that the program assignment itself does not stray far from the original idea, I thought the idea of using 3 semaphores would be enough. full and empty handling the amount able to be removed and inserted and the 1 mutex to lock down the critical region. I feel like add more would over complicate things and require more overhead

There was another design I had in mind, but after testing, it reveals it was actually slower than the current design. My alternative design was having two mutexes rather than 1. I felt the full and empty semaphores were pretty much set in stone as they do their job and don't need any more or any less. But I was wonder about the mutex. Yes the region should be locked down, but should the consumer be held down by the producer gain access to the critical region or vice versa? I added a second mutex so that each producer and consumer would have their own. This is so when the producer is trying to access the buffer, it doesnt block the consumer from removing from the buffer. Both mutexes work to prevent deadlock among the threads that are the same consumer/producer, but wouldnt it work faster if the producer doesnt block the consumer? 
Of course I tested it with the "time ./testbbuffer" and here is my data:
With 2 Mutex:
    5p5c
real	0m19.003s
user	0m0.003s
sys	0m0.002s

real	0m20.003s
user	0m0.000s
sys	0m0.005s

real	0m18.003s
user	0m0.002s
sys	0m0.003s

With 1 Mutex:
    5p5c
real	0m16.003s
user	0m0.003s
sys	0m0.003s

real	0m17.002s
user	0m0.005s
sys	0m0.000s

real	0m16.003s
user	0m0.003s
sys	0m0.002s
_____________________________________
With 2 Mutex:
    1p1c
real	1m8.006s
user	0m0.002s
sys	0m0.003s

With 1 Mutex:
    1p1c
real	1m8.006s
user	0m0.003s
sys	0m0.002s
______________________________________
With 2 Mutex:
    10p10c
real	0m10.003s
user	0m0.002s
sys	0m0.005s

real	0m11.003s
user	0m0.001s
sys	0m0.004s

With 1 Mutex:
    10p10c
real	0m8.002s
user	0m0.003s
sys	0m0.002s

real	0m8.002s
user	0m0.002s
sys	0m0.004s
_____________________________________
With 2 Mutex:
    3p5c
real	0m24.003s
user	0m0.004s
sys	0m0.001s

real	0m21.003s
user	0m0.004s
sys	0m0.002s

With 1 Mutex:
    3p5c
real	0m20.003s
user	0m0.001s
sys	0m0.004s

real	0m20.003s
user	0m0.000s
sys	0m0.005s

As you can see, having two mutexes slows down the process. This is interesting because I figured it should actually speed things up. My idea is that the lock down of a single mutex on a producer could slow down a consumer trying to remove on the buffer. But i guess the slow down could happen cuz there of the extra mutex overhead or because having two mutexes does work but not all the time as in the consumer/producer has the buffer empty/full respectively. 
I honestly not entirely sure why, but unless the time function is wrong, having two mutexes slows down the program. I suppose keeping it minimal is the way to go. Just do enough with the bare minimum. 