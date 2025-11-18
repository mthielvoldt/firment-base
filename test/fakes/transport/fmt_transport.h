/** fmt_transport.h
 * Common interface for the firment transport layers (spi, uart, etc.)
 * Dependency graph: (interfaces)
 *
 * (fmt_comms.h) -> fmt_comms.c -> (fmt_transport.h) -> [fmt_spi.c / fmt_uart.c]
 *
 * These are function pointers for two reasons:
 * 1. Several communication transports (spi, uart, can...) may implement this interface.
 * 2. Unit tests can cover fmt_comms.c using spys at the transport layer.
 * 3. Unit tests can compile all transports and switch which one is active.
 *
 * TODO: SOURCE THIS FILE FROM CENTRAL LOCATION
 */
#ifndef fmt_transport_H
#define fmt_transport_H

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"

typedef volatile struct _transportErrCount
{
  uint16_t dataLost;
  uint16_t unhandledArmEvent;
  uint16_t armRxError;
  uint8_t modeFault;
} transportErrCount_t;

typedef void (*rxCallback_t)(const uint8_t *rxData);

extern void (*fmt_startTxChain)(void);
extern bool (*fmt_linkTransport)(queue_t *sendQueue, rxCallback_t rxCallback);
extern const transportErrCount_t *(*fmt_getTransportErrCount)(void);

bool fmt_initTransport(void);

#endif // fmt_transport_H