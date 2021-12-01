#include "../icmp.h"

#include <gtest/gtest.h>

TEST(IcmpTest, Encode) {
    Icmp packet(0x8, 0x0, 0xa2b3, 0x0);
    uint8_t temp[] = {8, 0, 0x4c, 0x55, 0xb3, 0xa2, 0, 0};
    std::vector<uint8_t> byte_array(temp, temp+sizeof(temp)); 
    EXPECT_EQ(packet.Encode(), byte_array);
}

TEST(IcmpTest, Decode) {
    Icmp packet;
}
