#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "buffer.h"
static int get_shared_memory() //connecting shared memory
{
    int shmid;
    shmid = shmget(SHM_KEY,
                   sizeof(shared_buffer),
                   0666);

    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

static void open_semaphores(sem_t **empty, sem_t **full, sem_t **mutex) //open sephamores
{
    *empty = sem_open(SEM_EMPTY, 0);
    *full  = sem_open(SEM_FULL, 0);
    *mutex = sem_open(SEM_MUTEX, 0);
    if (*empty == SEM_FAILED ||
        *full  == SEM_FAILED ||
        *mutex == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <consumer_id> <num_items>\n",
               argv[0]);
        return 1;
    }
    int consumer_id = atoi(argv[1]);
    int num_items   = atoi(argv[2]);
    int shmid = get_shared_memory();
    shared_buffer_t *buffer =
        (shared_buffer_t *)shmat(shmid, NULL, 0);
    if (buffer == (void *)-1)
    {
        perror("shmat");
        return 1;
    }
    sem_t *empty;
    sem_t *full;
    sem_t *mutex;
    open_semaphores(&empty,
                    &full,
                    &mutex);

    for (int i = 0; i < num_items; i++)
    {
        item_t item;
        sem_wait(full); //wait until item exists
        sem_wait(mutex); //enter critical sectiob
        item = buffer->buffer[buffer->tail]; //remove item
        buffer->tail =
            (buffer->tail) % BUFFER_SIZE;
        buffer->count--;
        printf("Consumer %d: Consumed value %d from Producer %d\n",
               consumer_id,
               item.value,
               item.producer_id);
        sem_post(mutex); //leave critical section
        sem_post(empty); //notify producer that slot free

        usleep(150000);
    }
    shmdt(buffer);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    return 0;
}
