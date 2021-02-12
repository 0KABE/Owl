#include <gtest/gtest.h>
#include "Pipeline/Crypto/Hkdf.hpp"

using namespace Owl;

TEST(HKDF, extract) {
    std::vector<uint8_t> salt{0x0, 0x1, 0x2};
    std::vector<uint8_t> key{0x0, 0x1, 0x2};

    auto result = HKDF::Extract(salt, key, EVP_get_digestbyname("sha256"));

    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    std::for_each(result.begin(), result.end(),
                  [&](auto i) { ss << std::setw(2) << (uint16_t) i; });
    ASSERT_EQ(ss.str(),
              "6386d4f49e49638d0f6ea47df958f7a899be63b4d08666d687f6740e5c026261");
};

TEST(HKDF, expand1) {
    std::vector<uint8_t> salt{0x0, 0x1, 0x2};
    std::vector<uint8_t> key{0x0, 0x1, 0x2};

    HKDF hkdf(salt, key, "sha256");
    std::vector<uint8_t> result(32);
    hkdf.Expand(result);
    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    std::for_each(result.begin(), result.end(),
                  [&](auto i) { ss << std::setw(2) << (uint16_t) i; });
    ASSERT_EQ(ss.str(),
              "8a967f57fcce432f0102074a4740bd970d471f1caf5c337e3f41013b32fd7bb7");
}

TEST(HKDF, expand2) {
    std::vector<uint8_t> salt(32, 0x00);
    std::vector<uint8_t> key{0x6f, 0xa5, 0x3a, 0x8, 0xcd, 0x12, 0xf, 0x86,
                             0x26, 0x45, 0xe3, 0xaa, 0x6b, 0x62, 0x4, 0x20,
                             0xd5, 0x1b, 0x80, 0x40, 0xac, 0x6e, 0xe8, 0x45,
                             0x1, 0x76, 0x9d, 0xa2, 0xc5, 0x28, 0xc7, 0xe5};

    {
        auto extract_key = HKDF::Extract(salt, key, EVP_get_digestbyname("sha1"));
        std::stringstream ss;
        ss << std::setfill('0') << std::hex;
        std::for_each(extract_key.begin(), extract_key.end(),
                      [&](auto i) { ss << std::setw(2) << (uint16_t) i; });
        ASSERT_EQ(ss.str(), "00e0360fa7a1d15001a14732cbd3d93305e24312");
    }

    {
        HKDF hkdf(salt, key, "sha1");
        std::vector<uint8_t> result(32);
        hkdf.Expand(result);
        std::stringstream ss;
        ss << std::setfill('0') << std::hex;
        std::for_each(result.begin(), result.end(),
                      [&](auto i) { ss << std::setw(2) << (uint16_t) i; });
        ASSERT_EQ(
                ss.str(),
                "2924f69f13a35280e3a56a62195c4221dac1dce3c1ec81191a71b92c34c9b42c");
    }
}

TEST(HKDF, expand3) {
    std::vector<uint8_t> salt(32, 0x00);
    std::vector<uint8_t> key(32, 0x55);

    HKDF hkdf(salt, key, "sha1");
    std::vector<uint8_t> result(32);
    hkdf.Expand(result);
    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    std::for_each(result.begin(), result.end(),
                  [&](auto i) { ss << std::setw(2) << (uint16_t) i; });
    ASSERT_EQ(ss.str(),
              "f4954937b32d004141dbd1dfd9379c69843819a3dd5446e8939d6e0ead9ff7c1");
}

