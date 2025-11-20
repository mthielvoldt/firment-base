#include <CppUTestExt/MockSupport.h>

extern "C" {
#include "queue.h"
#include <string.h>
}

static const uint8_t *deQResult = NULL;
static bool enQReturn = true;

bool initQueue(
    size_t itemSize, uint32_t length, queue_t *queue, uint8_t *itemsStore,
    uint32_t highestSenderPriority)
{
  return mock().actualCall("initQueue").returnBoolValue();
}

bool enqueueBack(queue_t *queue, const void *src)
{
  return mock().actualCall("enqueueBack").returnBoolValue();
}

bool dequeueFront(queue_t *queue, void *result)
{
  if (deQResult)
  {
    memcpy(result, deQResult, queue->itemSize);
    return true;
  }
  else
  {
    return false;
  }
}

uint32_t numItemsInQueue(queue_t *queue)
{
  return 0;
}