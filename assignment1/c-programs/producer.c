#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "buffer.h"
static int get_shared_memory() //attach to/create memory
{
    int shmid;
    shmid = shmget(SHM_KEY,
                   sizeof(shared_buffer_t),
                   IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmid;
}

static void open_semaphores(sem_t **empty, //create/open semaphores
                            sem_t **full,
                            sem_t **mutex)
{
    *empty = sem_open(SEM_EMPTY, O_CREAT, 0644, BUFFER_SIZE);
    *full = sem_open(SEM_FULL, O_CREAT, 0644, 0);
    *mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, 1);
    if (*empty == SEM_FAILED ||
        *full == SEM_FAILED ||
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
        printf("Usage: %s <producer_id> <num_items>\n",
               argv[0]);
        return 1;
    }
    int producer_id = atoi(argv[1]);
    int num_items = atoi(argv[2]);
    int shmid = get_shared_memory();
    shared_buffer_t *buffer = (shared_buffer_t *)shmat(shmid, NULL, 0);
    if (buffer == (void *)-1)
    {
        perror("shmat");
        return 1;
    }
    if (buffer->head == 0 && //starts buffer once
        buffer->tail == 0 &&
        buffer->count == 0)
    {
        /* Already effectively initialized. */
    }
    sem_t *empty;
    sem_t *full;
    sem_t *mutex; 
    open_semaphores(&empty, &full, &mutex);

    for (int i = 0; i < num_items; i++)
    {
        item_t item;
        item.producer_id = producer_id;
        item.value = producer_id * 1000 + i;
        sem_wait(empty); //waiting for slot
        sem_wait(mutex); //enters critical section
        buffer->buffer[buffer->head] = item; //inserts item
        buffer->head =
            (buffer->head + 1) % BUFFER_SIZE;
        buffer->count++;
        printf("Producer %d: Produced value %d\n",
               producer_id,
               item.value);
        sem_post(mutex); //exit critical section
        sem_post(full); //signal full item
        usleep(100000);
    }
    shmdt(buffer);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    return 0;
}
