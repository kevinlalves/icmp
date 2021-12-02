#include "../icmp.h"

#include <gtest/gtest.h>

TEST(IcmpTest, Encode) {
    Icmp packet(0x8, 0x0, 0xa2b3, 0x0);
    std::vector<uint8_t> byte_array{8, 0, 0x55, 0x4c, 0xa2, 0xb3, 0, 0}; 
    EXPECT_EQ(packet.Encode(), byte_array);

    packet.IncrementSeq();
    byte_array[3] = 0x4b;
    byte_array[7] = 1; 
    EXPECT_EQ(packet.Encode(), byte_array);

    packet.IncrementSeq();
    byte_array[3] = 0x4a;
    byte_array[7] = 2; 
    EXPECT_EQ(packet.Encode(), byte_array);

}
