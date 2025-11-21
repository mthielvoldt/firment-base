#include <CppUTestExt/MockSupport.h>

extern "C"
{
#include "queue.h"
#include <string.h>
}

static const uint8_t *deQResult = NULL;
static bool enQReturn = true;

QueueHandle_t xQueueCreateStatic(
    UBaseType_t uxQueueLength,
    UBaseType_t uxItemSize,
    uint8_t *pucQueueStorageBuffer,
    StaticQueue_t *pxQueueBuffer)
{
  return (QueueHandle_t)mock().actualCall("xQueueCreateStatic").returnPointerValue();
}

BaseType_t xQueueSend(
    QueueHandle_t xQueue,
    const void *const pvItemToQueue,
    TickType_t xTicksToWait)
{
  return mock().actualCall("xQueueSend").returnIntValue();
}

BaseType_t xQueueReceive(
    QueueHandle_t xQueue,
    void *const pvBuffer,
    TickType_t xTicksToWait)
{
  return mock()
      .actualCall("xQueueReceive")
      .withOutputParameter("pvBuffer", pvBuffer)
      .returnIntValue();
}

UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue)
{
  return mock().actualCall("uxQueueMessagesWaiting").returnUnsignedIntValue();
}
