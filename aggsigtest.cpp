#include "aggsigtest.hpp"
#include "sha256.hpp"
#include "blake3.h"
#include "blake2s.h"

using namespace bls12_381;

void aggsigtest::testg1add(const std::vector<uint8_t>& op1, const std::vector<uint8_t>& op2, const std::vector<uint8_t>& res)
{
    bls_g1 r;
    const bls_g1* lhs = reinterpret_cast<const bls_g1*>(op1.data());
    const bls_g1* rhs = reinterpret_cast<const bls_g1*>(op2.data());
    bls_g1_add(*lhs, *rhs, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g1)), "bls_g1_add test failed");
}

void aggsigtest::testg2add(const std::vector<uint8_t>& op1, const std::vector<uint8_t>& op2, const std::vector<uint8_t>& res)
{
    bls_g2 r;
    const bls_g2* lhs = reinterpret_cast<const bls_g2*>(op1.data());
    const bls_g2* rhs = reinterpret_cast<const bls_g2*>(op2.data());
    bls_g2_add(*lhs, *rhs, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g2)), "bls_g2_add test failed");
}

void aggsigtest::testg1mul(const std::vector<uint8_t>& point, const std::vector<uint8_t>& scalar, const std::vector<uint8_t>& res)
{
    bls_g1 r;
    const bls_g1* p = reinterpret_cast<const bls_g1*>(point.data());
    const bls_scalar* s = reinterpret_cast<const bls_scalar*>(scalar.data());
    bls_g1_mul(*p, *s, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g1)), "bls_g1_mul test failed");
}

void aggsigtest::testg2mul(const std::vector<uint8_t>& point, const std::vector<uint8_t>& scalar, const std::vector<uint8_t>& res)
{
    bls_g2 r;
    const bls_g2* p = reinterpret_cast<const bls_g2*>(point.data());
    const bls_scalar* s = reinterpret_cast<const bls_scalar*>(scalar.data());
    bls_g2_mul(*p, *s, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g2)), "bls_g2_mul test failed");
}

void aggsigtest::testg1exp(const std::vector<std::vector<uint8_t>>& points, const std::vector<std::vector<uint8_t>>& scalars, const std::vector<uint8_t>& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g1 r;
    const bls_g1* p = reinterpret_cast<const bls_g1*>(points.data()->data());
    const bls_scalar* s = reinterpret_cast<const bls_scalar*>(scalars.data()->data());
    bls_g1_exp(p, s, points.size(), r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g1)), "bls_g1_exp test failed");
}

void aggsigtest::testg2exp(const std::vector<std::vector<uint8_t>>& points, const std::vector<std::vector<uint8_t>>& scalars, const std::vector<uint8_t>& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g2 r;
    const bls_g2* p = reinterpret_cast<const bls_g2*>(points.data()->data());
    const bls_scalar* s = reinterpret_cast<const bls_scalar*>(scalars.data()->data());
    bls_g2_exp(p, s, points.size(), r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g2)), "bls_g2_exp test failed");
}

void aggsigtest::testpairing(const std::vector<std::vector<uint8_t>>& g1_points, const std::vector<std::vector<uint8_t>>& g2_points, const std::vector<uint8_t>& res)
{
    check(g1_points.size() == g2_points.size(), "number of elements in g1_points and g2_points must be equal");
    bls_gt r;
    const bls_g1* p1 = reinterpret_cast<const bls_g1*>(g1_points.data()->data());
    const bls_g2* p2 = reinterpret_cast<const bls_g2*>(g2_points.data()->data());
    bls_pairing(p1, p2, g1_points.size(), r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_gt)), "bls_pairing test failed");
}

void aggsigtest::testg1map(const std::vector<uint8_t>& e, const std::vector<uint8_t>& res)
{
    bls_g1 r;
    const bls_fp* element = reinterpret_cast<const bls_fp*>(e.data());
    bls_g1_map(*element, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g1)), "bls_g1_map test failed");
}

void aggsigtest::testg2map(const std::vector<uint8_t>& e, const std::vector<uint8_t>& res)
{
    bls_g2 r;
    const bls_fp2* element = reinterpret_cast<const bls_fp2*>(e.data());
    bls_g2_map(*element, r);
    check(0 == std::memcmp(r, res.data(), sizeof(bls_g2)), "bls_g2_map test failed");
}

// Construct an extensible-output function based on SHA256
void xmd_sh256(
    uint8_t *buf,
    int buf_len,
    const uint8_t *in,
    int in_len,
    const uint8_t *dst,
    int dst_len
)
{
    const unsigned int SHA256HashSize = 32;
    const unsigned int SHA256_Message_Block_Size = 64;
    const unsigned ell = (buf_len + SHA256HashSize - 1) / SHA256HashSize;
    if (buf_len < 0 || ell > 255 || dst_len > 255)
    {
        return;
    }
    const uint8_t Z_pad[SHA256_Message_Block_Size] = { 0, };
    const uint8_t l_i_b_0_str[] = {
        static_cast<uint8_t>(buf_len >> 8),
        static_cast<uint8_t>(buf_len & 0xff),
        0,
        static_cast<uint8_t>(dst_len)
    };
    const uint8_t *dstlen_str = l_i_b_0_str + 3;
    uint8_t b_0[SHA256HashSize];
    sha256 sha;
    sha.update(Z_pad, SHA256_Message_Block_Size);
    sha.update(in, in_len);
    sha.update(l_i_b_0_str, 3);
    sha.update(dst, dst_len);
    sha.update(dstlen_str, 1);
    sha.digest(b_0);
    uint8_t b_i[SHA256HashSize + 1] = { 0, };
    for (unsigned i = 1; i <= ell; ++i)
    {
        for (unsigned j = 0; j < SHA256HashSize; ++j)
        {
            b_i[j] = b_0[j] ^ b_i[j];
        }
        b_i[SHA256HashSize] = i;
        sha256 s;
        s.update(b_i, SHA256HashSize + 1);
        s.update(dst, dst_len);
        s.update(dstlen_str, 1);
        s.digest(b_i);
        const int rem_after = buf_len - i * SHA256HashSize;
        const int copy_len = SHA256HashSize + (rem_after < 0 ? rem_after : 0);
        memcpy(buf + (i - 1) * SHA256HashSize, b_i, copy_len);
    }
}

std::array<uint64_t, 8> scalar_fromBytesBE(const std::array<uint8_t, 8*8>& in)
{
    std::array<uint64_t, 8> out;
    for(uint64_t i = 0; i < 8; i++)
    {
        int64_t a = 8*8 - i*8;
        out[i] =
            static_cast<uint64_t>(in[a-1])       | static_cast<uint64_t>(in[a-2]) <<  8 |
            static_cast<uint64_t>(in[a-3]) << 16 | static_cast<uint64_t>(in[a-4]) << 24 |
            static_cast<uint64_t>(in[a-5]) << 32 | static_cast<uint64_t>(in[a-6]) << 40 |
            static_cast<uint64_t>(in[a-7]) << 48 | static_cast<uint64_t>(in[a-8]) << 56;
    }
    return out;
}

void g2_fromMessage(const std::vector<uint8_t>& msg, const string& dst, bls_g2& res)
{
    uint8_t buf[4 * 64];
    xmd_sh256(buf, 4 * 64, msg.data(), msg.size(), reinterpret_cast<const uint8_t*>(dst.c_str()), dst.length());

    array<uint64_t, 8> k;
    //std::vector<uint8_t> t;
    //t.resize(96);
    bls_fp t[2];
    //std::vector<uint8_t> p, q, res;
    //p.resize(288);
    //q.resize(288);
    //res.resize(288);
    bls_g2 p, q;

    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf));
    bls_fp_mod(reinterpret_cast<uint8_t*>(&k[0]), t[0]);
    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 64));
    bls_fp_mod(reinterpret_cast<uint8_t*>(&k[0]), t[1]);

    bls_g2_map(*reinterpret_cast<const bls_fp2*>(t), p);

    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 2*64));
    bls_fp_mod(reinterpret_cast<uint8_t*>(&k[0]), t[0]);
    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 3*64));
    bls_fp_mod(reinterpret_cast<uint8_t*>(&k[0]), t[1]);

    bls_g2_map(*reinterpret_cast<const bls_fp2*>(t), q);
    bls_g2_add(p, q, res);
}

const string CIPHERSUITE_ID = "BLS_SIG_BLS12381G2_XMD:SHA-256_SSWU_RO_NUL_";
const std::vector<uint8_t> G1_ONE_NEG = {0x16, 0x0c, 0x53, 0xfd, 0x90, 0x87, 0xb3, 0x5c, 0xf5, 0xff, 0x76, 0x99, 0x67, 0xfc, 0x17, 0x78, 0xc1, 0xa1, 0x3b, 0x14, 0xc7, 0x95, 0x4f, 0x15, 0x47, 0xe7, 0xd0, 0xf3, 0xcd, 0x6a, 0xae, 0xf0, 0x40, 0xf4, 0xdb, 0x21, 0xcc, 0x6e, 0xce, 0xed, 0x75, 0xfb, 0x0b, 0x9e, 0x41, 0x77, 0x01, 0x12, 0x3a, 0x88, 0x18, 0xf3, 0x2a, 0x6c, 0x52, 0xff, 0x70, 0x02, 0x3b, 0x38, 0xe4, 0x9c, 0x89, 0x92, 0x55, 0xd0, 0xa9, 0x9f, 0x8d, 0x73, 0xd7, 0x89, 0x2a, 0xc1, 0x44, 0xa3, 0x5b, 0xf3, 0xca, 0x12, 0x17, 0x53, 0x4b, 0x96, 0x76, 0x1b, 0xff, 0x3c, 0x30, 0x44, 0x77, 0xe9, 0xed, 0xd2, 0x44, 0x0e, 0xfd, 0xff, 0x02, 0x00, 0x00, 0x00, 0x09, 0x76, 0x02, 0x00, 0x0c, 0xc4, 0x0b, 0x00, 0xf4, 0xeb, 0xba, 0x58, 0xc7, 0x53, 0x57, 0x98, 0x48, 0x5f, 0x45, 0x57, 0x52, 0x70, 0x53, 0x58, 0xce, 0x77, 0x6d, 0xec, 0x56, 0xa2, 0x97, 0x1a, 0x07, 0x5c, 0x93, 0xe4, 0x80, 0xfa, 0xc3, 0x5e, 0xf6, 0x15};
const std::vector<uint8_t> GT_ONE = {0xfd, 0xff, 0x02, 0x00, 0x00, 0x00, 0x09, 0x76, 0x02, 0x00, 0x0c, 0xc4, 0x0b, 0x00, 0xf4, 0xeb, 0xba, 0x58, 0xc7, 0x53, 0x57, 0x98, 0x48, 0x5f, 0x45, 0x57, 0x52, 0x70, 0x53, 0x58, 0xce, 0x77, 0x6d, 0xec, 0x56, 0xa2, 0x97, 0x1a, 0x07, 0x5c, 0x93, 0xe4, 0x80, 0xfa, 0xc3, 0x5e, 0xf6, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::vector<uint8_t> msg = {51, 23, 56, 93, 212, 129, 128, 27, 251, 12, 42, 129, 210, 9, 34, 98};

void aggsigtest::verify(const std::vector<uint8_t>& pk, const std::vector<uint8_t>& sig)
{
    bls_g1 g1_points[2];
    bls_g2 g2_points[2];

    memcpy(&g1_points[0], G1_ONE_NEG.data(), sizeof(bls_g1));
    memcpy(&g2_points[0], sig.data(), sizeof(bls_g2));

    bls_g2 p_msg;
    g2_fromMessage(msg, CIPHERSUITE_ID, p_msg);
    memcpy(&g1_points[1], pk.data(), sizeof(bls_g1));
    memcpy(&g2_points[1], p_msg, sizeof(bls_g2));

    bls_gt r;
    bls_pairing(g1_points, g2_points, 2, r);
    check(0 == std::memcmp(r, GT_ONE.data(), sizeof(bls_gt)), "bls signature verify failed");
}

void aggsigtest::testgarbage()
{
    bls_g1 valid_g1 = {0x90, 0xac, 0xe8, 0xf5, 0x20, 0xa5, 0xe0, 0x56, 0xea, 0xb0, 0x99, 0xd4, 0xe6, 0xa6, 0xf7, 0x61, 0xa8, 0xf5, 0x13, 0x01, 0xe1, 0xe1, 0x59, 0x10, 0x90, 0x84, 0x53, 0x00, 0x7d, 0x94, 0xe3, 0x3f, 0x8e, 0x86, 0xb8, 0x34, 0x80, 0x8d, 0xcb, 0x53, 0x23, 0x31, 0x31, 0x53, 0xbe, 0x06, 0x6d, 0x01, 0xd4, 0x96, 0x60, 0xf5, 0x94, 0xa8, 0x05, 0xad, 0x9a, 0x42, 0x49, 0xd0, 0xaf, 0x41, 0x90, 0x40, 0x7e, 0x3e, 0x22, 0x1e, 0x1d, 0x6f, 0xda, 0xd8, 0x66, 0xe0, 0x22, 0x30, 0x83, 0x57, 0x77, 0x6e, 0xd5, 0xe3, 0x96, 0x77, 0x7c, 0x7f, 0x7d, 0xfd, 0x02, 0x0e, 0x7d, 0x73, 0x65, 0x11, 0xac, 0x06, 0xfd, 0xff, 0x02, 0x00, 0x00, 0x00, 0x09, 0x76, 0x02, 0x00, 0x0c, 0xc4, 0x0b, 0x00, 0xf4, 0xeb, 0xba, 0x58, 0xc7, 0x53, 0x57, 0x98, 0x48, 0x5f, 0x45, 0x57, 0x52, 0x70, 0x53, 0x58, 0xce, 0x77, 0x6d, 0xec, 0x56, 0xa2, 0x97, 0x1a, 0x07, 0x5c, 0x93, 0xe4, 0x80, 0xfa, 0xc3, 0x5e, 0xf6, 0x15};
    bls_g2 valid_g2 = {0x3c, 0x38, 0xa5, 0x9a, 0x6c, 0x01, 0x6f, 0xb5, 0x98, 0xd8, 0xda, 0x53, 0x1e, 0x20, 0x5e, 0x4d, 0x8e, 0x09, 0x53, 0x86, 0x86, 0x75, 0x76, 0x0f, 0x83, 0x33, 0xc7, 0xfe, 0xe7, 0x25, 0xd8, 0x1d, 0xdf, 0x24, 0x6d, 0xb7, 0x57, 0x92, 0xeb, 0x7c, 0xf0, 0xab, 0x8a, 0xeb, 0xa0, 0x51, 0x80, 0x0b, 0xab, 0xf4, 0x07, 0xdd, 0xd8, 0xec, 0x72, 0x1d, 0xb8, 0x3f, 0xe1, 0xb1, 0x09, 0x8b, 0x93, 0x34, 0xbf, 0x45, 0xad, 0x63, 0xf1, 0x0c, 0xf5, 0xa6, 0xb5, 0x6b, 0x60, 0xf7, 0x4b, 0x39, 0x16, 0x15, 0xd7, 0x37, 0x30, 0x88, 0xd1, 0x82, 0x71, 0x13, 0x58, 0xa6, 0x03, 0x2f, 0x8c, 0x31, 0xfe, 0x16, 0x94, 0xff, 0x3f, 0x32, 0x54, 0xa7, 0xe0, 0x2e, 0x3e, 0x28, 0x11, 0xce, 0x0b, 0xec, 0xde, 0xc6, 0x03, 0x1b, 0x19, 0x3b, 0x55, 0x41, 0xd5, 0x77, 0xb4, 0x9c, 0xdb, 0x6c, 0xf0, 0x9f, 0x91, 0x99, 0x17, 0xb0, 0x97, 0x76, 0xb1, 0xf0, 0x5c, 0x97, 0x7f, 0xfa, 0x17, 0xc2, 0xde, 0x87, 0x62, 0x05, 0xf4, 0x02, 0xf7, 0xe4, 0xfb, 0x00, 0x89, 0x6c, 0xb9, 0xb3, 0xdf, 0x1a, 0x25, 0xf7, 0x61, 0x23, 0x9e, 0x56, 0x25, 0xdd, 0xaa, 0xb4, 0xbf, 0xac, 0xce, 0x8e, 0x6c, 0x50, 0x5c, 0x13, 0xeb, 0x73, 0x63, 0x2b, 0x12, 0xce, 0xdd, 0x30, 0xb0, 0x80, 0x4f, 0xe2, 0x9f, 0x31, 0x70, 0x15, 0xc3, 0x07, 0xec, 0x2d, 0xfe, 0x48, 0xcb, 0x50, 0x60, 0xd8, 0x4f, 0xd2, 0xc2, 0x75, 0x6f, 0x41, 0xae, 0x79, 0xf4, 0xc0, 0xa3, 0xa3, 0x7b, 0xa6, 0x2b, 0x22, 0x87, 0x7b, 0x23, 0x04, 0x2b, 0xb3, 0x0a, 0x06, 0x3d, 0x4a, 0xd3, 0x25, 0x5d, 0xe8, 0x44, 0x54, 0x5e, 0x89, 0xf0, 0x88, 0x60, 0x41, 0xb5, 0x13, 0x41, 0x0d, 0xf5, 0x2d, 0x38, 0x0f, 0xac, 0x23, 0xc3, 0x12, 0x77, 0x99, 0x73, 0x81, 0x11, 0x56, 0x51, 0xc0, 0x20, 0x14, 0xa9, 0xae, 0xea, 0xae, 0xc8, 0x5c, 0x34, 0xb8, 0x1a, 0x93, 0xee, 0x73, 0xcd, 0x71, 0x2c, 0xe2, 0x41, 0xf3, 0x94, 0xdc, 0x86, 0x0e, 0xa5, 0xe7, 0x92, 0xb0, 0xf5, 0x0e};
    bls_g1 invalid_g1 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    bls_g2 invalid_g2 = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    bls_gt res;
    check(0 == bls_pairing(&valid_g1, &valid_g2, 1, res), "valid g1 & valid g2");
    check(-1 == bls_pairing(&invalid_g1, &valid_g2, 1, res), "invalid g1 & valid g2");
    check(-1 == bls_pairing(&valid_g1, &invalid_g2, 1, res), "valid g1 & invalid g2");
    check(-1 == bls_pairing(&invalid_g1, &invalid_g2, 1, res), "invalid g1 & invalid g2");

    // valid fp (MODULUS - 1) & fp2 (coefficient B)
    uint64_t MODULUSM1[6] = {0xb9fe'ffff'ffff'aaaa, 0x1eab'fffe'b153'ffff, 0x6730'd2a0'f6b0'f624, 0x6477'4b84'f385'12bf, 0x4b1b'a7b6'434b'acd7, 0x1a01'11ea'397f'e69a};
    uint64_t B[12] = {0xaa27'0000'000c'fff3, 0x53cc'0032'fc34'000a, 0x478f'e97a'6b0a'807f, 0xb1d3'7ebe'e6ba'24d7, 0x8ec9'733b'bf78'ab2f, 0x09d6'4551'3d83'de7e, 0xaa27'0000'000c'fff3, 0x53cc'0032'fc34'000a, 0x478f'e97a'6b0a'807f, 0xb1d3'7ebe'e6ba'24d7, 0x8ec9'733b'bf78'ab2f, 0x09d6'4551'3d83'de7e};
    uint64_t invalid_fp[6] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    uint64_t invalid_fp2[12] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

    bls_g1 res_g1;
    bls_g2 res_g2;
    check(0 == bls_g1_map(*reinterpret_cast<bls_fp*>(&MODULUSM1), res_g1), "valid g1 map");
    check(0 == bls_g2_map(*reinterpret_cast<bls_fp2*>(&B), res_g2), "valid g2 map");
    check(-1 == bls_g1_map(*reinterpret_cast<bls_fp*>(&invalid_fp), res_g1), "invalid g1 map");
    check(-1 == bls_g2_map(*reinterpret_cast<bls_fp2*>(&invalid_fp2), res_g2), "invalid g2 map");
}

void aggsigtest::blake3wasm()
{
    uint8_t output[BLAKE3_OUT_LEN];
    for(int i = 0; i < 10000; i++)
    {
        msg[i%msg.size()] = i%256;
        // Initialize the hasher.
        blake3_hasher hasher;
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, msg.data(), msg.size());
        // Finalize the hash. BLAKE3_OUT_LEN is the default output length, 32 bytes.
        blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);
        check(!(output[0] == output[1] && output[1] == output[2] && output[2] == output[3] && output[3] == output[4]), "error");
    }
    // print digest as hex string
    uint64_t N = BLAKE3_OUT_LEN;
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string s(2 + N * 2, ' ');
    s[0] = '0';
    s[1] = 'x';
    for(uint64_t i = 0; i < N; i++)
    {
        s[2 + 2*i]     = hexmap[(output[i] & 0xF0) >> 4];
        s[2 + 2*i+1]   = hexmap[ output[i] & 0x0F      ];
    }
    // s == "0x8f5c7c8a3f7700d8d9f3fc0add1e95f17fcb1f258a4a520e6b1fd58b8af0f55e"
    //check(output[0] == 0x8f, s);
}

void aggsigtest::blake2swasm()
{
    uint8_t output[32];
    for(int i = 0; i < 10000; i++)
    {
        //msg[i%msg.size()] = i%256;
        Blake2sContext hasher;
        blake2sInit(&hasher, NULL, 0, 32);
        //blake2sUpdate(&hasher, msg.data(), msg.size());
        blake2sUpdate(&hasher, NULL, 0);
        blake2sFinal(&hasher, output);
        check(!(output[0] == output[1] && output[1] == output[2] && output[2] == output[3] && output[3] == output[4]), "error");
    }
    // print digest as hex string
    uint64_t N = 32;
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string s(2 + N * 2, ' ');
    s[0] = '0';
    s[1] = 'x';
    for(uint64_t i = 0; i < N; i++)
    {
        s[2 + 2*i]     = hexmap[(output[i] & 0xF0) >> 4];
        s[2 + 2*i+1]   = hexmap[ output[i] & 0x0F      ];
    }
    // s == "0x59252351fde518bf747b24cb4e73a6abf30ea6c9998626ebc4bf795c452a2122"
    // s == "0x69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9" // zero input, zero key
    check(output[0] == 0x69, s);
}
