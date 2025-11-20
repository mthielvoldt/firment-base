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
    mock().expectNCalls(2, "initQueue").andReturnValue(true);
    fmt_initComms();
  };
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};

bool fake_sendMsg(Top msg)
{
  return mock().actualCall("fmt_sendMsg").withParameter("which_sub", msg.which_sub).returnBoolValue();
}

void expectNoErrors(void)
{
  UT_PTR_SET(fmt_sendMsg, fake_sendMsg);
  mock().expectNoCall("fmt_sendMsg");
  reportCommsErrors();
}

void expectErrors(void)
{
  UT_PTR_SET(fmt_sendMsg, fake_sendMsg);
  mock().expectOneCall("fmt_sendMsg").
    withIntParameter("which_sub", Top_FirmentErrorTlm_tag).andReturnValue(true);
  reportCommsErrors();
}

TEST(fmt_base, initComms_succeeds)
{
  // CHecks just setup.
}

TEST(fmt_base, rxCallback_enqueues_if_crc_good)
{
  // stimulus
  const rxCallback_t rxCallback = getRxCallback();
  const uint8_t packet[MAX_PACKET_SIZE_BYTES] = {};

  // check
  mock().expectOneCall("enqueueBack").andReturnValue(true);
  rxCallback(packet);

  expectNoErrors();
}


// TEST(fmt_base, rxCallback_throws_if_crc_bad)
// {
// }

TEST(fmt_base, rxCallback_throws_if_rxQ_full)
{
  const rxCallback_t rxCallback = getRxCallback();
  const uint8_t packet[MAX_PACKET_SIZE_BYTES] = {};

  mock().expectOneCall("enqueueBack").andReturnValue(false);
  rxCallback(packet);

  expectErrors();
}

TEST(fmt_base, sendMsg_enqueues_with_crc_if_encode_succeeds)
{
  Top *msg = getGoodMsg();
  mock().expectOneCall("enqueueBack").andReturnValue(true);
  fmt_sendMsg(*msg);
  expectNoErrors();
}

TEST(fmt_base, sendMsg_throws_if_sendQ_full)
{
  Top *msg = getGoodMsg();
  mock().expectOneCall("enqueueBack").andReturnValue(false);
  fmt_sendMsg(*msg);
  expectErrors();
}

TEST(fmt_base, sendMsg_throws_if_encode_fails)
{
  Top badMsg = {};
  mock().expectNoCall("enqueueBack");
  fmt_sendMsg(badMsg);
  expectErrors();
}
/*

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
