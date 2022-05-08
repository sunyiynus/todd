#include "catch2/catch_all.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "utilities.h"
#include <bitset>




TEST_CASE("Test bitset to unsigned char...", "[test function WriteBitsetToUchar]")
{
    std::bitset<16> bitmap;
    REQUIRE(bitmap.size() == 16);
    bitmap.set(0, true);
    bitmap.set(9, true);
    REQUIRE(bitmap[9]);
    REQUIRE(bitmap[0]);
    const unsigned char* bits = todd::WriteBitsetToUchar(bitmap);
    WARN(bits[1]);
    unsigned char bit = (bits[9 / 8] & (todd::SIG_BIT[9 % 8]));
    REQUIRE(bit == static_cast<unsigned char>(todd::SIG_BIT[1]));
    delete [] bits;
    CHECK(false);
    
}
