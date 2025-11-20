#include "test_shared.h"
#include <pb_encode.h>

bool encode(uint8_t *buff, Top msg)
{
  uint8_t *txMsg = buff + LENGTH_SIZE_BYTES;
  pb_ostream_t ostream = pb_ostream_from_buffer(txMsg, MAX_MESSAGE_SIZE_BYTES);
  bool encode_ok = pb_encode(&ostream, Top_fields, &msg);
  if (encode_ok)
    buff[LENGTH_POSITION] = ostream.bytes_written;
  return encode_ok;
}

Top *getGoodMsg(void)
{
  static Top goodMsg = {
      .which_sub = Top_FirmentErrorTlm_tag,
      .sub = {.FirmentErrorTlm = {.armRxError = 1}},
  };
  return &goodMsg;
}
