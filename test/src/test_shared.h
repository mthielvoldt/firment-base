#ifndef TEST_SHARED_H
#define TEST_SHARED_H

#include <project_msg.pb.h>
#include <fmt_sizes.h>
#include <stdint.h>

bool encode(uint8_t *buff, Top msg);
Top *getGoodMsg(void);

#endif