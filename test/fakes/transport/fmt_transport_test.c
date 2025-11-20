#include "fmt_transport.h"
#include "fmt_transport_test.h"

void (*fmt_startTxChain)(void) = NULL;
bool (*fmt_linkTransport)(queue_t *sendQueue, rxCallback_t rxCallback) = NULL;
const transportErrCount_t *(*fmt_getTransportErrCount)(void) = NULL;

static transportErrCount_t errCount = {};
static rxCallback_t _rxCallback;
static queue_t *_sendQueue;

static void fmt_startTxChain_test(void)
{
}

static bool fmt_linkTransport_test(queue_t *sendQueue, rxCallback_t rxCallback)
{
  _rxCallback = rxCallback;
  _sendQueue = sendQueue;
  return true;
}

static const transportErrCount_t* getErrCount_test(void) 
{ 
  return &errCount;
}

bool fmt_initTransport(void)
{
  fmt_linkTransport = fmt_linkTransport_test;
  fmt_startTxChain = fmt_startTxChain_test;
  fmt_getTransportErrCount = getErrCount_test;
  return true;
}

const rxCallback_t getRxCallback(void)
{
  return _rxCallback;
}

queue_t* getSendQueue(void)
{
  return _sendQueue;
}
