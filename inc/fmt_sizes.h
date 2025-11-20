#ifndef FMT_SIZES_H
#define FMT_SIZES_H

#include <stdint.h>

#define LENGTH_SIZE_BYTES 1U // a prefix indicating subsequent message length
#define LENGTH_POSITION 0
#define PAYLOAD_POSITION (LENGTH_POSITION + LENGTH_SIZE_BYTES)
#define CRC_SIZE_BYTES 2U
#define MAX_PACKET_SIZE_BYTES 64U // Must be less than 259; len sent in 1 byte.
#define MAX_MESSAGE_SIZE_BYTES \
  (MAX_PACKET_SIZE_BYTES - LENGTH_SIZE_BYTES - CRC_SIZE_BYTES)

/* in the next line, 13 breaks down as follows:
4B count 4B value, 1B each for types: (count, value, text, sub, top) */
#define MAX_LOG_TEXT_SIZE (MAX_MESSAGE_SIZE_BYTES - 13)
#define SEND_QUEUE_LENGTH 10U
#define RX_QUEUE_LENGTH 9U
#define MAX_SENDER_PRIORITY 16U

/** Get the position of the CRC in bytes from the first element of the packet.
 * CRC position must be 16-bit aligned (even number) for hardware CRC engines.
 * so if the length of the buffer (including length prefix,) is odd, there
 * will be a byte of padding, between the buffer and the CRC.
 * The value of the padding byte (if present) will be checked by the CRC, but it
 * has no effect on the decoded message.
 */
inline static uint32_t getCRCPosition(const uint8_t *packet)
{
  return ((packet[LENGTH_POSITION] + PAYLOAD_POSITION + 1) >> 1) << 1;
}

#endif // FMT_SIZES_H
