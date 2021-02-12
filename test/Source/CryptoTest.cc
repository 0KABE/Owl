#include "Pipeline/Crypto/CryptoFactory.hpp"
#include <gtest/gtest.h>

using namespace Owl;

#define Test_Case(test_name, crypto_method)                                    \
  TEST(CryptoFactory, test_name) {                                             \
    std::string       plain    = crypto_method "_plain";                       \
    const std::string password = crypto_method "_password";                    \
    const std::string method   = crypto_method;                                \
                                                                               \
    auto crypto = CryptoFactory::GetInstance().Build(method, password);        \
                                                                               \
    boost::asio::streambuf in;                                                 \
    for (int i = 0; i < 1000; ++i) {                                           \
      std::ostream os(&in);                                                    \
      os << plain;                                                             \
      auto &wrapped   = crypto->Wrap(in);                                      \
      auto &unwrapped = crypto->UnWrap(wrapped);                               \
                                                                               \
      std::istream is(&unwrapped);                                             \
      std::string  result;                                                     \
      is >> result;                                                            \
      ASSERT_EQ(plain, result);                                                \
      plain += "-append_data";                                                 \
    }                                                                          \
  }

// Stream
// clang-format off
Test_Case(rc4_md5, "rc4-md5")

Test_Case(aes_128_ctr, "aes-128-ctr")

Test_Case(aes_192_ctr, "aes-192-ctr")

Test_Case(aes_256_ctr, "aes-256-ctr")

Test_Case(aes_128_cfb, "aes-128-cfb")

Test_Case(aes_192_cfb, "aes-192-cfb")

Test_Case(aes_256_cfb, "aes-256-cfb")

Test_Case(chacha20_ietf, "chacha20-ietf")

Test_Case(camellia_128_cfb, "camellia-128-cfb")

Test_Case(camellia_192_cfb, "camellia-192-cfb")

Test_Case(camellia_256_cfb, "camellia-256-cfb")

// AEAD
Test_Case(aes_128_gcm, "aes-128-gcm")

Test_Case(aes_192_gcm, "aes-192-gcm")

Test_Case(aes_256_gcm, "aes-256-gcm")

Test_Case(chacha20_ietf_poly1305, "chacha20-ietf-poly1305")
