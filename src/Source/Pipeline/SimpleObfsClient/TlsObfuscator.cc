#include "Pipeline/SimpleObfsClient/TlsObfuscator.hpp"
#include "Pipeline/SimpleObfsClient/SimpleObfsClientFactory.hpp"
#include <boost/endian.hpp>
#include <random>

using boost::endian::big_to_native;
using boost::endian::native_to_big;

#define CT_HTONS(x) native_to_big<uint16_t>(x)
#define CT_HTONL(x) native_to_big<uint32_t>(x)
#define __PACKED __attribute__((__packed__))
#define __SFINIT(member, ...) member = __VA_ARGS__

struct ClientHello {
    uint8_t content_type;
    uint16_t version;
    uint16_t len;

    uint8_t handshake_type;
    uint8_t handshake_len_1;
    uint16_t handshake_len_2;
    uint16_t handshake_version;

    uint32_t random_unix_time;
    uint8_t random_bytes[28];
    uint8_t session_id_len;
    uint8_t session_id[32];
    uint16_t cipher_suites_len;
    uint8_t cipher_suites[56];
    uint8_t comp_methods_len;
    uint8_t comp_methods[1];
    uint16_t ext_len;
} __PACKED;

struct ExtServerName {
    uint16_t ext_type;
    uint16_t ext_len;
    uint16_t server_name_list_len;
    uint8_t server_name_type;
    uint16_t server_name_len;
} __PACKED;

struct ExtSessionTicket {
    uint16_t session_ticket_type;
    uint16_t session_ticket_ext_len;
} __PACKED;

struct ExtOthers {
    uint16_t ec_point_formats_ext_type;
    uint16_t ec_point_formats_ext_len;
    uint8_t ec_point_formats_len;
    uint8_t ec_point_formats[3];

    uint16_t elliptic_curves_type;
    uint16_t elliptic_curves_ext_len;
    uint16_t elliptic_curves_len;
    uint8_t elliptic_curves[8];

    uint16_t sig_algos_type;
    uint16_t sig_algos_ext_len;
    uint16_t sig_algos_len;
    uint8_t sig_algos[30];

    uint16_t encrypt_then_mac_type;
    uint16_t encrypt_then_mac_ext_len;

    uint16_t extended_master_secret_type;
    uint16_t extended_master_secret_ext_len;
} __PACKED;

struct ServerHello {
    uint8_t content_type;
    uint16_t version;
    uint16_t len;

    uint8_t handshake_type;
    uint8_t handshake_len_1;
    uint16_t handshake_len_2;
    uint16_t handshake_version;

    uint32_t random_unix_time;
    uint8_t random_bytes[28];
    uint8_t session_id_len;
    uint8_t session_id[32];
    uint16_t cipher_suite;
    uint8_t comp_method;
    uint16_t ext_len;

    uint16_t ext_renego_info_type;
    uint16_t ext_renego_info_ext_len;
    uint8_t ext_renego_info_len;

    uint16_t extended_master_secret_type;
    uint16_t extended_master_secret_ext_len;

    uint16_t ec_point_formats_ext_type;
    uint16_t ec_point_formats_ext_len;
    uint8_t ec_point_formats_len;
    uint8_t ec_point_formats[1];
} __PACKED;

struct ChangeCipherSpec {
    uint8_t content_type;
    uint16_t version;
    uint16_t len;
    uint8_t msg;
} __PACKED;

struct EncryptedHandshake {
    uint8_t content_type;
    uint16_t version;
    uint16_t len;
} __PACKED;

static const ClientHello kClientHelloTemplate = {
        __SFINIT(.content_type, 0x16),
        __SFINIT(.version, CT_HTONS(0x0301)),
        __SFINIT(.len, 0),

        __SFINIT(.handshake_type, 1),
        __SFINIT(.handshake_len_1, 0),
        __SFINIT(.handshake_len_2, 0),
        __SFINIT(.handshake_version, CT_HTONS(0x0303)),

        __SFINIT(.random_unix_time, 0),
        __SFINIT(.random_bytes, { 0 }),

        __SFINIT(.session_id_len, 32),
        __SFINIT(.session_id, { 0 }),

        __SFINIT(.cipher_suites_len, CT_HTONS(56)),
        __SFINIT(.cipher_suites,
                 {
                     0xc0, 0x2c, 0xc0, 0x30, 0x00, 0x9f, 0xcc, 0xa9, 0xcc, 0xa8,
                     0xcc, 0xaa, 0xc0, 0x2b, 0xc0, 0x2f, 0x00, 0x9e, 0xc0, 0x24,
                     0xc0, 0x28, 0x00, 0x6b, 0xc0, 0x23, 0xc0, 0x27, 0x00, 0x67,
                     0xc0, 0x0a, 0xc0, 0x14, 0x00, 0x39, 0xc0, 0x09, 0xc0, 0x13,
                     0x00, 0x33, 0x00, 0x9d, 0x00, 0x9c, 0x00, 0x3d, 0x00, 0x3c,
                     0x00, 0x35, 0x00, 0x2f, 0x00, 0xff
                 }),

        __SFINIT(.comp_methods_len, 1),
        __SFINIT(.comp_methods, { 0 }),

        __SFINIT(.ext_len, 0),
};

static const ExtServerName kExtServerNameTemplate = {
        __SFINIT(.ext_type, 0),
        __SFINIT(.ext_len, 0),
        __SFINIT(.server_name_list_len, 0),
        __SFINIT(.server_name_type, 0),
        __SFINIT(.server_name_len, 0),
};

static const ExtSessionTicket kExtSessionTicketTemplate = {
        __SFINIT(.session_ticket_type, CT_HTONS(0x0023)),
        __SFINIT(.session_ticket_ext_len, 0),
};

static const ExtOthers kExtOthersTemplate = {
        __SFINIT(.ec_point_formats_ext_type, CT_HTONS(0x000B)),
        __SFINIT(.ec_point_formats_ext_len, CT_HTONS(4)),
        __SFINIT(.ec_point_formats_len, 3),
        __SFINIT(.ec_point_formats, { 0x01, 0x00, 0x02 }),

        __SFINIT(.elliptic_curves_type, CT_HTONS(0x000a)),
        __SFINIT(.elliptic_curves_ext_len, CT_HTONS(10)),
        __SFINIT(.elliptic_curves_len, CT_HTONS(8)),
        __SFINIT(.elliptic_curves,
                 { 0x00, 0x1d, 0x00, 0x17, 0x00, 0x19, 0x00, 0x18 }),

        __SFINIT(.sig_algos_type, CT_HTONS(0x000d)),
        __SFINIT(.sig_algos_ext_len, CT_HTONS(32)),
        __SFINIT(.sig_algos_len, CT_HTONS(30)),
        __SFINIT(.sig_algos,
                 {
                     0x06, 0x01, 0x06, 0x02, 0x06, 0x03, 0x05, 0x01, 0x05, 0x02,
                     0x05, 0x03, 0x04, 0x01, 0x04, 0x02, 0x04, 0x03, 0x03, 0x01,
                     0x03, 0x02, 0x03, 0x03, 0x02, 0x01, 0x02, 0x02, 0x02, 0x03
                 }),

        __SFINIT(.encrypt_then_mac_type, CT_HTONS(0x0016)),
        __SFINIT(.encrypt_then_mac_ext_len, 0),

        __SFINIT(.extended_master_secret_type, CT_HTONS(0x0017)),
        __SFINIT(.extended_master_secret_ext_len, 0),
};

static const ServerHello kServerHelloTemplate = {
        __SFINIT(.content_type, 0x16),
        __SFINIT(.version, CT_HTONS(0x0301)),
        __SFINIT(.len, CT_HTONS(91)),

        __SFINIT(.handshake_type, 2),
        __SFINIT(.handshake_len_1, 0),
        __SFINIT(.handshake_len_2, CT_HTONS(87)),
        __SFINIT(.handshake_version, CT_HTONS(0x0303)),

        __SFINIT(.random_unix_time, 0),
        __SFINIT(.random_bytes, { 0 }),

        __SFINIT(.session_id_len, 32),
        __SFINIT(.session_id, { 0 }),

        __SFINIT(.cipher_suite, CT_HTONS(0xCCA8)),
        __SFINIT(.comp_method, 0),
        __SFINIT(.ext_len, 0),

        __SFINIT(.ext_renego_info_type, CT_HTONS(0xFF01)),
        __SFINIT(.ext_renego_info_ext_len, CT_HTONS(1)),
        __SFINIT(.ext_renego_info_len, 0),

        __SFINIT(.extended_master_secret_type, CT_HTONS(0x0017)),
        __SFINIT(.extended_master_secret_ext_len, 0),

        __SFINIT(.ec_point_formats_ext_type, CT_HTONS(0x000B)),
        __SFINIT(.ec_point_formats_ext_len, CT_HTONS(2)),
        __SFINIT(.ec_point_formats_len, 1),
        __SFINIT(.ec_point_formats, { 0 }),
};

static const ChangeCipherSpec kChangeCipherSpecTemplate = {
        __SFINIT(.content_type, 0x14),
        __SFINIT(.version, CT_HTONS(0x0303)),
        __SFINIT(.len, CT_HTONS(1)),
        __SFINIT(.msg, 0x01),
};

static const EncryptedHandshake kEncryptedHandshakeTemplate = {
        __SFINIT(.content_type, 0x16),
        __SFINIT(.version, CT_HTONS(0x0303)),
        __SFINIT(.len, 0),
};

Owl::Buffer &Owl::TlsObfuscator::Obfuscate(Owl::Buffer &buffer) {
    if (!mHandleOBFS) {
        Buffer tmp;

        size_t buf_len = buffer.size();
        size_t hello_len = sizeof(ClientHello);
        size_t server_name_len = sizeof(ExtServerName);
        size_t host_len = mHostname.size();
        size_t ticket_len = sizeof(ExtSessionTicket);
        size_t other_ext_len = sizeof(ExtOthers);
        size_t tls_len = buf_len + hello_len + server_name_len + host_len +
                         ticket_len + other_ext_len;

        boost::asio::buffer_copy(tmp.prepare(buffer.size()), buffer.data());
        tmp.commit(buffer.size());
        buffer.consume(buffer.size());
        boost::asio::mutable_buffer prependData = buffer.prepare(tls_len);

        /* Client Hello Header */
        ClientHello *hello = static_cast<ClientHello *>(prependData.data());
        memcpy(hello, &kClientHelloTemplate, hello_len);
        hello->len = CT_HTONS(tls_len - 5);
        hello->handshake_len_2 = CT_HTONS(tls_len - 9);
        hello->random_unix_time = CT_HTONL((uint32_t) time(NULL));
        RandBytes(hello->random_bytes, 28);
        RandBytes(hello->session_id, 32);
        hello->ext_len = CT_HTONS(tls_len - hello_len);

        /* Session Ticket */
        ExtSessionTicket *ticket =
                (ExtSessionTicket *) ((uint8_t *) hello + hello_len);
        memcpy(ticket, &kExtSessionTicketTemplate, ticket_len);
        ticket->session_ticket_ext_len = CT_HTONS(buf_len);
        memcpy((uint8_t *) ticket + ticket_len, tmp.data().data(), buf_len);

        /* SNI */
        ExtServerName *server_name =
                (ExtServerName *) ((uint8_t *) ticket + ticket_len + buf_len);
        memcpy(server_name, &kExtServerNameTemplate, server_name_len);
        server_name->ext_len = CT_HTONS(host_len + 3 + 2);
        server_name->server_name_list_len = CT_HTONS(host_len + 3);
        server_name->server_name_len = CT_HTONS(host_len);
        memcpy((uint8_t *) server_name + server_name_len, mHostname.data(), host_len);

        /* Other Extensions */
        memcpy((uint8_t *) server_name + server_name_len + host_len,
               &kExtOthersTemplate, other_ext_len);

        buffer.commit(tls_len);
        mHandleOBFS = true;
    } else {
        ObfsAppData(buffer);
    }

    return buffer;
}

Owl::Buffer &Owl::TlsObfuscator::DeObfuscate(Owl::Buffer &buffer) {
    bool finish = false;

    while (!finish) {

        switch (deObfsStatus) {
            case RECEIVE_HELLO: {
                size_t size = sizeof(ServerHello) + sizeof(ChangeCipherSpec);
                if (buffer.size() >= size) {
                    buffer.consume(size);
                    deObfsStatus = RECEIVE_FRAME_LEN;
                } else {
                    finish = true;
                }
                break;
            }

            case RECEIVE_FRAME_LEN: {
                size_t size = sizeof(EncryptedHandshake);
                if (buffer.size() >= size) {
                    const EncryptedHandshake *encryptedHandshake =
                            static_cast<const EncryptedHandshake *>(buffer.data().data());
                    frame.len =
                            boost::endian::big_to_native<uint16_t>(encryptedHandshake->len);
                    buffer.consume(size);
                    deObfsStatus = RECEIVE_FRAME;
                } else {
                    finish = true;
                }
                break;
            }

            case RECEIVE_FRAME:
                if (buffer.size() >= frame.len) {
                    auto mutableBuff = frame.buff.prepare(frame.len);
                    boost::asio::buffer_copy(mutableBuff, buffer.data());
                    buffer.consume(frame.len);
                    frame.buff.commit(frame.len);
                    deObfsStatus = RECEIVE_FRAME_LEN;
                } else {
                    finish = true;
                }
                break;
        }
    }

    return frame.buff;
}

void Owl::TlsObfuscator::ObfsAppData(Owl::Buffer &buffer) {
    Header header;
    header.length.uInt16 = boost::endian::native_to_big<uint16_t>(buffer.size());

    Buffer tmp;
    memcpy(tmp.prepare(5).data(), &header, sizeof header);
    tmp.commit(sizeof header);

    boost::asio::buffer_copy(tmp.prepare(buffer.size()), buffer.data());
    tmp.commit(buffer.size());
    buffer.consume(buffer.size());
    boost::asio::buffer_copy(buffer.prepare(tmp.size()), tmp.data());
    buffer.commit(tmp.size());
}

void Owl::TlsObfuscator::RandBytes(uint8_t *buf, size_t len) {
    static std::default_random_engine e{std::random_device{}()};
    static std::uniform_int_distribution<uint16_t> u{0, 255};
    std::generate_n(buf, len, [&] { return u(e); });
}

static auto reg = Owl::SimpleObfsClientFactory::Register<Owl::TlsObfuscator>("tls");
