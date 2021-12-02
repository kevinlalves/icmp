#include "../icmp.h"

#include <gtest/gtest.h>

TEST(IcmpTest, Encode) {
  Icmp *packet = new Icmp(0x8, 0, 0xa2b3, 0x0);
  // byte_array is calculated by you as the theoretical right answer
  std::vector<uint8_t> byte_array{8, 0, 0x55, 0x4c, 0xa2, 0xb3, 0, 0}; 
  EXPECT_EQ(packet->Encode(), byte_array);

  packet->IncrementSeq();
  byte_array[3] = 0x4b;
  byte_array[7] = 1; 
  EXPECT_EQ(packet->Encode(), byte_array);

  packet->IncrementSeq();
  byte_array[3] = 0x4a;
  byte_array[7] = 2; 
  EXPECT_EQ(packet->Encode(), byte_array);
  byte_array.clear();
  delete packet;
  // introducing random values
  packet = new Icmp(0x4a, 0x93, 0x9ddf, 0x3f6);
  byte_array = {0x4a, 0x93, 0x13, 0x97, 0x9d, 0xdf, 0x3, 0xf6};
  EXPECT_EQ(packet->Encode(), byte_array);
  byte_array.clear();
  delete packet;

  packet = new Icmp(0x44, 0x3a, 0x7b27, 0xf754);
  byte_array = {0x44, 0x3a, 0x49, 0x49, 0x7b, 0x27, 0xf7, 0x54};
  EXPECT_EQ(packet->Encode(), byte_array);
  byte_array.clear();
  delete packet;
  // using class function to set new payloads
  packet = new Icmp(0x8, 0, 0x2981, 0xf697);
  int64_t time_epoch = 0x7450768153f8e6; // random 8 byte number
  packet->SetPayload(time_epoch);
  byte_array = {0x8, 0, 0xc, 0xc2, 0x29, 0x81, 0xf6, 0x97, 
                0, 0x74, 0x50, 0x76, 0x81, 0x53, 0xf8, 0xe6}; // payload at second line
  EXPECT_EQ(packet->Encode(), byte_array);
  byte_array.clear();
  delete packet;

  packet = new Icmp(0x8, 0, 0x90ca, 0x1e48);
  time_epoch = 0x58d8beeb8054fa;
  packet->SetPayload(time_epoch);
  byte_array = {0x8, 0, 0x2f, 0x5b, 0x90, 0xca, 0x1e, 0x48, 
                0, 0x58, 0xd8, 0xbe, 0xeb, 0x80, 0x54, 0xfa};
  EXPECT_EQ(packet->Encode(), byte_array);

  delete packet;
}

// generates random byte_arrays to match with the expected packet created
TEST(IcmpTest, Decode) {
  // without payload
  // expected bytes
  std::vector<uint8_t> byte_array{0x5b, 0x4e, 0x8a, 0x46, 0x54, 0xb1, 0xc5, 0xb9};
  // decoded packet
  Icmp packet;
  EXPECT_EQ(packet.Decode(byte_array), 0);
  EXPECT_EQ(packet.Encode(), byte_array);
  byte_array.clear();

  // with payload
  byte_array = {0x8, 0, 0x2f, 0x5b, 0x90, 0xca, 0x1e, 0x48, 
                0, 0x58, 0xd8, 0xbe, 0xeb, 0x80, 0x54, 0xfa};
  EXPECT_EQ(packet.Decode(byte_array), 0);
  EXPECT_EQ(packet.Encode(), byte_array);
  byte_array.clear();

  // corrupted packet (wrong checksum)
  byte_array = {0x5, 0, 0xc, 0xc2, 0x29, 0x81, 0xf6, 0x97, 
                0, 0x74, 0x50, 0x76, 0x81, 0x53, 0xf8, 0xd6};
  EXPECT_EQ(packet.Decode(byte_array), -1);
  EXPECT_EQ(packet.Encode(), byte_array);
}
