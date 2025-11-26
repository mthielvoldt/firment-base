#include "fmt_transport.h"
#include "fmt_transport_test.h"
#include <stddef.h>

fmt_startTxChain_t fmt_startTxChain = NULL;
fmt_linkTransport_t fmt_linkTransport = NULL;
fmt_getTransportErrCount_t fmt_getTransportErrCount = NULL;

static transportErrCount_t errCount = {};
static rxCallback_t _rxCallback = NULL;
// static txCallback not needed: it's just a wrapper for dequeue(), already mocked.

static void fmt_startTxChain_test(void)
{
}

static bool fmt_linkTransport_test(txCallback_t pullTxPacket, rxCallback_t rxCallback)
{
  _rxCallback = rxCallback;
  (void)pullTxPacket;
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

rxCallback_t getRxCallback(void)
{
  return _rxCallback;
}
