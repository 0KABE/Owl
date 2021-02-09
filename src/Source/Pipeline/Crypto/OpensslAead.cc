#include "Pipeline/Crypto/OpensslAead.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"

#define DEFINE_AND_REGISTER(family, size, mode)                                \
  using _##family##_##size##_##mode##_ =                                       \
      Owl::OpensslAEAD<(size >> 3), (size >> 3), 12, 16,                            \
                  EVP_##family##_##size##_##mode>;                             \
  static const Owl::CryptoFactory::Register<_##family##_##size##_##mode##_>         \
      register_##family##_##size##_##mode##_(#family "-" #size "-" #mode);

DEFINE_AND_REGISTER(aes, 128, gcm)
DEFINE_AND_REGISTER(aes, 192, gcm)
DEFINE_AND_REGISTER(aes, 256, gcm)