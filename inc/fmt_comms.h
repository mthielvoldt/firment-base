#ifndef fmt_comms_h
#define fmt_comms_h

#include <messages.pb.h>

bool fmt_initComms(void);

void reportCommsErrors(void);

/** fmt_sendMsg
 * Queues a message for transmit.
 * Should only be called from a single context - not thread-safe.
 * 
 * These functions are pointerized to facilitate unit-testing.  There is only
 * one implementation, but it's easier to test modules that send or receive data
 * if these can be mocked. 
 * 
 * The following files set these pointers to functions defined therein:
 * - fmt_comms.c
 * - stub_comms.c (test double)
 */
extern bool (*fmt_sendMsg)(Top message);

extern bool (*fmt_getMsg)(Top *message);

#endif // fmt_comms_h