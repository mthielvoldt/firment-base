#ifndef queue_H
#define queue_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct
{
  uint32_t highestSenderPriority; // min numeric value; highest preemtion priority.
  uint_fast16_t maxNumItems;
  volatile uint_fast16_t front;
  volatile uint_fast16_t back;
  volatile uint_fast16_t numItemsWaiting;
  size_t itemSize;
  uint8_t *items;
} queue_t;

bool initQueue(
    size_t itemSize, uint32_t length, queue_t *queue, uint8_t *itemsStore, 
    uint32_t highestSenderPriority);

bool enqueueBack(queue_t *queue, const void *src);

bool enqueueFront(queue_t *queue, const void *src);

bool peekFront(queue_t *queue, void *result);

bool dequeueFront(queue_t *queue, void *result);

bool dequeueBack(queue_t *queue, void *result);

uint32_t numItemsInQueue(queue_t *queue);

uint32_t emptySpacesInQueue(queue_t *queue);

#endif // queue_H
