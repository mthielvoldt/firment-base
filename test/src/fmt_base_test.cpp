#include <CppUTest/TestHarness.h>
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "test_shared.h"
#include <fmt_comms.h>
#include <fmt_transport_test.h>
#include <signal.h> // for debugging tests: raise(SIGINT);
}

TEST_GROUP(fmt_base)
{
  void setup()
  {
  };
  void teardown()
  {  
    mock().clear();
  };
};

TEST(fmt_base, initComms_succeeds)
{
  mock().expectNCalls(2, "initQueue").andReturnValue(true);
  fmt_initComms();
  mock().checkExpectations();
}
/*
TEST(fmt_base, rxCallback_enqueues_if_crc_good)
{
  // stimulus
  const rxCallback_t rxCallback = getRxCallback();
  const uint8_t packet[MAX_PACKET_SIZE_BYTES] = {};
  setNextCrcCheckResult(true);
  rxCallback(packet);
  
  // check
  queue_t *rxQueue = getSendQueue();
  CHECK_EQUAL(1, numItemsInQueue(rxQueue));
}


TEST(fmt_base, rxCallback_throws_if_crc_bad)
{
}

TEST(fmt_base, rxCallback_throws_if_rxQ_full)
{
}

TEST(fmt_base, sendMsg_enqueues_with_crc_if_encode_succeeds)
{
}

TEST(fmt_base, sendMsg_throws_if_sendQ_full)
{
}

TEST(fmt_base, sendMsg_throws_if_encode_fails)
{
}

TEST(fmt_base, getMsg_places_decoded_if_rxQ_has_packet)
{
}

TEST(fmt_base, getMsg_returns_false_if_rxQ_empty)
{
}

TEST(fmt_base, getMsg_throws_if_decode_fails)
{
}

TEST(fmt_base, getMsg_throws_if_crc_bad)
{
}

TEST(fmt_base, reportCommsErrors_relays_error_counts_to_sendQ)
{
}

*/
