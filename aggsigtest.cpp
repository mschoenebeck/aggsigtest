#include "aggsigtest.hpp"
#include "sha256.hpp"

using namespace bls12_381;

void aggsigtest::testg1add(const bls_g1& op1, const bls_g1& op2, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_add(op1, op2, r);
    check(r == res, "bls_g1_add test failed");
}

void aggsigtest::testg2add(const bls_g2& op1, const bls_g2& op2, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_add(op1, op2, r);
    check(r == res, "bls_g2_add test failed");
}

void aggsigtest::testg1mul(const bls_g1& point, const bls_scalar& scalar, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_mul(point, scalar, r);
    check(r == res, "bls_g1_mul test failed");
}

void aggsigtest::testg2mul(const bls_g2& point, const bls_scalar& scalar, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_mul(point, scalar, r);
    /*
    uint64_t N = 144;
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string s(2 + N * 2, ' ');
    s[0] = '0';
    s[1] = 'x';
    for(uint64_t i = 0; i < N; i++)
    {
        s[2 + 2*i]     = hexmap[(r[i] & 0xF0) >> 4];
        s[2 + 2*i+1]   = hexmap[ r[i] & 0x0F      ];
    }
    */
    check(r == res, "bls_g2_mul test failed");
}

void aggsigtest::testg1exp(const std::vector<bls_g1>& points, const std::vector<bls_scalar>& scalars, const bls_g1& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g1 r;
    bls_g1_exp(points, scalars, r);
    check(r == res, "bls_g1_exp test failed");
}

void aggsigtest::testg2exp(const std::vector<bls_g2>& points, const std::vector<bls_scalar>& scalars, const bls_g2& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g2 r;
    bls_g2_exp(points, scalars, r);
    check(r == res, "bls_g2_exp test failed");
}

void aggsigtest::testpairing(const std::vector<bls_g1>& g1_points, const std::vector<bls_g2>& g2_points, const bls_gt& res)
{
    check(g1_points.size() == g2_points.size(), "number of elements in g1_points and g2_points must be equal");
    bls_gt r;
    bls_pairing(g1_points, g2_points, r);
    check(r == res, "bls_pairing test failed");
}

void aggsigtest::testg1map(const bls_fp& e, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_map(e, r);
    check(r == res, "bls_g1_map test failed");
}

void aggsigtest::testg2map(const bls_fp2& e, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_map(e, r);
    check(r == res, "bls_g2_map test failed");
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

bls_g2 g2_fromMessage(const std::vector<uint8_t>& msg, const string& dst)
{
    uint8_t buf[4 * 64];
    xmd_sh256(buf, 4 * 64, msg.data(), msg.size(), reinterpret_cast<const uint8_t*>(dst.c_str()), dst.length());

    array<uint64_t, 8> k;
    bls_fp2 t;
    bls_g2 p, q, res;

    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf));
    bls_fp_mod(*reinterpret_cast<array<uint8_t, 64>*>(&k[0]), *reinterpret_cast<bls_fp*>(&t[0]));
    //t.c0 = fp::modPrime(k);
    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 64));
    bls_fp_mod(*reinterpret_cast<array<uint8_t, 64>*>(&k[0]), *reinterpret_cast<bls_fp*>(&t[48]));
    //t.c1 = fp::modPrime(k);

    bls_g2_map(t, p);

    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 2*64));
    bls_fp_mod(*reinterpret_cast<array<uint8_t, 64>*>(&k[0]), *reinterpret_cast<bls_fp*>(&t[0]));
    //t.c0 = fp::modPrime(k);
    k = scalar_fromBytesBE(*reinterpret_cast<array<uint8_t, 64>*>(buf + 3*64));
    bls_fp_mod(*reinterpret_cast<array<uint8_t, 64>*>(&k[0]), *reinterpret_cast<bls_fp*>(&t[48]));
    //t.c1 = fp::modPrime(k);

    bls_g2_map(t, q);
    bls_g2_add(p, q, res);

    return res;
}

const string CIPHERSUITE_ID = "BLS_SIG_BLS12381G2_XMD:SHA-256_SSWU_RO_NUL_";
const bls_g1 G1_ONE_NEG = {0x16, 0x0c, 0x53, 0xfd, 0x90, 0x87, 0xb3, 0x5c, 0xf5, 0xff, 0x76, 0x99, 0x67, 0xfc, 0x17, 0x78, 0xc1, 0xa1, 0x3b, 0x14, 0xc7, 0x95, 0x4f, 0x15, 0x47, 0xe7, 0xd0, 0xf3, 0xcd, 0x6a, 0xae, 0xf0, 0x40, 0xf4, 0xdb, 0x21, 0xcc, 0x6e, 0xce, 0xed, 0x75, 0xfb, 0x0b, 0x9e, 0x41, 0x77, 0x01, 0x12, 0x3a, 0x88, 0x18, 0xf3, 0x2a, 0x6c, 0x52, 0xff, 0x70, 0x02, 0x3b, 0x38, 0xe4, 0x9c, 0x89, 0x92, 0x55, 0xd0, 0xa9, 0x9f, 0x8d, 0x73, 0xd7, 0x89, 0x2a, 0xc1, 0x44, 0xa3, 0x5b, 0xf3, 0xca, 0x12, 0x17, 0x53, 0x4b, 0x96, 0x76, 0x1b, 0xff, 0x3c, 0x30, 0x44, 0x77, 0xe9, 0xed, 0xd2, 0x44, 0x0e, 0xfd, 0xff, 0x02, 0x00, 0x00, 0x00, 0x09, 0x76, 0x02, 0x00, 0x0c, 0xc4, 0x0b, 0x00, 0xf4, 0xeb, 0xba, 0x58, 0xc7, 0x53, 0x57, 0x98, 0x48, 0x5f, 0x45, 0x57, 0x52, 0x70, 0x53, 0x58, 0xce, 0x77, 0x6d, 0xec, 0x56, 0xa2, 0x97, 0x1a, 0x07, 0x5c, 0x93, 0xe4, 0x80, 0xfa, 0xc3, 0x5e, 0xf6, 0x15};
const bls_gt GT_ONE = {0xfd, 0xff, 0x02, 0x00, 0x00, 0x00, 0x09, 0x76, 0x02, 0x00, 0x0c, 0xc4, 0x0b, 0x00, 0xf4, 0xeb, 0xba, 0x58, 0xc7, 0x53, 0x57, 0x98, 0x48, 0x5f, 0x45, 0x57, 0x52, 0x70, 0x53, 0x58, 0xce, 0x77, 0x6d, 0xec, 0x56, 0xa2, 0x97, 0x1a, 0x07, 0x5c, 0x93, 0xe4, 0x80, 0xfa, 0xc3, 0x5e, 0xf6, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::vector<uint8_t> msg = {51, 23, 56, 93, 212, 129, 128, 27, 251, 12, 42, 129, 210, 9, 34, 98};

void aggsigtest::verify(const bls_g1& pk, const bls_g2& sig)
{
    std::vector<bls_g1> g1_points;
    std::vector<bls_g2> g2_points;

    g1_points.push_back(G1_ONE_NEG);
    g2_points.push_back(sig);

    g1_points.push_back(pk);
    g2_points.push_back(g2_fromMessage(msg, CIPHERSUITE_ID));

    bls_gt r;
    bls_pairing(g1_points, g2_points, r);
    check(r == GT_ONE, "bls signature verify failed");
}
