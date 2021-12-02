#include "../icmp.h"

#include <gtest/gtest.h>

TEST(IcmpTest, Encode) {
    Icmp *packet = new Icmp(0x8, 0x0, 0xa2b3, 0x0);
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
    delete packet;
    // introducing random values
    packet = new Icmp(0x4a, 0x93, 0x9ddf, 0x3f6);
    byte_array[0] = 0x4a;
    byte_array[1] = 0x93;
    byte_array[2] = 0x13;
    byte_array[3] = 0x97;
    byte_array[4] = 0x9d;
    byte_array[5] = 0xdf;
    byte_array[6] = 3;
    byte_array[7] = 0xf6;
    EXPECT_EQ(packet->Encode(), byte_array);

    delete packet;
}
