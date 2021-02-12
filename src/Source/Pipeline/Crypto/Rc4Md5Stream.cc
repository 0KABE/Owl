#include "Pipeline/Crypto/Rc4Md5Stream.hpp"
#include "Pipeline/Crypto/CryptoFactory.hpp"

using Rc4Md5 = Owl::RC4MD5Stream<16, 16>;
static const Owl::CryptoFactory::Register<Rc4Md5> rc4_md5("rc4-md5");