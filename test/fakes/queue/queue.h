#ifndef queue_H
#define queue_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define pdTRUE ((BaseType_t)1)
#define pdFALSE ((BaseType_t)0)
#define errQUEUE_EMPTY ((BaseType_t)0)
#define errQUEUE_FULL ((BaseType_t)0)
typedef struct
{
  uint32_t highestSenderPriority; // min numeric value; highest preemtion priority.
  uint_fast16_t maxNumItems;
  volatile uint_fast16_t front;
  volatile uint_fast16_t back;
  volatile uint_fast16_t numItemsWaiting;
  size_t itemSize;
  uint8_t *items;
} StaticQueue_t;

typedef StaticQueue_t *QueueHandle_t;
typedef uint32_t UBaseType_t;
typedef int32_t BaseType_t;
typedef uint32_t TickType_t;

QueueHandle_t xQueueCreateStatic(
    UBaseType_t uxQueueLength,
    UBaseType_t uxItemSize,
    uint8_t *pucQueueStorageBuffer,
    StaticQueue_t *pxQueueBuffer);

BaseType_t xQueueSend(
    QueueHandle_t xQueue,
    const void *const pvItemToQueue,
    TickType_t xTicksToWait);

BaseType_t xQueueReceive(
    QueueHandle_t xQueue,
    void *const pvBuffer,
    TickType_t xTicksToWait);

UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue);

/** Extends from FreeRTOS queue for bare-metal applications that leverage
 * NVIC for priority management.
 */
BaseType_t xQueueSetHighestSenderPriority(
    QueueHandle_t xQueue,
    UBaseType_t priority);

#endif // queue_H
