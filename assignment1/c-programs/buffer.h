//Assignment A2
//Keaton Surfield
#ifndef BUFFER_H
#define BUFFER_H
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h> 
#define SHM_KEY       0x1234
#define BUFFER_SIZE   10
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL  "/sem_full"
#define SEM_MUTEX "/sem_mutex"
typedef struct
{
    int value;         
    int producer_id;   
} item_t;
typedef struct
{
    item_t buffer[BUFFER_SIZE];
    int head;          
    int tail;          
    int count;          
} shared_buffer_t; 
int create_shared_memory(void);
int open_shared_memory(void);
shared_buffer_t *attach_shared_memory(int shmid);
void detach_shared_memory(shared_buffer_t *buffer);
sem_t *open_empty_semaphore(void); //semaphore opens
sem_t *open_full_semaphore(void);
sem_t *open_mutex_semaphore(void);
#endif
