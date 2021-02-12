#include "Pipeline/Crypto/OpensslStream.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"

#define DEFINE_AND_REGISTER(family, size, mode)                                \
  using _##family##_##size##_##mode##_ =                                       \
      Owl::OpensslStream<(size >> 3), 16, EVP_##family##_##size##_##mode>;     \
  static const Owl::CryptoFactory::Register<_##family##_##size##_##mode##_>    \
      register_##family##_##size##_##mode##_(#family "-" #size "-" #mode);

DEFINE_AND_REGISTER(aes, 128, cfb)
DEFINE_AND_REGISTER(aes, 192, cfb)
DEFINE_AND_REGISTER(aes, 256, cfb)

DEFINE_AND_REGISTER(aes, 128, ctr)
DEFINE_AND_REGISTER(aes, 192, ctr)
DEFINE_AND_REGISTER(aes, 256, ctr)

DEFINE_AND_REGISTER(camellia, 128, cfb)
DEFINE_AND_REGISTER(camellia, 192, cfb)
DEFINE_AND_REGISTER(camellia, 256, cfb)